#include "stack.h"
void stack_free(stack *ns){
  if(ns == NULL){
    return;
  }
  stack_free(ns->rest);
  free(ns);
}

stack* init_stack(stack* s){
  stack *rtn = calloc(1, sizeof(stack));
  //constructing the new pointers to return
  rtn->size = 0;
  return rtn;
}

stack *push(unsigned int v, stack *r){
  stack *rtn = malloc(sizeof(stack));
  if(rtn == NULL){
    fprintf(stderr, "malloc failed");
    exit(1);
  }
  //constructing the new pointers to return
  rtn->val = v;
  rtn->rest = r;
  rtn->size = r->size + 1;
  return rtn;
}

unsigned int pop(stack **r){
  if((*r)->size == 0){
    return -1;
  }
  unsigned int rtn = (*r)->val;
  stack *t = *r;
  *r = (*r)->rest;
  free(t);
  return rtn;
}