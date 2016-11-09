//
//  Interpret.cpp
//  MinusMinus
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl. All rights reserved.
//

#include "Interpret.h"
#include <fstream>

using namespace std;

///** specialChar
// Check if special character delimiter of '(', ')', '=', '>', '<', or ':'
bool Interpret::specialChar(char ch){
    return ch == '(' || ch == ')' || ch == ',' ||              // call or def
    ch == '=' || ch == '>' || ch == '<' || ch == ':' || // logic or assignment
    ch == '-' || ch == '+' || ch == '*' || ch == '*';   // math
}

//** validID
// check for valid identifier
bool Interpret::validID(string id) {
    bool valid = true;
    char ch;
    for (int x = 0; x < id.length(); x++) {
        ch = id[x];
        if (!isalnum(ch))
            valid = false;
    }
    return valid;
}


bool Interpret::validVar(string token, SymbolTable local) {
    Symbol temp;
    temp.symbol = token; // look up variable

    return local.get(temp); // valid local variable

}

//** compare
// take two values, use the SymbolTable to look up locals, and return true or false
bool Interpret::compare(string &line, SymbolTable& local){
    bool result = false;
    string operand1 = nextToken(line, false);
    string operation = nextToken(line, false);
    string operand2 = nextToken(line, false);
    if (operand1 == "" || operation == "" || operand2 == "")
        errorMsg("invalid operand for boolean");
    else {
        int value1 = valueToken(operand1, local);
        int value2 = valueToken(operand2, local);
        switch (operation[0])
        {
            case '>' :
                result = value1 > value2;
                break;
            case '=':
                result = value1 = value2;
                break;
            case '<':
                result = value1 < value2;
                break;
            default:
                errorMsg("Invalid comparision operation of " + operation);
        }
    }
    return result;
}

//** valueToken
// either get an integer literal or look up a variable in the SymbolTable then return the value
int Interpret::valueToken(string token, SymbolTable& local) {
    int result = 0;
    int x = 0;
    Symbol temp;
    bool valid;
    if (token != "") {
        if (isdigit(token[x]) || token[x] == '-') { // numeric value
            do
            {
                result *= 10;
                result += (token[x] - '0');
                if (!isdigit(token[x])) {
                    errorMsg("Invalid number of '" + token +"'");
                    x = token.length(); // exit loop
                }
                else
                    x++;
            }while (x < token.length());
        }
        else { // must be local variable
            temp.symbol = token; // look up variable
            if (local.get(temp)) { // valid local variable
                result = stk.peek(temp.offset, valid);
            }
            else
                errorMsg("Invalid variable '" + token + "'");
        }
    }
    return result;
}

