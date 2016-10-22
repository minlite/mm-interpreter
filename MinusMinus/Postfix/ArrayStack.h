// Lab #2 (Infix-to-Posfix Converter)
// Oliver Day (#1203353)
// @file ArrayStack.h

#ifndef _ARRAY_STACK_H
#define _ARRAY_STACK_H

#include "StackInterface.h"

const int MAX_STACK = 50;

template<class ItemType>
class ArrayStack : public StackInterface<ItemType>
{
private:
   ItemType items[MAX_STACK];     // Array of stack items
   int      top;                  // Index to top of stack

public:
   ArrayStack();                  // Default constructor
    
   bool isEmpty() const;
    
   bool push(const ItemType& newEntry);
    
   bool pop();
    
   ItemType peek() const;
};  // end ArrayStack

#include "ArrayStack.cpp"
#endif
