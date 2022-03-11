#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// LogError* - These are little helper functions for error handling.
void LogError(const char *Str) {
    throw STD exception(Str);
}