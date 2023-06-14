#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main(){
    matrix* full;
    matrix_create(&full);
    int up=full->size;
    if(up==0){
        printf("Matrix is zero\n");
        free(full);
        return 0;}
    row* table=full->matr;
    printf("First matrix\n");
    for(int i=0;i<up;i++){
        int right=table[i].size;
        printf("\"");
        for(int j=0;j<right;j++){
            printf("%d, ",table[i].next[j]);
        }
        printf("\"\n");
    }
    row* t=full->matr;
   matrix* newfull=newmatrix(full);
    row* newtable=newfull->matr;
    printf("Second matrix\n");
    for(int i=0;i<up;i++){
        int right=newtable[i].size;
        printf("\"");
        for(int j=0;j<right;j++){
            printf("%d, ",newtable[i].next[j]);
        }
        printf("\"\n");
    }
    destructor(&full);
    destructor(&newfull);
    return 0;
}