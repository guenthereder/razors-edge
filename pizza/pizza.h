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
#include <limits>

using namespace std;

#define M 'M'
#define T 'T'
#define N 'X'

/* T...Tomato, M...Mushroom, N...Taken */
using Row = vector<char>;
using Pizza = vector<Row>;

/* slice format (x1,y1,x2,y2), start-end of solution slice */
using Slice    = array<int, 4>;
using Solution = vector<Slice>;

class Data {
public:
   Data(int r, int c, int minI, int maxC, ifstream *infile):
      rows(r),columns(c),minIngrediant(minI),maxCells(maxC) {
         minCells = (2 * minIngrediant);
         pizza = new Pizza();
         solution = new Solution();
         
         readPizzaInput(infile);
         //printPizza();

         pizzaCopy = new Pizza(*pizza);
   }
   
   ~Data() {
      delete pizza;
      delete solution;
   }
   
   void runGreedy();
   
private:
   /* algorithmic */
   bool generateSlice(int x, int y, Slice *slice);
   void removeSliceFromPizza(Slice *slice);

   void transposePizzaFromCopy();
   bool isValidCell(int x, int y);
   bool isTomato(int x, int y);
   bool isMushroom(int x, int y);
   
   /* read input */
   void readPizzaInput(ifstream *infile);

   /* output, debug */
   void printPizza();
   void printSolution();
   void printSlice(Slice slice);

   int rows, columns, minIngrediant, maxCells, minCells;
   
   Pizza *pizza, *pizzaCopy;
   Solution *solution, *solutionCopy;
};

#endif
