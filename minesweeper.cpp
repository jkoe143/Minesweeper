#include <iostream> 
using namespace std;

#ifndef SEED_VALUE
#define SEED_VALUE 0
#endif
const int width = 5;
const int height = 8;


const int UNFLAGGED_MINE = -1; 
const int FLAGGED_MINE = -2; 
const int INCORRECT_FLAGGED_MINE = -3; 
const int UNKNOWN = -4; 
const int num_mine = 10;


int c00, c01, c02, c03, c04, c10, c11, c12, c13, c14, c20, c21, c22, c23, c24, c30, c31, c32, c33, c34, c40, c41, c42, c43, c44, c50, c51, c52, c53, c54, c60, c61, c62, c63, c64, c70, c71, c72, c73, c74;
int* mineField[height][width] = {
  {&c00, &c01, &c02, &c03, &c04},
  {&c10, &c11, &c12, &c13, &c14},
  {&c20, &c21, &c22, &c23, &c24},
  {&c30, &c31, &c32, &c33, &c34},
  {&c40, &c41, &c42, &c43, &c44},
  {&c50, &c51, &c52, &c53, &c54},
  {&c60, &c61, &c62, &c63, &c64},
  {&c70, &c71, &c72, &c73, &c74}
};

int num_incorrect_flagged_mine = 0; 
int num_correct_flagged_mine = 0; 
bool explode = false;


char getTag(int c, bool explode);
void displayField(bool explode);
void setCell(int i, int j, int value);
int getCell(int i, int j);
void setMine(int num_of_mine);
void initField(int num_of_mine);
bool isMine(int i, int j);
void reveal(int i,int j); 
void executeCmd(char action, int row, int col);


char getTag(int c, bool explode) {
  char tag;
  switch (c) {
    case UNKNOWN:
      tag = '-';
      break;
    case UNFLAGGED_MINE:
      if (explode) 
        tag = '*';
      else 
        tag = '-';
      break;
    case FLAGGED_MINE:
      tag = '!';
      break;
    case INCORRECT_FLAGGED_MINE:
      if (explode)
        tag = '&';
      else
        tag = '!';
      break;
    default:
      tag = static_cast<char>(c) + '0';
  };
  return tag;
}


void displayField(bool explode) {
  cout << "   0 1 2 3 4" <<endl << endl;
  cout << "0  " << getTag(c00, explode) << " " << getTag(c01, explode) << " " << getTag(c02, explode) << " " << getTag(c03, explode) << " " << getTag(c04, explode) << endl;
  cout << "1  " << getTag(c10, explode) << " " << getTag(c11, explode) << " " << getTag(c12, explode) << " " << getTag(c13, explode) << " " << getTag(c14, explode) << endl;
  cout << "2  " << getTag(c20, explode) << " " << getTag(c21, explode) << " " << getTag(c22, explode) << " " << getTag(c23, explode) << " " << getTag(c24, explode) << endl;
  cout << "3  " << getTag(c30, explode) << " " << getTag(c31, explode) << " " << getTag(c32, explode) << " " << getTag(c33, explode) << " " << getTag(c34, explode) << endl;
  cout << "4  " << getTag(c40, explode) << " " << getTag(c41, explode) << " " << getTag(c42, explode) << " " << getTag(c43, explode) << " " << getTag(c44, explode) << endl;
  cout << "5  " << getTag(c50, explode) << " " << getTag(c51, explode) << " " << getTag(c52, explode) << " " << getTag(c53, explode) << " " << getTag(c54, explode) << endl;
  cout << "6  " << getTag(c60, explode) << " " << getTag(c61, explode) << " " << getTag(c62, explode) << " " << getTag(c63, explode) << " " << getTag(c64, explode) << endl;
  cout << "7  " << getTag(c70, explode) << " " << getTag(c71, explode) << " " << getTag(c72, explode) << " " << getTag(c73, explode) << " " << getTag(c74, explode) << endl;
}


void setCell(int i, int j, int value) {
  *mineField[i][j] = value;
}


int getCell(int i, int j) {
  return *mineField[i][j];
}

void setMine(int num_of_mine) {
  int current_mine = 0;
  while (current_mine < num_of_mine) {
    int i = rand() % height; 
    int j = rand() % width;
    if (getCell(i,j) != UNFLAGGED_MINE) {
      setCell(i,j, UNFLAGGED_MINE);
      current_mine++;
    }
  }
}

void initField(int num_of_mine) {
  for (int i = 0; i < height; i++){
    for (int j = 0;j < width; j++){
      setCell(i, j, UNKNOWN);
    }
  }
  setMine(num_of_mine);
}


bool isMine(int i, int j) {
  if (getCell(i,j) == UNFLAGGED_MINE || getCell(i,j) == FLAGGED_MINE){
    return true;
  } else{
    return false;
  }
}

int count(int i,int j){
  int count = 0;
  for(int a = i-1; a <= i +1; a++){
    for(int b = j-1; b <= j +1; b++){
      if(a == i && b ==j){
        continue;
      }
      if(a >= 0 && a < height && b >= 0 && b < width){
        if(isMine(a, b)){
          count++;
        }
      }
    }
  }
  return count;
}


void reveal(int i,int j) {
  if (isMine(i, j)) {
    explode = true;
  } else {
    setCell(i, j, count(i,j));
  }
}



void executeCmd(char action, int row, int col) {
  int i = row;
  int j = col;
  switch(action){
    case ('f'):
      if (isMine(i,j)){
        setCell(i,j,FLAGGED_MINE);
        num_correct_flagged_mine++;
      } else if(!isMine(i,j)){
        setCell(i,j,INCORRECT_FLAGGED_MINE);
        num_incorrect_flagged_mine++;
      }
      break;
    case ('r'):
      reveal(i,j);
      break;
    case ('u'):
      if (getCell(i,j) == FLAGGED_MINE){
        setCell(i,j,UNFLAGGED_MINE);
        num_correct_flagged_mine --;
      } else if(getCell(i,j) == INCORRECT_FLAGGED_MINE){
        setCell(i,j,UNFLAGGED_MINE);
        num_incorrect_flagged_mine --;
      }
      break;
  }
}



int main() {
  srand(SEED_VALUE);
  initField(num_mine);
  char cmd, row_ch, col_ch;	
  displayField(false);
  while (!explode && (num_correct_flagged_mine < num_mine || num_incorrect_flagged_mine > 0)) {
    cout << "Enter cmd:" << endl;
    cin >> cmd >> row_ch >> col_ch;
    executeCmd(cmd, row_ch - '0', col_ch - '0' );
    displayField(explode);
    cout << "Number of mine left: " << num_mine - (num_correct_flagged_mine + num_incorrect_flagged_mine) << endl;
  }
  if (!explode) {
    cout << ":)" << endl;

  } else {
    cout << ":(" << endl;
  }
  return 0;
}
