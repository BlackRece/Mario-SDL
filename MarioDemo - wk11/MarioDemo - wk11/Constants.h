#ifndef CONSTANTS_H
#define CONSTANTS_H

//Screen Dimensions.
#define SCREEN_WIDTH				512
#define SCREEN_HEIGHT				416
#define SCREENSHAKE_DURATION		0.25f

//Movement. 
#define MOVEMENT_SPEED				0.25f
#define KOOPA_SPEED					0.10f //96.0f is too much!!!
#define GOOMBA_SPEED				0.05f
#define TILE_HEIGHT					32
#define TILE_WIDTH					32

//Gravity.
#define JUMP_FORCE_DECREMENT		400.0f
#define INITIAL_JUMP_FORCE			200.0f
#define INITIAL_JUMP_FORCE_SMALL	150.0f
#define GRAVITY						200.0f

//Level.
#define MAP_HEIGHT					13
#define MAP_WIDTH					16

//Enemy.
#define INJURED_TIME				2.5f
#define SPAWN_TIME					200.0f

//Misc.
#define CHARACTERSTATE_PLAYER_DEATH true
#define ANIMATION_DELAY				0.15f
#define COIN_VALUE					10

#endif // !CONSTANTS_H

