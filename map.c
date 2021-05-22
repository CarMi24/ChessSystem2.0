#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"



#define NO_SIZE -1
#define IDENTICAL 0
#define EMPTY 0


typedef struct node{
    MapDataElement data_element;
    MapKeyElement key_element;
    struct node *next;
} *Node;


struct Map_t
{  
    Node list;
    Node iterator;
    int mapSize;
    copyMapDataElements copyData;
    copyMapKeyElements copyKey;
    freeMapDataElements freeData;
    freeMapKeyElements freeKey;
    compareMapKeyElements compareKeys;
};


/**
 * 
 * 
 ******STATIC FUNCTIONS******
 *
 * 
 */


static Node createNode(MapKeyElement key, MapDataElement data, copyMapKeyElements copyKey, copyMapDataElements copyData)
{
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL)
    {
        return new_node;
    }
    new_node->key_element = copyKey(key);
    new_node->data_element = copyData(data);
    if(new_node->key_element == NULL || new_node->data_element == NULL)
    {
        free(new_node);
        return new_node;
    }
    new_node->next = NULL;
    return new_node;
}

static void destroyElements(Node list, freeMapDataElements freeElement, freeMapKeyElements freeKey)
{
    if(list == NULL)
    {
        return;
    }
    destroyElements(list->next,freeElement,freeKey);
    freeElement(list->data_element);
    freeKey(list->key_element);
    free(list);
}

static Node findNodeByKey(Node list, MapKeyElement key, compareMapKeyElements compareKeys)
{
    Node iterator = list;
    Node smaller = NULL;
    while(iterator!=NULL)
    {
        if(compareKeys(key,iterator->key_element) <= IDENTICAL)
        {
            return smaller;
        }
        smaller = iterator;
        iterator = iterator->next;
    }
    return smaller;
}
static MapResult updateExistingKey(Node to_update, MapDataElement data,freeMapDataElements freeData, copyMapDataElements copyData)
{
    freeData(to_update->data_element);
    to_update->data_element = copyData(data);
    if(to_update->data_element == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    return MAP_SUCCESS; 
}

static MapResult insertNewNode(Map map, MapKeyElement key, MapDataElement data, Node smaller)
{
    Node bigger = NULL;
    if(map->list == NULL || smaller == NULL)
    {
        bigger = map->list;
        map->list = createNode(key,data,map->copyKey,map->copyData);
        if(map->list == NULL)
        {
            return MAP_OUT_OF_MEMORY;
        }
        map->list->next = bigger;
    }
    else
    {
        Node bigger = smaller->next;
        smaller->next = createNode(key,data,map->copyKey,map->copyData);
        if(smaller->next == NULL)
        {
            smaller->next = bigger;
            return MAP_OUT_OF_MEMORY;
        }
        smaller->next->next = bigger;
    }
    map->mapSize++;
    return MAP_SUCCESS;
}

static void removeNextNode(Map map,Node remove_next)
{
    Node new_next = NULL;
    if(remove_next != NULL)
    {
        new_next = map->list->next;
        map->freeData(map->list->data_element);
        map->freeKey(map->list->key_element);
        free(map->list);
        map->list = new_next;
    }

    new_next = remove_next->next->next;
    map->freeData(remove_next->next->data_element);
    map->freeKey(remove_next->next->key_element);
    free(remove_next->next);
    remove_next->next = new_next;
}

/**
 * 
 * 
 ******MAP.C Application*****
 *
 * 
 */

Map mapCreate(copyMapDataElements copyDataElements, copyMapKeyElements copyKeyElements,
                freeMapDataElements freeDataElements, freeMapKeyElements freeKeyElements,
                compareMapKeyElements compareKeyElements)
{
    if(copyDataElements == NULL || copyKeyElements == NULL || freeDataElements == NULL
         || freeKeyElements == NULL || compareKeyElements == NULL)
    {
        return NULL;
    }

    Map new_map = malloc(sizeof(*new_map));
    if(new_map == NULL)
    {
        return NULL;
    }
   
    new_map->list = NULL;
    new_map->iterator = NULL;
    new_map->mapSize = 0;
    new_map->copyData = copyDataElements;
    new_map->copyKey = copyKeyElements;
    new_map->freeData = freeDataElements;
    new_map->freeKey = freeKeyElements;
    new_map->compareKeys =  compareKeyElements;

    return new_map;
}

void mapDestroy(Map map)
{
    if(map == NULL)
    {
        return;
    }

    destroyElements(map->list,map->freeData,map->freeKey);
    free(map);
}

Map mapCopy(Map map)
{
    if(map == NULL)
    {
        return NULL;
    }
    
    Map clone = mapCreate(map->copyData,map->copyKey,map->freeData,map->freeKey,map->compareKeys);
    if(clone == NULL)
    {
        return NULL;
    }

    Node temp = map->list;
    Node temp_clone = clone->list;
    while(temp!=NULL)
    {
        if(insertNewNode(clone,temp->key_element,temp->key_element,temp_clone)!=MAP_SUCCESS)
        {
            mapDestroy(clone);
            return NULL;
        }
        temp_clone=temp_clone->next;
        temp = temp->next;
    }

    return clone;
}

int mapGetSize(Map map)
{
    if(map==NULL)
    {
        return NO_SIZE;
    }

    return map->mapSize;
}

bool mapContains(Map map, MapKeyElement element)
{
    if(map == NULL || element == NULL || map->mapSize == EMPTY)
    {
        return false;
    }
    
    Node key_node = findNodeByKey(map->list, element ,map->compareKeys);
    
    if(key_node == NULL)
    {
        return map->compareKeys(map->list->key_element,element) == IDENTICAL;
    }
    if(key_node->next == NULL)
    {
        return false;
    }
    return  map->compareKeys(key_node->next->key_element,element) == IDENTICAL;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if(map == NULL || keyElement == NULL || dataElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }

    Node to_update = findNodeByKey(map->list,keyElement,map->compareKeys);

    if(to_update!=NULL && to_update->next != NULL && map->compareKeys(to_update->next->key_element,keyElement) == IDENTICAL)
    {
        return updateExistingKey(to_update,dataElement,map->freeData,map->copyData);
    }
    
    return insertNewNode(map,keyElement,dataElement,to_update);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(mapContains(map,keyElement))
    {
        Node to_get = findNodeByKey(map->list,keyElement,map->compareKeys);
        return to_get == NULL ? map->list->data_element : to_get->next->data_element;
    }
    return NULL;
    
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    
    if(!mapContains(map,keyElement))
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Node to_remove = findNodeByKey(map->list,keyElement,map->compareKeys);
    removeNextNode(map,to_remove);
    
    return MAP_SUCCESS;
}

MapKeyElement mapGetFirst(Map map)
{
    if(map == NULL || map->list == NULL)
    {
        return NULL;
    }
    map->iterator = map->list;
    return map->copyKey(map->iterator->key_element);
}

MapKeyElement mapGetNext(Map map)
{
    if(map == NULL || map->iterator->next == NULL)
    {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return map->copyKey(map->iterator->key_element);
}