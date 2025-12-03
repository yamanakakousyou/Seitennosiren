#include "stdafx.h"
#include "Map.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Sord.h"
#include "Food.h"
#include "BackGround.h"
#include "Pouse.h"

namespace 
{
    const float MAP_SIZE = 200.0f;  // ミニマップの大きさ
    const float SCALE = 0.05f;      // 縮尺（3D→ミニマップ）
}

Map::Map() {}
Map::~Map() {}

bool Map::Start()
{
    m_playerIcon.Init("Assets/sprite/player_icon.dds", 16.0f, 16.0f);
    m_enemyIcon.Init("Assets/sprite/enemy_icon.dds", 16.0f, 16.0f);
    m_bossIcon.Init("Assets/sprite/boss_icon.dds", 16.0f, 16.0f);
    m_sordIcon.Init("Assets/sprite/sord_icon.dds", 16.0f, 16.0f);
    m_meatIcon.Init("Assets/sprite/meat_icon.dds", 16.0f, 16.0f);
    m_onigiriIcon.Init("Assets/sprite/onigiri_icon.dds", 16.0f, 16.0f);
    m_back.Init("Assets/sprite/minimap1.1.dds", MAP_SIZE, MAP_SIZE);
    m_back2.Init("Assets/sprite/minimap2.dds", MAP_SIZE, MAP_SIZE);
    m_back3.Init("Assets/sprite/minimap3.dds", MAP_SIZE, MAP_SIZE);

    // アイコンは中心ピボット（真ん中に置く前提）
    m_playerIcon.SetPivot(Vector2(0.5f, 0.5f));
    m_enemyIcon.SetPivot(Vector2(0.5f, 0.5f));
    m_bossIcon.SetPivot(Vector2(0.5f, 0.5f));
    m_sordIcon.SetPivot(Vector2(0.5f, 0.5f));
    m_meatIcon.SetPivot(Vector2(0.5f, 0.5f));
    m_onigiriIcon.SetPivot(Vector2(0.5f, 0.5f));



    // 背景ピボットは画像の扱いに合わせて調整してください（左上なら(0,0)、中央なら(0.5,0.5)）
    // 今は左上基準で扱う（既存処理に合わせる）
    m_back.SetPivot(Vector2(-0.25f, 0.3f));
    m_back2.SetPivot(Vector2(-0.1f, 0.0f));
    m_back3.SetPivot(Vector2(-0.1f, 0.0f));

    m_player = FindGO<Player>("player");
   
    m_backGround = FindGO<BackGround>("backGround");
    return true;
}

void Map::Update()
{
    if (FindGO<Pouse>("pouse")) {
        return;
    }
    m_back.Update();
    m_back2.Update();
    m_back3.Update();
}

void Map::Render(RenderContext& rc)
{
    RECT clientRect{};
    HWND hWnd = GetActiveWindow();
    if (hWnd)
        GetClientRect(hWnd, &clientRect);
    float screenWidth = (clientRect.right - clientRect.left) > 0 ? float(clientRect.right - clientRect.left) : 1920.0f;
    float screenHeight = (clientRect.bottom - clientRect.top) > 0 ? float(clientRect.bottom - clientRect.top) : 1080.0f;

    // ミニマップ表示位置（右上）
    Vector3 mapPos;
    mapPos.x = screenWidth / 2 - MAP_SIZE / 2 - 50.0f;
    mapPos.y = screenHeight / 2 - MAP_SIZE / 2 - 50.0f;
    mapPos.z = 0.0f;

    // 背景選択
    auto bg = FindGO<BackGround>("backGround");
    int stageID = bg ? bg->GetStageID() : 0;
    SpriteRender* currentBack;

    if (stageID == 0)
        currentBack = &m_back;
    else if (stageID == 1)
        currentBack = &m_back2;
    else
        currentBack = &m_back3;

    // 背景は左上固定
    currentBack->SetPosition(mapPos);
    currentBack->Update();
    currentBack->Draw(rc);

    if (!m_player) return;
    
    //ワールド座標 → ミニマップ絶対座標

    //　ステージ全体のワールド範囲
    const float WORLD_MIN_X = -500.0f;
    const float WORLD_MAX_X = 500.0f;
    const float WORLD_MIN_Z = -500.0f;
    const float WORLD_MAX_Z = 500.0f;

    const float worldWidth = WORLD_MAX_X - WORLD_MIN_X;
    const float worldHeight = WORLD_MAX_Z - WORLD_MIN_Z;

    auto ToMiniMap = [&](const Vector3 worldPos) 
        {
        Vector3 mini;
        float nx = (worldPos.x - WORLD_MIN_X) / worldWidth;   
        float ny = (worldPos.z - WORLD_MIN_Z) / worldHeight;  
        mini.x = mapPos.x + nx * MAP_SIZE;
        mini.y = mapPos.y + ny * MAP_SIZE;
        mini.z = 0.0f;
        return mini;
        };

    // 各アイコン描画

    // プレイヤー
    if (auto player = FindGO<Player>("player"))
    {
        m_playerIcon.SetPosition(ToMiniMap(player->GetPosition()));
        m_playerIcon.Update();
        m_playerIcon.Draw(rc);
    }

    // 敵
    auto enemies = FindGOs<Enemy>("enemy");

    // アイコン数を敵数に合わせる
    if (m_enemyIcons.size() < enemies.size()) 
    {
        for (size_t i = m_enemyIcons.size(); i < enemies.size(); ++i) 
        {
            auto icon = new SpriteRender();
            icon->Init("Assets/sprite/enemy_icon.dds", 16.0f, 16.0f);
            icon->SetPivot(Vector2(0.5f, 0.5f));
            m_enemyIcons.push_back(icon);
        }
    }

    for (size_t i = 0; i < enemies.size(); ++i) 
    {
        m_enemyIcons[i]->SetPosition(ToMiniMap(enemies[i]->GetPosition()));
        m_enemyIcons[i]->Update();
        m_enemyIcons[i]->Draw(rc);
    }

    //ボス
    if (auto boss = FindGO<Boss>("boss")) 
    {
        m_bossIcon.SetPosition(ToMiniMap(boss->GetPosition()));
        m_bossIcon.Update();
        m_bossIcon.Draw(rc);
    }

    //アイテム
    if (auto meat = FindGO<Food>("meat")) 
    {
        m_meatIcon.SetPosition(ToMiniMap(meat->GetPosition()));
        m_meatIcon.Update();
        m_meatIcon.Draw(rc);
    }
    if (auto onigiri = FindGO<Food>("onigiri")) 
    {
        m_onigiriIcon.SetPosition(ToMiniMap(onigiri->GetPosition()));
        m_onigiriIcon.Update();
        m_onigiriIcon.Draw(rc);
    }

    // 剣
    if (auto sord=FindGO<Sord>("item")) 
    {
        m_sordIcon.SetPosition(ToMiniMap(sord->GetPosition()));
        m_sordIcon.Update();
        m_sordIcon.Draw(rc);
    }
}