#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include "utilities.h"
#include "pathfinder.h"

/* Interpret's input and responds accordingly by calling the proper function */
int Command(char *input, element ***A, int *pN, int *pWW, int *pWB, char *pWinner,node **history,int *hSize); //0/-1 = proper execution, 1 = Panic situation

int main(void){
    int N = 0, WW, WB = 10, i, Panic = 0,hSize=0;
    char Winner = '\0', input[256]; 
    node *history = NULL; // History is NULL so clearboard can free it
    element **A = NULL; // A will hold player positions/Wall positions and orientations

    /*Initializing with default values*/
    Panic = boardsize(&A,9,&N);
    if(Panic == 1){ // Malloc failed
        printf("? Not enough memory! \n\n");
        fflush(stdout);
        return 1;
    }
    clearboard(A,N,&history,&hSize);
    WW = WB = 10;

    /* Preprocessing input before calling command function */
    char temp[256]; // Temp will hold the preprocessed string according to the protocol
    while(Panic == 0){ // Loop until Panic or quit command
        fgets(input,sizeof(input),stdin);
        if(*input == '\n') continue; // A single newline character is not considered a command
            for(int i = 0;i < strlen(input);i++){
                // Remove all control charactes (except for HT and LF) and replace HT with blank characters   
                if((input[i] >= 1 && input[i] <= 31) || input[i] == 127){
                    if(input[i] == '\t'){
                        temp[i] = ' ';
                        continue;
                    } 
                    temp[i] = input[i+1];
                    i++;
                    continue;
                }
                // End string if input has comments (#)
                if(input[i] == '#'){
                    temp[i] == '\0'; 
                    break;
                }
                temp[i] = input[i];
            }   
        Panic = Command(temp,&A,&N,&WW,&WB,&Winner,&history,&hSize);  // Interprets command based on the given string 
    }

    if (Panic == 1) return 1; // Panic situation 

    for(i = 0;i < N;i++)
        free(A[i]);
    free(A);

    for(i = 0; i < hSize;i++)
        remove_at_start(&history);

    if (Panic == -1) return 0;  // User gave quit command
    
    return 0;
}

