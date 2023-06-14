#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
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

void destructor(Tree* t){
    Node* item = t->root;
    Node* tmp;
    while(item!=NULL){
        tmp =item;
        item = item->next;
        free(tmp->info);
        free(tmp);
    }
}

void deep (Tree* t){
	Node* ptr=t->root;
	int arr[50];
	int top=0;
	int bottom=-1;
	int tmp=0;
	if(ptr==NULL){
		t->depth=0;
		return ;
	}
	int max=0;
	while(ptr!=NULL){
		max++;
		if(max>tmp)tmp=max;
		if(ptr->right!=NULL){
			bottom++;
			arr[bottom]=max;
		}
		if(ptr->left!=NULL)ptr=ptr->left;
		else{
			ptr=ptr->next;
			if(ptr!=NULL){
				max=arr[top];
				top++;
			}
		}
	}
	t->depth=tmp;
}

void sew2 (Tree *t) {
    Node* r = t->root;
    Node* tmp=NULL;
    Node* head=NULL;
    Node* re=NULL;
    while(1){
        if(r->left!=NULL && r->right!=NULL){
            tmp=head;
            head=r->right;
            head->next=tmp;
            r->next=r->left;
            r=r->left;
        }
        if(r->left==NULL && r->right!=NULL){
            r->next=r->right;
            r=r->right;
        }
        if(r->left!=NULL && r->right==NULL){
            r->next=r->left;
            r=r->left;
        }
        if(r->left==NULL && r->right==NULL){
            r->next=head;
            r=r->next;
            if(head==NULL) break;
            head=head->next;
        }
    }
}


Node* create_root(int k, char* info){
    Node* n1=(Node*)malloc(sizeof(Node));
    n1->key=k;
    n1->info=info;
    n1->left=NULL;
    n1->right=NULL;
    n1->next=NULL;
    return n1;
}

int add_el(Tree* t,int k,char* info){
    Node* item=t->root;
    Node* peak;
    int count=1;
    if(find_el_rek(item,k)!=NULL) return 1;
    Node* now=create_root(k, info);
    t->number++;
    if(item == NULL){
        t->root=now;
	t->depth=1;
        return 0;
    }
    while(item != NULL){
	count++;
        peak=item;
        if(k<item->key)item=item->left;
        else item=item->right;
    }
    if(k<peak->key)peak->left=now;
    if(k>peak->key)peak->right=now;
    if(count>t->depth)t->depth=count;
    sew2(t);
    return 0;
}

void Table(Tree* t){
	scanf("%*[^\n]");
	scanf("%*c");
	char* inf=getstr();
	int count=200;
	int a=357;
	for(int i=0; i<count; i++){
		int k=rand()%1000;
		char* kol=strdup(inf);
		int res=add_el(t, k, kol);
		if(res==0){
			clock_t begin=clock();
			Vector* rc=special_find(t->root,a);
			for(int i=0; i<rc->count; i++)printf("key=%u, info=\"%s\"\n",rc->space[i]->key, rc->space[i]->info);
			clock_t end=clock();
   			double total=(double)(end-begin)/CLOCKS_PER_SEC;
			printf("%d. %lf\n",i+1,total);
			
		}
		else i--;
	}
//	free(inf);
}

int draw(Tree* t){
    if(t->root==NULL) return 1;//top-индекс в котором находится начало
    Node* arr[100]={NULL};//bottom-индекс в котором находится конец
    int top=0, bottom=0, level=2, count=2, max=1, len,k=1, num=0, num2=0;//level-уровень на котором сейчас находится дерево
    arr[0]=t->root;//count-количество потомков которое будет на следующем уровне
    for(int j=1; j<t->depth-1; j++)max*=2;//max-максимальное количество элементов которое может существовать на последнем уровне
    if(max==1)len=0;//len-количество клеточек между элементами на текущем уровне
    else len = (max*2-1)*4;
    while(num<t->number){
        for(int j=0; j<len; j++)printf(" ");
        if(arr[top]!=NULL){
            num++;
            printf("%4d",arr[top]->key);
        }
        if(arr[top]==NULL) printf("    ");
        for(int j=0; j<len+4; j++)printf(" ");
        bottom++;
        if(arr[top]!=NULL && arr[top]->left!=NULL)arr[bottom]=arr[top]->left;
        bottom++;
        if(arr[top]!=NULL && arr[top]->right!=NULL)arr[bottom]=arr[top]->right;
        top++;
        if(count == level){
            max/=2;
            int len2=len;
            printf("\n");
            int kol=num2;
            for(int i=0;i<t->depth-k;i++){
	           // for(int j=0; j<len2; j++)printf(" ");
	           // printf("/");
	            num2=kol; 
            	for(int j=0;j<count; j++){
            		if(j%2==1 || j==1){
            			for(int p=0; p<4*i; p++)printf(" ");	
            			if (arr[num2]!=NULL && arr[num2]->right!=NULL)printf("\\");
            			//if (arr[top-1]!=NULL && arr[top-1]->right!=NULL)printf("\\");
            			else printf(" ");
            			for(int p=0; p<len2+4; p++)printf(" ");
            			num2++;
            		}
            		if(j%2==0 || j==0){
            			for(int p=0; p<len2; p++)printf(" ");	
            		//	if (arr[top-1]==NULL || arr[top-1]->left==NULL)printf(" ");
            		//	if (arr[top-1]!=NULL && arr[top-1]->left!=NULL)printf("/");
            			if (arr[num2]!=NULL && arr[num2]->left!=NULL)printf("/");
            			else printf(" ");
            			for(int p=0; p<2+4*i; p++)printf(" ");       			 
            		}
            	}
            	len2-=4;
            	printf("\n");
            	
            }
            k++;
            len = (max*2-1)*4;
            if(max==0)len=0;
            count = 0;
            level*=2;
        }
        count+=2;
    }
    printf("\n");
    return 0;
}

