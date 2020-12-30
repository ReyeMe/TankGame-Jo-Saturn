#ifndef __LinkedList_H__
#define __LinkedList_H__

/**
 * Linked list node
 */
typedef struct linked_List linked_List;
struct linked_List
{
    void *Current;
    linked_List *Next;
};

/**
 * Initialize list
 * @param list Allocated linked list
 */
void linkedList_Initialize(linked_List * list);

/**
 * Clear list
 * @param list Allocated linked list
 */
void linkedList_Clear_all(linked_List * list);

/**
 * Load map file
 * @param list Allocated linked list
 * @param item Item to add
 */
void linkedList_Add(linked_List * list, void * item);

/**
 * Load map file
 * @param list Allocated linked list
 * @param item Item to remove
 */
void linkedList_Remove(linked_List * list, void * item);

#endif