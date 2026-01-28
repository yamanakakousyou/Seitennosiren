#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "Boss.h"
#include "GameOver.h"
#include "Food.h"
#include "Message.h"
#include "Pouse.h"

namespace 
{
	const int PLAYERHP = 20;
	const int PLAYERMAXHP = 20;
	const int SATIETY = 100;
}

bool Player::Start()
{
	//アニメーションクリップをロードする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Playeridle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/PlayerWalk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(false);
	animationClips[enAnimationClip_Attack].Load("Assets/animData/PlayerAttack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	//モデルを初期化する
	modelRender.Init("Assets/modelData/Player.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//キャラコンを初期化する
	//m_position = Vector3(0.0f, 0.0f, 0.0f);

	m_characterController.Init(25.0f, 75.0f, m_position);

	modelRender.SetScale(0.45f, 0.45f, 0.45f);

	m_rotation.SetRotationDegY(0);
	modelRender.SetRotation(m_rotation);
	m_game = FindGO<Game>("game");
	m_enemy = FindGO<Enemy>("enemy");
	m_boss = FindGO<Boss>("boss");
	m_message = FindGO<Message>("message");

	//入れる
	m_PlayerHP = PLAYERHP;
	m_PlayerMaxHP = PLAYERMAXHP;
	m_satiety = SATIETY;

	m_spriteRender.Init("Assets/sprite/HP.DDS", 1024, 128);
	m_spriteRender.SetPosition({ -200.0f, 500.0f, 0.0f });
	m_spriteRender.SetScale({ 0.5f, 0.5f, 0.5f });
	m_spriteRender.SetPivot(Vector2(0.0f, 0.5f));

	m_satietyRender.Init("Assets/sprite/Satiety.DDS", 1024, 128);
	m_satietyRender.SetPosition({ -200.0f,430.0f,0.0f });
	m_satietyRender.SetScale({ 0.5f,0.5f,0.5f });
	m_satietyRender.SetPivot(Vector2(0.0f, 0.5f));

	m_state = PlayerState::Idle;
	m_attackTimer = 0.0f;
	m_currentAnim = -1;

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/Attack.wav");	//音の読み込み。
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/Damage.wav");	//音の読み込み。
	return true;
}

Player::Player()
{

}

Player::~Player()
{
	DeleteGO(m_attackSE);
}

void Player::Update()
{
	if (FindGO<Pouse>("pouse")) 
	{
		return;
	}

	float deltaTime = g_gameTime->GetFrameDeltaTime();

	// 攻撃中タイマー更新
	if (m_state == PlayerState::Attack)
	{
		m_attackTimer -= deltaTime;
		if (m_attackTimer <= 0.0f)
		{
			m_state = PlayerState::Idle;
			modelRender.PlayAnimation(enAnimationClip_Idle);
		}
	}

	//移動タイマー更新
	if (m_state == PlayerState::Walk)
	{
		m_moveTimer -= deltaTime;
		if (m_moveTimer <= 0.0f)
		{
			m_state = PlayerState::Idle;
			modelRender.PlayAnimation(enAnimationClip_Idle);
		}
	}
	PlayerMoveTurn();
	ManageState();
	PlayAnimation();
	modelRender.Update();
	m_spriteRender.Update();
	m_satietyRender.Update();
	PlayerUI();
	PlayerTakeDamage(dmg);
	Invebtory();
	
}

void Player::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		m_rotation.SetRotationYFromDirectionXZ(moveSpeed);
		//絵描きさんに回転を教える。
		modelRender.SetRotation(m_rotation);
	}
}

void Player::ManageState()
{
	////xかzの移動速度があったら(スティックの入力があったら)。
	//if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	//{

	//	playerState = 1;

	//	modelRender.PlayAnimation(enAnimationClip_Walk);
	//}
	//else
	//{

	//	//ステートを0(待機)にする。
	//	playerState = 0;
	//	modelRender.PlayAnimation(enAnimationClip_Idle);
	//}
}

void Player::PlayAnimation()
{
	//switch (playerState)
	//{
	//	//プレイヤーステートが0(待機)だったら。
	//case 0:
	//	//待機アニメーションを再生する。
	//	modelRender.PlayAnimation(enAnimationClip_Idle);
	//	break;
	//}
}

