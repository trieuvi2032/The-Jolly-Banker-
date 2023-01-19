#ifndef BSTREE_H_	
#define BSTREE_H_	
#include <iostream>
#include "account.h"
using namespace std;

class BSTree {
public:
	BSTree();
	~BSTree();

	// Getters 
	int size() const;

	bool Insert(Account* account);
	bool Retrieve(const int& account_id, Account*& account) const; // Find the account in tree
	bool Delete(const int& account_id, Account*& account); 
	bool IsEmpty() const;
	void CleanUp();
	void Display() const;
	
private:
	struct Node {
		Account* p_acct;
		Node* right;
		Node* left;
	};
	Node* root_ = nullptr;
	int size_;

	// Helpers
	bool InsertRecursively(Node* curr, Account* p_account);
	void DisplayRecursively(Node* node) const;
	void DeleteNode(Node* curr);
};
#endif
