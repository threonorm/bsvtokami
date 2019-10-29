
#include <iostream>

using namespace std;

#include "Stmt.h"

void indent(int depth) {
    for (int i = 0; i < depth; i++)
        cout << " ";
}

void indent(ostream &s, int depth) {
    for (int i = 0; i < depth; i++)
        s << " ";
}

Stmt::Stmt(StmtType stmtType)
        : stmtType(stmtType) {
}

shared_ptr<Stmt> Stmt::rename(string prefix, LexicalScope &scope) {
    return shared_ptr<Stmt>();
}

RuleDefStmt::RuleDefStmt(const string &name, const shared_ptr<Expr> &guard, const vector<shared_ptr<Stmt>> &stmts)
        : Stmt(RuleDefStmtType), name(name), guard(guard), stmts(stmts) {
}

void RuleDefStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "rule " << name;
    if (guard) {
        cout << " when (";
        guard->prettyPrint(0);
        cout << ")";
    }
    cout << ";" << endl;
    for (size_t i = 0; i < stmts.size(); i++) {
        shared_ptr<Stmt> stmt(stmts.at(i));
        if (stmt)
            stmt->prettyPrint(depth + 1);
    }
    indent(4 * depth);
    cout << "endrule //" << name << endl;
}

shared_ptr<RuleDefStmt> RuleDefStmt::ruleDefStmt() {
    return static_pointer_cast<RuleDefStmt, Stmt>(shared_from_this());
}

shared_ptr<Stmt> RuleDefStmt::rename(string prefix, LexicalScope &parentScope) {
    LexicalScope scope(parentScope);
    shared_ptr<Expr> renamedGuard;
    if (guard)
        renamedGuard = guard->rename(prefix, parentScope);
    vector<shared_ptr<Stmt>> renamedStmts;
    for (size_t i = 0; i < stmts.size(); i++) {
        renamedStmts.push_back(stmts[i]->rename(prefix, scope));
    }
    return shared_ptr<Stmt>(new RuleDefStmt(prefix + name, renamedGuard, renamedStmts));
}

RegWriteStmt::RegWriteStmt(const string &regName, const shared_ptr<Expr> &rhs)
        : Stmt(RegWriteStmtType), regName(regName), rhs(rhs) {
}

void RegWriteStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << regName << " <= ";
    if (rhs)
        rhs->prettyPrint(depth + 1);
    else
        cout << "no_rhs";
    cout << ";" << endl;
}

shared_ptr<RegWriteStmt> RegWriteStmt::regWriteStmt() {
    return static_pointer_cast<RegWriteStmt, Stmt>(shared_from_this());
}

shared_ptr<Stmt> RegWriteStmt::rename(string prefix, LexicalScope &scope) {
    string renamedRegName = regName;
    string replacement = scope.lookup(regName);
    if (replacement.size()) {
        renamedRegName = replacement;
    }
    shared_ptr<Expr> renamedRHS;
    if (rhs)
        renamedRHS = rhs->rename(prefix, scope);
    return shared_ptr<Stmt>(new RegWriteStmt(renamedRegName, renamedRHS));
}

ActionBindingStmt::ActionBindingStmt(const shared_ptr<BSVType> &bsvtype, const string &name,
                                     const shared_ptr<Expr> &rhs)
        : Stmt(ActionBindingStmtType), bsvtype(bsvtype), name(name), rhs(rhs) {

}

void ActionBindingStmt::prettyPrint(int depth) {
    indent(4 * depth);
    bsvtype->prettyPrint();
    cout << " " << name << " <- ";
    rhs->prettyPrint(depth + 1);
    cout << ";" << endl;

}

shared_ptr<ActionBindingStmt>
ActionBindingStmt::actionBindingStmt() { return static_pointer_cast<ActionBindingStmt, Stmt>(shared_from_this()); }


shared_ptr<Stmt> ActionBindingStmt::rename(string prefix, LexicalScope &scope) {
    string renamedVar = prefix + name;
    shared_ptr<Expr> renamedRHS;
    cerr << "Renaming action binding" << endl;
    rhs->prettyPrint(4);
    if (rhs)
        renamedRHS = rhs->rename(prefix, scope);
    scope.bind(name, renamedVar);
    return shared_ptr<Stmt>(new ActionBindingStmt(bsvtype, renamedVar, renamedRHS));
}

VarBindingStmt::VarBindingStmt(const shared_ptr<BSVType> &bsvtype, const string &name,
                               const shared_ptr<Expr> &rhs)
        : Stmt(VarBindingStmtType), bsvtype(bsvtype), name(name), rhs(rhs) {

}

