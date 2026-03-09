#include "pch.h"
#include "Overlap.h"
#include "SceneManager.h"
#include "Object.h"
#include "Core.h"

bool Overlap::OverlapBox(Vec2 _pos, Vec2 _size,
    Layer _layer, std::vector<Collider*>& _stored, bool _debug)
{
    _stored.clear();

    const auto& objects = GET_SINGLE(SceneManager)->GetCurScene()->GetLayerObjects(_layer);

    Vec2 queryHalfSize = { _size.x / 2.0f, _size.y / 2.0f };

    float queryMinX = _pos.x - queryHalfSize.x;
    float queryMaxX = _pos.x + queryHalfSize.x;
    float queryMinY = _pos.y - queryHalfSize.y;
    float queryMaxY = _pos.y + queryHalfSize.y;

    if (objects.empty())
        return false;

    for (Object* obj : objects)
    {
        if (obj == nullptr)
            continue;

        Collider* collider = obj->GetComponent<Collider>();
        if (collider != nullptr && collider->IsEnable())
        {
            Vec2 colPos = collider->GetUpdatedPos();
            Vec2 colSize = collider->GetSize();
            Vec2 colHalfSize = { colSize.x / 2.0f, colSize.y / 2.0f };

            float colMinX = colPos.x - colHalfSize.x;
            float colMaxX = colPos.x + colHalfSize.x;
            float colMinY = colPos.y - colHalfSize.y;
            float colMaxY = colPos.y + colHalfSize.y;

 
            bool separatedX = (queryMaxX < colMinX) || (queryMinX > colMaxX);

            bool separatedY = (queryMaxY < colMinY) || (queryMinY > colMaxY);

            if (!separatedX && !separatedY)
            {
                _stored.push_back(collider);
            }
        }
    }

    if (_debug)
    {
        HDC hdc = GET_SINGLE(Core)->GetMainDC();

        GDISelector pen(hdc, PenType::GREEN);
        GDISelector brush(hdc, BrushType::HOLLOW);
        RECT_RENDER(hdc, _pos.x, _pos.y, _size.x, _size.y);

        if (!_stored.empty())
        {
            GDISelector pen(hdc, PenType::RED);
            GDISelector brush(hdc, BrushType::HOLLOW);
            for (Collider* col : _stored)
            {
                Vec2 cpos = col->GetUpdatedPos();
                Vec2 csize = col->GetSize();
                RECT_RENDER(hdc, cpos.x, cpos.y, csize.x, csize.y);
            }
        }
    }

    return !_stored.empty();
}
