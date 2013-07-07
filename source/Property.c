/*
 By: Justin Meiners
 
 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "Property.h"
#include <string.h>

static void *(*_Property_Malloc)(size_t sz) = malloc;
static void (*_Property_Free)(void *ptr) = free;

struct Property
{
    PropertyType_t type;
    char* key;
    
    double doubleValue;
    int intValue;
    char* stringValue;
    
    struct Property* next;
    struct Property* previous;
    struct Property* child;
};

struct PropertySet
{
    PropertyRef root;
};

void Property_InstallAllocators(void *(*mallocFunc)(size_t sz), void (*freeFunc)(void *ptr))
{
    _Property_Malloc = mallocFunc;
    _Property_Free = freeFunc;
}

PropertyRef Property_Create(PropertyType_t type)
{
    PropertyRef property = _Property_Malloc(sizeof(struct Property));
    
    if (property)
    {
        property->type = type;
        property->key = 0;
        property->stringValue = 0;
        property->next = 0;
        property->previous = 0;
        property->child = 0;
    }
    
    return property;
}

void Property_Destroy(PropertyRef property)
{
    if (property)
    {
        if (property->stringValue)
        {
            _Property_Free(property->stringValue);
            property->stringValue = 0;
        }
        
        _Property_Free(property);
    }
}

char* Property_Name(PropertyRef property)
{
    return property->key;
}

PropertyType_t Property_Type(PropertyRef property)
{
    return property->type;
}

void Property_SetDoubleValue(PropertyRef property, double doubleValue)
{
    property->doubleValue = doubleValue;
    property->intValue = (int)doubleValue;
}

double Property_DoubleValue(PropertyRef property)
{
    return property->doubleValue;
}

void Property_SetIntValue(PropertyRef property, int intValue)
{
    property->intValue = intValue;
    property->doubleValue = (double)intValue;
}

void Property_SetStringValue(PropertyRef property, const char* stringValue)
{
    if (property->stringValue)
    {
        _Property_Free(property->stringValue);
        property->stringValue = NULL;
    }
    
    if (stringValue)
    {
        property->stringValue = _Property_Malloc(strlen(stringValue) + 1);
        strcpy(property->stringValue, stringValue);
        
        property->doubleValue = atof(property->stringValue);
        property->intValue = atoi(property->stringValue);
    }
}

char* Property_StringValue(PropertyRef property)
{
    return property->stringValue;
}

PropertyRef Property_ItemAt(PropertyRef property, int index)
{
    PropertyRef current = property->child;
    
    while (current && index > 0)
    {
        --index;
        current = current->next;
    }
    
    return current;
}

void Property_AddItem(PropertyRef property, PropertyRef item)
{
    PropertyRef current = property->child;
    
    if (!current)
    {
        property->child = item;
    }
    else
    {
        while (current && current->next)
        {
            current = current->next;
        }
        
        current->next = item;
        item->previous = current;
    }
}

void Property_RemoveItemAt(PropertyRef property, int index)
{
    PropertyRef current = property->child;
    
    while (current && index > 0)
    {
        current = current->next;
        index--;
    }
    
    if (!current)
    {
        return;
    }
    
    if (current->previous)
    {
        current->previous->next = current->next;
    }
    
    if (current->next)
    {
        current->next->previous = current->previous;
    }
    
    if (current == property->child)
    {
        property->child = current->next;
    }
    
    current->next = NULL;
    current->previous = NULL;
}

int Property_ItemCount(PropertyRef property)
{
    int count = 0;
    
    PropertyRef current = property->child;
    
    while (current)
    {
        ++count;
        current = current->next;
    }
    
    return count;
}

PropertyRef Property_EntryFor(PropertyRef property, const char* key)
{
    PropertyRef current = property->child;
    
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current;
        }
        
        current = current->next;
    }
    
    return current;
}

void Property_AddEntryFor(PropertyRef property, PropertyRef entry, const char* key)
{
    if (entry->key)
    {
        _Property_Free(entry->key);
        entry->key = 0;
    }
    
    if (key)
    {        
        entry->key = _Property_Malloc(strlen(key) + 1);
        strcpy(entry->key, key);
        
        Property_AddItem(property, entry);
    }
}

void Property_RemoveEntryFor(PropertyRef property, const char* key)
{
    PropertyRef current = property->child;
    
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (current->previous)
            {
                current->previous->next = current->next;
            }
            
            if (current->next)
            {
                current->next->previous = current->previous;
            }
            
            if (current == property->child)
            {
                property->child = current->next;
            }
            
            return;
        }

        current = current->next;
    }
}

int Property_EntryCount(PropertyRef property)
{
    return Property_ItemCount(property);
}

PropertySetRef PropertySet_Create(PropertyRef root)
{
    PropertySetRef set = _Property_Malloc(sizeof(struct PropertySet));
    
    if (set)
    {
        set->root = root;
    }
    
    return set;
}

static void _PropertySet_Destroy_r(PropertyRef property)
{
    PropertyRef child = property->child;
    
    while (child)
    {
        _PropertySet_Destroy_r(child);
        child = child->next;
    }
    
    Property_Destroy(property);
}

void PropertySet_Destroy(PropertySetRef set)
{
    if (set)
    {
        if (set->root)
        {
            _PropertySet_Destroy_r(set->root);
        }
        
        _Property_Free(set);
    }
}

PropertyRef PropertySet_Root(PropertySetRef set)
{
    return set->root;
}

