char* toLow(char* string);
char* toUpper(char* string);
void toVertex(int N, vertex* v, int i, int j);     // Returns 0 if failed
void toArray(int N, vertex* v, int* i, int* j);    // Returns 0 if failed
int abs(int n);
int connected(element **A, int ar, int ac, int br, int bc); //1 if they are connected 0 if not
int find(element **A, int N, char P, int *r, int *c);
int max(int a, int b);
int min(int a, int b);
int execute(element **A, int N, int move, int i, int j, int *pWW, int *pWB, char p, char *pWinner, node **history, int *hSize, int genmove);

// A funtion that you give a move adjascent to the player, or a wall to play, and it returns wether or not the move is possible
// If the move is on the enemy player, the funbtion returns new coordinates for the best possible move. Returns 0 if move illegal, 1 if legal, -1 if invalid args are passed
// and -2 for panic
int legal_move(element **A, int N, int *pWW, int *pWB, char player, char type, int *iptr, int *jptr, char orient);
