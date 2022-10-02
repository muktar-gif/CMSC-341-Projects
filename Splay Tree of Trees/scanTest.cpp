// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include "Scanner.h"

using namespace std;

int main()
{

  // Characters used in the ASCII art, in order.
  vector<string> chars = {"█", "░"};

  Scanner emptySplay(15, 20, chars);

  // empty test
  Scanner emptyConstructor(emptySplay);
  Scanner emptyAssignment = emptySplay;

  // checking initilizations for empty trees
  cout << "Printing Empty Constructor Test (prints nothing)" << endl;
  emptyConstructor.dump();
  emptyConstructor.inorder();

  cout << "Printing Empty Assignment Test (prints nothing)" << endl;
  emptyAssignment.dump();
  emptyAssignment.inorder();

  // Scanner with 15 lines, 34 characters per line, 2 characters
  Scanner splayA(15, 34, chars);

  // Read data files
  if (!splayA.loadFiles("scrambled.txt", "weights.txt"))
  {
    cerr << "Error reading data files." << endl;
  }
  else
  {
    cout << "Normal Test:" << endl;
    splayA.dump();
    splayA.inorder();
  }

  cout << endl;

  // copy constructor test
  cout << "Copy Constructor Test:" << endl;
  Scanner splayB(splayA);

  splayB.dump();
  splayB.inorder();
  cout << endl;

  // assignemnt operator test
  cout << "Assignment Operator Test:" << endl;
  Scanner splayC = splayA;
  // splayC = splayC;

  splayC.dump();
  splayC.inorder();
  cout << endl;

  Scanner incompleteImage(15, 20, chars);

  // Read data files
  if (!incompleteImage.loadFiles("scrambled.txt", "weights.txt"))
  {
    cerr << "Error reading data files." << endl;
  }

  cout << "Deep copy check: " << endl;
  cout << "SplayA: " << endl;

  splayA.dump();
  splayA.inorder();
  cout << endl;

  cout << "SplayA changed copy (should be an incomplete image): " << endl;

  // checking if data is deep copied
  Scanner deepCopyCheck = splayA;
  deepCopyCheck = incompleteImage;

  deepCopyCheck.dump();
  deepCopyCheck.inorder();
  cout << endl;

  // checking if splayA is the same
  cout << "SplayA: " << endl;

  splayA.dump();
  splayA.inorder();
  cout << endl;

  return 0;
}
