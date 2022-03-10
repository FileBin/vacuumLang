#pragma once
#include "stdafx.hpp"
#include "AST.hpp"

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<AST::ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

llvm::Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

std::unique_ptr<AST::PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}