/**
 * team: razors edge
 * challenge: pizza
 * */

#include "pizza.h"

using namespace std;

bool Data::isValidCell(int x, int y) {
   if(y > pizza->size()) {
      return false;
   } else if(x > pizza->at(y).size()) {
      return false;
   }
   if(pizza->at(y).at(x) != N) {
      return true;
   } else {
      return false;
   }
}

bool Data::isTomato(int x, int y) {
   if(!isValidCell(x,y)) {
      return false;
   }
   if(pizza->at(y).at(x) == T) {
      return true;
   } else {
      return false;
   }
}

bool Data::isMushroom(int x, int y) {
   if(!isValidCell(x,y)) {
      return false;
   }
   if(pizza->at(y).at(x) == M) {
      return true;
   } else {
      return false;
   }
}

void Data::runGreedy() {
   
   //cout << "max cells: " << maxCells << ", mincells: " << minCells << endl;
   
   for(int y=0; y < rows; ++y) {
      for(int x=0; x < columns; ++x) {
         int reduction = 0;
         Slice slice={0,0,0,0};
         if(isValidCell(x,y) && generateSlice(x,y,&slice)) {
            solution->push_back(slice);
            //printSlice(slice);
            removeSliceFromPizza(&slice);
            //printPizza();
         }
      }
   }
   
   solutionCopy = new Solution(*solution);
   //printSolution();
   
   int neg1 = 0;
   int neg2 = 0;
   for(int y=0; y < rows; ++y) {
      for(int x=0; x < columns; ++x) {
         if(isValidCell(x,y)) {
            ++neg1;
         }
      }
   }
   //cout << "max: " << columns*rows << ", achived: " << ((columns*rows)-neg1) << endl;
   
   /* we run the same algroithm on the transposed input */
   if(neg1 > 0) {

      transposePizzaFromCopy();

      for(int y=0; y < rows; ++y) {
         for(int x=0; x < columns; ++x) {
            int reduction = 0;
            Slice slice={0,0,0,0};
            if(isValidCell(x,y) && generateSlice(x,y,&slice)) {
               solution->push_back(slice);
               //printSlice(slice);
               removeSliceFromPizza(&slice);
               //printPizza();
            }
         }
      }

      //printSolution();

      for(int y=0; y < rows; ++y) {
         for(int x=0; x < columns; ++x) {
            if(isValidCell(x,y)) {
               ++neg2;
            }
         }
      }
      //cout << "max: " << columns*rows << ", achived: " << ((columns*rows)-neg2) << endl;
   }
   
   if(neg1 <= neg2) {
      solution = solutionCopy;
   }
   
   printSolution();
}

void Data::transposePizzaFromCopy() {
   delete pizza;
   delete solution;
   
   pizza = new Pizza();
   solution = new Solution();
   
   for(int i=0; i < columns; ++i) {
      auto r = new Row();
      pizza->push_back(*r);
   }
   
   for(int y=0; y < rows; ++y) {
      for(int x=0; x < columns; ++x) {
         (*pizza)[columns-x-1].push_back((*pizzaCopy)[y][x]);
      }
   }
   swap(columns,rows);
}

/**
 * we use a greedy kind of 'best' fit by starting with the largest not taken slice
 * and then try all 'shapes' of that size, then we reduce the size and try again
 * */
