char* toLow(char* string);
char* toUpper(char* string);
void toVertex(int N, vertex* v, int i, int j);     // Returns 0 if failed
void toArray(int N, vertex* v, int* i, int* j);    // Returns 0 if failed
int abs(int n);
int connected(element **A, int ar, int ac, int br, int bc); //1 if they are connected 0 if not
int find(element **A, int N, char P, int *r, int *c);
int max(int a, int b);
int min(int a, int b);
