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
