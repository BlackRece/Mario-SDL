#ifndef GAMESCREENLEVEL2_H
#define GAMESCREENLEVEL2_H

/* STD lib */
#include <unordered_map>
#include <vector>

/* SDL lib */
#include <SDL.h>

/* usr lib */
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include "CharacterCoin.h"
#include "CharacterKoopa.h"
#include "CharacterSpiny.h"
#include "CharacterLuigi.h"
#include "CharacterMario.h"
#include "SoundEffect.h"
#include "TTF_Text.h"

class GameScreenManager;
class PowBlock;

class GameScreenLevel2 :
	public GameScreen {
private:
	SCREENS					mScreen;
	TTF_Text				mLuigiFont;
	Mix_Music*				mMusic;
	bool					mIsGameOver;
	bool					mIsHighScore;
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
	Vector2D				mSpawnPos;
	FACING					mSpawnDir;

	vector<CharacterCoin*>	mCoins;

	LevelMap*				mLevelMap;
	float					mLevelOffset, mLevelBGOffset, mLevelFGOffset;
	float					mSpeedOffset;
	PowBlock*				mPowBlock;
	float					mPowTimer;

	std::unordered_map<string, SoundEffect*> mSounds;

	bool					mScreenshake;
	float					mScreenshakeTime;
	float					mWobble;
	float					mBackgroundYPos;

	void CleanUp();

	void CreateCoins(Vector2D position, int value);
	void CreateKoopa_G(Vector2D position, FACING direction, float speed);
	void CreateKoopa_R(Vector2D position, FACING direction, float speed);
	void CreateSpiny(Vector2D position, FACING direction, float speed);

	void DoScreenshake();
	void LoadMusic(string musicPath);

	void SetLevelMap();
	bool SetUpLevel();
	void SetUpSFX();
	void SetUpMusic();

	void UpdateCoins(float deltaTime, SDL_Event e);
	void UpdateEnemies(float deltaTime, SDL_Event e);
	bool UpdateHighScore();
	void UpdatePOWBlock(float deltaTime);
	void UpdateScreenShake(float deltaTime, SDL_Event e);

public:
	GameScreenLevel2(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager);
	~GameScreenLevel2();

	void Render() override;
	void RenderHighScore();
	void RenderHUD();
	void Update(float deltaTime, SDL_Event e) override;

	SCREENS GetScreenID() { return mScreen; }
};

#endif // !GAMESCREENLEVEL2_H