//** execute
// execute starting at lineNum. Will use stk to allocate local variables and update lineNum in execution.
//   Will return true if ended with no errors and deallocate parms and locals from stk.
//   Space for return value and the parms were allocated BEFORE execute was called
//   Note: use recursion when executing another function (save the lineNum in a local variable
//   and restore when other function is done)
bool Interpret::execute(int numParmsVars, SymbolTable& table) {
    int parmOffset = 0; // parms actually start at 1 for stack frame, return value is at 0
    bool success = true;
    int countParms = 0;
    int countIfs = 0;
    int countWhiles = 0;
    Stack<int> whileLineNumStack;
    int retValIndex = stk.getStackSize() - numParmsVars - 1;
    string temp = lines[lineNum];
    string token;
    Symbol v;
    token = nextToken(temp, false); // get from header the function name
    
    cout<<"Return val index at "<<retValIndex<<endl;
    if (nextToken(temp, false) == "(")
    {
        token = nextToken(temp, true);
        while (token != "" && token != ")")
        {

            if(token.length() > 1 && token[token.length()-1] == ')') {
                token = token.substr(0, token.length()-1);
                temp = ')' + temp;
            }

            if (validID(token)) {
                countParms++;
                v.symbol = token;
                v.offset = retValIndex + countParms;
                cout<<"Added "<< v.symbol << " at "<<v.offset<<endl;
                table.add(v);
            }
            else if (token != "," && token != "")
                errorMsg("Parameter not a valid ID");
            token = nextToken(temp, false);
        }
    }
    else
        errorMsg("Bad function header");
    if (token != ")")
        errorMsg("Missing ending ')'");
    if (countParms != numParmsVars)
        errorMsg("Number of arguments do not match number of parameters");
    lineNum++;
    while (errorCount == 0 && lineNum < size)
    { // interpret the line
        temp = lines[lineNum]; // get the line
        token = nextToken(temp, false);
        if (token == "declare")
        {
            string var = nextToken(temp, false);
            while (var != "") {
                Symbol symbol;
                symbol.symbol = var;
                symbol.offset = stk.getStackSize();
                table.add(symbol);
                
                stk.push(0);
                
                var = nextToken(temp, false);
            }
        }
        else if (token == "if")
        {
            if (!compare(temp, table))
            {
                int countTheIf = 1;
                lineNum++;
                temp = lines[lineNum];
                temp = nextToken(temp, false);
                while (lineNum < size && temp != "endif" && countTheIf > 0)
                {
                    temp = lines[lineNum];
                    lineNum++;
                    temp = nextToken(temp, false);
                    if (temp == "if")
                        countTheIf++;
                    else if (temp == "endif")
                        countTheIf--;
                }
                lineNum--;
                if (lineNum == size || countTheIf > 0)
                    errorMsg("No matching endif");
            }
            else countIfs++;
        }
        else if (token == "endif")
        {
            if (countIfs == 0)
                errorMsg("endif without matching if");
            else
                countIfs--;
        }
        else if (token == "while")
        {
            if (!compare(temp, table))
            {
                int countTheWhile = 1;
                lineNum++;
                temp = lines[lineNum];
                temp = nextToken(temp, false);
                while (lineNum < size && temp != "endwhile" && countTheWhile > 0)
                {
                    temp = lines[lineNum];
                    lineNum++;
                    temp = nextToken(temp, false);
                    if (temp == "while")
                        countTheWhile++;
                    else if (temp == "endwhile")
                        countTheWhile--;
                }
                lineNum--;
                if (lineNum == size || countTheWhile > 0)
                    errorMsg("No matching endwhile");
            }
            else {
                countWhiles++;
                whileLineNumStack.push(lineNum);
            }
        }
        else if (token == "endwhile")
        {
            if (countWhiles == 0)
                errorMsg("endwhile without matching while");
            else {
                countWhiles--;
                // Jump back to the start of current while to re-evaluate
                lineNum = whileLineNumStack.peek() - 1;
                whileLineNumStack.pop();
            }

        }
        else if (token == "input")
        {
            token = nextToken(temp, false);
            if (token == "")
                errorMsg("Empty Input statement");
            else if (token[0] == '\"')
            {
                printString(token);
                token = nextToken(temp, false);
            }
            v.symbol = token;
            if (table.get(v)) {
                int num;
                cin >> num;
                stk.poke(v.offset, num);
            }
            else
                errorMsg("Input variable " + v.symbol + " not found");
        }
        else if (token == "print" || token == "println")
        {
            bool newline = (token == "println");
            token = nextToken(temp, false);
            while (token != "")
            {
                if (token[0] == '\"')
                {
                    printString(token);
                } else {
                    Symbol f;
                    f.symbol = token;
                    if(table.get(f)) {
                        bool success = true;
                        cout<<stk.peek(f.offset, success);
                        if(!success)
                            errorMsg("Undefined variable.");
                    } else {
                        errorMsg("Undefined variable.");
                    }
                } 
                
                token = nextToken(temp, false);
            } 
            
            if (newline) {
                cout << endl;
            }
        }
        else if (token == "return")
        {
            string tempOrig = temp;
            string token = nextToken(temp, false);
            int val = 0;
            if(isalpha(token[0])) {
                // Check if defined
                cout<<"Treating "<<token<<" as a variable"<<endl;
                Symbol f;
                f.symbol = token;
                if(table.get(f)) {
                    bool success = true;
                    val = stk.peek(f.offset, success);
                    if(!success) {
                        errorMsg("Undefined return variable.");
                    }
                } else {
                    errorMsg("Undefined return variable.");
                }
            } else {
                // Expression
                cout<<"Treating "<<tempOrig<<" as an expression"<<endl;
                bool success = true;
                val = equation(tempOrig, table, success);
                cout<<"Result is " <<val<<endl;
                if (!success)
                    errorMsg("Bad equation");
            }
            while (stk.getStackSize() != retValIndex + 1)
                stk.pop();
            stk.poke(retValIndex, val);
            return true;
        }
        else if (token == "rem")
        {
        }
        else if (validID(token) && token != "")
        { // assignment
            v.symbol = token;
            if (table.get(v))
            { // valid identifier
                int dest = v.offset;
                token = nextToken(temp, false);
                if (token == ":=")
                { // assign
                    string temp2 = temp;
                    token = nextToken(temp2, false);
                    string token2 = nextToken(temp2, false);
                    if (validID(token) && token2 == "(")
                    { // function call
                        Symbol f;
                        f.symbol = nextToken(temp, false);
                        if (validID(f.symbol) && functions.get(f))
                        { // valid function
                            int oldLineNum = lineNum;
                            SymbolTable local; // calling routine must create a symboltable to pass
                            stk.push(0); // and create a return value
                            token = nextToken(temp, false); // get the "("
                            if(temp[temp.length() - 1] == ')') {
                                temp = temp.substr(0, temp.length() - 1);
                                token = nextToken(temp, true);
                                int count = 0;
                                int result;
                                bool success = true;
                                string lastToken = "";
                                while (token != "")
                                {
                                    if (token != ",") {
                                        cout<<"Token is: "<<token<<endl;
                                        cout<<"Stack size is: " <<stk.getStackSize()<<endl;
                                        count++;
                                        result = equation(token, table, success);
                                        stk.push(result);
                                    } else if (token == token2) {
                                        errorMsg("Two commas next to each other");
                                    }
                                    else {
                                        //errorMsg("Parameter not a valid ID");
                                    }

                                    lastToken = token;
                                    token = nextToken(temp, true);
                                }
                                
                                lineNum = f.offset;
                                execute(count, local);
                                result = stk.peek(stk.getStackSize() - 1, success);
                                cout<<"Result is " <<result<<endl;
                                stk.poke(dest, result);
                                
                                lineNum = oldLineNum;
                            }
                        }
                        else
                            errorMsg("Function " + f.symbol + " not found");
                    }
                    else
                    { // expression
                        bool success = true;
                        cout<<"The expression is: "<<temp<<endl;
                        int result = equation(temp, table, success);
                        cout<<"Result is " <<result<<endl;
                        stk.poke(dest, result);
                        if (!success)
                            errorMsg("Bad equation");
                    }
                }
                else
                    errorMsg("Missing assignment operator");
            }
            else
                errorMsg("Assignment variable not found");
        }
        else if(token == "") {
            // Do nothing
            // Blank line
        }
        else
            errorMsg("Unrecognized command");
        lineNum++;
    }
    
    stk.pop(parmOffset); // remove the parameters
    return success;
}

