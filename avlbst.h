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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    void rotateLeft(AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key,Value>* node);
    void rebalance(AVLNode<Key,Value>* node);
    void updateBalance(AVLNode<Key,Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    AVLNode<Key,Value>* newNode= new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
    if(this->root_ == nullptr){
        this->root_= newNode;
        return;
    }
    AVLNode<Key,Value>* parent = nullptr;
    AVLNode<Key,Value>* cur= static_cast<AVLNode<Key,Value>*>(this->root_);

    while(cur != nullptr){
        parent = cur;
        if(new_item.first < cur->getKey()){
            cur = cur->getLeft();
        }
        else if(new_item.first > cur->getKey()){
            cur = cur -> getRight();
        }
        else{
            cur->setValue(new_item.second);
            delete newNode;
            return;
        }
    }
    newNode->setParent(parent);
    if(new_item.first < parent->getKey()){
        parent->setLeft(newNode);
    }
    else{
        parent->setRight(newNode);
    }
    AVLNode<Key, Value>* n= newNode;
    while(parent != nullptr){
        if(n == parent->getLeft()){
            parent->updateBalance(-1);
        }
        else{
            parent->updateBalance(1);
        }
        if(parent->getBalance() ==0){
            break;
        }
        else if(std::abs(parent->getBalance())==2){
            rebalance(parent);
            break;
        }
        n=parent;
        parent = parent->getParent();
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key,Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(node == nullptr){
        return;
    }
    

    AVLNode<Key,Value>* parent = node->getParent();
    BinarySearchTree<Key,Value>::remove(key);
    while(parent != nullptr){
        if(key < parent->getKey()){
            parent->updateBalance(1);
        }
        else{
            parent->updateBalance(-1);
        }
        if(std::abs(parent->getBalance())==2){
            rebalance(parent);

        }
        if(parent->getBalance() != 0){
            break;
        }

        parent = parent ->getParent();
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

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node){
    AVLNode<Key, Value>* rightChild = node->getRight();
    AVLNode<Key,Value>* grandChild= rightChild->getLeft();

    node->setRight(grandChild);
    if(grandChild != nullptr){
        grandChild->setParent(node);
    }
    rightChild->setParent(node->getParent());
    if(node->getParent() == nullptr){
        this->root_ = rightChild;
    }
    else if(node == node->getParent()->getLeft()){
        node->getParent()->setLeft(rightChild);
    }
    else{
        node->getParent()->setRight(rightChild);
    }

    rightChild->setLeft(node);
    node->setParent(rightChild);

    if(rightChild->getBalance()==1){
        node->setBalance(0);
        rightChild->setBalance(0);
    }
    else if(rightChild->getBalance()==0){
        node->setBalance(1);
        rightChild->setBalance(-1);
    }
    else{
        if(grandChild != nullptr){
            if(grandChild->getBalance() ==1){
                node->setBalance(0);
                rightChild->setBalance(-1);
            }
            else if(grandChild->getBalance() == -1){
                node->setBalance(1);
                rightChild->setBalance(0);
            }
            else{
                node->setBalance(0);
                rightChild->setBalance(0);
            }
            grandChild->setBalance(0);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node){
    AVLNode<Key,Value>* leftChild = node->getLeft();
    AVLNode<Key,Value>* grandChild = leftChild->getRight();

    node->setLeft(grandChild);
    if(grandChild!= nullptr){
        grandChild->setParent(node);
    }

    leftChild->setParent(node->getParent());
    if(node->getParent()==nullptr){
        this->root_ = leftChild;

    }
    else if(node==node->getParent()->getLeft()){
        node->getParent()->setLeft(leftChild);
    }
    else{
        node->getParent()->setRight(leftChild);
    }

    leftChild->setRight(node);
    node->setParent(leftChild);

    if(leftChild->getBalance()==-1){
        node->setBalance(0);
        leftChild->setBalance(0);
    }
    else if(leftChild->getBalance()==0){
        node->setBalance(-1);
        leftChild->setBalance(1);
    }
    else{
        if(grandChild!=nullptr){
            if(grandChild->getBalance()==-1){
                node->setBalance(0);
                leftChild->setBalance(1);
            }
            else if(grandChild ->getBalance()==1){
                node->setBalance(-1);
                leftChild->setBalance(0);
            }
            else{
                node->setBalance(0);
                leftChild->setBalance(0);
            }
            grandChild->setBalance(0);
        }
    }

}
template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node){
    if(node->getBalance() == -2){
        //left heavy
        if(node->getLeft()->getBalance() <= 0){
            rotateRight(node);
        }
        else{
            rotateLeft(node->getLeft());
            rotateRight(node);
        }
    }
    else if(node->getBalance()==2){
        if(node->getRight()->getBalance() >=0){
            rotateLeft(node);
        }
        else{
            rotateRight(node->getRight());
            rotateLeft(node);
        }
    }
}
template<class Key, class Value>
void AVLTree<Key, Value>::updateBalance(AVLNode<Key, Value>* node){
    if(node->getBalance() < -1 || node->getBalance() >1){
        rebalance(node);
    }
    if(node->getParent() != nullptr){
        if(node == node->getParent()->getLeft()){
            node->getParent()->updateBalance(-1);
        }
        else{
            node->getParent()->updateBalance(-1);
        }
    }
}


#endif
