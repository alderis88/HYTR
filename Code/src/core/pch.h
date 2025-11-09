#pragma once

// Disable assertions in Release builds for portable deployment
#ifndef _DEBUG
#define NDEBUG
#endif

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <cassert>

// Force disable assertions in Release mode
#ifndef _DEBUG
#ifdef assert
#undef assert
#endif
#define assert(x) ((void)0)
#endif

#include <fstream>
#include <streambuf>
#include <cstdint>
#include <random>

#include "SFML/Graphics.hpp"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "redirections.h"
