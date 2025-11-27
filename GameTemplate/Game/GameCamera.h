#pragma once
class Player;
class Pouse;
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	//更新処理
	void Update();

	//注視点から視点に向かうベクトル。
	Vector3 toCameraPos;
	//プレイヤー
	Player* player;
	//ステート
	int state = 0;
};