void VarBindingStmt::prettyPrint(int depth) {
    indent(4 * depth);
    if (bsvtype) bsvtype->prettyPrint();
    cout << " " << name;
    if (rhs) {
        cout << " = ";
        rhs->prettyPrint(depth + 1);
    }
    cout << ";" << endl;
}

shared_ptr<VarBindingStmt> VarBindingStmt::varBindingStmt() {
    return static_pointer_cast<VarBindingStmt, Stmt>(shared_from_this());
}

shared_ptr<Stmt> VarBindingStmt::rename(string prefix, LexicalScope &scope) {
    string renamedVar = prefix + name;
    shared_ptr<Expr> renamedRHS;
    if (rhs)
        renamedRHS = rhs->rename(prefix, scope);
    scope.bind(name, renamedVar);
    return shared_ptr<Stmt>(new VarBindingStmt(bsvtype, renamedVar, renamedRHS));
}

VarAssignStmt::VarAssignStmt(const shared_ptr<LValue> &lhs, const string &op, const shared_ptr<Expr> &rhs)
        : Stmt(VarAssignStmtType), lhs(lhs), op(op), rhs(rhs) {

}

void VarAssignStmt::prettyPrint(int depth)
{
    indent(4 * depth);
    lhs->prettyPrint(depth);
    cout << " " << op << " ";
    rhs->prettyPrint(depth+1);
    cout << endl;
}

shared_ptr<VarAssignStmt> VarAssignStmt::varAssignStmt() {
    return static_pointer_cast<VarAssignStmt, Stmt>(shared_from_this());
}

shared_ptr<struct Stmt> VarAssignStmt::rename(string prefix, LexicalScope &scope) {
    shared_ptr<LValue> newLHS; //FIXME
    return shared_ptr<Stmt>(new VarAssignStmt(newLHS, op, rhs->rename(prefix, scope)));
}


MethodDeclStmt::MethodDeclStmt(const string &name, const shared_ptr<BSVType> &returnType,
                               const std::vector<std::string> &params,
                               const std::vector<std::shared_ptr<BSVType>> &paramTypes)
        : Stmt(MethodDeclStmtType), name(name), returnType(returnType),
          params(params), paramTypes(paramTypes) {}

void MethodDeclStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "method ";
    returnType->prettyPrint(depth + 1);
    cout << " " << name << "(";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0)
            cout << ", ";
        paramTypes[i]->prettyPrint(depth + 1);
        cout << " " << params[i];
    }
    cout << ");" << endl;
}

shared_ptr<MethodDeclStmt> MethodDeclStmt::methodDeclStmt() {
    return static_pointer_cast<MethodDeclStmt, Stmt>(shared_from_this());
}

MethodDefStmt::MethodDefStmt(const string &name, const shared_ptr<BSVType> &returnType,
                             const std::vector<std::string> &params,
                             const std::vector<std::shared_ptr<BSVType>> &paramTypes,
                             const shared_ptr<Expr> &guard,
                             const vector<std::shared_ptr<Stmt>> &stmts)
        : Stmt(MethodDefStmtType), name(name), returnType(returnType),
          params(params), paramTypes(paramTypes),
          guard(guard), stmts(stmts) {}

MethodDefStmt::~MethodDefStmt() {

}

void MethodDefStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "method ";
    returnType->prettyPrint(depth + 1);
    cout << " " << name << "(";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0)
            cout << ", ";
        paramTypes[i]->prettyPrint(depth + 1);
        cout << " " << params[i];
    }
    cout << ");" << endl;
    for (size_t i = 0; i < stmts.size(); i++) {
        stmts.at(i)->prettyPrint(depth + 1);
    }
    indent(4 * depth);
    cout << "endmethod" << endl;
}

shared_ptr<MethodDefStmt> MethodDefStmt::methodDefStmt() {
    return static_pointer_cast<MethodDefStmt, Stmt>(shared_from_this());
}

shared_ptr<Stmt> MethodDefStmt::rename(string prefix, LexicalScope &parentScope) {
    LexicalScope scope(parentScope);
    shared_ptr<Expr> renamedGuard;
    if (guard)
        renamedGuard = guard->rename(prefix, parentScope);
    vector<shared_ptr<Stmt>> renamedStmts;
    for (size_t i = 0; i < stmts.size(); i++) {
        renamedStmts.push_back(stmts[i]->rename(prefix, scope));
    }
    return shared_ptr<Stmt>(new MethodDefStmt(name, returnType, params, paramTypes, renamedGuard, renamedStmts));
}

