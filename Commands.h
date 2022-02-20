typedef struct Vertex{
     char x;
     int y;
}vertex;

typedef struct Element{
    char P;            //values :'B'/'W'/' '
    char w_or;         //values :'H'/'V'/' '
    vertex V;          //values :"A3"/"B9" etc. = coordinates
}element;

void undo(int, element **A, int, int *, int *, char *);
void boardsize(element **A, int *pN, int *pWW, int *pWB);
void clearboard(element **A, int N, int *pWW, int *pWB);
void showboard(element **A, int N, int WW, int WB);
int playmove(element **A, int N, char *player, char *pos, char *pWinner, char*** history, int* hSize);
int playwall(element **A, int N, int *pWW, int *pWB, char *player, char *pos, char *orientation, char*** history, int* hSize);
char *genmove(element **A, int N, char *player, int *pWW, int *pWB, char*** history);
char* toLow(char* string);
void toVertex(int N, vertex* v, int i, int j);     // Returns 0 if failed
void toArray(int N, vertex* v, int* i, int* j);    // Returns 0 if failed

/*typedef struct coord{
    char x;
    unsigned int y;
}vertex;

/*typedef struct INT{
    unsigned int X : 31;
}int;

typedef char * string;        //non white-space
typedef char * color;         //"white"/"w"/"black"/"b"         :NOT CASE SENSITIVE
typedef char * orientation;   //"horizontal"/"h"/"vertical"/"v" :NOT CASE SENSITIVE
typedef char * boolean;       //"false"/"f"/"true"/"t"          :NOT CASE SENSITIVE

typedef struct Move{
    color c;
    vertex v;
}move;

typedef struct Wall{
    color c;
    vertex v;
    orientation o;
}wall;*/