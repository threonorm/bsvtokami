//
// Created by Jamey Hicks on 10/28/19.
//

#include "GenerateKami.h"


GenerateKami::GenerateKami() {

}

void GenerateKami::open(const string &filename) {
    this->filename = filename;
    cerr << "Opening Kami file " << filename << endl;
    out.open(filename);
}

void GenerateKami::close() {
    cerr << "Closing Kami file " << filename << endl;
    out.close();
}

void GenerateKami::generateStmts(std::vector<shared_ptr<struct Stmt>> stmts) {
    for (int i = 0; i < stmts.size(); i++) {
        shared_ptr<Stmt> stmt = stmts[i];
        generateKami(stmt);
        out << endl;
    }
}

void GenerateKami::generateKami(shared_ptr<Stmt> stmt, int depth) {
    if (shared_ptr<ActionBindingStmt> actionBindingStmt = stmt->actionBindingStmt()) {
        generateKami(actionBindingStmt, depth);
    } else if (shared_ptr<VarBindingStmt> varBindingStmt = stmt->varBindingStmt()) {
        generateKami(varBindingStmt, depth);
    } else if (shared_ptr<BlockStmt> blockStmt = stmt->blockStmt()) {
        generateKami(blockStmt, depth);
    } else if (shared_ptr<CallStmt> callStmt = stmt->callStmt()) {
        generateKami(callStmt, depth);
    } else if (shared_ptr<ExprStmt> exprStmt = stmt->exprStmt()) {
        generateKami(exprStmt, depth);
    } else if (shared_ptr<FunctionDefStmt> functionDefStmt = stmt->functionDefStmt()) {
        generateKami(functionDefStmt, depth);
    } else if (shared_ptr<IfStmt> ifStmt = stmt->ifStmt()) {
        generateKami(ifStmt, depth);
    } else if (shared_ptr<ImportStmt> importStmt = stmt->importStmt()) {
        generateKami(importStmt, depth);
    } else if (shared_ptr<InterfaceDeclStmt> interfaceDeclStmt = stmt->interfaceDeclStmt()) {
        generateKami(interfaceDeclStmt, depth);
    } else if (shared_ptr<InterfaceDefStmt> interfaceDefStmt = stmt->interfaceDefStmt()) {
        out << "(* interfaceDefStmt: " << endl;
        interfaceDefStmt->prettyPrint(out, 1);
        out << "*)" << endl;
    } else if (shared_ptr<MethodDeclStmt> methodDeclStmt = stmt->methodDeclStmt()) {
        generateKami(methodDeclStmt, depth);
    } else if (shared_ptr<MethodDefStmt> methodDefStmt = stmt->methodDefStmt()) {
        generateKami(methodDefStmt, depth);
    } else if (shared_ptr<ModuleDefStmt> moduleDefStmt = stmt->moduleDefStmt()) {
        generateKami(moduleDefStmt, depth);
    } else if (shared_ptr<PatternMatchStmt> patternMatchStmt = stmt->patternMatchStmt()) {
        out << "(* PatternMatchStmt" << endl;
        patternMatchStmt->prettyPrint(out, 1);
        out << "*)" << endl;
    } else if (shared_ptr<RegisterStmt> registerStmt = stmt->registerStmt()) {
        generateKami(registerStmt, depth);
    } else if (shared_ptr<RegReadStmt> regReadStmt = stmt->regReadStmt()) {
        generateKami(regReadStmt, depth);
    } else if (shared_ptr<RegWriteStmt> regWriteStmt = stmt->regWriteStmt()) {
        generateKami(regWriteStmt, depth);
    } else if (shared_ptr<ReturnStmt> returnStmt = stmt->returnStmt()) {
        generateKami(returnStmt, depth);
    } else if (shared_ptr<RuleDefStmt> ruleDefStmt = stmt->ruleDefStmt()) {
        generateKami(ruleDefStmt, depth);
    } else if (shared_ptr<TypedefStructStmt> typedefStructStmt = stmt->typedefStructStmt()) {
        generateKami(typedefStructStmt, depth);
    } else if (shared_ptr<TypedefSynonymStmt> typedefSynonymStmt = stmt->typedefSynonymStmt()) {
        generateKami(typedefSynonymStmt, depth);
    } else if (shared_ptr<VarAssignStmt> varAssignStmt = stmt->varAssignStmt()) {
        out << "(* VarAssignStmt" << endl;
        varAssignStmt->prettyPrint(out, 1);
        out << "*)" << endl;
    } else {
        assert(0);
    }
}


