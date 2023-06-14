#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "func.h"

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

void destr(Graph* g){
	for(int i=0; i<g->csize; i++){
		free(g->data[i]->name);
		List* tmp=g->data[i]->adjacency;
		while(tmp!=NULL){
			if(tmp->adjacency>=i)free(tmp->connection);
			List* tmp1=tmp;
			tmp=tmp->next;
			free(tmp1);
		}
		free(g->data[i]);
	}
	free(g->data);
	//free(g);
}

int find_vertex(Graph* g, char* name){
    for(int i=0; i<g->csize; i++){
        if(strcmp(g->data[i]->name, name)==0)return i;
    }
    return -1;
}

void increase_size(Graph* g){
    g->msize*=2;
    g->data=(Vertex**)realloc(g->data,g->msize * sizeof(Vertex*));
} 

void create_data(Graph* g){
    g->data=(Vertex**)malloc(sizeof(Vertex*)*10);
    g->msize=10;
    g->csize=0;
}

List* create_list(int i){
    List* new=(List*)malloc(sizeof(List));
    new->next=NULL;
    new->adjacency=i;
    return new;
}
Edge* create_edge(Vertex* tmp1, Vertex* tmp2, int delay){
    Edge* new=(Edge*)malloc(sizeof(Edge));
    new->delay=delay;
    new->ports[0]=tmp1->port;
    new->ports[1]=tmp2->port;
    return new;
}

int add_edge(Graph* g,char* name1, char* name2, int delay){
    if(g->data==NULL) return 3;
    int i=find_vertex(g, name1), j=find_vertex(g, name2);
    if(i==-1)return 1;
    if(j==-1)return 2;
    Vertex* tmp1=g->data[i];
    Vertex* tmp2=g->data[j];
    List* new1=create_list(j);
    List* new2=create_list(i);
    Edge* new=create_edge(tmp1,tmp2,delay);
    new1->connection=new;
    new2->connection=new;
    if(tmp1->adjacency==NULL)tmp1->adjacency=new1;
    else{
        List* ptr=tmp1->adjacency;
        while(ptr->next!=NULL){
            if(ptr->adjacency==j){
            	free(new1);
            	free(new2);
            	free(new);
            	return 4;
            }
            ptr=ptr->next;
        }
        ptr->next=new1;
    }
    if(tmp2->adjacency==NULL)tmp2->adjacency=new2;
    else{
        List* ptr=tmp2->adjacency;
        while(ptr->next!=NULL)ptr=ptr->next;
        ptr->next=new2;
    }
    return 0;
}
int del_edge(Graph* g, int i, int j){
    Vertex* v1=g->data[i];
    Vertex* v2=g->data[j];
    List* connect1=v1->adjacency;
    List* prev=v1->adjacency;
    while (connect1!=NULL && connect1->adjacency!=j){
        prev=connect1;
        connect1=connect1->next;
    }
    if(connect1==NULL)return 1;
    if(prev==connect1)v1->adjacency=connect1->next;
    else prev->next=connect1->next;
    free(connect1->connection);
    connect1->connection=NULL;
    free(connect1);
    return 0;
}

int del_edge1(Graph* g, char* name1, char* name2){
    if(g->data==NULL)return 2;
    int i1=find_vertex(g, name1);
    int i2=find_vertex(g, name2);
    if(i1 == -1)return 1;
    if(i2 == -1)return 4;
    int rc=del_edge(g, i1, i2);
    if(rc == 1)return 3;
    rc=del_edge(g, i2, i1);
    return 0;
}

int del_vert(Graph* g, char* name){
    if(g->data==NULL)return 2;
    int i=find_vertex(g,name);
    if(i==-1) return 1;
    Vertex* tmp=g->data[i]; 
    List* connect=tmp->adjacency;
    while(connect!=NULL){
    	List* tmp=connect;
        if(i!=connect->adjacency)del_edge(g, connect->adjacency, i);
        connect=connect->next;
        free(tmp);
    }
    tmp->adjacency=NULL;
    free(tmp->name);
    free(g->data[i]);
    g->data[i]=NULL;
    g->csize--;
    g->data[i]=g->data[g->csize];
    g->data[g->csize]=NULL;
    return 0;
}

