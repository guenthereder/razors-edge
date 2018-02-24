/**
 * team: razors edge
 * challenge: pizza
 * */

#include "pizza.h"

using namespace std;

void Data::runGreedy() {
   
}

void Data::printPizza() {
   for(auto row : pizza) {
      for(auto i : row) {
         cout << i;
      }
      cout << endl;
   }
}

void Data::greedySeach() {}

void Data::computePossibleSlices() {}

void Data::printPerformance() {
   cout << "Total Cells on Pizza: " << (rows * columns) << endl;
}

void Data::printSolution() {
   if(!solution.empty()) {
      cout << "Found Slices: ";
      for(auto slice : solution) {
         cout << "[(" << slice[0] << ","   << slice[1] << ")],[("
                      << slice[2] << "),(" << slice[3] << ")] " << endl;
      }
   }
}

/* This will get us the pizza in the array but in an (y,x) access fashion
 * instead of an (x,y) way, i.e., transposed. This can be ignored as our
 * solution must consider this 'transposed' variant as well.
 *
 * T - tomato, M - mushroom
 * */
void Data::readPizzaInput(ifstream *infile) {
   string line;
   
   while(getline(*infile, line)) {
      auto row = new Row();
      for(char r : line) {
         if(r == 'T') {
            row->push_back(T);
         } else if (r == 'M'){
            row->push_back(M);
         } else {
            cout << "Error reading file, found " << r << " in line!" << endl;
         }
      }
      pizza.push_back(*row);
   }
}

int main(int argc,  char** argv) {
   if (argc < 2) {
      cout << "usage: " << argv[0] << " <inputfile>" << endl;
      return 0;
   }

   string fileName = argv[1];
   ifstream infile(fileName);

   int rows, columns, minIngrediant, maxCells;
   infile >> rows >> columns >> minIngrediant >> maxCells;
   
   Data data(rows,columns,minIngrediant,maxCells,&infile);
  
   data.runGreedy();
   
   return 0;
}