void Player::PlayerMoveTurn()
{
	if (m_isInventoryOpen) return;

	static TurnType lastTurn = TurnType::Player;
	if (m_game->GetCurrentTurn() != lastTurn) 
	{
		HasMoved = false;
	}

	lastTurn = m_game->GetCurrentTurn();

	if (m_game->GetCurrentTurn() != TurnType::Player)
		return;

	if (HasMoved) return;

	Vector3 moveDir = Vector3::Zero;

	static bool wasPressed = false;
	bool isPressed = false;

	//コントローラーでの十字キー移動
	if (g_pad[0]->IsTrigger(enButtonUp) ) { moveDir.z += 1.0f; isPressed = true; }
	if (g_pad[0]->IsTrigger(enButtonDown)) { moveDir.z -= 1.0f; isPressed = true; }
	if (g_pad[0]->IsTrigger(enButtonRight)) { moveDir.x += 1.0f; isPressed = true; }
	if (g_pad[0]->IsTrigger(enButtonLeft)) { moveDir.x -= 1.0f; isPressed = true; }


	////キーボードでのWASD移動
	if (GetAsyncKeyState('W') & 0x8000) { moveDir.z += 1.0f; isPressed = true; }
	if (GetAsyncKeyState('S') & 0x8000) { moveDir.z -= 1.0f; isPressed = true; }
	if (GetAsyncKeyState('D') & 0x8000) { moveDir.x += 1.0f; isPressed = true; }
	if (GetAsyncKeyState('A') & 0x8000) { moveDir.x -= 1.0f; isPressed = true; }


	if (!wasPressed && isPressed && moveDir.Length() > 0.0f) 
	{
		moveDir.Normalize();
		float step = 1000.0f;

		Vector3 moveSpeed = moveDir * step;
		m_position = m_characterController.Execute(moveSpeed, 1.0f / 60.0f);
		modelRender.SetPosition(m_position);

		float angle = atan2f(moveDir.x, moveDir.z);
		Quaternion rot;
		rot.SetRotationY(angle);
		m_rotation = rot;
		modelRender.SetRotation(rot);

		// すでに攻撃中なら無視
		if (m_state == PlayerState::Attack) return;

		// 攻撃ステートに移行
		m_state = PlayerState::Walk;
		m_moveTimer = 0.08f; // 攻撃アニメ時間（秒）
		m_currentAnim = -1;   // アニメリセット
		modelRender.PlayAnimation(enAnimationClip_Walk);

		m_satiety -= 1;
		if (m_satiety < 0) 
		{
			m_satiety = 0;
			// 満腹度0ならHPが減る
			m_PlayerHP -= 1;
			if (m_PlayerHP < 0) m_PlayerHP = 0;
		}

		HasMoved = true;
		m_game->NextTurn();
	}

	wasPressed = isPressed; // 状態更新

	// 攻撃ボタン
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		// すでに攻撃中なら無視
		if (m_state == PlayerState::Attack) return;

		// 攻撃ステートに移行
		m_state = PlayerState::Attack;
		m_attackTimer = 1.0f; // 攻撃アニメ時間（秒）
		m_currentAnim = -1;   // アニメリセット
		modelRender.PlayAnimation(enAnimationClip_Attack);

		//m_attackSE->Play(false);

		auto se = NewGO<SoundSource>(0);
		se->Init(6);       // ← WaveFileBank の番号
		se->Play(false);

		// 攻撃処理
		PlayerAttack();

		m_satiety -= 1;
		if (m_satiety < 0) 
		{
			m_satiety = 0;
			m_PlayerHP -= 1;
			if (m_PlayerHP < 0) m_PlayerHP = 0;
		}

		HasMoved = true;
		m_game->NextTurn();
	}
}

void Player::PlayerUI()
{
	// HP表示用のバッファを用意
	wchar_t hpText[32];
	swprintf_s(hpText, sizeof(hpText) / sizeof(wchar_t), L"HP%d/20", m_PlayerHP);

	m_fontRender.SetText(hpText); // ここはSetText等、実際のテキスト設定関数に合わせてください
	m_fontRender.SetPosition({ -600.0f, 530.0f, 0.0f });
	m_fontRender.SetScale(1.5);
	m_fontRender.SetColor(g_vec4Black);

	float hpRate = (float)m_PlayerHP / 20.0f; // 最大HPを20として割合計算
	m_spriteRender.SetScale({ 0.5f * hpRate, 0.5f, 0.5f });



	// 満腹度テキスト
	wchar_t satietyText[32];
	swprintf_s(satietyText, sizeof(satietyText) / sizeof(wchar_t), L"満腹度 %d/100", m_satiety);
	m_satietyFontRender.SetText(satietyText);
	m_satietyFontRender.SetPosition({ -650.0f, 450.0f, 0.0f }); // 少し下
	m_satietyFontRender.SetScale(1.0);
	m_satietyFontRender.SetColor(g_vec4Black);

	// 満腹度バーの割合
	float satietyRate = (float)m_satiety / 100.0f; // 0～100%
	m_satietyRender.SetScale({ 0.5f * satietyRate, 0.5f, 0.5f });
}

