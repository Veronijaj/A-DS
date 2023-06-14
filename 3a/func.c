#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
	for(int i=0; i<t->csize; i++)free(ks[i].info);
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

int reorganize(Table* t){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    for(int i=0;i<t->csize;i++){
        if(ks[i].busy==0){
            free(ks[i].info);
            (t->csize)--;
            ks[i]=ks[t->csize];
            i--;
        }
    }
    t->msize=t->csize;
    t->ks=(KeySpace*)realloc(t->ks,(t->msize)*sizeof(KeySpace));
    return 0;
}

void create_table(Table* t){
    printf("Enter the maximum size of the table: ");
   // scanf("%d", &(t->msize));
   int n;
     do{
                n=scanf("%d",&(t->msize));
                if(n<0){
                	return ;
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
}

int find_el(int kl, Table* t){
    KeySpace* ks=t->ks;
    for(int i=0; i<(t->csize); i++){
        if(ks[i].busy==0) return -2;
        if(ks[i].key==kl) return i;
    }
    return -1;
}

int add_el(Table* t, unsigned int kl, unsigned int rk, char* inf){
    KeySpace* k=t->ks;
    if(find_el(kl,t)>-1) return 1;
    if(find_el(rk,t)<=-1 && rk!=0) return 3;
    if(t->csize==t->msize) return 2;
    k[t->csize].key=kl;
    k[t->csize].info=inf;
    k[t->csize].busy=1;
    k[t->csize].par=rk;
    (t->csize)++;
    return 0;
}

int del_el(Table* t, int kl){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    int p=find_el(kl,t);
    if(p==-1 || p==-2) return 4;
    ks[p].busy=0;
    for(int i=0; i<t->csize; i++) if(ks[i].par==kl) ks[i].busy=0;
    return 0; 
}

int print(Table* t){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    for(int i=0; i<(t->csize); i++){
        if(ks[i].busy==1)printf("%d | %d | %s\n", ks[i].key, ks[i].par, ks[i].info);
    }
    return 0;
}

char *fgetstr(FILE* file) {
    int len = 0;
    char* str = (char*)malloc(1);
    char buf[81];
    int n;
    *str='\0';
    n = fscanf(file,"%s\n", buf);
    if (n<0) {
    	free(str);
        return NULL;
    }
    if (n==0) {
        scanf("%*c");
    } else {
        int lenbuf = strlen(buf);
        int strl = len+ lenbuf;
        str = (char*)realloc(str, strl+1);
        str = strcat(str, buf);
    }
    return str;
}

int load(Table* t, char* fn){//загружает информацию с файла в структурный массив
    int per1, per2, rs;
    char* tinf;
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
        read = fscanf(file,"%d,%d[^,]",&per1, &per2);
        tinf=fgetstr(file);
        if(tinf==NULL) return 7;
        read++;
        if (read == 3){
            rs=add_el(t,per1,per2,tinf);
            if(rs==1) count--; 
            if(rs==2) return 2;
            count++; 
        } 
        if (read != 3 && !feof(file)) return 7;
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
    int rc, n;

    int k, rk;
    char *info;
    printf("Enter key: -->");
    n = scanf("%d",&k);
    if(n <= 0 || k<=0) return 0;
    printf("Enter parent's key: -->");
    n = scanf("%d",&rk);
    if(n == 0 || k<0) return 0;
    printf("Enter info:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    info = getstr();
    if (info == NULL) return 0; 
    rc = add_el(t, k, rk, info);
    printf("%s: %u, %u\n", errmsgs[rc], k, rk);
    return 1;
}

int D_Del(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc, n;
    int k;
    printf("Enter key: -->");
    n = scanf("%d",&k);
    if(n <= 0 || k<=0) return 0;
    rc = del_el(t, k);
    printf("%s: %u\n", errmsgs[rc], k);
    return 1;
}

int D_Find(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc, n;
    int k;
    printf("Enter key: -->");
    n = scanf("%d",&k);
    if(n <= 0 || k<=0) return 0;
    rc = find_el(k, t);
    int l;
    if(rc<0)l=4;
    if(rc>-1)l=0;
    printf("%s: %u\n", errmsgs[l], k);
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

int D_Reorg(Table *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    //printf("kotik\n");
    rc = reorganize(t);
    printf("%s\n", errmsgs[rc]);
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
        if(n <= 0) rc = 0;
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