void convert(int key,FILE* file){
	int k=key;
	int len=0;
	while(k!=0){
		len++;
		k/=10;
	}
	printf("%d", len);
	char str[len];
	int ls=len;
	k=key;
	while(k>9){
		str[len-1]=k%10+'0';
		k/=10;
		len--;
	}
	str[0]=k+'0';
	fwrite(str,sizeof(char),ls,file);
}

void makegv(Tree* t, FILE* file){
	Node* ptr = t->root;
//	if(ptr==NULL)return 1;
	int count=0;
//	printf("%d",ptr->key);
	char str[]={'-','>','\t','\n','\"'};
	char* str2;
	while(ptr!=NULL){
		if(ptr->left!=NULL){
			fwrite(str+2, sizeof(char), 1, file);
			fwrite(str+4, sizeof(char), 1, file);
			convert(ptr->key, file);
			fwrite(str+4, sizeof(char), 1, file);
			fwrite(str, sizeof(char), 1, file);
			fwrite(str+1, sizeof(char), 1, file);
			fwrite(str+4, sizeof(char), 1, file);
			convert(ptr->left->key, file);
			fwrite(str+4, sizeof(char), 1, file);
			fwrite(str+3, sizeof(char), 1, file);
		}
		if(ptr->right!=NULL){
			fwrite(str+2, sizeof(char), 1, file);
			fwrite(str+4, sizeof(char), 1, file);
			convert(ptr->key,file);
			fwrite(str+4, sizeof(char), 1, file);
			fwrite(str, sizeof(char), 1, file);
			fwrite(str+1, sizeof(char), 1, file);
			fwrite(str+4, sizeof(char), 1, file);
			convert(ptr->right->key, file);
			fwrite(str+4, sizeof(char), 1, file);
			fwrite(str+3, sizeof(char), 1, file);
		}
		ptr=ptr->next;
	}
}

int del_el_rek (Tree* t,int key) {
    Node* ptr = t->root;
    ptr=find_el_rek(ptr, key);
    if(ptr==NULL) return 1;
    Node* par = find_parent(t, key);
    printf("%d", par->key);
    t->number--;
    if(ptr->left==NULL){
        if(par==ptr){
        	if(ptr->right==NULL){
        		t->root==NULL;
        		return 0;
        	}
        	t->root=ptr->right;
        }
        if(par->left == ptr) par->left=ptr->right;
        if(par->right == ptr) par->right=ptr->right;
        free(ptr->info);
        free(ptr);
        return 0;
    }
    if(ptr->right==NULL){
        if(par==ptr)t->root=ptr->left;
        if(par->left == ptr) par->left=ptr->left;
        if(par->right == ptr) par->right=ptr->left;
        free(ptr->info);
        free(ptr);
        return 0;
    }
    if(ptr->right!=NULL && ptr->left!=NULL){
        Node* ptr2=ptr->right;
        Node* par2=NULL;
        while(ptr2->left != NULL){
            par2 = ptr2;
            ptr2 = ptr2->left;
        }
        if(par == ptr)t->root=ptr2;
        if(par->left == ptr) par->left=ptr2;
        if(par->right == ptr) par->right=ptr2;
        if(par2!=NULL) par2->left=ptr2->right;
        if(ptr->left != ptr2) ptr2->left=ptr->left;
        if(ptr->right != ptr2) ptr2->right=ptr->right;
        free(ptr->info);
        free(ptr);
        return 0;
    }
}

Node* find_el_rek(Node* ptr, int k){
    if(ptr == NULL) return NULL;
    if(ptr->key == k) return ptr;
    if (k < ptr->key) return find_el_rek(ptr->left, k);
    if (k >= ptr->key) return find_el_rek(ptr->right, k);
}

