//
//  main.cpp
//  MinusMinus
//
//  Authors: Miro Markaravanes, Oliver Day, Carlos Vallejo
//
//  Created by Tony Biehl on 8/15/16.
//  Copyright © 2016 Tony Biehl, Miro Markaravanes, Oliver Day, and Carlos Vallejo. All rights reserved.
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
