#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "func.h"

typedef struct stack{
    char** data;
    size_t size;
    size_t max;
}stack;

void empty(stack* list){
        list=(stack*)malloc(sizeof(stack));
}
char* open_directory(){
    fflush(stdin);
    int n;
    char* dirname=getstr();
//    add_history(dirname);
    if(dirname==NULL) return NULL;;
    DIR* dp = opendir(dirname);
    if ( dp==NULL) {
        fprintf(stderr, "Can't open current directory!\n");
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
    stack* list=(stack*)malloc(sizeof(stack));
    list->size=1;
    list->max=3;
    list->data=(char**)malloc((list->max)*sizeof(char*));
    list->data[0]=lom;
   // list->head=first;
   // first->top=1;
   return list;
}

stack* new_elem(stack* list, char* lom){
        if(list->size==list->max){
                free(list->data[0]);
                for(int i=0; i<list->size-1; i++)list->data[i]=list->data[i+1];
                (list->size)--;
        }
    (list->size)++;
    list->data[list->size -1]=lom;
    return list;
}

void print_history(stack* list){
    int i=0;
    while(i<list->size){
        printf("%s ",list->data[i]);
        i++;
    }
    printf("\n");
}
void clean(stack** list){
    int i=0;
    while(i<(*list)->size){
        free((*list)->data[i]);
        i++;
    }
    free((*list)->data);
    free(*list);
}

char *getstr() {
    int len = 0;
    char* str = (char*)malloc(1);
    char buf[81];
    int n;
    *str='\0';
    printf("Enter an absolute or relative directory name: ");
    do {
        n = scanf("%80[^\n]", buf);
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
