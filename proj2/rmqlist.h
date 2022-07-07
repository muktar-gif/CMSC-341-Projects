// CMSC 341 - Spring 2020 - Project 2
// Author: Muhammed Muktar
// File: rmqlist.h
//
// Range Min Query Project

#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>

using std::cout;
using std::endl;
using std::invalid_argument;
using std::ostream;
using std::range_error;
using std::sqrt;
using std::swap;

// Macro for a two-argument min function
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// forward declarations
template <class K, class V>
class RMQList;
template <class K, class V>
class Node;
template <class K, class V>
ostream &operator<<(ostream &sout, const Node<K, V> &x);

// *********************************************
// Node - node class for the RMQList linked list
//   key and value are templated (types K and V)
// *********************************************

template <class K, class V>
class Node
{
  friend RMQList<K, V>;

public:
  Node(K key = K(), V value = V(), Node<K, V> *next = nullptr)
  {
    _key = key;
    _value = value;
    _next = next;
  }
  friend ostream &operator<< <K, V>(ostream &sout, const Node<K, V> &x);

private:
  K _key;
  V _value;
  Node *_next;
};

// Overloaded insertion operator for Node
template <class K, class V>
ostream &operator<<(ostream &sout, const Node<K, V> &x)
{
  sout << "Key: " << x._key << ", Value: " << x._value;
  return sout;
}

// *******************************************************
// RMQList - list container (linked list) with RMQ support
//   key and value are templated (types K and V)
// *******************************************************

template <class K, class V>
class RMQList
{
public:
  // Create an empty RMQList object
  RMQList();

  // Destructor, Copy Constructor, Assignment Operator
  ~RMQList();
  RMQList(const RMQList<K, V> &rhs);
  const RMQList<K, V> &operator=(const RMQList<K, V> &rhs);

  // In-line function. Returns the size (number of elements).
  int size() const { return _listSize; }

  // In-line function. Returns true if the list is empty; false
  // otherwise.
  bool empty() const { return _head == nullptr; }

  // Insert an element into the list; list must be kept in increasing
  // order by key; duplicate keys are not allowed, so return false if
  // there is already an entry with the specified key, true otherwise.
  // Should check if key > largest current key and, if so, append to
  // the end of the list without iteration.
  bool insert(const K &key, const V &value);

  // Remove an element from the list; return false if no element
  // exists with the specified key value, true otherwise
  bool remove(const K &key);

  // Update value for the element with given key; return false if
  // there is no element with the given key, true otherwise
  bool update(const K &key, const V &value);

  // RMQ Query for k1 to k2.  Throws exceptions (see documentation).
  V query(const K &k1, const K &k2);

  // Dump the list entries
  void dumpList() const;

  // Dump the RMQ info and table.  What gets dumped depends on which
  // RMQ method is used.
  void dumpTable() const;

  // Clear the data data strucures
  void clear();

private:
  Node<K, V> *_head;
  Node<K, V> *_tail;
  int _listSize;

  // **********************************
  // Private variables for RMQ go here!
  // **********************************

  V **_minValues;    // stores minimum values
  bool _updatedVal;  // indicates when the update minValue
  int _minValueSize; // current size of minValues

  // *******************************************
  // Declarations for private functions go here!
  // *******************************************

  // Desc:           get the index of a key given in the link list
  // Pre-condition:  key exists and list is not empty
  // Post-condition: returns index corresponding to key
  int getIndex(K key);

  // Desc:           updated _minValue when needed in query.
  // Pre-condition:  list is not empty
  // Post-condition: updates _minValue
  void updateMinValue();
};

template <class K, class V>
RMQList<K, V>::RMQList()
{
  _head = NULL;
  _tail = NULL;
  _listSize = 0;

  _minValueSize = 0;
  _updatedVal = false;
}

template <class K, class V>
RMQList<K, V>::~RMQList()
{

  clear();
}

