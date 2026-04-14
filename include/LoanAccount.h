#ifndef LOANACCOUNT_H
#define LOANACCOUNT_H

#include "Account.h"
#include <vector>

// Structure to represent an EMI payment
struct EMIPayment {
    int month;
    double principalPayment;
    double interestPayment;
    double totalPayment;
    double balance;
};

// LoanAccount class - inherits from Account
class LoanAccount : public Account {
private:
    double loanAmount;
    double loanInterestRate;
    int loanTerm; // in months
    int monthsPaid;
    LoanStatus loanStatus;
    std::vector<EMIPayment> repaymentSchedule;

public:
    // Constructor
    LoanAccount(double principal, double rate = 0.08, int termMonths = 12);

    // Destructor
    ~LoanAccount();

    // Pure virtual implementations
    AccountType getAccountType() const;
    void applyMonthlyProcessing();

    // Loan-specific methods
    void generateRepaymentSchedule();
    bool makeEMIPayment(double amount, const std::string& description = "EMI Payment");
    void displayRepaymentSchedule() const;
    double calculateRemainingBalance() const;

    // Static utility method
    static double calculateEMI(double principal, double ratePerAnnum, int monthlyTerm);

    // Getters
    double getLoanAmount() const;
    double getLoanInterestRate() const;
    int getLoanTerm() const;
    int getMonthsPaid() const;
    LoanStatus getLoanStatus() const;

    // Display
    void displayAccountInfo() const;

private:
    // Helper to update loan status
    void updateLoanStatus();
};

#endif // LOANACCOUNT_H