//** nextToken
// Chop off the first part of the string that:
//    1) ends with white space or () special character
//    2) ends with "
//    3) or if comma flag on, skip whitespace and ends with a comma
string Interpret::nextToken(string& line, bool comma){
    string token = "";
    char ch;
    int x = 0;
    while (x < line.length() && (line[x] == ' ' || line[x] == '\t')) // go past leading whitespace
        x++;
    bool notDelimit = true;
    bool stringConst = false;
    while (x < line.length() && notDelimit) {
        ch = line[x];
        if (ch == '\"') {  // string constant
            if (stringConst) {
                stringConst = false;
                notDelimit = false;
            }
            else
                stringConst = true;
        }
        if (stringConst) { // if stringConst, tack on without looking for delimiters
            token += ch;
            x++; // move to next character
        }
        else if (comma) {
            if (ch == ' ' || ch == '\t') // skip white space
                x++;
            else if (ch == ',') { // tack on without white space
                if (token == "") { // delimiter by itself?
                    token += ch; // yes, return the delimiter by itself
                    x++;
                }
                notDelimit = false; // exit
            }
            else {
                token += ch;
                x++;
            }
        }
        else if (ch == ' ' || ch == '\t' || specialChar(ch)) { // check for delimiter
            if (specialChar(ch)) { // special token
                if (token == "") { // actual delimiter or beginning white space?
                    token += ch;
                    x++;
                    if (x < line.length() && token == ":" && line[x] == '=') { // assignment op of :=
                        token += '=';
                        x++;
                    }
                }
            }
            else { // move past space or tab
                x++;
            }
            notDelimit = false;
        }
        else { // add to token
            token += ch;
            x++; // move to next character
        }
    }
    if (x < line.length())
        line = line.substr(x, line.length());
    else
        line = "";
    //cout << "Token " << token << endl;
    return token;
}

