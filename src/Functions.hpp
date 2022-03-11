#pragma once
#include "stdafx.hpp"
#include "AST.hpp"

/// LogError* - These are little helper functions for error handling.
void LogError(const char *Str) {
    throw Str;
}