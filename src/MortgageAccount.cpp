#include "../include/MortgageAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// =====================================================================
// MortgageAccount.cpp
// ---------------------------------------------------------------------
// Implementation of the mortgage feature.  Notice how the typedefs
// declared in the header are used heavily through this file - that is
// the whole point of the exercise.
// =====================================================================

// PMI threshold: anything above 80% LTV requires PMI.
static const Percentage  PMI_LTV_THRESHOLD     = 0.80;
static const Percentage  PMI_ANNUAL_RATE       = 0.0075;  // 0.75% per year of loan balance
static const Percentage  DEFAULT_PROPERTY_TAX  = 0.012;   // 1.2% of market value, annually
static const Percentage  DEFAULT_INSURANCE     = 0.0035;  // 0.35% of market value, annually
static const TermInMonths MONTHS_PER_YEAR      = 12;

// ---------------------------------------------------------------------
// Static helpers
// ---------------------------------------------------------------------
Money MortgageAccount::computeLoanPrincipal(const Property& prop) {
    Money principal = prop.marketValue - prop.downPayment;
    if (principal < 0) principal = 0;
    return principal;
}

Percentage MortgageAccount::computeLTV(const Property& prop) {
    if (prop.marketValue <= 0.0) return 0.0;
    return (prop.marketValue - prop.downPayment) / prop.marketValue;
}

const char* MortgageAccount::mortgageKindToString(MortgageKind kind) {
    switch (kind) {
        case FIXED_RATE:      return "Fixed Rate";
        case ADJUSTABLE_RATE: return "Adjustable Rate (ARM)";
        case INTEREST_ONLY:   return "Interest Only";
    }
    return "Unknown";
}

// ---------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------
MortgageAccount::MortgageAccount(const Property&  prop,
                                 Rate             annualRate,
                                 TermInYears      years,
                                 MortgageKind     kind)
    : LoanAccount(computeLoanPrincipal(prop),
                  annualRate,
                  years * MONTHS_PER_YEAR),
      property(prop),
      mortgageKind(kind),
      termYears(years),
      baseRate(annualRate),
      propertyTaxRate(DEFAULT_PROPERTY_TAX),
      insuranceRate(DEFAULT_INSURANCE),
      pmiRequired(false)
{
    if (prop.marketValue <= 0.0) {
        throw std::invalid_argument("Property market value must be positive");
    }
    if (prop.downPayment < 0.0) {
        throw std::invalid_argument("Down payment cannot be negative");
    }
    if (prop.downPayment > prop.marketValue) {
        throw std::invalid_argument("Down payment cannot exceed market value");
    }
    if (years <= 0) {
        throw std::invalid_argument("Mortgage term (years) must be positive");
    }

    recomputePmiFlag();
    buildAmortizationSchedule();
}

MortgageAccount::~MortgageAccount() {
    // schedule and balanceByMonth are STL containers; auto-cleaned.
}

// ---------------------------------------------------------------------
// Account / LoanAccount overrides
// ---------------------------------------------------------------------
AccountType MortgageAccount::getAccountType() const {
    // Re-uses the LOAN enum value because the AccountRegistry, the
    // generic reporting code and the Java port all treat a mortgage
    // as a flavour of loan.  This is the "join with loan account"
    // requirement: same enum, same abstract contract.
    return LOAN;
}

void MortgageAccount::applyMonthlyProcessing() {
    if (!isActive) return;

    // Delegate the loan-status update logic to LoanAccount.
    LoanAccount::applyMonthlyProcessing();

    // Mortgage-specific monthly hook: re-evaluate PMI requirement,
    // because as principal is paid down the LTV drops and PMI can
    // be removed once below 80%.
    recomputePmiFlag();
}

