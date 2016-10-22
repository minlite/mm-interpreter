//
//  Symbols.cpp
//  MinusMinus
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl. All rights reserved.
//

#include "SymbolTable.h"

using namespace std;


SymbolTable::SymbolTable()
{ 
    size = 0;
}

//** add
// takes Symbol item to add to symbol table, returns true if successful, else false
bool SymbolTable::add(Symbol item)
{
    bool notFound = true;
    int index = 0;
    while (index < size && notFound)
    {
        if (items[index].symbol == item.symbol)
            notFound = false;
        index++;
    }
    if (notFound)
    {
        items[size] = item;
        size++;
    }
    return notFound;
}

//** get
// takes Symbol item as a call by reference, uses the symbol to search for the item,
// if found, updates offset and numeric fields and returns true, else returns false
bool SymbolTable::get(Symbol& item)
{
    int foundIndex = -1;
    int index = 0;
    bool found = false;
    while (index < size && foundIndex < 0)
    {
        if (items[index].symbol == item.symbol)
            foundIndex = index;
        index++;
    }
    if (foundIndex >= 0)
    {
        item.offset = items[foundIndex].offset;
        found = true;
    }
    return found;
}

//** getSize
// return the size of the SymbolTable
int SymbolTable::getSize() {
    return size;
}
