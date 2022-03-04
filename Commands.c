#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Commands.h"
#include "utilities.h"
#include "pathfinder.h"
#include "minimax.h"

int insert_at_start(node **start, char *string)
{
    node *temp = *start;
    *start = malloc(sizeof(node));
    if (*start == NULL)
    {
        *start = temp;
        return 0;
    }

    (*start)->move = string;
    (*start)->nextNode = temp;

    return 1;
}

void remove_at_start(node** start)
{
    if ((*start) != NULL)
    {
        node *next = (*start)->nextNode;
        free((*start)->move);
        free(*start);
        *start = next;
    }
}

/* I don't know how to make this more readable, im sorry */
void showboard(element **A,int N,int WW,int WB){
    int i,j;
    for(i = 1;i <= (2*N + 3);i++){  // Board big enough to show all rows,collumns and the coordinates                
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
            fflush(stdout);
        }
        // Print wall numbers on the side
        if(i == 3){
            printf("black walls: %d",WB);
            fflush(stdout);
        }
        if(i == 5){
            printf("white walls: %d",WW);
            fflush(stdout);
        } 
        
        printf("\n");
        fflush(stdout);
    }
    printf("\n\n");
    fflush(stdout);
}

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

void clearboard(element **A, int N, node **history, int *hSize){
    int i,j;
    // Board Configuration at the start of the game
    for(i = 0;i < N;i++){
        for(j = 0;j < N;j++){
            // Players
            if(i == 0 && j == (int)(N/2))        // Black starting position
                A[i][j].P = 'B';
            else if(i == N-1 && j == (int)(N/2)) // White starting position
                A[i][j].P = 'W';
            else 
                A[i][j].P = ' ';
            
            // Walls
            A[i][j].w_or = ' ';
        }
    }

    // Game history = empty
    for(i = 0; i < *hSize; i++)
    {
        remove_at_start(history);
    }
    *hSize = 0;
}

