#pragma once
#include "stdafx.hpp"
#include "ASTheader.h"
#include "Operators.hpp"
#include "Keyword.hpp"
#include "Types.hpp"
#include "Metadata.hpp"
class CompilerModule;
struct Enviroment;

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

namespace AST {
    interface IASTNode {
        virtual llvm::Value* codeGen(Metadata* pmeta) = 0;
    };

    class ScopeAST : public IASTNode {
    public:
        STD shared_ptr<CompilerModule> module;
        STD shared_ptr<Enviroment> env;
        STD vector<STD unique_ptr<IASTNode>> children;
        ScopeAST(CompilerModule* module, STD shared_ptr<Enviroment> env) : module(module), env(env) {}
        void push(IASTNode* node) {
            children.push_back(STD unique_ptr<IASTNode>(node));
        }

        llvm::Value* codeGen(Metadata* pmeta) override;
    };
    /// ExprAST - Base class for all expression nodes.
    class ExprAST : public IASTNode {
    public:
        typedef STD shared_ptr<ScopeAST> Scope;
        Scope scope;
        Type* type;
        STD shared_ptr<CompilerModule> module;
        ExprAST(CompilerModule* module, Type* type, Scope scope) : module(module), type(type), scope(scope) {}
        virtual ~ExprAST() = default;
    };

    /// NumberExprAST - Expression class for numeric literals like "1.0".
    class NumberExprAST : public ExprAST
    {
        String str;
    public:
        NumberExprAST(CompilerModule* module, Scope scope, Type* type, String str) : ExprAST(module, type, scope), str(str) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// VariableExprAST - Expression class for referencing a variable, like "a".
    class VariableExpression : public ExprAST {
        Type* type;
        String name;

    public:
        VariableExpression(CompilerModule* module, Scope scope, Type* type, String name) : ExprAST(module, type, scope), name(name), type(type) {}

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST
    {
        Operator Op;
        STD unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(CompilerModule* module, Scope scope, Type* ty, Operator Op, STD unique_ptr<ExprAST> LHS,
            STD unique_ptr<ExprAST> RHS)
            : ExprAST(module, ty, scope), Op(Op),
            LHS(STD move(LHS)),
            RHS(STD move(RHS))
        {
            if (!Type::isStd(ty))
                logError("Not standart type (" + ty->toString() + ") occured whe generating division operator!");
            type = ty;
        }

        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST {
        String Callee;
        STD vector<STD unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(CompilerModule* module, Type* type, Scope scope, const String& Callee,
            STD vector<STD unique_ptr<ExprAST>> Args)
            : ExprAST(module, type, scope), Callee(Callee), Args(STD move(Args)) {}
        llvm::Value* codeGen(Metadata* pmeta) override;
    };

    /// PrototypeAST - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class PrototypeAST : IASTNode
    {
        STD shared_ptr<CompilerModule> module;
        String name;
        STD vector<::Variable> args;

    public:
        PrototypeAST(CompilerModule* module, const String& name, STD vector<::Variable> args)
            : module(module), name(name), args(STD move(args)) {}

        llvm::Function* codeGen(Metadata* pmeta) override;
        const String& getName() const { return name; }
    };

    /// FunctionAST - This class represents a function definition itself.
    class FunctionAST : IASTNode
    {
        STD shared_ptr<CompilerModule> module;
        STD unique_ptr<PrototypeAST> Proto;
        STD unique_ptr<ScopeAST> Body;
        STD shared_ptr<Enviroment> env;

    public:
        FunctionAST(CompilerModule* module,
            STD unique_ptr<PrototypeAST> Proto,
            STD shared_ptr<Enviroment> env,
            TokenBufferStream body)
            : module(module),
            Proto(STD move(Proto)),
            env(env),
            Body(STD move(Body)) {}

        llvm::Function* codeGen(Metadata* pmeta) override;
    };
}

#include "CompilerModule.hpp"
#include "Enviroment.hpp"


//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//
namespace AST {
    llvm::Value* NumberExprAST::codeGen(Metadata* pmeta) {
        using namespace llvm;
        auto s = ToStdString(str);
    }

