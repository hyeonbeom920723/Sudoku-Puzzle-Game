/***********************************************************************
* Program:
*    Project 13, Sudoku
*    Brother Comeau, CS 124
* Author:
*    Ian Lee
* Summary: 
*    Plays the game sudoku
*
*    Estimated:  0.0 hrs   
*    Actual:     0.0 hrs
*      CompareValue function is most difficult in this project 
************************************************************************/

#include <iostream>
#include <fstream>

using namespace std;

/**********************************************************************
 * Debugging Tools
 ***********************************************************************/
//#define DEBUGcomputeValues
 
//Enables debugging messages inside computeValues
#ifdef DEBUGcomputeValues
#define DebugCV(x) x
#define DebugCVR(x)
#else
#define DebugCV(x)
#define DebugCVR(x) x
#endif

int convertLetterToRowNum(char letter);
void changeSquare(char * coordinates, int board[][9]);
void displayValues(char * coordinates, int board[][9]);
void displayBoard(int board[][9]);

/**********************************************************************
 * Get the filename
 ***********************************************************************/
char * getBoard()
{
   char * filename = new char[256];
 
   cout << "Where is your board located? ";
   cin >> filename;
 
   return filename;
}
 
/**********************************************************************
 * Save and Read the file
 ***********************************************************************/
char * getSaveFile()
{
   char * filename = new char[256];
 
   cout << "What file would you like to write your board to: ";
   cin >> filename;
 
   return filename;
}

/**********************************************************************
 * Read file
 ***********************************************************************/
void readFile(char * filename, int board[][9])
{
   ifstream file(filename);
   if (file.fail())
   {
      cout << "Error reading file \"" << filename << "\"" << endl;
      return;
   }
 
   int row = 0;
   int col = 0;
 
   while (file >> board[row][col])
   {
      col++;
       
      if (file == "\n")
         row++;
   }
 
   file.close();
 
   return;
}
 
/**********************************************************************
 * Ask user for coordinates of the square
 ***********************************************************************/
void getCoordinates(char coordinates[])
{
   cout << "What are the coordinates of the square: ";
   cin >> coordinates;
 
   return;
}
 
/**********************************************************************
 * Ask user for value
 ***********************************************************************/
int getValue(char * coordinates)
{
   int value = 0;
 
   cout << "What is the value at \'" << coordinates << "\': ";
   cin >> value;
 
   return value;
}

/**********************************************************************
 * Control to keep playing the game
 * like give some options for playing
 ***********************************************************************/
bool interact(int board[][9])
{
   cout << "Options:" << endl;
   cout << "   ?  Show these instructions" << endl;
   cout << "   D  Display the board" << endl;
   cout << "   E  Edit one square" << endl;
   cout << "   S  Show the possible values for a square" << endl;
   cout << "   Q  Save and Quit" << endl;
 
   cout << endl;
   displayBoard(board);
 
   char coordinates[2] = {0};
 
   while (true)
   {
      cout << endl << "> ";
       
      char userInput = '0';
      cin >> userInput;
 
      switch (userInput)
      {
         case '?':
            cout << "Options:" << endl;
            cout << "   ?  Show these instructions" << endl;
            cout << "   D  Display the board" << endl;
            cout << "   E  Edit one square" << endl;
            cout << "   S  Show the possible values for a square" << endl;
            cout << "   Q  Save and Quit" << endl;
            cout << endl;
            break;
 
         case 'D':
         case 'd':
            displayBoard(board);
            break;
 
         case 'E':
         case 'e':
            getCoordinates(coordinates);
            changeSquare(
                        coordinates,
                        board
                        );
            break;
 
         case 'S':
         case 's':
            getCoordinates(coordinates);
            displayValues(
                         coordinates,
                         board
                         );
            break;
    
         case 'Q':
         case 'q':
            return true;
     
         default:
            cout << "ERROR: Invalid command" << endl;
      }
   }
}
  
/**********************************************************************
 * This function computes the value
 ***********************************************************************/
