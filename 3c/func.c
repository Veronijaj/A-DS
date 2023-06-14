#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
//#include <cmath>
#include "func.h"
//#include "global.h"
/*typedef struct KeySpace {
    int busy;// признак занятости элемента
    unsigned int key;// ненулевой ключ элемента
    unsigned int par;// ключ родительского элемента, может быть нулевым
    char* info; // массив с информацией типа Item
}KeySpace;

typedef struct Table{
    KeySpace *ks; 
    int msize;// размер области пространства ключей
    int csize;// количество элементов в области пространства ключей
}Table;

int D_Add(Table *);
int D_Find(Table *); 
int D_Del(Table *); 
int D_Show(Table *);
int D_Load(Table *);
int D_Reorg(Table *);*/
//const char* choice[]={"0- Exit", "1- Add element", "2- Find element", "3- Delete element", "4- Show table", "5- Add elements from a text file", "6- Reorganize the table"};
//const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
//int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Load, D_Reorg};
//const int N=sizeof(choice)/sizeof(choice[0]);

void destructor(Table* t){
	KeySpace* ks=t->ks;
	//for(int i=0; i<t->csize; i++)free(ks[i].info);
	free(ks);
}

char *getstr() {
    int len = 0;
    char* str = (char*)malloc(1);
    char buf[81];
    int n;
    *str='\0';
    do {
        n = scanf("%80[^\n]", buf);
        if (n<0) {
            free(str);
            str = NULL;
            continue;
        }
        if (n==0) {
        	scanf("%*[^\n]");
            scanf("%*c");
        } else {
            int lenbuf = strlen(buf);
            int strl = len+ lenbuf;
            str = (char*)realloc(str, strl+1);
            str = strcat(str, buf);
        }
    } while (n>0);
    return str;
}

int hesh1(int k1, Table* t){
	int h=k1%(t->msize);
	return h;
}
int hesh2(int k1, Table* t){
    int k=k1;
    int m=t->msize;
    int w=0;
    while(k!=0){
        k/=10;
        w++;
    }
    if(w==0)w=1;
//    printf("%d\n",k1);
    int r=1;
    for(int i=0; i<=w;i++)r*=w;
    int p=(k1*m/(10*w*r)+1);
    return p;
}

int create_table(Table* t){ // cool
    printf("Enter the maximum size of the table: ");
   // scanf("%d", &(t->msize));
   int n;
     do{
                n=scanf("%d",&(t->msize));
                if(n<0){
                	return -1;
                    break;
                }if(n==0){
                    printf("Incorrect value.Try again!\n");
                    scanf("%*[^\n]");
                    scanf("%*c");
                }
                if(n== 1 && (t->msize<1)){
                    printf("Incorrect value.Try again!\n");
                    n=0;
                }
            }while(n!=1);

    t->csize=0;
    t->ks=(KeySpace*)malloc((t->msize)*sizeof(KeySpace));
    for(int i=0; i<t->msize;i++) t->ks[i].busy=0;
    return 0;
}

int find_el(int kl, Table* t){
    KeySpace* ks=t->ks;
    int h=hesh1(kl, t);
    int h2=hesh2(kl, t);
    int b;
    for(int i=0; i<t->msize;i++){
    	b=(h+i*h2)%(t->msize); 
        if(ks[b].busy==0) return -1;
        if(ks[b].busy==2 && ks[b].key==kl) return -2;
        if(ks[b].busy==1 && ks[b].key==kl) return b; 
    }
    return -1;
}

int hesh(int kl, Table* t){
	KeySpace* ks=t->ks;
	int h=hesh1(kl, t);
    int h2=hesh2(kl, t);
    int b=h;
    for(int i=0; i<(t->msize);i++){
    	b=(h+i*h2)%(t->msize); 
        if(ks[b].busy==0) return b;
        if(ks[b].busy==1 && ks[b].key==kl) return -1;
    }
    printf("%d\n",b);
    return -1;
}
int add_el(Table* t, unsigned int kl, unsigned int inf){
    KeySpace* k=t->ks;
    if(t->csize==t->msize) return 2;
    int b=hesh(kl, t);
    if(b<=-1) return 1;
    //printf("%d\n",b);
    k[b].key=kl;
    k[b].info=inf;
    k[b].busy=1;
    (t->csize)++;
    return 0;
}

int del_el(Table* t, int kl){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    int p=find_el(kl,t);
    if(p==-1 || p==-2) return 4;
    ks[p].busy=2;
    return 0; 
}

