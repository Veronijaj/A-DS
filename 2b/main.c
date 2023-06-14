#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "func.h" 
int main(void) {
    stack* list=NULL;
    int a,n,f=1;
    char* lom, *som;
    lom=open_directory();
    if(lom==NULL)return 1;
    list=create_node(lom);
    while(f){
        printf("1-View directiry\n2-View the history of actions\n");
	    do{
	        n=scanf("%d",&a);	
	        if(n<0){
	        	f=0;
	            break;
	        }if(n==0){
	        	printf("Incorrect value.True again!\n");
	            scanf("%*[^\n]");
	            scanf("%*c");
			}
			if(n== 1 && (a<1 || a>2)){
				printf("Incorrect value.True again!\n");
				n=0;
			}
	        if (n>0){
		        switch (a){
		        case 1:
		        	scanf("%*[^\n]");
		        	scanf("%*c");
		            som=open_directory();
		            if(som!=NULL)list=new_elem(list,som);
		            break;
		        case 2:
		            printf("History of directories\n");
		            print_history(list);
		        default:
		            break;
		        }
	        }
	    }while(n!=1);
    }
    clean(&list);
    return 0;
}
