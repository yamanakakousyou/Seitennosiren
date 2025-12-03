#pragma once
class Game;
class Title;
class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start();
	//更新処理
	void Update();
	//描画処理
	void Render(RenderContext& rc);
public:
	SpriteRender	m_spriteRender;		//スプライトレンダー。

private:
	Game*			m_game = nullptr;
	SoundSource*	m_soundSource = nullptr;
};
