// CMSC 341 - Spring 2020 - Project 4
// mytest.cpp: test file for JQueue class.

#include "jqueue.h"
using namespace std;

// Priority functions
int priorityFn1(const job_t &job);
int priorityFn2(const job_t &job);

void printHeap(JQueue heap)
{

  // prints skew heap information
  cout << endl;
  heap.printJobQueue();
  cout << "Dump the queue" << endl;
  heap.dump();

  cout << "Number of jobs: " << heap.numJobs() << endl;
}

int main()
{

  // empty test
  JQueue emptyQueue(priorityFn1);
  JQueue emptyCopyCon(emptyQueue);
  JQueue emptyAssign = emptyQueue;

  cout << "Display empty queue: " << endl;
  printHeap(emptyQueue);

  cout << "Display empty copy constructor queue: " << endl;
  printHeap(emptyQueue);

  cout << "Display empty assignment operator queue: " << endl;
  printHeap(emptyQueue);

  JQueue heapA(priorityFn1);

  // creates jobs
  job_t testJob1("test1", 50, 3, 11, 16, 1024, 120);
  job_t testJob2("test2", 50, 7, 5, 128, 512, 300);
  job_t testJob3("test3", 20, 3, 0, 128, 1024, 1800);
  job_t testJob4("test1a", 50, 3, 11, 16, 1024, 120);

  heapA.insertJob(testJob3);
  heapA.insertJob(testJob1);
  heapA.insertJob(testJob2);
  heapA.insertJob(testJob4);

  // Print the queue
  cout << "\nDisplay Normal Heap:" << endl;
  printHeap(heapA);

  cout << "\nCopy Constructor Test: " << endl;

  // copy constructor  test
  JQueue heapB(heapA);

  // makes jobs for copy constructor
  job_t makeJob1("CC job test1", 30, 4, 12, 120, 100, 199);
  job_t makeJob2("CC job test2", 40, 5, 45, 55, 450, 723);
  job_t makeJob3("CC job test3", 23, 7, 23, 34, 1700, 129);

  heapB.insertJob(makeJob1);
  heapB.insertJob(makeJob2);
  heapB.insertJob(makeJob3);

  cout << "\nDisplay Copy Constructor:" << endl;
  printHeap(heapB);

  cout << "\nAssignment Operator Test: " << endl;

  // assignment operator test
  JQueue heapC(priorityFn1);

  // self assignment test
  heapC = heapC;

  heapC = heapA;

  // make jobs for assignment operator
  job_t makeJob4("AO job test1", 60, 3, 2, 60, 170, 99);
  job_t makeJob5("AO job test2", 45, 5, 16, 234, 435, 657);
  job_t makeJob6("AO job test3", 34, 12, 8, 54, 776, 744);

  heapC.insertJob(makeJob4);
  heapC.insertJob(makeJob5);
  heapC.insertJob(makeJob6);

  cout << "\nDisplay Assignment Operator:" << endl;
  printHeap(heapC);

  // check deep copy
  cout << "\nDisplay Normal Heap (Deep Copy Check):" << endl;
  printHeap(heapA);

  // merge test
  JQueue merge1(priorityFn1);
  JQueue merge2(priorityFn2);

  // make merge1
  merge1.insertJob(testJob1);
  merge1.insertJob(testJob2);

  // make merge2
  merge2.insertJob(testJob3);
  merge2.insertJob(testJob4);

  // throws domain error since priority is not the same
  // merge1.mergeWithQueue(merge2);

  // changes priority
  merge2.setPriorityFn(priorityFn1);
  merge1.mergeWithQueue(merge2);

  cout << "\nDisplay Merge1:" << endl;
  printHeap(merge1);

  cout << "\nDisplay Merge2 (Should be empty):" << endl;
  printHeap(merge2);

  return 0;
}

int priorityFn1(const job_t &job)
{

  float pri;
  // 100 (regular user) + 100 (regular group) + job priority
  pri = 200 + job._priority;

  // If special user (7 or 23) add 200
  if (job._user == 7 || job._user == 23)
    pri += 200;

  // If special group (0 or 11) add 100
  if (job._group == 0 || job._group == 11)
    pri += 100;

  // Subtract weighted proc, mem, and time
  //   1.0 * proc, 0.01 * mem, (1/3600) * time
  pri = pri - job._proc - 0.01 * job._mem - (1.0 / 3600.0) * job._time;

  return int(pri);
}

int priorityFn2(const job_t &job)
{
  return int(job._time);
}
