#ifndef TRANSACTION_H_
#define TRANSACTION_H_
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Transaction {
	friend ostream& operator<<(ostream& stream, const Transaction& trans);
public:
	// Constructors & Destructor
	Transaction();
	Transaction(char type, int account_id);
	Transaction(char type, int account_id, string first_name, string last_name);
	Transaction(char type, int account_id, int fund_id);
	Transaction(char type, int account_id, int fund_id, int amount);
	Transaction(char type, int from_account, int from_fund_id, int to_account, int to_fund_id, int amount);
	Transaction(const Transaction& trans);
	~Transaction();

	// Fail transaction
	Transaction(char type, int from_account, int from_fund_id, int to_account, int to_fund_id, int amount, string fail);
	Transaction(char type, int account_id, int fund_id, int amount, string fail);
	Transaction& operator=(const Transaction& trans);

	// Getters
	char type() const;
	string first_name() const;
	string last_name() const;
	string fail() const;
	int account_id() const; // This can be id from original account or the "transfer-from-account"
	int fund_id() const;    // Original fund ID
	int destination_account_id() const;  // The destination account
	int destination_fund_id() const;     // The destination fund ID
	int amount() const;
	void set_amount(int amount);

	// Action
	bool IsFail() const;

private:
	char type_; // Transaction type
	string first_name_;
	string last_name_;
	string fail_;
	int amount_;
	int account_id_;
	int fund_id_;
	int destination_account_id_, destination_fund_id_; // To this account and this fund ID
};
#endif

