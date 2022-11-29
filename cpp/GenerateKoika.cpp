//
// Created by Jamey Hicks on 11/7/19.
//

#include "GenerateKoika.h"
#include <map>
#include <string>

GenerateKoika::GenerateKoika() {
    instances.clear();
}

void GenerateKoika::open(const std::string &filename) {
    this->filename = filename;
    cerr << "Opening Koika file " << filename << endl;
    out.open(filename);
    out << "Require Import Lia." << endl;
    out << "Require Import String." << endl;
    out << "Require Import NArith." << endl;
    out << "Require Import LangFjfj2." << endl;
    out << "Require Import Indexification." << endl;
    out << "Require Import FjfjParsing." << endl;
    out << "Local Set Universe Polymorphism." << endl;
    out << "Unset Universe Minimization ToSet."<< endl;
    out << endl;
}

void GenerateKoika::close() {
    cerr << "Closing Koika file " << filename << endl;
    out.close();
}

void GenerateKoika::generateStmts(std::vector<shared_ptr<struct Stmt>> stmts) {

    // out << "(begin " << endl; 
    for (int i = 0; i < stmts.size(); i++) {
        shared_ptr<Stmt> stmt = stmts[i];
        generateKoika(stmt);
    }
    // out << ")" << endl; 
}

void GenerateKoika::generateKoika(shared_ptr<Stmt> stmt, int depth) {
    if (shared_ptr<ActionBindingStmt> actionBindingStmt = stmt->actionBindingStmt()) {
        generateKoika(actionBindingStmt, depth);
    } else if (shared_ptr<VarBindingStmt> varBindingStmt = stmt->varBindingStmt()) {
        generateKoika(varBindingStmt, depth);
    } else if (shared_ptr<BlockStmt> blockStmt = stmt->blockStmt()) {
        generateKoika(blockStmt, depth);
    } else if (shared_ptr<ExprStmt> exprStmt = stmt->exprStmt()) {
        generateKoika(exprStmt, depth);
    } else if (shared_ptr<IfStmt> ifStmt = stmt->ifStmt()) {
        generateKoika(ifStmt, depth);
    } else if (shared_ptr<ImportStmt> importStmt = stmt->importStmt()) {
        generateKoika(importStmt, depth);
    } else if (shared_ptr<InterfaceDeclStmt> interfaceDeclStmt = stmt->interfaceDeclStmt()) {
        generateKoika(interfaceDeclStmt, depth);
    } else if (shared_ptr<MethodDeclStmt> methodDeclStmt = stmt->methodDeclStmt()) {
        generateKoika(methodDeclStmt, depth);
    } else if (shared_ptr<MethodDefStmt> methodDefStmt = stmt->methodDefStmt()) {
        generateKoika(methodDefStmt, depth);
    } else if (shared_ptr<ModuleDefStmt> moduleDefStmt = stmt->moduleDefStmt()) {
        generateKoika(moduleDefStmt, depth);
    } else if (shared_ptr<RegWriteStmt> regWriteStmt = stmt->regWriteStmt()) {
        generateKoika(regWriteStmt, depth);
    } else if (shared_ptr<RegReadStmt> regReadStmt = stmt->regReadStmt()) {
            generateKoika(regReadStmt, depth);
    } else if (shared_ptr<ModuleInstStmt> ModuleInstStmt= stmt->moduleInstStmt()) {
            generateKoika(ModuleInstStmt, depth);
    // } else if (shared_ptr<RegisterStmt> RegisterStmt= stmt->registerStmt()) {
    //         generateKoika(RegisterStmt, depth);
    } else if (shared_ptr<ReturnStmt> returnStmt = stmt->returnStmt()) {
        generateKoika(returnStmt, depth);
    } else if (shared_ptr<CallStmt> callStmt = stmt->callStmt()) {
        generateKoika(callStmt, depth);
    } else if (shared_ptr<RuleDefStmt> ruleDefStmt = stmt->ruleDefStmt()) {
        generateKoika(ruleDefStmt, depth);
    } else if (shared_ptr<TypedefStructStmt> typedefStructStmt = stmt->typedefStructStmt()) {
        generateKoika(typedefStructStmt, depth);
    } else if (shared_ptr<TypedefSynonymStmt> typedefSynonymStmt = stmt->typedefSynonymStmt()) {
        generateKoika(typedefSynonymStmt, depth);
    }
}