template <class K, class V>
RMQList<K, V>::RMQList(const RMQList<K, V> &rhs)
{

  // if rhs is not empty
  if (!rhs.empty())
  {

    _listSize = rhs._listSize;
    _minValueSize = rhs._minValueSize;
    _updatedVal = rhs._updatedVal;

    // copies rhs minValue if not empty
    if (rhs._minValueSize != 0)
    {

      // allocating memory for _minVlaues
      _minValues = new V *[_minValueSize];

      for (int i = 0; i < _minValueSize; i++)
      {

        _minValues[i] = new V[_minValueSize];
      }

      // deep copies minValues
      for (int r = 0; r < rhs._minValueSize; r++)
      {

        for (int c = r; c < rhs._minValueSize; c++)
        {

          _minValues[r][c] = rhs._minValues[r][c];
        }
      }
    }

    // copying list
    _head = new Node<K, V>(rhs._head->_key, rhs._head->_value, nullptr);
    Node<K, V> *currentListNode = _head;

    Node<K, V> *tempRHSNode = rhs._head->_next;
    while (tempRHSNode != NULL)
    {

      Node<K, V> *newNode = new Node<K, V>(tempRHSNode->_key, tempRHSNode->_value, nullptr);
      currentListNode->_next = newNode;
      currentListNode = newNode;

      tempRHSNode = tempRHSNode->_next;
    }

    _tail = currentListNode;
  }
  else
  {

    _head = NULL;
    _tail = NULL;
    _listSize = 0;

    _minValueSize = 0;
    _updatedVal = false;
  }
}

template <class K, class V>
const RMQList<K, V> &RMQList<K, V>::operator=(const RMQList<K, V> &rhs)
{

  // if rhs is empty
  if (!rhs.empty())
  {

    // checks self-assignment
    if (this != &rhs)
    {

      // if memory is already allocated
      if (this->_head)
      {

        // deallocates
        clear();
      }

      _listSize = rhs._listSize;
      _minValueSize = rhs._minValueSize;
      _updatedVal = rhs._updatedVal;

      // copies rhs minValue if not empty
      if (rhs._minValueSize != 0)
      {

        // allocating memory for _minVlaues
        _minValues = new V *[_minValueSize];

        for (int i = 0; i < _minValueSize; i++)
        {

          _minValues[i] = new V[_minValueSize];
        }

        // deep copies minValues
        for (int r = 0; r < rhs._minValueSize; r++)
        {

          for (int c = r; c < rhs._minValueSize; c++)
          {

            _minValues[r][c] = rhs._minValues[r][c];
          }
        }
      }

      // copying list
      _head = new Node<K, V>(rhs._head->_key, rhs._head->_value, nullptr);
      Node<K, V> *currentListNode = _head;

      Node<K, V> *tempRHSNode = rhs._head->_next;
      while (tempRHSNode != NULL)
      {

        Node<K, V> *newNode = new Node<K, V>(tempRHSNode->_key, tempRHSNode->_value, nullptr);
        currentListNode->_next = newNode;
        currentListNode = newNode;

        tempRHSNode = tempRHSNode->_next;
      }

      _tail = currentListNode;
    }
  }

  return *this;
}

template <class K, class V>
bool RMQList<K, V>::insert(const K &key, const V &value)
{

  Node<K, V> *newNode = new Node<K, V>(key, value);

  // check if list is empty
  if (empty())
  {

    _head = newNode;
    _tail = newNode;
    _listSize++;

    _updatedVal = true;

    return true;
  }

  // checks last key to insert
  if (key > _tail->_key)
  {

    _tail->_next = newNode;
    _tail = newNode;
    _listSize++;

    _updatedVal = true;
    return true;
  }

  // checks rest of list to insert
  Node<K, V> *currentNode = _head, *prev = _head;
  while (currentNode != NULL)
  {

    if (key < currentNode->_key)
    {

      newNode->_next = currentNode;
      _listSize++;
      _updatedVal = true;

      if (currentNode == _head)
      {
        _head = newNode;
      }
      else
      {
        prev->_next = newNode;
      }

      return true;
    }
    else if (key == currentNode->_key)
    {

      // key found and node is invalid
      delete newNode;
      return false;
    }

    prev = currentNode;
    currentNode = currentNode->_next;
  }

  // key position not found, failed to insert
  delete newNode;
  return false;
}

template <class K, class V>
bool RMQList<K, V>::remove(const K &key)
{

  Node<K, V> *current = _head;
  Node<K, V> *prev = _head;

  while (current != nullptr)
  {

    // if key is found
    if (key == current->_key)
    {

      // if there is only one node
      if (_listSize == 1)
      {

        _head = nullptr;
        _tail = nullptr;
      }
      // if head needs to be removed
      else if (current == _head)
      {

        _head = _head->_next;
      }
      // if tail needs to be removed
      else if (current == _tail)
      {

        _tail = prev;
        _tail->_next = nullptr;
      }
      // links around node
      else
      {

        prev->_next = current->_next;
      }

      delete current;
      _listSize--;

      _updatedVal = true;
      return true;
    }

    prev = current;
    current = current->_next;
  }

  return false;
}

