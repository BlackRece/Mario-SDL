#include "GameScreenLevel1.h"

#include <iostream>
#include "Texture2D.h"
#include "Constants.h"
#include "Collisions.h"

bool GameScreenLevel1::SetUpLevel() {
	bool success = true;

	//Load the background texture.
	mBackgroundTexture = new Texture2D(mRenderer);
	if (!mBackgroundTexture->LoadFromFile("images/Test.bmp")) {
		std::cout << "Failed to load background texture!" << std::endl;
		success = false;
	}

	//Set up player character.
	marioCharacter = new CharacterMario(mRenderer, "images/Mario.png", Vector2D(64, 330));
	luigiCharacter = new CharacterLuigi(mRenderer, "images/Luigi.png", Vector2D(352, 330));

	return success;
}

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer, SCREENS screen) :
	GameScreen(renderer, screen) {
	mScreen = screen;
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1() {
	delete mBackgroundTexture;
	mBackgroundTexture = nullptr;

	delete marioCharacter;
	marioCharacter = nullptr;

	delete luigiCharacter;
	luigiCharacter = nullptr;
}

void GameScreenLevel1::Render() {
	mBackgroundTexture->Render(Vector2D(), SDL_FLIP_NONE);

	marioCharacter->Render();
	luigiCharacter->Render();
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e) {
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
}
