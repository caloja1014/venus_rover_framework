#ifndef MAP_CUSTOM
#define MAP_CUSTOM
#include "opprocesor.h"

struct EntryMap
{
    int key;
    struct OpProcesor *value;
    struct EntryMap *next;
};

struct MapCustom
{
    struct EntryMap *head;
    struct EntryMap *current;
    int size;
};
struct MapCustom *create_map();
struct OpProcesor *get_value_of_map(struct MapCustom *map,int key);
void put_value(struct MapCustom *map, int key, struct OpProcesor *info_data);
bool *has_key(struct MapCustom *map, int key);
void delete_key(struct MapCustom *map, int key);
#endif /* MAP_CUSTOM */