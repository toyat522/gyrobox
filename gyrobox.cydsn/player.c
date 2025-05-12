#include "player.h"

#include "GUI.h"
#include "config.h"
#include "gamemap.h"

// Variables to store player states
static float x = 0;
static float y = 0;
static float prevx = 0;
static float prevy = 0;
static float xvel = 0;
static float yvel = 0;

/*
This function returns the player x.
*/
int GetPlayerX() { return (int)x; }

/*
This function returns the player y.
*/
int GetPlayerY() { return (int)y; }

/*
This function initializes the player state.
*/
void InitPlayer(int startX, int startY) {
  x = startX;
  y = startY;
}

uint8_t IsPlayerObsCollide() {
  // Iterate through rectangle bounding player
  int dx, dy;
  for (dy = -PLAYER_RADIUS; dy <= PLAYER_RADIUS; dy++) {
    for (dx = -PLAYER_RADIUS; dx <= PLAYER_RADIUS; dx++) {
      int pixelTileX = (GetPlayerX() + dx) / TILE_SIZE;
      int pixelTileY = (GetPlayerY() + dy) / TILE_SIZE;
      int dCenterX = TILE_SIZE / 2 - (GetPlayerX() + dx) % TILE_SIZE;
      int dCenterY = TILE_SIZE / 2 - (GetPlayerY() + dy) % TILE_SIZE;

      // If the pixel in consideration is in a tile with an obstacle and
      // it is part of the player pixel and
      // part of the obstacle pixel, then it is a collision
      if (GAME_MAP[pixelTileY][pixelTileX] == 'x' &&
          dx * dx + dy * dy < (PLAYER_RADIUS + 0.5) * (PLAYER_RADIUS + 0.5) &&
          dCenterX * dCenterX + dCenterY * dCenterY <
              (OBS_RADIUS + 0.5) * (OBS_RADIUS + 0.5)) {
        return 1;
      }
    }
  }
  // If no pixels of the player is in an obstacle, there is no collision
  return 0;
}

/*
This function checks if the player has collided with a wall.
*/
uint8_t IsPlayerWallCollide() {
  // Iterate through rectangle bounding player
  int dx, dy;
  for (dy = -PLAYER_RADIUS; dy <= PLAYER_RADIUS; dy++) {
    for (dx = -PLAYER_RADIUS; dx <= PLAYER_RADIUS; dx++) {
      int pixelTileX = (GetPlayerX() + dx) / TILE_SIZE;
      int pixelTileY = (GetPlayerY() + dy) / TILE_SIZE;

      // If the pixel in consideration is in the wall, it is a collision
      if (GAME_MAP[pixelTileY][pixelTileX] == '#') {
        return 1;
      }
    }
  }
  // If no pixels of the player is in the wall, there is no collision
  return 0;
}

/*
This function checks if the player has collided with the ending square.
*/
uint8_t IsPlayerEndCollide() {
  // Iterate through rectangle bounding player
  int dx, dy;
  for (dy = -PLAYER_RADIUS; dy <= PLAYER_RADIUS; dy++) {
    for (dx = -PLAYER_RADIUS; dx <= PLAYER_RADIUS; dx++) {
      int pixelTileX = (GetPlayerX() + dx) / TILE_SIZE;
      int pixelTileY = (GetPlayerY() + dy) / TILE_SIZE;

      // If the pixel in consideration is in the end square, it is a collision
      if (GAME_MAP[pixelTileY][pixelTileX] == 'E') {
        return 1;
      }
    }
  }
  // If no pixels of the player is in the end square, there is no collision
  return 0;
}

/*
This function updates the player states.
*/
void UpdatePlayer(MPU_DATA_t mpuData) {
  // Set previous x, y values
  prevx = x;
  prevy = y;

  // Update player x
  xvel -= mpuData.yAccel * ACCEL_SCALE;
  x += xvel;

  // If new position causes player to collide with wall, revert
  if (IsPlayerWallCollide()) {
    xvel = 0;
    x = prevx;
  }

  // Update player y
  yvel += mpuData.zAccel * ACCEL_SCALE;
  y += yvel;

  // If new position causes player to collide with wall, revert
  if (IsPlayerWallCollide()) {
    yvel = 0;
    y = prevy;
  }

  // Keep player in TFT boundary
  if (x <= PLAYER_RADIUS) {
    xvel = 0;
    x = PLAYER_RADIUS;
  } else if (x >= TFT_WIDTH - PLAYER_RADIUS - 1) {
    xvel = 0;
    x = TFT_WIDTH - PLAYER_RADIUS - 1;
  }
  if (y <= PLAYER_RADIUS) {
    yvel = 0;
    y = PLAYER_RADIUS;
  } else if (y >= TFT_HEIGHT - PLAYER_RADIUS - 1) {
    yvel = 0;
    y = TFT_HEIGHT - PLAYER_RADIUS - 1;
  }
}

/*
This function draws the player on the screen.
*/
void DrawPlayer() {
  GUI_SetColor(BG_COLOR);
  GUI_FillCircle(prevx, prevy, PLAYER_RADIUS);
  GUI_SetColor(PLAYER_COLOR);
  GUI_FillCircle(x, y, PLAYER_RADIUS);
}

/* [] END OF FILE */