void GenerateKami::generateKami(const shared_ptr<Expr> &expr, int depth, int precedence) {
    if (shared_ptr<OperatorExpr> opexpr = expr->operatorExpr()) {
        generateKami(opexpr, depth, precedence);
    } else if (shared_ptr<CallExpr> callExpr = expr->callExpr()) {
        generateKami(callExpr, depth, precedence);
    } else if (shared_ptr<FieldExpr> fieldexpr = expr->fieldExpr()) {
        generateKami(fieldexpr, depth, precedence);
    } else {
        out << "Expr " << expr->exprType << " { ";
        expr->prettyPrint(out, depth);
        out << " }";
    }
}

void GenerateKami::generateKami(const shared_ptr<BSVType> &bsvtype, int depth) {
    out << bsvtype->name;
    if (bsvtype->params.size()) {
        out << "(";
        for (int i = 0; i < bsvtype->params.size(); i++) {
            if (i > 0)
                out << ", ";
            generateKami(bsvtype->params[i], depth);
        }
        out << ")";
    }
}

void GenerateKami::generateKami(const shared_ptr<ActionBindingStmt> &actionbinding, int depth) {
    shared_ptr<BSVType> bsvtype = actionbinding->bsvtype;
    if (bsvtype && bsvtype->name == "Reg") {
        indent(out, depth);
        out << "Register \"" << actionbinding->name << "\" : ";
        generateKami(bsvtype, depth+1);
        out << " <- Default";
        //FIXME: check for initializer
        //generateKami(actionbinding->rhs, depth + 1);
    } else {
        indent(out, depth);
        out << "Call ";
        //FIXME: Call
        generateKami(actionbinding->rhs, depth+1);
    }

}

void GenerateKami::generateKami(const shared_ptr<BlockStmt> &blockstmt, int depth) {
    int num_stmts = blockstmt->stmts.size();
    for (int i = 0; i < num_stmts; i++) {
        shared_ptr<Stmt> stmt = blockstmt->stmts[i];
        generateKami(stmt, depth + 1);
        if (i < num_stmts - 1) {
            out << ";" << endl;
        }
    }
}

void GenerateKami::generateKami(const shared_ptr<CallStmt> &callStmt, int depth) {
    indent(out, depth);
    out << "Call " << callStmt->name << " : ";
    generateKami(callStmt->interfaceType, depth + 1);
    out << " <- ";
    generateKami(callStmt->rhs, depth + 1);
    out << " ;" << endl;
}

void GenerateKami::generateKami(const shared_ptr<ExprStmt> &stmt, int depth) {
    indent(out, depth);
    out << "(* expr " << stmt->expr->exprType << " *) ";
    generateKami(stmt->expr, depth + 1);
}


void GenerateKami::generateKami(const shared_ptr<FunctionDefStmt> &functiondef, int depth) {
    indent(out, depth);
    out << "Function (instancePrefix--\"" << functiondef->name << "\") (* args *) (* result type *) := " << endl;
    indent(out, depth); out << "(" << endl;
    int num_stmts = functiondef->stmts.size();
    for (int i = 0; i < num_stmts; i++) {
        shared_ptr<Stmt> stmt = functiondef->stmts[i];
        generateKami(stmt, depth + 1);
        if (i < num_stmts - 1) {
            out << ";";
        }
        out << endl;
    }
    indent(out, depth); out << ")" << endl;
}

void GenerateKami::generateKami(const shared_ptr<IfStmt> &stmt, int depth) {
    indent(out, depth);
    out << "If (";
    generateKami(stmt->condition, depth + 1);
    out << ") then (" << endl;
    generateKami(stmt->thenStmt, depth + 1);
    out << endl;
    indent(out, depth);
    out << ") else (" << endl;
    if (stmt->elseStmt)
        generateKami(stmt->elseStmt, depth + 1);
    else
        out << "Retv";
    out << endl;
    indent(out, depth);
    out << ") as v; Ret v" << endl;
}

void GenerateKami::generateKami(const shared_ptr<ImportStmt> &stmt, int depth) {
    out << "(* import " << stmt->name << " *)" << endl;
}

void GenerateKami::generateKami(const shared_ptr<InterfaceDeclStmt> &stmt, int depth) {

}

void GenerateKami::generateKami(const shared_ptr<MethodDeclStmt> &stmt, int depth) {

}

void GenerateKami::generateKami(const shared_ptr<MethodDefStmt> &methoddef, int depth) {
    indent(out, depth);
    out << "Method (instancePrefix--\"" << methoddef->name << "\") (* args *) (* result type *) := " << endl;
    indent(out, depth); out << "(" << endl;
    int num_stmts = methoddef->stmts.size();
    for (int i = 0; i < num_stmts; i++) {
        shared_ptr<Stmt> stmt = methoddef->stmts[i];
        generateKami(stmt, depth + 1);
        if (i < num_stmts - 1) {
            out << ";";
        }
        out << endl;
    }
    indent(out, depth); out << ")" << endl;
}