//** Interpret
Interpret::Interpret(){
    size = errorCount = lineNum = 0;
}

//** start
// start the program by loading the xxx.mm (xxx for program, mm is minus, minus program),
//   find the functions, put them in the SymbolTable functions (function name and line number where it starts),
//   and, if the first pass is OK, start excuting the code by calling execute and passing it a firstlocal SymbolTable
void Interpret::start(string fileName){
    ifstream inFile(fileName);
    if (inFile.fail()) { // could not open
        cout << "Could not open program\n";
        return;
    }
    string line, function, temp;
    size = 0;
    SymbolTable firstLocal; // first local table to be passed
    // read in program
    while (getline(inFile, line)) {
        lines[size] = line;
        function = nextToken(line, false);
        temp = nextToken(line, false); // is it a function?
        if (temp == "(") // a function, place in general functions symbol table
        {
            while (temp != ")" && line != "")
                temp = nextToken(line, false);
            if (temp != ")")
                errorMsg("Bad function of " + function);
            else {
                Symbol newFunction;
                newFunction.symbol = function;
                newFunction.offset = size;
                functions.add(newFunction);
                cout << "Added function " << function << " offset " << newFunction.offset << endl;
            }
        }
        size++; // bump size for next line
    }
    // set up the call to the first function (assume no parms)
    stk.push(0); // put space for return value
    if (errorCount == 0 && execute(0, firstLocal))
        cout << "\nProgram stop\n";
    else
        cout << endl << errorCount << " errors\n";
    
}

//** errorMsg
// print lineNum and error and count it via errorCount. Return false for convenience
bool Interpret::errorMsg(string msg) {
    cout << "Error at line " << lineNum+1 << ": " << msg << endl;
    errorCount++;
    return false;
}