    llvm::Value* VariableExpression::codeGen(Metadata* pmeta) {
        // Look this variable up in the function.
        llvm::Value* V = scope->env->findVar(name, module.get());
        if (!V)
            logError("Unknown variable \'" + name + "\'!");
        return V;
    }

    llvm::Value* BinaryExprAST::codeGen(Metadata* pmeta)
    {
        using Cmp = llvm::CmpInst;
        using T = llvm::Type;
        auto L = LHS->codeGen(pmeta);
        auto R = RHS->codeGen(pmeta);
        if (!L || !R)
            return nullptr;

        auto& b = module->getBuilder();
        auto& c = module->getContext();

        switch (type->getStdType())
        {
        case StdType::Signed:
            switch (Op.ty)
            {
            case Operator::Add:
                return b.CreateAdd(L, R, "iadd");
            case Operator::Sub:
                return b.CreateSub(L, R, "isub");
            case Operator::Mul:
                return b.CreateMul(L, R, "imul");
            case Operator::Div:
                return b.CreateSDiv(L, R, "sdiv");
            case Operator::Mod: // l - (r*(l/r))
                return b.CreateSub(L, b.CreateMul(R, b.CreateSDiv(L, R, "sdiv"), "imul"), "isub");
            case Operator::Less:
                return b.CreateICmpSLT(L, R, "scmplt");
            case Operator::Greater:
                return b.CreateICmpSGT(L, R, "scmpsgt");
            case Operator::LessEqual:
                return b.CreateICmpSLE(L, R, "scmple");
            case Operator::GreaterEqual:
                return b.CreateICmpSGE(L, R, "scmpge");
            case Operator::Equal:
                return b.CreateICmpEQ(L, R, "icmpeq");
            default:
                logError("Invalid binary operator!");
                return 0;
            }
        case StdType::Unsigned:
            switch (Op.ty)
            {
            case Operator::Add:
                return b.CreateAdd(L, R, "iadd");
            case Operator::Sub:
                return b.CreateSub(L, R, "isub");
            case Operator::Mul:
                return b.CreateMul(L, R, "imul");
            case Operator::Div:
                return b.CreateUDiv(L, R, "udiv");
            case Operator::Mod: // l - (r*(l/r))
                return b.CreateSub(L, b.CreateMul(R, b.CreateUDiv(L, R, "udiv"), "imul"), "isub");
            case Operator::Less:
                return b.CreateICmpSLT(L, R, "scmplt");
            case Operator::Greater:
                return b.CreateICmpSGT(L, R, "scmpsgt");
            case Operator::LessEqual:
                return b.CreateICmpSLE(L, R, "scmple");
            case Operator::GreaterEqual:
                return b.CreateICmpSGE(L, R, "scmpge");
            case Operator::Equal:
                return b.CreateICmpEQ(L, R, "icmpeq");
            default:
                logError("Invalid binary operator!");
                return 0;
            }
        case StdType::Float:
            switch (Op.ty)
            {
            case Operator::Add:
                return b.CreateFAdd(L, R, "fadd");
            case Operator::Sub:
                return b.CreateFSub(L, R, "fsub");
            case Operator::Mul:
                return b.CreateFMul(L, R, "fmul");
            case Operator::Div:
                return b.CreateFDiv(L, R, "fdiv");
            case Operator::Mod: //  trunc(l/r)*r
                return b.CreateFPTrunc(b.CreateFDiv(L, R, "fdiv"), type->getLlvmType(c), "ftruncf");
            case Operator::Less:
                return b.CreateFCmpULT(L, R, "fcmplt");
            case Operator::Greater:
                return b.CreateFCmpUGT(L, R, "fcmpsgt");
            case Operator::LessEqual:
                return b.CreateFCmpULE(L, R, "fcmple");
            case Operator::GreaterEqual:
                return b.CreateFCmpUGE(L, R, "fcmpge");
            case Operator::Equal:
                return b.CreateFCmpUEQ(L, R, "fcmpeq");
            default:
                logError("Invalid binary operator!");
                return 0;
            }
        default:
            logError("Unknown type");
            return 0;
        }
    }

