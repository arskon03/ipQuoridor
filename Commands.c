#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"

void showboard(element **A,int N,int WW,int WB){
    int i,j;
    for(i = 1;i <= (2*N + 3);i++){      //board big enough to show all rows,collumns and the coordinates                
        for(j = 1;j <= N+2;j++){ 
            //First and last lines are always the coordinates
            if(i == 1 || i == (2*N+3)){                 
                if(j == 1 || j == N+2) printf("    ");
                else printf(" %c  ", 'A'+(j-2));
            }
            //second and second to last lines are always the same
            else if(i == 2 || i == (2*N+2)){
                if (j == 1 ) printf("   +");
                else if(j == N+2) printf("    ");
                else printf("---+");
            }
            //Rows with blocks of the board
            else if(i%2 == 1){
                if(j == 1) printf("%2d |",N-(i/2)+1);
                else if(j == N+2) printf(" %d  ",N-(i/2)+1);
                else{
                    printf(" %c ",A[(i-3)/2][j-2].P);
                    //printing 'H' instead of '|' for vertical wall if it exists
                    if(i == 3) printf("%c", (A[(i-3)/2][j-2].w_or == 'V') ? 'H' : '|');
                    else printf("%c", (A[(i/2)-1][j-2].w_or == 'V' || A[(i/2)-2][j-2].w_or == 'V') ? 'H' : '|');
                }
            }
            //Rows between the blocks
            else{ //i%2 == 0
                if(j == 1) printf("   +");
                else if(j == N+2) printf("    ");
                else if (j == 2){
                    for(int k = 1; k <= 3; k++)
                        printf("%c",(A[(i/2)-2][j-2].w_or == 'H') ? '=' : '-');
                    printf("%c",(A[(i/2)-2][j-2].w_or == 'H') ? '=' : (A[(i/2)-2][j-2].w_or == 'V') ? 'H' : '+');
                } //printing '=' instead of '-' for horizontal wall if it exists
                else{
                    for(int k = 1; k <= 3; k++)
                        printf("%c",(A[(i/2)-2][j-2].w_or == 'H' || A[(i/2)-2][j-3].w_or == 'H') ? '=' : '-');
                    printf("%c",(A[(i/2)-2][j-2].w_or == 'H') ? '=' :(A[(i/2)-2][j-2].w_or == 'V') ? 'H' : '+');
                }
            }
        }
        if(i == 3) printf("black walls: %d",WB);
        if(i == 5) printf("white walls: %d",WW);
        printf("\n");
    }
    printf("\n\n");
}

/*Resets the size of the board and the rest are considered arbitrary*/
int boardsize(element ***A, int nValue, int *pN){
    int i;
    if(*A != NULL){
        for(i = 0;i < *pN;i++)
            free((*A)[i]);
        free(*A);
    }
    element **temp = NULL;
    temp = malloc(nValue * sizeof(element *));
    if(temp == NULL) return 1;
    for (i = 0;i < nValue;i++){
        temp[i] = malloc(nValue * sizeof(element));
        if (temp[i] == NULL) return 1;
    }
    *pN = nValue;
    *A = temp;
    //(*A)[nValue - 1][nValue - 1].P = 'g';
    return 0;
}

