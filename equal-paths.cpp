#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

//helper to help recursively check if the paths are equal to each other
//depth will track where we are in the AVL Tree
//leadDepth will store the depth of each of the leaves we are in
bool checkIfEqual(Node* node, int depth, int& leafDepth);


bool equalPaths(Node * root)
{
  // Add your code below
  //initialize the depth to be -1, which means we haven't found any leaves yet
  int leafDepth=-1;
  //call the recursive function
  return checkIfEqual(root, 0, leafDepth);

}

bool checkIfEqual(Node* node, int depth, int& leafDepth){
  //base case: if the current node is null, then there is no leaf 
  if(node == nullptr){
    return true;
  }

  //if the current node is a leaf then there are no children
  if(node -> left == nullptr && node-> right ==nullptr){
    //if this is the first leaf (since initialized to -1)
    if(leafDepth == -1){
      leafDepth = depth;
      return true;
    }
    //check if the depth of other leaves matches the depth of first leaf
    return depth == leafDepth;
  }

  //recursively check the left and right 
  //since you are going down the tree, also take note of the depth by +1
  bool left = checkIfEqual(node -> left, depth+1, leafDepth);
  bool right= checkIfEqual(node-> right, depth +1, leafDepth);
  //both must be true in order to return true and have the tree be valid
  return left && right;
}

