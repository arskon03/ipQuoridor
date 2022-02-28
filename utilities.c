#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "utilities.h"

// Convert a string to all lowercase
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

// Convert a string to all uppercase
char* toUpper(char* string)
{
    int size = strlen(string);
    char* cap = malloc((sizeof(char) * size + 1));

    if (cap != NULL)
        for(int i = 0; i < size + 1; i++)
        {
            char new = string[i];
            if (new >= 'a' && new <= 'z')
                new -= 'a' - 'A';

            cap[i] = new;
        }

    return cap;
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

int abs(int n)
{
    if (n < 0)
        return -1*n;
    else
        return n;
}

/* Checking if the cells are adjacent with no wall between them 
   If they are, player can move from point A(ar,ac) to point B(br,bc) */
int connected(element **A, int ar, int ac, int br, int bc){
    // Same row = horizontal neighbours
    if(ar == br){
        if(ac == bc - 1){ // Neighbour to the right
            if(ar == 0)
                return (A[ar][ac].w_or == 'V') ? 0 : 1;
                // 0 = Not connected
                // 1 = Connected
            else 
                return (A[ar][ac].w_or == 'V' || A[ar - 1][ac].w_or == 'V') ? 0 : 1;  
        }
        else if(ac == bc + 1){ // Neighbour to the left 
            if(ar == 0){
                return (A[br][bc].w_or == 'V') ? 0 : 1;
            }
            else
                return (A[br][bc].w_or == 'V' || A[br - 1][bc].w_or == 'V') ? 0 : 1;
        }
        else return 0; // Not neighbours
    }
    // Same collumn = vertical neighbours
    else if(ac == bc){
        if(ar == br - 1){ // Neighbour below
            if(ac == 0)
                return (A[ar][ac].w_or == 'H') ? 0 : 1;
            else
                return (A[ar][ac].w_or == 'H' || A[ar][ac - 1].w_or == 'H') ? 0 : 1;
        }
        else if(ar == br + 1){ // Neighbour above
            if(ac == 0)
                return (A[br][bc].w_or == 'H') ? 0 : 1;
            else
                return (A[br][bc].w_or == 'H' || A[br][bc -1].w_or == 'H') ? 0 : 1;
        }
        else // Not neighbours
            return 0;
    }
    else return 0; // Not neighbours
}

/* Find position of given player */
int find(element **A, int N, char P,int *r, int *c){
    int i,j;
    for(i = 0;i < N;i++)
        for(j = 0;j < N;j++){
            if(A[i][j].P == P){ 
                *r = i;
                *c = j;
                return 1;
            }
        }
    return 0; // P is invalid or player doesn't exist within A (probably won't be needed)
}

/* Find tha maximum of two values */
int max(int a, int b){
    if(a > b)
        return a;
    else  // a <= b (if they are equal it doesn't matter which one is returned)
        return b;
}

/* Find the minimum of two values */
int min(int a, int b){
    if (a < b)
        return a;
    else  // a >= b (if they are equal it doesn't matter which one is returned)
        return b;
}

/* Interpret's and executes move based by calling the proper function 
   The genmove variable exists to inform the function that it was called by genmove */
int execute(element **A, int N, int move, int i, int j, int *pWW, int *pWB, char p, char *pWinner, node **history, int *hSize, int gemomve){
    int dr[4] = {-1, +1, 0, 0}; // Direction vectors for rows
    int dc[4] = {0, 0, +1, -1}; // Direction vectors for collumns
    printf("%d\n",move);
    if(move <= 4){
        i += dr[move - 1];
        j += dc[move - 1];
        // Make coordinates into a vertex and then into a string to pass on to playmove
        vertex v;
        toVertex(N, &v, i, j);
        char *pos;
        sprintf(pos,"%c%d", v.x, v.y);
        if(genmove) printf("= %C%d\n\n", v.x, v.y);
        playmove(A, N, &p, pos, pWinner, history, hSize);
        return 0;
    }

    /* All other cases are wall placements */
    else{
        // Find position of the wall based on the number of the move 
        int r,c;
        r = ((move - 4)/2)/(N-1); // Both are even there is no need for casting
        c = ((move - 4)/2)%(N-1);

        // Find orientation based on the number of the move
        char o = (move%2) ? 'h' : 'v'; // Odd numbers for horizontal placement and even for vertical

        // Make coordinates into a vertex and then into a string to pass on to playwall
        vertex v;
        toVertex(N, &v, r, c);
        char *pos;
        sprintf(pos,"%c%d", v.x, v.y);
        if(genmove) printf("= %C%d %c\n\n", v.x, v.y, o);
        playwall(A, N, pWW, pWB, &p, pos, &o, history, hSize);
        return 0;
    }
}