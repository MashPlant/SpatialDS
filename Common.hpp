//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_COMMON_HPP
#define SPATIALDS_COMMON_HPP

#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <type_traits>

#if __has_include(<parallel/algorithm>)
#include <parallel/algorithm>
namespace par = __gnu_parallel;
#else
namespace par = std;
#endif

using Float = float;
#endif //SPATIALDS_COMMON_HPP
