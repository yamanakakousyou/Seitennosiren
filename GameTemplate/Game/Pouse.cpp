#include "stdafx.h"
#include "Pouse.h"
#include "Game.h"
#include "Title.h"

Pouse::Pouse()
{

}

Pouse::~Pouse()
{

}

bool Pouse::Start()
{
    // ポーズ画面のスプライト初期化
    m_spriteRender.Init("Assets/sprite/pouse.dds", 1920.0f, 1080.0f);
    m_spriteRender.SetPosition({ 960.0f, 540.0f, 0.0f });
    return true;
}

void Pouse::Update()
{
    if (!g_pad[0]) return;

    // STARTでポーズ解除
    if (g_pad[0]->IsTrigger(enButtonStart)) {
        DeleteGO(this);
        return;
    }

    // Aボタンでタイトルに戻る
    if (g_pad[0]->IsTrigger(enButtonA)) {
        // ゲーム本体を破棄
        m_game = FindGO<Game>("game");
        if (m_game) DeleteGO(m_game);

        // タイトルを生成
        NewGO<Title>(0, "title");

        // 自分(Pouse)を破棄
        DeleteGO(this);
        return;
    }
}

void Pouse::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
}