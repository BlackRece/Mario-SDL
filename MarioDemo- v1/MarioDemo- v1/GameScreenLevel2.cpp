#include "GameScreenLevel2.h"

#include <iostream>
#include "Texture2D.h"
#include "PowBlock.h"
#include "Constants.h"
#include "Collisions.h"
#include "HighScores.h"
#include <string>

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer, SCREENS screen, GameScreenManager* manager) :
	GameScreen(renderer, screen, manager), mScreen(screen), mIsHighScore(false),
	mLevelOffset(TILE_WIDTH/2), mLevelBGOffset(TILE_WIDTH / 2), mSpeedOffset (0.0f) {
	
	string fontPath = "fonts/PixelFlag.ttf";
	if (!mLuigiFont.LoadFont(renderer, fontPath, 28))
		std::cout << "Failed setting up font glyph array! (" <<
		fontPath << ")" << std::endl;

	mGameCounter = GAME_DURATION;
	mLevelMap = nullptr;
	mPowTimer = BLOCK_TIME;

	mEnemies.clear();
	mSpawnTimer = 0.0f;
	mSpawnCounter = 0;
	mSpawnType = 1;
	mSpawnDir = FACING::FACING_LEFT;
	mSpawnPos = { 13 * TILE_WIDTH, -TILE_HEIGHT };

	mCoins.clear();

	mMarioScore = 0;
	mLuigiScore = 0;

	mMarioLives = MAX_LIVES;
	mLuigiLives = MAX_LIVES;

	mSpawnMario = { 4 * TILE_WIDTH, 16 * TILE_HEIGHT };
	mSpawnLuigi = { 2 * TILE_WIDTH, 16 * TILE_HEIGHT };

	mMusic = nullptr;

	SetUpLevel();
	SetUpSFX();
	SetUpMusic();
}

GameScreenLevel2::~GameScreenLevel2() {
	CleanUp();
}

