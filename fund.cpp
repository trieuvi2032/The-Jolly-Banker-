#include "fund.h"

Fund::Fund() {
	balance_ = 0;
}

Fund::Fund(string fund_name) {
	fund_name_ = fund_name;
}

Fund::~Fund() {
}

string Fund::fund_name() const {
	return fund_name_;
}

int Fund::balance() const {
	return balance_;
}

void Fund::set_fund_name(string fund_name) {
	fund_name_ = fund_name;
}

bool Fund::Deposit(int amount) {
	// Make sure amount is valid
	if (amount < 0) {
		return false;
	}

	balance_ += amount;
	return true;
}

bool Fund::WithDraw(int amount) {
	// Make sure amount is valid
	if (amount < 0) {
		return false;
	} 

	balance_ -= amount;
	return true;
}

bool Fund::Log(Transaction trans) {
	if ((trans.account_id() == trans.destination_account_id()) && (trans.fund_id() == trans.destination_fund_id())) {
		return false;
	}

	history_transaction_.push_back(trans);
	return true;
}

void Fund::PrintAllHistory() {
	if (history_transaction_.size() > 0) {
		cout << fund_name() << ": $" << balance() << endl;
		for (int i = 0; i < history_transaction_.size(); i++) {
			cout << "  " << history_transaction_[i] << endl;
		}
	}
}

void Fund::PrintFundHistory() {
	for (int i = 0; i < history_transaction_.size(); i++) {
		cout << "  " << history_transaction_[i] << endl;
	}
}
