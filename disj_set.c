#include <stdlib.h>
#include "disj_set.h"
#include "mem.h"

/**
 * Function makes a set consisting a single "node"
 * This "node" will have value given from param, no parent and depth of 1
 * @param value Wanted value to be stored
 * @return Pointer to element with wanted value
 */
disj_set_element *disj_set_make_set(uint value) {
    // Init
    disj_set_element *temp = NULL;

    // Mem alloc
    temp = (disj_set_element *) mymalloc(sizeof(disj_set_element));
    if (!temp) return NULL;

    // Fill the temp
    temp->parent = NULL;
    temp->value = value;
    temp->depth = 1;

    return temp;
}

/**
 * Function finds the representative of the set
 * plus fixes the path to the representative, so the next time this function is called
 * it will be O(1)
 * @param el Pointer to disjoint set data structure element, that is being examined
 * @return Pointer to element representing the representative parent of the set
 */
disj_set_element *disj_set_find(disj_set_element *el) {
    // Init
    disj_set_element *orig = el;

    // Sanity check
    if (!el) return NULL;

    // Get to the repre of the set
    while (el->parent != NULL) el = el->parent;

    // Compress the path, so it's eventually O(1)
    if (el != orig->parent && el != orig) orig->parent = el;

    return el;
}

/**
 * Function unions two disjoint sets, uses find() function
 * If the sets were not disjoint to start with, will just return pointer to the main root
 * If the sets were disjoint, it will merge them by depth, so the optimum complexity can be achieved
 * @param el1 Pointer to element1 that is to be union-ed
 * @param el2 Pointer to element2 that is to be union-ed
 * @return Pointer to element representing the new representative parent of the new made set
 */
disj_set_element *disj_set_union(disj_set_element *el1, disj_set_element *el2) {
    // Init
    disj_set_element *root1 = NULL;
    disj_set_element *root2 = NULL;
    disj_set_element *new_root = NULL;

    // Sanity check
    if (!el1 || !el2) return NULL;

    // Find root parent of input elements
    root1 = disj_set_find(el1);
    root2 = disj_set_find(el2);

    // Set new root to one of them
    new_root = root1;

    // Merge if root parents are different, merge by depth
    if (root1 != root2) {
        if (root1->depth > root2->depth) {
            root2->parent = root1;
            root1->depth++;
        } else {
            root1->parent = root2;
            root2->depth++;
            // Don't forget to change the repre new root parent, which is defaultly set to root1
            new_root = root2;
        }
    }

    return new_root;
}

/**
 * Function frees one element of the disjoint set data structure
 * @param el Double pointer to an element that is to be freed
 */
void disj_set_element_free(disj_set_element **el) {
    // Sanity check
    if (!*el) return;

    myfree((void **) el);
    *el = NULL;
}