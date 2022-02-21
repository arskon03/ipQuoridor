#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"

int Command(char *input, element **A, int *pN, int *pWW, int *pWB, char *pWinner,char ***history,int *hSize);     //0 = proper execution, 1 = Panic situation

int main(int argc, char **argv){             //board size and number of walls for each player
    int N = 9, WW = 10, WB = 10, i, Panic;

    char Winner = '\0', input[30];   //(INPUT NEEDS TO BE BIGGER)
    element **A;                     //A will hold player positions/Wall positions/orientation and the coordinates as vertices
    /*if(argc == 1){      

        N = 9;
        WW = WB = 10;
    }
    else if(argc == 2){
        if(atoi(argv[1]+1)%2 == 1) N = atoi(argv[1]+1);               //dimension must be odd
        else{ 
            printf("Only odd numbers are accepted as dimensions\n");
            return 1;
        }
        WW = WB = 10;
    }
    else if (argc == 3){
        if(atoi(argv[1]+1)%2 == 1) N = atoi(argv[1]+1);
        else{
            printf("Only odd numbers are accepted as dimensions\n");
            return 1;
        }
        WW = WB = atoi(argv[2]+1);
    }
    else{
        printf("Program Usage: ./ipquoridor -<size> -<walls>");
        return 1;
    }*/
    printf("Dimension = %d\nWalls = %d\n",N,WW);              //TEST (CHECK)
    /*Initializing with default values*/
    A = malloc(N*sizeof(element *));
    if (A == NULL){
        printf("? Not enough memory!\n\n");
        return 1;
    }
    for (i = 0;i < N;i++){
        A[i] = malloc(N*sizeof(element));
        //if(i < N-1) W[i] = malloc((N-1)*sizeof(char));
        if (A[i] == NULL){
            printf("? Not enough memory!\n\n");
            return 1;
        }
        for(int j = 0;j < N;j++){                     //Starting values of P
            if(i == 0 && j == (int)(N/2)){
                A[i][j].P = 'B';
            }
            else if(i == N-1 && j == (int)(N/2)){
                A[i][j].P = 'W';
            }
            else A[i][j].P = ' ';
            A[i][j].w_or = ' ';                     //Starting values of w_or
            A[i][j].V.x = 'A' + j;
            A[i][j].V.y = N - i;
            printf("%c %c %c %d\t",A[i][j].P,A[i][j].w_or,A[i][j].V.x,A[i][j].V.y);
        }
        printf("\n");
    }
    int hSize = 0;
    char **history;
    char temp[30]; //temp will hold the preprocessed string according to the protocol
    /*Preprocessing input before calling command function*/
    while(Winner == '\0'){
        fgets(input,sizeof(input),stdin);
            for(int i = 0;i < 30;i++){         
                if(input[i] >= 1 && input[i] <= 31 || input[i] == 127){
                    if(input[i] == '\t'){
                        temp[i] = ' ';
                        continue;
                    } 
                    temp[i] = input[i+1];
                    i++;
                    continue;
                }
                if(input[i] == '#'){
                    temp[i] == '\0';         //temp ends here so that everything after the '#' character is removed
                    break;
                }
                temp[i] = input[i];
            }   
        //printf("%s\n",temp);                              //TEST (CHECK)
        Panic = Command(temp,A,&N,&WW,&WB,&Winner,&history,&hSize);       //interprets command based on the given string 
        if (Panic == 1 || Panic == -1) break;
    }
    if (Panic == 1) return 1;
    if (Panic == -1) return 0;  //user gave quit command
    for(i = 0;i < N;i++)
        free(A[i]);
    free(A);
    return 0;
}

