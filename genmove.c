#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"
#include "genmove.h"

/* This is a breadth-first search based pathfinder algorithm */
/* Sources: https://en.wikipedia.org/wiki/Breadth-first_search
            https://www.youtube.com/watch?v=KiCBXu4P-2Y&ab_channel=WilliamFiset */
int pathfinder(element **A, int N, char P, int sr, int sc){
    qptr q = NULL;
    int i,r,c,rr,cc;
    int dr[4] = {-1, +1, 0, 0}; // Direction vectors for rows
    int dc[4] = {0, 0, +1, -1}; // Direction vectors for collumns
    int moves = 0; // Number of steps 
    int d = (P == 'W') ? 0 : N-1; // = Destination (assuming the function is called with the right parameters)

    int next_layer = 0; // Equal to the nodes added to the queue in this layer.Used to keep track of the moves
    int this_layer = 1; // Equal to the nodes in this layer. Used to know when we move to the next layer

    int end = 0;
    int **visited; // 1 if cell has already been visited, 0 if not
    visited = malloc(N * sizeof(int *));
    if(visited == NULL){
        printf("? not enough memory!\n\n");
        return -100;
    }
    for(i = 0;i < N;i++){
        visited[i] = malloc(N * sizeof(int));
        if(visited[i] == NULL){
            printf("? not enough memory!\n\n");
            return -100;
        }
    }
    // Find shortest path (only number of steps)
    enqueue(&q, sr, sc);
    visited[sr][sc] = 1;
    while(!empty(q)){
        r = q->x;
        c = q->y;
        dequeue(&q);
        if(r == d){
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
        if(this_layer == 0){
            this_layer = next_layer;
            next_layer = 0;
            moves++;
        }
        printf("%d\n",moves);
    }
    // Process ended so we free allocated memory and return
    for(i = 0;i < N;i++) free(visited[i]);
    free(visited);
    if(end) return moves;
    return -1; // The path doesn't exist
}

/* Enqueue = Insert at end of list */
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

/* Dequeue = delete first node */
void dequeue(qptr *ptr){
    if(*ptr != NULL){
        qptr temp = *ptr;
        *ptr = (*ptr)->next;
        free(temp);
    }
}

/* Checking if queue is empty */
int empty(qptr q){
    if (q == NULL)
        return 1;
    else
        return 0;
}

/* Checking if the cells are adjacent with no wall between them */
int connected(element **A, int ar, int ac, int br, int bc){
    // Same row = horizontal neighbours
    if(ar == br){
        if(ac == bc + 1){ // Neighbour to the right
            if(ar == 0){
                if(A[ar][ac].w_or == 'V')
                    return 0; // 0 = Not connected
                else
                    return 1; // 1 = Connected
            }
            else if(A[ar][ac].w_or == 'V' || A[ar - 1][ac].w_or == 'V')
                return 0; 
            else
                return 1; 
        }
        else if(ac == bc -1){ // Neighbour to the left 
            if(ar == 0){
                if(A[br][bc].w_or == 'V')
                    return 0;
                else
                    return 1;
            }
            else if(A[br][bc].w_or == 'V' || A[br - 1][bc].w_or == 'V')
                return 0;
            else 
                return 1;
        }
        else // Not neighbours
            return 0;
    }
    // Same collumn = vertical neighbours
    else if(ac == bc){
        if(ar == br + 1){ // Neighbour below
            if(ac == 0){
                if(A[ar][ac].w_or == 'H')
                    return 0;
                else
                    return 1;
            }
            else if(A[ar][ac].w_or == 'H' || A[ar][ac - 1].w_or == 'H')
                return 0;
            else
                return 1;
        }
        else if(ar == br - 1){ // Neighbour above
            if(ac == 0){
                if(A[br][bc].w_or == 'H')
                    return 0;
                else
                    return 1;
            }
            if(A[br][bc].w_or == 'H' || A[br][bc -1].w_or == 'H')
                return 0;
            else
                return 1;
        }
        else // Not neighbours
            return 0;
    }
    else // Not neighbours
        return 0;
}
