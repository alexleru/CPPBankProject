#include "../include/Constants.h"
#include "../include/Globals.h"
#include "../include/Utils.h"
#include "../include/BondCalculator.h"

// SCC A
#include "../include/Account.h"
#include "../include/Transaction.h"

// SCC B
#include "../include/Deposit.h"
#include "../include/Withdrawal.h"
#include "../include/Transfer.h"
#include "../include/LoanPayment.h"
#include "../include/LoggingVisitor.h"

// SCC C
#include "../include/Bank.h"
#include "../include/Customer.h"
#include "../include/Loan.h"
#include "../include/AuditLogger.h"
#include "../include/NotificationCenter.h"
#include "../include/BranchManager.h"
#include "../include/RiskAnalyzer.h"

// SCC D
#include "../include/ReportEngine.h"
#include "../include/ReportFilter.h"
#include "../include/ReportFormatter.h"
#include "../include/ReportWriter.h"

// Native age-verification library wrapper (outside the SCC graph).
#include "../include/AgeVerifier.h"

#include <iostream>
#include <limits>
#include <stdexcept>

static void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// -------------------------------------------------------------------------
// Scenario 1 — exercises the mega-SCC (Account/Transaction + Bank/Customer/
// Loan/AuditLogger/NotificationCenter/BranchManager/RiskAnalyzer) plus
// SCC B (Visitor) via LoggingVisitor.
// -------------------------------------------------------------------------
static void runBankFlow() {
    std::cout << "\n=== NEW BANK FLOW (SCC A+B+C scenario) ===\n\n";

    Bank* bank = new Bank("Demo Bank");

    AuditLogger* logger = new AuditLogger();
    NotificationCenter* nc = new NotificationCenter(logger);
    bank->setAuditLogger(logger);
    bank->setNotificationCenter(nc);

    RiskAnalyzer* ra = new RiskAnalyzer(bank, logger);
    BranchManager* mgr = new BranchManager(bank, ra);
    bank->addBranchManager(mgr);

    Customer* alice = new Customer("C0001", "Alice", "Smith", bank);
    nc->subscribe(alice);
    bank->registerCustomer(alice);
    mgr->assignCustomer(alice);

    Customer* bob = new Customer("C0002", "Bob", "Jones", bank);
    nc->subscribe(bob);
    bank->registerCustomer(bob);
    mgr->assignCustomer(bob);

    Account* aliceChk = new Account("A1001", 0.0, NULL, NULL, NULL);
    alice->addAccount(aliceChk);

    Account* aliceSav = new Account("A1002", 0.0, NULL, NULL, NULL);
    alice->addAccount(aliceSav);

    Account* bobChk = new Account("A2001", 0.0, NULL, NULL, NULL);
    bob->addAccount(bobChk);

    LoggingVisitor visitor(std::cout);

    std::cout << "\n-- Transactions --\n";

    Deposit* d1 = new Deposit(aliceChk, 1000.0);
    aliceChk->addTransaction(d1);
    d1->apply();
    d1->accept(visitor);

    Deposit* d2 = new Deposit(bobChk, 500.0);
    bobChk->addTransaction(d2);
    d2->apply();
    d2->accept(visitor);

    Transfer* t1 = new Transfer(aliceChk, aliceSav, 250.0);
    aliceChk->addTransaction(t1);
    t1->apply();
    t1->accept(visitor);

    Withdrawal* w1 = new Withdrawal(bobChk, 100.0);
    bobChk->addTransaction(w1);
    w1->apply();
    w1->accept(visitor);

    // Loan path exercises BranchManager -> RiskAnalyzer -> AuditLogger ->
    // NotificationCenter inside the mega-SCC.
    std::cout << "\n-- Loan flow --\n";
    Loan* loan = new Loan("L0001", alice, aliceChk, ra, 5000.0);
    alice->addLoan(loan);
    mgr->approveLoan(loan, logger);

    LoanPayment* lp = new LoanPayment(aliceChk, loan, 200.0);
    aliceChk->addTransaction(lp);
    lp->apply();
    lp->accept(visitor);

    std::cout << "\n-- Final balances --\n";
    std::cout << "  " << aliceChk->getId() << " (Alice/Chk) = "
              << Utils::formatCurrency(aliceChk->getBalance()) << "\n";
    std::cout << "  " << aliceSav->getId() << " (Alice/Sav) = "
              << Utils::formatCurrency(aliceSav->getBalance()) << "\n";
    std::cout << "  " << bobChk->getId()   << " (Bob/Chk)   = "
              << Utils::formatCurrency(bobChk->getBalance())   << "\n";
    std::cout << "  Loan " << loan->getId() << " outstanding = "
              << Utils::formatCurrency(loan->getOutstanding()) << "\n";

    std::cout << "\n-- Audit log --\n";
    logger->dump(std::cout);

    delete bank;  // cascades through all owned objects
    std::cout << "\n(Bank dismantled cleanly.)\n";
}