void GenerateKoika::generateKoika(const shared_ptr<Expr> &expr, int depth, int precedence) {
    if (shared_ptr<CallExpr> callExpr = expr->callExpr()) {
        generateKoika(callExpr, depth, 0);
    } else if (shared_ptr<VarExpr> varExpr = expr->varExpr())  {
        generateKoika(varExpr, depth, 0);
    }
    else if (shared_ptr<CondExpr> condExpr = expr->condExpr())  {
        generateKoika(condExpr, depth, 0);
    }
    else if (shared_ptr<MethodExpr> methodExpr = expr->methodExpr())  {
        generateKoika(methodExpr, depth, 0);
    }
    else if (shared_ptr<OperatorExpr> operatorExpr = expr->operatorExpr())  {
        generateKoika(operatorExpr, depth, 0);
    }
    else if (shared_ptr<IntConst> intconst= expr->intConst())  {
        out << expr->intConst()->value;
    }
}


void GenerateKoika::generateKoika(const shared_ptr<CondExpr> &expr, int depth) {
    out << "(if ";
    generateKoika(expr->cond, depth + 1);
    out << "" << endl;
    generateKoika(expr->thenExpr, depth + 1);
    out << "" << endl;
    generateKoika(expr->elseExpr, depth + 1);
    out << ")";
}

void GenerateKoika::generateKoika(const shared_ptr<MethodExpr> &expr, int depth, int precedence) {
    out << expr->methodName << " ";
    generateKoika(expr->object, depth);
//  logstream << "method expr ";
//     expr->object->bsvtype->to_string();
//     logstream << " " << expr->methodName << " at " << expr->sourcePos.toString() << endl;

}

void  GenerateKoika::generateKoika(const shared_ptr<ModuleInstStmt> &stmt, int depth) {
    // out << "MODINST" ;
    // Currently don't do anything
   // out << "reg " << registerStmt->regName  <<endl ;
}


void GenerateKoika::generateKoika(const shared_ptr<BSVType> &bsvtype, int depth) {
    // out << "bsvtype" << "{ ";
    // bsvtype->prettyPrint(out, depth);
    // out << " }" << endl;
}

void GenerateKoika::generateKoika(const shared_ptr<ActionBindingStmt> &stmt, int depth) {
    out << "ACTION " << stmt->name;
    generateKoika(stmt->rhs, depth);
}

void GenerateKoika::generateKoika(const shared_ptr<BlockStmt> &blockstmt, int depth) {
    indent(out, depth);
    cerr << "Block statement" << endl;
    out << "(begin " << endl; 
    indent(out, depth+1);
    for (int i = 0; i < blockstmt->stmts.size(); i++) {
        shared_ptr<Stmt> stmt = blockstmt->stmts[i];
        generateKoika(stmt);
    }
    indent(out, depth);
    out << ")" << endl; 
}

void GenerateKoika::generateKoika(const shared_ptr<ExprStmt> &stmt, int depth) {
    indent(out, depth+1);
    generateKoika(stmt->expr, depth+1);
    out << endl;
}

void GenerateKoika::generateKoika(const shared_ptr<IfStmt> &stmt, int depth) {
    out << "(if ";
    generateKoika(stmt->condition, depth + 1);
    out << "" << endl;
    generateKoika(stmt->thenStmt, depth + 1);
    out << "" << endl;
    generateKoika(stmt->elseStmt, depth + 1);
    out << ")" << endl;
}

void GenerateKoika::generateKoika(const shared_ptr<ImportStmt> &stmt, int depth) {
    out << "Require Import " << stmt->name << "." << endl;
}


void GenerateKoika::generateKoika(const shared_ptr<MethodDefStmt> &methoddef, int depth) {
    out << endl;

    indent(out, depth);
    out << "Definition " << methoddef->name << " := " << endl;
    // Does not do the same thing in the case of value method and action method 
    if (methoddef->returnType->name == "Action") {
        indent(out, depth+1); out << "(action_method (";

        for (int i = 0; i < methoddef->params.size(); i++){
            out << methoddef->params[i] << " ";
        }
        out << ")" << endl;
        indent(out, depth+1);
        out << "(begin " << endl;

        indent(out, depth+2);
        if (methoddef->guard){
            out << "(if ";
            generateKoika(methoddef->guard , depth+2);
            out << endl;
            indent(out, depth+2);
            out << " pass abort)"<<endl;
            indent(out, depth+2);
        }
        cerr << methoddef->name << endl;
        cerr << methoddef->stmts.size() << endl;
        for (int i = 0; i < methoddef->stmts.size(); i++) {

            shared_ptr<Stmt> stmt = methoddef->stmts[i];
            generateKoika(stmt, depth+1);
            out << endl;
            indent(out, depth+2);
        }

        indent(out, depth+1);
        out << "))." << endl;
    } else {
        indent(out, depth+1); out << "(value_method (";
        for (int i = 0; i < methoddef->params.size(); i++){
            out << methoddef->params[i] << " ";
        }
        out << ")" << endl;

        indent(out, depth+1);
        out << "(begin " << endl;
        indent(out, depth+2);
        if (methoddef->guard){
            out << "(if ";
            generateKoika(methoddef->guard , depth+2);
            out << endl;
            indent(out, depth+2);
            out << " pass abort)"<<endl;
        }
        indent(out, depth+2);
        for (int i = 0; i < methoddef->stmts.size(); i++) {
            shared_ptr<Stmt> stmt = methoddef->stmts[i];
            generateKoika(stmt, depth+1);
            out << endl;
            indent(out, depth+2);
        }
        indent(out, depth+1);
        out << "))." << endl;

    }

 

}

