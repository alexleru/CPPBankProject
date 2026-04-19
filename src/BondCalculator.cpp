#include "../include/BondCalculator.h"
#include "../include/Utils.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// ---------------------------------------------------------------------------
// Constructor / destructor
// ---------------------------------------------------------------------------
BondCalculator::BondCalculator(double nominal,
                               int termMonths,
                               double fixedMonthlyRate,
                               double maxRandomRate,
                               double annualDiscountRate)
    : nominal(nominal),
      termMonths(termMonths),
      fixedMonthlyRate(fixedMonthlyRate),
      maxRandomRate(maxRandomRate),
      annualDiscountRate(annualDiscountRate),
      randomSeed(0),
      scheduleGenerated(false) {

    if (nominal <= 0.0) {
        throw std::invalid_argument("Bond nominal must be positive");
    }
    if (termMonths <= 0) {
        throw std::invalid_argument("Bond term (months) must be positive");
    }
    if (fixedMonthlyRate < 0.0) {
        throw std::invalid_argument("Fixed monthly coupon rate cannot be negative");
    }
    if (maxRandomRate < 0.0 || maxRandomRate > BOND_MAX_RANDOM_RATE) {
        throw std::invalid_argument("Random coupon part must be in [0, 3%]");
    }
    if (annualDiscountRate < 0.0) {
        throw std::invalid_argument("Discount rate cannot be negative");
    }
}

BondCalculator::~BondCalculator() {
}

// ---------------------------------------------------------------------------
// Random helpers
// ---------------------------------------------------------------------------
double BondCalculator::generateRandomRate() const {
    // Uniformly distributed value in [0, maxRandomRate]
    double r = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    return r * maxRandomRate;
}

void BondCalculator::requireSchedule() const {
    if (!scheduleGenerated || schedule.empty()) {
        throw std::runtime_error(
            "Cash flow schedule has not been generated yet. "
            "Call generateCashFlowSchedule() first.");
    }
}

// ---------------------------------------------------------------------------
// Schedule generation
// ---------------------------------------------------------------------------
void BondCalculator::generateCashFlowSchedule() {
    // Use a time-based seed so successive calls produce different streams.
    unsigned int seed = static_cast<unsigned int>(std::time(NULL));
    generateCashFlowSchedule(seed);
}

void BondCalculator::generateCashFlowSchedule(unsigned int seed) {
    randomSeed = seed;
    std::srand(randomSeed);

    schedule.clear();
    schedule.reserve(static_cast<size_t>(termMonths));

    double runningTotal = 0.0;

    for (int m = 1; m <= termMonths; ++m) {
        BondCashFlow cf;
        cf.month = m;
        cf.fixedCouponRate = fixedMonthlyRate;
        cf.randomCouponRate = generateRandomRate();
        cf.totalCouponRate = cf.fixedCouponRate + cf.randomCouponRate;
        cf.couponPayment = nominal * cf.totalCouponRate;

        runningTotal += cf.couponPayment;
        cf.cumulativeCoupon = runningTotal;

        cf.discountedCoupon = presentValueOfCashFlow(
            cf.couponPayment, m, annualDiscountRate);
        cf.futureValueContribution = futureValueOfCashFlow(
            cf.couponPayment, m, termMonths, annualDiscountRate);

        schedule.push_back(cf);
    }

    scheduleGenerated = true;
}

// ---------------------------------------------------------------------------
// Static time-value-of-money helpers
// ---------------------------------------------------------------------------
double BondCalculator::presentValueOfCashFlow(double amount,
                                              int periodMonth,
                                              double annualDiscountRate) {
    double monthlyRate = annualDiscountRate / 12.0;
    return amount / std::pow(1.0 + monthlyRate, static_cast<double>(periodMonth));
}

double BondCalculator::futureValueOfCashFlow(double amount,
                                             int periodMonth,
                                             int totalMonths,
                                             double annualDiscountRate) {
    double monthlyRate = annualDiscountRate / 12.0;
    int remaining = totalMonths - periodMonth;
    if (remaining < 0) {
        remaining = 0;
    }
    return amount * std::pow(1.0 + monthlyRate, static_cast<double>(remaining));
}

// ---------------------------------------------------------------------------
// Primary calculations
// ---------------------------------------------------------------------------
double BondCalculator::calculatePresentValue() const {
    requireSchedule();

    double pvCoupons = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        pvCoupons += schedule[i].discountedCoupon;
    }

    // Face value repaid at maturity, discounted back to today.
    double pvFace = presentValueOfCashFlow(nominal, termMonths, annualDiscountRate);
    return pvCoupons + pvFace;
}

