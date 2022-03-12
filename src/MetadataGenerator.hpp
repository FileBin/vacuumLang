#pragma once
#include "stdafx.hpp"
#include "Types.hpp"

struct Metadata {
    Tree<Type> ClassTree, InterfaceTree;

    Metadata(): ClassTree(Type::Object) {
        ClassTree.root->AddChild(Type::SByte);
        ClassTree.root->AddChild(Type::Byte);
        ClassTree.root->AddChild(Type::Short);
        ClassTree.root->AddChild(Type::UShort);
        ClassTree.root->AddChild(Type::Int);
        ClassTree.root->AddChild(Type::UInt);
        ClassTree.root->AddChild(Type::Num);
        ClassTree.root->AddChild(Type::UNum);

        ClassTree.root->AddChild(Type::Flt);
        ClassTree.root->AddChild(Type::Dbl);
        ClassTree.root->AddChild(Type::BigDbl);

        ClassTree.root->AddChild(Type::Char);
    }
};

class MetadataGenerator {
public:
    Metadata metadata;
};