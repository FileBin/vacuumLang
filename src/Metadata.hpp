#pragma once
#include "stdafx.hpp"

class Type;

struct Metadata {
    Tree<Type> classTree, InterfaceTree;
    Metadata();
};

#include "Lexer.hpp"
#include "Types.hpp"

Metadata::Metadata() : classTree(Type::createPrototype(this, "Object")) {
    auto val_ty = classTree.root->addChild(Type::createPrototype(this, "ValueType"));

    auto node_bool = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Bool)));
    auto node_sbyte = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::SByte)));
    auto node_byte = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Byte)));
    auto node_short = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Short)));
    auto node_ushort = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UShort)));
    auto node_int = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Int)));
    auto node_uint = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UInt)));
    auto node_num = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Num)));
    auto node_unum = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::UNum)));

    auto node_flt = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Flt)));
    auto node_dbl = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Dbl)));

    auto node_char = val_ty->addChild(Type::createPrototype(this, Type::getName(Type::Char)));

    node_bool->data->type = Type::Bool;
    node_sbyte->data->type = Type::SByte;
    node_byte->data->type = Type::Byte;
    node_short->data->type = Type::Short;
    node_ushort->data->type = Type::UShort;
    node_int->data->type = Type::Int;
    node_uint->data->type = Type::UInt;
    node_num->data->type = Type::Num;
    node_unum->data->type = Type::UNum;
    node_flt->data->type = Type::Flt;
    node_dbl->data->type = Type::Dbl;
    node_char->data->type = Type::Char;

    node_bool->data->createDefinition(val_ty->data);
    node_sbyte->data->createDefinition(val_ty->data);
    node_byte->data->createDefinition(val_ty->data);
    node_short->data->createDefinition(val_ty->data);
    node_ushort->data->createDefinition(val_ty->data);
    node_int->data->createDefinition(val_ty->data);
    node_uint->data->createDefinition(val_ty->data);
    node_num->data->createDefinition(val_ty->data);
    node_unum->data->createDefinition(val_ty->data);
    node_flt->data->createDefinition(val_ty->data);
    node_dbl->data->createDefinition(val_ty->data);
    node_char->data->createDefinition(val_ty->data);
}