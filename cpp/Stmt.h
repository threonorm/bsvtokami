#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

using namespace std;

#include "BSVType.h"
#include "Expr.h"
#include "LValue.h"
#include "LexicalScope.h"

void indent(int depth);

void indent(ostream &s, int depth);

enum StmtType {
    InvalidStmtType,
    ActionBindingStmtType,
    BlockStmtType,
    ExprStmtType,
    InterfaceDeclStmtType,
    IfStmtType,
    ImportStmtType,
    MethodDeclStmtType,
    MethodDefStmtType,
    ModuleDefStmtType,
    PackageDefStmtType,
    ReturnStmtType,
    TypedefStructStmtType,
    TypedefSynonymStmtType,
    VarBindingStmtType,
    VarAssignStmtType,
    RuleDefStmtType,
    RegWriteStmtType
};

class ActionBindingStmt;

class BlockStmt;

class ExprStmt;

class IfStmt;

class ImportStmt;

class InterfaceDeclStmt;

class MethodDeclStmt;

class MethodDefStmt;

class ModuleDefStmt;

class RegWriteStmt;

class ReturnStmt;

class RuleDefStmt;

class TypedefStructStmt;

class TypedefSynonymStmt;

class VarBindingStmt;

class VarAssignStmt;

class Stmt : public enable_shared_from_this<Stmt> {
protected:
    const StmtType stmtType;

public:
    Stmt(StmtType stmtType);

    virtual ~Stmt() {}

    virtual void prettyPrint(int depth = 0) = 0;

    virtual shared_ptr<ActionBindingStmt> actionBindingStmt() { return shared_ptr<ActionBindingStmt>(); }

    virtual shared_ptr<BlockStmt> blockStmt() { return shared_ptr<BlockStmt>(); }

    virtual shared_ptr<ExprStmt> exprStmt() { return shared_ptr<ExprStmt>(); }

    virtual shared_ptr<IfStmt> ifStmt() { return shared_ptr<IfStmt>(); }

    virtual shared_ptr<ImportStmt> importStmt() { return shared_ptr<ImportStmt>(); }

    virtual shared_ptr<InterfaceDeclStmt> interfaceDeclStmt() { return shared_ptr<InterfaceDeclStmt>(); }

    virtual shared_ptr<MethodDeclStmt> methodDeclStmt() { return shared_ptr<MethodDeclStmt>(); }

    virtual shared_ptr<MethodDefStmt> methodDefStmt() { return shared_ptr<MethodDefStmt>(); }

    virtual shared_ptr<ModuleDefStmt> moduleDefStmt() { return shared_ptr<ModuleDefStmt>(); }

    virtual shared_ptr<RegWriteStmt> regWriteStmt() { return shared_ptr<RegWriteStmt>(); }

    virtual shared_ptr<ReturnStmt> returnStmt() { return shared_ptr<ReturnStmt>(); }

    virtual shared_ptr<RuleDefStmt> ruleDefStmt() { return shared_ptr<RuleDefStmt>(); }

    virtual shared_ptr<TypedefStructStmt> typedefStructStmt() { return shared_ptr<TypedefStructStmt>(); }

    virtual shared_ptr<TypedefSynonymStmt> typedefSynonymStmt() { return shared_ptr<TypedefSynonymStmt>(); }

    virtual shared_ptr<VarBindingStmt> varBindingStmt() { return shared_ptr<VarBindingStmt>(); }

    virtual shared_ptr<VarAssignStmt> varAssignStmt() { return shared_ptr<VarAssignStmt>(); };

    virtual shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope);
};

class ImportStmt : public Stmt {
public:
    explicit ImportStmt(const string name);

    ~ImportStmt() override {}

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<ImportStmt> importStmt() override;

    const string name;
};

class TypedefSynonymStmt : public Stmt {
public:
    TypedefSynonymStmt(const shared_ptr<BSVType> &typedeftype, const shared_ptr<BSVType> &type);

    ~TypedefSynonymStmt() override {}

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<TypedefSynonymStmt> typedefSynonymStmt() override;

private:
    shared_ptr<BSVType> typedeftype; // type being defined
    shared_ptr<BSVType> type;
};

class TypedefStructStmt : public Stmt {
public:
    TypedefStructStmt(const string &name, const shared_ptr<BSVType> &structType,
                      const vector<string> &members,
                      const vector<shared_ptr<BSVType>> &memberTypes);

    virtual ~TypedefStructStmt() override {}

    void prettyPrint(int depth) override;

    virtual shared_ptr<TypedefStructStmt> typedefStructStmt() override;

public:
    const string name;
    const shared_ptr<BSVType> structType;
    const vector<string> members;
    const vector<shared_ptr<BSVType>> memberTypes;
};

class InterfaceDeclStmt : public Stmt {
public:
    InterfaceDeclStmt(const string &name, const shared_ptr<BSVType> &interfaceType,
                      const vector<shared_ptr<Stmt>> &decls);

    ~InterfaceDeclStmt() override {}

    void prettyPrint(int depth) override;

    virtual shared_ptr<InterfaceDeclStmt> interfaceDeclStmt() override;

    const string name;
    const shared_ptr<BSVType> interfaceType;
    const vector<shared_ptr<Stmt>> decls;
};

class PackageDefStmt : public Stmt {
public:
    PackageDefStmt(const string &name, const vector<shared_ptr<Stmt>> &package_stmts);

    void prettyPrint(int depth) override;

    shared_ptr<Stmt> lookup(const string &name);
    const vector<shared_ptr<Stmt>> stmts;
private:
    string name;
    map<string, shared_ptr<Stmt>> bindings;
};