ModuleDefStmt::ModuleDefStmt(const std::string &name, const std::shared_ptr<BSVType> &interfaceType,
                             const std::vector<std::string> &params,
                             const std::vector<std::shared_ptr<BSVType>> &paramTypes,
                             const std::vector<std::shared_ptr<Stmt>> &stmts)
        : Stmt(ModuleDefStmtType), name(name),
          params(params), paramTypes(paramTypes),
          interfaceType(interfaceType), stmts(stmts) {

}

ModuleDefStmt::~ModuleDefStmt() {

}

void ModuleDefStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "module ";
    interfaceType->prettyPrint(depth + 1);
    cout << " " << name << "(";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0)
            cout << ", ";
        paramTypes[i]->prettyPrint(depth + 1);
        cout << " " << params[i];
    }
    cout << ");" << endl;
    for (size_t i = 0; i < stmts.size(); i++) {
        if (stmts.at(i))
            stmts.at(i)->prettyPrint(depth + 1);
        else
            cout << "Empty stmt ..." << endl;
    }
    indent(4 * depth);
    cout << "endmodule" << endl;
}

shared_ptr<ModuleDefStmt> ModuleDefStmt::moduleDefStmt() {
    return static_pointer_cast<ModuleDefStmt, Stmt>(shared_from_this());
}

shared_ptr<Stmt> ModuleDefStmt::rename(string prefix, LexicalScope &parentScope) {
    LexicalScope scope(&parentScope);
    vector<string> renamedParams;
    vector<shared_ptr<Stmt>> renamedStmts;
    //FIXME: rename module?
    for (size_t i = 0; i < params.size(); i++) {
        string renamedParam(prefix + params[i]);
        renamedParams.push_back(renamedParam);
        scope.bind(params[i], renamedParam);
    }
    for (size_t i = 0; i < stmts.size(); i++) {
	cout << "renaming stmt" << endl;
	stmts[i]->prettyPrint();
        renamedStmts.push_back(stmts[i]->rename(prefix, scope));
    }
    return shared_ptr<Stmt>(new ModuleDefStmt(name, interfaceType, renamedParams, paramTypes, renamedStmts));
}

IfStmt::IfStmt(const shared_ptr<Expr> &condition, const shared_ptr<Stmt> &thenStmt,
               const shared_ptr<Stmt> &elseStmt) : Stmt(IfStmtType), condition(condition), thenStmt(thenStmt),
                                                   elseStmt(elseStmt) {}

IfStmt::~IfStmt() {

}

void IfStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "if (";
    condition->prettyPrint(depth + 1);
    cout << ") ";
    thenStmt->prettyPrint(depth + 1);
    if (elseStmt) {
        indent(4 * depth);
        cout << "else ";
        elseStmt->prettyPrint(depth + 1);
    }
    cout << endl;
}

shared_ptr<IfStmt> IfStmt::ifStmt() { return static_pointer_cast<IfStmt, Stmt>(shared_from_this()); }

shared_ptr<struct Stmt> IfStmt::rename(string prefix, LexicalScope &scope) {
    if (elseStmt)
        return shared_ptr<Stmt>(new IfStmt(condition->rename(prefix, scope),
                                           thenStmt->rename(prefix, scope),
                                           elseStmt->rename(prefix, scope)));
    else
        return shared_ptr<Stmt>(new IfStmt(condition->rename(prefix, scope),
                                           thenStmt->rename(prefix, scope),
                                           shared_ptr<Stmt>()));
}

BlockStmt::BlockStmt(const std::vector<std::shared_ptr<Stmt>> &stmts) : Stmt(BlockStmtType), stmts(stmts) {}

BlockStmt::~BlockStmt() {}

void BlockStmt::prettyPrint(int depth) {
    cout << "begin" << endl;
    for (size_t i = 0; i < stmts.size(); i++) {
        if (stmts.at(i))
            stmts.at(i)->prettyPrint(depth + 1);
        else
            cout << "emptystmt:" << to_string(i) << endl;
    }
    indent(4 * depth);
    cout << "end" << endl;
}

shared_ptr<BlockStmt> BlockStmt::blockStmt() { return static_pointer_cast<BlockStmt, Stmt>(shared_from_this()); }

shared_ptr<struct Stmt> BlockStmt::rename(string prefix, LexicalScope &parentScope) {
    LexicalScope scope(&parentScope);
    vector<shared_ptr<Stmt>> renamedStmts;
    for (size_t i = 0; i < stmts.size(); i++) {
        renamedStmts.push_back(stmts[i]->rename(prefix, scope));
    }
    return shared_ptr<Stmt>(new BlockStmt(renamedStmts));
}


void ReturnStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "return ";
    value->prettyPrint(depth);
    cout << ";" << endl;
}

