#include "bstree.h"

BSTree::BSTree() {
}

BSTree::~BSTree() {
}

int BSTree::size() const {
	return size_;
}

bool BSTree::Insert(Account* account) {
	// Insert account root_ is nullptr
	if (root_ == nullptr) {
		root_ = new Node();
		root_->p_acct = account;
		size_++;
		root_->left = nullptr;
		root_->right = nullptr;
		return true;

	} else {
		Node* curr = root_;
		InsertRecursively(curr, account); // Or keep traversing to kind the right spot
	}
	return false;
}

bool BSTree::Retrieve(const int& account_id, Account*& account) const {
	Node* curr = root_;
	bool found = false;

	// Keep searching till it found the account or null
	while (!found) {
		if (curr != nullptr && account_id == curr->p_acct->account_id()) {
			found = true;
			account = curr->p_acct;
			return found;
		} else if (curr != nullptr && account_id > curr->p_acct->account_id()) {
			curr = curr->right;
		} else if (curr != nullptr && account_id < curr->p_acct->account_id()) {
			curr = curr->left;
		} else {
			break;
		}
	}

	return found;
}

void BSTree::DeleteNode(Node* curr) {
	if (curr == nullptr) {
		return;
	}

	// If one side doesn't have any account, get the other side 
	if (curr->left == nullptr) {
		curr = curr->right;
		return;
	}

	if (curr->right == nullptr) {
		curr = curr->left;
		return;
	}

	// Get the smallest node to be next root
	Node* next_root = curr->right;
	Node* prev = nullptr;

	// Keep traverse till left node of right tree is null
	while (next_root->left != nullptr) {
		prev = next_root;
		next_root = next_root->left;
	}

	// Get the smallest node to be new root of this subtree
	next_root->left = curr->left;

	if (curr->right != next_root) {
		prev->left = next_root->right;
		next_root->right = curr->right;
	}

	// Connect it back to curr to make it back to root
	curr = next_root;
}

bool BSTree::Delete(const int& account_id, Account*& account) {
	if (root_ == nullptr) {
		return false;
	}

	Node* curr = root_;
	Node* prev = nullptr;

	// If the account to be deleted is the first node
	while (curr != nullptr && curr->p_acct->account_id() != account_id) {
		prev = curr;
		if (account_id < curr->p_acct->account_id()) {
			curr = curr->left;
		} else {
			curr = curr->right;
		}
	}

	// Delete the first node
	if (prev == nullptr && curr != nullptr) {
		account = curr->p_acct; // Point the account to that object need to be deleted 
		size_--;
		DeleteNode(curr);
		root_ = curr;
		return true;

	} else if (prev != nullptr && curr != nullptr && prev->right == curr) {
		account = curr->p_acct;
		size_--;
		DeleteNode(curr);
		prev->right = curr;  // root_ is connected to prev so from this point 
		return true;

	} else if (prev != nullptr && curr != nullptr && prev->left == curr) {
		account = curr->p_acct;
		DeleteNode(curr);
		size_--;
		prev->left = curr;
		return true;
	}
	return false;
}

bool BSTree::IsEmpty() const {
	return root_ == nullptr;
}

void BSTree::CleanUp() {
	delete root_;
	root_ = nullptr;
}

void BSTree::Display() const {
	if (root_ == nullptr) {
		cerr << "ERROR: The account list is empty. Please try again!" << endl;
		return;
	}

	DisplayRecursively(root_);
}

bool BSTree::InsertRecursively(Node* curr, Account* p_account) {
	if (p_account->account_id() > curr->p_acct->account_id()) {
		if (curr->right == nullptr) { // Traverse right till find null spot
			Node* ins_acct = new Node();
			ins_acct->p_acct = p_account;
			ins_acct->left = nullptr;
			ins_acct->right = nullptr;
			curr->right = ins_acct;
			size_++;
			return true;
		} else {
			return InsertRecursively(curr->right, p_account);
		}
	} else if (p_account->account_id() < curr->p_acct->account_id()) {
		if (curr->left == nullptr) { // Traverse left till find null spot
			Node* ins_acct = new Node();
			ins_acct->p_acct = p_account;
			ins_acct->left = nullptr;
			ins_acct->right = nullptr;
			curr->left = ins_acct;
			size_++;
			return true;
		} else {
			return InsertRecursively(curr->left, p_account);
		}
	} else {
		cerr << "ERROR: Account " << p_account->account_id() << " is already open. Transaction refused." << endl;
		return false;
	}
}

void BSTree::DisplayRecursively(Node* node) const {
	if (node == nullptr) {
		return;
	}

	// Inorder-traversal Print
	DisplayRecursively(node->left);
	cout << *node->p_acct << endl;
	DisplayRecursively(node->right);
}

