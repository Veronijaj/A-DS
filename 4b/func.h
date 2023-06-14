#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Len{
	int len;
	struct Len* next;
}Len;

typedef struct Info{
    int info;
    struct Info* next;
}Info;

typedef struct Node{
    char* KeySpace[3];
    struct Node* NodeSpace[4];
    Info* InfoSpace[3];
    struct Node* parent; 
}Node;

typedef struct Tree{
    Node* root;
}Tree;

void symbiosis(Node* y, char* key);
void base_del(Node* ptr, char* key);
int D_Load(Tree *t);
void traverse (Node *node);

void destr (Tree *tree) {
	Node *ptr = tree->root;
	traverse(ptr);
	//free(tree->root);
	free(tree);
}

void traverse (Node *node) {
	if (node == NULL) return;
	traverse(node->NodeSpace[0]);
	traverse(node->NodeSpace[1]);
	traverse(node->NodeSpace[2]);
	traverse(node->NodeSpace[3]);
	for (int j=0; j<3; ++j) {
		Info *ptr = node->InfoSpace[j];
		while (ptr != NULL) {
			Info *prev = ptr;
			ptr=ptr->next;
			free(prev);
		}
	}
	int i=0;
	while ((i!=3) && (node->KeySpace[i] != NULL)) {
		free(node->KeySpace[i]);
		++i;
	}
	free(node);
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

void print_node(Node* ptr){ 
    int i=0;
    while(ptr->KeySpace[i]!=NULL && i!=3){
        printf("%s: ", ptr->KeySpace[i]);
        Info* tmp=ptr->InfoSpace[i];
        while(tmp!=NULL){
            printf("%d, ", tmp->info);
            tmp=tmp->next;
        }
        printf("\n");
        i++;
    }
}

int print(Node* ptr){
    if(ptr == NULL) return 0;
    print_node(ptr);
    print(ptr->NodeSpace[0]);
    print(ptr->NodeSpace[1]);
    print(ptr->NodeSpace[2]);
    print(ptr->NodeSpace[3]);
}

Info* find_el_rek(Node* ptr, char* k){
    while(ptr!=NULL){
        if(strcmp(k,ptr->KeySpace[0])==0)return ptr->InfoSpace[0];
        else if(strcmp(k,ptr->KeySpace[0])==-1){
            ptr=ptr->NodeSpace[0];
        }
        else{
            if(ptr->KeySpace[1]==NULL)ptr=ptr->NodeSpace[1];
            else{ 
                if(strcmp(k,ptr->KeySpace[1])==0)return ptr->InfoSpace[1];
                else if(strcmp(k,ptr->KeySpace[1])==-1)ptr=ptr->NodeSpace[1];
                else{
                    if(ptr->KeySpace[2]==NULL)ptr=ptr->NodeSpace[2];
                    else{
                        if(strcmp(k,ptr->KeySpace[2])==0)return ptr->InfoSpace[2];
                        else if(strcmp(k,ptr->KeySpace[2])==-1)ptr=ptr->NodeSpace[2];
                        else ptr=ptr->NodeSpace[3];
                    }
                }
            }
        }

    }
    return NULL;
}

Node* find_node(Tree* t, char* k){
    Node* ptr=t->root;
    while(ptr!=NULL){
        if(strcmp(k,ptr->KeySpace[0])==0)return ptr;
        else if(strcmp(k,ptr->KeySpace[0])==-1){
            ptr=ptr->NodeSpace[0];
        }
        else{
            if(ptr->KeySpace[1]==NULL)ptr=ptr->NodeSpace[1];
            else{ 
                if(strcmp(k,ptr->KeySpace[1])==0)return ptr;
                else if(strcmp(k,ptr->KeySpace[1])==-1)ptr=ptr->NodeSpace[1];
                else{
                    if(ptr->KeySpace[2]==NULL)ptr=ptr->NodeSpace[2];
                    else{
                        if(strcmp(k,ptr->KeySpace[2])==0)return ptr;
                        else if(strcmp(k,ptr->KeySpace[2])==-1)ptr=ptr->NodeSpace[2];
                        else ptr=ptr->NodeSpace[3];
                    }
                }
            }
        }

    }
    return NULL;
}

Node* create_peak(char* key, Info* info){
    Node* peak=(Node*)malloc(sizeof(Node));
    for(int i=0; i<3; i++){
        peak->InfoSpace[i]=NULL;
        peak->KeySpace[i]=NULL;
    }
    peak->InfoSpace[0]=info;
    peak->KeySpace[0]=key;
    peak->parent=NULL;
    for(int i=0; i<4; i++)peak->NodeSpace[i]=NULL;
    return peak;
}

void add_in_peak(Node* peak, char* key, Info* info){
    for(int i=0; i<3; i++){
        if(peak->KeySpace[i]==NULL){
            peak->KeySpace[i]=key;
            peak->InfoSpace[i]=info;
            break;
        }
        if(strcmp(key,peak->KeySpace[i])==-1){
            for(int j=2; j>i; j--){
                peak->KeySpace[j]=peak->KeySpace[j-1];
                peak->InfoSpace[j]=peak->InfoSpace[j-1];
            }
            peak->KeySpace[i]=key;
            peak->InfoSpace[i]=info;
            break;
        }
    }
}

int add_el(Tree* t,char* key,int info){
    Node* item=t->root;
    Info* nodeinf=(Info*)malloc(sizeof(Info));
    nodeinf->info=info;
    nodeinf->next=NULL;
    if(item == NULL){
        t->root=create_peak(key, nodeinf);
        return 0;
    }
    Info* doub=find_el_rek(item,key);
    if(doub!=NULL){
        while(doub->next!=NULL)doub=doub->next;
        doub->next=nodeinf;
        free(key);
        return 5;
    }
    char** KS=item->KeySpace;
    Info** IS=item->InfoSpace;
    int place=-1;
    while(item->NodeSpace[0]!=NULL || KS[2]!=NULL){
        if(KS[2]!=NULL){
            Node* left=create_peak(KS[0], IS[0]);
            Node* right=create_peak(KS[2], IS[2]); 
            left->NodeSpace[0]=item->NodeSpace[0];
            left->NodeSpace[1]=item->NodeSpace[1];
            right->NodeSpace[0]=item->NodeSpace[2];
            right->NodeSpace[1]=item->NodeSpace[3];
            if(item->NodeSpace[0]!=NULL){
                left->NodeSpace[0]->parent=left;
                left->NodeSpace[1]->parent=left;
                right->NodeSpace[0]->parent=right;
                right->NodeSpace[1]->parent=right;               
            }
            if(item->parent==NULL){
                Node* new=create_peak(KS[1], IS[1]);
                right->parent=new;
                left->parent=new;
                new->NodeSpace[0]=left;
                new->NodeSpace[1]=right;
                t->root=new;
                free(item);
                item=new;
            }
            else{
                add_in_peak(item->parent, KS[1], IS[1]);
                left->parent=item->parent;
                right->parent=item->parent;
                for(int i=3; i>place+1; i--)item->parent->NodeSpace[i]=item->parent->NodeSpace[i-1];
                item->parent->NodeSpace[place]=left;
                item->parent->NodeSpace[place+1]=right;
                Node* parent=item->parent;
                free(item);
                item=parent;
            }
            KS=item->KeySpace;
            IS=item->InfoSpace;
        }
        if(strcmp(KS[0], key)==1){
            item=item->NodeSpace[0];
            place=0;
        }
        else{
            if(KS[1]==NULL || strcmp(KS[1], key)==1){
                item=item->NodeSpace[1];
                place=1;
            }
            else{
                if(KS[2]==NULL || strcmp(KS[2], key)==1){
                    item=item->NodeSpace[2];
                    place=2;
                }
                else{
                    item=item->NodeSpace[3];
                    place=2;
                }
            }
        }
        KS=item->KeySpace;
        IS=item->InfoSpace;
    }
    add_in_peak(item, key, nodeinf);
    return 0;
}

char *fgetstr(FILE* file) {
    int len = 0;
    char* str = (char*)malloc(1);
    char buf[81];
    int n;
    *str='\0';
    n = fscanf(file,"%s[^ \n]", buf);
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

int load(Tree* t, char* fn){//загружает информацию с файла в структурный массив
    int per1, per2, rs, info;
    char* key;
    FILE *file;
    file = fopen(fn, "r");
    if (file == NULL) return 6;
    int read;
    do {
        key=fgetstr(file);
        if(key==NULL) return 7;
        read = fscanf(file,"%d\n",&info);
        read++;
        if (read == 2){
            rs=add_el(t,key,info);
        }
        else if (read != 2 && !feof(file)){
        	printf("bebe");
        	return 7;

        }
        if (ferror(file)) {
        	printf("lop");
            fclose(file);
            return 7;
        }
    } while (!feof(file));
    fclose(file);
    return 0;
}

int del_some_inf(Node* ptr, int i){// если под данным ключём хранится несколько информаций
    Info* head=ptr->InfoSpace[i];
    int count=0;
    while(head!=NULL){//вывод существующих реализаций
        count++;
        printf("%d, ", head->info);
        head=head->next;
    }
    printf("\n");
    int version, n;
    char *errchoice = "";
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("Select a key release: ");
        n = scanf("%d",&version);
        if(n == 0){
            scanf("%*[^\n]");
            scanf("%*c");
            version = -1;
        }
        if(n<0) return 1;
       // version++;
    } while(version <= 0 || version>count);
    version++;
    Info* prev=ptr->InfoSpace[i];
    head=ptr->InfoSpace[i];
    for(int j=1; j<version; j++){
        prev=head;
        head=head->next;
    }
    if(prev==head)ptr->InfoSpace[i]=head->next;                       
    else prev->next=head->next;
    head->next=NULL;                        
    free(head);
    return 0;
}

void offset(Node* tmp, int k, int y){//смещение: внутри узлаБ если там убирается ключ и смещение его детей
//	tmp->KeySpace[k]=NULL;
    for(int i=k; i<2; i++){
        tmp->KeySpace[i]=tmp->KeySpace[i+1];
        tmp->InfoSpace[i]=tmp->InfoSpace[i+1];
    }
    for(int i=y; i<3; i++)tmp->NodeSpace[i]=tmp->NodeSpace[i+1];
    if(y<4)tmp->NodeSpace[3]=NULL;
    tmp->KeySpace[2]=NULL;
    tmp->InfoSpace[2]=NULL;
}
int destribution(Node* ptr){ //перераспределение: корень спускается на место пустого узла, 
                            //вместо корня поднимается другой элемент с левого или правого поддерева, которые не пусты
    Node* parent=ptr->parent;
    int i=0;
   // if()
    for(; i<4; i++)if(parent->NodeSpace[i]==ptr)break;
    if(i>0 && parent->NodeSpace[i-1]->KeySpace[1]!=NULL){
        add_in_peak(ptr, parent->KeySpace[i-1], parent->InfoSpace[i-1]);
        int j=2;
        for(; j>-1; j--)if(parent->NodeSpace[i-1]->KeySpace[j]!=NULL)break;
        parent->KeySpace[i-1]=parent->NodeSpace[i-1]->KeySpace[j];
        parent->InfoSpace[i-1]=parent->NodeSpace[i-1]->InfoSpace[j];
        parent->NodeSpace[i-1]->KeySpace[j]=NULL;
        parent->NodeSpace[i-1]->InfoSpace[j]=NULL;
        for(int k=3;k>0;k--)ptr->NodeSpace[k]=ptr->NodeSpace[k-1];
        ptr->NodeSpace[0]=parent->NodeSpace[i-1]->NodeSpace[j+1];
        if(ptr->NodeSpace[0]!=NULL)ptr->NodeSpace[0]->parent=ptr;
        parent->NodeSpace[i-1]->NodeSpace[j+1]=NULL;
        return 0;
    }
    if(i<3 && parent->NodeSpace[i+1]!=NULL && parent->NodeSpace[i+1]->KeySpace[1]!=NULL){
        add_in_peak(ptr, parent->KeySpace[i], parent->InfoSpace[i]);
        int j=0;
        while(j<4 && ptr->NodeSpace[j]!=NULL)j++;
        if(ptr->NodeSpace[0]!=NULL){
            ptr->NodeSpace[j]=parent->NodeSpace[i+1]->NodeSpace[0];
            ptr->NodeSpace[j]->parent=ptr;
        }
        parent->KeySpace[i]=parent->NodeSpace[i+1]->KeySpace[0];
        parent->InfoSpace[i]=parent->NodeSpace[i+1]->InfoSpace[0];
        if(ptr->NodeSpace[0]==NULL)offset(parent->NodeSpace[i+1], 0, 4);
        else offset(parent->NodeSpace[i+1],0,0);     
        return 0;
    }
    return 1;
}

Node* destribution2(Node* ptr){ //перераспределение: корень спускается на место пустого узла
    Node* parent=ptr->parent;
    int i=0;//позиция элемента относительно родительского узла
    for(; i<3; i++)if(parent->NodeSpace[i]==ptr)break;
    if(i>0){
        add_in_peak(ptr, parent->KeySpace[i-1], parent->InfoSpace[i-1]);
        add_in_peak(ptr, parent->NodeSpace[i-1]->KeySpace[0], parent->NodeSpace[i-1]->InfoSpace[0]);
        if(ptr->NodeSpace[0]!=NULL){
	        ptr->NodeSpace[3]=ptr->NodeSpace[1];
	        ptr->NodeSpace[2]=ptr->NodeSpace[0];
	        ptr->NodeSpace[1]=parent->NodeSpace[i-1]->NodeSpace[1];
	        ptr->NodeSpace[0]=parent->NodeSpace[i-1]->NodeSpace[0];
            ptr->NodeSpace[1]->parent=ptr;
            ptr->NodeSpace[0]->parent=ptr;
        }
        free(parent->NodeSpace[i-1]);
        offset(parent, i-1, i-1);
        return parent->NodeSpace[i-1];
    }
    else{
        add_in_peak(ptr, parent->KeySpace[i], parent->InfoSpace[i]);
        add_in_peak(ptr, parent->NodeSpace[i+1]->KeySpace[0], parent->NodeSpace[i+1]->InfoSpace[0]);
        ptr->NodeSpace[3]=parent->NodeSpace[i+1]->NodeSpace[1];
        ptr->NodeSpace[2]=parent->NodeSpace[i+1]->NodeSpace[0];
        if(ptr->NodeSpace[0]!=NULL){
            ptr->NodeSpace[3]->parent=ptr;
            ptr->NodeSpace[2]->parent=ptr;
        }
        free(parent->NodeSpace[i+1]);
        offset(parent, i, i+1);
        return ptr;
    }
    return NULL;
}
Node* situation1(Node* y, Node* parent, int i){
    int rc=-1;
    if(y->KeySpace[1]==NULL)rc=destribution(y);
    if(rc==1 && y->KeySpace[1]==NULL)y=destribution2(y);
    return y;
}

Node* process(Node* x){// обработка корня
    if(x->KeySpace[1]==NULL){
        Node* y=x->NodeSpace[0];
        Node* z=x->NodeSpace[1];
        if(y->KeySpace[1]==NULL && z->KeySpace[1]==NULL){
            add_in_peak(x, y->KeySpace[0],y->InfoSpace[0]);
            add_in_peak(x, z->KeySpace[0], z->InfoSpace[0]);
            x->NodeSpace[0]=y->NodeSpace[0];
            x->NodeSpace[1]=y->NodeSpace[1];
            x->NodeSpace[2]=z->NodeSpace[0];
            x->NodeSpace[3]=z->NodeSpace[1];
            if(x->NodeSpace[0]!=NULL){
            	x->NodeSpace[0]->parent=x;
            	x->NodeSpace[1]->parent=x;
            	x->NodeSpace[2]->parent=x;
            	x->NodeSpace[3]->parent=x;		
            }
            free(y);
            free(z);
        }
    }
    return x;
}

Node* find_right(Node* y){
    while(y->NodeSpace[0]!=NULL){
        int j=0;
        while(j<4 && y->NodeSpace[j]!=NULL)j++;
        y=y->NodeSpace[j-1];
    }
    return y;
}

Node* find_left(Node* y){
    while(y->NodeSpace[0]!=NULL){
        y=y->NodeSpace[0];
    }
    return y;
}

void situation2(Node* x, int i){
    Node* y=x->NodeSpace[i];
    Node* z=x->NodeSpace[i+1];
    if(y->KeySpace[1]!=NULL){
        Node* tmp=find_right(y);
        int j=0;
        while( j<3 && tmp->KeySpace[j]!=NULL)j++;
        j--;
        free(x->KeySpace[i]);
        free(x->InfoSpace[i]);
        char* new=strdup(tmp->KeySpace[j]);
        x->KeySpace[i]=new;
        Info* newinf=(Info*)malloc(sizeof(Info));
        newinf->info=tmp->InfoSpace[j]->info;
        newinf->next=tmp->InfoSpace[j]->next;
        tmp->InfoSpace[j]->next=NULL;
        x->InfoSpace[i]=newinf;
        symbiosis(y,tmp->KeySpace[j]);
        return ;
    }
    else if(z->KeySpace[1]!=NULL){
        Node* tmp=find_left(z);
        free(x->KeySpace[i]);
        free(x->InfoSpace[i]);
        char* new=strdup(tmp->KeySpace[0]);
        Info* newinf=(Info*)malloc(sizeof(Info));
        newinf->info=tmp->InfoSpace[0]->info;
        newinf->next=tmp->InfoSpace[0]->next;
        tmp->InfoSpace[0]->next=NULL;
        x->KeySpace[i]=new;
        x->InfoSpace[i]=newinf;
        symbiosis(z, tmp->KeySpace[0]);
        return ;
    }
    else{
        char* k=x->KeySpace[i];
        add_in_peak(y, x->KeySpace[i], x->InfoSpace[i]);
        add_in_peak(y, z->KeySpace[0], z->InfoSpace[0]);
        y->NodeSpace[2]=z->NodeSpace[0];
        y->NodeSpace[3]=z->NodeSpace[1];
        if(y->NodeSpace[0]!=NULL){
            y->NodeSpace[2]->parent=y;
            y->NodeSpace[3]->parent=y;
        }
        free(z);
        offset(x,i,i+1);
        symbiosis(y,k);
        return ;
    }
}

void symbiosis(Node* head, char* key){
    head=process(head);
   // printf("%s", head->KeySpace[0]);
    while(head->NodeSpace[0]!=NULL){
        int j=0; //поиск ключа если он есть в корне дерева
        for(; j<3; j++){
            if(head->KeySpace[j]==NULL){
                j=3;
                break;
            }
            if(strcmp(key, head->KeySpace[j])==0)break;
        }
        int i=0;//позиция ключа относительно root, номер ребёнка
        for(; i<3 && head->KeySpace[i]!=NULL; i++)if(strcmp(key, head->KeySpace[i])==-1)break;
        if(j==3) head=situation1(head->NodeSpace[i], head, i);
        else{
            situation2(head,j);
            return ;
        }
    }
    printf(" ny");
    base_del(head,key);
    return ;    
}

void base_del(Node* ptr, char* key){
    int count2;
    for(int i=0; i<3 && ptr->KeySpace[i]!=NULL; i++){
        if(strcmp(ptr->KeySpace[i], key)==0){             
            count2 = i; // номер, который занимает ключ в узле
            break;
        }
    }
    free(ptr->KeySpace[count2]);
    free(ptr->InfoSpace[count2]);
    offset(ptr,count2,4);
    return ;
}

int del_list(Node* ptr, int count, Tree* tree){
    char* key=ptr->KeySpace[count];
    if(ptr->KeySpace[1]!=NULL){//если в основании листа несколько элементов
            free(ptr->KeySpace[count]);
            free(ptr->InfoSpace[count]);
            offset(ptr,count, 4);
            return 0;
        }
    else{// если в основании листа 1 элемент
        Node* parent=ptr->parent;
        if(parent==NULL){// если в дереве один элемент
            free(ptr->InfoSpace[0]);
            free(ptr->KeySpace[0]);
            free(ptr);
            tree->root=NULL;
            return 0;
        }
        for(int i=0; i<4 && parent->NodeSpace[i]!=NULL; i++){
                int rc=destribution(ptr); // если в соседнем листе не один элемент 
            if(parent->NodeSpace[i]==ptr ){// нашли позицию удаляемого листа в родительском узле
                if(rc==0){
                    base_del(ptr, key);  
                    return 0;
                }
                if(rc==1 && parent->KeySpace[1]!=NULL){
                    char* key=ptr->KeySpace[0];
                    ptr=destribution2(ptr);//если в родительском узле не один элемент
                    base_del(ptr, key);
                    return 0;
                }
                else symbiosis(tree->root, ptr->KeySpace[count]);
                break;
            }
        }
    }
}

int del(Tree* tree, char* key){
    if(tree->root==NULL)return 2;//если дерево пустое
    Node* ptr=find_node(tree, key);//поиск узла, содержащего ключ
    if(ptr==NULL)return 1;// если ключ не найден
    int count;
    for(int i=0; i<3 && ptr->KeySpace[i]!=NULL; i++){
        if(strcmp(ptr->KeySpace[i], key)==0){
            if(ptr->InfoSpace[i]->next!=NULL){
                int q=del_some_inf(ptr, i);// если у нас несколько информации по заданному ключу
                if(q==1) return 3;
                return 0;
            }
            count = i; // номер, который занимает ключ в узле
            break;
        }
    }
    printf("???%d %s???",count,ptr->KeySpace[count]);    
    if(ptr->NodeSpace[0]==NULL){ //мы дошли до листа
        del_list(ptr, count, tree);
        return 0;
    }
    else{
        symbiosis(tree->root, key);
        return 0;
    }
}

void special_find(Tree* t){
	Node* head=t->root;
	while(head->NodeSpace[0]!=NULL){
		int i=0;
		while(i<3 && head->NodeSpace[i+1]!=NULL)i++;
		head=head->NodeSpace[i];
	}
	int j=0; 
	while(head->KeySpace[j+1]!=NULL)j++;
	printf("%s: ", head->KeySpace[j]);
	Info* tmp=head->InfoSpace[j];
	while(tmp!=NULL){
		printf("%d, ", tmp->info);
		tmp=tmp->next;
	}
}

int D_Show(Tree *t){
    const char *errmsgs[] = {"Successfully", "Table is empty"};
    int rc;
    rc = print(t->root);
    if(t->root==NULL)rc=1;
    printf("%s!\n", errmsgs[rc]);
    return 1;
}

int D_Find(Tree *t){
    const char *errmsgs[] = {"Successfully", "Key not found"};
    int n;
    Info* info;
    Node* rc=t->root;
    char* key;
    char* errchoice="";
    printf("Enter key: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    key = getstr();
    if (key == NULL) return 0;
    info = find_el_rek(rc, key);
    if(info==NULL)printf("%s\n", errmsgs[1]);
    else{
        printf("%s\nkey =  %s\n",errmsgs[0], key);
        while(info!=NULL){
            printf("%d", info->info);
            info=info->next;
        }
    }
    free(key);
    return 1;
}

int D_Add(Tree *t){ 
    int rc, n, info, rk;
    char *key;
    char *errchoice = "";
    printf("Enter key:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    key = getstr();
    if (key == NULL) return 0;
    do{
        printf("%s",errchoice);
        errchoice = "You are wrong. Repeate, please!\n";
        printf("Enter information: -->");
        n = scanf("%d",&info);
        if(n == 0){
                scanf("%*[^\n]");
                scanf("%*c");
                info = -1;
        }
        if(n<0) return 0;
    } while(info < 0);
    char* new=strdup(key);
    rc = add_el(t, key, info);
    if(rc==0 || rc==5) printf("Successfully! %s\n", new);
    if(rc==1) printf("Duplicate key %s\n", key);
    free(new);
    return 1;
}

int draw2(Node* ptr, int len){
	int k, t=0;
	if(ptr==NULL)return 0;
	int j=0;
//	if(ptr->NodeSpace[0]==NULL){
	for(int i=0; i<len-1; i++)printf(" ");
	printf("|\n");
	while(j<3 && ptr->KeySpace[j]!=NULL){
		if(t<strlen(ptr->KeySpace[j]));
		t=strlen(ptr->KeySpace[j]);
		for(int i=0; i<len; i++)printf(" ");
		if(j==0)printf("--");
		else printf("  ");
		printf("%s\n",ptr->KeySpace[j]);
		j++;
	}
	if(ptr->NodeSpace[0]==NULL)return 1;
//	}
	k=draw2(ptr->NodeSpace[0], len+t+4);
/*	if(k==1){
		for(int i=0; i<len; i++)printf(" ");
		printf("--%s\n",ptr->KeySpace[0]);
	}*/
	if(ptr->KeySpace[1]!=NULL){
		k=draw2(ptr->NodeSpace[1], len+t+4);
	//	if(k==1){
	//		for(int i=0; i<len; i++)printf(" ");
	//		printf("--%s\n",ptr->KeySpace[1]);
	//	}
	}
	else{
		k=draw2(ptr->NodeSpace[1], len+t+4);
		return 1;
	}
	if(ptr->KeySpace[2]!=NULL){
		k=draw2(ptr->NodeSpace[2], len+t+4);
	/*	if(k==1){
			for(int i=0; i<len; i++)printf(" ");
			printf("--%s\n",ptr->KeySpace[2]);
		}*/
	}
	else{
		k=draw2(ptr->NodeSpace[2], len+t+4);
		return 1;
	}
	k=draw2(ptr->NodeSpace[3], len+t+4);
	return 1;
}

int draw(Node* ptr, int len){
	int k;
	if(ptr==NULL)return 0;
	int j=0;
	if(ptr->NodeSpace[0]==NULL){
	while(ptr->KeySpace[j]!=NULL){
		for(int i=0; i<len; i++)printf(" ");
		printf("--");
		printf("%s\n",ptr->KeySpace[j]);
		j++;
	}
	return 1;
	}
	k=draw(ptr->NodeSpace[0], len+strlen(ptr->KeySpace[0])+4);
	if(k==1){
		for(int i=0; i<len; i++)printf(" ");
		printf("--%s\n",ptr->KeySpace[0]);
	}
	if(ptr->KeySpace[1]!=NULL){
		k=draw(ptr->NodeSpace[1], len+strlen(ptr->KeySpace[1])+4);
		if(k==1){
			for(int i=0; i<len; i++)printf(" ");
			printf("--%s\n",ptr->KeySpace[1]);
		}
	}
	else{
		k=draw(ptr->NodeSpace[1], len+strlen(ptr->KeySpace[0])+4);
		return 1;
	}
	if(ptr->KeySpace[2]!=NULL){
		k=draw(ptr->NodeSpace[2], len+strlen(ptr->KeySpace[2])+4);
		if(k==1){
			for(int i=0; i<len; i++)printf(" ");
			printf("--%s\n",ptr->KeySpace[2]);
		}
	}
	else{
		k=draw(ptr->NodeSpace[2], len+strlen(ptr->KeySpace[1])+4);
		return 1;
	}
	k=draw(ptr->NodeSpace[3], len+strlen(ptr->KeySpace[2])+4);
	return 1;
}

int D_Draw(Tree *t){
	int len=1;
	Node* tmp=t->root;
	int rc=draw2(tmp,len);
	if(rc==0)printf("Table is empty\n");
	return 1;
		
}

int D_Special(Tree *t){
	if(t->root==NULL){
		printf("Table is empty\n");
		return 1;
	}
	special_find(t);
	return 1;
}
int D_Del(Tree *t){
    const char *errmsgs[] = {"Successfully","Key not found", "Table is empty"};
    int rc, n;
    char* key;
    printf("Enter key: -->");
    scanf("%*[^\n]");
    scanf("%*c");
    key = getstr();
    if (key == NULL) return 0;
    rc = del(t, key);
    if(rc==3) return 0;
    printf("%s: %s\n", errmsgs[rc], key);
    free(key);
    return 1;
}

int D_Load(Tree *t){
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key", "Key not found", "Table is empty", "Error opening file", "File format incorrec"};
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

int main(){
    const char* choice[]={"0- Exit", "1- Add element", "2- Find element",
    "3- Delete element", "4- Show table", "5- Special find", "6- Add elements from a text file", "7- Draw a tree"};
    const int N=sizeof(choice)/sizeof(choice[0]);
    int (*fptr[])(Tree*) = {NULL, D_Add, D_Find, D_Del, D_Show, D_Special, D_Load, D_Draw};
    Tree* t=malloc(sizeof(Tree));
    t->root=NULL; 
    int rc;
    while(rc = dialog(choice, N)){
        if(rc==-1)break;
        if(!fptr[rc](t))break;
    }
    destr(t);
    printf("That's all. Bye!\n");
    return 0;
}