/*Walls cleared/Players at starting positions/Game history empty*/
void clearboard(element **A, int N, char ***history, int *hSize){
    int i,j;
    for(i = 0;i < N;i++)
        for(j = 0;j < N;j++){
            //(*A)[i][j].P = 'g';
            if(i == 0 && j == (int)(N/2))        //black starting position
                A[i][j].P = 'B';
            else if(i == N-1 && j == (int)(N/2)) //white starting position
                A[i][j].P = 'W';
            else 
                A[i][j].P = ' ';
            
            A[i][j].w_or = ' ';
            toVertex(N, &A[i][j].V, i, j);
            /*A[i][j].V.x = 'A' + j;
            A[i][j].V.y = N - i;*/
        }
    free(*history);
    *history = NULL;
    *hSize = 0;
}
/*Places wall on the right position with the right orientation*/
int playwall(element **A, int N, int *pWW, int *pWB, char *player, char *pos, char *orientation, char*** history, int* hSize){
    //make sure wall position is valid
    if (strlen(pos) > 3)
    {
        printf("? illegal move\n\n");
        return 0;
    }
    //Check if player is valid
    char p = 'E';
    char *playerBuff = toLow(player);
    if(playerBuff == NULL){  //malloc fail
        printf("? Not enough memory!\n\n");
        return 1;
    }
    if (strcmp(playerBuff,"black") == 0 || strcmp(playerBuff,"b") == 0)
        p = 'B';
    else if (strcmp(playerBuff,"white") == 0 || strcmp(playerBuff,"w") == 0)
        p = 'W';
    if (p == 'E'){ //invalid input
        printf("? invalid syntax\n\n");
        return 0;
    }
    free(playerBuff);
    //Find wall position
    vertex v;
    char *temp = toUpper(pos);
    if(temp == NULL){ //malloc failed
        printf("? Not enough memory!\n\n");
        return 1;
    }
    v.x = temp[0];
    free(temp);
    char numbers[3];
    int i, j;
    for(i = 0; i < strlen(pos) - 1; i++)
        numbers[i] = pos[i + 1];
    numbers[2] = '\0';
    v.y = atoi(numbers);
    //check if position is valid
    if(v.y <= 1 || v.y > N){         //walls cannit ve placed on the last row
        printf("? illegal move\n\n");
        return 0;
    }
    toArray(N, &v, &i, &j);
    if( j < 0 || j >= N-1  || A[i][j].w_or != ' '){  //walls cannot be placed on the last collumn
        printf("? illegal move\n\n");                //or on top of each other
        return 0;
    }
    //Find wall orientation
    char *orientBuff = toLow(orientation);
    if(orientBuff == NULL){ //malloc fail
        printf("? Not enough memory!\n\n");
        return 1;
    }
    char o = 'E';
    if(strcmp(orientBuff,"horizontal") == 0 || strcmp(orientBuff,"h") == 0)
        //checking if wall can be placed there
        if (j == 0 && A[i][j+1].w_or != 'H') //first collumn can't have a horizontal wall at the left it
            o = 'H';
        else if(A[i][j-1].w_or != 'H' && A[i][j+1].w_or != 'H')
            o = 'H';
        else{
            printf("? illegal move\n\n");
            return 0;
        }
    else if (strcmp(orientBuff,"vertical") == 0 || strcmp(orientBuff,"v") == 0)
        //checking if wall can be placed there
        if(i == 0 && A[i+1][j].w_or != 'V') //first row can't have a vertical wall above it
            o = 'V';
        else if(A[i-1][j].w_or != 'V' && A[i+1][j].w_or != 'V')
            o = 'V';
        else{
            printf("? illegal move\n\n");
            return 0;
        }
    free(orientBuff);
    //Removing 1 wall from said player if it exists
    if(p == 'B' && *pWB > 0)
            (*pWB)--;
    else if(p == 'W' && *pWW > 0)
            (*pWW)--; 
    else{
        printf("? illegal move\n\n");
        return 0;
    }
    //Placing wall with orientation
    A[i][j].w_or = o;
    printf("=\n\n");
    //Adding action to game history
    char action[7]; //This string will hold the description of the action performed
    char** tempS = realloc(*history, (++*hSize) * sizeof(char*));
    if (tempS == NULL){
        printf("? not enough memory!\n\n");
        return 1;
    }
    sprintf(action, "W%c%02d%c",A[i][j].V.x,A[i][j].V.y,o);
    tempS[*hSize - 1] = action;
    *history = tempS;
    return 0;
}

// Convert a string to all lowercase
char* toLow(char* string)
{
    int size = strlen(string);
    char* low = malloc((sizeof(char) * size + 1));

    if (low != NULL)
        for(int i = 0; i < size + 1; i++)
        {
            char new = string[i];
            if (new >= 'A' && new <= 'Z')
                new += 'a' - 'A';

            low[i] = new;
        }

    return low;
}
// Convert a string to all uppercase
char* toUpper(char* string)
{
    int size = strlen(string);
    char* cap = malloc((sizeof(char) * size + 1));

    if (cap != NULL)
        for(int i = 0; i < size + 1; i++)
        {
            char new = string[i];
            if (new >= 'a' && new <= 'z')
                new -= 'a' - 'A';

            cap[i] = new;
        }

    return cap;
}

// Convert array coordinates to vertex coordinates
void toVertex(int N, vertex* v, int i, int j)
{
    v->x = 'A' + j;
    v->y = N - i;
}

// Convert vertex coordinates to array coordinates
void toArray(int N, vertex* v, int* i, int* j)
{
    *i = N - v->y;
    *j = v->x - 'A';
}

