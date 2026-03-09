#pragma once
#include <functional>
class Animator;
class Texture;
struct tAnimFrame
{
    Vec2  vLT;        // ??? ?? ????
    Vec2  vSlice;     // ??? ???(??, ????)
    float fDuration;  // ??? ?????? ?????ð?(??)
    Vec2  vOffset;    // ???? ????(????)
};

class Animation
{
public:
    Animation();
    ~Animation();
public:
    void Update();
    void Render(HDC _hdc);

    // ??????? ???????? ?????? ????
    void Create(Texture* _tex, Vec2 _lt, Vec2 _sliceSize,
        Vec2 _step, UINT _frameCount, float _durationPerFrame);

    // 재생 파라미터 설정(Animator::Play에서 호출)
    void ConfigurePlayback(PlayMode _mode, int _loopCount,
        float _speed);

    // ? ??????/?????ð? ????
    void ResetToFirstFrame();

    void SetEventPerFlame(int evtStartFrame, int evtFrameCount, const std::function<void()>& onFrameEvent);

public:
    void SetOwner(Animator* _owner) { m_owner = _owner; }
    void SetName(const wstring& _name) { m_name = _name; }
    const wstring& GetName() const { return m_name; }

    // ???? ???
    void SetFrameOffset(int _index, Vec2 _offset);
    int  GetCurFrame() const { return m_curFrame; }
    int  GetMaxFrame() const { return (int)m_frames.size(); }
    bool IsFinished()  const { return m_finished; }

    // Flip controls
    void SetFlipX(bool _flip) { m_flipX = _flip; }
    void SetFlipY(bool _flip) { m_flipY = _flip; }
    void SetFlip(bool _flipX, bool _flipY) { m_flipX = _flipX; m_flipY = _flipY; }
    bool GetFlipX() const { return m_flipX; }
    bool GetFlipY() const { return m_flipY; }

    // Scale controls (render size)
    void SetScale(const Vec2& _scale) { m_scale = _scale; }
    void SetScale(float _uniform) { m_scale = { _uniform, _uniform }; }
    void SetScale(float _x, float _y) { m_scale = { _x, _y }; }
    const Vec2& GetScale() const { return m_scale; }

    // Finish event callback
    void SetOnFinished(const std::function<void()>& cb) { m_onFinished = cb; }
    void ClearOnFinished() { m_onFinished = nullptr; }

private:
    void AdvanceFrame();

private:
    Animator* m_owner;
    Texture* m_tex;
    std::wstring            m_name;
    std::vector<tAnimFrame> m_frames;

    // ??? ????
    int      m_curFrame;
    float    m_accTime;
    PlayMode m_mode;
    int      m_loopCount;
    float    m_speed;
    bool     m_finished;

    // Flip state
    bool     m_flipX;
    bool     m_flipY;

    // Render scale
    Vec2     m_scale;

    // Event
    std::function<void()> m_onFinished;
    
    // Per-frame event range
    int m_evtStartFrame;     // inclusive
    int m_evtEndFrame;       // inclusive
    int m_lastEventFrame;    // to ensure one-call-per-frame-enter
    bool m_evtActive;
    std::function<void()> m_onFrameEvent;
};
