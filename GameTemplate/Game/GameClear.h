#pragma once

class Game;

//�Q�[���N���A
class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();

	bool Start()override;
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);

private:
	SpriteRender	m_spriteRender;

	Game* m_game = nullptr;
};
