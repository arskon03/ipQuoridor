#include <stdio.h>
#include "Commands.h"
#include "pathfinder.h"
#include "utilities.h"

/* This is a minimax algorith with A-B pruning 
   Maximizing player starts as 1 (true) 
   Alpha starts as really small and beta really large */
int minimax(int r, int c,int depth, int alpha, int beta, int maximizingplayer, int possiblemoves, int *move){ 
    int i;
    int cr,cc; // Child position
    int eval;  //evaluation of current move

    // If depth has reached 0 process is over
    if(depth == 0)
        return; //ADD EVALUATION
    
    // Process for maximizing player
    if(maximizingplayer){
        int max_eval = -1000000;
        // For each hypothetical child (possible moves)
        for(i = 1; i <= possiblemoves;i++){
            // Maximizingplayer = false (other player's turn)
            int eval = minimax(cr, cc, depth - 1, alpha, beta, 0, possiblemoves, i);
            max_eval = max(max_eval, eval);
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
            // Maximizing player = true (other player's turn)
            eval = minimax(cr, cc, depth -1, alpha, beta, 1, possiblemoves, i); 
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            if(beta <= alpha)
                break;
        }
        return min_eval;
    }
}