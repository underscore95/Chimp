#pragma once

// Define (to any value) to enable debug features
// Comment out to disable (defining to 0 will still enable)
//#define DEBUG_AUTOHOST_AUTOCONNECT 0 // Automatically host the server and connect to it
// Autostart prevents some packets being sent btw
//#define DEBUG_AUTOSTART_WITH_1_PLAYER 0 // Starts the game even if only one player connected (by sending a fake start packet)

//#define DEBUG_DISABLE_WAVE_SPAWNING 0
//#define DEBUG_LOTS_OF_LIVES 0

// Keep these always
#define DEBUG_BLOON_SPEED_MULTIPLIER 1.0f
#define DEBUG_TOWER_ATTACK_SPEED_MULTIPLIER 1.0f
#define DEBUG_PROJECTILE_SPEED_MULTIPLIER 250.0f