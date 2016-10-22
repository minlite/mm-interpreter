// Lab #2 (Infix-to-Posfix Converter)
// Oliver Day (#1203353)
// @file main.cpp

#include <iostream>
#include <string>
#include "ArrayStack.h"


using namespace std;

// funciton descriptions given below with implementations

string infixToPostfix(string infixExp);

bool isOperand(char ch);
bool isOperator(char op);
int operatorWeight(char op);
int getPrecedence(char op1, char op2);

bool validExp(string infixExp);
bool isBalancedPar(string infixExp);
bool isDoubleOperand(string infixExp);
bool isDoubleOperator(string infixExp);



int main()
{
    // string to hold the infix expression provided by user
    string infixExp = "";
    
    // prompt alerting user of parameters for infix-to-postfix converter
    cout << "************************** Lab #2 **************************\n\n";
    cout << " __________________________________________________________\n";
    cout << "|                                                          |\n";
    cout << "|           Infix-To-Postfix Converter Parameters          |\n";
    cout << "|__________________________________________________________|\n";
    cout << "|                                                          |\n";
    cout << "| 1. Enter a syntatically correct infix expression         |\n";
    cout << "|                                                          |\n";
    cout << "|     i)   No operands in succession of each other         |\n";
    cout << "|     ii)  No operators in succession of each other        |\n";
    cout << "|     iii) Balanced paranthesis                            |\n";
    cout << "|     iv)  Syntatically incorrect infix expression         |\n";
    cout << "|          in between balanced paranthesis or              |\n";
    cout << "|          entered expression begins with an operator      |\n";
    cout << "|                                                          |\n";
    cout << "|                                                          |\n";
    cout << "| 2. No spaces can be in the entered expression            |\n";
    cout << "|                                                          |\n";
    cout << "| 3. Expression must be less than " << MAX_STACK << " characters in length. |\n";
    cout << "|                                                          |\n";
    cout << "| 4. Available Operators: +, -, *, /                       |\n";
    cout << "|                                                          |\n";
    cout << "|__________________________________________________________|\n\n";
    
    // store user provided infix expression
    cout << "Infix Expression: ";
    getline(cin, infixExp);
    
    
    // check if expression user provided satisfies infix grammar,
    // if expression has proper infix grammar then call infixToPostfix function,
    // if not then alert user that an invalid infix expression was entered.
    if(validExp(infixExp))
    {
        cout << "\nPostfix Expression: " << infixToPostfix(infixExp) << "\n\n";
        
        cout << " __________________________________________\n";
        cout << "|                                          |\n";
        cout << "| A valid infix expression was entered.    |\n";
        cout << "|__________________________________________|\n\n";
    }
    else
    {
        cout << " __________________________________________\n";
        cout << "|                                          |\n";
        cout << "| An invalid infix expression was entered. |\n";
        cout << "|__________________________________________|\n\n";
    }
    
    cout << "************************************************************\n\n";
    
    return 0;
}

// infixToPostifx /////////////////////////////////////////////////////////////////////////
// converts infix expression to a postfix expression

string infixToPostfix(string infixExp)
{
    // initiaiize an empty string for postfix expression
    string postfixExp = "";
    
    // create stack to store operators and '('s during conversion of expressions
    ArrayStack<char> opStack;
    
    
    // loop to scan each each element of the infix expression
    for(int index = 0; index < infixExp.length(); index++)
    {
        // if operator
        if(isOperator(infixExp[index]))
        {
            // add operators from opStack to postfix expression
            // if stack is not empty and
            // if the character is not an open parenthese and
            // if the operator on the opStack has a higher weight then the current operator
            while(!opStack.isEmpty() && opStack.peek() != '('
                                      && getPrecedence(opStack.peek(), infixExp[index]))
            {
                postfixExp += opStack.peek();
                opStack.pop();
            }
            
            // if conditions for the while loop are not met then add current operator from
            // infix expression to opStack
            opStack.push(infixExp[index]);
        }
        
        // if operand, then add operand to postfix expression
        else if(isOperand(infixExp[index]))
        {
            postfixExp += infixExp[index];
        }
        
        // if open brace. then add open parenthese to opStack
        else if(infixExp[index] == '(')
        {
            opStack.push(infixExp[index]);
        }
        
        // if close brace, then add operators from opStack to postfix expressoion
        else if(infixExp[index] == ')')
        {
            while(!opStack.isEmpty() && opStack.peek() != '(')
            {
                postfixExp += opStack.peek();
                opStack.pop();
            }
            
            opStack.pop();
        }
        
    } // end of for loop
    
    // after dealing wiht all parentheses,
    // add operators from opStack to postfix expression until opStack is empty
    while(!opStack.isEmpty())
    {
        postfixExp += opStack.peek();
        opStack.pop();
    }
    
    return postfixExp;
}

// ------------------------------------------------------------------------------- end infixToPostfix


// start isOperand ////////////////////////////////////////////////////////////////
// test whether or not the passed character is an operand

bool isOperand(char ch)
{
    bool result = false;
    
    
    // is character in the range of 'a' to 'z'
    if(ch >= 'a' && ch <= 'z')
        result = true;
    
    // is character in the range of 'A' to 'z'
    else if(ch >= 'A' && ch <= 'Z')
        result = true;
    
    
    return result;
}