bool Data::generateSlice(int x, int y, Slice *slice) {
   int x_space = columns - x;
   int y_space = rows - y;
   
   if(x_space * y_space < minCells) {
      return false;
   }

   int max_x_slice = (x_space < maxCells) ? x_space : maxCells;
   int max_y_slice = (y_space < maxCells) ? y_space : maxCells;

   int sliceRows = 0;
   vector<int> possibleLengths = {};

   int min_col = max_x_slice;
   for(int row = 0; row < max_y_slice; ++row) {
      int cnt = 0;
      int loop_invariant = (max_x_slice/(row+1) == 0) ? 1 : max_x_slice/(row+1);
      
      while( ((row+1) * loop_invariant) <= maxCells ) { ++loop_invariant;}
      --loop_invariant;
      
      if(x+loop_invariant > columns) loop_invariant = columns-x;
      
      for(int column = 0; column < loop_invariant; ++column) {
         if(isValidCell(x+column,y+row)) {
            ++cnt;
         } else {
            /* set min_col */
            if(cnt < min_col) {
               min_col = cnt;
            }
            break;
         }
         /* min_col is the limit for the possible rectangle */
         if(column >= min_col) {
            break;
         }
      }
      possibleLengths.push_back(cnt);
   }

   if(possibleLengths.empty()) {
      return false;
   }
   
   //for(int i : possibleLengths) {cout << i << " ";}
   
   max_y_slice = possibleLengths.size();
   int minRowLength = possibleLengths[0];
   int currentRowsOfSlice = 1;

   /* slice would be too small */
   while( currentRowsOfSlice < possibleLengths.size() &&
         (possibleLengths[currentRowsOfSlice-1] * currentRowsOfSlice) < minCells) {
      ++currentRowsOfSlice;
   }
   
   int sliceWidth = (possibleLengths[currentRowsOfSlice-1] > max_x_slice/currentRowsOfSlice) ? max_x_slice/currentRowsOfSlice : possibleLengths[currentRowsOfSlice-1];
   
   if(sliceWidth == 0) {++sliceWidth;}

   while(currentRowsOfSlice * sliceWidth <= maxCells) {++currentRowsOfSlice;}
   --currentRowsOfSlice;

   if(currentRowsOfSlice > y_space) {
      currentRowsOfSlice = y_space;
   }
   
   do {
      int tomatoCnt     = 0;
      int mushroomCnt   = 0;
      bool isValidSlice = false;
      
      sliceWidth = (possibleLengths[currentRowsOfSlice-1] > max_x_slice/currentRowsOfSlice) ? max_x_slice/currentRowsOfSlice : possibleLengths[currentRowsOfSlice-1];
      
      while( (sliceWidth*currentRowsOfSlice <= maxCells) && sliceWidth <= possibleLengths[currentRowsOfSlice-1]) {
         ++sliceWidth;
      }
      --sliceWidth;
      
      if(minRowLength < sliceWidth) {
         sliceWidth = minRowLength;
      } else {
         minRowLength = sliceWidth;
      }
      for(int row = 0; row < currentRowsOfSlice; ++row) {
         for(int column = 0; column < sliceWidth; ++column) {
            if(isTomato(x+column,y+row))   {++tomatoCnt;  }
            if(isMushroom(x+column,y+row)) {++mushroomCnt;}
            if(tomatoCnt >= minIngrediant && mushroomCnt >= minIngrediant &&
               ((row+1)*(column+1) >= minCells) && ((row+1)*(column+1) <= maxCells) ) {
               (*slice)[0] = x;
               (*slice)[1] = y;
               (*slice)[2] = x+column;
               (*slice)[3] = y+row;
               isValidSlice = true;
            }
         }
      }
      if(isValidSlice) {
         return true;
      }
      ++currentRowsOfSlice;
   } while(currentRowsOfSlice < max_y_slice);

   return false;
}


void Data::removeSliceFromPizza(Slice *slice) {
   for(int x = (*slice)[0]; x <= (*slice)[2]; ++x) {
      for(int y = (*slice)[1]; y <= (*slice)[3]; ++y) {
         (*pizza)[y][x] = N;
      }
   }
}

/* printing */

void Data::printPizza() {
   for(auto row : *pizza) {
      for(auto i : row) {
         cout << i;
      }
      cout << endl;
   }
}

void Data::printSlice(Slice slice) {
   cout << slice[0] << " " << slice[1] << " "
                << slice[2] << " " << slice[3] << endl;
}

void Data::printSolution() {
   if(!solution->empty()) {
      cout << solution->size() << endl;
      for(auto slice : *solution) {
         printSlice(slice);
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
      pizza->push_back(*row);
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
   string line; getline(infile, line); // remove the first line
   
   Data data(rows,columns,minIngrediant,maxCells,&infile);
  
   data.runGreedy();
   
   return 0;
}

