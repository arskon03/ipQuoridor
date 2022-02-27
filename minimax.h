int minimax(element **A, int N, int *pWW, int *pWB, char *pWinner,int depth,int alpha, int beta,
            int maximizingplayer, int possiblemoves, int *move, node **history, int *hSize);
int evaluation(element **A, int N);