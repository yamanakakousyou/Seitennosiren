#include "stdafx.h"
#include "Pouse.h"
#include "Game.h"
#include "Title.h"
#include "Player.h"
#include "Enemy.h"
#include "Sord.h"
#include "Food.h"
#include "Boss.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Message.h"

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
    if (g_pad[0]->IsTrigger(enButtonStart)) 
    {
        DeleteGO(this);
        return;
    }

    // Aボタンでタイトルに戻る
    if (g_pad[0]->IsTrigger(enButtonA)) 
    {

        DeleteGO(FindGO<Player>("player"));
        DeleteGO(FindGO<Enemy>("enemy"));
        DeleteGO(FindGO<Sord>("item"));
        DeleteGO(FindGO<Food>("meat"));
        DeleteGO(FindGO<Food>("onigiri"));
        DeleteGO(FindGO<BackGround>("backGround"));
        DeleteGO(FindGO<GameCamera>("gamecamera"));
        DeleteGO(FindGO<Message>("message"));
        DeleteGO(FindGO<Map>("map"));

        // 最後にGameを削除
        DeleteGO(FindGO<Game>("game"));

        // タイトルを生成
        NewGO<Title>(0, "title");

        // 自分(Pouse)も削除
        DeleteGO(this);
        return;
    }
}

void Pouse::Render(RenderContext& rc)
{
    m_spriteRender.Draw(rc);
}