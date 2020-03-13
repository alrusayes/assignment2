#include <iostream>
#include<stdlib.h>
#include <cstring>
#include <algorithm>
#include <iterator>
#include<fstream>
#include<stdexcept>
#include <thread>
#include <chrono>

using namespace std;


void set_x(int x);
void set_y(int y);
int get_board_x();
int get_board_y();
void print_board();
void generate_board(int x, int y);
void change_cell_state(int x, int y);
void apply_changes();
bool check_board();
float set_random_cell(float percent);
bool get_board_index(int x, int y);
bool get_board_index_2(int x, int y);
int get_all_living_cells();
int get_border_type();
void get_border_type(int type);
int mirror_setup(int boardLimit, int coord);
int torus_setup(int boardLimit, int coord);
bool check_int(string userString);
float input_float(string prompt);
int input_integer(string prompt);
bool check_book(string prompt, string boolTrue, string boolFalse);
void clc_con();
void set_board_file(string filename, int x, int y);
void choose_file();
void print_board_file();
void output_file();
int get_gen_total();
void set_gen_mode(int type);
bool check_neighbor_alive(int xCoord, int yCoord, int xDir, int yDir);
int check_neighbors_total(int xCoord, int yCoord);
void update_cell_status(int xCoord, int yCoord);
void update_gen();
void simulate();
int generationCount;
int inbetweenGenMode;


string is_cell_alive = "X";
string is_cell_dead  = "-";
int border_type;//classic/mirror/donut
int board_x;//width limit
int board_y;//height limit
int total_cells;
int live_total_cells;
float random_cell;
bool board[100][100];
bool board_backend[100][100];
string outputFileName;

float set_random_cell(float percent)
{
  random_cell = percent;
}
int get_border_type()
{
  return border_type;
}
void set_border_type(int type)
{
  border_type = type;
}

bool get_board_index(int x, int y)//returns index at that point in main board
{
  bool val = board[x][y];
  if(border_type == 2)//adjusts for torus mode
  {
    val = board[torus_setup(board_x,x)][torus_setup(board_y,y)];
  }
  return val;
}

bool get_board_index_2(int x, int y)//returns index at that point in backend board
{
  bool val = board_backend[x][y];
  if(border_type == 2)//adjusts for torus mode
  {
    val = board_backend[torus_setup(board_x,x)][torus_setup(board_y,y)];
  }
  return val;
}

void set_x(int x)
{
  board_x = x;
}
void set_y(int y)
{
  board_y = y;
}
int get_board_x()
{
  return board_x;
}
int get_board_y()
{
  return board_y;
}

int get_all_living_cells()
{
  return live_total_cells;
}

void randis_cell_alive()//randomly picks a point and turns it alive if it is dead. restarts if point found is alive.
{
  int randX = rand()%board_x;
  int randY = rand()%board_y;
  if(board_backend[randX][randY]==false)
  {
    change_cell_state(randX,randY);
  }
  else
  {
    randis_cell_alive();
  }
}

void change_cell_state(int x, int y)//switches the value of the cell at the given point. adds or removes from the living counter
{
  if(board_backend[x][y]==false)
  {
    live_total_cells+=1;
  }
  else
  {
    live_total_cells-=1;
  }
  board_backend[x][y] = !board_backend[x][y];
}

void generate_board(int x, int y)//sets the board up with the width, height, and density
{
  board_x = x;
  board_y = y;
  total_cells = x*y;
  live_total_cells = 0;
  float limit = total_cells * random_cell;
  for(int i = 1; i <= limit; i++)
  {
    randis_cell_alive();
  }
}

void print_board()//prints out the board
{
  for(int y = board_y; y >= 1; y--)
  {
    for(int x = 1; x <= board_x; x++)
    {
      if(board_backend[x][y]==true)
      {
        cout << is_cell_alive;
      }
      else
      {
        cout << is_cell_dead;
      }
    }
    cout << endl;
  }
}

void apply_changes()//makes the main board equal to the backend
{
  memcpy( board, board_backend, sizeof(board) );
}

bool check_board()//determins if the board and the backend are the same. used to determin if it has stabilized
{
  for(int x = 1; x <= board_x; x++)
  {
    for(int y = 1; y <= board_y; y++)
    {
      if(board[x][y] != board_backend[x][y])
      {
        return false;
      }
    }
  }
  return true;
}

int mirror_setup(int boardLimit, int coord)
{
  int retarget = coord;
  if(coord > boardLimit)
  {
    retarget = (boardLimit + 1) - (coord - boardLimit);
  }
  if(coord < 1)
  {
    retarget =  -1*coord + 1;
  }
  return retarget;
}

