#include <iostream>
using namespace std;

struct node {
	node* left;
	node* right;
	int value;
};

class BinarySearchTree {
	private:
		node* root;
	public:
		BinarySearchTree() {
			root = NULL;
		}
	
		node* search(node* curNode) {
			return NULL;
		}
	
		bool insert(int i) {
			node* newNode = new node;
			newNode->left = NULL;
			newNode->right = NULL;
			newNode->value = i;
			
			if (root == NULL) {
				root = newNode;
			}
			
			return true;
		}
		
};

int main() {
	BinarySearchTree myTree;
	myTree.insert(5);
	cout << myTree.root->value << endl;
	
	return 1;
}

if (add < current->value)
	if current->left == null
		current->left = new node(add);
		return new node;
	else:
		return insert(current->left, add)