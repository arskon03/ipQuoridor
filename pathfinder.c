#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"
#include "utilities.h"
#include "pathfinder.h"


int pathfinder(element **A, int N, char P, int sr, int sc){
    if(sr < 0 && sc < 0){
        if(find(A,N,P,&sr,&sc) == 0) return -100; //P doesn't exist within A (PANIC)
    }
    qptr q = NULL;
    int i, r, c, rr, cc;
    int dr[4] = {-1, +1, 0, 0}; // Direction vectors for rows
    int dc[4] = {0, 0, +1, -1}; // Direction vectors for collumns
    int moves = 0; // Number of steps taken
    int d = (P == 'W') ? 0 : N-1; // = Destination (assuming the function is called with the right parameters)

    int next_layer = 0; // Equal to the nodes added to the queue in this layer.Used to keep track of the moves
    int this_layer = 1; // Equal to the nodes left to check in this layer. Used to know when we move to the next layer

    int end = 0;
    int **visited; // 1 if cell has already been visited, 0 if not
    visited = malloc(N * sizeof(int *));
    if(visited == NULL){
        printf("? not enough memory! \n\n");
        fflush(stdout);
        return -100;
    }
    for(i = 0;i < N;i++){
        visited[i] = malloc(N * sizeof(int));
        if(visited[i] == NULL){
            printf("? not enough memory! \n\n");
            fflush(stdout);
            return -100;
        }
        for(int j = 0;j < N;j++) visited[i][j] = 0;
    }
    // Find shortest path (only number of steps)
    enqueue(&q, sr, sc);
    visited[sr][sc] = 1;
    while(!empty(q)){
        r = q->x;
        c = q->y;
        dequeue(&q);
        if(r == d){ // Destination reached
            end = 1;
            break;
        }

        // Explore connections(adjacent cells with no walls between)
        for(i = 0;i < 4;i++){
            rr = r + dr[i];
            cc = c + dc[i];
            // Discard cells that don't exist
            if (rr < 0 || cc < 0) continue;
            if (rr >= N || cc >= N) continue;

            // Discard cells that are visited or not connected
            if(visited[rr][cc]) continue;
            if(!connected(A,r,c,rr,cc)) continue;

            // Enqueue and mark as visited
            enqueue(&q, rr, cc);
            visited[rr][cc] = 1;
            next_layer++;
        }
        this_layer--;

        // If this layer ended move to the next
        if(this_layer == 0){
            this_layer = next_layer;
            next_layer = 0;
            moves++;
        }
    }

    // Process ended so we free allocated memory and return
    while(q != NULL) dequeue(&q);
    for(i = 0;i < N;i++) free(visited[i]);
    free(visited);
    if(end) return moves;
    return -1; // The path doesn't exist
}

void enqueue(qptr *ptr, int r, int c){
    // Find last node
    while(*ptr != NULL)
        ptr = &((*ptr)->next);

    // Insert new node at end
    *ptr = malloc(sizeof(struct Qnode));
    (*ptr)->x = r;
    (*ptr)->y = c;
    (*ptr)->next = NULL;
}

void dequeue(qptr *ptr){
    if(*ptr != NULL){
        qptr temp = *ptr;
        *ptr = (*ptr)->next;
        free(temp);
    }
}

int empty(qptr q){
    if (q == NULL)
        return 1;
    else
        return 0;
}
