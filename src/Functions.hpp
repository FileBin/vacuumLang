#pragma once
#include "stdafx.hpp"
#include <stdexcept>

/// logError - This is little helper functions for error handling.
void logError(String Str) {
    throw Str;
}