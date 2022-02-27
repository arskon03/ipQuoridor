#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "pathfinder.h"
#include "utilities.h"

/* I don't know how to make this more readable, im sorry */
void showboard(element **A,int N,int WW,int WB){
    int i,j;
    for(i = 1;i <= (2*N + 3);i++){     // Board big enough to show all rows,collumns and the coordinates                
        for(j = 1;j <= N+2;j++){ 
            // First and last lines are always the coordinates
            if(i == 1 || i == (2*N+3)){                 
                if(j == 1 || j == N+2) printf("    ");
                else printf(" %c  ", 'A'+(j-2));
            }
            // Second and second to last lines are always the same
            else if(i == 2 || i == (2*N+2)){
                if (j == 1 ) printf("   +");
                else if(j == N+2) printf("    ");
                else printf("---+");
            }
            // Rows with blocks of the board
            else if(i%2 == 1){
                if(j == 1) printf("%2d |",N-(i/2)+1);
                else if(j == N+2) printf(" %d  ",N-(i/2)+1);
                else{
                    printf(" %c ",A[(i-3)/2][j-2].P);
                    // Printing 'H' instead of '|' for vertical wall if it exists
                    if(i == 3) printf("%c", (A[(i-3)/2][j-2].w_or == 'V') ? 'H' : '|');
                    else printf("%c", (A[(i/2)-1][j-2].w_or == 'V' || A[(i/2)-2][j-2].w_or == 'V') ? 'H' : '|');
                }
            }
            // Rows between the blocks
            else{ // i%2 == 0
                if(j == 1) printf("   +");
                else if(j == N+2) printf("    ");
                else if (j == 2){
                    for(int k = 1; k <= 3; k++)
                        printf("%c",(A[(i/2)-2][j-2].w_or == 'H') ? '=' : '-');
                    printf("%c",(A[(i/2)-2][j-2].w_or == 'H') ? '=' : (A[(i/2)-2][j-2].w_or == 'V') ? 'H' : '+');
                } // Printing '=' instead of '-' for horizontal wall if it exists
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

/* Resets the size of the board and the rest are considered arbitrary */
int boardsize(element ***A, int nValue, int *pN){
    int i;
    // Free previous matrix
    if(*A != NULL){
        for(i = 0;i < *pN;i++)
            free((*A)[i]);
        free(*A);
    }
    // Allocate matrix with the new size
    element **temp = malloc(nValue * sizeof(element *));
    if(temp == NULL) return 1;
    for (i = 0;i < nValue;i++){
        temp[i] = malloc(nValue * sizeof(element));
        if (temp[i] == NULL) return 1;
    }
    *pN = nValue;
    *A = temp;
    return 0;
}

/* Walls cleared/Players at starting positions/Game history empty */
void clearboard(element **A, int N, char ***history, int *hSize){
    int i,j;
    // Board Configuration at the start of the game
    for(i = 0;i < N;i++)
        for(j = 0;j < N;j++){
            if(i == 0 && j == (int)(N/2))        // Black starting position
                A[i][j].P = 'B';
            else if(i == N-1 && j == (int)(N/2)) // White starting position
                A[i][j].P = 'W';
            else 
                A[i][j].P = ' ';
            
            A[i][j].w_or = ' ';
            toVertex(N, &A[i][j].V, i, j);
            /*A[i][j].V.x = 'A' + j;
            A[i][j].V.y = N - i;*/
        }
    // Game history = empty
    free(*history); 
    *history = NULL;
    *hSize = 0;
}

/*Places wall on the right position with the right orientation*/
int playwall(element **A, int N, int *pWW, int *pWB, char *player, char *pos, char *orientation, char*** history, int* hSize){
    // Make sure wall position is valid
    /*if (strlen(pos) > 3)
    {
        printf("? illegal move\n\n");
        return 0;
    }*/
    // Check if player is valid
    char p = 'E';
    char *playerBuff = toLow(player);
    if(playerBuff == NULL){  // Malloc fail
        printf("? Not enough memory!\n\n");
        return 1;
    }
    if (strcmp(playerBuff,"black") == 0 || strcmp(playerBuff,"b") == 0)
        p = 'B';
    else if (strcmp(playerBuff,"white") == 0 || strcmp(playerBuff,"w") == 0)
        p = 'W';
    if (p == 'E'){ // Invalid input
        printf("? invalid syntax\n\n");
        return 0;
    }
    free(playerBuff);
    // Find wall position
    vertex v;
    char *temp = toUpper(pos);
    if(temp == NULL){ // Malloc failed
        printf("? Not enough memory!\n\n");
        return 1;
    }
    v.x = temp[0];
    free(temp);
    char numbers[3];
    int i, j;
    for(i = 0; i < strlen(pos) - 1; i++)
        if(pos[i + 1] < '0' || pos[i + 1] > '9'){   // PLAYMOVE NEEDS THIS
            printf("? invalid syntax\n\n");
            return 0;
        }
        else
            numbers[i] = pos[i + 1];
    numbers[2] = '\0';
    v.y = atoi(numbers);
    // Check if position is valid
    if(v.y <= 1 || v.y > N){         // Walls cannot be placed on the last row
        printf("? illegal move\n\n");
        return 0;
    }
    toArray(N, &v, &i, &j);
    if( j < 0 || j >= N-1  || A[i][j].w_or != ' '){  // Walls cannot be placed on the last collumn
        printf("? illegal move\n\n");                // Or on top of each other
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
        // Checking if wall can be placed there
        if (j == 0 && A[i][j+1].w_or != 'H') // First collumn can't have a horizontal wall at the left it
            o = 'H';
        else if(A[i][j-1].w_or != 'H' && A[i][j+1].w_or != 'H')
            o = 'H';
        else{
            printf("? illegal move\n\n");
            return 0;
        }
    else if (strcmp(orientBuff,"vertical") == 0 || strcmp(orientBuff,"v") == 0)
        // Checking if wall can be placed there
        if(i == 0 && A[i+1][j].w_or != 'V') // First row can't have a vertical wall above it
            o = 'V';
        else if(A[i-1][j].w_or != 'V' && A[i+1][j].w_or != 'V')
            o = 'V';
        else{
            printf("? illegal move\n\n");
            return 0;
        }
    if(o == 'E'){
        printf("? invalid syntax\n\n");
        return 0;
    }
    free(orientBuff);
    // Placing wall with orientation
    A[i][j].w_or = o;
    // Check if wall is blocking the path of Black
    int pr,pc;
    int Path = pathfinder(A, N, 'B', -1, -1);
    if(Path == -100) return 1; // Malloc/find failed (PANIC)
    else if(Path == -1){
        // If path is blocked remove the wall and the move is illegal
        A[i][j].w_or = ' ';
        printf("? illegal move\n\n");
        return 0;
    }
    // Same for white player
    Path = pathfinder(A, N, 'W', -1, -1);
    if(Path == -100) return 1; // Malloc/find failed (PANIC)
    else if(Path == -1){
        A[i][j].w_or = ' ';
        printf("? illegal move\n\n");
        return 0;
    }
    // Removing 1 wall from said player if it exists
    if(p == 'B' && *pWB > 0)
        (*pWB)--;
    else if(p == 'W' && *pWW > 0)
        (*pWW)--; 
    printf("= \n\n");
    // Adding action to game history
    char action[8]; // This string will hold the description of the action performed
    char** tempS = realloc(*history, (++*hSize) * sizeof(char*));
    if (tempS == NULL){
        printf("? not enough memory!\n\n");
        return 1;
    }
    sprintf(action, "W%c%02d%c%c",A[i][j].V.x,A[i][j].V.y,o, p);
    tempS[*hSize - 1] = action;
    //tempS[*hSize] = '\0';
    *history = tempS;
    return 0;
}

/* Undoes last move a given number of times */
void undo(int times, element **A, int N, int *pWW, int *pWB, char *pWinner,char ***history, int *hSize){
    int i;
    for(i = 0;i < times;i++){
        
    }
}

/* Executes the given move if its legal */
int playmove(element **A, int N, char *player, char *pos, char *pWinner, char*** history, int* hSize)
{
    // Make sure the move is not random words
    /*
    if (strlen(pos) > 3)
    {
        printf("? illegal move\n\n");
        return 0;
    }
    */
    char move[10];

    // Store the type of move
    char type = 'M';
    char p = 'E';
    char op = 'E';

    // Store in a char what player made the move
    char *playerBuff = toLow(player);
    if (playerBuff == NULL)
        return 1; // Malloc failed (PANIC)

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
    for(i = 0; i < strlen(pos) - 1; i++){
        if(pos[i + 1] < '0' || pos[i + 1] > '9'){   // PLAYMOVE NEEDS THIS
            printf("? invalid syntax\n\n");
            return 0;
        }
        else
            numbers[i] = pos[i + 1];
    }

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
        int tempI = (ind < 3) ? ( (i > 0 && ind == 1) ? vi - 1 : ( (i < N - 1) ? vi + 1 : vi) ) : vi;
        int tempJ = (ind > 2) ? ( (j > 0 && ind == 3) ? vj - 1 : ( (j < N - 1) ? vj + 1 : vj) ) : vj;
         printf("vi: %d, vj: %d, tempI: %d, tempJ: %d\n", vi, vj, tempI, tempJ);

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
    //printf("prevI: %d, prevJ: %d\n", prevI, prevJ);
    // If you didn't find somewhere adjascent then it is an illegal move
    if (found == -1)
    {
        //printf("123\n"); //problem is here
        printf("? illegal move\n\n");
        return 0;
    }
    if (!connected(A, prevI, prevJ, i, j))
    {
        printf("? illegal move\n\n");
        return 0;
    }
    //printf("prevI: %d, prevJ: %d\n", prevI, prevJ);

    // If you found the oponent then search the adjascent vertex in the direction of the move. If there isn't the player there, then illegal
    if (found == 2)
    {
        found = 0;
        int di[4] = {-1, +1, 0, 0}; // Direction vectors for rows
        int dj[4] = {0, 0, +1, -1}; // Direction vectors for collumns

        int pJ = 0;
        int pI = 0;
        for (int ind = 0; ind < 4; ind++)
        {
            pJ = (prevJ + dj[ind] >= 0 && prevJ + dj[ind] < N) ? prevJ + dj[ind] : prevJ;
            pI = (prevI + di[ind] >= 0 && prevI + di[ind] < N) ? prevI + di[ind] : prevI;

            if (A[pI][pJ].P == p)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("? illegal move\n\n");
            return 0;
        }

        if (!connected(A, pI, pJ, prevI, prevJ))
        {
            printf("? illegal move\n\n");
            return 0;
        }

        int diffI = i - prevI;
        int diffJ = j - prevJ;

        if (prevI - diffI == pI && prevJ - diffJ == pJ)
        {
            prevI = pI;
            prevJ = pJ;
        }
        else
        {
            diffI = pI - prevI;
            diffJ = pJ - prevJ;

            int bI = prevI - diffI;
            int bJ = prevJ - diffJ;
            short bCheck = 0;

            if ((bI < 0 || bI >= N) || (bJ < 0 || bJ >= N))
                bCheck = 1;
            
            if (!bCheck && (!connected(A, prevI, prevJ, bI, bJ)))
                bCheck = 1;

            if (!bCheck)
            {
                printf("? illegal move\n\n");
                return 0;
            }

            prevI = pI;
            prevJ = pJ;
        }
        /*
        if (checkI && checkJ)
        {
            if (A[prevI - diffI][prevJ - diffJ].P == p)
            {
                prevI -= diffI;
                prevJ -= diffJ;
                found = 1;
            }
        }
        int pI = 0, pJ = 0;
        //find(A, N, p, &pI, &pJ);

        if(found == -1 && !connected(A, prevI, prevJ, i, j)) // In genmove.h
        {
            int vcoord = 0;
            found = 0;
            ind = 1;

            if (diffI == 0)
                vcoord = prevI;
            else
                vcoord = prevJ;

            while (!found)
            {
                int tempI = (vcoord == prevI) ? ((ind == 1 && prevI > 0) ? prevI - 1 : ((prevI < N - 1) ? prevI + 1 : prevI)) : prevI;
                int tempJ = (vcoord == prevJ) ? ((ind == 1 && prevJ > 0) ? prevJ - 1 : ((prevJ < N - 1) ? prevJ + 1 : prevJ)) : prevJ;
                printf("prevI: %d, prevJ: %d, tempI: %d, tempJ: %d\n", prevI, prevJ, tempI, tempJ);

                if (A[tempI][tempJ].P == p)
                {
                    prevI = tempI;
                    prevJ = tempJ;
                    found = 1;
                }

                if (ind == 2 && found == 0)
                    found = -1;
            }
        }
        
        if (found == -1)
        {
            printf("? illegal move\n\n");
            return 0;
        }
        */
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

/* Calls minimax with the apropriate parameters and executes the move minimax chooses */
void genmove(element **A, int N, char *player, int *pWW, int *pWB, char *pWinner, char*** history, int *hSize){
    char p,op;
    char *playerBuff = toLow(player);
    if (playerBuff == NULL)
        return NULL; // Maloc failed (PANIC)

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

    // Possible moves = 4(max 4 directions to move) + every possible wall placement
    int possiblemoves = 4 + (N-1)*(N-1)*2;
    int i,j;
    int move; // This variable will represent the move chosen by minimax

    // Find said player's position and call minimax to find the best move
    find(A,N,p,&i,&j);
    minimax(A, N, pWW, pWB, pWinner, i, j, 3, -1000000, 1000000, 1, possiblemoves, &move, history, hSize);

    // Interpret move and call the proper function
    execute(A, N, move, i, j, pWW, pWB, p, pWinner, history, hSize, 1); // Function exists in utilities.c
    return;
}