int add_vertex(Graph* g,char* name,int port){
    if(g->data==NULL) create_data(g);
    int i=find_vertex(g, name);
    if(i!=-1)return 1;
   // Vertex* tmp=g->data[i];
    if(g->csize==g->msize) increase_size(g);
    Vertex* new=(Vertex*)malloc(sizeof(Vertex));
    new->name=name;
    new->port=port;
    new->color=0;
    new->adjacency=NULL;
    g->data[g->csize]=new;
    g->csize++;
    return 0;
}

int print(Graph* g){
    if(g->data==NULL)return 1;
    for(int i=0; i<g->csize; i++){
        printf("%s(%d)| ",g->data[i]->name, g->data[i]->port);
        List* ptr=g->data[i]->adjacency;
        while(ptr!=NULL){
            printf("%s(%d)",g->data[ptr->adjacency]->name, ptr->connection->delay);
            if(ptr->next!=NULL)printf("->");
            ptr=ptr->next;
        }
        printf("\n");
    }
    return 0;
}

int change_name(Graph* g, int i, char* name){
    int j=find_vertex(g, name);
    if(j!=-1)return 1;
    free(g->data[i]->name);
    g->data[i]->name=name;
    return 0;
}

int change_port(Graph* g,int i,int port){
    List* tmp=g->data[i]->adjacency;
    while(tmp!=NULL){
        if(tmp->connection->ports[0]==g->data[i]->port){
            tmp->connection->ports[0]=port;
        }
        else tmp->connection->ports[1]=port;
    }
    g->data[i]->port=port;
    return 0;
}

List* find_edge(Graph* g, int i, int j){
    Vertex* tmp=g->data[i];
    List* ptr=g->data[j]->adjacency;
    printf("%s, %s\n", tmp->name, g->data[j]->name);
    while(ptr!=NULL){
        if(ptr->adjacency==i)return ptr;
        ptr=ptr->next;
    }
    return NULL;
}

int change_computer(Graph* g, int i, int j, char* name){
    int k=find_vertex(g, name);
    if(k==-1)return 1;
    Vertex* tmp1=g->data[i];
    Vertex* tmp2=g->data[j];
    List* ptr1=tmp1->adjacency;
    List* prev1=tmp1->adjacency;
    List* ptr2=tmp2->adjacency;
    while(ptr1->adjacency!=j){
        prev1=ptr1;
        ptr1=ptr1->next;
    }
    if(ptr1==prev1)tmp1->adjacency=ptr1->next;
    else prev1->next=ptr1->next;
    if(ptr1->connection->ports[0]==tmp1->port)ptr1->connection->ports[0]=g->data[k]->port;
    else ptr1->connection->ports[1]=g->data[k]->port;
    while(ptr2->adjacency!=i) ptr2=ptr2->next;
    ptr2->adjacency=k;
    ptr2=g->data[k]->adjacency;
    if(ptr2==NULL)g->data[k]->adjacency=ptr1;
    else{
        while(ptr2->next!=NULL)ptr2=ptr2->next;
        ptr2->next=ptr1;
    }
    return 0;
}

int change_delay(Graph* g,int i,int j,int delay){
    List* tmp=g->data[i]->adjacency;
    while(tmp->adjacency!=j)tmp=tmp->next;
    tmp->connection->delay=delay;
    return 0;
}

