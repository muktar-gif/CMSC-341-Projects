// CMSC 341 - Spring 2020 - Project 3

#include "BST.h"

// Constructor for the BNode nested class
BST::BNode::BNode(string ch, int rank, int height, int size)
{
  _data = ch;
  _key = rank;
  _height = height;
  _size = size;
  _right = nullptr;
  _left = nullptr;
}

// Constructor for the BST class; requires a balance function
BST::BST(balfn_t imbalanced)
{

  _root = nullptr;
  _imbalanced = imbalanced;
}

BST::~BST()
{

  // root is not empty
  if (_root != nullptr)
  {
    deleteTree(_root);
  }
}

BST::BST(const BST &rhs)
{

  if (rhs._root != nullptr)
  {

    // copies tree
    _root = nullptr;
    copyBST(rhs._root, _root, true);
    _imbalanced = rhs._imbalanced;
  }
  else
  {

    _root = nullptr;
    _imbalanced = rhs._imbalanced;
  }
}

BST &BST::operator=(const BST &rhs)
{

  // root is not empty
  if (rhs._root != nullptr)
  {

    // checks self assignement;
    if (this != &rhs)
    {

      // checks allocated memmory
      if (this->_root)
      {

        // clears tree
        deleteTree(_root);
      }

      // copies tree
      _root = nullptr;
      copyBST(rhs._root, _root, true);
      _imbalanced = rhs._imbalanced;
    }
  }
  else
  {

    _root = nullptr;
    _imbalanced = rhs._imbalanced;
  }

  return *this;
}

// Insert a (character, rank) pair
bool BST::insert(string ch, int rank)
{

  BNode *newNode = new BNode(ch, rank);

  // empty tree
  if (_root == nullptr)
  {

    _root = newNode;
  }
  else
  {

    BNode *currentNode = _root;
    BNode *prevNode;

    // update height varibles
    int countHeight = 0;

    // traverses to last node key can be inserted at
    while (currentNode != nullptr)
    {

      prevNode = currentNode;
      currentNode->_size++;
      countHeight++;

      // rank is less than key
      if (rank < currentNode->_key)
      {

        currentNode = currentNode->_left;
      }
      // rank is greater than key
      else if (rank > currentNode->_key)
      {

        currentNode = currentNode->_right;
      }
      else
      {

        // duplicate key found, undoes changes to _size
        undoSize(rank);

        delete newNode;
        return false;
      }
    }

    // node height needs to possibly be updated
    if (prevNode->_left == nullptr && prevNode->_right == nullptr)
    {

      updateHeight(countHeight, rank);
    }

    // inserts node to the left
    if (rank < prevNode->_key)
    {

      prevNode->_left = newNode;
    }
    // inserts node to the right
    else
    {

      prevNode->_right = newNode;
    }

    // checks for rebalancing
    checkBalance(rank);
  }

  return true;
}

int BST::size() const
{
  return (_root == nullptr ? 0 : _root->_size);
}

int BST::height() const
{
  return (_root == nullptr ? -1 : _root->_height);
}

void BST::dump(bool verbose) const
{

  // prints tree
  print(_root, verbose);
  cout << endl;
}

void BST::print(BNode *node, bool verbose) const
{

  if (node == nullptr)
  {
    return;
  }

  if (verbose)
    cout << "(";

  print(node->_left, verbose);

  cout << node->_data;

  if (verbose)
  {

    cout << ":" << node->_height
         << ":" << node->_size;
  }

  print(node->_right, verbose);

  if (verbose)
    cout << ")";
}

void BST::deleteTree(BNode *node)
{

  // deletes tree
  if (node == nullptr)
    return;

  deleteTree(node->_left);
  deleteTree(node->_right);
  delete node;
}

int BST::makeSortedArray(BNode *node, BNode **array, int index)
{

  if (node == nullptr)
  {
    return index;
  }

  // move left
  index = makeSortedArray(node->_left, array, index);

  // adds node to array
  array[index] = node;
  index++;

  // move right
  index = makeSortedArray(node->_right, array, index);

  return index;
}

void BST::makeBST(BNode *array[], int firstIndex, int lastIndex, BNode *prevNode, bool insertLeft, bool atRoot)
{

  // no more array avaible
  if (firstIndex > lastIndex)
  {

    prevNode->_left = nullptr;
    prevNode->_right = nullptr;

    return;
  }

  // gets node index
  int nodeIndex = (lastIndex + firstIndex) / 2;

  BNode *updateNode;
  updateNode = array[nodeIndex];

  // updates node's values
  updateNode->_size = (lastIndex - firstIndex) + 1;
  updateNode->_height = ((lastIndex - firstIndex) + 1) / 2;

  updateNode->_left = nullptr;
  updateNode->_right = nullptr;

  // not at root
  if (!atRoot)
  {

    // inserts left
    if (insertLeft)
    {

      prevNode->_left = updateNode;
    }
    // inserts right
    else
    {

      prevNode->_right = updateNode;
    }
  }
  else
  {
    _root = updateNode;
  }

  makeBST(array, firstIndex, nodeIndex - 1, updateNode, true, false);
  makeBST(array, nodeIndex + 1, lastIndex, updateNode, false, false);
}

