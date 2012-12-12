#include <typeinfo>
#include "expression.h"
#define Nfunc 10
string funcNames[]={"Exp","Sqrt","Erf","Cos","Sin","Tan","Cosh","Sinh","Tanh",
    "Log"};
double (*funcPointers[])(double)={exp,sqrt,erf,cos,sin,tan,cosh,sinh,tanh,log};
/* find {{{ */
int find(const string &s, const char c) {
    int n=s.size()-1;
    int p=0;
    while(n>=0) {
        if(s[n]=='(' || s[n]=='[' || s[n]=='{') {
            p++;
        } else if(s[n]==')' || s[n]==']' || s[n]=='}') {
            p--;
        } else if(p==0 && s[n]==c) {
            return n;
        }
        n--;
    }
    if(p!=0)
        throw incorExpr;
    return -1;
}
/* }}} */
/* parseString {{{ */
Expression *parseString(const string &s) {
    int n=s.size();
    if(n!=0) {
        //Search for binary operator
        char c[]={'+','-','*','/','^'};
        for(int i=0;i<5;i++) {
            int index=find(s,c[i]);
            if(index!=-1) {
                string sl=s.substr(0,index);
                string sr=s.substr(index+1);
                return new BinaryOp(c[i],sl,sr);
            }
        }
        //s=(...) ?
        if(s[0]=='(') {
            string sn=s.substr(1,n-2);
            return parseString(sn);
        }
        //s=Fun[...] ?
        int bra,ket;
        bra=s.find('[');
        ket=s.find(']');
        if(bra!=-1) {
            string sn=s.substr(0,bra);
            string sa=s.substr(bra+1,ket-bra-1);
            return new SingleValFunction(sn,sa);
        }
        //s={{...}} MConstant ?
        if(s[0]=='{') {
            if(s[1]=='{') {
                for(int j=2;s[j]!='}';j++)
                    if(s[j]==',')
                        return new MConstant(s);
                return new KConstant(s);
            }
            return new BConstant(s);
        }
        //s=Constant ?
        int t=(int)s.at(0);
        if(t>47 && t<58)
            return new Constant(s);
        return new Variable(s);
    }
    return new Constant("0");
}
/* }}} */
ostream &operator<<(ostream &os, Expression *exp) {
    if(typeid(*exp)==typeid(Constant)) {
        Constant *tmp=(Constant*)exp;
        os << tmp->value();
    } else if(typeid(*exp)==typeid(Variable)) {
        Variable *tmp=(Variable*)exp;
        os << tmp->name();
    } else if(typeid(*exp)==typeid(BinaryOp)) {
        BinaryOp *op=(BinaryOp*)exp;
        os << '(' << op->left() << op->op() << op->right() << ')';
    } else if(typeid(*exp)==typeid(SingleValFunction)) {
        SingleValFunction *fun=(SingleValFunction*)exp;
        os << funcNames[fun->i()] << '[' << fun->arg() << ']';
    }
    return os;
}
Constant &Constant::operator=(const Constant &other) {
    if(&other!=this) {
        _c=other._c;
    }
    return *this;
}
/* KConstant class implementation {{{ */
KConstant::KConstant(const string &s) {
    int l=s.size();
    int n=0;
    for(int i=1;i<l-1;i++) {
        if(s[i]==',')
            n++;
    }
    n++;
    _k=Ket<double>(n);
    string c=s.substr(1,l-2);
    for(int i=0;i<n;i++) {
        int k=c.find('{');
        int p=c.find('}');
        string v=c.substr(k+1,p-k-1);
        _k[i]=atof(v.c_str());
        c=c.substr(p+1);
    }
}
/* }}} */
/* BConstant class implementation {{{ */
BConstant::BConstant(const string &s) {
    int l=s.size();
    int n=0;
    for(int i=1;i<l-1;i++) {
        if(s[i]==',')
            n++;
    }
    n++;
    _b=Bra<double>(n);
    string c=s.substr(1,l-2);
    for(int i=0;i<n-1;i++) {
        int l=c.find(',');
        string v=c.substr(0,l);
        _b[i]=atof(v.c_str());
        c=c.substr(l+1);
    }
    _b[n-1]=atof(c.c_str());
}
/* }}} */
/* MConstant class implementation {{{ */
MConstant::MConstant(const string &s) {
    int l=s.size();
    int n,m;
    n=m=0;
    for(int i=1;i<l-1;i++) {
        if(s[i]=='{')
            n++;
        else if(s[i]==',')
            m++;
    }
    m-=n-1;
    m=m/n+1;
    _m=Matrix<double>(n,m);
    string c=s.substr(1,l-2);
    //cerr << "c=" << c << endl;
    for(int i=0;i<n;i++) {
        int k=c.find('{');
        int p=c.find('}');
        string r=c.substr(k+1,p-k-1);
        //cerr << "r=" << r << endl;
        for(int j=0;j<m-1;j++) {
            int l=r.find(',');
            string v=r.substr(0,l);
            _m.at(i,j)=atof(v.c_str());
            r=r.substr(l+1);
            //cerr << v << " ";
        }
        _m.at(i,m-1)=atof(r.c_str());
        //cerr << r << endl;
        c=c.substr(p+1);
    }
}
/* }}} */
/* Variable class implementation {{{ */
Expression *Variable::simplify(VarDef &vars) {
    if(vars.find(_var)!=vars.end())
        return vars[_var]->simplify(vars);
    return this;
}
void *Variable::evaluate(VarDef &vars) {
    if(vars.find(_var)==vars.end())
        throw undefVar;
    return vars[_var]->evaluate(vars);
}
bool Variable::find(const char *var) {
    if(_var==var)
        return true;
    return false;
}
/* }}} */
/* BinaryOp class implementation {{{ */
/* BinaryOp {{{ */
BinaryOp::BinaryOp(const char c, const string &sl, const string &sr) {
    _c=c;
    _left=parseString(sl);
    _right=parseString(sr);
}
BinaryOp::BinaryOp(const char c, Expression *l, Expression *r) {
    _c=c;
    _left=l;
    _right=r;
}
/* }}} */
/* print {{{ */
void BinaryOp::print(void) {
    cerr << "(";
    _left->print();
    cerr << _c;
    _right->print();
    cerr << ")";
    return;
}
/* }}} */
/* simplify {{{ */
Expression *BinaryOp::simplify(VarDef &vars) {
    Expression *left=_left->simplify(vars);
    Expression *right=_right->simplify(vars);
    if(typeid(*left)==typeid(Constant)) {
        /* scalar lhs. {{{ */
        double lhs=*((double*)left->evaluate(vars));
        if(typeid(*right)==typeid(Constant)) {
            /* scalar rhs. {{{ */
            double rhs=*((double*)right->evaluate(vars));
            switch(_c) {
                case '+':
                    lhs+=rhs;
                    break;
                case '-':
                    lhs-=rhs;
                    break;
                case '*':
                    lhs*=rhs;
                    break;
                case '/':
                    lhs/=rhs;
                    break;
                case '^':
                    lhs=pow(lhs,rhs);
                    break;
            }
            return new Constant(lhs);
            /* }}} */
        } else if(typeid(*right)==typeid(MConstant)) {
            /* matrix rhs. {{{ */
            if(_c!='*')
                throw incompatibleSizes;
            Matrix<double> rhs=*((Matrix<double>*)right->evaluate(vars));
            rhs*=lhs;
            return new MConstant(rhs);
            /* }}} */
        } else if(typeid(*right)==typeid(BConstant)) {
            /* bra rhs. {{{ */
            if(_c!='*')
                throw incompatibleSizes;
            Bra<double> rhs=*((Bra<double>*)right->evaluate(vars));
            rhs*=lhs;
            return new BConstant(rhs);
            /* }}} */
        } else if(typeid(*right)==typeid(KConstant)) {
            /* ket rhs. {{{ */
            if(_c!='*')
                throw incompatibleSizes;
            Ket<double> rhs=*((Ket<double>*)right->evaluate(vars));
            rhs*=lhs;
            return new KConstant(rhs);
            /* }}} */
        } else if(typeid(*right)==typeid(Variable)) {
            if(_c=='*' && lhs==1)
                return right;
        }
        /* }}} */
    } else if(typeid(*left)==typeid(MConstant)) {
        /* matrix lhs. {{{ */
        Matrix<double> lhs=*((Matrix<double>*)left->evaluate(vars));
        if(typeid(*right)==typeid(Constant)) {
            /* scalar rhs. {{{ */
            double rhs=*((double*)right->evaluate(vars));
            if(_c=='*')
                lhs*=rhs;
            else if(_c=='/')
                lhs/=rhs;
            else
                throw incompatibleSizes;
            return new MConstant(lhs);
            /* }}} */
        } else if(typeid(*right)==typeid(MConstant)) {
            /* matrix rhs. {{{ */
            Matrix<double> rhs=*((Matrix<double>*)right->evaluate(vars));
            switch(_c) {
                case '+':
                    lhs+=rhs;
                    break;
                case '-':
                    lhs-=rhs;
                    break;
                case '*':
                    lhs=lhs*rhs;
                    break;
                default:
                    throw undefVar;
            }
            return new MConstant(lhs);
            /* }}} */
        } else if(typeid(*right)==typeid(BConstant)) {
            /* bra rhs. {{{ */
            throw incompatibleSizes;
            /* }}} */
        } else if(typeid(*right)==typeid(KConstant)) {
            /* ket rhs. {{{ */
            Ket<double> rhs=*((Ket<double>*)right->evaluate(vars));
            if(_c!='*')
                throw incompatibleSizes;
            return new KConstant(lhs*rhs);
            /* }}} */
        }
        /* }}} */
    } else if(typeid(*left)==typeid(BConstant)) {
        /* bra lhs. {{{ */
        Bra<double> lhs=*((Bra<double>*)left->evaluate(vars));
        if(typeid(*right)==typeid(Constant)) {
            /* scalar rhs. {{{ */
            double rhs=*((double*)right->evaluate(vars));
            if(_c=='*')
                lhs*=rhs;
            else if(_c=='/')
                lhs/=rhs;
            else
                throw incompatibleSizes;
            return new BConstant(lhs);
            /* }}} */
        } else if(typeid(*right)==typeid(MConstant)) {
            /* matrix rhs. {{{ */
            Matrix<double> rhs=*((Matrix<double>*)right->evaluate(vars));
            if(_c!='*')
                throw incompatibleSizes;
            return new BConstant(lhs*rhs);
            /* }}} */
        } else if(typeid(*right)==typeid(BConstant)) {
            /* bra rhs. {{{ */
            throw incompatibleSizes;
            /* }}} */
        } else if(typeid(*right)==typeid(KConstant)) {
            /* ket rhs. {{{ */
            Ket<double> rhs=*((Ket<double>*)right->evaluate(vars));
            if(_c!='*')
                throw incompatibleSizes;
            return new Constant(lhs*rhs);
            /* }}} */
        }
        /* }}} */
    } else if(typeid(*left)==typeid(KConstant)) {
        /* ket lhs. {{{ */
        Ket<double> lhs=*((Ket<double>*)left->evaluate(vars));
        if(typeid(*right)==typeid(Constant)) {
            /* scalar rhs. {{{ */
            double rhs=*((double*)right->evaluate(vars));
            if(_c=='*')
                lhs*=rhs;
            else if(_c=='/')
                lhs/=rhs;
            else
                throw incompatibleSizes;
            return new KConstant(lhs);
            /* }}} */
        } else if(typeid(*right)==typeid(MConstant)) {
            /* matrix rhs. {{{ */
            throw incompatibleSizes;
            /* }}} */
        } else if(typeid(*right)==typeid(BConstant)) {
            /* bra rhs. {{{ */
            Bra<double> rhs=*((Bra<double>*)right->evaluate(vars));
            if(_c!='*')
                throw incompatibleSizes;
            return new MConstant(lhs*rhs);
            /* }}} */
        } else if(typeid(*right)==typeid(KConstant)) {
            /* ket rhs. {{{ */
            throw incompatibleSizes;
            /* }}} */
        }
        /* }}} */
    } else if(typeid(*left)==typeid(Variable)) {
        if(typeid(*right)==typeid(Constant)) {
            double rhs=*((double*)right->evaluate(vars));
            if((_c=='/' || _c=='*') && rhs==1)
                return left;
        }
    }
    return new BinaryOp(_c,left,right);
}
/* }}} */
/* evaluate {{{ */
void *BinaryOp::evaluate(VarDef &vars) {
    Expression *tmp=simplify(vars);
    if(typeid(*tmp)!=typeid(Constant))
        throw undefVar;
    return tmp->evaluate(vars);
}
/* }}} */
bool BinaryOp::find(const char *var) {
    return _left->find(var) || _right->find(var);
}
/* }}} */
/* SingleValFunction class implementation {{{ */
SingleValFunction::SingleValFunction(const string &fun, const string &s) {
    _fun=-1;
    for(int i=0;i<Nfunc;i++)
        if(fun.compare(funcNames[i])==0)
            _fun=i;
    if(_fun==-1)
        throw unknownFunction;
    _arg=parseString(s);
}
SingleValFunction::SingleValFunction(const int fun, Expression *arg) {
    _fun=fun;
    _arg=arg;
}
void SingleValFunction::print(void) {
    cerr << funcNames[_fun] << "[";
    _arg->print();
    cerr << "]";
    return;
}
Expression *SingleValFunction::simplify(VarDef &vars) {
    Expression *tmp=_arg->simplify(vars);
    if(typeid(*tmp)==typeid(Constant))
        return new Constant(funcPointers[_fun](
                    *((double*)tmp->evaluate(vars))));
    return new SingleValFunction(_fun,tmp);
}
void *SingleValFunction::evaluate(VarDef &vars) {
    Expression *tmp=_arg->simplify(vars);
    if(typeid(*tmp)!=typeid(Constant))
        throw undefVar;
    return new double(funcPointers[_fun](*((double*)tmp->evaluate(vars))));
}
bool SingleValFunction::find(const char *var) {
    return _arg->find(var);
}
/* }}} */
/* expression.cpp */
