#ifndef MORTGAGEACCOUNT_H
#define MORTGAGEACCOUNT_H

#include "LoanAccount.h"
#include <string>
#include <vector>
#include <map>

// =====================================================================
// MortgageAccount
// ---------------------------------------------------------------------
// A mortgage is modelled as a specialised long-term loan, so this class
// inherits from LoanAccount.  It adds property data (address, market
// value, down payment) and the mortgage-specific concepts of LTV
// (loan-to-value) and PMI (private mortgage insurance).
//
// The whole module is written with as many `typedef` aliases as is
// reasonable, because the project is being used to study C++03 -> Java
// modernisation, and `typedef` is the only alias mechanism available
// in C++03 (no `using` aliases).  Each typedef below documents what it
// will become in Java.
// =====================================================================

// ---------------------------------------------------------------------
// Domain primitive aliases (semantic typedefs over basic types)
// ---------------------------------------------------------------------
typedef double        Money;            // -> java BigDecimal / double
typedef double        Percentage;       // 0.045 == 4.5%   -> double
typedef double        Rate;             // annual rate as fraction
typedef int           TermInMonths;     // -> int
typedef int           TermInYears;      // -> int
typedef int           MonthIndex;       // 1..N            -> int
typedef unsigned long PropertyId;       // -> long
typedef std::string   PropertyAddress;  // -> String
typedef std::string   AccountIdRef;     // -> String

// ---------------------------------------------------------------------
// Mortgage-specific enum (kept out of Enums.h on purpose so this
// feature is self-contained, just like LoanAccount keeps EMIPayment
// inside its own header).
// ---------------------------------------------------------------------
enum MortgageType {
    FIXED_RATE,        // classic 15/20/30-year fixed
    ADJUSTABLE_RATE,   // ARM
    INTEREST_ONLY      // interest-only schedule
};

// Typedef for the enum itself - matches the project's C++03 style of
// aliasing enum types to make signatures more self-describing.
typedef enum MortgageType MortgageKind;

// ---------------------------------------------------------------------
// Property descriptor structure + typedef alias.
// In C++03 it is idiomatic to write `typedef struct { ... } Name;`
// but we use the cleaner two-line form so the struct also has a
// regular C++ name.
// ---------------------------------------------------------------------
struct PropertyInfo {
    PropertyId      id;
    PropertyAddress address;
    Money           marketValue;
    Money           downPayment;
};
typedef struct PropertyInfo PropertyInfo_t;
typedef PropertyInfo_t      Property;     // friendlier domain name

// ---------------------------------------------------------------------
// Mortgage installment record (one row of the amortisation schedule).
// It mirrors EMIPayment but adds escrow + insurance breakdown.
// ---------------------------------------------------------------------
struct MortgageInstallment {
    MonthIndex month;
    Money      principalPart;
    Money      interestPart;
    Money      escrowPart;       // taxes + property insurance escrow
    Money      pmiPart;          // private mortgage insurance
    Money      totalPayment;
    Money      remainingBalance;
};
typedef struct MortgageInstallment MortgageInstallment_t;
typedef MortgageInstallment_t      Installment;

// ---------------------------------------------------------------------
// Container aliases (the bread-and-butter use case for typedef in
// C++03; they hide the > > template-closing pain and rename verbose
// STL types into something domain-meaningful).
// ---------------------------------------------------------------------
typedef std::vector<Installment>            AmortizationSchedule;
typedef AmortizationSchedule::iterator      ScheduleIterator;
typedef AmortizationSchedule::const_iterator ScheduleConstIterator;

typedef std::map<MonthIndex, Money>         BalanceTimeline;
typedef BalanceTimeline::const_iterator     BalanceTimelineIt;

// ---------------------------------------------------------------------
// Function-pointer typedef (one of the most useful uses of typedef in
// C++03 - raw function-pointer syntax is unreadable otherwise).
// Maps to java.util.function.DoubleUnaryOperator.
// ---------------------------------------------------------------------
typedef Money (*MortgageRule)(Money currentBalance);

// =====================================================================
// MortgageAccount class
// =====================================================================
class MortgageAccount : public LoanAccount {
public:
    // Self-typedef of the class - a pattern often seen in legacy
    // C++03 code to expose the class name as a member type, similar
    // to how STL containers expose `value_type`.
    typedef MortgageAccount self_type;

private:
    Property              property;
    MortgageKind          mortgageKind;
    TermInYears           termYears;
    Rate                  baseRate;             // annual, before adjustments
    Percentage            propertyTaxRate;      // annual, of marketValue
    Percentage            insuranceRate;        // annual, of marketValue
    bool                  pmiRequired;          // true if LTV > 0.80
    AmortizationSchedule  schedule;             // typedef-aliased vector
    BalanceTimeline       balanceByMonth;       // typedef-aliased map

public:
    // -----------------------------------------------------------------
    // Constructor
    //   loanPrincipal = marketValue - downPayment (the actual borrowed
    //   amount that LoanAccount tracks as `loanAmount`).
    // -----------------------------------------------------------------
    MortgageAccount(const Property&  prop,
                    Rate             annualRate,
                    TermInYears      years,
                    MortgageKind     kind = FIXED_RATE);

    // Destructor
    ~MortgageAccount();

    // -----------------------------------------------------------------
    // Account / LoanAccount overrides
    // -----------------------------------------------------------------
    AccountType getAccountType() const;
    void        applyMonthlyProcessing();
    void        displayAccountInfo() const;

    // -----------------------------------------------------------------
    // Mortgage-specific behaviour
    // -----------------------------------------------------------------
    void                        buildAmortizationSchedule();
    const AmortizationSchedule& getAmortizationSchedule() const;
    const BalanceTimeline&      getBalanceTimeline() const;
    void                        displayAmortizationSchedule() const;
    bool                        makeMortgagePayment(Money amount,
                                                    const std::string& description = "Mortgage Payment");

    // Calculations
    Percentage  calculateLTV() const;             // loan-to-value ratio
    Money       calculateMonthlyPayment() const;  // P+I only
    Money       calculateEscrowMonthly() const;   // taxes+insurance
    Money       calculatePmiMonthly() const;      // 0 if pmiRequired==false
    Money       calculateTotalMonthlyPayment() const; // P+I+T+I+PMI

    // Strategy-via-function-pointer hook (mirrors Bank::applyToAllAccounts)
    Money       applyMortgageRule(MortgageRule rule);

    // Accessors (use the typedefs in their signatures!)
    const Property&  getProperty() const;
    MortgageKind     getMortgageKind() const;
    TermInYears      getTermYears() const;
    Rate             getBaseRate() const;
    bool             isPmiRequired() const;

    // Static helpers
    static Money       computeLoanPrincipal(const Property& prop);
    static Percentage  computeLTV(const Property& prop);
    static const char* mortgageKindToString(MortgageKind kind);

private:
    void recomputePmiFlag();
};

#endif // MORTGAGEACCOUNT_H
