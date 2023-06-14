#ifndef FUNC_H
#define FUNC_H

typedef struct Node{
    int key;
    struct Node* left;
    struct Node* right;
    struct Node* next;
    char* info;
}Node;

typedef struct Tree{
    Node* root;
    int depth;
    int number;
}Tree;

typedef struct Vector{
    Node** space;
    int count;
}Vector;

Node* find_parent(Tree* t, int k);
Node* find_el_rek(Node* t, int k);
char *getstr();
void destructor(Tree* t);
void sew2 (Tree *t);
void makegv(Tree* t, FILE* file);
Node* create_root(int k, char* info);
int add_el(Tree* t,int k,char* info);
int del_el_rek (Tree* t,int key);
int draw(Tree* t);
Vector* special_find(Node* ptr, int k);
char *fgetstr(FILE* file);
int load(Tree* t, char* fn);
int print(Tree* t);
int D_Add(Tree *t);
int D_Find(Tree *t);
int D_Load(Tree *t);
int D_Load2(Tree *t);
int D_Special(Tree *t);
int D_Del(Tree *t);
int D_Show(Tree *t);
int D_Draw(Tree *t);
int D_Graph(Tree* t);
int dialog(const char *choice[], int N);

#endif