int D_Change_Edge(Graph *g){ 
    int rc, n, choice, delay;
    char * name1, *name2, *name;
    char *errchoice = "";
    printf("Enter first name of computer for connection:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name1 = getstr();
    if (name1 == NULL) return 0;
    printf("Enter second name of computer for connection:\n");
    name2 = getstr();
    if (name2 == NULL) return 0;
    int i=find_vertex(g, name1);
    if(i==-1){
        printf("Computer %s not found in the system\n",name1);
        free(name1);
        return 1;
    }
    int j=find_vertex(g, name2);
    if(j==-1){
        printf("Computer %s not found in the system\n",name2);
        free(name2);
        return 1;
    }
    List* tmp=find_edge(g,i,j);
    if(tmp==NULL){
        printf("Connection not found\n");
        return 1;
    }
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("\nWhat do you want to change?\n1.first computer: %s\n2.second computer: %s\n3.time of delay: %d\n", name1, name2,tmp->connection->delay);
        n = scanf("%d",&choice);
        if(n == 0){
                scanf("%*[^\n]");
                scanf("%*c");
                choice = -1;
        }
        if(n<0) return 0;
    } while(choice<=0 || choice>3);
    errchoice = "";
    switch (choice){
        case 1:
            printf("Enter new name of first computer:\n");
            scanf("%*[^\n]");
            scanf("%*c");
            name = getstr();
            rc=change_computer(g, i, j, name);
            break;
        case 2:
            printf("Enter new name of second computer:\n");
            scanf("%*[^\n]");
            scanf("%*c");
            name = getstr();
            rc=change_computer(g, j, i, name);
            break;
        
        case 3:
            do{
                printf("%s",errchoice);
                errchoice = "You are wrong. Repeate, please!\n";
                printf("Enter new port of computer:\n");
                n = scanf("%d",&delay);
                if(n == 0){
                        scanf("%*[^\n]");
                        scanf("%*c");
                        delay = -1;
                }
                if(n<0) return 0;
            } while(delay<0);
            rc=change_delay(g, i, j, delay);
            break;
    }
    if(rc==0) printf("Successfully!\n");
    if(rc==1) printf("There isn't computer %s in the system\n", name);
    free(name);
    return 1;
}

int D_Change_Vertex(Graph *g){ 
    int rc, n, choice, port;
    char * name, *name1;
    char *errchoice = "";
    if(g->data==NULL){
        printf("The computer network is empty\n");
        return 1;
    }
    printf("Enter name of computer:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name = getstr();
    if (name == NULL) return 0;
    int i=find_vertex(g, name);
    if(i==-1){
        printf("Computer %s not found in the system",name);
        free(name);
        return 1;
    }
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("\nWhat do you want to change?\n1.name: %s\n2.port: %d\n", g->data[i]->name, g->data[i]->port);
        n = scanf("%d",&choice);
        if(n == 0){
                scanf("%*[^\n]");
                scanf("%*c");
                choice = -1;
        }
        if(n<0) return 0;
    } while(choice<=0 || choice>2);
    switch (choice){
        case 1:
            printf("Enter new name of computer:\n");
            scanf("%*[^\n]");
            scanf("%*c");
            name1 = getstr();
            rc=change_name(g, i, name1);
            break;
        
        case 2:
            do{
                printf("%s",errchoice);
                errchoice = "You are wrong. Repeate, please!\n";
                printf("Enter new port of computer:\n");
                n = scanf("%d",&port);
                if(n == 0){
                        scanf("%*[^\n]");
                        scanf("%*c");
                        port = -1;
                }
                if(n<0) return 0;
            } while(port<0);
            rc=change_port(g, i, port);
            break;
    }
    if(rc==0) printf("Successfully!\n");
    if(rc==1){
        printf("Computer %s is already there\n", name1);
        free(name1);
    }
    if(rc==2) printf("There isn't computer %s in the system\n", name);
    free(name);
    return 1;
}

Color* create_color(Color* c, Vertex* v){
    Color* c1=(Color*)malloc(sizeof(Color));
    v->color=1;
    c1->now=v;
    c1->next=c;
    return c1;
}

void treverse1(Graph* g, Color* c){
    if(c==NULL)return ;
    printf("%s: %d\n",c->now->name, c->now->port);
    List* tmp=c->now->adjacency;
    Color* c1=c;
    c=c->next;
    free(c1);
    while(tmp!=NULL){
        if(g->data[tmp->adjacency]->color==0)c=create_color(c, g->data[tmp->adjacency]);
        tmp=tmp->next;
    }
    treverse1(g, c);
}