    llvm::Value* CallExprAST::codeGen(Metadata* pmeta)
    {
        // Look up the name in the global module table.
        llvm::Function* CalleeF = module->getModule().getFunction(ToStdString(Callee));
        if (!CalleeF)
            logError("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != Args.size())
            logError("Incorrect # arguments passed");

        std::vector<llvm::Value*> ArgsV;
        for (unsigned i = 0, e = Args.size(); i != e; ++i)
        {
            ArgsV.push_back(Args[i]->codeGen(pmeta));
            if (!ArgsV.back())
                return nullptr;
        }

        return module->getBuilder().CreateCall(CalleeF, ArgsV, "calltmp");
    }

    llvm::Function* PrototypeAST::codeGen(Metadata* pmeta)
    {
        using namespace llvm;
        using llvm::Type;
        using llvm::Function;
        // Make the function type:  double(double,double) etc.
        std::vector<Type*> Doubles(args.size(), Type::getDoubleTy(module->getContext()));
        FunctionType* FT =
            FunctionType::get(Type::getDoubleTy(module->getContext()), Doubles, false);

        Function* F =
            Function::Create(FT, Function::ExternalLinkage, ToStdString(name), module->getModule());

        // Set names for all arguments.
        unsigned idx = 0;
        for (auto& arg : F->args()) {
            arg.setName(ToStdString(args[idx++].getName()));
        }
        return F;
    }

    llvm::Function* FunctionAST::codeGen(Metadata* pmeta) {
        using namespace llvm;
        // First, check for an existing function from a previous 'extern' declaration.
        llvm::Function* TheFunction = module->getModule().getFunction(ToStdString(Proto->getName()));

        if (!TheFunction)
            TheFunction = Proto->codeGen(pmeta);

        if (!TheFunction)
            return nullptr;

        // Create a new basic block to start insertion into.
        BasicBlock* BB = BasicBlock::Create(module->getContext(), "entry", TheFunction);
        module->getBuilder().SetInsertPoint(BB);

        // Record the function arguments in the NamedValues map.
        for (auto& Arg : TheFunction->args())
            env->addParam(Utf8ToWstring(std::string(Arg.getName())), &Arg);

        if (llvm::Value* RetVal = Body->codeGen(pmeta))
        {
            // Finish off the function.
            module->getBuilder().CreateRet(RetVal);

            // Validate the generated code, checking for consistency.
            llvm::verifyFunction(*TheFunction);

            return TheFunction;
        }

        // Error reading body, remove function.
        TheFunction->eraseFromParent();
        return nullptr;
    }
}
// static void HandleDefinition() {
//     if (auto FnAST = ParseDefinition())
//     {
//         if (auto *FnIR = FnAST->codegen())
//         {
//             fprintf(stderr, "Read function definition:");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }

// static void HandleExtern() {
//     if (auto ProtoAST = ParseExtern()) {
//         if (auto *FnIR = ProtoAST->codegen()) {
//             fprintf(stderr, "Read extern: ");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }

// static void HandleTopLevelExpression() {
//     // Evaluate a top-level expression into an anonymous function.
//     if (auto FnAST = ParseTopLevelExpr()) {
//         if (auto *FnIR = FnAST->codegen()) {
//             fprintf(stderr, "Read top-level expression:");
//             FnIR->print(errs());
//             fprintf(stderr, "\n");

//             // Remove the anonymous expression.
//             FnIR->eraseFromParent();
//         }
//     } else {
//         // Skip token for error recovery.
//         getNextToken();
//     }
// }