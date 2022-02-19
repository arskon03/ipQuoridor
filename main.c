#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"

int Command(char */*,char **,int,int *,int *,char **/);     //0 = proper execution, 1 = Panic situation

int main(int argc,char **argv){         //board size and number of walls for each player
    int N,WW,WB,i,Panic;
    char **P,**W,Winner = '\0',input[30];   //P = matrix with player positions(W,B) and blank characters everywhere else
    /*if(argc == 1){                        //W = matrix with wall positions (H for horizontal,V for vertical) and blank characters everywhere else
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
    }
    printf("Dimension = %d\nWalls = %d\n",N,WW);              //TEST (CHECK)
    P = malloc(N*sizeof(char *));
    W = malloc((N-1)*sizeof(char *));
    if (P == NULL || W == NULL){
        printf("Not enough memory!\n");
        return 1;
    }
    for (i = 0;i < N;i++){
        P[i] = malloc(N*sizeof(char));
        if(i < N-1) W[i] = malloc((N-1)*sizeof(char));
        if (P[i] == NULL){
            printf("Not enough memory!\n");
            return 1;
        }
        for(int j = 0;j < N;j++){                      //Starting values of the matrix
            if(i == 0 && j == (int)(N/2)){
                P[i][j] = 'B';
            }
            else if(i == N-1 && j == (int)(N/2)){
                P[i][j] = 'W';
            }
            else P[i][j] = ' ';
        }
    }*/
    while(Winner == '\0'){
        fgets(input,sizeof(input),stdin);
        char* temp[30];
            for(int i = 0;i < 30;i++){         //temp will hold the preprocessed string according to the protocol
                if(input[i] >= 1 && input[i] <= 31 || input[i] == 127 ){
                    if(input[i] == '\t'){
                        temp[i] = ' ';
                        continue;
                    } 
                    temp[i] = input[i+1];
                    i++;
                    continue;
                }
                if(input[i] == '#'){
                    temp[i] == '\0';                      //temp ends here so that everything after the '#' character is removed
                    break;
                }
                temp[i] = input[i];
            }   
            printf("%s\n",temp);
        Panic = Command(temp,P,N,&WW,&WB,&Winner);            //interprets command based on the given string 
        if (Panic == 1) break;     
    }
    if (Panic == 1) return 1;
    /*for(i = 0;i < N;i++){
        free(P[i]);
        free(W[i]);
    }
    free(P);
    free(W);
    return 0;*/
}

int Command(char *input,char **P,int N,int *pWW,int *pWB,char *pWinner){
    char *com = NULL,*player = NULL,*pos = NULL,*orientation = NULL;   //tokens extracted from original string                                         
    com = strtok(input," \n");                                       //\n is needed as a delimiter in order to be replaced by a \0
    /*player = strtok(NULL," \n");
    pos = strtok(NULL," \n");
    orientation = strtok(NULL," \n");*/
    //printf("%s %s %s %s\n",com,player,pos,w_direction);                 //TEST (CHECK)
    if(strcmp(com,"showboard") == 0){
        printf("=\nshowboard\n");
        //showboard(P,N,*pWW,*pWB);
    }
    else if(strcmp(com,"playmove") == 0){
        if(player == NULL || pos == NULL)                               //player and position are needed to execute the move
            printf("? invalid syntax\n");
        else{
            printf("=\nplaymove\n");
            //playmove(P,N,player,pos,pWinner);
        }
    }
    else if(strcmp(com,"playwall") == 0){
        if(player == NULL || player == NULL || orientation == NULL)     //player,position,and wall direction are neeeded to execute the move
            printf("? invalid syntax\n\n");
        else{
            printf("=\nplaywall\n");
            //playwall(P,N,pWW,pWB,player,pos,w_direction);
        }
    }
    else if(strcmp(com,"genmove") == 0){
        if(player == NULL)
            printf("? invalid syntax\n\n");
        else
            printf("genmove\n");                                       //genmove determines the "best move" and returns a string with
            //Command(genmove(P,N,player,pWW,pWB),P,N,pWW,pWB,pWinner);  //the command that is passed on to a recursive call of the command function
    }
    else if(strcmp(com,"winner") == 0){
        if(*pWinner == 'B') printf("= true black\n\n");
        else if(*pWinner == 'W') printf("= true white\n\n");
        else printf("= false\n\n");
    }
    //else if(*com == '#') return 0;
    else printf("? unknown command\n");
    return 0;
}