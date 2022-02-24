#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"
#include "genmove.h"

/* This is a breadth-first search based pathfinder algorithm*/
/* Source: https://en.wikipedia.org/wiki/Breadth-first_search */
int pathfinder(A,N){
    int r,c,i,j;
    //Initializing an adjacency list as an N^2 X N^2 matrix
    //Where row r represents the cell of A[i][j] where N*i + j == r and same goes for the collumns(c = N*i + j)
    //So AdjL[r][c] is 0 when cell r of A is not connected to element c and 1 when they are connected
    //Here connected means that player can move from m to n, so they are adjacent with no wall between them
    int **AdjL = malloc(N*N * sizeof(element*));
    if(AdjL == NULL){
        printf("? Not enough memory\n\n");
        return 1;
    }
    for(r = 0; r < N*N;r++){
        AdjL[r] = malloc(N*N * sizeof(element));
        if(AdjL[r] == NULL){
            printf("? Not enough memory\n\n");
            return 1;
        }
    }
    //Calculating connection's
    for(r = 0;r < N*N;r++)
        for(c = 0;c < N*N;c++){
            //main diagonal is 0 because it represents a cell's connection to itself
            if(r = c) AdjL[r][c] = 0;
        }
}