void MortgageAccount::displayAccountInfo() const {
    LoanAccount::displayAccountInfo();
    std::cout << "--- Mortgage details ---\n";
    std::cout << "Property ID       : " << property.id << "\n";
    std::cout << "Property Address  : " << property.address << "\n";
    std::cout << "Market Value      : " << Utils::formatCurrency(property.marketValue) << "\n";
    std::cout << "Down Payment      : " << Utils::formatCurrency(property.downPayment) << "\n";
    std::cout << "LTV               : " << (calculateLTV() * 100.0) << " %\n";
    std::cout << "Mortgage Kind     : " << mortgageKindToString(mortgageKind) << "\n";
    std::cout << "Term              : " << termYears << " years ("
              << (termYears * MONTHS_PER_YEAR) << " months)\n";
    std::cout << "Base Rate         : " << (baseRate * 100.0) << " % per annum\n";
    std::cout << "PMI Required      : " << (pmiRequired ? "Yes" : "No") << "\n";
    std::cout << "Monthly Payment   : " << Utils::formatCurrency(calculateTotalMonthlyPayment())
              << "  (P+I+Tax+Ins"
              << (pmiRequired ? "+PMI" : "")
              << ")\n";
}

// ---------------------------------------------------------------------
// Schedule
// ---------------------------------------------------------------------
void MortgageAccount::buildAmortizationSchedule() {
    schedule.clear();
    balanceByMonth.clear();

    const Money        principal      = computeLoanPrincipal(property);
    const TermInMonths totalMonths    = termYears * MONTHS_PER_YEAR;
    const Rate         monthlyRate    = baseRate / static_cast<Rate>(MONTHS_PER_YEAR);
    const Money        baseMonthlyPI  = LoanAccount::calculateEMI(principal, baseRate, totalMonths);
    const Money        escrowMonthly  = calculateEscrowMonthly();

    Money remaining = principal;

    for (MonthIndex m = 1; m <= totalMonths; ++m) {
        Installment row;
        row.month         = m;
        row.interestPart  = remaining * monthlyRate;
        row.principalPart = baseMonthlyPI - row.interestPart;

        // Last-month rounding correction
        if (m == totalMonths) {
            row.principalPart = remaining;
        }

        // Interest-only schedules don't pay down principal until the
        // last instalment (balloon).
        if (mortgageKind == INTEREST_ONLY && m < totalMonths) {
            row.principalPart = 0.0;
        }
        if (mortgageKind == INTEREST_ONLY && m == totalMonths) {
            row.principalPart = remaining;
        }

        row.escrowPart = escrowMonthly;

        // PMI is charged monthly while LTV > threshold.
        Money pmi = 0.0;
        if (property.marketValue > 0.0) {
            Percentage currentLTV = remaining / property.marketValue;
            if (currentLTV > PMI_LTV_THRESHOLD) {
                pmi = (remaining * PMI_ANNUAL_RATE) / static_cast<Rate>(MONTHS_PER_YEAR);
            }
        }
        row.pmiPart = pmi;

        row.totalPayment = row.principalPart
                         + row.interestPart
                         + row.escrowPart
                         + row.pmiPart;

        remaining -= row.principalPart;
        if (remaining < 0.0) remaining = 0.0;
        row.remainingBalance = remaining;

        schedule.push_back(row);
        balanceByMonth[m] = remaining;
    }
}

const AmortizationSchedule& MortgageAccount::getAmortizationSchedule() const {
    return schedule;
}

const BalanceTimeline& MortgageAccount::getBalanceTimeline() const {
    return balanceByMonth;
}

