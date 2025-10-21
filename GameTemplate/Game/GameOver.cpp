#include "stdafx.h"
#include "GameOver.h"
#include "Title.h"
#include "Game.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	//ゲームオーバーの画像を読み込む
	m_spriteRender.Init("Assets/sprite/Gameover.dds", 1920.0f, 1080.0f);
	//m_game = FindGO<Game>("game");
	return true;
}

void GameOver::Update()
{
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//タイトルのオブジェクトをつくる
		NewGO<Title>(0, "title");
		////自身を削除する
		DeleteGO(this);
		DeleteGO(m_game);
	}
}

//描画処理
void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