int torus_setup(int boardLimit, int coord)//retargets coordinates to fit within the board confines
{
  int retarget = coord;
  if(coord > boardLimit)
  {
    retarget = coord - boardLimit;
  }
  if(coord < 1)
  {
    retarget = boardLimit + coord;
  }


  return retarget;
}

bool check_book(string prompt, string boolTrue, string boolFalse)//prompts the user for a boolean answer. user must answer with either the second or third input
{
  string userInput = "";

  do
  {
    cout << prompt << endl;
    cin >> userInput;
  }while(userInput != boolTrue && userInput != boolFalse);//repeat while incorrect input is given. Stop when correct is given
  if(userInput == boolTrue)
  {
    return true;
  }
  else
  {
    return false;
  }
}

float input_float(string prompt)//prompts the user for a float answer.
{
  while(true)
  {
    try
    {
      float percent;
      cout << prompt << endl;
      cin >> percent;
      if(percent > 0 && percent <= 1)
      {
        return percent;
        break;
      }
      cin.clear();
      cin.ignore();
    }
    catch(const exception& e)
    {

    }
  }
}

int input_integer(string prompt)//prompts the user for a int answer.
{
  while(true)
  {
    int num;
    cout << prompt << endl;
    try
    {

      cin >> num;
      if(num > 0 && num < 100)
      {
        return num;
        break;
      }
      cin.clear();
      cin.ignore();
    }
    catch(const exception& e)
    {

    }
  }
}


void clc_con()
{
  for(int i = 0; i < 15; i ++)
  {
    cout << endl;
  }
}


void set_board_file(string filename, int x, int y)//sets the backend board up from a file
{
  ifstream file(filename);
  set_x(x);
  set_y(y);
  if(file.is_open())
  {
    int lineCount = 0;
    while(!file.eof())
    {
      lineCount +=1;
      string local_string;
      file >> local_string;
      if(lineCount > 2)
      {
        for(int e = 0; e < x; e++)
        {
          if(local_string[e] == 'X')
          {
            int localx=e+1;
            int localy=lineCount-3;
            change_cell_state(localx, (y-localy));
          }
        }
      }
    }
  }
}


Listen, lets pretend the below method doesnt exist.
void choose_file()//selects file for import and does error checking
{
  bool loaded = false;
  int fileX;
  int fileY;
  while(true)
  {
    string filename = "";
    cout<< "Insert the exact name and extension for the starter game: " << endl;
    cin >> filename;
    ifstream file(filename);
    try
    {
      fileX = 0;
      fileY = 0;
      int lineCount = 0;
      ifstream file(filename);
      if(file.is_open())//if file is open
      {
        while(!file.eof())//while there is a next line
        {
          string local_string;
          file >> local_string;
          if(lineCount <= 1)//sets the X and Y value of the board from the first two lines
          {
              int tempInt = stoi (local_string,nullptr,10);
              if(tempInt > 0 && tempInt <=100)
              {
                if(lineCount < 1)
                {
                  fileY = tempInt;//sets board height
                }
                else
                {
                  fileX = tempInt;//sets board width
                }
              }
              else
              {
                throw exception();
              }
          }
          if(lineCount > 1)//reads the rest of the board
          {
            int count = count(local_string.begin(),local_string.end(), 'X') + count(local_string.begin(),local_string.end(), '-');



            if(local_string.size()!=fileX || lineCount > fileY+2 || local_string.size()!=count)//checks the dimensions of the supplied board to confirm it is the same as the ints above. Also Checks for invalid characters
            {
              throw exception();
            }
          }
          lineCount+=1;
        }
        loaded = true;
      }
      if (loaded == true)//checks to see if file was properly loaded
      {
        set_board_file(filename, fileX, fileY);
      }
      else
      {
        throw exception();
      }
      file.close();
      break;
    }
    catch(const exception& e)
    {
      cout<< "Incorrect File Format. Read the Readme.txt for more info about format. " << endl;
      file.close();
    }
  }
}

void output_file()//creates the output file
{
  cout<< "Insert the exact name and extension for the output file: " << endl;
  cin >> outputFileName;//user input
  ofstream outputFile;
  outputFile.open(outputFileName);
  outputFile.close();
}

void print_board_file()//prints the board to the supplied output file
{
  ofstream outputFile;
  outputFile.open(outputFileName, ios::out | ios::app);
  outputFile << endl << endl << get_gen_total()<<endl;
  for(int y = get_board_y(); y >= 1; y--)
  {
    for(int x = 1; x <= get_board_x(); x++)
    {
      if(get_board_index(x,y)==true)
      {
        outputFile << "X";
      }
      else
      {
        outputFile << "-";
      }
    }
    outputFile << endl;
  }
  outputFile.close();
}

int get_gen_total()
{
  return generationCount;
}

void set_gen_mode(int type)
{
  inbetweenGenMode = type;
}

