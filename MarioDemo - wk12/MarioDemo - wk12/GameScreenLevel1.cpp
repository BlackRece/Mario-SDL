#include "GameScreenLevel1.h"

#include <iostream>
#include "Texture2D.h"
#include "PowBlock.h"
#include "Constants.h"
#include "Collisions.h"
#include "CharacterGoomba.h"
#include <string>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	string fontPath = "fonts/PixelFlag.ttf";// "fonts/SuperPlumberBrothers.ttf";
	if (!mMarioFont.LoadFont(renderer, fontPath, 28))
		std::cout << "Failed setting up font glyph array! (" <<
		fontPath << ")" << std::endl;
	
	mLevelMap = nullptr;
	mPowTimer = BLOCK_TIME;
	
	mEnemies.clear();
	mSpawnTimer = 0.0f;
	mSpawnCounter = 0;

	mCoins.clear();

	mMarioScore = 0;
	mLuigiScore = 0;
	
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
		for (int i = 0; i < mEnemies.size(); i++) {
			delete mEnemies[i].second;
		}

		mEnemies.clear();
	}

	if (!mCoins.empty()) {
		for (int i = 0; i < mCoins.size(); i++) {
			delete mCoins[i];
		}

		mCoins.clear();
	}
}

void GameScreenLevel1::CreateCoins(Vector2D position, int value) {
	CharacterCoin* coinCharacter = 
		new CharacterCoin(mRenderer, COIN_PATH, mLevelMap, position);
	mCoins.push_back(coinCharacter);
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* koopaCharacter = new CharacterKoopa(
		mRenderer, "images/Koopa.png", mLevelMap, position, direction, speed);
	mEnemies.push_back(make_pair('K', koopaCharacter));
	mSpawnCounter++;
}

void GameScreenLevel1::CreateGoomba(Vector2D position, FACING direction, float speed) {
	CharacterGoomba* goombaCharacter = new CharacterGoomba(
		mRenderer, "images/Goomba2.png", mLevelMap, position, direction, speed);
	mEnemies.push_back(make_pair('G', goombaCharacter));
	mSpawnCounter++;
}


void GameScreenLevel1::DoScreenshake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	if (!mEnemies.empty()) {
		for (int i = 0; i < mEnemies.size(); i++) {
			if (mEnemies[i].first == 'K') {
				static_cast<CharacterKoopa*>(mEnemies[i].second)->TakeDamage();
			} else {
				static_cast<CharacterGoomba*>(mEnemies[i].second)->TakeDamage();
			}
		}
	}

	if (!mCoins.empty()) {
		for (int i = 0; i < mCoins.size(); i++) {
			mCoins[i]->Jump();
		}
	}
}

void GameScreenLevel1::Render() {
	//Draw the background.
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	int charWidth = mMarioFont.GetWidth('W');
	int charHeight = mMarioFont.GetHeight('H');
	string sMarioScore = "Mario: " + std::to_string(mMarioScore);
	string sLuigiScore = "Luigi: " + std::to_string(mLuigiScore);

	//Draw HUD.
	//box dimensions
	SDL_Rect marioPos = {
		charWidth,
		SCREEN_HEIGHT - charHeight,
		charWidth * sMarioScore.length(),
		charHeight
	};
	SDL_SetRenderDrawColor(mRenderer, 240, 20, 0, 255);
	SDL_RenderFillRect(mRenderer, &marioPos);

	//text dimensions
	marioPos.x += 2;
	marioPos.y += 2;

	mMarioFont.RenderString(marioPos, sMarioScore, { 0x00, 0x00, 0x00 });

	//box dimensions
	SDL_Rect luigiPos = {
		(SCREEN_WIDTH - charWidth) - (charWidth * sLuigiScore.length()),
		SCREEN_HEIGHT - charHeight,
		charWidth * sLuigiScore.length(),
		charHeight
	};
	SDL_SetRenderDrawColor(mRenderer, 36, 178, 36, 255);
	SDL_RenderFillRect(mRenderer, &luigiPos);
	
	//text dimensions
	luigiPos.x += 2;
	luigiPos.y += 2;

	mMarioFont.RenderString(luigiPos, sLuigiScore, {0x00, 0x00, 0x00});

	//Draw the Enemies.
	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		if (mEnemies[i].first == 'K') {
			static_cast<CharacterKoopa*>(mEnemies[i].second)->Render();
		} else {
			static_cast<CharacterGoomba*>(mEnemies[i].second)->Render();
		}
	}

	//Draw the characters.
	marioCharacter->Render();
	luigiCharacter->Render();

	//Draw the POW block.
	mPowBlock->Render();

	//Draw the Coins.
	for (int i = 0; i < mCoins.size(); i++) {
		mCoins[i]->Render();
	}
}

