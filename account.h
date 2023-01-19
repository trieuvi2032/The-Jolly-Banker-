#ifndef ACCOUNT_H_	
#define ACCOUNT_H_	
#include <iostream>
#include <vector>
#include "fund.h"
#include "transaction.h"
using namespace std;

class Account {
	friend ostream& operator<<(ostream& stream, Account& acct); // To open account
public:
	// Constructor
	Account();
	Account(int id, string first_name, string last_name);
	// Destructor
	~Account();

	// Getters
	int account_id() const;
	string first_name() const;
	string last_name() const;

	// Actions
	bool AddToFund(int fund_id, int amount);
	bool RemoveFromFund(int fund_id, int amount, Transaction& trans, Transaction& trans1);
	void RecordTrans(Transaction trans, int fund_id);
	void RemoveFromJoinFund(int fund_id1, int fund_id2, int amount, Transaction& trans, Transaction& copy);
	void PrintTransactions();
	void PrintFundTransaction(int fund_id);
	void Error(string first_name, string last_name, int amount, int fund_id, Transaction& trans);
	int TotalAmount(int fund_id) const;
	string FundName(int fund_id);

	// Operator overload
	bool operator==(const Account& acc) const;

private:
	int account_id_;
	string first_name_;
	string last_name_;
	Fund funds_[8]; // Each account has total 8 fund IDs
};
#endif

