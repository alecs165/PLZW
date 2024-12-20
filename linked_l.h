#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct linked_list linked_list;  
struct linked_list{
    //storing all the possible values in case
    //we run into collisions in the hash function
    unsigned int val;
    unsigned int c;
    unsigned int pref;
    linked_list *rest;
};

/// @brief frees each element of the linked list
/// @param ns the linked list to free
void l_l_free(linked_list *ns);

/// @brief adds an element to the front of the list
/// @param v character
/// @param pref prefix
/// @param c code value
/// @param r the list
/// @return the modified list
linked_list *l_cons(unsigned int v, unsigned int pref, unsigned int c, linked_list *r);