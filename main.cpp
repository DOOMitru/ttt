#include <stdio.h>
#include <stdlib.h>

extern int getch();

#define ROWS 3
#define COLS 3

#define PLAYER1 'X'
#define PLAYER2 'O'
#define EMPTYCELL ' '

#define TRUE  1
#define FALSE 0

char board[ROWS][COLS],
     menu_selection,
     player_choice,
     firstrun;
unsigned char currentplayer,
              player_error,
              winner,
              tie;
int xWins, oWins, ties;

void printGrid(int howMany, int spaces);
void printGameBoard();
void clearGameBoard();
void printGameHeader();
void printGameMenu();
void resetGameState();
void resetGameStats();
void resetGame();
void printGameWinner();
void printTieGame();
void printWinOrTie();
void clearScreen();
void updateScreen();

void computersTurn();
void playersTurn();
void switchPlayer(); // ok
void getInput(int * x, int * y); // ok
char getMenuSelection(); // ok

unsigned char isBoardFull(); // ok
unsigned char checkBoard(); // ok
unsigned char validMove(int row, int col); // ok
unsigned char intelligentMove(int * row, int * col); // ok
unsigned char keepPlaying();

/******************************************************************************/
int main()
{
    do
    {
        resetGame();
        printGameHeader();
        printGameMenu();

        // 1. Single Player  3. Watch
        // 2. Multiplayer    4. Quit
        menu_selection = getMenuSelection();

        if(menu_selection != '4')
        {
            printGameHeader();
            printGameBoard();

            while(keepPlaying());
        }
    }
    while(menu_selection != '4');

    clearScreen();
    printGameHeader();
    printf("      Thanks for playing!\n");

    return 0;
}
/******************************************************************************/
// printGrid()
// Prints the tic-tac-toe board
// @return NONE
// @params howMany - how many squares to print
//         spaces - how many spaces to place before the grid
void printGrid(int howMany, int spaces)
{
    int i = 0, j = 0, k = 0;
    if(howMany < 1 || howMany > 19) return;

    for(j = 0; j < howMany; j++)
    {
        for(i = 0; i < howMany; i++)
        {
            if(i == 0)
            {
                printf(" "); for(k = 0; k < spaces; k++) printf(" ");
            }
            printf(" ---");
        }
        printf("\n");

        for(i = 0; i < howMany; i++)
        {
            if(i == 0)
            {
                printf(" "); for(k = 0; k < spaces; k++) printf(" ");
            }
            printf("| %c ",board[j][i]);
        }
        printf("|\n");
    }
    for(i = 0; i < howMany; i++)
    {
        if(i == 0)
        {
            printf(" "); for(k = 0; k < spaces; k++) printf(" ");
        }
        printf(" ---");
    }
    printf("\n");
}
/******************************************************************************/
// printGameBoard()
// Prints the tic-tac-toe board
// @return NONE
// @params NONE
void printGameBoard()
{
    printf("O_WINS:%*d   TIES:%*d   X_WINS:%*d\n\n",3,oWins,3,ties,3,xWins);
    printGrid(3, 9); printf("\n");
}
/******************************************************************************/
// clearGameBoard()
// Sets every cell on the board to the EMPTYCELL
// @return NONE
// @params NONE
void clearGameBoard()
{
    unsigned char i = 0, j = 0;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            board[i][j] = EMPTYCELL;
}
/******************************************************************************/
// printGameHeader()
// Prints the introduction
// @return NONE
// @params NONE
void printGameHeader()
{
    printf("----------------------------------\n");
    printf(" Dumitru Jr. Campan's Tic Tac Toe \n");
    printf("----------------------------------\n");
}
/******************************************************************************/
// printGameMenu()
// Prints the game menu
// @return NONE
// @params NONE
void printGameMenu()
{
    int i = 0;
    printf("             GAME MENU \n");

    for(i = 0; i < 34; i++) printf("-");

    printf("\n 1. Single Player        3. Watch\n"
           "\n 2. Multiplayer          4. Quit\n");

    for(i = 0; i < 34; i++) printf("-");

    printf("\n");
}
/******************************************************************************/
// resetGameState()
// Resets the entire game state
// @return NONE
// @params NONE
void resetGameState()
{
    currentplayer = 1; xWins = 0; oWins = 0; ties = 0;
    winner = FALSE; tie = FALSE; player_error = FALSE; firstrun = TRUE;
    clearGameBoard();
}
/******************************************************************************/
// resetGameStats()
// Resets the game stats
// @return NONE
// @params NONE
void resetGameStats()
{
    xWins = 0; oWins = 0; ties = 0;
}
/******************************************************************************/
// resetGame()
// Reset everything including clearning the screen
// @return NONE
// @params NONE
void resetGame()
{
    clearScreen();
    resetGameState();
}
/******************************************************************************/
// printGameWinner()
// Prints the winner footer
// @return NONE
// @params NONE
void printGameWinner()
{
    printf("----------------------------------\n");
    printf("             WINNER: %c           \n",
        (currentplayer == 1) ? PLAYER1 : PLAYER2);
    printf("----------------------------------\n");
}
/******************************************************************************/
// printTieGame()
// Prints the tie footer
// @return NONE
// @params NONE
void printTieGame()
{
    printf("----------------------------------\n");
    printf("              ! TIE !\n");
    printf("----------------------------------\n");
}
/******************************************************************************/
// printWinOrTie()
// Print winner or tied game
// @return NONE
// @params NONE
void printWinOrTie()
{
    if(winner == TRUE) printGameWinner();
    else if(tie == TRUE) printTieGame();
}
/******************************************************************************/
// clearScreen()
// Clears the screen
// @return NONE
// @params NONE
void clearScreen()
{
    // if ported, replace with system equivalent
    system("cls"); // windows only
}
/******************************************************************************/

