#include "bank.h"

Bank::Bank() {
}

Bank::~Bank() {
}

bool Bank::ReadFile(const string& file_name) {
	string line;
	ifstream in_file(file_name);
	if (!in_file) {
		cerr << "ERROR: Can't open the file!" << endl;
		return false;
	}

	while (!in_file.eof()) {
		getline(in_file, line);
		if (line.empty()) {
			break;
		}

		istringstream parse_line(line);
		char type;
		parse_line >> type;

		if (type == 'O') {
			int acct_id;
			string first_name, last_name;
			parse_line >> acct_id >> last_name >> first_name;
			Transaction trans(type, acct_id, first_name, last_name);
			transactions_list_.push(trans);

		} else if (type == 'D' || type == 'W') {
			int acct_id, fund_id, amount;
			parse_line >> acct_id >> fund_id >> amount;
			Transaction trans(type, acct_id, fund_id, amount);
			transactions_list_.push(trans);

		} else if (type == 'T') {
			int acct_id, fund_id;
			int dest_acct, dest_fund, amount;
			parse_line >> acct_id >> fund_id >> dest_acct >> dest_fund >> amount;
			Transaction trans(type, acct_id, fund_id, dest_acct, dest_fund, amount);
			transactions_list_.push(trans);

		} else if (type == 'A') {
			int acct_id;
			parse_line >> acct_id;
			Transaction trans(type, acct_id);
			transactions_list_.push(trans);

		} else if (type == 'F') {
			int acct_id, fund_id;
			parse_line >> acct_id >> fund_id;
			Transaction trans(type, acct_id, fund_id);
			transactions_list_.push(trans);

		} else {
			cerr << "ERROR: " << type << " is invalid type of Transaction." << endl;
		}
	}
	return false;
}

