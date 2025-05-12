#include "game.h"

#include <stdint.h>

#include "GUI.h"
#include "config.h"
#include "gamemap.h"
#include "player.h"

// Variables to store game states
static int startX = 0;
static int startY = 0;
static int endX = 0;
static int endY = 0;
static uint8_t isGameEnded = 0;

/*
This function initializes the map and the player.
*/
void InitGame() {
  isGameEnded = 0;
  ProcessMap();
  InitPlayer(startX, startY);
}

/*
This function resets the game (e.g. when the player dies)
*/
void ResetGame() {
  InitPlayer(startX, startY);
}

/*
This function ends the game (e.g. when the player enters the goal square)
*/
void EndGame() {
  isGameEnded = 1;
  GUI_Clear();
}

/*
This function draws the map on the TFT display and set the game states
from the map.
*/
void ProcessMap() {
  // Iterate through the map
  int i, j;
  for (i = 0; i < GAME_MAP_HEIGHT; i++) {
    for (j = 0; j < GAME_MAP_WIDTH; j++) {
      if (GAME_MAP[i][j] == '#') {
        // The '#' symbol is for the wall
        GUI_SetColor(FG_COLOR);
        GUI_FillRect(j * TILE_SIZE, i * TILE_SIZE, (j + 1) * TILE_SIZE - 1,
                     (i + 1) * TILE_SIZE - 1);
      } else if (GAME_MAP[i][j] == 'x') {
        // The 'x' symbol is for the obstacle
        GUI_SetColor(OBS_COLOR);
        GUI_FillCircle(j * TILE_SIZE + TILE_SIZE / 2,
                       i * TILE_SIZE + TILE_SIZE / 2, OBS_RADIUS);
      } else if (GAME_MAP[i][j] == 'S') {
        // The 'S' symbol is for the starting location
        startX = j * TILE_SIZE + TILE_SIZE / 2;
        startY = i * TILE_SIZE + TILE_SIZE / 2;
      } else if (GAME_MAP[i][j] == 'E') {
        // The 'E' symbol is for the ending location
        endX = j * TILE_SIZE + TILE_SIZE / 2;
        endY = i * TILE_SIZE + TILE_SIZE / 2;

        // Draw the end square with a different color
        GUI_SetColor(GOAL_COLOR);
        GUI_FillRect(j * TILE_SIZE, i * TILE_SIZE, (j + 1) * TILE_SIZE - 1,
                     (i + 1) * TILE_SIZE - 1);
      }
    }
  }
}

/*
This function is continuously ran while game is playing.
*/
void GameLoop(MPU_DATA_t mpuData) {
  if (isGameEnded) {

    // If game ended, congratulate user
    GUI_SetFont(&GUI_Font32_1);
    GUI_SetColor(FG_COLOR);
    GUI_DispStringAt("Congratulations!", 10, 70);
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispStringAt("Press button to return", 10, 120);

  } else {

    // If the player collided with an obstacle, restart
    if (IsPlayerObsCollide()) {
      ResetGame();
    }

    // If the player collided with the end square, end game
    if (IsPlayerEndCollide()) {
      EndGame();
      return;
    }

    // Draw and update player state
    DrawPlayer();
    UpdatePlayer(mpuData);
  }
}

/* [] END OF FILE */