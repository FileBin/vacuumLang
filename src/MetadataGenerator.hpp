#pragma once
#include "stdafx.hpp"
#include "Types.hpp"
#include "Functions.hpp"

struct Metadata {
    Tree<Type> ClassTree, InterfaceTree;

    Metadata() : ClassTree(Type::Object) {
        auto val_ty = ClassTree.root->AddChild(Objects::ValueType());
        val_ty->AddChild(Type::SByte);
        val_ty->AddChild(Type::Byte);
        val_ty->AddChild(Type::Short);
        val_ty->AddChild(Type::UShort);
        val_ty->AddChild(Type::Int);
        val_ty->AddChild(Type::UInt);
        val_ty->AddChild(Type::Num);
        val_ty->AddChild(Type::UNum);

        val_ty->AddChild(Type::Flt);
        val_ty->AddChild(Type::Dbl);
        val_ty->AddChild(Type::BigDbl);

        val_ty->AddChild(Type::Char);
    }
};

#include "Lexer.hpp"

class MetadataGenerator {
public:
    typedef STD shared_ptr<Lexer<char_t>> pLexer;

protected:
    Metadata metadata;
    pLexer lexer;

public:
    MetadataGenerator(pLexer lexer) : lexer(lexer) {
        
    }
};