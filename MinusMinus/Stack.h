//
//  Stack.h
//  MinusMinus
//
//  Authors: Miro Markaravanes, Oliver Day, Carlos Vallejo
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl, Miro Markaravanes, Oliver Day, and Carlos Vallejo. All rights reserved.
//

#ifndef Stack_h
#define Stack_h

#include <stdio.h>

const int STACK_SIZE = 100;

template<class T>
class Stack
{
private:
    T values[STACK_SIZE];
    int size;
    
public:
    Stack();
    
    // push value onto stack, return true if successful, else false
    bool push(T);
    
    // pop value from stack, return true if successful, else false
    bool pop();
    
    // pop number of values from stack, return true if successful, else false
    bool pop(int);
    
    // peek value from top of stack. 
    T peek();
    
    // peek value from stack using the index. Return success false if out of range, else true
    T peek(int index, bool& success);
    
    // poke value to stack using the index. Return false if out of range, else true
    bool poke(int index, T value);
    
    // return the number of items on stack
    int getStackSize();
    
    // clear the stack;
    void clear();

    // print the stack;
    void print();
};

#include "Stack.cpp"

#endif /* Stack_h */