// Add logic for walls and going onto ather player
int playmove(element **A, int N, char *player, char *pos, char *pWinner, char*** history, int* hSize)
{
    // Make sure the move is not random words
    if (strlen(pos) > 3)
    {
        printf("? illegal move\n\n");
        return 0;
    }
    char move[10];

    // Store the type of move
    char type = 'M';
    char p = 'E';
    char op = 'E';

    // Store in a char what player made the move
    char *playerBuff = toLow(player);
    if (playerBuff == NULL)
        return 1;

    if (strcmp(playerBuff,"black") == 0 || strcmp(playerBuff,"b") == 0)
    {
        p = 'B';
        op = 'W';
    }     
    else if (strcmp(playerBuff,"white") == 0 || strcmp(playerBuff,"w") == 0)
    {
        p = 'W';
        op = 'B';
    }
    
    // If char *player is invalid throw error
    if (p == 'E')
    {
        printf("? invalid syntax\n\n");
        return 0;
    }
    free(playerBuff);
    
    // Find the position in the matrix the move is at
    vertex v;
    char *temp = toUpper(pos);
    if (temp == NULL)
    {
        printf("? not enough memory!\n\n");
        return 1;
    }

    v.x = temp[0];
    free(temp);

    char numbers[3];
    int i, j;
    for(i = 0; i < strlen(pos) - 1; i++)
        numbers[i] = pos[i + 1];

    numbers[2] = '\0';
    v.y = atoi(numbers);

    // If outside range then it is an illegal move
    if(v.y <= 0 || v.y > N)
    {
        printf("? illegal move\n\n");
        return 0;
    }
    toArray(N, &v, &i, &j);
    if( j < 0 || j >= N )
    {
        printf("? illegal move\n\n");
        return 0;
    }

    // If vertex is not empty then is is an illegal move
    if(A[i][j].P != ' ')
    {
        printf("? illegal move\n\n");
        return 0;
    }

    // Find the previous position of the player
    int prevI = -1, prevJ = -1;

    short found = 0;
    int ind = 1;
    int vi = i, vj = j;
    char vp = p;
    
    /* Try to find a vertex adjascent to the target that the player is in. If not, try to find one with the opponent.
     * If it fails at both then it is an illegal move */
    while(!found)
    {
        int tempI = (ind < 3) ? ( (i > 0) ? vi - 1 : ( (i < N - 1) ? vi + 1 : -1) ) : i;
        int tempJ = (ind > 2) ? ( (j > 0) ? vj - 1 : ( (j < N - 1) ? vj + 1 : -1) ) : j;

        if ( tempI == -1 || tempJ == -1 ) 
            return 1;     //1 = panic = crash

        if(A[tempI][tempJ].P == vp)
        {
            prevI = tempI;
            prevJ = tempJ;
            
            if (vp == op)
                found = 2;
            else
                found = 1;
        }

        if (ind == 4 && !found)
        {
            if(vp == op)
                found = -1;
            else
            {
                vp = op;
                ind = 0;
            }
        }

        ind++;
    }
    
    /*
    if (i > 0)
        if (A[i - 1][j].P == p)
        {
            prevI = i-1;
            prevJ = j;
        }
        else if(A[i - 1][j].P == op)
        {

        }
    
    if (i < N - 1)
        if (A[i + 1][j].P == p)
        {
            prevI = i+1;
            prevJ = j;
        }
    
    if (j > 0)
        if (A[i][j - 1].P == p)
        {
            prevI = i;
            prevJ = j-1;
        }
    
    if (j < N - 1)
        if (A[i][j + 1].P == p)
        {
            prevI = i;
            prevJ = j+1;
        }
    */
    // If you didn't find somewhere adjascent then it is an illegal move
    if (found == -1)
    {
        printf("123\n"); //problem is here
        printf("? illegal move\n\n");
        return 0;
    }
    // If you found the oponent then search the adjascent vertex in the direction of the move. If there isn't the player there, then illegal
    else if (found == 2)
    {
        int diffI = i - prevI;
        int diffJ = j - prevJ;
        if (A[prevI - diffI][prevJ - diffJ].P != p)
        {
            printf("? illegal move\n\n");
            return 0;
        }
        else
        {
            prevI -= diffI;
            prevJ -= diffJ;
        }
    }

    // Move the player
    A[i][j].P = p;
    A[prevI][prevJ].P = ' ';

    // If history is not passed as NULL then store the move and increament move count
    if (history != NULL)
    {
        vertex prevV;
        toVertex(N, &prevV, prevI, prevJ); //could use A[prevI][prevJ].v
        
        sprintf(move, "%c%c%02d>%c%02d%c", type, prevV.x, prevV.y, v.x, v.y, p);

        char** temp = realloc(*history, (++*hSize) * sizeof(char*));
        if (temp == NULL)
        {
            printf("? not enough memory!\n\n");
            return 1;
        }

        temp[*hSize - 1] = move;
    
        *history = temp;
    }

    // If player is white and is at the end of the board or is black and its at the start, then we have a winner
    if((p == 'W' && i == 0) || (p == 'B' && i == N-1))
        *pWinner = p;

    printf("=\n\n");

    return 0;
}

int abs(int n)
{
    if (n < 0)
        return -1*n;
    else
        return n;
}