char getMenuSelection()
{
    char ch;
    do
    {
        printf("Enter your choice   <1,2,3,4>: ");
        ch = getch(); printf("<%c>\n", ch);
    }
    while(ch != '1' && ch != '2' &&
          ch != '3' && ch != '4');

    if(ch == '1') // single player
    {
        do // let user pick 'X' or 'O'
        {
            printf("Which would you prefer? (X/O): ");
            player_choice = getch();
            printf("'%c'\n",player_choice);
        }while(player_choice != 'x' && player_choice != 'X' &&
               player_choice != 'o' && player_choice != 'O');

        if(player_choice == 'x' || player_choice == 'X')
            currentplayer = 1;
        else currentplayer = 2;
    }

    return ch;
}

unsigned char intelligentMove(int * x, int * y)
{
    int i = 0, j = 0, k = 0, row = -1, col = -1, counter = 0;
    char player = (currentplayer == 1) ? PLAYER1 : PLAYER2;
    //char ch;

for(k = 0; k < 2; k++)
{
    //if(k == 0) printf("offensive\n"); else printf("defensive\n");

    // check for possible wins
    row = -1; col = -1;
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            if(board[i][j] == player)
                counter++;
            else if(board[i][j] == EMPTYCELL){
                if(row < 0 || col < 0)
                    row = i; col = j;
            }
        }
        if(counter == 2 && row != -1 && col != -1){
            *x = row; *y = col;
            // printf("row true %i,%i\n", *x, *y); ch = getch();
            return TRUE;
        }
        counter = 0;
        row = -1; col = -1;
    }

    row = -1; col = -1;

    for(j = 0; j < COLS; j++)
    {
        for(i = 0; i < ROWS; i++)
        {
            if(board[i][j] == player)
                counter++;
            else if(board[i][j] == EMPTYCELL){
                if(row < 0 || col < 0)
                    row = i; col = j;
            }
        }
        if(counter == 2 && row != -1 && col != -1){
            *x = row; *y = col;
            //printf("col true %i,%i\n", *x, *y); ch = getch();
            return TRUE;
        }
        counter = 0;
        row = -1; col = -1;
    }

    if(board[0][0] == player && board[1][1] == player && board[2][2] == EMPTYCELL){
        *x = 2; *y = 2;
        //printf("diag 1 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    } else if(board[0][0] == player && board[1][1] == EMPTYCELL && board[2][2] == player){
        *x = 1; *y = 1;
        //printf("diag 1 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    } else if(board[0][0] == EMPTYCELL && board[1][1] == player && board[2][2] == player){
        *x = 0; *y = 0;
        //printf("diag 1 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    } else if(board[0][2] == player && board[1][1] == player && board[2][0] == EMPTYCELL){
        *x = 2; *y = 0;
        //printf("diag 2 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    } else if(board[0][2] == player && board[1][1] == EMPTYCELL && board[2][0] == player){
        *x = 1; *y = 1;
        //printf("diag 2 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    } else if(board[0][2] == EMPTYCELL && board[1][1] == player && board[2][0] == player){
        *x = 0; *y = 2;
        //printf("diag 2 true %i,%i", *x, *y); ch = getch();
        return TRUE;
    }

    player = (currentplayer == 1) ? PLAYER2 : PLAYER1;
}

    if(board[1][1] == EMPTYCELL && !firstrun)
    {
        *x = 1; *y = 1; return TRUE;
        //printf("midcell"); ch = getch();
    }
    //printf("false"); ch = getch();
    return FALSE;
}

