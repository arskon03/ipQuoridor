typedef struct Qnode *qptr;

/* This struct is basically a list that works in a First-In-First-Out way thus representing a queue */
struct Qnode{
    int x;
    int y;
    qptr next;
};

/* This is a breadth-first search based pathfinder algorithm 
 * Sources: https://en.wikipedia.org/wiki/Breadth-first_search
 *          https://www.youtube.com/watch?v=KiCBXu4P-2Y&ab_channel=WilliamFiset */
int pathfinder(element **A, int N, char P, int sr, int sc);


/* Enqueue = Insert at end of list */
void enqueue(qptr *q, int r, int c);

/* Dequeue = delete first node of the list */
void dequeue(qptr *q);

/* Checking if queue(list) is empty */
int empty(qptr q);
