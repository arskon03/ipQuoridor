int minimax(element **A, int N, char p, int *pWW, int *pWB, int*pWinner,int depth,int alpha, int beta,
            int maximizingplayer, int possiblemoves, int *move, char ***history, int *hSize);
int evaluation(element **A, int N, int move, int maximizingplayer, int r, int c);