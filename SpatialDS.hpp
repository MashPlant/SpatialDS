//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_SPATIALDS_HPP
#define SPATIALDS_SPATIALDS_HPP

#include "Common.hpp"

template<typename T>
struct PointTraits;

template<typename T, typename Category>
struct SpatialDS;

struct BruteTag;

struct KDTreeTag;

// I pick the name myself
// pow stand for pow(2, n), e.g., QuadTree & OctTree
struct PowSplitTreeTag;

struct SortSweepTag;

struct UniformGridTag;

// do nothing
struct ControlGroupTag;

#endif //SPATIALDS_SPATIALDS_HPP
