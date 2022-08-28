// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include <cmath>
#include "Scanner.h"

// Balance function for use with BSTs.  This implements the
// height-balance property for AVL trees.  Returns true if
// height-balance property is violated.
bool Scanner::heightImbalance(int leftHeight, int rightHeight, int leftSize, int rightSize)
{
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

// SNode constructor.  bounds contains the upper and lower bounds on
// the weights for this BST.
Scanner::SNode::SNode(pair<int, int> bounds)
{
  _bounds = bounds;
  _root = new BST(heightImbalance);
  _left = _right = nullptr;
}

Scanner::SNode::~SNode()
{

  if (_root != nullptr)
    delete _root;
}

// Initialize a Scanner object to process an image with 'lines' lines
// of text, each of which is 'range' characters long.  The ASCII
// characters corresponding to the character indices are passed in
// 'chars'.
Scanner::Scanner(int lines, int range, vector<string> chars)
{

  _forest = nullptr;
  _range = range;
  _lines = lines;

  _chars = chars;
}

Scanner::~Scanner()
{

  if (_forest != nullptr)
    deleteTree(_forest);
}

Scanner::Scanner(const Scanner &rhs)
{

  if (rhs._forest != nullptr)
  {

    // copies tree
    _forest = nullptr;
    copySplay(rhs._forest, _forest, true);

    _lines = rhs._lines;
    _range = rhs._range;
    _chars = rhs._chars;
  }
  else
  {

    _forest = nullptr;
    _lines = rhs._lines;
    _range = rhs._range;

    _chars = rhs._chars;
  }
}

Scanner &Scanner::operator=(const Scanner &rhs)
{

  // empty scanner
  if (rhs._forest != nullptr)
  {

    // checks self assignment
    if (this != &rhs)
    {

      // checks for allocated memory
      if (this->_forest)
      {

        // clears tree
        deleteTree(_forest);
      }

      // copies tree
      _forest = nullptr;
      copySplay(rhs._forest, _forest, true);

      _lines = rhs._lines;
      _range = rhs._range;
      _chars = rhs._chars;
    }
  }
  else
  {

    _forest = nullptr;
    _lines = rhs._lines;
    _range = rhs._range;

    _chars = rhs._chars;
  }

  return *this;
}

// Read the data files and insert into the datastructure.  File in
// 'ascii' contains the scrambled character indices; file 'weights'
// contains the corresponding weights.
bool Scanner::loadFiles(string ascii, string weights)
{

  fstream loadScrambled, loadWeight;

  loadScrambled.open(ascii);
  loadWeight.open(weights);

  // files are invalid
  if (!(loadScrambled) && !(loadWeight))
    return false;

  string getWeight = "";
  string getCharacter = "";

  // runs line times
  for (int i = 0; i < _lines; i++)
  {

    // runs range times
    for (int j = 0; j < _range; j++)
    {

      // if not at the end of line
      if (j != _range - 1)
      {
        getline(loadScrambled, getCharacter, ',');
        getline(loadWeight, getWeight, ',');
      }
      else
      {
        getline(loadScrambled, getCharacter, '\n');
        getline(loadWeight, getWeight, '\n');
      }

      insert(stoi(getWeight), stoi(getCharacter));
    }
  }

  return true;
}

// Insert a node; splay the node that is inserted.
bool Scanner::insert(int weight, int ch)
{

  // out of bounds
  if (weight < -1 || weight > (_lines * _range))
    return false;

  // calculates range based off of weight
  int rangeIndex = ceil(double(weight) / _range);
  int secondBound = rangeIndex * _range;
  int firstBound = secondBound - (_range - 1);

  string data = _chars[ch - 1];

  if (_forest == nullptr)
  {

    pair<int, int> makeBounds;
    makeBounds.first = firstBound;
    makeBounds.second = secondBound;

    SNode *newNode = new SNode(makeBounds);
    newNode->_root->insert(data, weight);

    _forest = newNode;
  }
  else
  {

    SNode *currentNode = _forest;
    SNode *prevNode;

    while (currentNode != nullptr)
    {

      prevNode = currentNode;

      // checks left nodes
      if (weight < currentNode->_bounds.first)
      {
        currentNode = currentNode->_left;
      }
      // checks right nodes
      else if (weight > currentNode->_bounds.second)
      {
        currentNode = currentNode->_right;
      }
      else
      {

        // weights are in bounds
        currentNode->_root->insert(data, weight);

        // doesnt work
        if (currentNode != _forest)
          splayTree(currentNode, weight);

        return true;
      }
    }

    // node with range does not exists, creating snode with range
    pair<int, int> makeBounds;
    makeBounds.first = firstBound;
    makeBounds.second = secondBound;

    SNode *newNode = new SNode(makeBounds);
    newNode->_root->insert(data, weight);

    // connects to tree
    if (newNode->_bounds.first < prevNode->_bounds.first)
    {
      prevNode->_left = newNode;
    }
    else
    {
      prevNode->_right = newNode;
    }

    splayTree(newNode, weight);
  }

  return true;
}

void Scanner::splayTree(SNode *target, int weight)
{

  // node is at root
  while (_forest != target)
  {

    SNode *currentNode = _forest;
    SNode *parent = _forest;
    SNode *grandParent = _forest;
    SNode *greatgrandParent = _forest;

    // tracks node directions for zig and zag
    bool firstZig = false, secondZig = false, firstZag = false, secondZag = false;

    while (currentNode != nullptr)
    {

      firstZig = secondZig;
      firstZag = secondZag;

      greatgrandParent = grandParent;
      grandParent = parent;
      parent = currentNode;

      // checks left nodes
      if (weight < currentNode->_bounds.first)
      {

        currentNode = currentNode->_left;
        secondZig = true;
        secondZag = false;
      }
      // checks right nodes
      else if (weight > currentNode->_bounds.second)
      {

        currentNode = currentNode->_right;
        secondZig = false;
        secondZag = true;
      }

      // if at target
      if (currentNode == target)
      {

        // if zig or zag
        if (parent == _forest)
        {

          // if zig
          if (secondZig)
          {
            _forest = zigRightRotate(parent);
          }
          // if zag
          else if (secondZag)
          {
            _forest = zagLeftRotate(parent);
          }
        }

        // zig-zig
        else if (firstZig && secondZig)
        {

          zigRightRotate(grandParent);

          // sets last rotation to root
          if (grandParent == _forest)
          {
            _forest = zigRightRotate(parent);
          }
          else
          {

            // connections last rotation to splay tree
            if (greatgrandParent->_right == grandParent)
            {
              greatgrandParent->_right = zigRightRotate(parent);
            }
            else if (greatgrandParent->_left == grandParent)
            {
              greatgrandParent->_left = zigRightRotate(parent);
            }
          }
        }
        // zag-zag
        else if (firstZag && secondZag)
        {

          zagLeftRotate(grandParent);

          // sets last rotation to root
          if (grandParent == _forest)
          {
            _forest = zagLeftRotate(parent);
          }
          else
          {

            // connects last rotation to splay tree
            if (greatgrandParent->_right == grandParent)
            {
              greatgrandParent->_right = zagLeftRotate(parent);
            }
            else if (greatgrandParent->_left == grandParent)
            {
              greatgrandParent->_left = zagLeftRotate(parent);
            }
          }
        }
        // zag-zig
        else if (firstZig && secondZag)
        {

          grandParent->_left = zagLeftRotate(parent);

          // sets last rotation to root
          if (grandParent == _forest)
          {
            _forest = zigRightRotate(grandParent);
          }
          else
          {

            // connects last rotation to splay tree
            if (greatgrandParent->_right == grandParent)
            {
              greatgrandParent->_right = zigRightRotate(grandParent);
            }
            else if (greatgrandParent->_left == grandParent)
            {
              greatgrandParent->_left = zigRightRotate(grandParent);
            }
          }
        }
        // zig-zag
        else if (firstZag && secondZig)
        {

          grandParent->_right = zigRightRotate(parent);

          // sets last rotation to root
          if (grandParent == _forest)
          {
            _forest = zagLeftRotate(grandParent);
          }
          else
          {

            // connects last rotation to splay tree
            if (greatgrandParent->_right == grandParent)
            {
              greatgrandParent->_right = zagLeftRotate(grandParent);
            }
            else if (greatgrandParent->_left == grandParent)
            {
              greatgrandParent->_left = zagLeftRotate(grandParent);
            }
          }
        }

        currentNode = nullptr;
      }
    }
  }
}

Scanner::SNode *Scanner::zigRightRotate(SNode *node)
{

  // left node
  SNode *leftNode = node->_left;

  // parent right child says the same, left changes
  node->_left = leftNode->_right;

  // leftNdoes left says the same, left changes
  leftNode->_right = node;

  return leftNode;
}
Scanner::SNode *Scanner::zagLeftRotate(SNode *node)
{

  // right node
  SNode *rightNode = node->_right;

  // parent left child says the same, right changes
  node->_right = rightNode->_left;

  // rightNodes right says the same, left changes
  rightNode->_left = node;

  // new parent
  return rightNode;
}

void Scanner::dump() const
{

  // prints tree from bst
  printSplay(_forest, false);
  cout << endl;
}

void Scanner::inorder() const
{

  // prints splaytree with bounds
  printSplay(_forest, true);
  cout << endl;
}

void Scanner::printSplay(SNode *node, bool printBound) const
{

  // prints tree
  if (node == nullptr)
  {
    return;
  }

  if (printBound)
    cout << "(";

  printSplay(node->_left, printBound);

  if (printBound)
    cout << node->_bounds.first << ":" << node->_bounds.second;
  else
    node->_root->dump(false);

  printSplay(node->_right, printBound);

  if (printBound)
    cout << ")";
}

void Scanner::deleteTree(SNode *node)
{

  if (node == nullptr)
    return;

  deleteTree(node->_left);
  deleteTree(node->_right);

  delete node;
}

void Scanner::copySplay(SNode *node, SNode *prevNode, bool insertLeft)
{

  if (node == nullptr)
    return;

  // creating new snode with proper bounds
  pair<int, int> makeBound(node->_bounds);
  SNode *newNode = new SNode(makeBound);

  // copies binary search tree
  *newNode->_root = *node->_root;

  if (_forest == nullptr)
  {
    _forest = newNode;
  }
  else
  {

    // left insert
    if (insertLeft)
      prevNode->_left = newNode;
    // right insert
    else
      prevNode->_right = newNode;
  }

  copySplay(node->_left, newNode, true);
  copySplay(node->_right, newNode, false);
}
