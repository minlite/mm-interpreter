//
//  SymbolTable.hpp
//  MinusMinus
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl. All rights reserved.
//

#ifndef SymbolTable_h
#define SymbolTable_h

#include <iostream>
#include <string>

using std:: string;

const int SYMBOLS_SIZE = 100;

// single entry into the SymbolTable
struct Symbol
{
    string symbol;
    int offset;
};

// Essentially a Bag structure with no order
class SymbolTable {
private:
    Symbol items[SYMBOLS_SIZE];
    int size;
public:
    SymbolTable();
    
    // takes Symbol item to add to symbol table, returns true if successful, else false
    bool add(Symbol item);
    
    // takes Symbol item as a call by reference, uses the symbol to search for the item,
    // if found, updates offset and numeric fields and returns true, else returns false
    bool get(Symbol& item);
    
    // return the size of the SymbolTable
    int getSize();
};

#endif /* SymbolTable_h */