int treverse(Graph* g, char* name){
	nulled(g);
    int i=find_vertex(g,name);
    if(i==-1)return 1;
    Color* c=NULL;
    c=create_color(c, g->data[i]);
    treverse1(g,c);
    return 0;
}

int D_Treverse(Graph* g){
    const char *errmsgs[] = {"Successfully", "The computer network is empty"};
    int rc;
    char* name;
    printf("Enter name of computer:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name = getstr();
    if (name == NULL) return 0;
    rc = treverse(g, name);
    printf("%s!\n", errmsgs[rc]);
    free(name);
    return 1;
}

void nulled(Graph* g){
    for(int i=0; i<g->csize; i++)g->data[i]->color=0;
}

int find_short(List* q, int* arr, int i, Graph* g){
    while(q!=NULL){
        if(arr[q->adjacency] > q->connection->delay + arr[i]) arr[q->adjacency] = q->connection->delay + arr[i];
        q=q->next;
    }
    int min=INT_MAX;
    int ind=-1;
    for(int k = 0; k<g->csize; k++){
        if(min>arr[k] && g->data[k]->color==0){
            min = arr[k];
            ind = k;
        }
    }
    return ind;
}
int comparator(const void* p, const void* q){
	int k=(((Edges*)p)->weight - ((Edges*)q)->weight);
	if(k<0)return -1;
	if(k==0)return 0;
	else return 1;
}

int mini(Graph* g){
	nulled(g);
	int len=g->csize*g->csize;
	Edges** eg=(Edges**)calloc(len,sizeof(Edges*));
	int i=0;
	for(int j=0; j<g->csize; j++){
		List* tmp=g->data[j]->adjacency;
		while(tmp!=NULL){
			if(tmp->adjacency>=j){
				eg[i]=(Edges*)malloc(sizeof(Edges));
				eg[i]->weight=tmp->connection->delay;
				eg[i]->v1=j;
				eg[i]->v2=tmp->adjacency;
				i++;
			}
			tmp=tmp->next;
		}
	}
//	eg=(Edges**)realloc(eg,i*sizeof(Edges*));
	qsort(eg, i, sizeof(Edges*), comparator);
	Graph* g1=(Graph*)malloc(sizeof(Graph));
	g1->data=NULL;
//	g1->csize=g->csize;
//	g1->msize=g->msize;
	for(int k=0; k<g->csize; k++){
		char* name=strdup(g->data[k]->name);
		add_vertex(g1, name, g->data[k]->port);
	}
	for(int k=0; k<i; k++){
		int p=eg[k]->v1;
		int q=eg[k]->v2;
		if(g1->data[p]->color==0 || g1->data[q]->color==0){
			add_edge(g1, g1->data[p]->name, g1->data[q]->name, eg[k]->weight);
			g1->data[p]->color=1;
			g1->data[q]->color=1;
		}
	}
	for(int k=0; k<i; k++){
		free(eg[k]);
	}
//	free(eg);
	print(g1);
	destr(g1);
	free(eg);
	free(g1);
}

int D_Mini(Graph* g){
	mini(g);
	return 1;
}



int Dijkstra(Graph* g, char* name1, char* name2){
    nulled(g);
    if(g->data==NULL)return 1;
    int i=find_vertex(g, name1);
    int j=find_vertex(g, name2);
    if(i==-1)return 2;
    if(j==-1)return 4;
    int arr[g->csize], arr1[g->csize];
    for(int k=0; k<g->csize;k++){
        arr1[k]=-1;
        arr[k]=INT_MAX;
        if(k==i)arr[k]=0;
    }
    int k=i;
   // arr1[0]=i;
   // int z=0;
    while(k!=-1 && k!=j){
        g->data[k]->color=1;
        List* q=g->data[k]->adjacency;
        k=find_short(q, arr, k, g);
      //  z++;
      //  arr1[z]=k;
    }
    if(k==-1) return 3;
    printf("Path: %d\n", arr[j]);
   // k=0;
  /*  while(k!=-1){
        printf("%s->", g->data[arr1[k]]->name);
        k++;
    }*/
    return 0;
}

int D_Find_Path(Graph* g){
    const char *errmsgs[] = {"Successfully", "The computer network is empty", "Computer %s not found in the system\n", "Path not found"};
    int rc;
    char* name1, *name2;
    printf("Enter names of two computers:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name1 = getstr();
    if (name1 == NULL) return 0;
    name2 = getstr();
    if (name2 == NULL) return 0;
    rc=Dijkstra(g, name1, name2);
    if(rc==0 || rc==1 || rc==3)printf("%s!\n", errmsgs[rc]);
    else{
        if(rc==2) printf("%s!\n", errmsgs[rc], name1);
        if(rc==4) printf("%s!\n", errmsgs[2], name1);
    }
    free(name1);
    free(name2);
    return 1;
}

int D_Show(Graph* g){
    const char *errmsgs[] = {"Successfully", "The computer network is empty"};
    int rc;
    rc = print(g);
    printf("%s!\n", errmsgs[rc]);
    return 1;
}

int D_Add_Edge(Graph *g){ 
    int rc, n, delay;
    char * name1, *name2;
    char *errchoice = "";
    printf("Enter first name of computer for connection:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name1 = getstr();
    if (name1 == NULL) return 0;
    printf("Enter second name of computer for connection:\n");
//    scanf("%*[^\n]");
//    scanf("%*c");
    name2 = getstr();
    if (name2 == NULL) return 0;
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("Traffic delay time -->");
        n = scanf("%d",&delay);
        if(n == 0){
                scanf("%*[^\n]");
                scanf("%*c");
                delay = -1;
        }
        if(n<0) return 0;
    } while(delay < 0);
    rc = add_edge(g, name1, name2, delay);
    if(rc==0) printf("Successfully!\n");
    if(rc==1) printf("There isn't computer %s in the system\n", name1);
    if(rc==2) printf("There isn't computer %s in the system\n", name2);
    if(rc==3) printf("The computer network is empty\n");
    if(rc==4) printf("This connection is alredy there");
    free(name1);
    free(name2);
    return 1;
}

int D_Del_Edge(Graph *g){
    const char *errmsgs[] = {"Successfully","There isn't computer", "The computer network is empty", "Connection not found"};
    int rc, n;
    char* name1, *name2;
    printf("Enter name of first computer: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    name1 = getstr();
    if (name1 == NULL) return 0;
    printf("Enter name of second computer: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    name2 = getstr();
    if (name2 == NULL) return 0;
    rc = del_edge1(g, name1, name2);
    if(rc==0 || rc==2 || rc==3)printf("%s: %s, %s\n", errmsgs[rc], name1, name2);
    if(rc==1)printf("%s %s\n", errmsgs[rc], name1);
    if(rc==4)printf("%s %s\n", errmsgs[1], name2);
    free(name1);
    free(name2);
    return 1;
}

int D_Del_Vert(Graph *g){
    const char *errmsgs[] = {"Successfully","There isn't computer", "The computer network is empty"};
    int rc, n;
    char* key;
    printf("Enter name of computer: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    key = getstr();
 //   printf("%s", key);
    if (key == NULL) return 0;
    rc = del_vert(g, key);
    if(rc==3) return 0;
    printf("%s: %s\n", errmsgs[rc], key);
    free(key);
    return 1;
}

int D_Add_Vert(Graph *g){ 
    int rc, n, port, rk;
    char * name;
    char *errchoice = "";
    printf("Enter name of computer:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    name = getstr();
    if (name == NULL) return 0;
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("Enter name of port: -->");
        n = scanf("%d",&port);
        if(n == 0){
                scanf("%*[^\n]");
                scanf("%*c");
                port = -1;
        }
        if(n<0) return 0;
    } while(port < 0);
    rc = add_vertex(g, name, port);
    if(rc==0) printf("Successfully! %s\n", name);
    if(rc==1){
    	printf("Computer %s is already there\n", name);
    	free(name);
    }
    return 1;
}

Graph* graph_create(Graph* g){
    g=(Graph*)malloc(sizeof(Graph));
    g->data=NULL;
    g->csize=0;
    g->msize=0;
    return g;
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

