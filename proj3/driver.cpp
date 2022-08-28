// CMSC 341 - Spring 2020 - Project 3

#include <vector>
#include "Scanner.h"

using namespace std;

int main()
{

  // Characters used in the ASCII art, in order.
  vector<string> chars = {"█", "░"};

  // Scanner with 15 lines, 34 characters per line, 2 characters

  Scanner s(15, 34, chars);

  // Read data files
  if (!s.loadFiles("scrambled.txt", "weights.txt"))
  {
    cerr << "Error reading data files." << endl;
  }
  else
  {
    s.dump();
    s.inorder();
  }

  return 0;
}