// -------------------------------------------------------------------------------- end isOperand

// isOperators ////////////////////////////////////////////////////////////////////
// test whether or not the passed character is one of the approved operators

bool isOperator(char op)
{
    bool result = false;
    
    if(op == '+' || op == '-' || op == '*' || op == '/')
        result = true;
    
    return result;
}

// -------------------------------------------------------------------------------- end isOperator


// operatorWeight //////////////////////////////////////////////////////////////////
// assign weight to operator for later determining operator precedence

int operatorWeight(char op)
{
    int weight = 0;
    
    // assign weight to passed in operator
    switch(op)
    {
        case '+':
        case '-':
            weight = 1;
            break;
            
        case '*':
        case '/':
            weight = 2;
            break;
    }
    
    return weight;
}

// -------------------------------------------------------------------------------- end operatorWeight

// getPrecedence //////////////////////////////////////////////////////////////////////////////
//

int getPrecedence(char op1, char op2)
{
    // store weights of each operator
    int op1Weight = operatorWeight(op1);    // operator at the top of the stack
    int op2Weight = operatorWeight(op2);    // operator reached in infix expression, not on stack
    
    
    if(op1Weight == op2Weight)              // if operators have teh same precedence, return true
        return true;
    
    
    return op1Weight > op2Weight ? true : false; // if the operator at the top of the stack has higher precedence
                                                 // return true; otherwise return false
}

// --------------------------------------------------------------------------------- end getPrecedence


// validExp ////////////////////////////////////////////////////////////////////////
// test if infix expression is valid

bool validExp(string infixExp)
{
    bool validString = true;
    
    // if any of the parameters for a valid infix expression are not met then
    // the string is invalid.
    if(!isBalancedPar(infixExp) || isDoubleOperand(infixExp) || isDoubleOperator(infixExp))
        validString = false;
    else
        validString = true;
    
    
    return validString;
}
// ----------------------------------------------------------------------------- end validExp


// isBalancedPar ///////////////////////////////////////////////////////////////
// determine if parentheses are balanced and that an operator does not follow
// an open "("

bool isBalancedPar(string infixExp)
{
    //
    bool balanced = true;
    bool containsProperInfix = true;
    bool parametersMet = true;
    
    // create stack to hold parentheses
    ArrayStack<char> parStack;
    
    
    int index = 0;
    
    // keep checking for balanced parentheses as longs
    // the end of the string has not been reached
    while(balanced && index < infixExp.length())
    {
        // store characters in
        // succession of each other
        char ch = infixExp[index];
        char nextCh = infixExp[index + 1];
        
        index++;
        
        // if first character of infix expression
        // is an operator then expression is invalid
        if(isOperator(infixExp[0]))
        {
            containsProperInfix = false;
            break;
        }
        
        // if an operator follows an open parenthese
        // then expression is invalid
        else if(ch == '(' && isOperator(nextCh))
        {
            containsProperInfix = false;
            break;
        }
        
        // if open parenthese push it on parStack
        else if(ch == '(')
            parStack.push(ch);
        
        // if close parenthese . . .
        else if(ch == ')')
        {
            if(!parStack.isEmpty())
                parStack.pop();         // . . . pop matching open parenthese
            else                        // . . . if no matching open parenthese
                balanced = false;       //       then the expression is not balanced
        }
        
                                        // note: all characters other than parantheses are ignored
        
    } // end while loop
    
    // if the expression does not have infix grammar the expression is invalid
    if(!containsProperInfix)
    {
        cout << "\nParameter #1(iv) not met!\n\n";
        parametersMet = false;
    }
    
    // if balanced and parStack is empty the expression is valid
    else if(balanced && parStack.isEmpty())
    {
        parametersMet = true;
    }
    
    // expression is not balanced
    else
    {
        cout << "\nParameter #1(iii) not met!\n\n";
        parametersMet = false;
    }
    
    
    return parametersMet;
}

// --------------------------------------------------------------------------------- end isBalancedPar

// isDoubleOperand /////////////////////////////////////////////////////////////////
// determine if two operands are written in succession in user provided infix expression

bool isDoubleOperand(string infixExp)
{
    bool doubleOperand = false;
    
    // scan infix expression for operands in succession of each other
    for(int index = 0; index < infixExp.length() - 1; index++)
    {
        if(isOperand(infixExp[index]) && isOperand(infixExp[index + 1]))
        {
            cout << "\nParameter #1(i) not met!\n\n";
            doubleOperand = true;
            break;
        }
            
    }
    return doubleOperand;
}

// ---------------------------------------------------------------------------------- end doubleOperand

// isDoubleOperator /////////////////////////////////////////////////////////////////
// determine if two operators are written in succession in user provided infix expression

bool isDoubleOperator(string infixExp)
{
    bool doubleOperator = false;
    
    // scan infix expression for operators in succession of each other
    for(int index = 0; index < infixExp.length() - 1; index++)
    {
        if(isOperator(infixExp[index]) && isOperator(infixExp[index + 1]))
        {
            cout << "\nParameter #1(ii) not met!\n\n";
            doubleOperator = true;
            break;
        }
    }
    
    return doubleOperator;
}

// ---------------------------------------------------------------------------------- end doubleOperator


