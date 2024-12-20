
#ifndef AB1D5576_F986_4B13_AE08_ABCF50CDF907
#define AB1D5576_F986_4B13_AE08_ABCF50CDF907
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "hashtable.h"
#include "stack.h" 

/// @brief sets hashmap equal to a pruned hashmap, pushes the new
/// code values to the stack
/// @param hm the hashmap
/// @param s stack of codes that must be changed
/// @param val_ctr biggest code (should be 1<<maxbits - 1)
/// @return the last code
unsigned int prune(hashmap **hm, unsigned int max_bits);

/// @brief adds an element to the front of the list
/// @param p
/// @param max_bit_length prefix
/// @return the encoded ints
void encode(unsigned int p, unsigned int max_bit_length);



#endif /* AB1D5576_F986_4B13_AE08_ABCF50CDF907 */ 