double BondCalculator::calculateFutureValue() const {
    requireSchedule();

    double fvCoupons = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        fvCoupons += schedule[i].futureValueContribution;
    }
    // Face value paid at maturity is not compounded.
    return fvCoupons + nominal;
}

double BondCalculator::calculateTotalCouponIncome() const {
    requireSchedule();
    return schedule.back().cumulativeCoupon;
}

double BondCalculator::calculateAverageCouponRate() const {
    requireSchedule();
    double sum = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        sum += schedule[i].totalCouponRate;
    }
    return sum / static_cast<double>(schedule.size());
}

double BondCalculator::calculateMacaulayDuration() const {
    requireSchedule();

    double weightedSum = 0.0;
    double pvSum = 0.0;

    for (size_t i = 0; i < schedule.size(); ++i) {
        double pv = schedule[i].discountedCoupon;
        weightedSum += static_cast<double>(schedule[i].month) * pv;
        pvSum += pv;
    }

    double pvFace = presentValueOfCashFlow(nominal, termMonths, annualDiscountRate);
    weightedSum += static_cast<double>(termMonths) * pvFace;
    pvSum += pvFace;

    if (pvSum <= 0.0) {
        return 0.0;
    }
    // Duration in months; divide by 12 to express in years.
    return (weightedSum / pvSum) / 12.0;
}

