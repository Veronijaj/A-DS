#ifndef FUNC_H
#define FUNC_H

typedef struct stack stack;

stack* create_node(char* lom);
stack* new_elem(stack* list, char* lom);
void clean(stack** list);
void print_history(stack* list);
char* open_directory();
void empty(stack* list);
char* getstr();

#endif