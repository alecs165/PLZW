#include "linked_l.h"

//linked list functions
void l_l_free(linked_list *ns){
  if(ns == NULL){
    return;
  }
  l_l_free(ns->rest);
  free(ns);
}

linked_list *l_cons(unsigned int v, unsigned int pref, unsigned int c, linked_list *r){
  linked_list *rtn = malloc(sizeof(linked_list));
  //constructing the new pointers to return
  rtn->val = v;
  rtn->pref = pref;
  rtn->c = c;
  rtn->rest = r;
  return rtn;
}