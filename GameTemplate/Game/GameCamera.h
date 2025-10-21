#pragma once
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	//更新処理
	void Update();

	//プレイヤー
	Player* player;
	//ステート
	int state = 0;
	//注視点から視点に向かうベクトル。
	Vector3 toCameraPos;
};