void GenerateKoika::generateKoika(const shared_ptr<ModuleDefStmt> &moduledef, int depth) {
    instances.clear();
    rules.clear();
    value_methods.clear();
    action_methods.clear();

    indent(out, depth);
    // out << "Module " << moduledef->name << "." << endl;

    // Gather the name of the action methods, value methods and everything
    for (int i = 0; i < moduledef->stmts.size(); i++) {
        shared_ptr<Stmt> stmt = moduledef->stmts[i];
        switch (stmt->stmtType) {
            case RegisterStmtType: {
                shared_ptr<RegisterStmt> registerStmt= stmt->registerStmt();
                instances[registerStmt->regName] = "reg";
                break;
            }
            case ModuleInstStmtType: {
                shared_ptr<ModuleInstStmt> moduleInstStmt = stmt->moduleInstStmt();
                shared_ptr<CallExpr> constructor = moduleInstStmt->rhs->callExpr();
                shared_ptr<VarExpr> var = constructor->function->varExpr();
                instances[moduleInstStmt->name] = var->name;
                break;
            }
            case MethodDefStmtType: {
                shared_ptr<MethodDefStmt> methodDefStmt = stmt->methodDefStmt();
                if (methodDefStmt->returnType->name == "Action") {
                    action_methods[methodDefStmt->name] = "UNKNOWN";
                } else {
                    value_methods[methodDefStmt->name] = "UNKNOWN";
                }
                break;
            }
            case RuleDefStmtType: {
                shared_ptr<RuleDefStmt> ruleDefStmt = stmt->ruleDefStmt();
                rules[ruleDefStmt->name] = "UNKNOWN";
                break;
            }
        }
    }


    indent(out,depth);
    out << "Local Instance submodules : instances := #|" <<endl; 
    for(std::map<string,string>::iterator instance = instances.begin(); instance!= instances.end(); ++instance) {
        indent(out,depth+1);
        out << instance->second << " " << instance->first;
        std::map<string,string>::iterator nxt_instance = next(instance, 1);
        if (nxt_instance != instances.end()) { out << ";"; }
        out << endl;
    }
    indent(out,depth);
    out << "|#." <<endl;

    for (int i = 0; i < moduledef->stmts.size(); i++) {
        shared_ptr<Stmt> stmt = moduledef->stmts[i];
        generateKoika(stmt, depth);
    }

    out << endl;
    indent(out,depth);
    out << "Global Instance " <<  moduledef->name;
    out << ": module _ :=" << endl;
    indent(out,depth);
    out <<"module#(rules [";
    for(std::map<string,string>::iterator instance = rules.begin(); instance!= rules.end(); ++instance) {
        out << instance->first;
        std::map<string,string>::iterator nxt_instance = next(instance, 1);
        if (nxt_instance != rules.end()) { out << ";"; }
    }
    out << "]" <<endl;
    indent(out,depth+1);
    out << "vmet [";
    for(std::map<string,string>::iterator instance = value_methods.begin(); instance!= value_methods.end(); ++instance) {
        out << instance->first;
        std::map<string,string>::iterator nxt_instance = next(instance, 1);
        if (nxt_instance != value_methods.end()) { out << ";"; }
    }
    out << "]" <<endl;
    indent(out,depth+1);
    out << "amet [";
    for(std::map<string,string>::iterator instance = action_methods.begin(); instance!= action_methods.end(); ++instance) {
        out << instance->first;
        std::map<string,string>::iterator nxt_instance = next(instance, 1);
        if (nxt_instance != action_methods.end()) { out << "; "; }
    }
    out << "])." << endl;
  
    // indent(out,depth);
    // out << "End " << moduledef->name << "." << endl;
}

