/*
 *  Challenge by Vardanator (SL)
 *
 *  Title: Possible Knight Moves
 *  Problem: Output should contain the number of the square of the chess
 *           that are under attack by the knight
 *
 *  For example:
 *      input : g6
 *      output: 6
 *
 *      input : a3
 *      output: 4
 *
 *  Solution: Hamza Mateen (DarkRanger on SL)
 *  language: C
 */

#include <stdlib.h>
#include <stdio.h>

#define UPPER_LIMIT 7
#define LOWER_LIMIT 0

int possibleMovesByKnight (int x, int y) {
  int numberOfMoves = 0;

  if (y + 1 <= UPPER_LIMIT) {
    if (x + 2 <= UPPER_LIMIT) 
      numberOfMoves++;
    
    if (x - 2 >= LOWER_LIMIT) 
      numberOfMoves++;
  }

  if (y - 1 >= LOWER_LIMIT) {
    if (x + 2 <= UPPER_LIMIT) 
      numberOfMoves++;
    
    if (x - 2 >= LOWER_LIMIT) 
      numberOfMoves++;
  }

  if (y + 2 <= UPPER_LIMIT) {
    if (x + 1 <= UPPER_LIMIT)
      numberOfMoves++;
  
    if (x - 1 >= LOWER_LIMIT) {
      numberOfMoves++;
    }
  }

  if (y - 2 >= LOWER_LIMIT) {
    if (x + 1 <= UPPER_LIMIT)
      numberOfMoves++;
  
    if (x - 1 >= LOWER_LIMIT) {
      numberOfMoves++;
    }
  }

  return numberOfMoves;
}


int main() {
  char move[3];
  char iter = 0;

  printf("Enter the position of Knight on the board, i.e g7 or a8: ");
  fgets(move, sizeof move, stdin);

  char x = move[0];
  int y = (int)move[1] - '0';
  
  printf("%d are possible moves for %s.\n", possibleMovesByKnight(move[0], (int)move[1] - '0'), move);

  return EXIT_SUCCESS;
}