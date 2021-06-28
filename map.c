#include "map.h"

struct MapCustom *create_map()
{
    struct MapCustom *map = (struct MapCustom *)malloc(sizeof(struct MapCustom));
    map->head = NULL;
    map->current = NULL;
    map->size = 0;
}
struct OpProcesor *get_value_of_map(struct MapCustom *map, int key)
{
    struct EntryMap *current = map->head;
    if ((map->head) == NULL)
    {
        return NULL;
    }
    while (current->key != key)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current->value;
}
void put_value(struct MapCustom *map, int key, struct OpProcesor *info_data)
{
    struct EntryMap *new_node = (struct EntryMap *) malloc(sizeof(struct EntryMap));
    new_node->key=key;
    new_node->value=info_data;
    new_node->next=map->head;
    map->head=new_node;
}
bool *has_key(struct MapCustom *map, int key)
{
    struct EntryMap *current = map->head;
    if ((map->head) == NULL)
    {
        return false;
    }
    while (current->key != key)
    {
        if (current->next == NULL)
        {
            return false;
        }
        else
        {
            current = current->next;
        }
    }
    return true;
}
void delete_key(struct MapCustom *map, int key){
    struct EntryMap *current=map->head;
    struct EntryMap *previous=NULL;
    if (current==NULL){
        return;
    }
    while (current->key!=key)
    {
        if (current->next==NULL){
            return;
        }
        else{
            previous=current;
            current=current->next;
        }
    }
    if(current==map->head){
        map->head=map->head->next;
    }else{
        previous->next=current->next;
    }
    

    
}