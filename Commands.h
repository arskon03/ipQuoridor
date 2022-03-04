/* This struct is basically a list that works in a Last-In-First_out way thus representing a stack */
typedef struct Node{
    char *move;
    struct Node *nextNode;
}node;

/* This struct is the coordinate system used on input*/
typedef struct Vertex{
     char x;
     int y;
}vertex;

/* Struct that holds the existence of a player or a wall in a certain cell of (element **) A */
typedef struct Element{
    char P;            //values :'B'/'W'/' '
    char w_or;         //values :'H'/'V'/' '
}element;

/* Inserts a node at the start of the game history stack*/ 
int insert_at_start(node** start, char* string);

/* Removes a node from the start of the game history stack*/
void remove_at_start(node** start);

/* Undoes the last action executed, a given number of times, based on the game history stack 
 * Returns 1 when failing and 0 otherwise */
int undo(int times, element **A, int N, int *pWW, int *pWB, char *pWinner, node **history, int *hSize);

/* Resets the size of the board and the rest are considered arbitrary
 * Returns 1 when failing and 0 otherwise */
int boardsize(element ***A, int nValue, int *pN);

/* Walls cleared/Players at starting positions/Game history empty */
void clearboard(element **A, int N,node **history,int *hSize);

/* Prints board with current configuration and each player's number of walls */
void showboard(element **A, int N, int WW, int WB);

/* Places wall on the right position with the right orientation (if the placement is legal)
 * Returns 1 when failing (Panic) and 0 otherwise */
int playwall(element **A, int N, int *pWW, int *pWB, char *player, char *pos, char *orientation, node **history, int* hSize, int print);

/* Executes the given move (if its legal) based on the given strings
 * Returns 1 when failing (Panic) and 0 otherwise */
int playmove(element **A, int N, char *player, char *pos, char *pWinner, node** history, int* hSize, int print);

/* Generates a move with the A.I.
 * Returns 0 when failing (Panic) and 1 otherwise */
int genmove(element **A, int N, char *player, int *pWW, int *pWB,char *pWinner, node **history, int *hSize);