void GameScreenLevel1::SetLevelMap() {
	/*
	*** Game Screen ***
	3 level maps ( foreground, playground and background )

	foreground and background defines decorations
	
	playground defines block and character interactions

	*/

	/*
	
	0 = POW Block
	*/

	//foreground map
	int fgmap[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	//playground map
	int bgmap[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	//playground map
	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
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
	luigiCharacter = new CharacterLuigi(mRenderer, "images/Luigi.png", Vector2D(352, 330), mLevelMap);

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
	UpdatePOWBlock(deltaTime);

	//Update enemies.
	UpdateEnemies(deltaTime, e);

	//Update coins.
	UpdateCoins(deltaTime, e);
}

void GameScreenLevel1::UpdateCoins(float deltaTime, SDL_Event e) {
	if (!mCoins.empty()) {
		int coinIndexToDelete = -1;
		for (int i = 0; i < mCoins.size(); i++) {
			mCoins[i]->Update(deltaTime, e);

			if (mCoins[i]->IsAlive()) {
				if (Collisions::Instance()->Circle(mCoins[i], marioCharacter)) {
					mMarioScore += mCoins[i]->mValue;
					mSounds["COIN"]->Play();
					mCoins[i]->SetAlive(false);
				}

				if (Collisions::Instance()->Circle(mCoins[i], luigiCharacter)) {
					mLuigiScore += mCoins[i]->mValue;
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
			if (mEnemies[i].second->GetPosition().y > 300.0f) {
			
				//Is the enemy off screen to the left / right?
				if (
					mEnemies[i].second->GetPosition().x < (float)(-mEnemies[i].second->GetCollisionBox().w * 0.5f) ||
					mEnemies[i].second->GetPosition().x > SCREEN_WIDTH - (float)(mEnemies[i].second->GetCollisionBox().w * 0.5f)
					) {
					if (mEnemies[i].first = 'K')
						static_cast<CharacterKoopa*>(mEnemies[i].second)->SetAlive(false);
					else
						static_cast<CharacterGoomba*>(mEnemies[i].second)->SetAlive(false);
				}
			} else {
				//Is the enemy off screen to the left?
				if ( mEnemies[i].second->GetPosition().x < 
					(float)(-mEnemies[i].second->GetCollisionBox().w * 0.5f ) ) {
					mEnemies[i].second->SetPosition(
						Vector2D(
						(float)(SCREEN_WIDTH - mEnemies[i].second->GetCollisionBox().w),
							mEnemies[i].second->GetPosition().y
						)
					);
				}

				//Is the enemy off screen to the right?
				if (mEnemies[i].second->GetPosition().x >
					SCREEN_WIDTH - (float)(mEnemies[i].second->GetCollisionBox().w * 0.5f)) {
					mEnemies[i].second->SetPosition(
						Vector2D(
						(float)(mEnemies[i].second->GetCollisionBox().w),
							mEnemies[i].second->GetPosition().y
						)
					);
				}
			}

			//Now do the update.
			mEnemies[i].second->Update(deltaTime, e);

			//Check to see if enemy collides with the player.
			//but first, ignore the collisions if the enemy is behind a pipe?
			if ( (
					mEnemies[i].second->GetPosition().y > 300.0f ||
					mEnemies[i].second->GetPosition().y <= 64.0f
				) && (
					mEnemies[i].second->GetPosition().x < 64.0f || 
					mEnemies[i].second->GetPosition().x > SCREEN_WIDTH-96.0f
				) ) {
			} else {
				//check collisions with player
				if (Collisions::Instance()->Circle(
					mEnemies[i].second->GetCollisionCircle(),
					marioCharacter->GetCollisionCircle()
				)) {
					if (mEnemies[i].first == 'K') {
						if (static_cast<CharacterKoopa*>(mEnemies[i].second)->IsInjured()) {
							mMarioScore += static_cast<CharacterKoopa*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterKoopa*>(mEnemies[i].second)->SetAlive(false);
						} else {
							mSounds["DEATH"]->Play();
							marioCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
						}
					} else {
						if (static_cast<CharacterGoomba*>(mEnemies[i].second)->IsInjured()) {
							mMarioScore += static_cast<CharacterGoomba*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterGoomba*>(mEnemies[i].second)->SetAlive(false);
						}
						else {
							mSounds["DEATH"]->Play();
							marioCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
						}
					}
				}

				if (Collisions::Instance()->Circle(
					mEnemies[i].second->GetCollisionCircle(),
					luigiCharacter->GetCollisionCircle()
				)) {
					if (mEnemies[i].first == 'K') {
						if (static_cast<CharacterKoopa*>(mEnemies[i].second)->IsInjured()) {
							mLuigiScore += static_cast<CharacterKoopa*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterKoopa*>(mEnemies[i].second)->SetAlive(false);
						} else {
							mSounds["DEATH"]->Play();
							luigiCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
						}
					} else {
						if (static_cast<CharacterGoomba*>(mEnemies[i].second)->IsInjured()) {
							mLuigiScore += static_cast<CharacterGoomba*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterGoomba*>(mEnemies[i].second)->SetAlive(false);
						} else {
							mSounds["DEATH"]->Play();
							luigiCharacter->SetState(CHARACTERSTATE_PLAYER_DEATH);
						}
					}
				}
			}

			//If the enemy is no longer alive, then schedule it for deletion.
			if (mEnemies[i].first == 'K') {
				if (!static_cast<CharacterKoopa*>(mEnemies[i].second)->IsAlive()) enemyIndexToDelete = i;
			} else {
				if (!static_cast<CharacterGoomba*>(mEnemies[i].second)->IsAlive()) enemyIndexToDelete = i;
			}
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
	int enemyType = (rand() % 5);
	if (mSpawnTimer <= 0.0f) {
		if (enemyType >= 3) {
			//Set up 2 more koopa guys.
			CreateKoopa(Vector2D(150, 32), FACING::FACING_RIGHT, KOOPA_SPEED);
			CreateKoopa(Vector2D(325, 32), FACING::FACING_LEFT, KOOPA_SPEED);
		} else {
			//Set up 2 more goomba guys.
			CreateGoomba(Vector2D(150, 96), FACING::FACING_RIGHT, GOOMBA_SPEED);
			CreateGoomba(Vector2D(325, 96), FACING::FACING_LEFT, GOOMBA_SPEED);
		}

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
