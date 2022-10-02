// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"

using std::cout;
using std::domain_error;
using std::endl;

JQueue::JQueue(prifn_t priFn)
{

  _heap = nullptr;
  _size = 0;
  priority = priFn;
}

JQueue::~JQueue()
{

  // if heap is not empty
  if (_heap != nullptr)
    clear();
}

JQueue::JQueue(const JQueue &rhs)
{

  // if heap is not empty
  if (rhs._heap != nullptr)
  {

    _heap = nullptr;
    _size = rhs._size;
    priority = rhs.priority;

    // copies heap
    copyHeap(rhs._heap, _heap, true);
  }
  else
  {

    _heap = nullptr;
    _size = 0;
    priority = rhs.priority;
  }
}

JQueue &JQueue::operator=(const JQueue &rhs)
{

  // if heap is not empty
  if (rhs._heap != nullptr)
  {

    // checks self assignemnt
    if (this != &rhs)
    {

      // checks for allocated memory
      if (this->_heap)
      {

        clear();
      }

      _heap = nullptr;
      _size = rhs._size;
      priority = rhs.priority;

      copyHeap(rhs._heap, _heap, true);
    }
  }
  else
  {

    _heap = nullptr;
    _size = 0;
    priority = rhs.priority;
  }

  return *this;
}

void JQueue::insertJob(const job_t &input)
{

  // makes new node with job input
  Node *newNode = new Node(input);

  // inserts in to heap
  _heap = merge(_heap, newNode);
  _size++;
}

job_t JQueue::getNextJob()
{

  // heap is empty
  if (_heap == nullptr)
    throw domain_error("Domain Error!");

  // copy job
  job_t getJob;
  getJob = _heap->_job;

  Node *temp = _heap;

  // merges left and right nodes to make new heap
  _heap = merge(_heap->_left, _heap->_right);
  _size--;

  // delete prev root
  delete temp;

  return getJob;
}

void JQueue::mergeWithQueue(JQueue &rhs)
{

  // if priority does not match
  if (priority != rhs.getPriorityFn())
    throw domain_error("Domain Error!");

  // merges heap with rhs
  _heap = merge(_heap, rhs._heap);
  _size += rhs.numJobs();

  // disconnects rhs to heap (basically emptying rhs)
  rhs._heap = nullptr;
  rhs._size = 0;
}

void JQueue::clear()
{

  // delte heap
  deleteHeap(_heap);
  _heap = nullptr;
  _size = 0;
}

int JQueue::numJobs() const
{

  return _size;
}

void JQueue::printJobQueue() const
{

  // prints job queue info
  printInfo(_heap);
}

prifn_t JQueue::getPriorityFn() const
{

  return priority;
}

void JQueue::setPriorityFn(prifn_t priFn)
{

  // sets new priority
  priority = priFn;

  // heap is not empty
  if (_heap != nullptr)
  {

    // makes array of skew heap
    Node *skewArray[_size];
    makeArray(_heap, skewArray, 0);

    // creates new heap
    _heap = skewArray[0];

    for (int i = 1; i < _size; i++)
    {

      _heap = merge(_heap, skewArray[i]);
    }
  }
}

void JQueue::dump() const
{

  // prints heap priority relationship
  printDump(_heap);
  cout << endl;
}

ostream &operator<<(ostream &sout, const job_t &job)
{
  sout << "Job: " << job._name << ", pri: " << job._priority
       << ", u: " << job._user << ", g: " << job._group << ", proc: "
       << job._proc << ", mem: " << job._mem << ", time: " << job._time;
  return sout;
}

ostream &operator<<(ostream &sout, const Node &node)
{
  sout << node._job;
  return sout;
}

void JQueue::copyHeap(Node *node, Node *prevNode, bool insertLeft)
{

  // node is null
  if (node == nullptr)
    return;

  // new node with job
  Node *newNode = new Node(node->_job);

  // tree is emtpy
  if (_heap == nullptr)
  {
    _heap = newNode;
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

  copyHeap(node->_left, newNode, true);
  copyHeap(node->_right, newNode, false);
}

void JQueue::deleteHeap(Node *node)
{

  // deletes tree
  if (node == nullptr)
    return;

  deleteHeap(node->_left);
  deleteHeap(node->_right);
  delete node;
}

Node *JQueue::merge(Node *tree1, Node *tree2)
{

  // tree 1 is null
  if (tree1 == nullptr)
    return tree2;

  // tree 2 is null
  else if (tree2 == nullptr)
    return tree1;

  Node *root, *rightNode;

  // priority of tree1 is greater than tree2
  if (priority(tree1->_job) > priority(tree2->_job))
  {

    // swap
    rightNode = tree1->_right;
    tree1->_right = tree1->_left;
    tree1->_left = rightNode;

    root = tree1;

    // merges left
    root->_left = merge(tree2, rightNode);
  }
  // priority of tree2 is greater than or equal to tree1
  else
  {

    // swap
    rightNode = tree2->_right;
    tree2->_right = tree2->_left;
    tree2->_left = rightNode;

    root = tree2;

    // merge left
    root->_left = merge(tree1, rightNode);
  }

  return root;
}

void JQueue::printDump(Node *node) const
{

  if (node == nullptr)
  {
    return;
  }

  cout << "(";

  printDump(node->_left);

  // prints priority
  cout << priority(node->_job);

  printDump(node->_right);

  cout << ")";
}

void JQueue::printInfo(Node *node) const
{

  if (node == nullptr)
  {
    return;
  }

  // prints node job info
  cout << "[" << priority(node->_job) << "] " << *node << endl;

  printInfo(node->_left);
  printInfo(node->_right);
}

int JQueue::makeArray(Node *node, Node *array[], int index)
{

  if (node == nullptr)
  {
    return index;
  }

  // pre order traversal
  // adds node to array
  array[index] = node;
  index++;

  // move left
  index = makeArray(node->_left, array, index);

  // move right
  index = makeArray(node->_right, array, index);

  node->_left = nullptr;
  node->_right = nullptr;

  return index;
}