class ModuleDefStmt : public Stmt {
public:
    ModuleDefStmt(const string &name, const shared_ptr<BSVType> &interfaceType,
                  const vector<string> &params,
                  const vector<shared_ptr<BSVType>> &paramTypes,
                  const vector<shared_ptr<Stmt>> &stmts);

    ~ModuleDefStmt() override;

    void prettyPrint(int depth) override;

    virtual shared_ptr<ModuleDefStmt> moduleDefStmt() override;

    shared_ptr<Stmt> rename(string prefix, LexicalScope &scope) override;

public:
    const string name;
    const shared_ptr<BSVType> interfaceType;
    const vector<string> params;
    const vector<shared_ptr<BSVType>> paramTypes;
    const vector<shared_ptr<Stmt>> stmts;
};

class MethodDeclStmt : public Stmt {
public:
    MethodDeclStmt(const string &name, const shared_ptr<BSVType> &returnType,
                   const vector<string> &params,
                   const vector<shared_ptr<BSVType>> &paramTypes);

    virtual ~MethodDeclStmt() override {}

    void prettyPrint(int depth) override;

    virtual shared_ptr<MethodDeclStmt> methodDeclStmt() override;

public:
    const string name;
    const shared_ptr<BSVType> returnType;
    const vector<string> params;
    const vector<shared_ptr<BSVType>> paramTypes;
};

class MethodDefStmt : public Stmt {
public:
    MethodDefStmt(const string &name, const shared_ptr<BSVType> &returnType,
                  const vector<string> &params,
                  const vector<shared_ptr<BSVType>> &paramTypes,
                  const shared_ptr<Expr> &guard,
                  const vector<shared_ptr<Stmt>> &stmts);

    virtual ~MethodDefStmt();

    void prettyPrint(int depth) override;

    virtual shared_ptr<MethodDefStmt> methodDefStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

public:
    const string name;
    const shared_ptr<BSVType> returnType;
    const vector<string> params;
    const vector<shared_ptr<BSVType>> paramTypes;
    const shared_ptr<Expr> guard;
    const vector<shared_ptr<Stmt>> stmts;
};

class RuleDefStmt : public Stmt {
public:
    const string name;
    const shared_ptr<Expr> guard;
    const vector<shared_ptr<Stmt>> stmts;

public:
    RuleDefStmt(const string &name, const shared_ptr<Expr> &guard, const vector<shared_ptr<Stmt>> &stmts);

    ~RuleDefStmt() override {}

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<RuleDefStmt> ruleDefStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};


class ActionBindingStmt : public Stmt {
public:
    const shared_ptr<BSVType> bsvtype;
    const string name;
    const shared_ptr<Expr> rhs;
public:
    ActionBindingStmt(const shared_ptr<BSVType> &bsvtype, const string &name,
                      const shared_ptr<Expr> &rhs);

    ~ActionBindingStmt() override {}

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<ActionBindingStmt> actionBindingStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class VarAssignStmt : public Stmt {
public:
    const shared_ptr<LValue> lhs;
    const string op;
    const shared_ptr<Expr> rhs;
public:
    VarAssignStmt(const shared_ptr<LValue> &lhs, const string &op, const shared_ptr<Expr> &rhs);

    ~VarAssignStmt() override = default;

    void prettyPrint(int depth = 0) override;

    shared_ptr<VarAssignStmt> varAssignStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class VarBindingStmt : public Stmt {
public:
    const shared_ptr<BSVType> bsvtype;
    const string name;
    const shared_ptr<Expr> rhs;
public:
    VarBindingStmt(const shared_ptr<BSVType> &bsvtype, const string &name,
                   const shared_ptr<Expr> &rhs);

    ~VarBindingStmt() override {}

    void prettyPrint(int depth = 0) override;

    shared_ptr<VarBindingStmt> varBindingStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class RegWriteStmt : public Stmt {
public:
    const string regName;
    const shared_ptr<Expr> rhs;
public:
    RegWriteStmt(const string &regName, const shared_ptr<Expr> &rhs);

    ~RegWriteStmt() override {}

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<RegWriteStmt> regWriteStmt() override;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class BlockStmt : public Stmt {
public:
    explicit BlockStmt(const vector<shared_ptr<Stmt>> &stmts);

    ~BlockStmt() override;

    void prettyPrint(int depth = 0) override;

    virtual shared_ptr<BlockStmt> blockStmt() override;

    const vector<shared_ptr<Stmt>> stmts;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class IfStmt : public Stmt {
public:
    IfStmt(const shared_ptr<Expr> &condition, const shared_ptr<Stmt> &thenStmt,
           const shared_ptr<Stmt> &elseStmt);

    ~IfStmt() override;

    void prettyPrint(int depth) override;

    virtual shared_ptr<IfStmt> ifStmt() override;

    const shared_ptr<Expr> condition;
    const shared_ptr<Stmt> thenStmt;
    const shared_ptr<Stmt> elseStmt;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class ReturnStmt : public Stmt {
public:
    ReturnStmt(const shared_ptr<Expr> value) : Stmt(ReturnStmtType), value(value) {}

    ~ReturnStmt() override {}

    void prettyPrint(int depth) override;

    virtual shared_ptr<ReturnStmt> returnStmt() override;

    const shared_ptr<Expr> value;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;

};

class ExprStmt : public Stmt {
public:
    ExprStmt(const shared_ptr<Expr> expr) : Stmt(ExprStmtType), expr(expr) {}

    ~ExprStmt() override {}

    void prettyPrint(int depth) override;

    virtual shared_ptr<ExprStmt> exprStmt() override;

    const shared_ptr<Expr> expr;

    shared_ptr<struct Stmt> rename(string prefix, LexicalScope &scope) override;
};
