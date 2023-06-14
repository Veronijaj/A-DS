#ifndef FUNC_H
#define FUNC_H
typedef struct KeySpace {
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
int D_Reorg(Table *);

//extern const char* choice[]={"0- Exit", "1- Add element", "2- Find element", "3- Delete element", "4- Show table", "5- Add elements from a text file", "6- Reorganize the table"};
//extern const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
//extern int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Load, D_Reorg};
//extern const int N=sizeof(choice)/sizeof(choice[0]);
void destructor(Table* t);
char *getstr();
int reorganize(Table* t);
void create_table(Table* t);
int find_el(int kl, Table* t);
int add_el(Table* t, unsigned int kl, unsigned int rk, char* inf);
int del_el(Table* t, int kl);
int print(Table* t);
char *fgetstr(FILE* file);
int load(Table* t, char* fn);
int dialog(const char *choice[], int N);

#endif
