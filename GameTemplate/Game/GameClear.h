#pragma once

class Game;

//ゲームクリア
class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();

	bool Start()override;
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);

private:
	SpriteRender	m_spriteRender;

	Game* m_game = nullptr;
};
