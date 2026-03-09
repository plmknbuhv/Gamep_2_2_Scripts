#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Object.h"
#include "Texture.h"

Animation::Animation()
    : m_owner(nullptr)
    , m_tex(nullptr)
    , m_curFrame(0)
    , m_accTime(0.f)
    , m_mode{}
    , m_loopCount(1)
    , m_speed(1.f)
    , m_finished(false)
    , m_flipX(false)
    , m_flipY(false)
    , m_scale{1.f, 1.f}
    , m_evtStartFrame(0)
    , m_evtEndFrame(INT_MIN)
    , m_lastEventFrame(INT_MIN)
    , m_evtActive(false)
{

}

Animation::~Animation()
{

}

void Animation::Create(Texture* _tex, Vec2 _lt, Vec2 _sliceSize,
    Vec2 _step, UINT _frameCount, float _durationPerFrame)
{
    m_tex = _tex;
    if (!_tex || _frameCount <= 0 || _durationPerFrame <= 0.f)
        return;
    m_frames.clear();
    m_frames.reserve(_frameCount);
    for (UINT i = 0; i < _frameCount; ++i)
    {
        // ??????
        tAnimFrame _fr;
        _fr.vLT = _lt + _step * i;
        _fr.vSlice = _sliceSize;
        _fr.fDuration = _durationPerFrame;
        _fr.vOffset = { 0.f, 0.f };
        m_frames.push_back(_fr);
    }
}

void Animation::ConfigurePlayback(PlayMode _mode, int _loopCount, float _speed)
{
    m_mode = _mode;
    m_loopCount = (_mode == PlayMode::Counted) ? max(1, _loopCount) : 1;
    m_speed = _speed;
    m_finished = false;
    m_lastEventFrame = -1;
}

void Animation::ResetToFirstFrame()
{
    m_curFrame = 0;
    m_accTime = 0.f;
    m_finished = false;
    m_lastEventFrame = INT_MIN;
}

void Animation::SetEventPerFlame(int evtStartFrame, int evtFrameCount, const std::function<void()>& onFrameEvent)
{
    if (evtFrameCount <= 0 || evtStartFrame < 0 || m_frames.empty())
    {
        m_evtActive = false;
        m_onFrameEvent = nullptr;
        m_evtStartFrame = 0;
        m_evtEndFrame = INT_MIN;
        m_lastEventFrame = INT_MIN;
        return;
    }

    int maxFrame = (int)m_frames.size();
    m_evtStartFrame = min(max(0, evtStartFrame), maxFrame - 1);
    m_evtEndFrame = min(m_evtStartFrame + evtFrameCount - 1, maxFrame - 1);
    m_onFrameEvent = onFrameEvent;
    m_evtActive = (m_onFrameEvent != nullptr);
    m_lastEventFrame = INT_MIN;
}

void Animation::SetFrameOffset(int _index, Vec2 _offset)
{
    if (_index < 0 || _index >= (int)m_frames.size())
        return;
    m_frames[(size_t)_index].vOffset = _offset;
}



void Animation::Update()
{
    if (!m_owner || !m_tex || m_frames.empty() || m_finished)
        return;
    AdvanceFrame();
}

void Animation::AdvanceFrame()
{
    const tAnimFrame& _fr = m_frames[(size_t)m_curFrame];
    
    bool shouldFireNow = m_evtActive && (bool)m_onFrameEvent &&
        (m_curFrame >= m_evtStartFrame) && (m_curFrame <= m_evtEndFrame) &&
        (m_lastEventFrame != m_curFrame);
    if (shouldFireNow)
    {
        m_lastEventFrame = m_curFrame;
        m_onFrameEvent();
    }

    m_accTime += fDT * m_speed;

    if (m_accTime >= _fr.fDuration)
    {
        m_accTime -= _fr.fDuration;
        ++m_curFrame;

        if (m_curFrame >= (int)m_frames.size())
        {
            bool justFinished = false;
            switch (m_mode)
            {
            case PlayMode::Once:
                m_curFrame = (int)m_frames.size() - 1;
                m_finished = true;
                justFinished = true;
                break;
            case PlayMode::Loop:
                m_curFrame = 0;
                break;
            case PlayMode::Counted:
            {
                --m_loopCount;
                if (m_loopCount > 0)
                    m_curFrame = 0;
                else
                {
                    m_curFrame = (int)m_frames.size() - 1;
                    m_finished = true;
                    justFinished = true;
                }
            }
            break;
            }

            if (justFinished && m_onFinished)
            {
                // Capture callback then clear to ensure single fire and avoid reentrancy issues if callback triggers Play
                auto cb = m_onFinished;
                m_onFinished = nullptr;
                cb();
            }
        }
        
        if (!m_finished)
        {
            bool shouldFireAfterAdvance = m_evtActive && (bool)m_onFrameEvent &&
                (m_curFrame >= m_evtStartFrame) && (m_curFrame <= m_evtEndFrame) &&
                (m_lastEventFrame != m_curFrame);
            if (shouldFireAfterAdvance)
            {
                m_lastEventFrame = m_curFrame;
                m_onFrameEvent();
            }
        }
    }
}

