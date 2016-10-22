// Lab #2 (Infix-to-Posfix Converter)
// Oliver Day (#1203353)
// @file ArrayStack.cpp


#include <cassert>           // for assert
#include "ArrayStack.h"

#ifndef _ARRAY_STACK
#define _ARRAY_STACK

template<class ItemType>
ArrayStack<ItemType>::ArrayStack() : top(-1)
{
}  // end default constructor

template<class ItemType>
bool ArrayStack<ItemType>::isEmpty() const
{
    return top < 0;
}

template<class ItemType>
bool ArrayStack<ItemType>::push(const ItemType& newEntry)
{
   bool result = false;
   if (top < MAX_STACK - 1)    // Does stack have room for newEntry?
   {
       top++;
       items[top] = newEntry;
       result = true;
   } // end if
   return result;
}   // end push

template<class ItemType>
bool ArrayStack<ItemType>::pop()
{
   bool result = false;
   if (!isEmpty())
   {
       top--;
       result = true;
   } // end if

   return result;
} // end pop

template<class ItemType>
ItemType ArrayStack<ItemType>::peek() const
{
   assert(!isEmpty());  // Enforce precondition

   // Stack is not empty; return top
   return items[top];
} // end peek


#endif
// end of implementation file
