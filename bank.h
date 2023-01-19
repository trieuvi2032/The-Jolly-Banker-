#ifndef BANK_H_
#define BANK_H_
#include<queue>
#include"bstree.h"
#include"transaction.h"
#include "account.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include <string>
using namespace std;

class Bank {
public:
	Bank();
	~Bank();

	bool ReadFile(const string& file_name);
	void ProcessQueue();
	void Display();

private:
	queue<Transaction> transactions_list_;
	BSTree account_list_;
};
#endif

