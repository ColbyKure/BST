
// BSTIterator.hpp
// CSE 100 Project 1
//
// Last modified by Sunny Sun and Colby Kure on 01/16/2019
//

#ifndef BSTITERATOR_HPP
#define BSTITERATOR_HPP

#include <iterator>
#include <list>
#include <stdio.h>

#include "BSTNode.hpp"

using namespace std;

template <typename Data>
class BSTIterator : public iterator<input_iterator_tag, Data> {
private:
    BSTNode<Data> *curr;

public:
    /** 
     * Constructor that initializes a BSTIterator pointing to curr. 
     */
    BSTIterator(BSTNode<Data>* curr) {
        this->curr = curr;
    }

    /** 
     * Dereference operator. 
     */
    Data operator*() const {
        return curr->data; 
    }

    /** 
     * Pre-increment operator. 
     */
    BSTIterator<Data>& operator++() {
        curr = curr->successor();
        return *this;
    }

    /** 
     * Post-increment operator. 
     */
    BSTIterator<Data> operator++(int) {
        BSTIterator before = BSTIterator(curr);
        ++(*this);
        return before;
    }

    /** 
     * Equality test operator. 
     */  
    bool operator==(BSTIterator<Data> const& other) const {
    	if(other.curr == this->curr){ //this is always a pointer
	    return true;
	}
	else{
	    return false;
	}
    }


    /** 
     * Inequality test operator. 
     */  
    bool operator!=(BSTIterator<Data> const& other) const {
    	if(other.curr != this->curr){ //this is always a pointer
	    return true;
	}
	else{
	    return false;
	}
    }
};

#endif  // BSTITERATOR_HPP
