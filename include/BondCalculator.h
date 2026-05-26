#ifndef BONDCALCULATOR_H
#define BONDCALCULATOR_H

#include <string>
#include <vector>

// Maximum allowed random (floating) part of the monthly coupon rate.
// The problem statement says the random component can be up to 3%.
#define BOND_MAX_RANDOM_RATE 0.03

// Default annual discount rate used for Present / Future Value
// calculations if the caller does not supply one.
#define BOND_DEFAULT_DISCOUNT_RATE 0.05

// Structure representing a single monthly cash flow row.
// All rates are expressed per month (not per annum).
struct BondCashFlow {
    int month;                      // 1..termMonths
    double fixedCouponRate;         // fixed monthly rate (decimal)
    double randomCouponRate;        // random monthly rate (decimal, 0..maxRandomRate)
    double totalCouponRate;         // fixed + random
    double couponPayment;           // nominal * totalCouponRate
    double cumulativeCoupon;        // running total of coupons received so far
    double discountedCoupon;        // PV of this coupon at discountRate
    double futureValueContribution; // FV of this coupon reinvested to maturity
};

// C-style "typedef struct" alias idiom — exposes BondCashFlow under a
// second name without C++11 `using`. The _t suffix is a common
// convention in code that originated from a C codebase.
typedef struct BondCashFlow BondCashFlow_t;

// BondCalculator - computes bond valuation and parameters.
//
// Inputs:
//   * nominal            - face value of the bond (principal repaid at maturity)
//   * termMonths         - total life of the bond in months
//   * fixedMonthlyRate   - deterministic part of the monthly coupon rate
//   * maxRandomRate      - upper bound for the random part (0..maxRandomRate)
//   * annualDiscountRate - market / required rate of return for PV / FV
//
// The calculator produces:
//   * a full month-by-month cash flow schedule
//   * Present Value of all cash flows (coupons + face value)
//   * Future Value of all cash flows (coupons reinvested to maturity)
//   * descriptive statistics of the random coupon stream
//   * Macaulay duration and a simple sensitivity analysis
class BondCalculator {
private:
    double nominal;
    int termMonths;
    double fixedMonthlyRate;
    double maxRandomRate;
    double annualDiscountRate;
    unsigned int randomSeed;
    bool scheduleGenerated;
    std::vector<BondCashFlow> schedule;

public:
    // Constructors
    BondCalculator(double nominal,
                   int termMonths,
                   double fixedMonthlyRate,
                   double maxRandomRate = BOND_MAX_RANDOM_RATE,
                   double annualDiscountRate = BOND_DEFAULT_DISCOUNT_RATE);

    ~BondCalculator();

    // Core operations
    void generateCashFlowSchedule();
    void generateCashFlowSchedule(unsigned int seed);

    // Primary calculations (require an existing schedule)
    double calculatePresentValue() const;
    double calculateFutureValue() const;
    double calculateTotalCouponIncome() const;
    double calculateAverageCouponRate() const;
    double calculateMacaulayDuration() const;

    // Display / reporting
    void displayCashFlowSchedule() const;
    void generateBondAnalysisReport() const; // 150+ line comprehensive report

    // Getters
    double getNominal() const;
    int getTermMonths() const;
    double getFixedMonthlyRate() const;
    double getMaxRandomRate() const;
    double getAnnualDiscountRate() const;
    unsigned int getRandomSeed() const;
    const std::vector<BondCashFlow>& getSchedule() const;

    // Static helpers exposed for reuse / testing
    static double presentValueOfCashFlow(double amount,
                                         int periodMonth,
                                         double annualDiscountRate);
    static double futureValueOfCashFlow(double amount,
                                        int periodMonth,
                                        int totalMonths,
                                        double annualDiscountRate);

private:
    // Generates a random rate in [0, maxRandomRate]. Uses std::rand internally
    // so the project stays on the C++03 standard already used by the build.
    double generateRandomRate() const;

    // Throws std::runtime_error if no schedule exists yet.
    void requireSchedule() const;
};

#endif // BONDCALCULATOR_H
