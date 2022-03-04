#include <stdio.h>
#include <limits.h>
#include "Commands.h"
#include "utilities.h"
#include "pathfinder.h"
#include "minimax.h"

int minimax(element **A, int N, int *pWW, int *pWB, char *pWinner,int depth, int alpha, int beta,
            int maximizingplayer, int possiblemoves, int *move, node **history, int *hSize, int maxDepth) { 
    int i;
    int r = 0, c = 0;
    int eval;   //evaluation of current move
    
    // If depth has reached 0 or there is a winner return the evaluation of the current board configuration
    if(depth == 0 || *pWinner != '\0'){
        return evaluation(A, N, pWinner);
    }

    // Process for maximizing player
    if(maximizingplayer){
        int max_eval = INT_MIN;
        // For each hypothetical child (possible moves)
        for(i = 1; i <= possiblemoves;i++){
            // Find maximizing player's position
            find(A, N, 'W', &r, &c);  // White is the maximizing player

            // Execute and then undo the move so the next minimax call can have the proper board configuration
            int check = execute(A, N, i, r, c, pWW, pWB, 'W', pWinner, history, hSize,0);
            
            // If move is illegal, try another move
            if (!check) continue;

            if (check == -2) return INT_MIN + 1; // If panic return MIN INT

            // Maximizingplayer = false (other player's turn)
            eval = minimax(A, N, pWW, pWB, pWinner, depth - 1, alpha, beta, 0, possiblemoves, move, history, hSize, maxDepth);
            if (eval == INT_MIN + 1) return INT_MIN + 1; // If panic return MIN INT
            
            // Smaller max_eval means that a better move was found (if they are the same we keep the first move)
            if(max_eval < eval && depth == maxDepth) // We only care about the first level of the hypothetical tree(depth == maxDepth)
                *move = i;

            max_eval = max(max_eval, eval);

            // Undo the action executed earlier so the board returns to its original configuration
            undo(1, A, N, pWW, pWB, pWinner, history, hSize);

            // Prune if needed
            alpha = max(alpha,eval);
            if(beta <= alpha)
                break;
        }
        return max_eval;
    }

    // Process for minimizing player
    else{
        int min_eval = INT_MAX;
        for(i = 1; i <= possiblemoves;i++){
            // Find minimizing player's position
            find(A, N, 'B', &r, &c); //Black is the minimizing player

            // Same as above
            int check = execute(A, N, i, r, c, pWW, pWB, 'B', pWinner, history, hSize, 0);

            // If move is illegal, try another move
            if (!check) continue;

            if (check == -2) return INT_MIN + 1; // If panic return MIN INT

            // Maximizing player = true (other player's turn)
            eval = minimax(A, N, pWW, pWB, pWinner, depth -1, alpha, beta, 1, possiblemoves, move, history, hSize, maxDepth);
            if (eval == INT_MIN + 1) return INT_MIN + 1; // If panic return MIN INT

            // Greater min_eval means that a better move was found (if they are the same we keep the first one)
            if(min_eval > eval && depth == maxDepth) // We only care about the first level of the hypothetical tree(depth == maxDepth)
                *move = i;

            min_eval = min(min_eval, eval);

            // Return to original configuration
            undo(1, A, N, pWW, pWB, pWinner, history, hSize);

            // Prune if needed
            beta = min(beta, eval);
            if(beta <= alpha)
                break;
        }
        return min_eval;
    }
}

int evaluation(element **A, int N, char *pWinner){
    // Evaluating according to the path
    // path(black) > path(white) returns a positive number (white has the advantage)
    // path(black) < path(white) returns a negative number (black has the advantage)
    // If Winner is white, then white has a significant avantage so we multiply the positive part with 10
    // Same if black is the wiiner but now we multiply the negative
    return (pathfinder(A, N, 'B', -1, -1) * ((*pWinner == 'W') ? 10 : 1)) - (pathfinder(A, N, 'W', -1, -1) * ((*pWinner == 'B') ? 10 : 1));
}