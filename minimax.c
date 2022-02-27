#include <stdio.h>
#include "Commands.h"
#include "pathfinder.h"
#include "utilities.h"

/* This is a minimax algorith with A-B pruning
   Maximizing player starts as 1 (true)
   Alpha starts as really small and beta really large */
int minimax(element **A, int N, int *pWW, int *pWB, int*pWinner, int r, int c,int depth,
            int alpha, int beta, int maximizingplayer, int possiblemoves, int *move, char ***history, int *hSize){ 
    int i;
    int cr,cc; // Child position
    int eval;  //evaluation of current move
    
    // If depth has reached 0 process is over
    if(depth == 0)
        return evaluation(A, N, move, maximizingplayer, r, c);
    
    // Process for maximizing player
    if(maximizingplayer){
        int max_eval = -1000000;
        // For each hypothetical child (possible moves)
        for(i = 1; i <= possiblemoves;i++){
            // Execute and then undo the move so the next minimax call can have the proper board configuration
            // Only for the first level of the hypothetical tree
            if(depth == 3)//execute(A, N, i, cr, cc, pWW, pWB, 'W', pWinner, history, hSize)  //ADD VARIABLES TO MINIMAX

            // Maximizingplayer = false (other player's turn)
            eval = minimax(A, N, pWW, pWB, pWinner, cr, cc, depth - 1, alpha, beta, 0, possiblemoves, move, history, hSize);
            max_eval = max(max_eval, eval);

            // Changing max_eval means that a better move was found 
            if(max_eval == eval && depth == 3) // We only care about the first level of the hypothetical tree(depth = 3)
                *move = i;
            
            // Undo the action executed earlier so the board returns to its original configuration
            if(depth == 3) undo(1, A, N, pWW, pWB, pWinner, history, hSize);

            // Prune if needed
            alpha = max(alpha,eval);
            if(beta <= alpha)
                break;
        }
        return max_eval;
    }

    // Process for minimizing player
    else{
        int min_eval = 1000000;
        for(i = 1; i <= possiblemoves;i++){
            // Same as above, for the second level of the tree which is the only level of the minimizing player
            if(depth == 3)//execute(A, N, i, cr, cc, pWW, pWB, 'W', pWinner, history, hSize)  //ADD VARIABLES TO MINIMAX

            // Maximizing player = true (other player's turn)
            eval = minimax(A, N, pWW, pWB, pWinner, cr, cc, depth -1, alpha, beta, 1, possiblemoves, move, history, hSize);
            min_eval = min(min_eval, eval);

            // Undo
            if(depth == 3) undo(1, A, N, pWW, pWB, pWinner, history, hSize);

            // Prune if needed
            beta = min(beta, eval);
            if(beta <= alpha)
                break;
        }
        return min_eval;
    }
}

/* Evaluate move based on board orientation and differences to the path */
int evaluation(element **A, int N, int move, int maximizingplayer, int r, int c){
    int dr[4] = {-1, +1, 0, 0}; // Direction vectors for rows
    int dc[4] = {0, 0, +1, -1}; // Direction vectors for collumns
    // First 4 moves = move according to the vector
    // The rest are wall placements at a certain position with certain orientation (both based on the number)
    if(move <= 4){ // Move
        int i,j;
        i = r + dr[move - 1]; //r,c are the current positions
        j = c + dc[move - 1]; //i,j are the positions of the move
        if(i < 0 || i >= N || j < 0 || j >= N) return 0; // Illegal move is evaluated as 0
        return pathfinder(A, N, 'W', r, c) - pathfinder(A, N, 'W', i, j);  //ADD PLAYER TO MINIMAX
    }
    else{  // Wall placement

    }
}