void Bank::ProcessQueue() {
	while (!transactions_list_.empty()) {
		Transaction trans = transactions_list_.front();
		Transaction trans1 = trans;
		transactions_list_.pop();

		// All account numbers need to be positive to process
		if (trans.account_id() < 0) {
			cerr << "ERROR: Account " << trans.account_id() << " is not a valid account. Account number needs to be positive." << endl;
			continue;
		}

		if (trans.type() == 'O') {
			// Open account
			Account* acct = new Account(trans.account_id(), trans.first_name(), trans.last_name());
			account_list_.Insert(acct);

		} else if (trans.type() == 'D') {
			// Deposit
			int acc_id = trans.account_id();
			int fund_id = trans.fund_id();
			int amount = trans.amount();

			Account* acct;
			if (account_list_.Retrieve(acc_id, acct)) {
				if (amount < 0) {
					cerr << "ERROR: Deposit Failed - Amount $" << amount << " is not a valid number." << endl;
					Transaction fail(trans.type(), acc_id, fund_id, amount, " (Failed)");
					acct->RecordTrans(fail, fund_id);
					continue;
				} else if  (trans.fund_id() < 0 || trans.fund_id() > 7) {
					cerr << "ERROR: Fund ID " << trans.fund_id() << " is not valid." << endl;
					continue;
				}

				acct->AddToFund(fund_id, amount);
				acct->RecordTrans(trans, fund_id);
			} else {
				cerr << "ERROR: Account " << acc_id << " not found. Can't deposit." << endl;
				continue;
			}

		} else if (trans.type() == 'W') {
			// Withdraw
			int acc_id = trans.account_id();
			int fund_id = trans.fund_id();
			int amount = trans.amount();

			Account* acct;
			if (account_list_.Retrieve(acc_id, acct)) {
				if (amount < 0) {
					cerr << "ERROR: Withdraw Failed - Amount $" << amount << " is not a valid number." << endl;
					Transaction fail(trans.type(), acc_id, fund_id, amount, " (Failed)");
					acct->RecordTrans(fail, fund_id);
					continue;

				} else if (trans.fund_id() < 0 || trans.fund_id() > 7) {
					cerr << "ERROR: Fund ID " << trans.fund_id() << " is not valid." << endl;
					continue;
				}
				acct->RemoveFromFund(fund_id, amount, trans, trans1);

			}  else {
				cerr << "ERROR: Account " << acc_id << " not found. Can't withdraw." << endl;
				continue;
			}

		} else if (trans.type() == 'T') {
			// Original Account
			int acc_id = trans.account_id();
			int fund_id = trans.fund_id();

			// Transfer Account
			int amount = trans.amount();
			int dest_acct = trans.destination_account_id();
			int dest_fund = trans.destination_fund_id();

			trans1.set_amount(-100);

			Account* from, * to;
			if (!account_list_.Retrieve(acc_id, from)) {
				// Make sure the sender also exists 
				cerr << "ERROR: Account " << acc_id << " not found. Transferal refused." << endl;
				continue;

			} else if (account_list_.Retrieve(acc_id, from) && account_list_.Retrieve(dest_acct, to)) {
				// Amount check
				if (amount < 0) {
					cerr << "ERROR: Transfer Failed - Amount $" << amount << " is not a valid number." << endl;
					Transaction fail(trans.type(), acc_id, fund_id, dest_acct, dest_fund, amount, " (Failed)");
					from->RecordTrans(fail, fund_id);
					continue;

				} else if (trans.fund_id() < 0 || trans.fund_id() > 7 || trans.destination_fund_id() < 0 || trans.destination_fund_id() > 7) {
					if (trans.fund_id() < 0 || trans.fund_id() > 7) {
						cerr << "ERROR: Fund ID " << trans.fund_id() << " is not valid." << endl;
					} else {
						cerr << "ERROR: Fund ID " << trans.destination_fund_id() << " is not valid." << endl;
					}
					continue;

				} else if (from->RemoveFromFund(fund_id, amount, trans, trans1)) {	
					// If transfer successfully, and if use money from linked fund, record it trans1 
					to->AddToFund(dest_fund, amount);
					to->RecordTrans(trans, dest_fund);
					if (trans1.account_id() == trans.account_id() && trans1.amount() != -100) {
						to->RecordTrans(trans1, dest_fund);
					}

				} else {
					cerr << "ERROR: Not enough funds to transfer $" << amount << " from account " << acc_id 
						<< " " << fund_id << " to " << dest_acct << " " << dest_fund << endl;
					Transaction fail('T', acc_id, fund_id, dest_acct, dest_fund, trans.amount(), " (Failed");
					to->RecordTrans(fail, dest_fund);
					continue;
				}

			} else {
				// If the transferal account doesn't exist, add this fail transaction to original account
				if (account_list_.Retrieve(acc_id, from) && !account_list_.Retrieve(dest_acct, to)) {
					cerr << "ERROR: Account " << dest_acct << " not found. Transferal refused." << endl;
					Transaction fail('T', acc_id, fund_id, dest_acct, dest_fund, trans.amount(), " (Failed");
					from->RecordTrans(fail, fund_id); // Recheck
				}
			}
		} else if (trans.type() == 'A') {
			// Print all the funds that has history transactions
			Account* acct;
			int acc_id = trans.account_id();
			if (account_list_.Retrieve(acc_id, acct)) {
				acct->PrintTransactions();
				cout << endl;
			} else {
				cerr << "ERROR: Account " << acc_id << " not found. Can't print the history transaction." << endl;
				continue;
			}

		} else if (trans.type() == 'F') {
			// Only print the fund required 
			Account* acct;
			int acc_id = trans.account_id();
			int fund_id = trans.fund_id();
			if (account_list_.Retrieve(acc_id, acct)) {
				if (trans.fund_id() < 0 || trans.fund_id() > 7) {
					cerr << "ERROR: Fund ID " << trans.fund_id() << " is not valid." << endl;
					continue;
				}
				acct->PrintFundTransaction(fund_id);
				cout << endl;
			} else {
				cerr << "ERROR: Account " << acc_id << " not found. Can't check the fund transaction." << endl;
				continue;
			}
		} else {
			cerr << "ERROR: Transaction type " << trans.type() << " is not a valid type." << endl;
		}
	}
}

void Bank::Display() {
	cout << "FINAL BALANCES: " << endl;
	account_list_.Display();
}
