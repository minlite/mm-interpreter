// Lab #2 (Infix-to-Posfix Converter)
// Oliver Day (#1203353)
// @file StackInterface.h

#ifndef _STACK_INTERFACE
#define _STACK_INTERFACE

template<class ItemType>
class StackInterface
{

public:

   virtual bool isEmpty() const = 0;
   virtual bool push(const ItemType& newEntry) = 0;
   virtual bool pop() = 0;
   virtual ItemType peek() const = 0;
};  // end StackInterface


#endif