int Command(char *input,element **A,int *pN,int *pWW,int *pWB,char *pWinner,char ***history,int *hSize){
    char *com = NULL,*arg1 = NULL,*arg2 = NULL,*arg3 = NULL;  //tokens extracted from original string                                         
    com = strtok(input," \n");                                //\n is needed as a delimiter in order to be replaced by a \0

    /* Command: name */
    if(strcmp(com,"name") == 0) printf("= sdi2100083\n\n");
    else if (strcmp(com,"known_command") == 0){ //ALWAYS TRUE
        arg1 = strtok(NULL," \n");
        if (strcmp(arg1,"name") == 0 || strcmp(arg1,"known_command") == 0)
            printf("= true\n\n");
        else if(strcmp(arg1,"list_commands") == 0 || strcmp(arg1,"quit") == 0)    //Trying to evade huge lines
            printf("= true\n\n");
        else if(strcmp(arg1,"boardsize") == 0 || strcmp(arg1,"clear_board") == 0)
            printf("= true\n\n");
        else if (strcmp(arg1,"walls") == 0 || strcmp(arg1,"playmove") == 0)
            printf("= true\n\n");
        else if(strcmp(arg1,"playwall") == 0 || strcmp(arg1,"genmove") == 0)
            printf("= true\n\n");
        else if(strcmp(arg1,"undo") == 0 || strcmp(arg1,"winner") == 0 || strcmp(arg1,"showboard") == 0)
            printf("= true\n\n");
        else printf("? false\n\n");
    }

    /* Command: list_commands */
    else if(strcmp(com,"list_commands") == 0){
        printf("=\nname\nknown_command\nlist_commands\nquit\nboardsize\n");
        printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard\n\n");
    }

    /* Command: quit */
    else if(strcmp(com,"quit") == 0){
        printf("=\n\n");
        return -1;
    }

    /* Command: undo */
    else if(strcmp(com,"undo") == 0){
        arg1 = strtok(NULL,input);
        int times = atoi(arg1);
        printf("=\nundo %d\n\n",times);                     //TEST 
        /*if(times <= 0) printf("=\n\n");
        else if (times <= MP){                              //MP = moves played
            printf("=\n\n");
            undo(times,P,W,N,pWW,pWB,pWinner);              //not sure about parameters
        }
        else printf("? cannot undo\n\n");*/            
    }

    /* Command: boardsize */
    else if(strcmp(com,"boardsize") == 0){
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL){ 
            printf("? invalid syntax\n\n");
            return 0;
        }
        int tempN = atoi(arg1);
        if(tempN > 0 && tempN <= 25 && tempN%2 == 1){
            printf("=\n\n");
            //boardsize(A,pN,pWW,pWB);                     //board configuration/number of wall/game history = ARBITRARY
        }
        else printf("? unacceptable size\n\n");
    }

    /* Command: clear_board */
    else if(strcmp(com,"clear_board") == 0){
        printf("=\n\n");
        //clearboard(A,N,pWW,pWB);                           //players starting position-wallls arbitrary-game history empty
    }

    /* Command: walls */
    else if(strcmp(com,"walls") == 0){
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL){
            printf("? invalid syntax\n\n");
            return 0;
        }
        *pWW = *pWB = atoi(arg1);
        printf("=\n\n");
    }

    /* Command: showboard */
    else if(strcmp(com,"showboard") == 0){
        //printf("=\nshowboard\n");
        showboard(A,*pN,*pWW,*pWB);
    }

    /* Command: playmove */
    else if(strcmp(com,"playmove") == 0){
        arg1 = strtok(NULL," \n");
        arg2 = strtok(NULL," \n");
        if(arg1 == NULL || arg2 == NULL)                  //player and position are needed to execute the move
            printf("? invalid syntax\n");//might need correction
        else{
            //printf("=\nplaymove\n");
            playmove(A,*pN,arg1,arg2,pWinner,history,hSize);
        }
    }

    /* Command: playwall */
    else if(strcmp(com,"playwall") == 0){
        arg1 = strtok(NULL," \n");
        arg2 = strtok(NULL," \n");
        arg3 = strtok(NULL," \n");
        if(arg1 == NULL || arg2 == NULL || arg3 == NULL)   //player,position,and wall direction are neeeded to execute the move
            printf("? invalid syntax\n\n");//might need correction
        else{
            printf("=\nplaywall\n");
            //playwall(A,*pN,pWW,pWB,arg1,arg2,arg3);
        }
    }

    /* Command: genmove */
    else if(strcmp(com,"genmove") == 0){
        arg1 = strtok(NULL," \n");
        if(arg1 == NULL) //might need correction
            printf("? invalid syntax\n\n");
        else
            printf("=\ngenmove\n\n");                                       //genmove determines the "best move" and returns a string with
            //Command(genmove(A,*pN,arg1,pWW,pWB),A,pN,pWW,pWB,pWinner);    //the command that is passed on to a recursive call of the command function
    }

    /* Command: winner */
    else if(strcmp(com,"winner") == 0){
        if(*pWinner == 'B') printf("= true black\n\n");
        else if(*pWinner == 'W') printf("= true white\n\n");
        else printf("= false\n\n");
    }
    else printf("? unknown command\n");
    return 0;
}