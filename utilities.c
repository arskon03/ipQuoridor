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