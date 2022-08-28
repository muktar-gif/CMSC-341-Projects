// CMSC 341 - Spring 2020 - Project 3

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BST.h"

// ********************************************
// Additional 'using' statements may be added.
// ********************************************
using std::fstream;


using std::string;
using std::pair;
using std::vector;

class Scanner { // tree of trees class

public:

  class SNode{
    friend Scanner;
  public:
    SNode(pair<int, int> bounds);
    ~SNode();
  private:
    pair<int, int> _bounds;
    BST* _root;
    SNode* _left;
    SNode* _right;
  };

  // Scanner constructor.  Pass in the number of lines in the image
  // (lines), the number of characers per line (range), and a vector
  // of strings containing the ASCII characters corresponding to the
  // indices in the scrambled image; e.g., chars[0] is the character
  // corresponding to "1", chars[1] corresponds to "2", etc.
  Scanner(int lines, int range, vector<string> chars);

  // Scanner destructor.  Must delete the splay tree and all of the
  // BSTs.
  ~Scanner();
  
  // Copy constructor.  Must make a deep copy and should function
  // correctly when the right-hand side is empty.
  Scanner(const Scanner& rhs);

  // Assignment operator.  Must make a deep copy, function correctly
  // when the right-hand side is empty, and protect against
  // self-assignment.
  Scanner& operator=(const Scanner& rhs);

  // Insert (character, weight) pair; splay the node in which the pair
  // is inserted.  Return 'false' if insertion fails, e.g. if the
  // weight is out-of-bounds; return 'true' otherwise.
  bool insert(int weight, int ch);
  
  // Load the data files (ASCII file and weights file) and insert data
  // into the data structure.  Returns 'false' if either file fails to
  // open; returns 'true' otherwise.
  bool loadFiles( string ascii, string weights );

  // Prints the unscrambled ASCII art.  Does an inorder traversal of
  // the splay tree, calling the BST dump() method for each BST.
  void dump() const;

  // Prints an inorder traversal of the splay tree, printing the
  // bounds (upper:lower) for each node in the splay tree.  Prints
  // parentheses to show the structure of the tree.
  void inorder() const;

private:
  SNode* _forest;         // pointer to splay tree root
  int _lines = 0;             // number of lines in the image
  vector<string> _chars;  // characters used in the ascii art.
			  // _chars[0] corresponds to charcter "1",
			  // _chars[1] to character "2", etc.

  // Balance function.  Implements the height-balance propery of AVL
  // trees.  Returns true if the property is violated. The leftSize
  // and rightSize arguments are not used.  Passed to the BST
  // constructor.
  static bool heightImbalance(int leftHeight, int rightHeight, int leftSize, int rightSize);

  int _range = 0;
  
  // *************************************************
  // Additional private function declarations go here!
  // *************************************************

  void copySplay(SNode *node, SNode *prevNode, bool insertLeft); //deep copies tree
  void printSplay(SNode *node, bool printBound) const; //prints inorder splay tree
  void deleteTree(SNode *node); //deletes tree
  void splayTree(SNode *node, int weight); //splayes tree
  SNode* zigRightRotate(SNode *node); //left rotation
  SNode* zagLeftRotate(SNode *node); //right rotation
  
};

#endif
