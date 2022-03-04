/* Convert a string to all lowercase */
char* toLow(char* string);

/* Convert a string to all uppercase */
char* toUpper(char* string);

/* Convert array coordinates to vertex coordinates */
void toVertex(int N, vertex* v, int i, int j);     // Returns 0 if failed

/*Convert vertex coordinates to array coordinates */
void toArray(int N, vertex* v, int* i, int* j);  // Returns 0 if failed

/* Returns the absolute of a number */
int abs(int n);

/* Checking if the cells are adjacent with no wall between them 
 * If they are, player can move from point A(ar,ac) to point B(br,bc)
 * 1 if they are connected 0 if not */
int connected(element **A, int ar, int ac, int br, int bc); 

/* Find position of given player */
int find(element **A, int N, char P, int *r, int *c);

/* Find tha maximum of two values */
int max(int a, int b);

/* Find the minimum of two values */
int min(int a, int b);

/* Interpret's and executes move based on the variable move by calling the proper function
 * Moves 1 to 4 are player movements based on the directional vectors used in the function
 * The rest are wall placements on a certain cell with a certain orientation
 * The genmove variable exists to inform the function that it was called by genmove 
 * Returns -1 when failing(Panic), 0 when the move is illegal And 1 otherwise(everything went well) */
int execute(element **A, int N, int move, int i, int j, int *pWW, int *pWB, char p, char *pWinner, node **history, int *hSize, int genmove);

/* A funtion that you give a move adjascent to the player, or a wall to play, and it returns wether or not the move is possible
 * If the move is on the enemy player, the funbtion returns new coordinates for the best possible move. 
 * Returns 0 if move illegal, 1 if legal, -1 if invalid args are passed and -2 for panic */
int legal_move(element **A, int N, int *pWW, int *pWB, char player, char type, int *iptr, int *jptr, char orient);
