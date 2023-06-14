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

int reorganize(Table* t, char* fn){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    char* copy="copy.bin";
    char* inf=NULL;
    FILE *file = fopen(copy, "wb");
    int k=t->csize;
    for(int i=0;i<t->csize;i++)if(ks[i].busy==0)k--;  
    t->msize=k;
    fwrite(&(t->msize), sizeof(int), 1, file);
    fwrite(&(t->msize), sizeof(int), 1, file);
    int p=0;
    for(int i=0;i<t->csize;i++){
    	if(ks[i].busy==1){
    		fwrite(&(t->ks[i]), sizeof(KeySpace),1,file);
    	}
    }
    for(int i=0;i<t->csize;i++){
    	if(ks[i].busy==1){
    		inf=find_inf(t, ks[i].key);
    		ks[p].busy=1;
    		ks[p].len=ks[i].len;
    		ks[p].par=ks[i].par;
    		ks[p].key=ks[i].key;
    		fseek(file,0,SEEK_END);
    		ks[p].offset=ftell(file);
    		fwrite(inf, sizeof(char),ks[p].len,file);
    		free(inf);
    		p++;
    	}
    }
    t->csize=k;
    t->ks=(KeySpace*)realloc(t->ks,(t->msize)*sizeof(KeySpace));
    fclose(t->fd);
    t->fd=NULL;
    t->fd=file;
    rename(copy,fn);
    fclose(t->fd);
    t->fd = NULL;
    FILE* fr = fopen(fn, "r+b");
    t->fd = fr;
    return 0;
}

int create_table(Table* t, char* fn){
    int n;
    do{
        printf("Enter the maximum size of the table: --> "); // создание нового файла и новой таблицы
        n=scanf("%d",&(t->msize));
        if(n<0){
                return 0;
                break;
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
   // fclose(t->fd);
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
    for(int i=0; i<(t->csize); i++){
        if(ks[i].key==kl && ks[i].busy!=0) return i;
    }
    return -1;
}

char* find_inf(Table* t, int kl){ // робит
    KeySpace* ks=t->ks;
    char* inf=NULL;
    int i=find_el(kl, t);
    if(i>=0){
        inf=(char*)malloc(ks[i].len);
        fseek(t->fd, ks[i].offset, SEEK_SET);
        fread(inf,sizeof(char),ks[i].len,t->fd);
    }
    return inf;
}

int add_el(Table* t, unsigned int kl, unsigned int rk, char* inf){ // робит
    KeySpace* k=t->ks;
    if(find_el(kl,t)>-1) return 1;
    if(find_el(rk,t)<=-1 && rk!=0) return 3;
    if(t->csize==t->msize) return 2;
    k[t->csize].key=kl;
    k[t->csize].par=rk;
    k[t->csize].busy=1;
    k[t->csize].len=strlen(inf)+1;
    fseek(t->fd,0,SEEK_END);
    k[t->csize].offset=ftell(t->fd);
    fwrite(inf, sizeof(char), k[t->csize].len, t->fd);
    (t->csize)++;
    return 0;
}

int del_el(Table* t, int kl){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    int p=find_el(kl,t);
    if(p<0) return 4;    
    ks[p].busy=0;
    for(int i=0; i<t->csize; i++)if(ks[i].par==kl)ks[i].busy=0;
    return 0;
}

int print(Table* t){
    KeySpace* ks=t->ks;
    if(t->csize==0)return 5;
    char* inf=NULL;
    for(int i=0; i<(t->csize); i++){    
    	inf=(char*)malloc(ks[i].len); 
    	fseek(t->fd,ks[i].offset,SEEK_SET); //смещаем файл на начало информации которую хотим вывести
    	fread(inf, sizeof(char), ks[i].len, t->fd); // считываем в буфер информацию длиной len
        if(ks[i].busy==1)printf("%d | %d | %s\n", ks[i].key, ks[i].par, inf); 
        free(inf);        
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
            free(tinf);
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

int load2(Table* t, char* fn){// робит 
    t->fd=fopen(fn, "r+b");// открывается существующий файл на чтение и запись
    if (t->fd == NULL){
    //	fclose(t->fd);
    	return 0; // если файл не открылся
    }	
    fread(&(t->msize), sizeof(int), 1, t->fd); // считываем максимальный размер таблицы
     printf("Maximum size of table: %d\n", t->msize); //отображаем пользователю
    t->ks=(KeySpace*)malloc(t->msize*sizeof(KeySpace)); //выделяем память под таблицу
    fread(&(t->csize), sizeof(int), 1, t->fd);// считываем размер таблицы
    printf("Size of table: %d\n", t->csize);
    fread(t->ks, sizeof(KeySpace), t->csize, t->fd);// считываем таблицу в вектор со структурами
    return 1;
}

int save(Table *t){
    fseek(t->fd, sizeof(int), SEEK_SET); // пропуск поля, где задаётся длина вектора
    fwrite(&t->csize, sizeof(int), 1, t->fd); // записываем в файл размер таблицы
    fwrite(t->ks, sizeof(KeySpace), t->csize, t->fd); // записываем в файл всю таблицу
    fclose(t->fd);
    t->fd = NULL;
    return 1;
}

int D_Add(Table *t){ //робит
    int rc, n, k, rk;
    char *info;
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
    } while(k <= 0);
	errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
    	printf("Enter parent key: -->");
        n = scanf("%d",&rk);
        if(n == 0){
        	scanf("%*[^\n]");
        	scanf("%*c");
        	k = -1;
        }
        if(n<0) return 0;
    } while(rk < 0);
    printf("Enter info:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    info = getstr();
    if (info == NULL) return 0;
    rc = add_el(t, k, rk, info);
    free(info);
    if(rc==0) printf("Successfully! %u, %u\n", k, rk);
    if(rc==1) printf("Duplicate key %u\n", k);
    if(rc==3) printf("There isn't parent key %u\n", rk);
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
    char* inf=NULL;
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
    if(inf){
        printf("key = %d, info = \"%s\"\n", k, inf);
        free(inf);
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
        create_table(t, *fn); // создаем новый файл
    }
    return 1;
}

int D_Reorg(Table *t, char* fn){
	
	const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    rc = reorganize(t, fn);
    printf("%s\n", errmsgs[rc]);
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
        if(n<0) return 0;
    } while(rc < 0 || rc >= N);
    return rc;
}
