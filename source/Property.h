/*
 By: Justin Meiners
 
 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#define PROPERTY_VERSION_1_0 0
    
#define PROPERTY_VERSION PROPERTY_VERSION_1_0

    
/* 
 v1.0:
 - Initial release
 
 
 */
    
/* 
 Future:
 - XML Loading
 - JSON Loading
 
 */
    
typedef enum
{
    kPropertyTypeDouble = 0,
    kPropertyTypeInt,
    kPropertyTypeString,
    kPropertyTypeArray,
    kPropertyTypeDictionary
} PropertyType_t;
    
/* for custom memory allocators */
extern void Property_InstallAllocators(void *(*mallocFunc)(size_t sz), void (*freeFunc)(void *ptr));

typedef struct Property* PropertyRef;
typedef struct PropertySet* PropertySetRef;

/* creation */
extern PropertyRef Property_Create(PropertyType_t type);
extern void Property_Destroy(PropertyRef property);

extern char* Property_Name(PropertyRef property);
extern PropertyType_t Property_Type(PropertyRef property);

/* double */
extern void Property_SetDoubleValue(PropertyRef property, double doubleValue);
extern double Property_DoubleValue(PropertyRef property);

/* int */
extern void Property_SetIntValue(PropertyRef property, int intValue);
extern int Property_IntValue(PropertyRef property);

/* string */
extern void Property_SetStringValue(PropertyRef property, const char* stringValue);
extern char* Property_StringValue(PropertyRef property);

/* array */
extern PropertyRef Property_ItemAt(PropertyRef property, int index); /* O(n) */
extern void Property_AddItem(PropertyRef property, PropertyRef item); /* O(n) */
extern void Property_RemoveItemAt(PropertyRef property, int index); /* O(n) */
extern int Property_ItemCount(PropertyRef property); /* O(n) */

/* dictionary */
extern PropertyRef Property_EntryFor(PropertyRef property, const char* key); /* O(n) */
extern void Property_AddEntryFor(PropertyRef property, PropertyRef entry, const char* key); /* O(n) */
extern void Property_RemoveEntryFor(PropertyRef property, const char* key); /* O(n) */
extern int Property_EntryCount(PropertyRef property); /* O(n) */

    
/* property set - for easily managing a tree of properties */
extern PropertySetRef PropertySet_Create(PropertyRef root);
    
/* recursively destroies root, and all of it's children */
extern void PropertySet_Destroy(PropertySetRef set); /* O(n) */

extern PropertyRef PropertySet_Root(PropertySetRef set);
    
#ifdef __cplusplus
}
#endif

#endif