shared_ptr<ReturnStmt> ReturnStmt::returnStmt() { return static_pointer_cast<ReturnStmt, Stmt>(shared_from_this()); }

shared_ptr<struct Stmt> ReturnStmt::rename(string prefix, LexicalScope &scope) {
    return shared_ptr<Stmt>(new ReturnStmt(value->rename(prefix, scope)));
}

void ExprStmt::prettyPrint(int depth) {
    indent(4 * depth);
    expr->prettyPrint(depth);
    cout << ";" << endl;
}

shared_ptr<ExprStmt> ExprStmt::exprStmt() { return static_pointer_cast<ExprStmt, Stmt>(shared_from_this()); }

shared_ptr<struct Stmt> ExprStmt::rename(string prefix, LexicalScope &scope) {
    return shared_ptr<Stmt>(new ExprStmt(expr->rename(prefix, scope)));
}

ImportStmt::ImportStmt(const std::string name) : Stmt(ImportStmtType), name(name) {

}

void ImportStmt::prettyPrint(int depth) {
    cout << "import " << name << " :: *;" << endl;
}

shared_ptr<ImportStmt> ImportStmt::importStmt() { return static_pointer_cast<ImportStmt, Stmt>(shared_from_this()); }

InterfaceDeclStmt::InterfaceDeclStmt(const std::string &name, const std::shared_ptr<BSVType> &interfaceType,
                                     const vector<std::shared_ptr<Stmt>> &decls)
        : Stmt(InterfaceDeclStmtType), name(name),
          interfaceType(interfaceType), decls(decls) {
}

void InterfaceDeclStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "interface ";
    interfaceType->prettyPrint(depth + 1);
    cout << ":" << endl;
    for (size_t i = 0; i < decls.size(); i++) {
        decls[i]->prettyPrint(depth + 1);
    }
    indent(4 * depth);
    cout << "endinterface" << endl;
}

shared_ptr<InterfaceDeclStmt>
InterfaceDeclStmt::interfaceDeclStmt() { return static_pointer_cast<InterfaceDeclStmt, Stmt>(shared_from_this()); }

TypedefSynonymStmt::TypedefSynonymStmt(const std::shared_ptr<BSVType> &typedeftype,
                                       const std::shared_ptr<BSVType> &type)
        : Stmt(TypedefSynonymStmtType), typedeftype(typedeftype), type(type) {

}

void TypedefSynonymStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "typedef ";
    type->prettyPrint();
    cout << " ";
    typedeftype->prettyPrint();
    cout << ":" << endl;
}

shared_ptr<TypedefSynonymStmt>
TypedefSynonymStmt::typedefSynonymStmt() { return static_pointer_cast<TypedefSynonymStmt, Stmt>(shared_from_this()); }

TypedefStructStmt::TypedefStructStmt(const std::string &name, const std::shared_ptr<BSVType> &structType,
                                     const std::vector<std::string> &members,
                                     const std::vector<std::shared_ptr<BSVType>> &memberTypes)
        : Stmt(TypedefStructStmtType),
          name(name), structType(structType),
          members(members), memberTypes(memberTypes) {
}

void TypedefStructStmt::prettyPrint(int depth) {
    indent(4 * depth);
    cout << "typedef struct {" << endl;
    for (size_t i = 0; i < members.size(); i++) {
        indent(4 * (depth + 1));
        memberTypes[i]->prettyPrint(depth + 1);
        cout << " " << members[i] << ";" << endl;
    }
    indent(4 * depth);
    cout << "} ";
    structType->prettyPrint(depth);
    cout << ":" << endl;
}

shared_ptr<TypedefStructStmt>
TypedefStructStmt::typedefStructStmt() { return static_pointer_cast<TypedefStructStmt, Stmt>(shared_from_this()); }

PackageDefStmt::PackageDefStmt(const string& name, const vector<shared_ptr<Stmt>> &stmts)
    : Stmt(PackageDefStmtType), name(name),
    stmts(stmts), bindings() {
    for (int i = 0; i < stmts.size(); i++) {
        if (shared_ptr<ModuleDefStmt> moduleStmt = stmts[i]->moduleDefStmt()) {
            bindings[moduleStmt->name] = moduleStmt;
        }
    }
}

shared_ptr<Stmt> PackageDefStmt::lookup(const string &name)
{
    return bindings[name];
}

void PackageDefStmt::prettyPrint(int depth) {
    for (size_t i = 0; i < stmts.size(); i++) {
        indent(4 * (depth + 1));
        stmts[i]->prettyPrint(depth + 1);
        cout << " " << stmts[i] << ";" << endl;
    }
}