void MortgageAccount::displayAmortizationSchedule() const {
    std::cout << "\n===============================================\n";
    std::cout << "MORTGAGE AMORTIZATION SCHEDULE\n";
    std::cout << "Property : " << property.address << "\n";
    std::cout << "Principal: " << Utils::formatCurrency(computeLoanPrincipal(property))
              << "  Term: " << termYears << "y  Rate: " << (baseRate * 100.0) << "%\n";
    std::cout << "Kind     : " << mortgageKindToString(mortgageKind) << "\n";
    std::cout << "===============================================\n";

    std::cout << std::left
              << std::setw(6)  << "Mo"
              << std::setw(14) << "Principal"
              << std::setw(14) << "Interest"
              << std::setw(12) << "Escrow"
              << std::setw(10) << "PMI"
              << std::setw(14) << "Total"
              << std::setw(14) << "Balance"
              << "\n";
    std::cout << "-----------------------------------------------------------------------\n";

    // Iterator typedef in action.
    for (ScheduleConstIterator it = schedule.begin(); it != schedule.end(); ++it) {
        const Installment& row = *it;
        std::cout << std::left
                  << std::setw(6)  << row.month
                  << std::setw(14) << Utils::formatCurrency(row.principalPart)
                  << std::setw(14) << Utils::formatCurrency(row.interestPart)
                  << std::setw(12) << Utils::formatCurrency(row.escrowPart)
                  << std::setw(10) << Utils::formatCurrency(row.pmiPart)
                  << std::setw(14) << Utils::formatCurrency(row.totalPayment)
                  << std::setw(14) << Utils::formatCurrency(row.remainingBalance)
                  << "\n";
    }
    std::cout << "===============================================\n\n";
}

// ---------------------------------------------------------------------
// Payments
// ---------------------------------------------------------------------
bool MortgageAccount::makeMortgagePayment(Money amount, const std::string& description) {
    // Re-uses LoanAccount's EMI machinery so the loan side stays in
    // sync (monthsPaid, schedule index, status updates).  This is
    // exactly the "join with loan account" goal.
    bool ok = LoanAccount::makeEMIPayment(amount, description);
    if (ok) {
        recomputePmiFlag();
    }
    return ok;
}

// ---------------------------------------------------------------------
// Calculations
// ---------------------------------------------------------------------
Percentage MortgageAccount::calculateLTV() const {
    Money remaining = calculateRemainingBalance();
    if (property.marketValue <= 0.0) return 0.0;
    return remaining / property.marketValue;
}

Money MortgageAccount::calculateMonthlyPayment() const {
    Money principal = computeLoanPrincipal(property);
    TermInMonths months = termYears * MONTHS_PER_YEAR;
    return LoanAccount::calculateEMI(principal, baseRate, months);
}

Money MortgageAccount::calculateEscrowMonthly() const {
    Money annual = property.marketValue * (propertyTaxRate + insuranceRate);
    return annual / static_cast<Rate>(MONTHS_PER_YEAR);
}

Money MortgageAccount::calculatePmiMonthly() const {
    if (!pmiRequired) return 0.0;
    Money remaining = calculateRemainingBalance();
    return (remaining * PMI_ANNUAL_RATE) / static_cast<Rate>(MONTHS_PER_YEAR);
}

Money MortgageAccount::calculateTotalMonthlyPayment() const {
    return calculateMonthlyPayment()
         + calculateEscrowMonthly()
         + calculatePmiMonthly();
}

// ---------------------------------------------------------------------
// Strategy-via-function-pointer hook
// ---------------------------------------------------------------------
Money MortgageAccount::applyMortgageRule(MortgageRule rule) {
    if (rule == NULL) {
        throw std::invalid_argument("MortgageAccount::applyMortgageRule: null rule");
    }
    Money current = calculateRemainingBalance();
    return rule(current);
}

// ---------------------------------------------------------------------
// Accessors
// ---------------------------------------------------------------------
const Property&  MortgageAccount::getProperty()      const { return property; }
MortgageKind     MortgageAccount::getMortgageKind()  const { return mortgageKind; }
TermInYears      MortgageAccount::getTermYears()     const { return termYears; }
Rate             MortgageAccount::getBaseRate()      const { return baseRate; }
bool             MortgageAccount::isPmiRequired()    const { return pmiRequired; }

// ---------------------------------------------------------------------
// Internal
// ---------------------------------------------------------------------
void MortgageAccount::recomputePmiFlag() {
    pmiRequired = (calculateLTV() > PMI_LTV_THRESHOLD);
}
