#ifndef __DISJ_SET_H__
#define __DISJ_SET_H__

#ifndef __UINT__
#define __UINT__
typedef unsigned int uint;
#endif

typedef struct the_disj_set_element {
    struct the_disj_set_element *parent;
    uint value;
    uint depth;
} disj_set_element;

disj_set_element *disj_set_make_set(uint value);
disj_set_element *disj_set_find(disj_set_element *el);
disj_set_element *disj_set_union(disj_set_element *el1, disj_set_element *el2);
void disj_set_element_free(disj_set_element **el);

#endif