void BST::updateHeight(int countHeight, int rank)
{

  BNode *currentNode = _root;

  // goes down the same path of nodes using rank value
  while (currentNode != nullptr)
  {

    // height needs to be incremented for added node
    if (currentNode->_height < countHeight)
      currentNode->_height++;

    // rank is less than key
    if (rank < currentNode->_key)
    {

      currentNode = currentNode->_left;
    }
    // rank is greater than key
    else
    {

      currentNode = currentNode->_right;
    }

    // height decrements moving down tree
    countHeight--;
  }
}

void BST::checkBalance(int rank)
{

  // boolean to track insertion
  bool leftInsert = false;

  BNode *currentNode = _root;
  BNode *prevNode = _root;

  // traverses to last node key can be inserted at
  while (currentNode != nullptr)
  {

    // holds values to check balance
    int leftHeight, rightHeight, leftSize, rightSize;

    // if there is no left node
    if (currentNode->_left == nullptr)
    {
      leftHeight = -1;
      leftSize = 0;
    }
    else
    {
      leftHeight = currentNode->_left->_height;
      leftSize = currentNode->_left->_size;
    }

    // if there is no right node
    if (currentNode->_right == nullptr)
    {
      rightHeight = -1;
      rightSize = 0;
    }
    else
    {
      rightHeight = currentNode->_right->_height;
      rightSize = currentNode->_right->_size;
    }

    // checks if node is imbalanced
    if (_imbalanced(leftHeight, rightHeight, leftSize, rightSize))
    {

      // makes sorted array
      BNode **sortArray = new BNode *[currentNode->_size];
      makeSortedArray(currentNode, sortArray, 0);

      // turns array into bst
      if (currentNode == _root)
      {
        makeBST(sortArray, 0, currentNode->_size - 1, prevNode, leftInsert, true);
      }
      else
      {
        makeBST(sortArray, 0, currentNode->_size - 1, prevNode, leftInsert, false);
      }

      delete[] sortArray;
      break;
    }

    // traverses through tree
    prevNode = currentNode;

    if (rank < currentNode->_key)
    {

      currentNode = currentNode->_left;
      leftInsert = true;
    }
    else
    {

      currentNode = currentNode->_right;
      leftInsert = false;
    }
  }
}

void BST::copyBST(BNode *node, BNode *prevNode, bool insertLeft)
{

  if (node == nullptr)
    return;

  // new node with values
  BNode *newNode = new BNode(node->_data, node->_key, node->_height, node->_size);

  // tree is emtpy
  if (_root == nullptr)
  {
    _root = newNode;
  }
  else
  {

    // inserts left
    if (insertLeft)
      prevNode->_left = newNode;
    // inserts right
    else
      prevNode->_right = newNode;
  }

  copyBST(node->_left, newNode, true);
  copyBST(node->_right, newNode, false);
}

bool BST::undoSize(int rank)
{

  BNode *currentNode = _root;

  // traverses to last node key can be inserted at
  while (currentNode != nullptr)
  {
    currentNode->_size--;

    // rank is less than key
    if (rank < currentNode->_key)
    {

      currentNode = currentNode->_left;
    }
    // rank is greater than key
    else if (rank > currentNode->_key)
    {

      currentNode = currentNode->_right;
    }
    else
    {
      // reached rank node where duplicate was found
      return true;
    }
  }

  return false;
}

/*
// A sample balance function.  This implements the height-balanced
// property from AVL trees.

bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize)
{
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

// A sample main to exercise BST separately.  This is *NOT* a thorough
// test program

int main()
{

  BST bst(imbalfn);

  bst.insert("A", 5);
  bst.insert("B", 10);
  bst.insert("C", 15);
  bst.insert("D", 20);
  bst.insert("E", 30);
  bst.insert("F", 40);
  bst.insert("G", 55);
  bst.insert("H", 60);
  bst.insert("I", 65);
  bst.insert("J", 70);
  bst.insert("K", 75);
  bst.insert("L", 80);

  bst.dump();
  bst.dump(true);

  cout << "size = " << bst.size() << endl;
  cout << "height = " << bst.height() << endl;

  return 0;
}
*/