#include "../include/LoanAccount.h"
#include "../include/Utils.h"
#include "../include/Constants.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// Constructor - initializes loan account
LoanAccount::LoanAccount(double principal, double rate, int termMonths)
    : Account(AccountType::LOAN, principal),
      loanAmount(principal),
      loanInterestRate(rate),
      loanTerm(termMonths),
      monthsPaid(0),
      loanStatus(LoanStatus::PENDING_APPROVAL) {
    
    // Set account balance to 0 initially (loan amount is principal, not balance)
    balance = 0;
    loanStatus = LoanStatus::ACTIVE;
    
    // Generate repayment schedule
    generateRepaymentSchedule();
}

// Pure virtual implementation - returns account type
AccountType LoanAccount::getAccountType() const {
    return AccountType::LOAN;
}

// Monthly processing - updates loan status
void LoanAccount::applyMonthlyProcessing() {
    if (isActive) {
        updateLoanStatus();
    }
}

// Generate complete repayment schedule
void LoanAccount::generateRepaymentSchedule() {
    repaymentSchedule.clear();
    
    // Calculate EMI (Equated Monthly Installment)
    double emi = calculateEMI(loanAmount, loanInterestRate, loanTerm);
    double remainingBalance = loanAmount;
    
    // Generate schedule for each month
    for (int month = 1; month <= loanTerm; ++month) {
        EMIPayment payment;
        payment.month = month;
        
        // Calculate interest for this month
        double monthlyRate = loanInterestRate / 12.0;
        double interestPayment = remainingBalance * monthlyRate;
        double principalPayment = emi - interestPayment;
        
        // Ensure last payment doesn't overshoot
        if (month == loanTerm) {
            principalPayment = remainingBalance;
        }
        
        payment.interestPayment = interestPayment;
        payment.principalPayment = principalPayment;
        payment.totalPayment = principalPayment + interestPayment;
        remainingBalance -= principalPayment;
        payment.balance = std::max(0.0, remainingBalance);
        
        repaymentSchedule.push_back(payment);
    }
}

// Make EMI payment
bool LoanAccount::makeEMIPayment(double amount, const std::string& description) {
    if (amount <= 0) {
        throw std::invalid_argument("Payment amount must be positive");
    }
    
    if (loanStatus == LoanStatus::PAID_OFF) {
        throw std::runtime_error("Loan is already fully paid off");
    }
    
    if (monthsPaid >= loanTerm) {
        loanStatus = LoanStatus::PAID_OFF;
        throw std::runtime_error("All payments have been completed");
    }
    
    // Get expected EMI for this month
    if (monthsPaid < static_cast<int>(repaymentSchedule.size())) {
        double expectedPayment = repaymentSchedule[monthsPaid].totalPayment;
        
        if (std::abs(amount - expectedPayment) > 0.01) { // Allow small rounding difference
            std::cout << "Warning: Expected payment is " << Utils::formatCurrency(expectedPayment) 
                      << ", but " << Utils::formatCurrency(amount) << " received.\n";
        }
        
        // Record transaction
        Transaction transaction(TransactionType::EMI_PAYMENT, amount, description);
        addTransaction(transaction);
        
        monthsPaid++;
        balance += amount;
        lastModifiedDate = std::time(nullptr);
        
        // Update loan status
        updateLoanStatus();
        
        std::cout << "EMI Payment of " << Utils::formatCurrency(amount) 
                  << " recorded successfully. Payment " << monthsPaid << " of " 
                  << loanTerm << std::endl;
        return true;
    }
    
    return false;
}

