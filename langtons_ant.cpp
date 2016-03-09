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

vector<vector<vector<string> > > grid;

int height = 128;
int width = 128;
int depth = 2;

string invert_color(string color){
  if (color == "W"){
    return "B";
  }else{
    return "W";
  }
}

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

  QMainWindow *window = new QMainWindow;
  QWidget *widget = new QWidget;
  QHBoxLayout *layout = new QHBoxLayout(widget);
  QLabel *label = new QLabel();
  QFont *font = new QFont("Courier");
  QPixmap *pix = new QPixmap(500,500);

  pix->fill(Qt::white);
  QPainter *painter = new QPainter(pix);

  window->setWindowTitle("Langton's Ant");
  window->resize(500,500);
  window->setCentralWidget(widget);

  layout->addWidget(label);
  window->show();

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
