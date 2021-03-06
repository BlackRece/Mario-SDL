#include "GameScreenLevel1.h"

#include <iostream>
#include "Texture2D.h"
#include "PowBlock.h"
#include "Constants.h"
#include "Collisions.h"
#include "CharacterGoomba.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	
	mLevelMap = nullptr;
	mPowTimer = BLOCK_TIME;
	
	mEnemies.clear();
	mSpawnTimer = 0.0f;
	mSpawnCounter = 0;

	mCoins.clear();
	
	SetUpLevel();
	SetUpSFX();
}

GameScreenLevel1::~GameScreenLevel1() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

	delete marioCharacter;
	marioCharacter = nullptr;

	delete luigiCharacter;
	luigiCharacter = nullptr;

	delete mPowBlock;
	mPowBlock = nullptr;

	if (!mEnemies.empty()){
		for (int i = 0; i < (int)mEnemies.size(); i++) {
			delete mEnemies[i];
		}

		mEnemies.clear();
	}

	if (!mCoins.empty()) {
		for (int i = 0; i < (int)mCoins.size(); i++) {
			delete mCoins[i];
		}

		mCoins.clear();
	}
}

void GameScreenLevel1::CreateCoins(Vector2D position, int value) {
	CharacterCoin* coinCharacter = 
		new CharacterCoin(mRenderer, "images/Coin.png", mLevelMap, position);
	mCoins.push_back(coinCharacter);
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* koopaCharacter = new CharacterKoopa(
		mRenderer, "images/Koopa.png", mLevelMap, position, direction, speed);
	mEnemies.push_back(koopaCharacter);
	mSpawnCounter++;
}

void GameScreenLevel1::CreateGoomba(Vector2D position, FACING direction, float speed) {
	CharacterGoomba* goombaCharacter = new CharacterGoomba(
		mRenderer, "images/Goomba.png", mLevelMap, position, direction, speed);
	mEnemies.push_back(goombaCharacter);
	mSpawnCounter++;
}


void GameScreenLevel1::DoScreenshake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	if (!mEnemies.empty()) {
		for (int i = 0; i < (int)mEnemies.size(); i++) {
			static_cast< CharacterKoopa* >(mEnemies[i])->TakeDamage();
		}
	}

	if (!mCoins.empty()) {
		for (int i = 0; i < (int)mCoins.size(); i++) {
			mCoins[i]->Jump();
		}
	}
}

void GameScreenLevel1::Render() {
	//Draw the background.
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	//Draw the Enemies.
	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		static_cast<CharacterKoopa*>(mEnemies[i])->Render();
	}

	//Draw the characters.
	marioCharacter->Render();
	luigiCharacter->Render();

	//Draw the POW block.
	mPowBlock->Render();

	//Draw the Coins.
	for (int i = 0; i < (int)mCoins.size(); i++) {
		mCoins[i]->Render();
	}
}

void GameScreenLevel1::SetLevelMap() {
	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	//Clear up and old map.
	if (mLevelMap != nullptr) {
		delete mLevelMap;
	}

	mLevelMap->LoadMapSurface("images/TextLevel.bmp");

	//mLevelMap->LoadBMP("images/TextLevel.bmp");

	//Set the new map.

	mLevelMap = new LevelMap(map);

}

bool GameScreenLevel1::SetUpLevel() {
	bool success = true;

	//Load the background texture.
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("images/BackgroundMB.png")) {
		std::cout << "Failed to load background texture!" << std::endl;
		success = false;
	}

	//Set up Level map.
	SetLevelMap();

	//Set up player character.
	marioCharacter = new CharacterMario(mRenderer, "images/Mario.png", Vector2D(64, 330), mLevelMap);
	luigiCharacter = new CharacterLuigi(mRenderer, "images/Luigi-sheet.png", Vector2D(352, 330), mLevelMap);

	//Set up our POW block.
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	//Set up 2 koopa guys.
	CreateKoopa(Vector2D(150, 32), FACING::FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), FACING::FACING_LEFT,  KOOPA_SPEED);

	//Set up 2 goomba guys.
	CreateGoomba(Vector2D(150, 96), FACING::FACING_RIGHT, GOOMBA_SPEED);
	CreateGoomba(Vector2D(325, 96), FACING::FACING_LEFT, GOOMBA_SPEED);

	//Set up some coins.
	CreateCoins(Vector2D(150, 32), COIN_VALUE);
	CreateCoins(Vector2D(325, 32), COIN_VALUE);

	return success;
}

