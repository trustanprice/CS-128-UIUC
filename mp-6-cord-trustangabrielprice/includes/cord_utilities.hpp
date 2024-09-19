#ifndef CORD_UTILITIES_HPP
#define CORD_UTILITIES_HPP

#include "cord.hpp"
#include "shared_pointer.hpp"

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                SharedPointer<Cord> right_cord);

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                              unsigned int lower_idx,
                              unsigned int upper_idx); 
                              /* base cases: 
                              1. substringing the entire thing 
                              2. if you are at a leaf (would have to create a new cord) 
                              3. RECURSIVE CASES
                                i. if my substring is enitrely within substring left
                                ii. if my substring is enitrely within substring right
                                iii. combination of both (recursively call both then concat them at the end)
                            DO THESE IN ORDER TO ALLEVIATE HAVING TO REDO THE RECURSIVE ORDERING */


#endif