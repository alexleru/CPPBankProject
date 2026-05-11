#!/usr/bin/env bash
# Build libage_verifier.so on Linux.
# Run from the project root: ./native/linux/build.sh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NATIVE_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
SRC="${NATIVE_ROOT}/src/age_verifier.cpp"
OUT="${SCRIPT_DIR}/libage_verifier.so"

g++ -std=c++03 -O2 -Wall -fPIC -shared -o "${OUT}" "${SRC}"
echo "Built ${OUT}"