template <class K, class V>
bool RMQList<K, V>::update(const K &key, const V &value)
{

  Node<K, V> *current = _head;

  while (current != NULL)
  {

    // founds key thats needs to be updated
    if (key == current->_key)
    {

      // updates value
      current->_value = value;

      _updatedVal = true;
      return true;
    }

    current = current->_next;
  }

  return false;
}

template <class K, class V>
V RMQList<K, V>::query(const K &k1, const K &k2)
{

  // empty list
  if (_listSize == 0)
  {

    throw range_error("Empty List");
  }

  // gets index of keys to access _minValues
  int indxK1 = getIndex(k1);
  int indxK2 = getIndex(k2);

  // checks if keys are valid
  if (indxK1 == -1 || indxK2 == -1)
  {

    throw invalid_argument("Out Of Range");
  }
  else if (indxK1 > indxK2)
  {

    throw invalid_argument("Out Of Range(k1 > k2)");
  }

  // minValue needs to be updated
  if (_updatedVal)
  {

    updateMinValue();
    _updatedVal = false;
  }

  return _minValues[indxK1][indxK2];
}

template <class K, class V>
void RMQList<K, V>::dumpList() const
{

  Node<K, V> *current = _head;

  // while not at the end of the list
  while (current != NULL)
  {

    // print node
    cout << *current << endl;

    current = current->_next;
  }

  cout << endl;
}

template <class K, class V>
void RMQList<K, V>::dumpTable() const
{

  // if there are no minvalue
  if (_minValueSize != 0)
  {

    cout << "(ROW,COL): min" << endl;

    // prints minvalues (row, col): minimum value
    for (int r = 0; r < _minValueSize; r++)
    {

      for (int c = r; c < _minValueSize; c++)
      {

        cout << "(" << r << "," << c << "): " << _minValues[r][c] << " ";
      }

      cout << endl;
    }

    cout << "\nRaw Table: " << endl;

    // prints all contents of _minValue
    for (int r = 0; r < _minValueSize; r++)
    {

      for (int c = 0; c < _minValueSize; c++)
      {

        if (c >= r)
        {

          cout << _minValues[r][c] << " ";
        }
        else
        {

          cout << "- ";
        }
      }

      cout << endl;
    }
  }
}

template <class K, class V>
void RMQList<K, V>::clear()
{

  Node<K, V> *temp;

  // deletes list
  while (_head != NULL)
  {
    temp = _head;
    _head = _head->_next;

    delete temp;
  }

  // if minValue is not empty
  if (_minValueSize != 0)
  {

    // deletes _minValues
    for (int i = 0; i < _minValueSize; i++)
    {

      delete[] _minValues[i];
    }

    delete[] _minValues;
  }

  // resets values
  _head = NULL;
  _tail = NULL;
  _listSize = 0;
  _minValueSize = 0;
  _updatedVal = false;
}

template <class K, class V>
void RMQList<K, V>::updateMinValue()
{

  // deletes previously computed values
  if (_minValueSize > 0)
  {

    for (int i = 0; i < _minValueSize; i++)
    {
      delete[] _minValues[i];
    }

    delete[] _minValues;
  }

  // allocates memory for minValues
  _minValueSize = _listSize;
  _minValues = new V *[_listSize];

  for (int i = 0; i < _listSize; i++)
  {

    _minValues[i] = new V[_listSize];
  }

  Node<K, V> *current = _head, *compare = _head;
  V min;

  for (int r = 0; r < _listSize; r++)
  {
    
    _minValues[r][r] = current->_value;
    compare = current->_next;

    for (int c = r + 1; c < _listSize; c++)
    {

      //comparing value to previous min
      min = MIN(_minValues[r][c - 1], compare->_value);
      compare = compare->_next;

      // inserts minvalue
      _minValues[r][c] = min;
    }

    current = current->_next;
  }
}

template <class K, class V>
int RMQList<K, V>::getIndex(K key)
{

  Node<K, V> *current = _head;
  int i = 0;

  while (current != NULL)
  {

    // key is found returns index i
    if (key == current->_key)
      return i;

    i++;
    current = current->_next;
  }

  // key was not found
  return -1;
}

#endif