void GameScreenLevel2::CleanUp() {
	Mix_HaltMusic();
	Mix_HaltChannel(-1);
	Mix_FreeMusic(mMusic);
	mMusic = nullptr;

	delete marioCharacter;
	marioCharacter = nullptr;

	delete luigiCharacter;
	luigiCharacter = nullptr;

	delete mPowBlock;
	mPowBlock = nullptr;

	if (!mEnemies.empty()) {
		for (int i = 0; i < (int)mEnemies.size(); i++) {
			delete mEnemies[i].second;
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

void GameScreenLevel2::CreateCoins(Vector2D position, int value) {
	CharacterCoin* coinCharacter =
		new CharacterCoin(mRenderer, COIN_PATH, mLevelMap, position);
	mCoins.push_back(coinCharacter);
}

void GameScreenLevel2::CreateKoopa_G(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* koopaCharacter = new CharacterKoopa(
		mRenderer, G_KOOPA_PATH, mLevelMap, position, direction, speed);
	koopaCharacter->mValue = G_KOOPA_VALUE;
	mEnemies.push_back(make_pair('K', koopaCharacter));
	mSpawnCounter++;
}

void GameScreenLevel2::CreateKoopa_R(Vector2D position, FACING direction, float speed) {
	CharacterKoopa* koopaCharacter = new CharacterKoopa(
		mRenderer, R_KOOPA_PATH, mLevelMap, position, direction, speed);
	koopaCharacter->mIsFlying = true;
	koopaCharacter->mValue = R_KOOPA_VALUE;
	mEnemies.push_back(make_pair('K', koopaCharacter));
	mSpawnCounter++;
}

void GameScreenLevel2::CreateSpiny(Vector2D position, FACING direction, float speed) {
	CharacterSpiny* spinyCharacter = new CharacterSpiny(
		mRenderer, R_SPINY_PATH, mLevelMap, position, direction, speed);
	spinyCharacter->mValue = R_SPINY_VALUE;
	mEnemies.push_back(make_pair('S', spinyCharacter));
	mSpawnCounter++;
}

void GameScreenLevel2::DoScreenshake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	if (!mEnemies.empty()) {
		for (int i = 0; i < (int)mEnemies.size(); i++) {
			if (mEnemies[i].first == 'K') {
				static_cast<CharacterKoopa*>(mEnemies[i].second)->TakeDamage();
			}
		}
	}

	if (!mCoins.empty()) {
		for (int i = 0; i < (int)mCoins.size(); i++) {
			mCoins[i]->Jump();
		}
	}
}

void GameScreenLevel2::Render() {
	//Draw the background.
	mLevelMap->RenderBackground(mLevelBGOffset, mBackgroundYPos);

	//Draw playground (aka. the interact-able play area).
	mLevelMap->RenderPlayground(mLevelOffset);

	//Draw the POW block.
	mPowBlock->Render(mSpeedOffset);

	//Draw the Enemies.
	for (unsigned int i = 0; i < mEnemies.size(); i++) {
		switch (mEnemies[i].first) {
			case 'K':static_cast<CharacterKoopa*>(mEnemies[i].second)->Render(mSpeedOffset); break;
			case 'S':static_cast<CharacterSpiny*>(mEnemies[i].second)->Render(mSpeedOffset); break;
		}
	}	

	//Draw the Coins.
	for (int i = 0; i < (int)mCoins.size(); i++) {
		mCoins[i]->Render(mSpeedOffset);
	}

	//Draw foreground
	mLevelMap->RenderForeground(mLevelOffset);

	//Draw the characters.
	if (mMarioLives > 0) marioCharacter->Render(mSpeedOffset);
	if (mLuigiLives > 0) luigiCharacter->Render(mSpeedOffset);

	//Draw HUD.
	RenderHUD();
}

void GameScreenLevel2::RenderHighScore() {
	std::string sScoreTitle = "New High Score!";
	std::string sScoreText;

	SDL_Color cScoreTitleCol = { 0xFF, 0x00, 0x00 };
	SDL_Color cScoreTextCol = { 0xFF, 0xFF, 0xFF };

	SDL_Rect scoreRect = {
		TILE_WIDTH * 2,
		TILE_HEIGHT * 8,
		TILE_WIDTH * 10,
		TILE_HEIGHT * 3
	};

	mLevelMap->RenderPlate(scoreRect);
	mLuigiFont.RenderString(scoreRect, sScoreTitle, cScoreTitleCol);

}

void GameScreenLevel2::RenderHUD() {
	int charWidth = mLuigiFont.GetWidth('W');
	int charHeight = mLuigiFont.GetHeight('H');
	string sMarioScore = "Mario: " + to_string(mMarioScore);
	string sLuigiScore = "Luigi: " + to_string(mLuigiScore);
	string sGameTime = "Time: " + to_string((int)mGameCounter);

	//mario score
	//box dimensions
	SDL_Rect marioPos = {
		charWidth,
		SCREEN_HEIGHT - charHeight,
		int(charWidth * sMarioScore.length()),
		charHeight
	};

	//score background box
	SDL_SetRenderDrawColor(mRenderer, 240, 20, 0, 255);
	SDL_RenderFillRect(mRenderer, &marioPos);

	//text dimensions
	marioPos.x += 2;
	marioPos.y += 2;

	mLuigiFont.RenderString(marioPos, sMarioScore, { 0x00, 0x00, 0x00 });

	//luigi score
	//box dimensions
	SDL_Rect luigiPos = {
		(SCREEN_WIDTH - charWidth) - int(charWidth * sLuigiScore.length()),
		SCREEN_HEIGHT - charHeight,
		charWidth * (int)sLuigiScore.length(),
		charHeight
	};

	//score background box
	SDL_SetRenderDrawColor(mRenderer, 36, 178, 36, 255);
	SDL_RenderFillRect(mRenderer, &luigiPos);

	//text dimensions
	luigiPos.x += 2;
	luigiPos.y += 2;

	mLuigiFont.RenderString(luigiPos, sLuigiScore, { 0x00, 0x00, 0x00 });

	//timer
	//box dimensions
	SDL_Rect timePos = {
		(SCREEN_WIDTH / 2) - int((charWidth * sGameTime.length() / 2)),
		int(charHeight * 0.5f),
		charWidth * (int)sGameTime.length(),
		charHeight
	};

	mLuigiFont.RenderString(timePos, sGameTime, { 0x00, 0x00, 0x00 });

	//game over plate
	if (mGameOverTimer > 0) {

		string sGameOverText;
		if (mMarioScore > mLuigiScore) sGameOverText = "Mario Wins!";
		if (mMarioScore < mLuigiScore) sGameOverText = "Luigi Wins!";
		if (mMarioScore == mLuigiScore) sGameOverText = "DRAW!";

		SDL_Rect gameOverPos = {
			int(SCREEN_WIDTH / 2) - int((charWidth * sGameOverText.length() / 2) - 1),
			(SCREEN_HEIGHT / 2) - (TILE_HEIGHT / 2) - 1,
			charWidth * (int)sGameOverText.length(),
			TILE_HEIGHT * 2
		};

		mLevelMap->RenderPlate(gameOverPos);

		mLuigiFont.RenderString(gameOverPos, sGameOverText, { 0x00, 0x00, 0x00 });
		gameOverPos.x += 2;
		gameOverPos.y += 2;
		mLuigiFont.RenderString(gameOverPos, sGameOverText, { 0xFF, 0xFF, 0xFF });
	}

	//highscore
	if (mIsHighScore) RenderHighScore();

}

void GameScreenLevel2::SetLevelMap() {
	/*
	*** Game Screen ***
	3 level maps ( foreground, playground and background )

	foreground and background defines decorations

	playground defines block and character interactions

	*/

	//Clear up old map.
	if (mLevelMap != nullptr) {
		delete mLevelMap;
	}

	//Set the new map.
	mLevelMap = new LevelMap(mRenderer);
}

bool GameScreenLevel2::SetUpLevel() {
	bool success = true;

	//Set up Level map.
	SetLevelMap();

	//Set up player character.
	marioCharacter = new CharacterMario(mRenderer, MARIO_PATH, mSpawnMario, mLevelMap, { 169, 203, 197 });
	luigiCharacter = new CharacterLuigi(mRenderer, LUIGI_PATH, mSpawnLuigi, mLevelMap, { 169, 203, 197 });

	//put player on screen
	marioCharacter->Jump();
	luigiCharacter->Jump();

	//Set up our POW block.
	mPowBlock = new PowBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	//Set up some coins (20).
	/*
	for (int coinX = 2; coinX < 14; coinX++) {
		//mid row
		if (coinX > 5 && coinX < 10)
			CreateCoins(
				Vector2D(float(coinX * TILE_WIDTH), float(4 * TILE_HEIGHT)), COIN_VALUE);

		if (coinX < 6 || coinX > 9) {
			//top row
			CreateCoins(
				Vector2D(float(coinX * TILE_WIDTH), float(1 * TILE_HEIGHT)), COIN_VALUE);

			//bot row
			CreateCoins(
				Vector2D(float(coinX * TILE_WIDTH), float(8 * TILE_HEIGHT)), COIN_VALUE);
		}
	}
	*/

	return success;
}

void GameScreenLevel2::SetUpSFX() {
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

	/* BACKGROUND MUSIC LOOP */
	//TODO: find different tune
	LoadMusic("music/Super Mario by Hizzleguy.mp3");
	if (Mix_PlayingMusic() == 0) {
		Mix_PlayMusic(mMusic, -1);
	}
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e) {
	//Adjust level offset
	char* col = new char[MAP_HEIGHT];
	mSpeedOffset = deltaTime * float(LEVEL_SPEED);

	//background
	if (mLevelBGOffset <= -float(TILE_WIDTH / 2)) {
		mLevelMap->GetMapCol('b', 0, col);
		mLevelMap->AddMapCol('b', col);

		mLevelBGOffset = TILE_WIDTH / 2;
	} else {
		mLevelBGOffset -= deltaTime * float(LEVEL_BG_SPEED);
	}

	//playground & foreground
	if (mLevelOffset <= -float(TILE_WIDTH / 2)) {
		//playground 
		mLevelMap->GetMapCol('p', 0, col);
		mLevelMap->AddMapCol('p', col);

		//foreground
		mLevelMap->GetMapCol('f', 0, col);
		mLevelMap->AddMapCol('f', col);

		mLevelOffset = TILE_WIDTH / 2;
	} else {
		mLevelOffset -= mSpeedOffset;
	}

	//Update game timer
	if (!mIsGameOver) {
		//TODO: make sure characters move with level
		//Do the screen shake if required.
		if (mScreenshake) UpdateScreenShake(deltaTime, e);

		//Update the player(s).
		if (mMarioLives > 0) {
			marioCharacter->Update(deltaTime, e);
			Vector2D marioOffset = marioCharacter->GetPosition();
			marioOffset.x -= mSpeedOffset;
			marioCharacter->SetPosition(marioOffset);
		}

		if (mLuigiLives > 0) {
			luigiCharacter->Update(deltaTime, e);
			Vector2D luigiOffset = luigiCharacter->GetPosition();
			luigiOffset.x -= mSpeedOffset;
			luigiCharacter->SetPosition(luigiOffset);
		}

		//Update our Pow Block.
		UpdatePOWBlock(deltaTime);

		if (!mIsHighScore) {
			//Update enemies.
			UpdateEnemies(deltaTime, e);
		}

		//Update coins.
		UpdateCoins(deltaTime, e);

		//TODO: update scrolling level

		//Update game over state if a player runs out of lives
		if (mMarioLives < 1 || mLuigiLives < 1) {
			mIsGameOver = true;
			mGameOverTimer = GAME_OVER_TIMER;
		}

		//Update game ove state if time runs out
		if (mGameCounter < 0) {
			mIsGameOver = true;
			mGameOverTimer = GAME_OVER_TIMER;
		} else {
			mGameCounter -= deltaTime;
		}

	} else {

		if (mGameOverTimer > 0) {
			if (!UpdateHighScore()) {
				mGameOverTimer -= deltaTime * 10;
			}
		} else {
			CleanUp();
			SetNextScreen(SCREENS::SCREEN_INTRO);
			//SetNextScreen(SCREENS::SCREEN_GAMEOVER);
		}
	}
}

bool GameScreenLevel2::UpdateHighScore() {
	bool result = false;
	int lowScore = 0, bestScore = 0;

	if (mMarioScore == mLuigiScore)
		bestScore = mLuigiScore;
	else
		bestScore = (mMarioScore > mLuigiScore) ? mMarioScore : mLuigiScore;

	if (HighScores::Instance()->LoadScores())
		lowScore = HighScores::Instance()->GetLowScore();

	if (lowScore < bestScore) {
		mIsHighScore = true;
	}
	/*
	singleton? no
	there's already a highscores screen
	use it for input as well as output!
	call from previous screen and pass in correct scores file
	load scores file "POWPanic.txt"
	sort scores
	get lowest score
	are new scores in the top ten
		if so,
			get player's name
			add score and name to high score
	show high scores
	*/
	return result;
}

void GameScreenLevel2::UpdateCoins(float deltaTime, SDL_Event e) {
	if (!mCoins.empty()) {
		int coinIndexToDelete = -1;
		for (int i = 0; i < (int)mCoins.size(); i++) {
			mCoins[i]->Update(deltaTime, e);

			if (mCoins[i]->IsAlive()) {
				if (Collisions::Instance()->Circle(
					mCoins[i]->GetCollisionCircle(),
					marioCharacter->GetCollisionCircle()
				)) {
					mMarioScore += mCoins[i]->mValue;
					mSounds["COIN"]->Play();
					mCoins[i]->SetAlive(false);
				}

				if (Collisions::Instance()->Circle(
					mCoins[i]->GetCollisionCircle(),
					luigiCharacter->GetCollisionCircle()
				)) {
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

}

void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e) {
	//TODO: REFACTOR THIS METHOD!!!

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
					mEnemies[i].second->GetPosition().x <
					(float)(-mEnemies[i].second->GetCollisionRadius()) ||
					mEnemies[i].second->GetPosition().x >
					SCREEN_WIDTH - (float)(mEnemies[i].second->GetCollisionRadius())
					) {
					switch (mEnemies[i].first) {
						case 'K':
							static_cast<CharacterKoopa*>(mEnemies[i].second)->SetAlive(false);
							break;
						case 'S':
							static_cast<CharacterSpiny*>(mEnemies[i].second)->SetAlive(false);
							break;
					}
				}
			} else {
				//Is the enemy off screen to the left?
				if (
					mEnemies[i].second->GetPosition().x <
					-mEnemies[i].second->GetCollisionRadius()
					) {
					//put enemy on the right
					mEnemies[i].second->SetPosition(
						Vector2D(
							(float)(SCREEN_WIDTH - mEnemies[i].second->GetCollisionRadius()),
							mEnemies[i].second->GetPosition().y
						)
					);
				}

				//Is the enemy off screen to the right?
				if (
					mEnemies[i].second->GetPosition().x >
					SCREEN_WIDTH - (float)(mEnemies[i].second->GetCollisionRadius())
					) {
					//put enemy on the left
					mEnemies[i].second->SetPosition(
						Vector2D(
							-mEnemies[i].second->GetCollisionRadius(),
							mEnemies[i].second->GetPosition().y
						)
					);
				}
			}

			//Now do the update.
			mEnemies[i].second->Update(deltaTime, e);

			//Check to see if enemy collides with the player.
			//but first, ignore the collisions if the enemy is behind a pipe?
			if ((
				mEnemies[i].second->GetPosition().y > 300.0f ||
				mEnemies[i].second->GetPosition().y <= 64.0f
				) && (
					mEnemies[i].second->GetPosition().x < 64.0f ||
					mEnemies[i].second->GetPosition().x > SCREEN_WIDTH - 96.0f
					)) {
			} else {
				//check collisions with player
				//mario
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
							marioCharacter->Respawn(mSpawnMario);
							mMarioLives--;
						}
					} else if (mEnemies[i].first == 'S') {
						if (static_cast<CharacterSpiny*>(mEnemies[i].second)->IsInjured()) {
							mMarioScore += static_cast<CharacterSpiny*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterSpiny*>(mEnemies[i].second)->SetAlive(false);
						} else {
							mSounds["DEATH"]->Play();
							marioCharacter->Respawn(mSpawnMario);
							mMarioLives--;
						}
					} 
				}

				//luigi
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
							luigiCharacter->Respawn(mSpawnLuigi);
							mLuigiLives--;
						}
					} else if (mEnemies[i].first == 'S') {
						if (static_cast<CharacterSpiny*>(mEnemies[i].second)->IsInjured()) {
							mLuigiScore += static_cast<CharacterSpiny*>(mEnemies[i].second)->mValue;
							mSounds["KILL"]->Play();
							static_cast<CharacterSpiny*>(mEnemies[i].second)->SetAlive(false);
						} else {
							mSounds["DEATH"]->Play();
							luigiCharacter->Respawn(mSpawnLuigi);
							mMarioLives--;
						}
					}
				}
			}

			//If the enemy is no longer alive, then schedule it for deletion.
			if (mEnemies[i].first == 'K') {
				if (!static_cast<CharacterKoopa*>(mEnemies[i].second)->IsAlive()) enemyIndexToDelete = i;
			} else if (mEnemies[i].first == 'S') {
				if (!static_cast<CharacterSpiny*>(mEnemies[i].second)->IsAlive()) enemyIndexToDelete = i;
			}
		}

		//-------------------------
		//Remove a dead enemies - 1 each update.
		//-------------------------
		if (enemyIndexToDelete != -1)
			mEnemies.erase(mEnemies.begin() + enemyIndexToDelete);

	} else {
		mSpawnTimer = 0.0f;
	}

	//--------------------
	//Add more enemies
	//--------------------
	if (mSpawnTimer <= 0.0f) {
		if (mSpawnDir == FACING::FACING_LEFT)
			mSpawnDir = FACING::FACING_RIGHT;
		else
			mSpawnDir = FACING::FACING_LEFT;

		switch (mSpawnType) {
			case 0: case 1:	case 3://Set up 2 more koopa guys.
				CreateKoopa_G(mSpawnPos, mSpawnDir, G_KOOPA_SPEED); break;

			case 2: case 4:	//Set up 2 more flying koopa guys.
				CreateKoopa_R(mSpawnPos, mSpawnDir, R_KOOPA_SPEED); break;

			case 5: //Set up 2 more spiny guys.
				CreateSpiny(mSpawnPos, mSpawnDir, GOOMBA_SPEED); break;
		}

		if (mSpawnType > 4)
			mSpawnType = 0;
		else
			mSpawnType++;

		//Reset spawn timer
		if (SPAWN_TIME - mSpawnCounter < SPAWN_TIME / 4) {
			mSpawnTimer = SPAWN_TIME / 4;
		} else {
			mSpawnTimer = (SPAWN_TIME / 2) - mSpawnCounter;
		}

	} else {
		mSpawnTimer -= deltaTime * 10;
	}
}

