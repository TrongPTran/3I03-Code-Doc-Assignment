/*
Langton's Ant

Here are the rules:

At a white square, turn 90° right, flip the color of the square, move forward one unit
At a black square, turn 90° left, flip the color of the square, move forward one unit

We are going to have the following represent the colors:
W: White
B: Black
AU/AR/AD/AL: Red (Ant) + Direction

And we are going to have each cell be either:
["W", "AD"] if the ant is present or
["W", "N"] if not, where N is a string that doesn't have A in it
*/


// Import all required libraries and headers
// This allows use of libraries, which the libraries can be called to help create the application and not recreate code that is already made
#include "langtons_ant_config.h"
#include <QApplication>
#include <QtWidgets>
#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QColor>

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

string direction;

// Initializing window variables
vector<vector<vector<string> > > grid;

int height = 128;
int width = 128;
int depth = 2;

// Conditions for when ant reaches a white or black square
// If statement when ant reaches a colour, simple conditions for changing the color
string invert_color(string color){
  if (color == "W"){
    return "B";
  }else{
    return "W";
  }
}

// Function for ant turning right
// Function provides a number of conditions when string "direction" reaches a condition
// Function is written as is because ant needs conditions stated at top in order to turn
// When ant reaches white square, the ant checks the direction it is facing, turns 90° right and returns a string its "new direction"
char turn_right(string direction){
  if (direction == "U"){
    return 'R';
  }else if(direction == "R"){
    return 'D';
  }else if(direction == "D"){
    return 'L';
  }else{
    return 'U';
  }
}

// Function for ant turning left
// Function provides a number of conditions when string "direction" reaches a condition
// Function is written as is because ant needs conditions stated at top in order to turn
// When ant reaches black square, the ant checks the direction it is facing, turns 90° left and returns a string its "new direction"
char turn_left(string direction){
  if (direction == "U"){
    return 'L';
  }else if(direction == "R"){
    return 'U';
  }else if(direction == "D"){
    return 'R';
  }else{
    return 'D';
  }
}


// Function which allows the ant to move forward
// The ant's position is tracked on a 2D board consisting of x and y coordinates which are created by a multidimensional array, the function needs to iterate through the array and check conditions for the ant to move
// The function starts with iterations through the x dimension of the matrix and y dimension of matrix, then checks if "A" is not the greatest value
// Afterso, the function will check conditionals and move accordingly to the ant's rules of movement, where the code will set a previous position of the matrix as the new position
// After which it will return the new matrix with the update position
vector<vector<vector<string>>> move_ant_forward(vector<vector<vector<string>>> matrix){
  for(int x=0;x<matrix.size();x++){
    for(int y=0;y<matrix[x].size();y++){
      if (matrix[x][y][1].find("A") != string::npos){
        //Get ant's direction
        direction = matrix[x][y][1].back();
        if (direction == "U"){
          matrix[x-1][y][1] = matrix[x][y][1];
        }else if(direction == "R"){
          matrix[x][y+1][1] = matrix[x][y][1];
        }else if(direction == "D"){
          matrix[x+1][y][1] = matrix[x][y][1];
        }else{
          matrix[x][y-1][1] = matrix[x][y][1];
        }
        matrix[x][y][1] = "";
        return matrix;
      }
    }
  }
}

// This function gets the current ant position and turns the ant
// The function like the move_ant_forward function needs to iterate through the array and check the conditions in which the ant will turn
// This function uses the matrix array to track the ants position, it will find the ants position , then checks if "A" is not the greatest value
// Afterso, the function will check conditionals and turn accordingly to the ant's rules of turning, whether it will turn left or right
// After that, the code will invert the color of the square and then call the move_ant_forward function to move the ant forward after its turn
// In which the it will return the new matrix with an updated position
vector<vector<vector<string>>> update_pixmap(vector<vector<vector<string>>> matrix){

  //Get current ant pos
  for(int x=0;x<matrix.size();x++){
    for(int y=0;y<matrix[x].size();y++){
      if (matrix[x][y][1].find("A") != string::npos){
        //Get ant's direction
        direction = matrix[x][y][1].back();
        if (matrix[x][y][0] == "W"){
          matrix[x][y][1].back() = turn_right(direction);
        }else{
          matrix[x][y][1].back() = turn_left(direction);
        }
        matrix[x][y][0] = invert_color(matrix[x][y][0]);
        matrix = move_ant_forward(matrix);
        return matrix;
      }
    }
  }
}


// The main function where all the interface setup happens in order to display Langton's Ant
int main(int argc, char **argv){

  QApplication app (argc, argv);

  //Set up sizes. (heightxwidth)
  grid.resize(height);
  for (int i = 0; i < height; ++i) {
    grid[i].resize(width);
    for (int j = 0; j < width; ++j){
      grid[i][j].resize(depth);
    }
  }

  //Fill with white cells 
  for (int x=0;x<grid.size();x++){
    for (int y=0;y<grid[x].size();y++){
      grid[x][y][0] = "W";
      grid[x][y][1] = "W";
    }
  }

  grid[128/2][128/2][1] = "AL";//Initial placement of ant
  
  // These objects are creating instances from the libraries, all of these are graphic related instances
  // These include the windows, its labels, and the pixels they will use
  // Libraries and these instances are used because the library is available for usage, coding these from scratch would be redundant and extremely inefficient
  QMainWindow *window = new QMainWindow;
  QWidget *widget = new QWidget;
  QHBoxLayout *layout = new QHBoxLayout(widget);
  QLabel *label = new QLabel();
  QFont *font = new QFont("Courier");
  QPixmap *pix = new QPixmap(500,500);


  // Create painter object and fill the whole map white
  // A rule with Langton's ant is that the ant's starting map must be blank, henceforth these are created to make the map white (blank)
  pix->fill(Qt::white);
  QPainter *painter = new QPainter(pix);

  window->setWindowTitle("Langton's Ant");
  window->resize(500,500);
  window->setCentralWidget(widget);

  layout->addWidget(label);
  window->show();

  // Reiterations through the grid in order to color the grid when there is a possible change in the grid because of the ant movement
  // Conditionals are set, these check if the grid is white or black, in which the brush will change to the opposite color to color the grid when the ant moves
  while (true){
    for(int x=0;x<grid.size();x++){
      for(int y=0;y<grid[x].size();y++){
        //draw square of appropriate color
        if (grid[x][y][1].find("A") != string::npos){
          painter->setBrush(QColor(255,0,0,255));
        }else if (grid[x][y][0] == "W"){
          painter->setBrush(QColor(255,255,255,255));
        }else if(grid[x][y][0] == "B"){
          painter->setBrush(QColor(0,0,0,255));
        }
        painter->drawRect(y*4,x*4,4,4);
      }
    }

    label->setPixmap(*pix);
    qApp->processEvents();
    grid = update_pixmap(grid);
    //this_thread::sleep_for(chrono::seconds(1));
  }
}
