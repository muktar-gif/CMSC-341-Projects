// CMSC 341 - Spring 2020 - Project 1
//
// File: mytest.cpp
//
// A test file for proj1

#include <iostream>
#include <tuple>
#include "Graph.h"
using namespace std;

void printInfo(EntryList);
void printItr(EntryList);
void printNb(Graph);
void printEg(Graph);

void printInfo(EntryList list)
{

  cout << "Size:" << list.size() << endl;
  cout << "Capacity: " << list.capacity() << endl;
}

void printItr(EntryList list)
{

  for (auto itr = list.begin(); itr != list.end(); itr++)
  {

    cout << *itr << endl;
  }
}

void printNb(Graph g)
{

  Graph::NbIterator nit;

  for (int i = 0; i < g.numVert(); i++)
  {

    cout << "\nThe neighbors of vertex " << i << " are:\n";

    for (nit = g.nbBegin(i); nit != g.nbEnd(i); nit++)
    {
      cout << "(" << (*nit).first << ", " << (*nit).second << ")"
           << " ";
    }
    cout << endl;
  }
}

void printEg(Graph g)
{

  Graph::EgIterator eit;
  std::tuple<int, int, int> edge;

  cout << "\nnumVert = " << g.numVert() << ", numEdge = " << g.numEdge() << endl;
  cout << "The edges in the graph are:\n";

  for (eit = g.egBegin(); eit != g.egEnd(); eit++)
  {

    edge = *eit; // get current edge

    // the two data members of a pair are first and second
    cout << "(" << get<0>(edge) << ", "
         << get<1>(edge) << ", "
         << get<2>(edge) << ") ";
  }
  cout << endl;
}

int main()
{

  cout
      << "\n||||||||||||||||||||||||||||||||EntryList Test"
      << "||||||||||||||||||||||||||||||||||\n"
      << endl;

  EntryList listA;
  EntryList emptyList;
  EntryList::Entry copyEntry;

  // empty list test
  EntryList emptyListConst(emptyList);
  EntryList emptyListAssign = emptyList;

  // shows empty list
  cout << "Empty List(listA) Info: " << endl;
  printInfo(listA);

  // populates list, insert test
  for (int i = 0; i < 10; i++)
  {
    listA.insert(EntryList::Entry(i, i + 2));
  }

  cout << endl;

  cout << "listA Info:" << endl;
  printInfo(listA);

  // update test
  for (int i = 0; i < 9; i += 2)
  {
    listA.update(EntryList::Entry(i, i + 1));
  }

  cout << endl;

  cout << "listA Info --Updated:" << endl;
  printInfo(listA);

  // fills up list, capacity doubles

  listA.insert(EntryList::Entry(10, 12));

  cout << "listA Info --Filled(double size):" << endl;
  printInfo(listA);

  // removes more then 3/4 of list, capacity should half;
  for (int i = 0; i < 8; i++)
  {

    if (listA.remove(i, copyEntry))
    {
      cout << "Removed from listA" << endl;
      cout << "Vertex:" << copyEntry.getVertex()
           << " Weight:" << copyEntry.getWeight() << endl;
    }
  }

  cout << endl;

  cout << "listA Info --1/4 Filled(half size):" << endl;
  printInfo(listA);

  cout << endl;

  cout << "listA Info --Entries Order/Duplicate Check:" << endl;
  printInfo(listA);

  EntryList listB(listA);

  cout << endl;

  cout << "listB Info --Copy Constructor:" << endl;
  printInfo(listB);

  listB.remove(8, copyEntry);

  cout << endl;

  cout << "listB Info --Updated:" << endl;
  printInfo(listB);

  EntryList listC;

  // self assignment test
  listC = listC;

  listC = listA;

  cout << endl;

  cout << "listC Info --Assignment Operator:" << endl;
  printInfo(listC);

  listC.insert(EntryList::Entry(7, 2));

  cout << endl;

  cout << "listC Info --Updated:" << endl;
  printInfo(listC);

  cout << endl;

  // deep copy check
  cout << "listA Info --DeepCopy Check(should be different from List A and B):" << endl;
  printInfo(listA);

  cout << endl;

  // Iterator test
  cout << "List A Iterator Print" << endl;
  printItr(listA);

  cout << endl;

  cout << "List C Iterator Print" << endl;
  printItr(listC);

  cout << endl;

  cout << "List B Iterator Print" << endl;
  printItr(listB);

  cout << endl;

  // empty iterator test
  // cout << "Empty Iterator Print" << endl;
  // printItr(emptyList);

  cout
      << "\n||||||||||||||||||||||||||||||||Graph test"
      << "||||||||||||||||||||||||||||||||||\n"
      << endl;

  Graph graphA(10);

  // empty constructor/assignment test
  Graph emptyGraphConst(graphA);
  Graph emptyGraphAssign = graphA;

  // add some edges
  graphA.addEdge(0, 1, 14);
  graphA.addEdge(0, 2, 22);
  graphA.addEdge(1, 3, 5);
  graphA.addEdge(1, 6, 16);
  graphA.addEdge(3, 4, 5);
  graphA.addEdge(3, 6, 13);
  graphA.addEdge(4, 7, 31);
  graphA.addEdge(4, 0, 16);
  graphA.addEdge(5, 1, 5);
  graphA.addEdge(5, 2, 13);
  graphA.addEdge(6, 5, 31);

  // dump the graph
  cout << "GraphA Info:" << endl;
  graphA.dump();

  // print neighbor iterator
  printNb(graphA);

  // print edge iterator
  // printEg(graphA);

  cout << "he" << endl;
  Graph::EgIterator eit;
  std::tuple<int, int, int> edge;

  cout << "\nnumVert = " << graphA.numVert() << ", numEdge = " << graphA.numEdge() << endl;
  cout << "The edges in the graph are:\n";

  for (eit = graphA.egBegin(); eit != graphA.egEnd(); eit++)
  {

    edge = *eit; // get current edge

    // the two data members of a pair are first and second
    cout << "(" << get<0>(edge) << ", "
         << get<1>(edge) << ", "
         << get<2>(edge) << ") ";
  }
  cout << endl;

  // remove edges
  cout << "Remove edges (2,22) and (1,14)" << endl;
  graphA.removeEdge(0, 2);
  graphA.removeEdge(0, 1);

  cout << endl;

  // print edge iterator
  cout << "New Graph A(Removed Edge):" << endl;
  printEg(graphA);

  // copy constructor test
  Graph graphB(graphA);

  graphB.addEdge(7, 4, 12);

  cout << endl;

  cout << "GraphB Info(Copy Constructor):" << endl;
  graphB.dump();

  // print neighbor iterator
  printNb(graphB);

  // print edge iterator
  printEg(graphB);

  // assignment opperator test;
  Graph graphC = graphA;

  // self assignment test
  graphC = graphC;

  graphC = graphA;

  graphC.addEdge(7, 5, 45);

  cout << endl;

  cout << "GraphC Info(Assignment Opperator):" << endl;
  graphC.dump();

  // print neighbor iterator
  printNb(graphC);

  // print edge iterator
  printEg(graphC);

  cout << endl;

  cout << "GraphA Info(Deep Copy Check):" << endl;
  graphA.dump();

  // print neighbor iterator
  printNb(graphA);

  // print edge iterator
  printEg(graphA);

  // invalid test
  // Graph testInvalid(0);
}
