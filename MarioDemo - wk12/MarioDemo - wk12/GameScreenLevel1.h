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
#include "CharacterCoin.h"
#include "SoundEffect.h"
#include "TTF_Text.h"

class Texture2D;
class Character;
class PowBlock;

class GameScreenLevel1 :
	public GameScreen {
private:
	Texture2D*				mBackgroundTexture;
	SCREENS					mScreen;
	TTF_Text				mMarioFont;

	CharacterMario*			marioCharacter;
	CharacterLuigi*			luigiCharacter;
	unsigned int			mMarioScore;
	unsigned int			mLuigiScore;

	vector< pair<char, Character*> >		mEnemies;
	float					mSpawnTimer;
	int						mSpawnCounter;

	vector<CharacterCoin*>	mCoins;

	LevelMap*				mLevelMap;
	PowBlock*				mPowBlock;
	float					mPowTimer;

	std::unordered_map<string, SoundEffect*> mSounds;

	bool					mScreenshake;
	float					mScreenshakeTime;
	float					mWobble;
	float					mBackgroundYPos;

	void DoScreenshake();
	void SetLevelMap();
	bool SetUpLevel();
	void SetUpSFX();
	void UpdatePOWBlock(float deltaTime);

	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateGoomba(Vector2D position, FACING direction, float speed);
	void CreateCoins(Vector2D position, int value);
	void UpdateCoins(float deltaTime, SDL_Event e);

public:
	GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen);
	~GameScreenLevel1();


	void Render();
	void Update(float deltaTime, SDL_Event e);

	SCREENS GetScreenID() { return mScreen; }
};

#endif // !GAMESCREENLEVEL1_H