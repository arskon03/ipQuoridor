typedef struct Qnode *qptr;
/* This struct is basically a list that works in a First-In-First-Out way thus representing a queue */
struct Qnode{
    int x;
    int y;
    qptr next;
};

int empty(qptr q);
void enqueue(qptr *q, int r, int c);
void dequeue(qptr *q);
int pathfinder(element **A, int N, char P, int sr, int sc);
