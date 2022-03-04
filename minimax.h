/* This is a minimax algorith with A-B pruning */
int minimax(element **A, int N, int *pWW, int *pWB, char *pWinner,int depth,int alpha, int beta,
            int maximizingplayer, int possiblemoves, int *move, node **history, int *hSize, int maxDepth);

/* Evaluates the current board configuration and returns the value to minimax */
int evaluation(element **A, int N, char *pWinner);