int Command(char *input,element ***A,int *pN,int *pWW,int *pWB,char *pWinner,node **history,int *hSize){
    if(input == NULL) return 1; // PANIC 

    char *com = NULL,*arg1 = NULL,*arg2 = NULL,*arg3 = NULL;  // Tokens extracted from original string                                       
    com = strtok(input," \n");                                // \n is needed as a delimiter in order to be replaced by a '\0'

    /* Only blank characters on input */
    if(com == NULL){
        printf("? unknown command \n\n");
        fflush(stdout);
    }

    /* Command: name */
    else if(strcmp(com,"name") == 0){
        printf("= 21-083.21-033 \n\n");
        fflush(stdout);
    }

    /* Command: known_command */
    else if (strcmp(com,"known_command") == 0){
        arg1 = strtok(NULL," \n");
        if (strcmp(arg1,"name") == 0 || strcmp(arg1,"known_command") == 0)
            printf("= true \n\n");
        else if(strcmp(arg1,"list_commands") == 0 || strcmp(arg1,"quit") == 0)    // Trying to evade huge lines
            printf("= true \n\n");
        else if(strcmp(arg1,"boardsize") == 0 || strcmp(arg1,"clear_board") == 0)
            printf("= true \n\n");
        else if (strcmp(arg1,"walls") == 0 || strcmp(arg1,"playmove") == 0)
            printf("= true \n\n");
        else if(strcmp(arg1,"playwall") == 0 || strcmp(arg1,"genmove") == 0)
            printf("= true \n\n");
        else if(strcmp(arg1,"undo") == 0 || strcmp(arg1,"winner") == 0 || strcmp(arg1,"showboard") == 0)
            printf("= true \n\n");
        else printf("? false \n\n");

        fflush(stdout);
    }

    /* Command: list_commands */
    else if(strcmp(com,"list_commands") == 0){
        printf("=\nname\nknown_command\nlist_commands\nquit\nboardsize\n");
        fflush(stdout);
        printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard \n\n");
        fflush(stdout);
    }

    /* Command: quit */
    else if(strcmp(com,"quit") == 0){
        printf("= \n\n");
        fflush(stdout);
        return -1;
    }

    /* Command: undo */
    else if(strcmp(com,"undo") == 0){
        int times;
        // Get given number of actions to undo based on input
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL)
            times = 1;
        else
            times = atoi(arg1);
        if(times <= 0) // Didn't add error messages (this response is based on naive's response)
        {
            printf("= \n\n");
            fflush(stdout);
        } 
        else if (times <= *hSize){ 
            printf("= \n\n");
            fflush(stdout);
            if(undo(times,*A,*pN,pWW,pWB,pWinner,history,hSize)) return 1; // PANIC
        }
        else // times > *hSize 
        {
            printf("? cannot undo \n\n");
            fflush(stdout);
        } 
    }

    /* Command: boardsize */
    else if(strcmp(com,"boardsize") == 0){
        // Get new boardsize base on given input
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL){ 
            printf("? invalid syntax \n\n");
            fflush(stdout);
            return 0;
        }
        int tempN = atoi(arg1);
        // Check if boardsize is valid and call function
        if(tempN > 0 && tempN <= 25 && tempN%2 == 1){
            int P = boardsize(A,tempN,pN);
            if(P == 1) return 1;  // Malloc failed (PANIC)
            printf("= \n\n");
            fflush(stdout);
        }
        else
        {
            printf("? unacceptable size \n\n");
            fflush(stdout);
        } 
    }

    /* Command: clear_board */
    else if(strcmp(com,"clear_board") == 0){
        printf("= \n\n");
        fflush(stdout);
        clearboard(*A,*pN,history,hSize);
    }

    /* Command: walls */
    else if(strcmp(com,"walls") == 0){
        // Get new number of walls based on given input
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL){
            printf("? invalid syntax \n\n");
            fflush(stdout);
            return 0;
        }
        int W = atoi(arg1);

        // Check if 0 is passed as an arguement or its a failed string conversion
        if(W == 0 && *arg1 != '0'){
            printf("? invalid syntax \n\n");
            fflush(stdout);
            return 0;
        }

        *pWW = *pWB = W;
        printf("= \n\n");
        fflush(stdout);
    }

    /* Command: showboard */
    else if(strcmp(com,"showboard") == 0){
        printf("= \n");
        fflush(stdout);
        showboard(*A,*pN,*pWW,*pWB);
    }

    /* Command: playmove */
    else if(strcmp(com,"playmove") == 0){
        // Get player and position to move based on given input
        arg1 = strtok(NULL," \n");
        arg2 = strtok(NULL," \n");
        if(arg1 == NULL || arg2 == NULL)  // Player and position are needed to execute the move
        {
            printf("? invalid syntax\n");  
            fflush(stdout);  
        }
        else{
            int P = playmove(*A,*pN,arg1,arg2,pWinner,history,hSize, 1);
            if(P == 1) return 1;     //malloc failed (PANIC)
        }
    }

    /* Command: playwall */
    else if(strcmp(com,"playwall") == 0){
        // Get player/wall position/wall orientation based on given input
        arg1 = strtok(NULL," \n");
        arg2 = strtok(NULL," \n");
        arg3 = strtok(NULL," \n");
        if(arg1 == NULL || arg2 == NULL || arg3 == NULL)   // Player,position,and wall direction are neeeded to execute the move
        {
            printf("? invalid syntax \n\n");
            fflush(stdout); 
        }
        else{
            int P = playwall(*A,*pN,pWW,pWB,arg1,arg2,arg3,history,hSize, 1);
            if(P == 1) return 1;     // Malloc failed (PANIC)
        }
    }

    /* Command: genmove */
    else if(strcmp(com,"genmove") == 0){
        // Find player based on given input
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL){ //might need correction
            printf("? invalid syntax \n\n");
            fflush(stdout);
        }
        else{
            // Generate best action(move or wall placement)                                     
            if(!genmove(*A, *pN, arg1, pWW, pWB, pWinner, history, hSize)) return 1;
        }
    }

    /* Command: winner */
    else if(strcmp(com,"winner") == 0){
        // Check if there's a winner and print the
        if(*pWinner == 'B') printf("= true black \n\n");
        else if(*pWinner == 'W') printf("= true white \n\n");
        else printf("= false \n\n");
        fflush(stdout);
    }
    else{
        printf("? unknown command \n\n");
        fflush(stdout);
    } 
    return 0;
}