int playwall(element **A, int N, int *pWW, int *pWB, char *player, char *pos, char *orientation, node **history, int* hSize, int print){
    // Make sure wall position is valid
    char p = 'E';
    char *playerBuff = toLow(player);
    if(playerBuff == NULL){  // Malloc fail
        printf("? Not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }
    if (strcmp(playerBuff,"black") == 0 || strcmp(playerBuff,"b") == 0)
        p = 'B';
    else if (strcmp(playerBuff,"white") == 0 || strcmp(playerBuff,"w") == 0)
        p = 'W';
    if (p == 'E'){ // Invalid input
        printf("? invalid syntax \n\n");
        fflush(stdout);
        return 0;
    }
    free(playerBuff);

    // Find wall position
    vertex v;
    char *temp = toUpper(pos);
    if(temp == NULL){ // Malloc failed
        printf("? Not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }
    v.x = temp[0];
    free(temp);
    int i, j;
    for(i = 1; i < strlen(pos); i++)
        if(pos[i] < '0' || pos[i] > '9'){ 
            printf("? invalid syntax \n\n");
            fflush(stdout);
            return 0;
        }
    
    v.y = atoi(pos + 1);

    // Check if position is valid
    if(v.y <= 1 || v.y > N){         // Walls cannot be placed on the last row
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }
    toArray(N, &v, &i, &j);
    if( j < 0 || j >= N-1  || A[i][j].w_or != ' '){  // Walls cannot be placed on the last collumn
        printf("? illegal move \n\n");               // Or on top of each other
        fflush(stdout);
        return 0;
    }

    //Find wall orientation
    char *orientBuff = toLow(orientation);
    if(orientBuff == NULL){ //malloc fail
        printf("? Not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }
    char o = 'E';
    if(strcmp(orientBuff,"horizontal") == 0 || strcmp(orientBuff,"h") == 0)
        // Checking if wall can be placed there
        if (j == 0){ // First collumn can't have a horizontal wall at the left it
            if(A[i][j+1].w_or != 'H')
                o = 'H';
        }
        else if(A[i][j-1].w_or != 'H' && A[i][j+1].w_or != 'H')
            o = 'H';
        else{
            printf("? illegal move \n\n");
            fflush(stdout);
            return 0;
        }
    else if (strcmp(orientBuff,"vertical") == 0 || strcmp(orientBuff,"v") == 0)
        // Checking if wall can be placed there
        if(i == 0){ // First row can't have a vertical wall above it
            if(A[i+1][j].w_or != 'V')
                o = 'V';
        }
        else if(A[i-1][j].w_or != 'V' && A[i+1][j].w_or != 'V')
            o = 'V';
        else{
            printf("? illegal move \n\n");
            fflush(stdout);
            return 0;
        }
    if(o == 'E'){
        printf("? invalid syntax \n\n");
        fflush(stdout);
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
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }

    // Same for white player
    Path = pathfinder(A, N, 'W', -1, -1);
    if(Path == -100) return 1; // Malloc/find failed (PANIC)
    else if(Path == -1){
        A[i][j].w_or = ' ';
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }

    // Removing 1 wall from said player if it exists
    if(p == 'B' && *pWB > 0)
        (*pWB)--;
    else if(p == 'W' && *pWW > 0)
        (*pWW)--; 
    if(print){
        printf("=  \n\n");
        fflush(stdout);
    }

    // Adding action to game history
    char *action = malloc(sizeof(char) * 8); // This string will hold the description of the action performed
    vertex V;
    toVertex(N, &V, i, j);
    sprintf(action, "W%c%02d%c%c",V.x,V.y,o, p);

    int check = insert_at_start(history, action);
    if (check == 0){
        printf("? not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }
    ++*hSize;
    return 0;
}

int undo(int times, element **A, int N, int *pWW, int *pWB, char *pWinner, node **history, int *hSize){
    
    for (int i = 0; i < times; i++)
    {
        char type = (*history)->move[0];

        // Decodes the move and undoes its action one at a time
        if (type == 'M')
        {
            char sLet, tLet, player;
            int sNum, tNum;

            sscanf((*history)->move, "%c%c%d>%c%d%c", &type, &sLet, &sNum, &tLet, &tNum, &player);

            vertex start, target;
            start.x = sLet;
            start.y = sNum;
            target.x = tLet;
            target.y = tNum;

            int si, sj, ti, tj;
            toArray(N, &start, &si, &sj);
            toArray(N, &target, &ti, &tj);

            if(A[ti][tj].P != player)
            {
                printf("? Didn't find player to undo \n\n");
                fflush(stdout);
                return 1;
            }

            A[ti][tj].P = ' ';
            A[si][sj].P = player;
            if((player == 'W' && ti == 0) || (player == 'B' && ti == N - 1))
                *pWinner = '\0';
        }
        else  // type == 'W'
        {
            char tLet, player, orient;
            int tNum;
            sscanf((*history)->move, "%c%c%d%c%c", &type, &tLet, &tNum, &orient, &player);

            vertex target;
            target.x = tLet;
            target.y = tNum;

            int ti, tj;
            toArray(N, &target, &ti, &tj);

            if(A[ti][tj].w_or != orient)
            {
                printf("? Didn't find correct wall to undo \n\n");
                fflush(stdout);
                return 1;
            }

            A[ti][tj].w_or = ' ';
            if(player == 'W')
                (*pWW)++;
            else // player == 'B'
                (*pWB)++;
        }

        // At the end of the loop remove the move that just got undone
        remove_at_start(history);
    }
    // Decrease the size of the history by times
    *hSize -= times;
    
    return 0;
}

int playmove(element **A, int N, char *player, char *pos, char *pWinner, node** history, int* hSize, int print)
{
    // Make sure the move is not random words
    char *move = malloc(sizeof(char) * 10);   //WHAT IS DIS

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
        printf("? invalid syntax \n\n");
        fflush(stdout);
        return 0;
    }
    free(playerBuff);
    
    // Find the position in the matrix the move is at
    vertex v;
    char *temp = toUpper(pos);
    if (temp == NULL)
    {
        printf("? not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }

    v.x = temp[0];
    free(temp);

    int i, j;
    for(i = 1; i < strlen(pos); i++)
        if(pos[i] < '0' || pos[i] > '9'){
            printf("? invalid syntax \n\n");
            fflush(stdout);
            return 0;
        }
    
    v.y = atoi(pos + 1);

    // If outside range then it is an illegal move
    if(v.y <= 0 || v.y > N)
    {
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }
    toArray(N, &v, &i, &j);
    if( j < 0 || j >= N )
    {
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }

    // If vertex is not empty then is is an illegal move
    if(A[i][j].P != ' ')
    {
        printf("? illegal move \n\n");
        fflush(stdout);
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
    
    // If you didn't find somewhere adjascent then it is an illegal move
    if (found == -1)
    {
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }
    if (!connected(A, prevI, prevJ, i, j))
    {
        printf("? illegal move \n\n");
        fflush(stdout);
        return 0;
    }

    /* If you found the oponent then search the adjascent vertexes to find the player. If you cannot, then it is an illegal move. If you find him, then check the
     * vertec in the direction of the target to the opponent. if the player is not there, there, then check whether or not there is a wall or the end of the board.
     * If not then it is an illegal move. If yes then check whether the player is on the right or left of the oppenet, relative to his position. If he is  not then
     * it is an illegal move */
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
            printf("? illegal move \n\n");
            fflush(stdout);
            return 0;
        }

        if (!connected(A, pI, pJ, prevI, prevJ))
        {
            printf("? illegal move \n\n");
            fflush(stdout);
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
                printf("? illegal move \n\n");
                fflush(stdout);
                return 0;
            }

            prevI = pI;
            prevJ = pJ;
        }
    }

    // Move the player
    A[i][j].P = p;
    A[prevI][prevJ].P = ' ';

    // If history is not passed as NULL then store the move and increament move count
    if (history != NULL) //IS THIS NEEDED?
    {
        vertex prevV;
        toVertex(N, &prevV, prevI, prevJ);
        
        sprintf(move, "%c%c%02d>%c%02d%c", type, prevV.x, prevV.y, v.x, v.y, p);

        int check = insert_at_start(history, move);
        if (check == 0)
        {
            printf("? not enough memory! \n\n");
            fflush(stdout);
            return 1;
        }
        ++*hSize;
    }

    // If player is white and is at the end of the board or is black and its at the start, then we have a winner
    if((p == 'W' && i == 0) || (p == 'B' && i == N-1))
        *pWinner = p;

    if(print)
    {
        printf("=  \n\n");
        fflush(stdout);
    }    
    
    return 0;
}

/* Calls minimax with the apropriate parameters and executes the move minimax chooses */
int genmove(element **A, int N, char *player, int *pWW, int *pWB, char *pWinner, node **history, int *hSize){
    char p,op;
    char *playerBuff = toLow(player);
    if (playerBuff == NULL)
        return 1; // Maloc failed (PANIC)

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
        printf("? invalid syntax \n\n");
        fflush(stdout);
        return 0;
    }
    free(playerBuff);

    // Possible moves = 4(max 4 directions to move) + every possible wall placement
    int possiblemoves = 4 + (N-1)*(N-1)*2;
    int i,j;
    find(A, N, p, &i, &j);
    int move = 1; // This variable will represent the move chosen by minimax

    //printf("Before minimax\n");

    // Call minimax to find the best move (White = maximizing player, Black = minimizing player)
    // We call the function starting with 1 or 0 for the maximizing player depending on p
    int check = minimax(A, N, pWW, pWB, pWinner, 3, INT_MIN, INT_MAX, (p == 'W') ? 1 : 0, possiblemoves, &move, history, hSize);

    if(check == INT_MIN + 1)
        return 0;
        

    // Interpret move and call the proper function
    return execute(A, N, move, i, j, pWW, pWB, p, pWinner, history, hSize, 1); // Function exists in utilities.c
}