void GameScreenLevel1::SetUpSFX() {
	pair<string, SoundEffect*> sfx;
	string path;

	/* DEATH SFX */
	//set path
	path = SFX_PATH; path += SFX_DEATH;

	//set sfx id
	sfx.first = "DEATH";

	//create sfx
	sfx.second = new SoundEffect(path);

	//fill sfx array if loading was successfull
	if (sfx.second != nullptr) mSounds.insert(sfx);

	/* GAME OVER SFX */
	path = SFX_PATH; path += SFX_GAME_OVER;
	sfx.first = "GAME_OVER";
	sfx.second = new SoundEffect(path);
	if (sfx.second != nullptr) mSounds.insert(sfx);

	/* JUMP SFX */
	path = SFX_PATH; path += SFX_JUMP;
	sfx.first = "JUMP";
	sfx.second = new SoundEffect(path);
	if (sfx.second != nullptr) mSounds.insert(sfx);

	/* COIN SFX */
	path = SFX_PATH; path += SFX_COIN;
	sfx.first = "COIN";
	sfx.second = new SoundEffect(path);
	if (sfx.second != nullptr) mSounds.insert(sfx);

	/* KILL SFX */
	path = SFX_PATH; path += SFX_KILL;
	sfx.first = "KILL";
	sfx.second = new SoundEffect(path);
	if (sfx.second != nullptr) mSounds.insert(sfx);

}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
	//DEBUG
	if (deltaTime > 16.0f) deltaTime = 16.0f;
	
	//--------------------------------------------
	//Do the screen shake if required.
	//--------------------------------------------
	if (mScreenshake) {
		mScreenshakeTime -= deltaTime;
		mWobble++;
		mBackgroundYPos = sin(mWobble);
		mBackgroundYPos *= 3.0f;

		//End the shake after the duration.
		if (mScreenshakeTime <= 0.0f) {
			mScreenshake = false;
			mBackgroundYPos = 0.0f;
		}
	}

	//Update the player(s).
	marioCharacter->Update(deltaTime, e);
	luigiCharacter->Update(deltaTime, e);

	// Old Circle Collision
	/*
	if (Collisions::Instance()->Circle(marioCharacter, luigiCharacter)) {
		cout << "Circle Bro Smash!" << endl;
	}
	*/

	// New Circle Collision
	if (Collisions::Instance()->Circle(
		marioCharacter->GetCollisionCircle(),
		luigiCharacter->GetCollisionCircle())
		) {
		cout << "Circle Bro Smash!" << endl;
	}

	if (Collisions::Instance()->Box(
		marioCharacter->GetCollisionBox(), luigiCharacter->GetCollisionBox())
		) {
		cout << "Box Bro Smash!" << endl;
	}

	//Update our Pow Block.
	//if (mPowBlock->IsAvailable()) {
		UpdatePOWBlock(deltaTime);
	//}

	//Update enemies.
	UpdateEnemies(deltaTime, e);

	//Update coins.
	UpdateCoins(deltaTime, e);
}