void Animation::Render(HDC _hdc)
{
    if (!m_owner || !m_tex || m_frames.empty())
        return;

    Object* obj = m_owner->GetOwner();
    Vec2 pos = obj->GetPos();

    const tAnimFrame& fr = m_frames[(size_t)m_curFrame];
    pos = pos + fr.vOffset;

    // Destination (screen) rectangle (centered), apply scale
    int dw = (int)(fr.vSlice.x * m_scale.x);
    int dh = (int)(fr.vSlice.y * m_scale.y);
    int dx = (int)(pos.x - dw / 2);
    int dy = (int)(pos.y - dh / 2);

    // ???(???) ????
    int sx = (int)fr.vLT.x;
    int sy = (int)fr.vLT.y;
    int sw = (int)fr.vSlice.x;
    int sh = (int)fr.vSlice.y;

    // Validate sizes
    if (dw <= 0 || dh <= 0 || sw <= 0 || sh <= 0)
    {
        std::cout << "[Error] Animation.cpp Render: size is zero or negative"
                  << " dw=" << dw << " dh=" << dh
                  << " sw=" << sw << " sh=" << sh << "\n";
        return;
    }

    HDC srcDC = m_tex->GetTextureDC();

    if (m_flipX || m_flipY)
    {
        // Render flipped to an intermediate compatible bitmap, then TransparentBlt it.
        HDC memDC = CreateCompatibleDC(_hdc);
        if (!memDC)
        {
            std::cout << "[Error] Animation.cpp Render: CreateCompatibleDC failed: " << GetLastError() << "\n";
            return;
        }
        HBITMAP memBmp = CreateCompatibleBitmap(_hdc, sw, sh);
        if (!memBmp)
        {
            std::cout << "[Error] Animation.cpp Render: CreateCompatibleBitmap failed: " << GetLastError() << "\n";
            DeleteDC(memDC);
            return;
        }

        HGDIOBJ oldBmp = SelectObject(memDC, memBmp);

        // Fill with colorkey color (magenta) to preserve transparency
        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
        RECT r{ 0, 0, sw, sh };
        FillRect(memDC, &r, brush);
        DeleteObject(brush);

        // Set stretch mode
        SetStretchBltMode(memDC, COLORONCOLOR);

        // Compute destination rect in temp DC with flipping via negative extents
        // Use sw/sh (not sw-1/sh-1) to avoid 1px offset when mapping with negative width/height.
        int ddx = m_flipX ? sw : 0;
        int ddy = m_flipY ? sh : 0;
        int ddw = m_flipX ? -sw : sw;
        int ddh = m_flipY ? -sh : sh;

        BOOL sret = StretchBlt(memDC, ddx, ddy, ddw, ddh,
                               srcDC, sx, sy, sw, sh, SRCCOPY);
        if (!sret)
        {
            std::cout << "[Error] Animation.cpp Render (StretchBlt flip) failed: " << GetLastError() << "\n";
        }

        BOOL tret = TransparentBlt(_hdc,
                                   dx, dy, dw, dh,
                                   memDC,
                                   0, 0, sw, sh,
                                   RGB(255, 0, 255));

        if (!tret)
        {
            std::cout << "[Error] Animation.cpp Render (TransparentBlt flipped) Error code: " << GetLastError() << "\n";
        }

        // Cleanup
        SelectObject(memDC, oldBmp);
        DeleteObject(memBmp);
        DeleteDC(memDC);
    }
    else
    {
        BOOL debug = TransparentBlt(_hdc,
            dx, dy, dw, dh,
            srcDC,
            sx, sy, sw, sh,
            RGB(255, 0, 255));
        if (debug == 0)
        {
            std::cout << "[Error] Animation.cpp Render (TransparentBlt) Error code: " << GetLastError() << "\n";
        }
    }
}
