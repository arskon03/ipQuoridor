void showboard(char **,int,int,int);
void playmove(char **,int,char *,char *,char *);
void playwall(char **,int,int *,int *,char *,char *,char *);
char *genmove(char **,int,char *,int *,int *);

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