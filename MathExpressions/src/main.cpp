#include <iostream>
#include <expression.h>
using namespace std;
int main() {
    string s="-1-2-1";
    Expression *exp=parseString(s);
    cerr << exp << endl;
    VarDef vars;
    Expression *sexp=exp->simplify(vars);
    cerr << sexp << endl;
    return 0;
}
