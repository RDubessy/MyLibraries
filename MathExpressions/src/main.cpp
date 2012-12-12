/* This file is a part of MathExpression. {{{
 * Copyright (C) 2012 Romain Dubessy
 *
 * MathExpression is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 *
 * MathExpression is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MathExpression.  If not, see <http://www.gnu.org/licenses/>.
 *
 * }}} */
#include <iostream>
#include <expression.h>
using namespace std;
int main() {
    string s="X+Exp[Y*Z]";
    Expression *exp=parseString(s);
    cerr << exp << endl;
    if(exp->find("X"))
        cerr << "X found" << endl;
    if(exp->find("Y"))
        cerr << "Y found" << endl;
    if(exp->find("Z"))
        cerr << "Z found" << endl;
    if(!(exp->find("R")))
        cerr << "R not found" << endl;
    return 0;
}
/* main.cpp */
