#include "GameScreenLevel1.h"

#include <iostream>
#include "Texture2D.h"
#include "PowBlock.h"
#include "Constants.h"
#include "Collisions.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	mLevelMap = nullptr;
	SetUpLevel();
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
}

void GameScreenLevel1::Render() {
	//Draw the background.
	mBackgroundTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	//Draw the characters.
	marioCharacter->Render();
	luigiCharacter->Render();

	//Draw the POW block.
	mPowBlock->Render();
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

	//Set the new map.
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock() {
	if (Collisions::Instance()->Box(marioCharacter->GetCollisionBox(), mPowBlock->GetCollisionBox())) {
		if (mPowBlock->IsAvailable()) {
			//Collided whilst jumping.
			if (marioCharacter->IsJumping()) {
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
				DoScreenshake();
				mPowBlock->TakeAHit();
				luigiCharacter->CancelJump();
			}
		}
	}
}

void GameScreenLevel1::DoScreenshake() {
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;
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

	return success;
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
	//--------------------------------------------
	//Do the screen shakeif required.
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
	if (mPowBlock->IsAvailable()) {
		UpdatePOWBlock();
	}
}