// computersTurn()
// Allows the computer to make a move on the board
// @return NONE
// @params NONE
void computersTurn()
{
    int x, y;

    updateScreen();

    printf(
            "      Player %c is thinking!\n",
            (currentplayer == 1) ? PLAYER1 : PLAYER2
    );
    // random wait for the computer to think :p
    if(menu_selection == '3') _sleep((rand() % 1500) + 500);
    else _sleep((rand() % 700) + 300); // between 300 and 1500 ms

    do // look for random move that's valid
    {
        if(!intelligentMove(&x,&y)) // try an intelligent move
        {
            // can't find intelligent move
            // try random one
            x = rand() % ROWS; // get a random row
            y = rand() % COLS; // get a random column
        }
    }
    while(!validMove(x,y) && !isBoardFull());

    if(checkBoard()) // check if the current player has won
    {
        if(currentplayer == 1) xWins++;
        else oWins++;
        winner = TRUE;
    }
    else if(isBoardFull()) // check if the board is full after move
    {
        // if it's full after the move
        // the game is tied since player hasn't won
        tie = TRUE;
        ties++;
    }
}

// playersTurn()
// Allows the current player to make a move on the board
// @return NONE
// @params NONE
void playersTurn()
{
    int x, y;
    unsigned char goodMove;

    do
    {
        getInput(&x,&y);
        goodMove = validMove(x-1,y-1);

        if(!goodMove)
            printf("     Invalid Move!   Try again\n");
    }
    while(!goodMove);

    if(checkBoard()) // check if the current player has won
    {
        if(currentplayer == 1) xWins++;
        else oWins++;
        winner = TRUE;
    }
    else if(isBoardFull()) // check if the board is full after move
    {
        // if it's full after the move
        // the game is tied since player hasn't won
        tie = TRUE;
        ties++;
    }
}

// isBoardFull()
// Checks if the board is all filled up with non-empty cells
// @return TRUE if the board is full
//         FALSE otherwise
// @params NONE
unsigned char isBoardFull()
{
    // basically looks for an empty cell
    unsigned char i = 0, j = 0;
    for(i = 0; i < ROWS; i++)
        for(j = 0; j < COLS; j++)
            if(board[i][j] == EMPTYCELL) // found it
                return FALSE;

    return TRUE; // didn't find one
}

