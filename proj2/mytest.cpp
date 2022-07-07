// CMSC 341 - Spring 2020 - Project 2
// Author: Muhammed Muktar
// File: mytest.cpp
//
//A test file for proj2


#include <iostream>
#include <vector>

#include "rmqlist.h"

using namespace std;

int main(){


  RMQList<int,int> rmqA;

  //empty test
  RMQList<int,int> emptyConstructor(rmqA);
  RMQList<int,int> emptyAssignment;

  
  emptyAssignment= rmqA;

  cout << "Empty List Info(Constructor):" << endl;
  emptyConstructor.dumpList();

  cout << endl;
  
  cout << "Empty List Info(Assignment):" << endl;
  emptyAssignment.dumpList();
 
  cout << endl;
  
  //inserts check
  if (rmqA.insert(0, 12))
    cout << "Insert (0,12) in rmqA" << endl;
  
  if (rmqA.insert(2, -5))
    cout << "Insert (2,-5) in rmqA" << endl;
  
  if (rmqA.insert(2, 20))
    cout << "Insert (2, 20) in rmqA" << endl;
  else
    cout << "Did not insert (2, 20) in rmqA" << endl;
    
  if (rmqA.insert(3, -14))
    cout << "Insert (3, -14) in rmqA" << endl;
  
  
  if (rmqA.insert(-1, 12))
    cout << "Insert (-1, 12) in rmqA" << endl;

  cout << endl;
  
  cout << "rmq A List Info:" << endl;
  rmqA.dumpList();

  cout << endl;

  
  RMQList<int,int> rmqB(rmqA);

  cout << "Remove key 0 & 2. Insert (-10,10)" << endl;

  //remove check
  if (rmqB.remove(0))
    cout << "Removed key 0 in rmqB" << endl;
  if (rmqB.remove(2))
    cout << "Removed key 2 in rmqB" << endl;

  if (rmqB.insert(-10,10))
    cout << "Insert (-10,10) in rmqB" << endl;
    
  cout << endl;
  
  cout << "rmq B List Info(Copy Constructor):" << endl;
  rmqB.dumpList();

  cout << endl;

  
  //assignment operator
  RMQList<int,int> rmqC;
  rmqC = rmqA;
  
  //self assignment test
  rmqC = rmqC;
  
  //update test
  if (!rmqC.update(30, 1))
    cout << "Did not update key 30 in rmqC" << endl;

  rmqC.update(0, -54);
  rmqC.update(2, -11);
  rmqC.update(3, 3);  
  
  rmqC.insert(1, 2);
  
  cout << "rmq C List Info(Assignment):" << endl;
  rmqC.dumpList();
  
  
  cout << endl;

  //deep copy check
  cout << "rmq A List Info(Deep Copy Check):" << endl;
  rmqA.dumpList();

  cout << endl;

  //double template test
  RMQList<double,double> rmqD;
  
  rmqD.insert(2.3, 8.2);
  rmqD.insert(-5.7, 3.4);
  rmqD.insert(4.1, 10.2);
  rmqD.insert(19.2, -7.2);
  rmqD.insert(-18.5, -2.1);
  
  cout << "rmq D List Info(Double Template Check):" << endl;
  rmqD.dumpList();

  //query test
  cout << "rmq A List Info:" << endl;
  rmqA.dumpList();

  cout << endl;
  
  cout << "Min from 0 to 3: " << rmqA.query(0,3) << endl;

  rmqA.dumpTable();

  cout << "Insert (-21, -80)" << endl; 
  rmqA.insert(-21, -80);

  cout << "Min from 0 to 3: " << rmqA.query(0,3) << endl;

  cout << "Remove 0" << endl;
  rmqA.remove(0);

  cout << "Min from -21 to 3: " << rmqA.query(-21,3) << endl;

  cout << "Update (3, -81)" << endl;
  rmqA.update(3, -81);

  cout << "Min from -21 to 3: " << rmqA.query(-21,3) << endl;

  cout << endl;
  
  rmqA.dumpTable();

  cout << endl;


  //copy constructor query test
  cout << "rmq B List Info:" << endl;
  rmqB.dumpList();

  cout << "Remove 3" << endl;
  rmqB.remove(3);

  cout << "Min from -10 to -1: " << rmqB.query(-10,-1) << endl;
  
  cout << endl;

  rmqB.dumpTable();

  cout << endl;

  //assignement opperator query test
  cout << "rmq C List Info:" << endl;
  rmqC.dumpList();

  cout << "Insert (-2,0)" << endl;
  rmqC.insert(-2, 0);

  cout << "Min from -2 to 3: " << rmqC.query(-2,3) << endl;

  cout << endl;

  rmqC.dumpTable();

  cout << endl;


  //clear test
  rmqA.clear();

  cout << "Cleared RMQ A" << endl;
  cout << "rmq A List Info(Assignment):" << endl;
  rmqA.dumpList();
  
  RMQList<int,int> rmqQueryA;


  
  int key = -100, value = 100;

  //insert query
  for (int i = 0; i <= 2000; i++){

    rmqQueryA.insert(key,value);

    key += 1;
    value -= 1;
    
  }

  int key1 = -100;
  int key2 = key - 1;

  cout << "Query Test: " << endl;
  
  //query test
  for (int i = 0; i <= 1000; i++){

    if (key1 > key2) {
      cout << "Min from (" << key1 << "," << key2 << "): "
	   << rmqQueryA.query(key2,key1) << endl;
    }
    else{
      
      cout << "Min from (" << key1 << "," << key2 << "): "
         << rmqQueryA.query(key1,key2) << endl;

    }
    key1 += 1;
    key2 -= 1;
    
    
  }
  
}

