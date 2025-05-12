#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_AUDIO   "default_sfx" // Default audio name
#define USBFS_DEVICE    0             // USB device address
#define SHT40_ADDR      0x44          // I2C address of SHT40
#define MAX_FILES       12            // Max number of files to read
#define FILENAME_BUF    64            // Buffer size of file name
#define AUDIO_BUF_SIZE  4096          // Buffer size of audio data
#define TFT_HEIGHT      240           // TFT height in pixels
#define TFT_WIDTH       320           // TFT width in pixels
#define LOOP_PERIOD_MS  20            // Milliseconds delay per loop

// Game config
#define ACCEL_SCALE     0.3           // MPU accel to pixel accel
#define PLAYER_RADIUS   3             // Player circle radius
#define OBS_RADIUS      3             // Obstacle radius
#define GAME_MAP_HEIGHT 24            // Game map height
#define GAME_MAP_WIDTH  32            // Game map width
#define TILE_SIZE       10            // Tile size in pixels

// Color Themes
#define PLAYER_COLOR    GUI_GREEN     // Player color
#define BG_COLOR        GUI_BLACK     // Background color
#define FG_COLOR        GUI_WHITE     // Foreground color
#define SELECT_COLOR    GUI_RED       // Selected color
#define OBS_COLOR       GUI_RED       // Obstacle color
#define GOAL_COLOR      GUI_ORANGE    // Obstacle color

#endif // CONFIG_H

/* [] END OF FILE */