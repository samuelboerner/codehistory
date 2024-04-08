#include <iostream>
#include <stdlib.h>
using namespace std;

struct node {
	int data;
	node* next;
};

class LinkedList {
	private:
		node* head;
		node* tail;
	public:
		LinkedList() {
			head = NULL;
			tail = NULL;
		}

		void append(int i) {
			node* temp = new node;
			temp->data = i;
			temp->next = NULL;
			
			if (tail == NULL) {
				head = temp;
				tail = temp;
			}
			else {
			tail->next = temp;
				
			// saying temp could also work here
			tail = tail->next;
			}
		}
	
		void sever(int i) {
			node* myNode = findNode(i);
			tail = myNode;
			tail->next = NULL;
		}
		
		node* findNode(int i) {
			node* curNode = head;
			for (int j = 0; j < i - 1; j++) {
				curNode = curNode->next;
			}
			
			return curNode;
		}
		
		
		
		void printNode(int i = 0) {
			if (i)
				cout << "My current node is " << findNode(i)->data << "!" << endl;
			else
				cout << "My current node is " << tail->data << "!" << endl;
		}
};

int main() {
	LinkedList myList;
	for (int i = 0; i < 10; i++) {
		myList.append(rand()%100);	
	}
	cout << rand()%100 << endl;
	myList.printNode(5);
	myList.sever(4);
	myList.append(5);
	myList.printNode();
	
	return 0;
}