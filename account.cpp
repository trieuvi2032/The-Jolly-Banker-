#include "account.h"

Account::Account() {
}

Account::Account(int id, string first_name, string last_name) {
	account_id_ = id;
	first_name_ = first_name;
	last_name_ = last_name;

	string funds[] = {"Money Market", "Prime Money Market","Long-Term Bond", "Short-Term Bond","500 Index Fund",
		"Capital Value Fund","Growth Equity Fund", "Growth Index Fund","Value Fund", "Value Stock Index"};

	for (int i = 0; i < 8; i++) {
		funds_[i].set_fund_name(funds[i]);
	}
}

Account::~Account() {
}

int Account::account_id() const {
	return account_id_;
}

string Account::first_name() const {
	return first_name_;
}

string Account::last_name() const {
	return last_name_;
}

bool Account::AddToFund(int fund_id, int amount) {
	if (fund_id < 0 || fund_id > 7) {
		cerr << "ERROR: Fund ID " << fund_id << " is invalid." << endl;
		return false;
	}

	if (amount < 0) {
		cerr << "ERROR: Amount $" << amount << " isn't valid." << endl;
		return false;
	}

	funds_[fund_id].Deposit(amount);
	return true;
}

bool Account::RemoveFromFund(int id, int amount, Transaction& trans, Transaction& trans1) {
	if (amount < 0) {
		cerr << "ERROR: Amount $" << amount << " isn't valid." << endl;
		return false;
	}

	// Check special case of linked Fund Index: 
	// 0 : Money Market
	// 1 : Prime Money Market
	// 2 : Long-Term Bond
	// 3 : Short-Term Bond
	if (id == 0 || id == 1 || id == 2 || id == 3) {
		// First two linked fund
		if (id == 0 || id == 1) {
			if (funds_[id].balance() >= amount) {
				funds_[id].WithDraw(amount);
				funds_[id].Log(trans);
				return true;

			} else if (funds_[0].balance() + funds_[1].balance() >= amount) {
				if (id == 0) {
					RemoveFromJoinFund(0, 1, amount, trans, trans1);
				} else {
					RemoveFromJoinFund(1, 0, amount, trans, trans1);
				}
			} else {
				Error(first_name(), last_name(), amount, id, trans);
				return false;
			}
		} else if (id == 2 || id == 3) {
			// Two other linked funds
			if (funds_[id].balance() >= amount) {
				funds_[id].WithDraw(amount);
				funds_[id].Log(trans);
				return true;

			} else if (funds_[2].balance() + funds_[3].balance() >= amount) {
				if (id == 2) {
					RemoveFromJoinFund(2, 3, amount, trans, trans1);
				} else {
					RemoveFromJoinFund(3, 2, amount, trans, trans1);
				}
			} else {
				Error(first_name(), last_name(), amount, id, trans);
				return false;
			}
		}
	} else {
		// Other individual funds
		if (funds_[id].balance() >= amount) {
			funds_[id].WithDraw(amount);
			funds_[id].Log(trans);

		} else {
			Error(first_name(), last_name(), amount, id, trans);
			return false;
		}
	}
	return true;
}

void Account::RecordTrans(Transaction trans, int fund_id) {
	funds_[fund_id].Log(trans);
}

void Account::RemoveFromJoinFund(int id1, int id2, int amount, Transaction& trans, Transaction& copy) {
	if ((funds_[id1].balance() + funds_[id2].balance()) >= amount) {
		// Withdraw from first fund
		int sub_total1 = funds_[id1].balance();
		funds_[id1].WithDraw(sub_total1);

		// Log transaction depends on the Type
		if (trans.type() == 'W') {
			Transaction his1('W', trans.account_id(), id1, sub_total1);
			funds_[id1].Log(his1);
		} else if (trans.type() == 'T') {
			Transaction his1('T', trans.account_id(), id1, trans.destination_account_id(), trans.destination_fund_id(), sub_total1);
			funds_[id1].Log(his1);
			trans = his1;
		}

		// Get the rest from second fund
		amount = amount - sub_total1;
		funds_[id2].WithDraw(amount);
		if (trans.type() == 'W') {
			Transaction his2('W', trans.account_id(), id2, amount);
			funds_[id2].Log(his2);
		} else if (trans.type() == 'T') {
			Transaction his2('T', trans.account_id(), id2, trans.destination_account_id(), trans.destination_fund_id(), amount);
			funds_[id2].Log(his2);		
			copy = his2;
		}

	} else {
		int sub_total1 = funds_[id1].balance();
		int sub_total2 = funds_[id2].balance();

		if (sub_total1 > amount) {
			funds_[id1].WithDraw(sub_total1);
			// Log transaction depends on the Type
			if (trans.type() == 'W') {
				Transaction his1('W', trans.account_id(), id1, sub_total1);
				funds_[id1].Log(his1);
			} else if (trans.type() == 'T') {
				Transaction his1('T', trans.account_id(), id1, trans.destination_account_id(), trans.destination_fund_id(), sub_total1);
				funds_[id1].Log(his1);
			}
			amount = amount - sub_total1;
		}

		if (sub_total2 >= amount) {
			funds_[id2].WithDraw(amount);
			if (trans.type() == 'W') {
				Transaction his2('W', trans.account_id(), id2, amount);
				funds_[id2].Log(his2);
			} else if (trans.type() == 'T') {
				Transaction his2('T', trans.account_id(), id2, trans.destination_account_id(), trans.destination_fund_id(), amount);
				funds_[id2].Log(his2);
			}
		} else {
			cerr << "ERROR: Not enough funds to withdraw $" << amount << " from " << first_name() << " "
				<< last_name() << " " << FundName(id2) << endl;
		}
	}
}

void Account::PrintTransactions() {
	cout << "Transaction History for " << first_name() << " " << last_name() << " by funds." << endl;
	for (int i = 0; i < 8; i++) {
		funds_[i].PrintAllHistory();
	}
}

void Account::PrintFundTransaction(int fund_id) {
	cout << "Transaction History for " << first_name() << " " << last_name() << " "
		<< FundName(fund_id) << ": $" << TotalAmount(fund_id) << endl;
	funds_[fund_id].PrintFundHistory();
}

void Account::Error(string first_name, string last_name, int amount, int fund_id, Transaction& trans) {
	cerr << "ERROR: Not enough funds to withdraw $" << amount << " from " << first_name << " "
		<< last_name << " " << FundName(fund_id) << endl;
	if (trans.type() == 'W') {
		Transaction his1('W', trans.account_id(), fund_id, amount, " (Failed)");
		funds_[fund_id].Log(his1);
	} else if (trans.type() == 'T') {
		Transaction his2('T', trans.account_id(), fund_id, trans.destination_account_id(), trans.destination_fund_id(), amount, " (Failed)");
		funds_[fund_id].Log(his2);
	}
}

int Account::TotalAmount(int fund_id) const {
	return funds_[fund_id].balance();
}

string Account::FundName(int fund_id) {
	return funds_[fund_id].fund_name();
}

bool Account::operator==(const Account& acc) const {
	return account_id() == acc.account_id();
}

ostream& operator<<(ostream& stream, Account& acct) {
	stream << acct.first_name() << " " << acct.last_name() << " Account ID: " << acct.account_id() << endl;
	for (int i = 0; i < 8; i++) {
		stream << "  " << acct.FundName(i) << ": $" << acct.TotalAmount(i);
		stream << endl;
	}
	return stream;
}
