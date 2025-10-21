#pragma once
class Game;
class Title;
class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start();
	//�X�V����
	void Update();
	//�`�揈��
	void Render(RenderContext& rc);

private:
	SpriteRender	m_spriteRender;		//�X�v���C�g�����_�[�B

	Game* m_game = nullptr;
	SoundSource* m_soundSource = nullptr;
};