bool check_neighbor_alive(int xCoord, int yCoord, int xDir, int yDir)//the xDir and yDir are units away from x and y Coord.
{
  int tempX = xCoord+xDir;
  int tempY = yCoord+yDir;
  switch(get_border_type())
  {
    case 0://normal border mode
      tempX = xCoord+xDir;
      tempY = yCoord+yDir;
      if(tempX > get_board_x() || tempY > get_board_y())
      {
        return false;
      }
      break;
    case 1://mirrored border mode
      tempX = mirror_setup(get_board_x(), xCoord + xDir);
      tempY = mirror_setup(get_board_y(), yCoord + yDir);
      break;
    case 2://torus border mode
      tempX = torus_setup(get_board_x(), xCoord + xDir);
      tempY = torus_setup(get_board_y(), yCoord + yDir);
      break;
  }
  get_board_index(tempX, tempY);
}

int check_neighbors_total(int xCoord, int yCoord)//counts the amount of living neighbors a space has
{
  int neighborCount = 0;
  for (int xDir = 1; xDir >= -1; xDir--)
  {
    for (int yDir = 1; yDir >= -1; yDir--)
    {
      if(yDir != 0 || xDir != 0)//to prevent counting self
      {
        if(check_neighbor_alive(xCoord, yCoord, xDir, yDir))
        {
          neighborCount+=1;
        }
      }
    }
  }
  return neighborCount;
}

void update_cell_status(int xCoord, int yCoord)//updates specified living cell and tries to adjust neightbors
{
  int neighborCount = check_neighbors_total(xCoord,yCoord);
  //Edit Living Cell
  if(get_board_index(xCoord, yCoord) && get_board_index_2(xCoord, yCoord))//if cell is alive in both board and backend
  {
    switch(neighborCount)
    {
      case 0:
      case 1:
      change_cell_state(xCoord,yCoord);//too few neighbors
      break;
      case 2:
      case 3://stable
      break;
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      change_cell_state(xCoord,yCoord);//not enough neighbors
      break;
    }
  }

  //Edit Dead Cell Neighbors
  for (int xDir = 1; xDir >= -1; xDir--)
  {
    for (int yDir = 1; yDir >= -1; yDir--)
    {
      if(get_board_index(xCoord + xDir, yCoord + yDir)==false && get_board_index_2(xCoord + xDir, yCoord + yDir)==false)//if cell is dead in both board and backend, prevents cell from being edited more than once per generation
      {
      if((yDir != 0 || xDir != 0) && get_board_index(xCoord + xDir, yCoord + yDir) == false)//to prevent counting self or living cells
        {
          int x = xCoord + xDir;
          int y = yCoord + yDir;
          int otherNeighbors=check_neighbors_total(x,y);
          if(get_border_type()==2)//retargets cells for torus/donut mode
          {
            x = torus_setup( get_board_x(), x);
            y = torus_setup( get_board_y(), y);
            otherNeighbors=check_neighbors_total(x,y);
          }
          if(otherNeighbors==3)//if there are three living neighbors, switch state to living
          {
            change_cell_state(x,y);
          }
        }
      }
    }
  }
}

void update_gen()//function for starting the next generation cycle
{
  generationCount+=1;
  for(int y=1;y<=get_board_y();y++)
  {
    for(int x=1;x<=get_board_x();x++)
    {
      if(get_board_index(x,y))
      {
        update_cell_status(x,y);
      }
    }
  }
}

void simulate()//starts the entire simulation
{
  int livingCells = get_all_living_cells();
  bool stopSim = false;
  clc_con();
  cout << generationCount << endl;//prints out current generation count
  print_board();
  apply_changes();//commits backend to main board

  string newLine;
  if(inbetweenGenMode == 0)
  {
    this_thread::sleep_for(chrono::seconds(1));
  }
  if(inbetweenGenMode == 1)
  {
    getline(cin, newLine);
    getline(cin, newLine);
  }
  if(inbetweenGenMode == 2)
  {
    print_board_file();
  }

  while(livingCells != 0 && stopSim == false)
  {
    clc_con();
    update_gen();
    cout << generationCount << endl;
    print_board();
    stopSim = check_board();
    apply_changes();
    livingCells = get_all_living_cells();
    if (stopSim)//if simulation is stable, end game
    {
      cout << "Game Stable, please press enter to end game. "<< endl;
      getline(cin, newLine);
      getline(cin, newLine);
    }
    if(inbetweenGenMode == 0)//inbetween generation pause
    {
      this_thread::sleep_for(chrono::seconds(1));
    }
    if(inbetweenGenMode == 1)//manual generation
    {
      getline(cin, newLine);
    }
    if(inbetweenGenMode == 2)//output to file for each generation
    {
      print_board_file();
    }
  }
}
