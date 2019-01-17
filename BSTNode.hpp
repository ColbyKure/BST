//
// BSTNode.hpp
// CSE 100 Project 1
//
// Last modified by Heitor Schueroff on 01/10/2019
//

#ifndef BSTNODE_HPP
#define BSTNODE_HPP

#include <iostream>
#include <iomanip>

using namespace std;

template <typename Data>
class BSTNode {
public:
    BSTNode<Data> *left;
    BSTNode<Data> *right;
    BSTNode<Data> *parent;
    Data const data;
    
    /** 
     * Constructor that initializes a BSTNode with the given data.
     */
    BSTNode(const Data &d) : data(d) {
        left = right = parent = nullptr;
    }

    /**
     * This function will tell what type of child 
     * return -1 - not child
     * return 0 - left child
     * return 1 - right child
     */
     static int typeChild(BSTNode<Data> * n) {
	 if (n->parent->left == n) {
	     return 0;
	 }
	 else if (n->parent->right == n) {
	     return 1;
	 }
         else {
	     return -1;
	 }
     }

    /**
     * Find the successor this node.
     *
     * The successor of a node is the node in the BST whose data
     * value is the next in the ascending order.
     *
     * Returns:
     *     the BSTNode that is the successor of this BSTNode,
     *     or 0 if there is none (this is the last node in the BST).
     */
    BSTNode<Data> *successor() {
    	BSTNode<Data> *curr = this;

	if(right == nullptr){
	    while(typeChild() == 1){
	    	curr = curr->parent;
	    }
	    if(curr->parent == nullptr) {
		return nullptr;
	    }
	    return curr->parent;
	}
	else{
	    curr = right;
	    while (curr->left != nullptr){
	    	curr = curr-> left;
	    }
	    return curr;
	}	    
};

/** 
 * Overload operator<< to print a BSTNode's fields to an ostream. 
 */
template <typename Data>
ostream &operator<<(ostream &stm, const BSTNode<Data> &n) {
    stm << '[';
    stm << setw(10) << &n;                  // address of the BSTNode
    stm << "; p:" << setw(10) << n.parent;  // address of its parent
    stm << "; l:" << setw(10) << n.left;    // address of its left child
    stm << "; r:" << setw(10) << n.right;   // address of its right child
    stm << "; d:" << n.data;                // its data field
    stm << ']';
    return stm;
}

#endif  // BSTNODE_HPP
