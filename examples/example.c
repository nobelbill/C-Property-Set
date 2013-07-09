/*
 By: Justin Meiners
 
 Copyright (c) 2013 Inline Studios
 Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include <stdio.h>
#include "Property.h"


int main(int argc, const char * argv[])
{
    /* create a root dictionary */
    PropertyRef root = Property_Create(kPropertyTypeDictionary);
    
    /* create an array of numbers */
    PropertyRef numbers = Property_Create(kPropertyTypeArray);
    
    int i;
    for (i = 0; i < 10; i ++)
    {
        PropertyRef num = Property_Create(kPropertyTypeInt);
        Property_SetIntValue(num, i);
        Property_AddItem(numbers, num);
    }

    /* add the array to the root */
    Property_AddEntryFor(root, numbers, "numbers");

    /* create an array of strings */
    PropertyRef strings = Property_Create(kPropertyTypeArray);
    
    for (i = 0; i < 5; i ++)
    {
        PropertyRef string = Property_Create(kPropertyTypeString);
        
        const char* value;
        switch (i)
        {
            case 0:
                value = "The";
                break;
            case 1:
                value = "Quick";
                break;
            case 2:
                value = "Brown";
                break;
            case 3:
                value = "Fox";
                break;
            case 4:
                value = "Jumped";
                break;
        }
        
        Property_SetStringValue(string, value);
        Property_AddItem(strings, string);
    }
    
    /* add strings to root */
    Property_AddEntryFor(root, strings, "strings");
    
    
    /* create a property set */
    PropertySetRef set = PropertySet_Create(root);

    
    
    /* destroy property set and attached properties */
    PropertySet_Destroy(set);

    return 0;
}

