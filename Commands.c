#include <stdio.h>
#include "Commands.h"

void showboard(element **A,int N,int WA,int WB){
    int i,j;
    for(i = 1;i <= (2*N + 3);i++){                      //board big enough to show all rows,collumns and the coordinates
        for(j = 1;j <= N+2;j++){ 
            if(i == 1 || i == (2*N+3)){                 //First and last line are always the coordinates
                if(j == 1 || j == N+2) printf("    ");
                else printf(" %c  ",'A'+(j-2)/4 - 1);
            }
            else if(i == 2 || i == (2*N+2)){              //second and second to last lines are always the same
                if (j == 1 ) printf("   +");
                else if(j == N+2) printf("+   ");
                else printf("---+");
            }
            else if(i%2 == 1){
                if(j == 1) printf(" %d |",N-i+1);
                else if(j == N+2) printf(" %d  ",N-i+1);
                else{
                    printf(" %c ",A[i][j].P);
                }
            }
            else{ //i%2 == 0
                if(j == 1) printf("   +");
                else if(j == N+2) printf("+   ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}