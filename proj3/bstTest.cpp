#include "BST.h"

bool imbalfn(int leftHeight, int rightHeight, int leftSize, int rightSize)
{
  return (leftHeight > rightHeight + 1) || (rightHeight > leftHeight + 1);
}

void print(BST tree)
{

  tree.dump();

  cout << "size = " << tree.size() << endl;
  cout << "height = " << tree.height() << endl;

  cout << endl;
}

int main()
{

  // test empty
  cout << "Empty Test: " << endl;
  BST emptyBST(imbalfn);
  BST emptyCopyCon(emptyBST);
  BST emptyAssign = emptyBST;

  print(emptyBST);

  cout << "Normal Test:" << endl;

  // make a normal bst
  BST bstA(imbalfn);

  bstA.insert("A", 5);
  bstA.insert("B", 10);
  bstA.insert("C", 15);
  bstA.insert("D", 20);
  bstA.insert("E", 30);
  bstA.insert("F", 40);
  bstA.insert("G", 55);
  bstA.insert("H", 60);
  bstA.insert("I", 65);
  bstA.insert("J", 70);
  bstA.insert("K", 75);
  bstA.insert("L", 80);

  print(bstA);

  // copy sontructor test
  cout << "Copy Constructor:" << endl;
  BST bstB(bstA);
  bstB.insert("@", 11);

  print(bstB);

  // assignment operator test
  cout << "Assignment Operator:" << endl;
  BST bstC(imbalfn);
  bstC = bstC;
  bstC = bstA;

  bstC.insert("^", 74);

  print(bstC);

  // check for deep copy
  cout << "Check Deep Copy:" << endl;
  print(bstA);

  return 0;
}
