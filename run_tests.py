#!/usr/bin/env python3
"""
Bank System test harness.

Reads docs/TC_SPEC.md, runs each test case as a separate invocation of the
BankSystem binary, and writes a timestamped log under logs/.

Usage:
    python run_tests.py

Prerequisites:
    Build the binary first:
        make                # Linux / macOS
        mingw32-make        # Windows (cmd.exe with MinGW)

Exit code:
    0  all cases pass
    1  one or more cases fail (or the binary is missing)
"""

import datetime
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SPEC_PATH = ROOT / "docs" / "TC_SPEC.md"
LOG_DIR = ROOT / "logs"
BINARY = ROOT / ("BankSystem.exe" if os.name == "nt" else "BankSystem")
TIMEOUT_S = 10


def parse_spec(text):
    """Parse TC_SPEC.md into a list of (tc_name, commands, expected) tuples."""
    cases = []
    tc_name = None
    section = None          # 'commands' | 'expected' | None
    in_code = False
    cmd_buf, exp_buf = [], []

    def flush():
        if tc_name is not None and (cmd_buf or exp_buf):
            cases.append((tc_name, "\n".join(cmd_buf), "\n".join(exp_buf)))

    for line in text.splitlines():
        stripped = line.strip()
        if not in_code and stripped.startswith("## ") and not stripped.startswith("### "):
            flush()
            tc_name = stripped[3:].strip()
            section = None
            cmd_buf, exp_buf = [], []
        elif not in_code and stripped.startswith("### "):
            label = stripped[4:].strip().lower()
            if "command" in label:
                section = "commands"
            elif "expected" in label:
                section = "expected"
            else:
                section = None
        elif stripped.startswith("```"):
            in_code = not in_code
        elif in_code and tc_name is not None and section == "commands":
            cmd_buf.append(line)
        elif in_code and tc_name is not None and section == "expected":
            exp_buf.append(line)
    flush()
    return [c for c in cases if c[0].startswith("TC-")]


def run_case(commands):
    proc = subprocess.run(
        [str(BINARY)],
        input=commands,
        capture_output=True,
        text=True,
        encoding="utf-8",
        errors="replace",
        timeout=TIMEOUT_S,
    )
    return proc.stdout, proc.stderr, proc.returncode


def evaluate(expected, actual):
    missing = []
    for line in expected.splitlines():
        line = line.strip()
        if line and line not in actual:
            missing.append(line)
    return missing


def main():
    if not BINARY.exists():
        print("ERROR: binary not found at", BINARY)
        print("Build it first: make   (or: mingw32-make on Windows)")
        return 1

    if not SPEC_PATH.exists():
        print("ERROR: spec not found at", SPEC_PATH)
        return 1

    cases = parse_spec(SPEC_PATH.read_text(encoding="utf-8"))
    if not cases:
        print("ERROR: no test cases found in", SPEC_PATH)
        return 1

    LOG_DIR.mkdir(exist_ok=True)
    stamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    log_path = LOG_DIR / ("test_run_" + stamp + ".log")

    passed, failed = 0, 0
    with log_path.open("w", encoding="utf-8") as log:
        log.write("Bank System Test Run\n")
        log.write("Timestamp : " + stamp + "\n")
        log.write("Binary    : " + str(BINARY) + "\n")
        log.write("Spec      : " + str(SPEC_PATH) + "\n")
        log.write("Total     : " + str(len(cases)) + "\n")
        log.write("=" * 70 + "\n\n")

        for tc_name, commands, expected in cases:
            try:
                stdout, stderr, rc = run_case(commands)
            except subprocess.TimeoutExpired:
                status = "FAIL"
                print("[" + status + "] " + tc_name + "  (timeout)")
                log.write("[" + status + "] " + tc_name + "  (timeout)\n\n")
                failed += 1
                continue

            missing = evaluate(expected, stdout)
            ok = (len(missing) == 0)
            status = "PASS" if ok else "FAIL"
            print("[" + status + "] " + tc_name)

            log.write("[" + status + "] " + tc_name + "\n")
            log.write("  exit code : " + str(rc) + "\n")
            if missing:
                log.write("  missing expected substrings:\n")
                for m in missing:
                    log.write("    - " + m + "\n")
            log.write("  stdin:\n")
            for line in commands.splitlines():
                log.write("    > " + line + "\n")
            log.write("  stdout:\n")
            for line in stdout.splitlines():
                log.write("    | " + line + "\n")
            if stderr:
                log.write("  stderr:\n")
                for line in stderr.splitlines():
                    log.write("    ! " + line + "\n")
            log.write("\n")

            if ok:
                passed += 1
            else:
                failed += 1

        summary = "Summary: " + str(passed) + " passed, " + str(failed) + " failed, " + str(len(cases)) + " total"
        log.write("=" * 70 + "\n")
        log.write(summary + "\n")

    print("")
    print("Summary: " + str(passed) + " passed, " + str(failed) + " failed, " + str(len(cases)) + " total")
    print("Log    : " + str(log_path))
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
