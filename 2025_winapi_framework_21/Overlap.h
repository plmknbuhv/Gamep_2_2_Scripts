#pragma once
#include "Collider.h"

class Overlap
{
public:
    static bool OverlapBox(Vec2 _pos, Vec2 _size,
        Layer _layer, std::vector<Collider*>& _stored, bool _debug = false);
};