Vector* special_find(Node* ptr, int k){
    Node* ptr2=ptr;
    Node* ptr3=ptr;
    int len1, len2;
    Vector* sf;
    if(ptr == NULL) return NULL;
    Node** arr=(Node**)malloc(2*sizeof(Node*));
    sf=(Vector*)malloc(sizeof(Vector));
    if(ptr3->left==NULL && ptr3->right==NULL){
        arr[0]=ptr3;
        sf->count=1;
        sf->space=arr;
        return sf;
    }
    while(ptr2->left!=NULL) ptr2=ptr2->left;
    while(ptr->right!=NULL) ptr=ptr->right;
    len1=abs(ptr->key - k);
    len2=abs(k - ptr2->key);
    if(len1>len2){
        arr[0]=ptr;
        sf->count=1;
        sf->space=arr;
        return sf;
    }
    if(len1<len2){
        arr[0]=ptr2;
        sf->count=1;
        sf->space=arr;
        return sf;
    }
    if(len1==len2){
        arr[0]=ptr;
        arr[1]=ptr2;
        sf->count=2;
        sf->space=arr;
        return sf;
    }
}

Node* find_parent(Tree* t, int k){
    Node* ptr = t->root;
    Node* par;
    if(ptr->key == k) return ptr;
    while (ptr != NULL){
        par=ptr;
        if (k<ptr->key) ptr = ptr->left;
        else ptr = ptr->right;
        if (ptr->key == k) return par;
    }
    return NULL;
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

int load(Tree* t, char* fn){//загружает информацию с файла в структурный массив
    int per1, per2, rs, key;
    char* info;
    FILE *file;
    file = fopen(fn, "r");
    if (file == NULL) return 6;
    int read;
    do {
        read = fscanf(file,"%d[^\n]",&key);
        info=fgetstr(file);
        if(info==NULL){
        	printf("kiskis");
        	return 7;
        }
        read++;
        if (read == 2){
            rs=add_el(t,key,info);
            if(rs==1) printf("Duplicate key %u\n", key);
        }
        if (read != 2 && !feof(file)){
        	printf("kiskis");
        	return 7;
        }
        if (ferror(file)) {
        	printf("kiskis");
            fclose(file);
            return 7;
        }
    } while (!feof(file));
    fclose(file);
    return 0;
}

int print(Tree* t){
    Node* ks=t->root;
    if(ks == NULL)return 5;
    while(ks!= NULL){
         printf("%d | %s\n", ks->key, ks->info);
         ks=ks->next;
    }
    return 0;
}

int D_Add(Tree *t){ //робит
	clock_t begin=clock();
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
    } while(k < 0);
    printf("Enter info:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    info = getstr();
    if (info == NULL) return 0;
    rc = add_el(t, k, info);
    if(rc==0) printf("Successfully! %u\n", k);
    if(rc==1) printf("Duplicate key %u\n", k);
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Find(Tree *t){
	clock_t begin=clock();
    int n, k;
    Node* rc=t->root;
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
    rc = find_el_rek(rc, k);
    if(rc==NULL)printf("Key not found\n");
    else printf("key =  %u, info =\" %s \"\n", rc->key, rc->info);
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Load(Tree *t){
	clock_t begin=clock();
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
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Special(Tree *t){
	clock_t begin=clock();
    const char *errmsgs[] = {"Successfully", "Table is empty"};
    int n, k;
    Node* head=t->root;
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
    Vector* rc = special_find(head, k);
    if(rc==NULL)printf("%s\n", errmsgs[1]);
    else{
    	printf("key =  %u:\n", k);
    	for(int i=0; i<rc->count; i++) printf("key = %u, info=\"%s\"\n", rc->space[i]->key, rc->space[i]->info);
    	free(rc->space);	
    	free(rc);
    }
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Load2(Tree* t){ // робит
	clock_t begin=clock();
//    printf("Enter file name: --> ");
 //   scanf("%*[^\n]");
//    scanf("%*c");
 //   char* fn = getstr();
//    if(fn == NULL) return 0;
    FILE* file = fopen("lop.gv", "w");
    if(file==NULL) return 0;
    char str[]="digraph My{\n";
    char str2[]="}";
   // int k = 57;
    fwrite(str,sizeof(char),strlen(str), file);
    makegv(t,file);
    //if(rc==1)printf("Table is empty!\n");
    fwrite(str2,sizeof(char),strlen(str2), file);
	fclose(file);
//	free("");
	clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    system("dot -Tpng lop.gv -o lop.png");
    system("catimg -w 300 lop.png");
    return 1;
}

int D_Graph(Tree* t){
	Table(t);
}

int D_Del(Tree *t){
	clock_t begin=clock();
    const char *errmsgs[] = {"Successfully","Key not found"};
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
    rc = del_el_rek(t, k);
    if(rc == 0){
    	sew2(t);
    	deep(t);
    }
    printf("%s: %u\n", errmsgs[rc], k);
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Show(Tree *t){
	clock_t begin=clock();
    const char *errmsgs[] = {"Successfully", "Duplicate key", "Table overflow", "There isn't parent key",
                             "Key not found", "Table is empty", "Error opening file", "File format incorrect"};
    int rc;
    rc = print(t);
    printf("%s!\n", errmsgs[rc]);
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
    return 1;
}

int D_Draw(Tree *t){
	clock_t begin=clock();
    const char *errmsgs[] = {"Successfully","Table is empty"};
    int rc;
    rc = draw(t);
    printf("%s!\n", errmsgs[rc]);
    clock_t end=clock();
    double total=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("%lf second program was in process\n",total);
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