void GameScreenLevel2::UpdatePOWBlock(float deltaTime) {
	Point2D tPos;

	mPowBlock->Update(deltaTime);

	for (int i = 0; i < mPowBlock->GetAmount(); i++) {
		//adjust pos by level speed
		tPos = mPowBlock->GetPos(i);
		tPos.x += mSpeedOffset;
		mPowBlock->SetPos(i, tPos);

		if (Collisions::Instance()->Box(
			marioCharacter->GetCollisionBox(), mPowBlock->GetCollisionBox(i))
			) {
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

		if (Collisions::Instance()->
			Box(
				luigiCharacter->GetCollisionBox(),
				mPowBlock->GetCollisionBox(i))
			) {
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
}

void GameScreenLevel2::UpdateScreenShake(float deltaTime, SDL_Event e) {
	mScreenshakeTime -= deltaTime;
	mWobble++;
	mBackgroundYPos = sin(mWobble);
	mBackgroundYPos *= 5.0f;

	//End the shake after the duration.
	if (mScreenshakeTime <= 0.0f) {
		mScreenshake = false;
		mBackgroundYPos = 0.0f;
	}
}

void GameScreenLevel2::SetUpMusic() {
	LoadMusic("music/Hizzleguy - Super Mario.mp3");
	if (mMusic != nullptr) {
		if (Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(mMusic, 1);
		}
	}
}

void GameScreenLevel2::LoadMusic(string musicPath) {
	mMusic = Mix_LoadMUS(musicPath.c_str());
	if (mMusic == nullptr) {
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}
