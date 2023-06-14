#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "func.h"

/*typedef struct KeySpace {
    int busy;// признак занятости элемента
    unsigned int key;// ненулевой ключ элемента
    unsigned int par;// ключ родительского элемента, может быть нулевым
    int len; // длина информации
    int offset; // смещение в файле (по отношение к началу файла)
//    char* info; // массив с информацией типа Item
}KeySpace;

typedef struct Table{
    KeySpace *ks;
    int msize;// размер области пространства ключей
    int csize;// количество элементов в области пространства ключей
    FILE *fd; // дискриптор файла, чтобы выполнять операции с файлом данных
}Table;

int D_Add(Table *);
int D_Find(Table *);
int D_Del(Table *);
int D_Show(Table *);
int D_Load(Table *);
int D_Reorg(Table *, char* );
char* find_inf(Table* t, int kl);*/

void destructor(Table* t){
        KeySpace* ks=t->ks;
        free(ks);
}

char *getstr() { // робит
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


int create_table(Table* t, char* fn){
    int n;
    do{
        printf("Enter the maximum size of the table: --> "); // создание нового файла и новой таблицы
        n=scanf("%d",&(t->msize));
        if(n<0){
        		free(fn);
                return 0;
                //break;
        }if(n==0){
            printf("Incorrect value.True again!\n");
            scanf("%*[^\n]");
            scanf("%*c");
        }
        if(n== 1 && (t->msize<1)){
            printf("Incorrect value.True again!\n");
            n=0;
        }
    }while(n!=1);
    t->csize=0;
    t->fd=fopen(fn, "w+b");
    if(t->fd==NULL){
        t->ks=NULL;
        fclose(t->fd);
        return 0;
    }
    t->ks=(KeySpace*)calloc((t->msize),sizeof(KeySpace)); // выделяем память под таблицу
    fseek(t->fd, 0, SEEK_SET);
    fwrite(&t->msize, sizeof(int), 1, t->fd);
    fwrite(&t->csize, sizeof(int), 1, t->fd);
    fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd);
    for(int i=0; i<t->msize;i++) t->ks[i].busy=0;
    return 1;
}

int find_el(int kl, Table* t){ //робит
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

int find_inf(Table* t, int kl){ // робит
    KeySpace* ks=t->ks;
    char* inf=NULL;
    int i=find_el(kl, t);
    if(i>=0){
        int p=ks[i].info;
	return p;
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
    return -1;
}


int add_el(Table* t, unsigned int kl, unsigned int rk){ // робит
    KeySpace* k=t->ks;
    if(t->csize==t->msize) return 2;
    int b=hesh(kl, t);
    if(b<=-1) return 1;
    k[b].key=kl;
    k[b].info=rk;
    k[b].busy=1;
    (t->csize)++;
    return 0;
}

int del_el(Table* t, int kl){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    int p=find_el(kl,t);
    if(p<0) return 4;    
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

int load2(Table* t, char* fn){// робит 
    t->fd=fopen(fn, "r+b");// открывается существующий файл на чтение и запись
    if (t->fd == NULL){
    //	fclose(t->fd);
    	return 0; // если файл не открылся
    }	
    fread(&(t->msize), sizeof(int), 1, t->fd); // считываем размер вектора
    printf("Maximum size of table: %d\n", t->msize);
    t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace)); //выделяем память под таблицу
    fread(&(t->csize), sizeof(int), 1, t->fd);// считываем размер таблицы
    printf("Size of table: %d\n", t->csize);
    fread(t->ks, sizeof(KeySpace), t->msize, t->fd);// считываем таблицу
    return 1;
}

int save(Table *t){
    fseek(t->fd, sizeof(int), SEEK_SET); // пропуск поля, где задаётся длина вектора
    fwrite(&t->csize, sizeof(int), 1, t->fd); // записываем в файл размер таблицы
    fwrite(t->ks, sizeof(KeySpace), t->msize, t->fd); // записываем в файл всю таблицу
    fclose(t->fd);
    t->fd = NULL;
    return 1;
}

int D_Add(Table *t){ //робит
    int rc, n, k, rk;
	char *errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
		printf("Enter key: -->");
        n = scanf("%d",&k);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	k = -1;
        }
        if(n<0) return 0;
    } while(k < 0);
	errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
    	printf("Enter info: ");
        n = scanf("%d",&rk);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	k = -1;
        }
        if(n<0) return 0;
    } while(rk < 0);
    rc = add_el(t, k, rk);
    if(rc==0) printf("Successfully! %u, %u\n", k, rk);
    if(rc==1) printf("Duplicate key %u\n", k);
    if(rc==2) printf("Table is overflow\n");
    return 1;
}

int D_Del(Table *t){
	 const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty"}; 
    int rc, n, k;
    char *errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
		printf("Enter key: -->");
        n = scanf("%d",&k);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	k = -1;
        }
        if(n<0) return 0;
    } while(k < 0);
    rc = del_el(t, k);
    printf("%s: %u\n", errmsgs[rc], k);
    return 1;
}

int D_Find(Table *t){ // робит
    int rc, n, k;
    int inf;
    char *errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
		printf("Enter key: -->");
        n = scanf("%d",&k);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	k = -1;
        }
        if(n<0) return 0;
    } while(k < 0);
    inf = find_inf(t, k);
    if(inf>=0){
        printf("key = %d, info = \"%d\"\n", k, inf);
    }
    else printf("Item %d was not found\n", k);
    return 1;
}

int D_Show(Table *t){ // робит
	const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty"};
    int rc;
    rc = print(t);
    printf("%s!\n", errmsgs[rc]);
    return 1;
}

int D_Load(Table *t){
	const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    printf("Enter file name: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    char* fn = getstr();
    if(fn==NULL) return 0;
    rc = load(t, fn);
    printf("%s\n", errmsgs[rc]);
    free(fn);
    return 1;
}

int D_Load2(Table* t, char** fn){ // робит
    int msize;
    printf("Enter file name: --> ");
    *fn = getstr();
    if(*fn == NULL) return 0;
    if (load2(t, *fn) == 0){ // указанный файл не существует
        if(create_table(t, *fn)==0)return 0; // создаем новый файл
        
    }
    return 1;
}

int D_Save(Table* t, char** fn){
	save(t);
	free(*fn);
	destructor(t);
	return 1;
}

int dialog(const char *choice[], int N){ //робит
    char *errchoice = "";
    int rc;
    int i, n;
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        for(i = 0; i < N; ++i) puts(choice[i]);
        printf("Make your choice: --> ");
        n = scanf("%d",&rc);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	rc = -1;
        }
        if(n<0) return -1;
    } while(rc < 0 || rc >= N);
    return rc;
}
