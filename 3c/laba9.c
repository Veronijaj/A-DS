#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "func.h"

int main(){
    const char* choice[]={"0- Exit", "1- Add element", "2- Find element", 
    "3- Delete element", "4- Show table", "5- Add elements from a text file"};
    const int N=sizeof(choice)/sizeof(choice[0]);
    int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Load};
    Table t;
    if(create_table(&t)==-1)return 0;
    int rc;
    while(rc = dialog(choice, N)){
    	if(rc==-1)break;
    	if(!fptr[rc](&t))break;
    }
    destructor(&t);
    printf("That's all. Bye!\n");
    return 0;
}
