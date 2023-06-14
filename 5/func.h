#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
//typedef struct List;

typedef struct Edges{
    int v1;
    int v2;
    int weight;
}Edges;

typedef struct Color{
    struct Vertex* now;
    struct Color* next;
}Color;

typedef struct Vertex{
    char* name;
    int port;
    int color;
    struct List* adjacency;
}Vertex;

typedef struct Edge{
    int ports[2];
    int delay;
}Edge;

typedef struct Graph{
    int csize;
    int msize;
    Vertex** data;
}Graph;

typedef struct List{
    int adjacency;
    Edge* connection;
    struct List* next;
}List;

char *getstr();

void destr(Graph* g);

int find_vertex(Graph* g, char* name);
void increase_size(Graph* g); 

void create_data(Graph* g);

List* create_list(int i);
Edge* create_edge(Vertex* tmp1, Vertex* tmp2, int delay);

int add_edge(Graph* g,char* name1, char* name2, int delay);
int del_edge(Graph* g, int i, int j);

int del_edge1(Graph* g, char* name1, char* name2);

int del_vert(Graph* g, char* name);

int add_vertex(Graph* g,char* name,int port);

int print(Graph* g);

int change_name(Graph* g, int i, char* name);

int change_port(Graph* g,int i,int port);

List* find_edge(Graph* g, int i, int j);

int change_computer(Graph* g, int i, int j, char* name);

int change_delay(Graph* g,int i,int j,int delay);

int D_Change_Edge(Graph *g);

int D_Change_Vertex(Graph *g);

Color* create_color(Color* c, Vertex* v);

void treverse1(Graph* g, Color* c);

int treverse(Graph* g, char* name);
int D_Treverse(Graph* g);

void nulled(Graph* g);

int find_short(List* q, int* arr, int i, Graph* g);

int mini(Graph* g);

int D_Mini(Graph* g);

int comparator(const void* p, const void* q);


int Dijkstra(Graph* g, char* name1, char* name2);

int D_Find_Path(Graph* g);

int D_Show(Graph* g);

int D_Add_Edge(Graph *g);

int D_Del_Edge(Graph *g);

int D_Del_Vert(Graph *g);

int D_Add_Vert(Graph *g);

Graph* graph_create(Graph* g);
int dialog(const char *choice[], int N);
#endif
