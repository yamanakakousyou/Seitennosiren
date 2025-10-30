#pragma once
#include <string>
#include <vector>
#include <chrono>

struct MessageData
{
    std::string text;                                   //文字をだす
    float duration;                                     // 表示時間（秒）
    std::chrono::steady_clock::time_point startTime;
};

class Message : public IGameObject
{
public:
    Message();
    ~Message();

    bool Start();
    void Update();
    void Render(RenderContext& rc);
    void AddMessage(const std::string& text);

    void NextMessage();

private:
    SpriteRender m_spriteRender; // 背景
    FontRender m_fontRender;     // テキスト
    std::string m_currentMessage;
    float m_sdRenderTimer = 0.0f;
};