void Player::PlayerAttack()
{
	float attackRange = 70.0f;   // 攻撃距離
	float attackAngle = 100.0f;  // 扇状の攻撃角度

	// プレイヤーの向きベクトルを取得
	Vector3 forward(0, 0, 1);
	m_rotation.Apply(forward); // 回転を適用して「今向いてる方向」にする
	forward.Normalize();

	for (Enemy* enemy : m_game->GetEnemies())  // ← ここ重要
	{
		if (!enemy) continue;

		Vector3 toEnemy = enemy->GetPosition() - m_position;
		float dist = toEnemy.Length();
		toEnemy.Normalize();

		if (dist <= attackRange)
		{
			float dot = forward.Dot(toEnemy);
			float angle = acosf(dot) * 180.0f / 3.14159f;

			if (angle < attackAngle)
			{
				int roll = rand() % 100;
				if (roll < 90)
				{
					enemy->EnemyTakeDamage(m_attackPower);
					m_message->AddMessage("EnemyDamage");
				}

				else
				{
					m_message->AddMessage("Miss");
				}
			}
		}
	}

	// ボスへの攻撃判定
	if (m_boss)
	{
		Vector3 toBoss = m_boss->GetPosition() - m_position;
		float distBoss = toBoss.Length();
		toBoss.Normalize();

		if (distBoss <= attackRange)
		{
			float dotB = forward.Dot(toBoss);
			float angleB = acosf(dotB) * 180.0f / 3.14159f;

			if (angleB < attackAngle)
			{
				int roll = rand() % 100;
				if (roll < 90)
				{
					m_boss->BossTakeDamage(m_attackPower);
					m_message->AddMessage("BossDamage");
				}

				else
				{
					m_message->AddMessage("Miss");
				}
			}
		}
	}
}

void Player::PlayerHeal(int recovery)
{
	m_PlayerHP += recovery;

	if (m_PlayerHP > m_PlayerMaxHP) 
	{
		m_PlayerHP = m_PlayerMaxHP; // 最大HPを超えないように
	}
}

void Player::PlayerEat(int amount)
{
	m_satiety += amount;
	if (m_satiety > 100) m_satiety = 100; // 最大値制限
}

void Player::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
}

void Player::SetBoss(Boss* boss)
{
	m_boss = boss;
}

void Player::AttackPower(int value)
{
	m_attackPower += value;
}


void Player::Invebtory()
{
	// 前フレームのキー状態を保持
	static bool prevX = false;
	static bool prevZ = false;
	static bool prevC = false;

	// 入力状態取得
	bool padSelect = (g_pad[0] && g_pad[0]->IsTrigger(enButtonSelect));
	bool padUseLeft = (g_pad[0] && g_pad[0]->IsTrigger(enButtonLB1));
	bool padUseRight = (g_pad[0] && g_pad[0]->IsTrigger(enButtonRB1));

	bool kbX = (GetAsyncKeyState('X') & 0x8000) != 0;
	bool kbZ = (GetAsyncKeyState('Z') & 0x8000) != 0;
	bool kbC = (GetAsyncKeyState('C') & 0x8000) != 0;

	//十字キー入力
	bool padUp = (g_pad[0] && g_pad[0]->IsTrigger(enButtonUp));
	bool padDown = (g_pad[0] && g_pad[0]->IsTrigger(enButtonDown));
	bool padA = (g_pad[0] && g_pad[0]->IsTrigger(enButtonA));

	// キーボード版
	bool isTriggerX = kbX && !prevX;

	// --- インベントリ開閉 ---
	if (padSelect || isTriggerX) 
	{
		m_isInventoryOpen = !m_isInventoryOpen;
		m_inventoryCursor = 0;
	}

	// --- フレーム終了時に更新 ---
	prevX = kbX;

	// 開いているときのみアイテム使用
	if (m_isInventoryOpen) 
	{

		const int maxCursor = 2;  // 1番, 2番, その他 → 常に2

		// 上移動
		if (padUp) 
		{
				m_inventoryCursor--;
			if (m_inventoryCursor < 0)
				m_inventoryCursor = 0;
		}

    // 下移動
    if (padDown) 
	{
        m_inventoryCursor++;
        if (m_inventoryCursor > maxCursor)
            m_inventoryCursor = maxCursor;
    }

    // Aボタン
    if (padA) 
	{

        if (m_inventoryCursor <= 1 && m_inventoryCursor < m_inventory.size()) 
		{
            // アイテム使用
            UseItem(m_inventoryCursor);
        }
        else if (m_inventoryCursor == 2) 
		{
            // その他 → ポーズ画面
            if (!FindGO<Pouse>("pouse")) 
			{
                NewGO<Pouse>(0, "pouse");
            }
            return;
        }
    }
}

}

