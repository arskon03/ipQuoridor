#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"

void showboard(element **A,int N,int WW,int WB){
    int i,j;
    for(i = 1;i <= (2*N + 3);i++){                      //board big enough to show all rows,collumns and the coordinates
        for(j = 1;j <= N+2;j++){ 
            if(i == 1 || i == (2*N+3)){               //First and last line are always the coordinates
                if(j == 1 || j == N+2) printf("    ");
                else printf(" %c  ", 'A'+(j-2));
            }
            else if(i == 2 || i == (2*N+2)){          //second and second to last lines are always the same
                if (j == 1 ) printf("   +");
                else if(j == N+2) printf("+   ");
                else printf("---+");
            }
            else if(i%2 == 1){
                if(j == 1) printf(" %d |",N-i+1);
                else if(j == N+2) printf(" %d  ",N-i+1);
                else{
                    printf(" %c ",A[i][j].P);
                    if(i == 3) printf("%c", (A[(i-3)/2][j-2].w_or == 'V') ? 'H' : '|');
                    //printf 'H' instead of '|' for vertical wall if it exists
                    else printf("%c", (A[(i-3)/2][j-2].w_or == 'V' || A[(i-3)/2-1][j-2].w_or == 'V') ? 'H' : '|');
                }
            }
            else{ //i%2 == 0
                if(j == 1) printf("   +");
                else if(j == N+2) printf("+   ");
                else if (j == 2){
                    for(int k = 1; k <= 3; k++)
                        printf("%c",(A[(i-3)/2][j-2].w_or == 'H') ? '=' : '-');
                    printf("%c",(A[(i-3)/2][j-2].w_or == 'H') ? '=' : '+');
                } //printing '=' instead of '-' for horizontal wall if it exists
                else{
                    for(int k = 1; k <= 3; k++)
                        printf("%c",(A[(i-3)/2][j-2].w_or == 'H' || A[(i-3)/2-1][j-2].w_or == 'H') ? '=' : '-');
                    printf("%c",(A[(i-3)/2][j-2].w_or == 'H') ? '=' : '+');
                }
            }
        }
        if(i == 3) printf("black walls: %d",WB);
        if(i == 5) printf("white walls: %d",WW);
        printf("\n");
    }
    printf("\n\n");
}

// Convert a string to all lowercase USELESS BULLSHIT
char* toLow(char* string)
{
    int size = strlen(string);
    char* low = malloc((sizeof(char) * size + 1));

    if (low != NULL)
        for(int i = 0; i < size + 1; i++)
        {
            char new = string[i];
            if (new >= 'A' && new <= 'Z')
                new += 'a' - 'A';

            low[i] = new;
        }

    return low;
}

// Convert array coordinates to vertex coordinates
void toVertex(int N, vertex* v, int i, int j)
{
    v->x = 'A' + j;
    v->y = N - i;
}

// Convert vertex coordinates to array coordinates
void toArray(int N, vertex* v, int* i, int* j)
{
    *i = N - v->y;
    *j = v->x - 'A';
}

// Add logic for walls and going onto ather player
int playmove(element **A, int N, char *player, char *pos, char *pWinner, char*** history, int* hSize)
{
    // Make sure the move is not random words
    if (strlen(pos) > 3)
    {
        printf("? illegal move\n\n");
        return;
    }
    char move[10];

    // Store the type of move
    char type = 'M';
    char p = 'E';

    // Store in a char what player made the move
    char *playerBuff = toLow(player);
    if (strcmp(playerBuff,"black") == 0 || strcmp(playerBuff,"b") == 0)
        p = 'B';
    else if (strcmp(playerBuff,"white") == 0 || strcmp(playerBuff,"w") == 0)
        p = 'W';

    // If char *player is invalid throw error
    if (p == 'E')
    {
        printf("? invalid syntax\n\n");
        return;
    }
    
    // Find the position in the matrix the move is at
    vertex v;
    v.x = pos[0];

    char numbers[3];
    int i, j;
    for(i = 0; i < strlen(pos) - 1; i++)
        numbers[i] = pos[i + 1];

    numbers[2] = '\0';
    v.y = atoi(numbers);
    if(v.y == 0)
    {
        printf("? illegal move\n\n");
        return;
    }

    toArray(N, &v, &i, &j);
    
    // Find the previous position of the player
    int prevI = -1, prevJ = -1;

    if (i > 0)
        if (A[i - 1][j].P == p)
        {
            prevI = i-1;
            prevJ = j;
        }
    
    if (i < N - 1)
        if (A[i + 1][j].P == p)
        {
            prevI = i+1;
            prevJ = j;
        }
    
    if (j > 0)
        if (A[i][j - 1].P == p)
        {
            prevI = i;
            prevJ = j-1;
        }
    
    if (j < N - 1)
        if (A[i][j + 1].P == p)
        {
            prevI = i-1;
            prevJ = j+1;
        }
    
    // If you didn't find somewhere adjascent then it is an illegal move
    if(prevI == -1 || prevJ == -1)
    {
        printf("? illegal move\n\n");
        return;
    }

    // Move the player
    A[i][j].P = p;
    A[prevI][prevJ].P = ' ';


    // If history is not passed as NULL then store the move and increament move count
    if (history != NULL)
    {
        vertex prevV;
        toVertex(N, &prevV, prevI, prevJ);
        
        sprintf(move, "%c%c%02d>%c%02d%c", type, prevV.x, prevV.y, v.x, v.y, p);

        char** temp = realloc(*history, (++*hSize) * sizeof(char*));
        if (temp == NULL)
        return 1;

        temp[*hSize] = move;
        *history = temp;
    }

    return 0;
}