//** printString
// print the string constant (remove the quotes)
void Interpret::printString(string s) {
    int len = s.length();
    if (len < 2 || s[0] != '\"' || s[len-1] != '\"')
        errorMsg("Invalid string constant");
    else
        cout << s.substr(1, len-2);
}
//** equation
// convert infix expression to postfix but store on a stack (thus making it into prefix)
//   then call the calculate passing that expression stack and local SymbolTable
//   and return the calculate's value
//   Note: this function will use precedence, nextToken, and a temporary operator stack
//         that follows a infix to postfix algorithm
int Interpret::equation(string exp, SymbolTable& local, bool& success){
    Stack<string> postFix;
    Stack<string> operatorStack;

    string token = nextToken(exp, false);
    //for (int i=0; i<exp.length(); i++)

    while(token != "")
    {
        if(token == "" || token == " ") {
            continue;
        } else if(token == "(") {
            operatorStack.push(token);
        } else if (isOperator(token)) {
            while (operatorStack.getStackSize() != 0 && operatorStack.peek() != "(" &&
                   precedence(token) <= precedence(operatorStack.peek()))
            {
                postFix.push(operatorStack.peek());
                operatorStack.pop();
            }
            operatorStack.push(token);   // save the operator
        } else if(token == ")") {
            while (operatorStack.getStackSize() != 0 && operatorStack.peek() != "(")
            {
                postFix.push(operatorStack.peek());
                operatorStack.pop();
            }
            operatorStack.pop() ;    // remove open parenthesis
            
        } else if(isalpha(token[0])) {
            // Variable.
            if(validVar(token, local))  {
                // Token
                postFix.push(token);
            }
        }
        else if(!isOperator(token)) {
            postFix.push(token);
        }

        token = nextToken(exp, false);
    }
    
    // append to postfixExp the operators remaining in the stack
    while (operatorStack.getStackSize() != 0)
    {
        postFix.push(operatorStack.peek());
        operatorStack.pop();
    }


    return calculate(postFix, local, success);
}

//** precedence
// return precedence of two operators (* and / have precedence of 1, all others precedence of 0)
int Interpret::precedence(string s){
    int pred = 0;
    if (s == "*" || s == "/")
        pred = 1;
    return pred;
}

//** isOperator
// check if operator (true if operator, else false)
bool Interpret::isOperator(string s){
    if (s == "+" || s == "-" || s == "*" || s == "/")
        return true;
    return false;
}

//** convertOperand
// either get an integer literal or look up a variable in the SymbolTable then return the value
int Interpret::convertOperand(string s, SymbolTable& local, bool& success){
    int result = 0;
    string::size_type sz;   // alias of size_t
    if (isdigit(s[0]) || s[0] == '-') {
        result = stoi (s,&sz);
    }
    else {
        Symbol parm;
        parm.symbol = s;
        local.get(parm);
        result = stk.peek(parm.offset, success);
    }
    return result;
}

//** calculate
// calculate value from the postFix stack by treating it like prefix (easy to do with a stack)
//   use the local SymbolTable to look up variables
int Interpret::calculate(Stack<string>& postFix, SymbolTable& local, bool& success) {
    int result = 0, result2 = 0;
    string s, operand1, operand2;
    if (postFix.getStackSize() == 0) { // error
        errorMsg("Blank equation");
        return 0;
    }
    s = postFix.peek(); // get operator
    postFix.pop();
    if (!isOperator(s)) {
        if (postFix.getStackSize() > 1) // single operand?
            success = false; // no, bad equation
        else // yes, get single operand value
            result = convertOperand(s, local, success);
    }
    else{ // regular equation
        if (postFix.getStackSize() == 0) { // no operand?
            errorMsg("Missing operand");
            return 0;
        }
        operand2 = postFix.peek(); // get operand 2
        if (isOperator(operand2)) //
            result2 = calculate(postFix, local, success);
        else {
            postFix.pop(); // remove operand
            result2 = convertOperand(operand2, local, success);
        }
        if (postFix.getStackSize() == 0) { // no operand?
            errorMsg("Missing second operand");
            return 0;
        }
        operand1 = postFix.peek(); // get operand 1
        if (isOperator(operand1)) //
            result = calculate(postFix, local, success);
        else {
            postFix.pop(); // remove operand
            result = convertOperand(operand1, local, success);
        }
        //cout << "operator " << s << " operand1 " << operand1 << " operand2 " << operand2 << endl;
        //cout << "Values " << result << " " << result2 << endl;
        switch (s[0]) // do operation
        {
            case '+' : result += result2;
                break;
            case '-' : result -= result2;
                break;
            case '*' : result *= result2;
                break;
            case '/' : result /= result2;
                break;
        }
    }
    return result;
}

