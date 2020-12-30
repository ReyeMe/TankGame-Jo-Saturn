#include <jo/jo.h>
#include "linkedList.h"

void linkedList_Initialize(linked_List * list)
{
    list->Current = JO_NULL;
    list->Next = JO_NULL;
}

void linkedList_Clear_all(linked_List * list)
{
    linked_List *current = list;

    while (current != JO_NULL)
    {
        if (current->Current != JO_NULL)
        {
            jo_free(current->Current);
            current->Current = JO_NULL;
        }

        linked_List *next = current->Next;
        current->Next = JO_NULL;

        if (list != current)
            jo_free(current);

        current = next;
    }
}

void linkedList_Add(linked_List * list, void * item)
{
    linked_List *current = list;

    while (current->Current != JO_NULL)
    {
        current = current->Next;
    }

    current->Current = item;

    // Add next empty item
    if (current->Next == JO_NULL)
    {
        linked_List *newItem = jo_malloc(sizeof(linked_List));
        newItem->Current = JO_NULL;
        newItem->Next = JO_NULL;
        current->Next = newItem;
    }
}

void linkedList_Remove(linked_List * list, void * item)
{
    linked_List *last = JO_NULL;
    linked_List *current = list;

    while (current->Next != JO_NULL && current->Current != item)
    {
        last = current;
        current = current->Next;
    }

    if (current->Current == item)
    {
        jo_free(current->Current);
        current->Current = JO_NULL;

        if (last != JO_NULL)
        {
            linked_List *next = current->Next;
            jo_free(current);
            last->Next = next;
        }
    }
}