void GenerateKami::generateKami(const shared_ptr<ModuleDefStmt> &moduledef, int depth) {
    indent(out, depth);
    out << "Module module'" << moduledef->name << "." << endl;
    indent(out, depth + 1);
    out << "(BKMODULE {" << endl;

    for (int i = 0; i < moduledef->stmts.size(); i++) {
        if (i != 0) {
            indent(out, depth + 1);
            out << "with " << endl;
        }
        shared_ptr<Stmt> stmt = moduledef->stmts[i];
        generateKami(stmt, depth + 1);
        out << endl;
    }
    indent(out, depth + 1);
    out << "})." << endl;
    out << "End module'" << moduledef->name << "." << endl;
}

void GenerateKami::generateKami(const shared_ptr<RegisterStmt> &registerStmt, int depth) {
    indent(out, depth);
    out << "Register \"" << registerStmt->regName << "\" : ";
    //FIXME: placeholder for type
    generateKami(registerStmt->elementType, depth);
    out << " <- ";
    out << "Default";
}
void GenerateKami::generateKami(const shared_ptr<RegReadStmt> &regread, int depth) {
    indent(out, depth);
    out << "Read " << regread->var << " : ";
    //FIXME: placeholder for type
    out << "<regtype>";
    out << " <- ";
    out << "\"" << regread->regName << "\"";
}

void GenerateKami::generateKami(const shared_ptr<RegWriteStmt> &regwrite, int depth) {
    indent(out, depth);
    out << "Write \"" << regwrite->regName << " : ";
    //FIXME: placeholder for type
    out << "<regtype>";
    out << " <- ";
    generateKami(regwrite->rhs, depth+1);
}

void GenerateKami::generateKami(const shared_ptr<ReturnStmt> &stmt, int depth) {
    indent(out, depth);
    out << "Ret ";
    generateKami(stmt->value, depth+1);
}

void GenerateKami::generateKami(const shared_ptr<RuleDefStmt> &ruledef, int depth) {
    indent(out, depth);
    out << "Rule (instancePrefix--\"" << ruledef->name << "\") := " << endl;
    indent(out, depth); out << "(" << endl;
    int num_stmts = ruledef->stmts.size();
    for (int i = 0; i < num_stmts; i++) {
        shared_ptr<Stmt> stmt = ruledef->stmts[i];
        generateKami(stmt, depth + 1);
        if (i < num_stmts - 1) {
            out << ";";
        }
        out << endl;
    }
    indent(out, depth); out << ")" << endl;
}

void GenerateKami::generateKami(const shared_ptr<TypedefStructStmt> &stmt, int depth) {

}

void GenerateKami::generateKami(const shared_ptr<TypedefSynonymStmt> &stmt, int depth) {

}

void GenerateKami::generateKami(const shared_ptr<VarBindingStmt> &stmt, int depth) {
    indent(out, depth);
    out << "LET " << stmt->name;
    if (stmt->bsvtype) {
        out << " : ";
        generateKami(stmt->bsvtype);
    }
    if (stmt->rhs) {
        out << " <- ";
        generateKami(stmt->rhs, depth + 1);
    }
}

void GenerateKami::generateKami(const shared_ptr<FieldExpr> &expr, int depth, int precedence) {
    generateKami(expr->object, depth, precedence);
    out << " ! ";
    out << "(* placeholdeer for type *)"; //FIXME struct type
    out << " @. \"" << expr->fieldName << "\"";
}

void GenerateKami::generateKami(const shared_ptr<VarExpr> &expr, int depth, int precedence) {

}

void GenerateKami::generateKami(const shared_ptr<CallExpr> &expr, int depth, int precedence) {
    expr->function->prettyPrint(out, depth);
    out << "( ";
    for (int i = 0; i < expr->args.size(); i++) {
        if (i > 0)
            out << ", ";
        expr->args[i]->prettyPrint(out, depth + 1);
    }
    out << " ) ;" << endl;
}

void GenerateKami::generateKami(const shared_ptr<IntConst> &expr, int depth, int precedence) {

}

void GenerateKami::generateKami(const shared_ptr<OperatorExpr> &expr, int depth, int precedence) {
    if (!expr->rhs)
        out << expr->op << " ";
    generateKami(expr->lhs, depth, precedence);
    if (expr->rhs) {
        out << " " << expr->op << " ";
        generateKami(expr->rhs, depth, precedence);
    }
}

void GenerateKami::generateKami(const shared_ptr<ArraySubExpr> &expr, int depth, int precedence) {

}

void GenerateKami::generateKami(const shared_ptr<EnumUnionStructExpr> &expr, int depth, int precedence) {

}


