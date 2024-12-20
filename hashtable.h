#include <stdio.h>
#include <ctype.h>
#include "linked_l.h"

typedef struct hashmap hashmap;  
typedef struct key_pair key_pair;

struct hashmap{
    //array of linked lists
    linked_list **arr;
    //the size of the hashtable as 1.3 * #possible elements
    unsigned int size;
};
struct key_pair{
    //storing values in array, with the code as the index
    unsigned int c;
    unsigned int pref;
};

/// @brief initialized the hashmap, sets the size 
/// @param num_elements the number of elements being stored in the hashmap-- this will be the size
/// @return returns the created hashmap
hashmap *create_hm(unsigned int num_elements);

/// @brief the hash function
/// @param pref
/// @param c the character
/// @param size the size of the hashmap
/// @return return_description
u_int64_t Hash(int pref, unsigned int c, unsigned int size);


/// @brief free all the data associated with the hashmap 
/// @param hm pointer to the hashmap
void destroy_hm(hashmap *hm);

/// @brief gets a value from the hashmap given (pref,c)
/// @param pref
/// @param c the character
/// @return the value
void* get_val(hashmap *hm, int pref, unsigned int c);

/// @brief sets a value to the hashmap with key (pref,c)
/// @param pref
/// @param c the character
/// @return NULL if no 'collision' with old value, the old value if it is being overwritten
void* set_val(hashmap *hm,int pref, unsigned int c, unsigned int val);

/// @brief prints the contents of the hashmap, codes, to prefix and values
/// @param hm pointer to the hashmap
void dump_hm(FILE *fptr, hashmap *hm);

/// @brief returns a sorted table of code ->key_pair
/// @param size the size of the resulting array (ie largest code)
/// @return a sorted table of code ->key_pair
key_pair* hm_table(hashmap *hm, unsigned int size);