#ifndef GAMESCREENLEVEL1_H
#define GAMESCREENLEVEL1_H

/* STD lib */
#include <vector>
#include <unordered_map>

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "LevelMap.h"
#include "CharacterKoopa.h"
#include "CharacterGoomba.h"
#include "CharacterSpiny.h"
#include "CharacterCoin.h"
#include "SoundEffect.h"
#include "TTF_Text.h"

class Texture2D;
class Character;
class PowBlock;
class GameScreenManager;

class GameScreenLevel1 :
	public GameScreen {
private:
	Texture2D*				mBackgroundTexture;
	SCREENS					mScreen;
	TTF_Text				mMarioFont;
	Mix_Music*				mMusic;

	bool					mIsGameOver;
	float					mGameOverTimer;
	float					mGameCounter;

	CharacterMario*			marioCharacter;
	CharacterLuigi*			luigiCharacter;
	unsigned int			mMarioScore, mMarioLives;
	unsigned int			mLuigiScore, mLuigiLives;
	Vector2D				mSpawnMario;
	Vector2D				mSpawnLuigi;

	vector< pair<char, Character*> >		mEnemies;
	float					mSpawnTimer;
	int						mSpawnCounter, mSpawnType;
	Vector2D				mSpawnLeft, mSpawnRight;
	FACING					mSpawnDir;

	vector<CharacterCoin*>	mCoins;

	LevelMap*				mLevelMap;
	PowBlock*				mPowBlock;
	float					mPowTimer;

	std::unordered_map<string, SoundEffect*> mSounds;

	bool					mScreenshake;
	float					mScreenshakeTime;
	float					mWobble;
	float					mBackgroundYPos;

	void SetLevelMap();
	bool SetUpLevel();
	void SetUpSFX();
	void SetUpMusic();

	void DoScreenshake();
	void LoadMusic(string musicPath);

	void CreateCoins(Vector2D position, int value);
	void CreateGoomba(Vector2D position, FACING direction, float speed);
	void CreateKoopa_G(Vector2D position, FACING direction, float speed);
	void CreateKoopa_R(Vector2D position, FACING direction, float speed);
	void CreateSpiny(Vector2D position, FACING direction, float speed);

	void CleanUp();

	void UpdateCoins(float deltaTime, SDL_Event e);
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void UpdateHighScore();
	void UpdatePOWBlock(float deltaTime);
	void UpdateScreenShake(float deltaTime, SDL_Event e);

public:
	GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager);
	~GameScreenLevel1();

	void Render() override;
	void RenderHUD();
	void Update(float deltaTime, SDL_Event e) override;

	SCREENS GetScreenID() { return mScreen; }
};

#endif // !GAMESCREENLEVEL1_H