// checkBoard()
// Checks if the current player is a winner
// @return TRUE if there's a winner on the board
//         FALSE otherwise
// @params NONE
unsigned char checkBoard()
{
    char player = 0;
    // switch character for current player
    switch(currentplayer)
    {
        case 1: player = PLAYER1; break;
        case 2: player = PLAYER2; break;
    }

    if( // check diagonals
       (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
       (board[1][1] == player && board[0][2] == player && board[2][0] == player) ||
        // check rows
       (board[0][0] == player && board[0][1] == player && board[0][2] == player) ||
       (board[1][0] == player && board[1][1] == player && board[1][2] == player) ||
       (board[2][0] == player && board[2][1] == player && board[2][2] == player) ||
        // check columns
       (board[0][0] == player && board[1][0] == player && board[2][0] == player) ||
       (board[0][1] == player && board[1][1] == player && board[2][1] == player) ||
       (board[0][2] == player && board[1][2] == player && board[2][2] == player)
       )
    {
        return TRUE;
    }

    return FALSE; // no winner yet
}

// checkMove(int x, int y)
// Checks for a valid position
// @return TRUE - if it's a valid move within boundaries
//         FALSE - if it's an invalid move
// @params x is the row starting at 0
//         y is the column starting at 0
// @post   if it's valid, the board position (x,y) is filled in with the
//         character representing the current player
unsigned char validMove(int row, int col)
{
    // check boundaries
    if(row < 0 || row > ROWS - 1) { return FALSE; }
    if(col < 0 || col > COLS - 1) { return FALSE; }

    if(board[row][col] == EMPTYCELL) // empty cell, valid move
    {
        // fill the cell with current player
        board[row][col] = (currentplayer == 1) ? PLAYER1 : PLAYER2;
        return TRUE;
    }
    else return FALSE; // not an empty cell
}

// getInput(int * x, int * y)
// @return NONE
// @params x - row on grid
//         y - column on grid
void getInput(int * x, int * y)
{
    // get row
    printf("       Player %c", (currentplayer == 1) ? PLAYER1 : PLAYER2);
    printf(", enter Row: ");
    scanf("%i", x);

    // get column
    printf("       Player %c", (currentplayer == 1) ? PLAYER1 : PLAYER2);
    printf(", enter Col: ");
    scanf("%i", y);
    printf("\n");
}

// keepPlaying()
// @return TRUE  if there's a winner or a tie
//         FALSE otherwise
// @params None
unsigned char keepPlaying()
{
    char ch;
    updateScreen();
    if(menu_selection == '3')
    {
        computersTurn();
    }
    else if(menu_selection == '2')
    {
        playersTurn();
    }
    else if(menu_selection == '1')
    {
        if((player_choice == 'x' || player_choice == 'X') && currentplayer == 1)
            playersTurn();
        else if((player_choice == 'o' || player_choice == 'O') && currentplayer == 2)
            playersTurn();
        else computersTurn();
    }

    updateScreen();

    if(winner || tie)
    {
        printWinOrTie();
        do
        {
            printf("       Play again? (Y/N): ");
            ch = 'Y';
            ch = getch(); printf("'%c'\n", ch);
        }while(ch != 'y' && ch != 'Y' &&
               ch != 'n' && ch != 'N');

        if(ch == 'y' || ch == 'Y')
        {
            if(menu_selection != '3')
            {
                do
                {
                    printf("     Reset scores? (Y/N): ");
                    ch = getch(); printf("'%c'\n", ch);
                }while(ch != 'y' && ch != 'Y' &&
                       ch != 'n' && ch != 'N');

                if(ch == 'y' || ch == 'Y')
                    resetGameStats();
            }

            clearGameBoard(); updateScreen(); switchPlayer();
            firstrun = TRUE; winner = FALSE; tie = FALSE;
            return TRUE;
        }
        else return FALSE;
    }
    else currentplayer = 3 - currentplayer;

    firstrun = FALSE;

    return TRUE;
}

void switchPlayer()
{
    if(menu_selection == '3')
        currentplayer = (rand() % 2) + 1;
    else if(menu_selection == '1')
    {
        if(player_choice == 'x' || player_choice == 'X')
            currentplayer = 1;
        else currentplayer = 2;
    }
}

void updateScreen()
{
    clearScreen();
    printGameHeader();
    printGameBoard();
}
