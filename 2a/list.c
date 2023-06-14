#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "func.h"

typedef struct stack{
    char* name;
    struct stack* next;
}stack;
void empty(stack* list){
        list=(stack*)malloc(sizeof(stack));
}
char* open_directory(){
    int n,len=0;
    char* dirname=getstr();
    if(dirname==NULL) return NULL;
    DIR* dp=opendir(dirname);
    if (dp==NULL) {
        printf("Can't open current directory!\n");
        free(dirname);
        return NULL;
    }
    struct dirent* de;
    while ( ( de = readdir(dp) ) ) {
            printf("%s\n", de->d_name);
    }
    closedir(dp);
    return dirname;
}

stack* create_node(char* lom){
    stack* first=(stack*)malloc(sizeof(stack));
    first->name=lom;
    first->next=NULL;
    return first;
}

stack* new_elem(stack* list, char* lom){
    stack* new=(stack*)malloc(sizeof(stack));
   // history* last=list->head;
    new->name=lom;
    new->next=list;
    list=new;
    return list;
}

void print_history(stack* list){
    stack* item=list;
   // item=list->head;
    while(item!=NULL){
        printf("%s ",item->name);
        item=item->next;
    }
    printf("\n");
}
void clean(stack** list){
    while((*list)!=NULL){
        free((*list)->name);
        stack* item=(*list)->next;
        free(*list);
        *list=item;
    }
    //free(list);
}

char *getstr() {
    int len = 0;
    char* str = (char*)malloc(1);
    char buf[81];
    int n;
    *str='\0';
    printf("Enter an absolute or relative directory name: ");
    do {
        n = scanf("%3[^\n]", buf);
        if (n<0) {
            free(str);
            str = NULL;
            continue;
        }
        if (n==0) {
            scanf("%*c");
        } else {
            int lenbuf = strlen(buf);
           // printf ("%d\n", *len);
            int strl = len+ lenbuf;
            str = (char*)realloc(str, strl+1);
            str = strcat(str, buf);
        }
    } while (n>0);
    return str;
}
