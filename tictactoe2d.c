#include <stdio.h>
#include <stdlib.h>

char grid[3][3];
char check();
void init_grid();
void get_player_move();
void get_computer_move();
void display_grid();


int main(){
  char done;
  printf("This is the game of Tic Tac Toe.\n");
  printf("You will be playing against the computer.\n");

  done = ' ';
  init_grid();

  do{
    display_grid();
    get_player_move();
    done = check();
    if(done != ' '){
      //check if winner is found.
      break;
    }
    get_computer_move();
    done = check();

  }while(done == ' ');

  if(done == 'X'){
    printf("Player 1 has won!\n");
  }
  else
    printf("Computer has beaten your butt.\n");

  display_grid();

  return 0;

  }



void init_grid(){
  int i, j;
  for(i = 0; i < 3; i++){
    for(j = 0 ; j < 3; j++){
      grid[i][j] = ' ';
    }
  }
}

//player move
void get_player_move(){
  int x, y;
  printf("Enter x and y coordinates for your move: \n");
  printf("x: ");
  scanf("%d", &x);
  printf("y: ");
  scanf(" %d", &y);
  if(grid[x][y] != ' '){
    printf("Invalid move, try again.\n");
    get_player_move();
  }
  else
    grid[x][y] = 'X';
}

//computer move
void get_computer_move(){
  int i, j;
  for(i = 0; i < 3; i++){
    for(j = 0; j < 3; j++){
      if(grid[i][j] == ' ')
        break;
    }
    if(grid[i][j] == ' ')
      break;
  }

  if(i*j == 9){
    printf("Draw\n");
    exit(0);
  }
  else
    grid[i][j] = 'O';
}

void display_grid(){
  int t;
  for(t = 0; t < 3; t++){
    printf(" %c | %c | %c ", grid[t][0], grid[t][1], grid[t][2]);
    if(t != 2)
      printf("\n---|---|---\n");
  }
  printf("\n");
}

char check(){
  int i;

  //row checking
  for(i = 0; i < 3; i++){
    if(grid[i][0] == grid[i][1] && grid[i][0] == grid[i][2])
      return grid[i][0];
  }
  //column checking
  for(i = 0; i < 3; i++){
  if(grid[0][i] == grid[1][i] && grid[0][i] == grid[2][i])
    return grid[0][i];
  }

  if(grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]){
  return grid[0][0];
  }

  if(grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]){
  return grid[0][2];


  }
return ' ';
}