void Player::AddItem(ItemType type)
{
	InventoryItem item;

	//肉
	if (type == ItemType::Meat)
	{
		item.type = ItemType::Meat;
		item.name = "Meat";
	}
	//おにぎり
	else if(type==ItemType::Onigiri)
	{
		item.type = ItemType::Onigiri;
		item.name = "Onigiri";
	}

	// インベントリに追加
	m_inventory.push_back(item);
}

//アイテムを使う
void Player::UseItem(int index)
{
	// 範囲外アクセス防止
	if (index < 0 || index >= m_inventory.size()) return;

	auto& item = m_inventory[index];

	// アイテムの種類に応じて効果を発動
	if (item.type == ItemType::Meat) 
	{
		PlayerHeal(2);      // HPを2回復
	}
	else if (item.type == ItemType::Onigiri) 
	{
		PlayerEat(10);      // 満腹度を10回復
	}

	// 使ったアイテムは削除
	m_inventory.erase(m_inventory.begin() + index);
}


void Player::DrawInventory(RenderContext& rc)
{	
	if (!m_isInventoryOpen) {
		m_itemFontRender.SetText(L"");
		return;
	}

	std::wstring allText;

	// 1番アイテム（ある場合）
	if (m_inventory.size() > 0)
	{
		allText += (m_inventoryCursor == 0 ? L"> " : L"  ");
		allText += L"1: " + std::wstring(m_inventory[0].name.begin(), m_inventory[0].name.end()) + L"\n";
	}
	else
	{
		allText += (m_inventoryCursor == 0 ? L"> " : L"  ");
		allText += L"1: ----\n";
	}

	// 2番アイテム（ある場合）
	if (m_inventory.size() > 1)
	{
		allText += (m_inventoryCursor == 1 ? L"> " : L"  ");
		allText += L"2: " + std::wstring(m_inventory[1].name.begin(), m_inventory[1].name.end()) + L"\n";
	}
	else
	{
		allText += (m_inventoryCursor == 1 ? L"> " : L"  ");
		allText += L"2: ----\n";
	}

	// 3番：その他（必ず出す）
	allText += (m_inventoryCursor == 2 ? L"> " : L"  ");
	allText += L"3: その他";

	m_itemFontRender.SetText(allText.c_str());
	m_itemFontRender.SetPosition({ -600.0f, 0.0f, 0.0f });
	m_itemFontRender.SetScale(1.2f);
	m_itemFontRender.SetColor(g_vec4Black);
}

void Player::PlayerTakeDamage(int dmg)
{
	m_PlayerHP -= dmg;

	if (m_PlayerHP <= 0) 
	{
		m_PlayerHP = 0;

		//ゲームオーバーを表示する
		NewGO<GameOver>(0, "gameover");
		DeleteGO(this);
		DeleteGO(m_game);
	}

	return;
}

void Player::Render(RenderContext& renderContext)
{
	modelRender.Draw(renderContext);
	m_spriteRender.Draw(renderContext);
	m_pouseFontRender.Draw(renderContext);
	m_fontRender.Draw(renderContext);
	m_itemFontRender.Draw(renderContext);
	m_satietyRender.Draw(renderContext);
	m_satietyFontRender.Draw(renderContext);
	DrawInventory(renderContext);
}