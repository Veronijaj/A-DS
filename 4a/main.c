#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "func.h"

int main(){
    const char* choice[]={"0- Exit", "1- Add element", "2- Find element",
    "3- Delete element", "4- Show table", "5- Special find", "6- Add elements from a text file", "7- Make png", "8- Draw a tree", "9- Timing"};
    const int N=sizeof(choice)/sizeof(choice[0]);
    int (*fptr[])(Tree*) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Special, D_Load, D_Load2, D_Draw, D_Graph};
    Tree t;
    t.root=NULL;
    t.number=0;
    int rc;
    while(rc = dialog(choice, N)){
        if(rc==-1)break;
        if(!fptr[rc](&t))break;
    }
    if(t.root!=NULL)destructor(&t);
    printf("That's all. Bye!\n");
    return 0;
}