void computeValues(char * coordinates, int board[][9],
                   int possibleValues[])
{
   int col = convertLetterToRowNum(coordinates[0]);
   int row = (coordinates[1] - '0') - 1;
 
   int verticalNeighbors[8] = {0};
   int vNIndex = 0;
 
   DebugCV(cerr << "Below: ");
   for (int i = row + 1; i < 9; i++)
   {
      verticalNeighbors[vNIndex] = board[i][col];
      vNIndex++;
      DebugCV(cerr << verticalNeighbors[vNIndex - 1]);
   }
   DebugCV(cerr << endl);
 
   DebugCV(cerr << "Above: ");
   for (int d = row - 1; d > -1; d--)
   {
      verticalNeighbors[vNIndex] = board[d][col];
      vNIndex++;
      DebugCV(cerr << verticalNeighbors[vNIndex - 1]);
   }
   DebugCV(cerr << endl);
    
   int horizontalNeighbors[8] = {0};
   int hNIndex = 0;
 
   DebugCV(cerr << "Right: ");
   for (int i = col + 1; i < 9; i++)
   {
      horizontalNeighbors[hNIndex] = board[row][i];
      hNIndex++;
      DebugCV(cerr << horizontalNeighbors[hNIndex - 1]);
   }
   DebugCV(cerr << endl);
 
   DebugCV(cerr << "Left: ");
   for (int d = col - 1; d > -1; d--)
   {
      horizontalNeighbors[hNIndex] = board[row][d];
      hNIndex++;
      DebugCV(cerr << horizontalNeighbors[hNIndex - 1]);
   }
   DebugCV(cerr << endl);
    
   int squareNeighbors[8] = {0};
   int sNIndex = 0;
 
   int majorCol = col / 3;
   int majorRow = row / 3;
   int neighborCol = 0;
   int neighborRow = 0;
 
   DebugCV(cerr << "Square: ");
   for (int i = 0; i < 3; i++)
   {
      for (int j = 0; j < 3; j++)
      {
         neighborCol = (majorCol * 3) + i;
         neighborRow = (majorRow * 3) + j;
 
         if (!(neighborCol == col && neighborRow == row))
         {
            squareNeighbors[sNIndex] = board[neighborRow][neighborCol];
            sNIndex++;
            DebugCV(cerr << squareNeighbors[sNIndex - 1]);
         }
      }
   }
   DebugCV(cerr << endl);
 
   int verticalPossible[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   int horizontalPossible[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   int squarePossible[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   int vIndex = 0;
   int hIndex = 0;
   int sIndex = 0;
 
   for (int i = 0; i < 8; i++)
   {
      vIndex = verticalNeighbors[i] - 1;
      hIndex = horizontalNeighbors[i] - 1;
      sIndex = squareNeighbors[i] - 1;
      
      if (vIndex >= 0)
      {
         DebugCV(cerr << "V Removed: " << verticalPossible[vIndex]
                      << endl);
         verticalPossible[vIndex] = 0;
      }
      if (hIndex >= 0)
      {
         DebugCV(cerr << "H Removed: " << horizontalPossible[hIndex]
                      << endl);
         horizontalPossible[hIndex] = 0;
      }
      if (sIndex >= 0)
      {
         DebugCV(cerr << "S Removed: " << squarePossible[sIndex]
                      << endl);
         squarePossible[sIndex] = 0;
      }
   }
    
   int possibleAnswers[9] = {0};
   int pAIndex = 0;
    
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         for (int n = 0; n < 9; n++)
         {
            if (verticalPossible[i] == horizontalPossible[j] 
                && horizontalPossible[j] == squarePossible[n]
                && squarePossible[n] != 0)
            {
               possibleAnswers[pAIndex] = verticalPossible[i];
               pAIndex++;
            }
         }
      }
   }
 
   int saveIndex = 0;
 
   while (saveIndex < 9 && possibleAnswers[saveIndex] != 0)
   {
      possibleValues[saveIndex] = possibleAnswers[saveIndex];
      saveIndex++;
   }
 
   return;
}
 
/**********************************************************************
 * Edit the square that user declared
 ***********************************************************************/
void changeSquare(char * coordinates, int board[][9])
{
   int col = convertLetterToRowNum(coordinates[0]);
   int row = (coordinates[1] - '0') - 1;
 
   if (!(row >= 0 && row < 9) && !(col >= 0 && col < 9))
   {
      cout << "ERROR: Square \'" << coordinates
           << "\' is invalid" << endl;
      return;
   }
 
   if (board[row][col] != 0)
   {
      cout << "ERROR: Square \'" << coordinates
           << "\' is filled" << endl;
      return;
   }
 
   int value = getValue(coordinates);
 
   if (value < 0 || value > 9)
   {
      cout << "ERROR: Value \'" << value
           << "\' in square \'" << coordinates
           << "\' is invalid" << endl;
      return;
   }
 
   int possibleValues[9] = {0};
   computeValues(coordinates, board, possibleValues);
   bool isValueValid = false;
 
   for (int i = 0; possibleValues[i] != 0; i++)
   {
      if (value == possibleValues[i])
      {
         isValueValid = true;
         break;
      }
   }
 
   if (!isValueValid)
   {
      cout << "ERROR: Value \'" << value
           << "\' in square \'" << coordinates
           << "\' is invalid" << endl;
      return;
   }
 
   board[row][col] = value;
   return;
}
 
/**********************************************************************
 * Convert letter to number
 ***********************************************************************/
int convertLetterToRowNum(char letter)
{
   int rowNum = 0;
 
   switch (letter)
   {
      case 'A':
      case 'a':
         return 0;
 
      case 'B':
      case 'b':
         return 1;
 
      case 'C':
      case 'c':
         return 2;
 
      case 'D':
      case 'd':
         return 3;
 
      case 'E':
      case 'e':
         return 4;
 
      case 'F':
      case 'f':
         return 5;
 
      case 'G':
      case 'g':
         return 6;
 
      case 'H':
      case 'h':
         return 7;
 
      case 'I':
      case 'i':
         return 8;
 
      default:
         return -1;
   }
}
 
/**********************************************************************
 * display the board
 ***********************************************************************/
void displayBoard(int board[][9])
{
   int row = 0;
   int col = 0;
 
   cout << "   A B C D E F G H I" << endl;
 
   while (row < 9)
   {
      if (col == 0)
      {
         cout << (row + 1) << "  ";
      }
 
      if (board[row][col] != 0)
      {
         cout << board[row][col];
      }
      else
      {
         if (col != 9)
         {
            cout << " ";
         }
      }
      col++;
 
      if (col % 3 == 0 && col != 9)
      {
         cout << "|";
      }
      else
      {
          
         if (col != 9)
         {
            cout << " ";
         }
      }
       
      if (col == 9)
      {
         col = 0;
         row++;
         cout << endl;
      }
       
       
      if ((row == 3 || row == 6) && col == 0)
      {
         cout << "   -----+-----+-----" << endl;
      }
   }
}
 
/**********************************************************************
 * Save the board
 ***********************************************************************/
void saveBoard(char * filename, int board[][9])
{
   ofstream file(filename);
   if (file.fail())
   {
      cout << "Error opening file \"" << filename << "\"" << endl;
      return;
   }
 
   int row = 0;
   int col = 0;
 
   while (row < 9)
   {
      file << board[row][col];
      col++;
       
      if (col == 9)
      {
         col = 0;
         row++;
         if (row < 9)
         {
            file << endl;
         }
      }
      else
      {
         file << " ";
      }
   }
 
   file.close();
 
   cout << "Board written successfully" << endl;
 
   return;
}
 
/**********************************************************************
 * put user's input in the related coordinate
 ***********************************************************************/
void displayValues(char * coordinates, int board[][9])
{
   int possibleValues[9] = {0};
   computeValues(coordinates, board, possibleValues);
    
   DebugCVR(cout << "The possible values for \'" << coordinates
        << "\' are: ");
 
   int index = 0;
    
   while (index < 9 && possibleValues[index] != 0)
   {
      cout << possibleValues[index];
      index++;
       
      if (index != 9 && possibleValues[index] != 0)
      {
         cout << ", ";
      }
   }
   cout << endl;
}

/**********************************************************************
 * Drive function
 ***********************************************************************/
int main()
{
   int board[9][9] = {0};
 
   char * fileLoadLocation = getBoard();
   readFile(fileLoadLocation, board);
    
   bool exitProgram = false;
   while (!exitProgram)
   {
      exitProgram = interact(board);
   }
 
   char * fileSaveLocation = getSaveFile();
   saveBoard(fileSaveLocation, board);
 
   return 0;
}


char * getBoard();
char * getSaveFile();
void getCoordinates(char coordinates[]);
int getValue(char * coordinates);
bool interact(int board[][9]);
void readFile(char * filename, int board[][9]);
void computeValues(char * coordinates, int board[][9], int possibleValues[]);



void saveBoard(char * filename, int board[][9]);
int convertLetterToRowNum(char letter);
void changeSquare(char * coordinates, int board[][9]);
void displayValues(char * coordinates, int board[][9]);
void displayBoard(int board[][9]);
