#include "Tree.h"
#include "tokens.h"
#include "generator.h"
#include <iostream>
#include <string>

using namespace std;

string registers[] = { "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

static ostream &operator<<(ostream &ostr, Expression *expr) {
    expr->operand(ostr);
    return ostr;
}

void Expression::operand(ostream &ostr) const {

}

void Number::operand(ostream &ostr) const {
    ostr << "$" << _value;
}

void Identifier::operand(ostream &ostr) const {
    if(_symbol->offset() == 0) {
        ostr << _symbol->name();
    } else {
        ostr << _symbol->offset() << "(%rbp)";
    }
}


void Block::generate() {
    for (auto stmt : _stmts) {
        stmt->generate();
    }
}

void Simple::generate() {
    _expr->generate();
}

void Function::generate() {
    int offsetCounter = 0;
    int symbolSize = _body->declarations()->symbols().size();
    int paramsSize = _id->type().parameters()->size();
    for (int x = 0; x < symbolSize; x++) {
        Symbol *currentSymbol = _body->declarations()->symbols().at(x);
        offsetCounter -= currentSymbol->type().size();
        currentSymbol->setOffset(offsetCounter);
    }
    while(offsetCounter % 16 != 0) {
        offsetCounter--;
    }
    
    cout << _id->name() << ": " << endl;
    cout << "pushq %rbp" << endl;
    cout << "movq %rsp, %rbp" << endl;
    cout << "subq $" << abs(offsetCounter) << ", %rsp" << endl;

    for (int y = 0; y < paramsSize; y++) {
        Symbol *currentSymbol = _body->declarations()->symbols().at(y);
        cout << "movl " << registers[y] << ", " << currentSymbol->offset() << "(%rbp)" << endl;
    }

    _body->generate();

    cout << "movq %rbp, %rsp" << endl;
    cout << "popq %rbp" << endl;
    cout << "ret" << endl;
    
    cout << ".globl " << _id->name() << endl;
}

void Assignment::generate() {
    cout << "movl " << _right << ", " << _left << endl;
}

void Call::generate() {
    for(unsigned int x = 0; x < _args.size(); x++) {
        cout << "movl " << _args.at(x) << ", " <<  registers[x] << endl;
    }
    cout << "movl $0, %eax" << endl;
    cout << "call " << _id->name() << endl;
}


void generateGlobals(Scope *scope) {
    for(auto symbol : scope->symbols()) {
        if(!(symbol->type().isFunction())) {
            cout << ".comm " << symbol->name() << ", " << symbol->type().size() << endl;
        }
    }
}