#include <stdio.h>
#include <stdlib.h>
#include "func.h"
void destructor (matrix ** m){
        row * item=(*m)->matr;
        for (int i=0; i<((*m)->size); i++){
                free(item[i].next);
        }
        free(item);
        free(*m);
}

void matrix_create(matrix** full){
    int n, m;
    int* arr;
    (*full)=(matrix*) malloc(sizeof(matrix));
    printf("Enter the number of lines\n");
    while(scanf("%d",&n)<=0){
        printf("Incorrect value. True again.\n");
        scanf("%*s");
    }
    //для eof
    (*full)->size=n;
        if(n==0) return ;
    row* table=(row*)malloc(n*sizeof(row));
    for(int i=0; i<n; i++){
        printf("Enter the number of line elements\n");
        while(scanf("%d",&m)!=1 || m<=0){
            printf("Incorrect value. True again.\n");
            scanf("%*s");
        }
        arr=create_line(m,i);
        table[i].next=arr;
        table[i].size=m;
    }
    (*full)->matr=table;
    //free(table);
}

int* create_line(int m, int i){
    int* arr=(int*)malloc(m*sizeof(int));
    for(int j=0; j<m; j++){
        printf("arr[%d][%d]=",i,j);
        while(scanf("%d",(arr+j))!=1){
            printf("Incorrect value. True again.\n");
            scanf("%*s");
        }
      //  fflush(stdin);
    }
    return arr;
}

matrix* newmatrix(const matrix* full){
    int f=1,k=-1,p=-1;
    matrix* newfull=(matrix*)malloc(sizeof(matrix));
    int up=full->size;
    row* newtable=(row*)malloc(up*sizeof(row));
    row* table=full->matr;
    for(int i=0;i<up;i++){
        int right=table[i].size;
        newtable[i].size=right;
        int* arr=(int*)malloc(right*sizeof(int));
        arr[0]=table[i].next[0];
        for(int j=1;j<right;j++){
            if(table[i].next[j]>table[i].next[j-1]&& f==1){
                f=0;
                k=j;
            }
            if(table[i].next[j]<table[i].next[j-1]){
                p=j;
            }
            arr[j]=table[i].next[j];
        }
        if(k!=-1 && p!=-1){
            int tmp=arr[k];
            arr[k]=arr[p];
            arr[p]=tmp;
        }
        p=-1;
        k=-1;
        f=1;
        newtable[i].next=arr;
    }
    newfull->size=up;
    newfull->matr=newtable;
    return newfull;

}
