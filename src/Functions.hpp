#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// logError - This is little helper functions for error handling.
void logError(String str) noexcept(false) {
    PRINT(SPREF "Error: " STRPARAM "\n", str.c_str());
    throw -1;
}

void logWarning(String str) {
    PRINT(SPREF "Warning: " STRPARAM, str.c_str());
}