int print(Table* t){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    for(int i=0; i<(t->msize); i++){
        if(ks[i].busy==1)printf("%d | %d \n", ks[i].key, ks[i].info);
    }
    return 0;
}

int load(Table* t, char* fn){//загружает информацию с файла в структурный массив
    int per1, per2, rs;
    KeySpace* ks=t->ks;
    FILE *file;
    file = fopen(fn, "r"); 
    if (file == NULL) return 6;
    int read, count= t->csize;
    do {
        if(count==t->msize){
            fclose(file);
            return 2;
        }
        read = fscanf(file,"%d,%d",&per1, &per2);

        if (read == 2){
            rs=add_el(t,per1,per2);
            if(rs==1) count--; 
            if(rs==2) return 2;
            count++; 
        } 
        if (read != 2 && !feof(file)) return 7;
        if (ferror(file)) {
            fclose(file);
            return 7;
        }
    } while (!feof(file));
    fclose(file);
    t->csize=count;
    return 0;
}

int D_Add(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key",
     "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc, n, k, rk;
    char* errchoice="";
    do{
    	printf("%s",errchoice);
    	errchoice = "You are wrong. Repeate, please!\n";
	    printf("Enter key: -->");
	    n = scanf("%u",&k);
	    if(n == 0){
	    	scanf("%*[^\n]");
	    	scanf("%*c");
	    	k=-1;
	    } 
	    if(n<0)return 0;
	}while(k<0);
	errchoice="";
    do{
    	printf("%s",errchoice);
    	errchoice = "You are wrong. Repeate, please!\n";
	    printf("Enter inf: ");
	    n = scanf("%d",&rk);
	    if(n == 0){
	    	scanf("%*[^\n]");
	    	scanf("%*c");
	    	k=-1;
	    } 
	    if(n<0)return 0;
	}while(rk<0);
    rc = add_el(t, k, rk);
    printf("%s: %u \n", errmsgs[rc], k);
    return 1;
}

int D_Del(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc, n;
    int k;
	char* errchoice="";
    do{
    	printf("%s",errchoice);
    	errchoice = "You are wrong. Repeate, please!\n";
	    printf("Enter key: -->");
	    n = scanf("%d",&k);
	    if(n == 0){
	    	scanf("%*[^\n]");
	    	scanf("%*c");
	    	k=-1;
	    } 
	    if(n<0)return 0;
	}while(k<0);
    rc = del_el(t, k);
    printf("%s: %u\n", errmsgs[rc], k);
    return 1;
}

int D_Find(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc, n;
    int k;
    char* errchoice="";
    do{
    	printf("%s",errchoice);
    	errchoice = "You are wrong. Repeate, please!\n";
	    printf("Enter key: -->");
	    n = scanf("%d",&k);
	    if(n == 0){
	    	scanf("%*[^\n]");
	    	scanf("%*c");
	    	k=-1;
	    } 
	    if(n<0)return 0;
	}while(k<0);
    rc = find_el(k, t);
    if(rc<0)printf("%s\n", errmsgs[4]);
    if(rc>-1)printf("key =  %u, info =\" %d \"\n", k, t->ks[rc].info);
   // printf("%s: %u\n", errmsgs[l], rc);
    return 1;
}

int D_Show(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    rc = print(t);
    printf("%s!\n", errmsgs[rc]);
    return 1;
}

int D_Load(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    printf("Enter file name: ");
    scanf("%*[^\n]");
    scanf("%*c");
    char* fn = getstr();
    if(fn==NULL){ 
    	
    	return 0;
    }
    rc = load(t, fn);
    printf("%s\n", errmsgs[rc]);
    free(fn);
    return 1;
}

int dialog(const char *choice[], int N){
	
    char *errchoice = "";
    int rc;
    int i, n;
    do{
        puts(errchoice);
        errchoice = "You are wrong. Repeate, please!";
        for(i = 0; i < N; ++i) puts(choice[i]);
        printf("Make your choice: --> ");
        n = scanf("%d",&rc);
        if(n == 0){
	    	scanf("%*[^\n]");
	    	scanf("%*c");
	    	rc=-1;
	    }  
        if(n < 0) return -1;
    } while(rc < 0 || rc >= N);
    return rc;
}

/*int main(){
    Table t;
    create_table(&t);
    int rc;
    while(rc = dialog(choice, N))if(!fptr[rc](&t))break;
    destructor(&t);
    printf("That's all. Bye!\n");
    return 0;
}*/