void GameScreenLevel1::UpdateCoins(float deltaTime, SDL_Event e) {
	if (!mCoins.empty()) {
		int coinIndexToDelete = -1;
		for (int i = 0; i < (int)mCoins.size(); i++) {
			mCoins[i]->Update(deltaTime, e);

			if (mCoins[i]->IsAlive()) {
				if (Collisions::Instance()->Circle(mCoins[i], marioCharacter)) {
					mSounds["COIN"]->Play();
					mCoins[i]->SetAlive(false);
				}

				if (Collisions::Instance()->Circle(mCoins[i], luigiCharacter)) {
					mSounds["COIN"]->Play();
					mCoins[i]->SetAlive(false);
				}

			} 

			if (!mCoins[i]->IsAlive()) coinIndexToDelete = i;
		}

		if (coinIndexToDelete != -1) 
			mCoins.erase(mCoins.begin() + coinIndexToDelete);
	}

	/*
	To add more coins:
	if counter has reached zero
		spawn coin at random co ords
		add to mCoins array
		reset timer
	else
		decrement counter
	*/
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e) {
	//---------------------------
	//Update the Enemies.
	//---------------------------
	if (!mEnemies.empty()) {
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemies.size(); i++) {
			
			//Check if enemy is on the bottomn row of tiles
			if (mEnemies[i]->GetPosition().y > 300.0f) {
			
				//Is the enemy off screen to the left / right?
				if (
					mEnemies[i]->GetPosition().x < (float)(-mEnemies[i]->GetCollisionBox().w*0.5f) ||
					mEnemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemies[i]->GetCollisionBox().w*0.5f)
					) static_cast<CharacterKoopa*>(mEnemies[i])->SetAlive(false);
			} else {
				//Is the enemy off screen to the left?
				if ( mEnemies[i]->GetPosition().x < 
					(float)(-mEnemies[i]->GetCollisionBox().w * 0.5f ) ) {
					mEnemies[i]->SetPosition(
						Vector2D(
						(float)(SCREEN_WIDTH - mEnemies[i]->GetCollisionBox().w),
							mEnemies[i]->GetPosition().y
						)
					);
				}

				//Is the enemy off screen to the right?
				if (mEnemies[i]->GetPosition().x >
					SCREEN_WIDTH - (float)(mEnemies[i]->GetCollisionBox().w * 0.5f)) {
					mEnemies[i]->SetPosition(
						Vector2D(
						(float)(mEnemies[i]->GetCollisionBox().w),
							mEnemies[i]->GetPosition().y
						)
					);
				}
			}

			//Now do the update.
			mEnemies[i]->Update(deltaTime, e);

			//Check to see if enemy collides with the player.
			if ( (
					mEnemies[i]->GetPosition().y > 300.0f ||
					mEnemies[i]->GetPosition().y <= 64.0f
				) && (
					mEnemies[i]->GetPosition().x < 64.0f || 
					mEnemies[i]->GetPosition().x > SCREEN_WIDTH-96.0f
				) ) {
				//Ignore the collisions if the enemy is behind a pipe?
			} else {
				if (Collisions::Instance()->Circle(
					mEnemies[i]->GetCollisionCircle(),
					marioCharacter->GetCollisionCircle()
				)) {
					if (static_cast<CharacterKoopa*>(mEnemies[i])->IsInjured()) {
						mSounds["KILL"]->Play();
						static_cast<CharacterKoopa*>(mEnemies[i])->SetAlive(false);
					} else {
						mSounds["DEATH"]->Play();
						marioCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
					}
				}

				if (Collisions::Instance()->Circle(
					mEnemies[i]->GetCollisionCircle(),
					luigiCharacter->GetCollisionCircle()
				)) {
					if (static_cast<CharacterKoopa*>(mEnemies[i])->IsInjured()) {
						mSounds["KILL"]->Play();
						static_cast<CharacterKoopa*>(mEnemies[i])->SetAlive(false);
					} else {
						mSounds["DEATH"]->Play();
						luigiCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
					}
				}
			}

			//If the enemy is no longer alive, then schedule it for deletion.
			if (!static_cast<CharacterKoopa*>(mEnemies[i])->IsAlive()) enemyIndexToDelete = i;
		}

		//-------------------------
		//Remove a dead enemies - 1 each update.
		//-------------------------
		if (enemyIndexToDelete != -1)
			mEnemies.erase(mEnemies.begin() + enemyIndexToDelete);
	}

	//--------------------
	//Add more enemies
	//--------------------
	if (mSpawnTimer <= 0.0f) {
		//Set up 2 more bad guys.
		CreateKoopa(Vector2D(150, 32), FACING::FACING_RIGHT, KOOPA_SPEED);
		CreateKoopa(Vector2D(325, 32), FACING::FACING_LEFT, KOOPA_SPEED);

		//Reset timer
		if (mSpawnCounter >= (SPAWN_TIME / 10)) {
			mSpawnTimer = SPAWN_TIME / 10;
		} else {
			mSpawnTimer = SPAWN_TIME - (mSpawnCounter * 10);
		}

	} else {
		mSpawnTimer -= deltaTime * 10;
	}
}

void GameScreenLevel1::UpdatePOWBlock(float deltaTime) {
	//Reset PowBlock
	if (mPowTimer <= 0.0f) {
		if (!mPowBlock->IsAvailable()) {
			mPowTimer = BLOCK_TIME;

			mPowBlock->ResetHits();
		}
	} else {
		mPowTimer -= deltaTime;
	}

	if (Collisions::Instance()->Box(marioCharacter->GetCollisionBox(), mPowBlock->GetCollisionBox())) {
		if (mPowBlock->IsAvailable()) {
			//Collided whilst jumping.
			if (marioCharacter->IsJumping()) {
				mSounds["KILL"]->Play();
				DoScreenshake();
				mPowBlock->TakeAHit();
				marioCharacter->CancelJump();
			}
		}
	}

	if (Collisions::Instance()->Box(luigiCharacter->GetCollisionBox(), mPowBlock->GetCollisionBox())) {
		if (mPowBlock->IsAvailable()) {
			//Collided whilst jumping.
			if (luigiCharacter->IsJumping()) {
				mSounds["KILL"]->Play();
				DoScreenshake();
				mPowBlock->TakeAHit();
				luigiCharacter->CancelJump();
			}
		}
	}
}
