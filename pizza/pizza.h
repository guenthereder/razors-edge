/**
 * team: razors edge
 * challenge: pizza
 * */

#ifndef PIZZA_H
#define PIZZA_H

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

#define M 'M'
#define T 'T'

/* T...Tomato, M...Mushroom, N...Taken */
using Row = vector<char>;
using Pizza = vector<Row>;

/* slice format (x1,y1,x2,y2), start-end of solution slice */
using Slice = array<int, 4>;
using Solution = vector<Slice>;

class Data {
public:
   Data(int r, int c, int minI, int maxC, ifstream *infile):
      rows(r),columns(c),minIngrediant(minI),maxCells(maxC) {
         
         readPizzaInput(infile);
         printPizza();
   }
   
   ~Data() {
      printSolution();
      printPerformance();
   }
   
   void runGreedy();
   
private:
   void readPizzaInput(ifstream *infile);
   void greedySeach();
   void computePossibleSlices();

   void printPizza();
   void printPerformance();
   void printSolution();

   int rows, columns, minIngrediant, maxCells;
   
   Pizza pizza;
   Solution solution;
};

#endif
