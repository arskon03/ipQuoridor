int minimax(element **A, int N, int *pWW, int *pWB, int*pWinner, int r, int c,int depth,
            int alpha, int beta, int maximizingplayer, int possiblemoves, int *move, char ***history, int *hSize);
int evaluation(element **A, int N, int move, int maximizingplayer, int r, int c);