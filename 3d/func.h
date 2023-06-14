#ifndef FUNC_H
#define FUNC_H

typedef struct KeySpace {
    int busy;// признак занятости элемента
    unsigned int key;// ненулевой ключ элемента
    unsigned int info;// ключ родительского элемента, может быть нулевым
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
int D_Load2(Table* t, char** fn);
int D_Save(Table* t, char** fn);
int find_inf(Table* t, int kl);
void destructor(Table* t);
char *getstr() ;
int reorganize(Table* t, char* fn);
int create_table(Table* t, char* fn);
int find_el(int kl, Table* t);
int add_el(Table* t, unsigned int kl, unsigned int rk);
int del_el(Table* t, int kl);
int print(Table* t);
int load(Table* t, char* fn);
int load2(Table* t, char* fn);
int save(Table *t);
int dialog(const char *choice[], int N);
int hesh(int kl, Table* t);
int hesh1(int kl, Table* t);
int hesh2(int kl, Table* t);

#endif
