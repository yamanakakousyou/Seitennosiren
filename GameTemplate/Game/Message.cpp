#include "stdafx.h"
#include "Message.h"

Message::Message()
{
}

Message::~Message()
{
}

bool Message::Start()
{
    // ウィンドウ背景用
    m_spriteRender.Init("Assets/sprite/message_window.dds", 512.0f, 128.0f);
    m_spriteRender.SetPosition({ 600.0f, 900.0f, 0.0f });
    m_fontRender.SetPosition({ 700.0f,-300.0f,0.0f });
    //m_spriteRender.SetMulColor({ 0.0f, 0.0f, 0.0f, 0.5f }); // 半透明

    return true;
}

void Message::AddMessage(const std::string& text)
{
    //テキストを入れる
    m_currentMessage = text;
}

void Message::NextMessage()
{

}

void Message::Update()
{
    m_spriteRender.Update();
}

void Message::Render(RenderContext& rc)
{
    // 背景を描画
    m_spriteRender.Draw(rc);

// std::string を std::wstring に変換
    std::wstring wtext(m_currentMessage.begin(), m_currentMessage.end());

    // 文字列をセット
    m_fontRender.SetText(wtext.c_str());

    // 色を設定
    m_fontRender.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    // ここで文字列をセット
    m_fontRender.Draw(rc);
}