// ---------------------------------------------------------------------------
// Cash flow schedule dump (short helper, not the main report)
// ---------------------------------------------------------------------------
void BondCalculator::displayCashFlowSchedule() const {
    requireSchedule();

    std::cout << "\nMonthly cash flow schedule:\n";
    std::cout << std::left
              << std::setw(8)  << "Month"
              << std::setw(14) << "Fixed %"
              << std::setw(14) << "Random %"
              << std::setw(14) << "Total %"
              << std::setw(16) << "Coupon"
              << std::setw(16) << "PV Coupon"
              << std::setw(16) << "FV Coupon"
              << std::endl;
    std::cout << std::string(98, '-') << std::endl;

    for (size_t i = 0; i < schedule.size(); ++i) {
        const BondCashFlow& cf = schedule[i];
        std::cout << std::left
                  << std::setw(8)  << cf.month
                  << std::setw(14) << (cf.fixedCouponRate * 100.0)
                  << std::setw(14) << (cf.randomCouponRate * 100.0)
                  << std::setw(14) << (cf.totalCouponRate * 100.0)
                  << std::setw(16) << Utils::formatCurrency(cf.couponPayment)
                  << std::setw(16) << Utils::formatCurrency(cf.discountedCoupon)
                  << std::setw(16) << Utils::formatCurrency(cf.futureValueContribution)
                  << std::endl;
    }
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------
double BondCalculator::getNominal() const { return nominal; }
int BondCalculator::getTermMonths() const { return termMonths; }
double BondCalculator::getFixedMonthlyRate() const { return fixedMonthlyRate; }
double BondCalculator::getMaxRandomRate() const { return maxRandomRate; }
double BondCalculator::getAnnualDiscountRate() const { return annualDiscountRate; }
unsigned int BondCalculator::getRandomSeed() const { return randomSeed; }
const std::vector<BondCashFlow>& BondCalculator::getSchedule() const { return schedule; }

// ===========================================================================
//                   COMPREHENSIVE BOND ANALYSIS REPORT
// ---------------------------------------------------------------------------
// This is intentionally a single, large function (> 150 LOC) that produces the
// complete human-readable analysis of the bond. It is the main "showcase"
// routine used from the UI layer and by the modernization comparison work.
// ===========================================================================
void BondCalculator::generateBondAnalysisReport() const {
    requireSchedule();

    // --- 1. Header -------------------------------------------------------
    std::cout << "\n";
    std::cout << "==============================================================\n";
    std::cout << "                  BOND VALUATION ANALYSIS REPORT              \n";
    std::cout << "==============================================================\n";
    std::cout << "Report timestamp : " << Utils::formatDate(std::time(NULL)) << "\n";
    std::cout << "Random seed used : " << randomSeed << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 2. Bond terms ---------------------------------------------------
    std::cout << "BOND TERMS\n";
    std::cout << "  Nominal (face value)      : " << Utils::formatCurrency(nominal) << "\n";
    std::cout << "  Term                      : " << termMonths
              << " months (" << std::fixed << std::setprecision(2)
              << (termMonths / 12.0) << " years)\n";
    std::cout << "  Fixed monthly coupon rate : " << std::setprecision(4)
              << (fixedMonthlyRate * 100.0) << " %\n";
    std::cout << "  Random monthly bound      : 0.0000 % ... "
              << (maxRandomRate * 100.0) << " %\n";
    std::cout << "  Annual discount rate      : "
              << (annualDiscountRate * 100.0) << " %\n";
    std::cout << "  Monthly discount rate     : "
              << ((annualDiscountRate / 12.0) * 100.0) << " %\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 3. Descriptive statistics on the coupon stream -----------------
    double sumCoupon = 0.0;
    double sumRandom = 0.0;
    double minCoupon = schedule[0].couponPayment;
    double maxCoupon = schedule[0].couponPayment;
    double minRandomRate = schedule[0].randomCouponRate;
    double maxRandomRateObserved = schedule[0].randomCouponRate;

    for (size_t i = 0; i < schedule.size(); ++i) {
        double c = schedule[i].couponPayment;
        double r = schedule[i].randomCouponRate;
        sumCoupon += c;
        sumRandom += r;
        if (c < minCoupon) minCoupon = c;
        if (c > maxCoupon) maxCoupon = c;
        if (r < minRandomRate) minRandomRate = r;
        if (r > maxRandomRateObserved) maxRandomRateObserved = r;
    }
    double avgCoupon = sumCoupon / static_cast<double>(schedule.size());
    double avgRandomRate = sumRandom / static_cast<double>(schedule.size());

    // Standard deviation of coupon amount (population std dev).
    double sqErr = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        double diff = schedule[i].couponPayment - avgCoupon;
        sqErr += diff * diff;
    }
    double stdDev = std::sqrt(sqErr / static_cast<double>(schedule.size()));

    std::cout << "COUPON STREAM STATISTICS\n";
    std::cout << "  Periods                   : " << schedule.size() << "\n";
    std::cout << "  Min coupon payment        : " << Utils::formatCurrency(minCoupon) << "\n";
    std::cout << "  Max coupon payment        : " << Utils::formatCurrency(maxCoupon) << "\n";
    std::cout << "  Avg coupon payment        : " << Utils::formatCurrency(avgCoupon) << "\n";
    std::cout << "  Std dev of coupon         : " << Utils::formatCurrency(stdDev) << "\n";
    std::cout << "  Avg random part of rate   : " << std::setprecision(4)
              << (avgRandomRate * 100.0) << " %\n";
    std::cout << "  Observed random range     : "
              << (minRandomRate * 100.0) << " % ... "
              << (maxRandomRateObserved * 100.0) << " %\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 4. Full monthly schedule ---------------------------------------
    std::cout << "MONTHLY CASH FLOW SCHEDULE\n";
    std::cout << std::left
              << std::setw(6)  << "M"
              << std::setw(11) << "Fixed%"
              << std::setw(11) << "Rand%"
              << std::setw(11) << "Total%"
              << std::setw(14) << "Coupon"
              << std::setw(14) << "CumCoupon"
              << std::setw(14) << "PV"
              << std::setw(14) << "FV"
              << "\n";
    std::cout << std::string(95, '-') << "\n";

    for (size_t i = 0; i < schedule.size(); ++i) {
        const BondCashFlow& cf = schedule[i];
        std::cout << std::left
                  << std::setw(6)  << cf.month
                  << std::setw(11) << std::setprecision(4) << (cf.fixedCouponRate * 100.0)
                  << std::setw(11) << (cf.randomCouponRate * 100.0)
                  << std::setw(11) << (cf.totalCouponRate * 100.0)
                  << std::setw(14) << Utils::formatCurrency(cf.couponPayment)
                  << std::setw(14) << Utils::formatCurrency(cf.cumulativeCoupon)
                  << std::setw(14) << Utils::formatCurrency(cf.discountedCoupon)
                  << std::setw(14) << Utils::formatCurrency(cf.futureValueContribution)
                  << "\n";
    }
    std::cout << "--------------------------------------------------------------\n";

    // --- 5. Present Value breakdown -------------------------------------
    double pvCoupons = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        pvCoupons += schedule[i].discountedCoupon;
    }
    double pvFace = presentValueOfCashFlow(nominal, termMonths, annualDiscountRate);
    double pvBond = pvCoupons + pvFace;

    std::cout << "PRESENT VALUE BREAKDOWN\n";
    std::cout << "  PV of coupon stream       : " << Utils::formatCurrency(pvCoupons) << "\n";
    std::cout << "  PV of nominal at maturity : " << Utils::formatCurrency(pvFace) << "\n";
    std::cout << "  --------------------------  \n";
    std::cout << "  PRESENT VALUE             : " << Utils::formatCurrency(pvBond) << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 6. Future Value breakdown --------------------------------------
    double fvCoupons = 0.0;
    for (size_t i = 0; i < schedule.size(); ++i) {
        fvCoupons += schedule[i].futureValueContribution;
    }
    double fvBond = fvCoupons + nominal;

    std::cout << "FUTURE VALUE BREAKDOWN (reinvesting coupons at discount rate)\n";
    std::cout << "  FV of coupon stream       : " << Utils::formatCurrency(fvCoupons) << "\n";
    std::cout << "  Nominal repaid at maturity: " << Utils::formatCurrency(nominal) << "\n";
    std::cout << "  --------------------------  \n";
    std::cout << "  FUTURE VALUE              : " << Utils::formatCurrency(fvBond) << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 7. Yield / return metrics --------------------------------------
    double totalCoupons = schedule.back().cumulativeCoupon;
    double totalPayout = totalCoupons + nominal;
    double totalReturn = (pvBond > 0.0)
                            ? (totalPayout - pvBond) / pvBond
                            : 0.0;
    double annualizedReturn = (termMonths > 0)
                            ? std::pow(totalPayout / std::max(pvBond, 1e-9),
                                       12.0 / static_cast<double>(termMonths)) - 1.0
                            : 0.0;
    double duration = calculateMacaulayDuration();

    std::cout << "YIELD AND RISK METRICS\n";
    std::cout << "  Total coupon income       : " << Utils::formatCurrency(totalCoupons) << "\n";
    std::cout << "  Total payout (coupons+par): " << Utils::formatCurrency(totalPayout) << "\n";
    std::cout << "  Absolute profit vs PV     : "
              << Utils::formatCurrency(totalPayout - pvBond) << "\n";
    std::cout << "  Total return over life    : " << std::setprecision(4)
              << (totalReturn * 100.0) << " %\n";
    std::cout << "  Annualized return         : "
              << (annualizedReturn * 100.0) << " %\n";
    std::cout << "  Macaulay duration (years) : " << duration << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // --- 8. Discount rate sensitivity -----------------------------------
    std::cout << "DISCOUNT RATE SENSITIVITY (Present Value)\n";
    std::cout << std::left
              << std::setw(16) << "Annual rate"
              << std::setw(18) << "PV of coupons"
              << std::setw(18) << "PV of face"
              << std::setw(18) << "PV of bond"
              << "\n";
    std::cout << std::string(70, '-') << "\n";

    double sensitivityRates[5];
    sensitivityRates[0] = std::max(0.0, annualDiscountRate - 0.02);
    sensitivityRates[1] = std::max(0.0, annualDiscountRate - 0.01);
    sensitivityRates[2] = annualDiscountRate;
    sensitivityRates[3] = annualDiscountRate + 0.01;
    sensitivityRates[4] = annualDiscountRate + 0.02;

    for (int s = 0; s < 5; ++s) {
        double ar = sensitivityRates[s];
        double cpv = 0.0;
        for (size_t i = 0; i < schedule.size(); ++i) {
            cpv += presentValueOfCashFlow(schedule[i].couponPayment,
                                          schedule[i].month, ar);
        }
        double fpv = presentValueOfCashFlow(nominal, termMonths, ar);
        std::cout << std::left
                  << std::setw(16) << ((ar * 100.0))
                  << std::setw(18) << Utils::formatCurrency(cpv)
                  << std::setw(18) << Utils::formatCurrency(fpv)
                  << std::setw(18) << Utils::formatCurrency(cpv + fpv)
                  << "\n";
    }
    std::cout << "--------------------------------------------------------------\n";

    // --- 9. Verdict -----------------------------------------------------
    std::cout << "INVESTMENT VERDICT\n";
    if (pvBond >= nominal) {
        std::cout << "  The bond is priced at a PREMIUM relative to its nominal.\n"
                  << "  Investors effectively pay more than par because the\n"
                  << "  coupon flow exceeds the discount rate requirement.\n";
    } else {
        std::cout << "  The bond is priced at a DISCOUNT relative to its nominal.\n"
                  << "  The market / discount rate demands a higher yield than\n"
                  << "  the coupon stream provides, so PV falls below par.\n";
    }
    std::cout << "==============================================================\n\n";
}
