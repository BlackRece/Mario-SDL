#ifndef CONSTANTS_H
#define CONSTANTS_H

//Screen Dimensions.
#define SCREEN_WIDTH				512
#define SCREEN_HEIGHT				448
#define SCREENSHAKE_DURATION		0.25f

//Movement. 
#define MOVEMENT_SPEED				0.25f
#define KOOPA_SPEED					0.10f //96.0f is too much!!!
#define GOOMBA_SPEED				0.05f
#define TILE_HEIGHT					32
#define TILE_WIDTH					32
#define TILE_SIZE					16

//Gravity.
#define JUMP_FORCE_DECREMENT		400.0f
#define INITIAL_JUMP_FORCE			300.0f
#define INITIAL_JUMP_FORCE_SMALL	100.0f
#define GRAVITY						200.0f

//Level.
#define MAP_HEIGHT					14
#define MAP_WIDTH					16
#define BLOCK_TIME					50.0f
#define TERRAIN_PATH				"images/TerrainSheet.png"

//Enemy.
#define INJURED_TIME				2.5f
#define SPAWN_TIME					200.0f
#define G_KOOPA_PATH				"images/GreenKoopaSheet.png"
#define G_KOOPA_VALUE				10
#define R_KOOPA_PATH				"images/RedKoopaSheet.png"
#define R_KOOPA_VALUE				20
#define R_SPINY_PATH				"images/RedSpinyShhet.png"
#define R_SPINY_VALUE				30

//Misc.
#define CHARACTERSTATE_PLAYER_DEATH true
#define ANIMATION_DELAY				0.15f
#define FONT_PATH					"fonts/BubblePixel.TTF"
#define COIN_PATH					"images/CoinSheet.png"
#define COIN_VALUE					100
#define COIN_FRAME_W				8		//number of frames wide
#define COIN_FRAME_H				1		//number of frames tall

//SFX.
#define SFX_PATH					"sounds/"
#define SFX_DEATH					"mb_die.wav"
#define SFX_GAME_OVER				"nsmb_game_over.wav"
#define SFX_JUMP					"mb_jump.wav"
#define SFX_COIN					"nsmb_coin.wav"
#define SFX_KILL					"mb_sc.wav"

#endif // !CONSTANTS_H

