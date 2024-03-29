// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#ifndef _JQUEUE_H
#define _JQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>

using std::ostream;
using std::out_of_range;
using std::string;

// Job scheduling parameters, min and max values

const int MINPRI = 0; // Min and max priority
const int MAXPRI = 100;

const int MINU = 0; // Min and max user id
const int MAXU = 100;

const int MING = 0; // Min and max group id
const int MAXG = 100;

const int MINPROC = 1; // Min and max number of procs
const int MAXPROC = 255;

const int MINMEM = 1;     // Min and max mem
const int MAXMEM = 65535; // Multiple of 1 Mb

const int MINTIME = 1;      // Min and max wall clock time in seconds
const int MAXTIME = 172800; // 48 hours

//
// job_struct: Simple job struct with default and non-default
// constructors
//
struct job_struct
{

  // Default job constructor
  job_struct()
  {
    _name = "";
    _priority = 0;
    _user = 0;
    _group = 0;
    _proc = 0;
    _mem = 0;
    _time = 0;
  }

  // Non-default job constructor
  job_struct(string name, int pri, int u, int g, int proc, int mem, int time)
  {
    if ((pri < MINPRI || pri > MAXPRI) || (u < MINU || u > MAXU) || (g < MING || g > MAXG) ||
        (proc < MINPROC || proc > MAXPROC) || (mem < MINMEM || mem > MAXMEM) ||
        (time < MINTIME || time > MAXTIME))
      throw out_of_range("Bad value to job constructor.");

    _name = name;
    _priority = pri;
    _user = u;
    _group = g;
    _proc = proc;
    _mem = mem;
    _time = time;
  }

  string _name;  // job name
  int _priority; // job priority, user selected
  int _user;     // user id
  int _group;    // group id
  int _proc;     // number of processors
  int _mem;      // memory per proc, multiples of 1Mb
  int _time;     // wall time, in sec
};

typedef struct job_struct job_t; // job type

struct Node
{
  job_t _job;   // job information
  Node *_right; // right child
  Node *_left;  // left child

  Node(job_t job)
  {
    _job = job;
    _right = nullptr;
    _left = nullptr;
  }
};

// Overloaded insertion operators for job_t and Node
ostream &operator<<(ostream &sout, const job_t &job);
ostream &operator<<(ostream &sout, const Node &node);

// Priority function pointer type
typedef int (*prifn_t)(const job_t &);

class Grader; // forward declaration (for grading purposes)

class JQueue
{
public:
  friend Grader; // for grading purposes

  // Constructor.  Create a job queue with given priority function.
  JQueue(prifn_t priFn);

  // Destructor
  ~JQueue();

  // Copy constructor
  JQueue(const JQueue &rhs);

  // Assignment Operator
  JQueue &operator=(const JQueue &rhs);

  // Add a job to the queue
  void insertJob(const job_t &job);

  // Extract the highest priority job
  job_t getNextJob();

  // Merge two job queues
  void mergeWithQueue(JQueue &rhs);

  // Remove all jobs from the queue
  void clear();

  // Return number of jobs in queue
  int numJobs() const;

  // Print the queue using preorder traversal.  Although the first job
  // printed should have the highest priority, the remaining jobs will
  // not necessarily be in priority order.
  void printJobQueue() const;

  // Get the current priority function.
  prifn_t getPriorityFn() const;

  // Set a new priority function.  Must rebuild the heap!!!
  void setPriorityFn(prifn_t priFn);

  // Dump the heap.  For debugging purposes.
  void dump() const;

private:
  Node *_heap; // Pointer to root of skew heap
  int _size;   // Current size of the heap

  // Function pointer; function to compute job priority
  prifn_t priority;

  /******************************************
   * Private function declarations go here! *
   ******************************************/

  // Desc:           recursively deletes root
  // Pre-condition:  root is not empty
  // Post-condition: root is deleted
  void deleteHeap(Node *root);

  // Desc:           deep copies given root to class _heap
  // Pre-condition:  given root is not empty
  // Post-condition: copies given root to _heap
  void copyHeap(Node *root, Node *prevNode, bool insertLeft);

  // Desc:           merges tree1 and tree2
  // Pre-condition:  N/A
  // Post-condition: tree1 and tree2 are merged
  Node *merge(Node *tree1, Node *tree2);

  // Desc:           prints heaps contents
  // Pre-condition:  N/A
  // Post-condition: heap priority is printed
  void printDump(Node *node) const;

  // Desc:           prints info sorted in heap
  // Pre-condition:  N/A
  // Post-condition: heap info is printed
  void printInfo(Node *node) const;

  // Desc:           stores given heap into an array
  // Pre-condition:  heap is not empty
  // Post-condition: array stores all heap nodes
  int makeArray(Node *node, Node *array[], int index);
};

#endif