// Calculate EMI using formula: EMI = P * r * (1 + r)^n / ((1 + r)^n - 1)
double LoanAccount::calculateEMI(double principal, double ratePerAnnum, int monthlyTerm) {
    if (principal <= 0 || monthlyTerm <= 0) {
        throw std::invalid_argument("Principal and term must be positive");
    }
    
    double monthlyRate = ratePerAnnum / 12.0;
    
    // If interest rate is 0, EMI is just principal / term
    if (monthlyRate < 0.00001) {
        return principal / monthlyTerm;
    }
    
    // Standard EMI calculation
    double numerator = principal * monthlyRate * std::pow(1 + monthlyRate, monthlyTerm);
    double denominator = std::pow(1 + monthlyRate, monthlyTerm) - 1;
    
    return numerator / denominator;
}

// Display repayment schedule
void LoanAccount::displayRepaymentSchedule() const {
    std::cout << "\n===============================================\n";
    std::cout << "LOAN REPAYMENT SCHEDULE\n";
    std::cout << "Loan Amount: " << Utils::formatCurrency(loanAmount) << std::endl;
    std::cout << "Interest Rate: " << (loanInterestRate * 100.0) << "% per annum\n";
    std::cout << "Loan Term: " << loanTerm << " months\n";
    std::cout << "===============================================\n";
    
    std::cout << std::left << std::setw(10) << "Month"
              << std::setw(18) << "EMI"
              << std::setw(18) << "Principal"
              << std::setw(18) << "Interest"
              << std::setw(18) << "Balance" << std::endl;
    std::cout << "-----------------------------------------------\n";
    
    for (const auto& payment : repaymentSchedule) {
        std::cout << std::left << std::setw(10) << payment.month
                  << std::setw(18) << Utils::formatCurrency(payment.totalPayment)
                  << std::setw(18) << Utils::formatCurrency(payment.principalPayment)
                  << std::setw(18) << Utils::formatCurrency(payment.interestPayment)
                  << std::setw(18) << Utils::formatCurrency(payment.balance) << std::endl;
    }
    
    std::cout << "===============================================\n\n";
}

// Calculate and return remaining balance
double LoanAccount::calculateRemainingBalance() const {
    if (monthsPaid <= 0 || monthsPaid > static_cast<int>(repaymentSchedule.size())) {
        return loanAmount;
    }
    return repaymentSchedule[monthsPaid - 1].balance;
}

// Getters
double LoanAccount::getLoanAmount() const {
    return loanAmount;
}

double LoanAccount::getLoanInterestRate() const {
    return loanInterestRate;
}

int LoanAccount::getLoanTerm() const {
    return loanTerm;
}

int LoanAccount::getMonthsPaid() const {
    return monthsPaid;
}

LoanStatus LoanAccount::getLoanStatus() const {
    return loanStatus;
}

// Update loan status based on payment progress
void LoanAccount::updateLoanStatus() {
    if (monthsPaid >= loanTerm) {
        loanStatus = LoanStatus::PAID_OFF;
    } else if (monthsPaid > 0) {
        loanStatus = LoanStatus::ACTIVE;
    }
}

// Display loan account specific information
void LoanAccount::displayAccountInfo() const {
    Account::displayAccountInfo();
    std::cout << "Loan Amount: " << Utils::formatCurrency(loanAmount) << std::endl;
    std::cout << "Interest Rate: " << (loanInterestRate * 100.0) << "% per annum" << std::endl;
    std::cout << "Loan Term: " << loanTerm << " months" << std::endl;
    std::cout << "Payments Made: " << monthsPaid << " of " << loanTerm << std::endl;
    std::cout << "Remaining Balance: " << Utils::formatCurrency(calculateRemainingBalance()) << std::endl;
    
    std::string statusStr;
    switch (loanStatus) {
        case LoanStatus::ACTIVE:
            statusStr = "Active";
            break;
        case LoanStatus::PAID_OFF:
            statusStr = "Paid Off";
            break;
        case LoanStatus::DEFAULTED:
            statusStr = "Defaulted";
            break;
        case LoanStatus::PENDING_APPROVAL:
            statusStr = "Pending Approval";
            break;
    }
    std::cout << "Loan Status: " << statusStr << std::endl;
}
