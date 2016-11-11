//
//  Interpret.hpp
//  MinusMinus
//
//  Authors: Miro Markaravanes, Oliver Day, Carlos Vallejo
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl, Miro Markaravanes, Oliver Day, and Carlos Vallejo. All rights reserved.
//

#ifndef Interpret_h
#define Interpret_h

#include "Stack.h"
#include "SymbolTable.h"

const int MAX_LINES = 100;

class Interpret
{
private:
    string lines[MAX_LINES];
    SymbolTable functions;
    Stack<int> stk;
    int size;
    int errorCount;
    int lineNum;
    
    // execute starting at lineNum. Will use stk to allocate local variables and update lineNum in execution.
    //   Will return true if ended with no errors and deallocate parms and locals from stk. 
    //   Space for return value and the parms were allocated BEFORE execute was called
	//   Note: use recursion when executing another function (save the lineNum in a local variable
	//   and restore when other function is done)
    bool execute(int, SymbolTable&);
    
    // Chop off the first part of the string that:
    //    1) ends with white space or () special character
    //    2) or if comma flag on, ends with a () or comma
    string nextToken(string&, bool comma);
    
    // print lineNum and error and count it via errorCount. Return false for convenience
    bool errorMsg(string);
    
    // Check if special character delimiter of '(', ')', '=', '>', '<', or ':'
    bool specialChar(char);
    
    // check for valid identifier
    bool validID(string);

    bool validVar(string token, SymbolTable local);
    
    // take parse compare, use the SymbolTable to look up locals, and return true or false
    bool compare(string &, SymbolTable&);
    
    // either get an integer literal or look up a variable in the SymbolTable then return the value
    int valueToken(string, SymbolTable&);
    
    // print the string constant (remove the quotes)
    void printString(string);
    
    // convert infix expression to postfix but store on a stack (thus making it into prefix)
    //   then call the calculate passing that expression stack and local SymbolTable 
    //   and return the calculate's value
    //   Note: this function will use precedence, nextToken, and a temporary operator stack
    //         that follows a infix to postfix algorithm
    int equation(string, SymbolTable&, bool&);
    
    // return precedence of two operators (* and / have precedence of 1, all others precedence of 0)
    int precedence(string);
    
    // calculate value from the postFix stack by treating it like prefix (easy to do with a stack)
    //   use the local SymbolTable to look up variables
    int calculate(Stack<string>&, SymbolTable&, bool&);
    
    // return true if operator, else false
    bool isOperator(string);
    
    // either get an integer literal or look up a variable in the SymbolTable then return the value
    int convertOperand(string, SymbolTable&, bool&);
    
public:
    Interpret();
    
    // start the program by loading the xxx.mm (xxx for program, mm is minus, minus program),
    //   find the functions, put them in the SymbolTable functions (function name and line number where it starts),
    //   and, if the first pass is OK, start excuting the code by calling execute and passing it a firstlocal SymbolTable
    void start(string);
    int getErrors();
};

#endif /* Interpret_h */