// -------------------------------------------------------------------------
// Scenario 2 — SCC D (Reporting pipeline). Fully isolated from SCC A/B/C.
// -------------------------------------------------------------------------
static void runReportFlow() {
    std::cout << "\n=== GENERATE REPORT (SCC D scenario) ===\n";

    ReportEngine* engine = new ReportEngine("2026-01-01 .. 2026-03-31");
    ReportFilter* filter = new ReportFilter(engine);
    engine->setFilter(filter);

    ReportFormatter* formatter = new ReportFormatter("plain-text");
    ReportWriter*    writer    = new ReportWriter(engine, formatter);
    engine->setWriter(writer);

    engine->generate(std::cout);

    delete engine;  // cascades: deletes filter and writer; writer deletes formatter
    std::cout << "\n(Report engine torn down.)\n";
}

// -------------------------------------------------------------------------
// Scenario 3 — Acyclic BondCalculator (kept from previous demo, untouched).
// -------------------------------------------------------------------------
static void runBondCalc() {
    std::cout << "\n=== CALCULATE BOND PARAMETERS ===\n\n";
    double nominal      = Utils::getValidatedAmount("Enter bond nominal: ");
    int    term         = Utils::getValidatedInteger("Enter bond term (months): ");
    double fixedRatePct = Utils::getValidatedAmount("Enter fixed monthly coupon rate (%): ");
    double maxRandPct   = Utils::getValidatedAmount("Enter max random monthly part (%, 0..3): ");
    double discRatePct  = Utils::getValidatedAmount("Enter annual discount rate (%): ");
    if (maxRandPct > 3.0) maxRandPct = 3.0;

    try {
        BondCalculator calc(nominal, term,
                            fixedRatePct / 100.0,
                            maxRandPct   / 100.0,
                            discRatePct  / 100.0);
        calc.generateCashFlowSchedule();
        std::cout << "PV: " << Utils::formatCurrency(calc.calculatePresentValue()) << "\n";
        std::cout << "FV: " << Utils::formatCurrency(calc.calculateFutureValue())  << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

// -------------------------------------------------------------------------
// Scenario 4 — Native library demo. AgeVerifier dynamically loads the
// platform-appropriate age_verifier.{dll,so} via LoadLibrary / dlopen and
// calls the C-ABI verify_age_21 entry point. Outside the SCC graph.
// -------------------------------------------------------------------------
static void runAgeVerification() {
    std::cout << "\n=== VERIFY AGE (21+) -- NATIVE LIBRARY ===\n\n";

    AgeVerifier verifier;
    if (!verifier.isLoaded()) {
        std::cerr << "Failed to load native library ("
                  << verifier.libraryPath() << "): "
                  << verifier.lastError() << "\n";
        return;
    }
    std::cout << "Loaded: " << verifier.libraryPath() << "\n\n";

    int day   = Utils::getValidatedInteger("Enter day of birth (1-31): ");
    int month = Utils::getValidatedInteger("Enter month of birth (1-12): ");
    int year  = Utils::getValidatedInteger("Enter year of birth (1900-9999): ");

    AgeVerifier::Result r = verifier.verify(day, month, year);
    switch (r) {
        case AgeVerifier::AGE_OK:
            std::cout << "Result: OK -- subject is 21 or older.\n";
            break;
        case AgeVerifier::AGE_UNDER:
            std::cout << "Result: UNDER -- subject is younger than 21.\n";
            break;
        case AgeVerifier::AGE_BAD_INPUT:
            std::cout << "Result: BAD INPUT -- not a real calendar date.\n";
            break;
        case AgeVerifier::AGE_LIB_ERROR:
            std::cout << "Result: LIB ERROR -- " << verifier.lastError() << "\n";
            break;
    }
}

static void displayMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "   " << BANK_NAME << " — SCC DEMO\n";
    std::cout << "=========================================\n";
    std::cout << "1. Run new-bank flow (SCC A+B+C scenario)\n";
    std::cout << "2. Generate report (SCC D scenario)\n";
    std::cout << "3. Calculate bond parameters (acyclic baseline)\n";
    std::cout << "4. Verify age 21+ (native library demo)\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    initializeGlobals();
    std::cout << BANK_NAME << " v" << VERSION
              << " — SCC demo (" << PLATFORM_NAME << ")\n";

    int choice = -1;
    while (true) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input.\n";
            continue;
        }
        if (choice == 0) break;
        switch (choice) {
            case 1: runBankFlow();         pauseScreen(); break;
            case 2: runReportFlow();       pauseScreen(); break;
            case 3: runBondCalc();         pauseScreen(); break;
            case 4: runAgeVerification();  pauseScreen(); break;
            default: std::cerr << "Invalid choice.\n";
        }
    }
    std::cout << "Goodbye.\n";
    return 0;
}