// void GenerateKoika::generateKoika(const shared_ptr<RegisterStmt> &registerStmt, int depth, vector<shared_ptr<Stmt>> &actionStmts) {
//     // indent(out, depth);
//     // out << "reg " << registerStmt->regName << ";" <<endl ;
// }
void GenerateKoika::generateKoika(const shared_ptr<RegWriteStmt> &reg, int depth) {
    // indent(out, depth);
    out << "{" << reg->regName <<" write ";
    generateKoika(reg->rhs, depth+1);
    out << "})";
}

void GenerateKoika::generateKoika(const shared_ptr<RegReadStmt> &regread, int depth) {
    // indent(out, depth);
    out << "(set "<< regread->var << " {" << regread->regName <<" read})";

}

void GenerateKoika::generateKoika(const shared_ptr<ReturnStmt> &stmt, int depth) {
    generateKoika(stmt->value, depth);
}

void GenerateKoika::generateKoika(const shared_ptr<RuleDefStmt> &ruleDef, int depth) {
    out << endl;
    indent(out, depth);
    out << "Definition " << ruleDef->name << " := " << endl;
    // Does not do the same thing in the case of value method and action method 
    indent(out, depth + 1); out << "(rule" << endl;

    indent(out, depth + 1);
    out << "(begin " << endl;
    indent(out, depth + 2);
    if (ruleDef->guard) {
        out << "(if ";
        generateKoika(ruleDef->guard, depth+2);
        out << endl;
        indent(out, depth + 2);
        out << " pass abort)"<<endl;
        indent(out, depth + 2);
    }
    for (int i = 0; i < ruleDef->stmts.size(); i++) {
        shared_ptr<Stmt> stmt = ruleDef->stmts[i];
        generateKoika(stmt, depth+1);
        out << endl;
        indent(out, depth + 2);
    }
    indent(out, depth + 1);
    out << "))." << endl;
}



void GenerateKoika::generateKoika(const shared_ptr<VarBindingStmt> &stmt, int depth) {
    // if (stmt->bsvtype) {
    //     out << " : ";
    //     generateKami(stmt->bsvtype, depth);
    // }
    if (stmt->rhs) {
        indent(out, depth);
        out << "(set " << stmt->name ;
        out << " ";
        generateKoika(stmt->rhs, depth + 1);
        out << ")" << endl;
    }
}
void GenerateKoika::generateKoika(const shared_ptr<VarExpr> &expr, int depth, int precedence) {
        out << expr->name;
}

void GenerateKoika::generateKoika(const shared_ptr<CallExpr> &expr, int depth, int precedence) {
    out << "{";
    generateKoika(expr->function, depth, 0);
    out << " (# ";
    for (int i = 0; i < expr->args.size(); i++) {
        shared_ptr<Expr> e= expr->args[i];
        generateKoika(e, depth, 0);
    }

    out << " )}" <<endl;
}

void GenerateKoika::generateKoika(const shared_ptr<CallStmt> &callStmt, int depth) {
//    indent(out, depth);
//     out << "Call " << " : ";
//     generateKami(callStmt->interfaceType, depth + 1);
//     out << " <- ";
//     generateKami(callStmt->rhs, depth + 1);
//     out << " ;" << endl;
// }
    // out << "{" << callStmt->name << " ";
    generateKoika(callStmt->rhs, depth);
    // out << "}";
}

void GenerateKoika::generateKoika(const shared_ptr<IntConst> &expr, int depth, int precedence) {
    out << expr->value;
}

void GenerateKoika::generateKoika(const shared_ptr<OperatorExpr> &expr, int depth, int precedence) {
    if (!expr->rhs) {
        out << "(" << expr->op << " ";
        generateKoika(expr->lhs, depth, precedence);
        out << ")";
    }
    if (expr->rhs) {
        out << "(" << expr->op << " ";
        generateKoika(expr->lhs, depth, precedence);
        out << " ";
        generateKoika(expr->rhs, depth, precedence);
        out << ")";
    }

}

void GenerateKoika::generateKoika(const shared_ptr<TypedefStructStmt> &stmt, int depth) {

}

void GenerateKoika::generateKoika(const shared_ptr<TypedefSynonymStmt> &stmt, int depth) {

}
void GenerateKoika::generateKoika(const shared_ptr<FieldExpr> &expr, int depth, int precedence) {

}


void GenerateKoika::generateKoika(const shared_ptr<ArraySubExpr> &expr, int depth, int precedence) {

}

void GenerateKoika::generateKoika(const shared_ptr<EnumUnionStructExpr> &expr, int depth, int precedence) {

}

void GenerateKoika::generateKoika(const shared_ptr<InterfaceDeclStmt> &stmt, int depth) {
	// No declaration of interface
}

void GenerateKoika::generateKoika(const shared_ptr<MethodDeclStmt> &stmt, int depth) {
	// No declaration of method 
}