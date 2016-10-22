//
//  main.cpp
//  MinusMinus
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl. All rights reserved.
//

#include <iostream>
#include "Interpret.h"

using namespace std;

int main() {
    Interpret prog;
    string file;
    cout << "Program name? ";
    getline(cin, file);
    prog.start(file);
    return 0;
}
