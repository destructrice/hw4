
#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char


};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* getSuccessor(AVLNode<Key, Value>* node);
    void rotateLeft (AVLNode<Key, Value> *n);
    void rotateRight (AVLNode<Key, Value> *n);
    void insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value>* child);
    void removeFix(AVLNode<Key, Value> *n, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{

    AVLNode<Key,Value>* new_node = new AVLNode<Key,Value>(new_item.first, new_item.second, NULL);
    new_node->setBalance(0);   
    new_node->setRight(NULL);
    new_node->setLeft(NULL);

    if (this->root_ == NULL) {
        this->root_ = new_node;
        return;
    }

    AVLNode<Key,Value> *parent = NULL;
    AVLNode<Key,Value>* next = static_cast<AVLNode<Key,Value>*>(this->root_);

    while (true){
        parent = next;
        if (new_item.first  == parent->getKey()){
            parent->setValue(new_item.second);
            return;
        }
        else if (new_item.first < parent->getKey()) {
            if (parent->getLeft() == NULL) {
                parent->setLeft(new_node);
                new_node->setParent(parent);
                break;
            }
            next = parent->getLeft();
        } 
        else {
            if (parent->getRight() == NULL) {
                parent->setRight(new_node);
                new_node->setParent(parent);
                break;
            }
            next = parent->getRight();
        }
    }

    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
        return;
    }
    else {
        if (parent->getLeft() == new_node){
            parent->setBalance(-1);
        }
        else {
            parent->setBalance(1);
        }
        insertFix(parent, new_node);
    }
}
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getSuccessor(AVLNode<Key, Value>* node) 
{
    if (node->getRight() != NULL) {
        node = node->getRight();
        while (node->getLeft() != NULL) {
            node = node->getLeft();
        }
        return node;
    }
    else{
        AVLNode<Key, Value>* parent = node->getParent();
        while(parent != NULL && node == parent->getRight()){
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value>* child)
 {
    
    if (parent == NULL || parent->getParent() == NULL) {
        return;
    }

    AVLNode<Key, Value> *grandparent = parent->getParent();

    if (parent == grandparent->getLeft()) { 
        grandparent->setBalance(grandparent->getBalance() - 1);

        if (grandparent->getBalance() == 0) {
            return; 
        }

        if (grandparent->getBalance() == -1) {
            insertFix(grandparent, parent);
            return;
        }


        if (child == parent->getLeft()) {
            rotateRight(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);

        } 
        else {
            rotateLeft(parent);
            rotateRight(grandparent);

            if (child->getBalance() == -1) {
                parent->setBalance(0);
                grandparent->setBalance(1);

            } else if (child->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);

            } else {
                parent->setBalance(-1);
                grandparent->setBalance(0);
            }
            child->setBalance(0);
        }

    } 
    else { 
        grandparent->setBalance(grandparent->getBalance() + 1);

        if (grandparent->getBalance() == 0) {
            return; 
        }

        if (grandparent->getBalance() == 1) {
            insertFix(grandparent, parent);
            return;
        }


        if (child == parent->getRight()) { 
            rotateLeft(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);

        } 
        else { 
            rotateRight(parent);
            rotateLeft(grandparent);

            if (child->getBalance() == 1) {
                parent->setBalance(0);
                grandparent->setBalance(-1);
            } 
            else if (child->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);

            } 
            else {
                parent->setBalance(1);
                grandparent->setBalance(0);
            }
            child->setBalance(0);
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
        AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

    if (node == NULL) {
        return;  // the value is not in the BST
    }

    if (node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* successor = getSuccessor(node);
        nodeSwap(node, successor);
    }

    AVLNode<Key, Value> *child = node->getLeft();
    if (node->getRight() != NULL) {
        child = node->getRight();
    }

    AVLNode<Key, Value>* parent = node->getParent();
    if (child != NULL){
        child->setParent(parent);
    }

    int diff;
    if (parent == NULL) {
        this->root_ = child;
    } 
    else {
        if (node == parent->getLeft()) {
            parent->setLeft(child);
            diff = 1;
        } 
        else {
            parent->setRight(child);
            diff = -1;
        }
    }


    delete node;

    removeFix(parent, diff);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
    if (n == NULL){
        return;
    }

    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* c;

    int ndiff = -1;
    if (p != NULL && n==p->getLeft()){
        ndiff = 1;
    }

    if (n->getBalance() + diff == -2){

        c = n->getLeft();
        if (c->getBalance() == -1 || c->getBalance() == 0){
            rotateRight(n);
            if(c->getBalance() == -1){
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            if(c->getBalance() == 0){
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
        }
        else{
            AVLNode<Key, Value>* g = c->getRight();
            rotateLeft(c);
            rotateRight(n);
            if (g->getBalance() == 1){
                n->setBalance(0);
                c->setBalance(-1);
                g->setBalance(0);
            }
            if(g->getBalance() == 0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            if (g->getBalance() == 1){
                n->setBalance(1);
                c->setBalance(0);
                g->setBalance(0);
            }
            removeFix(p,ndiff);
        }
    }
    else if (n->getBalance() == 0){
        n->setBalance(n->getBalance() + diff);
        return;
    }
    else{
        n->setBalance(0);
        removeFix(p,ndiff);
    }


    if (n->getBalance() + diff == 2){

        c = n->getRight();
        if (c->getBalance() == 1 || c->getBalance() == 0){
            rotateLeft(n);
            if(c->getBalance() == 1){
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            if(c->getBalance() == 0){
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
        }
        else{
            AVLNode<Key, Value>* g = c->getLeft();
            rotateRight(c);
            rotateLeft(n);            
            if (g->getBalance() == -1){
                n->setBalance(0);
                c->setBalance(1);
                g->setBalance(0);
            }
            if(g->getBalance() == 0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            if (g->getBalance() == -1){
                n->setBalance(-1);
                c->setBalance(0);
                g->setBalance(0);
            }
            removeFix(p,ndiff);
        }
    }
    else if (n->getBalance() == 0){
        n->setBalance(n->getBalance() + diff);
        return;
    }
    else{
        n->setBalance(0);
        removeFix(p,ndiff);
    }


}
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value> *n)
{
    AVLNode<Key, Value>* y = n->getRight();
    AVLNode<Key, Value>* rootParent = n->getParent();
    y->setParent(rootParent);

    //set the root parent
    if (n->getParent() == NULL) {        
        this->root_ = n->getRight();
    }
    else if (rootParent->getRight() == n){
        rootParent->setRight(y);
    }
    else{
        rootParent->setLeft(y);
    }    

    //pointer shifts
    AVLNode<Key, Value>* c = y->getLeft();

    y->setLeft(n);
    n->setParent(y);
    n->setRight(c);
    if (c != NULL){
        c->setParent(n);
    }
    
}

/**
* Rotates n down and to the right
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value> *n)
{
    AVLNode<Key, Value>* y = n->getLeft();
    AVLNode<Key, Value>* rootParent = n->getParent();

    y->setParent(rootParent);
    if (n->getParent() == NULL) {        
        this->root_= y;
    }
    else if (rootParent->getRight() == n){
        rootParent->setRight(y);
    }
    else{
        rootParent->setLeft(y);
    }    

    AVLNode<Key, Value>* c = y->getRight();

    y->setRight(n);
    n->setParent(y);
    n->setLeft(c);
    if (c != NULL){
        c->setParent(n);
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif