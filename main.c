#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/background.h"
#include "images/win.h"
#include "images/lose.h"
#include "images/mario.h"
#include"images/avocado.h"
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

void initialize(struct rect *playerP, struct ball *bP, struct rect *enemyP) {
  playerP->row = 80;
  playerP->col = 20;
  playerP->width =8;
  playerP->height = 25;

  bP->row = 80;
  bP->col = 120;
  bP->size = 3;
  bP->velRow = 1;
  bP->velCol = 1;

  enemyP->row = 80;
  enemyP->col = WIDTH - 20;
  enemyP->width = 8;
  enemyP->height = 25;

  vBlankCounter = 0;
}

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  struct rect player;
  struct ball b;
  struct rect enemy;

  enum gba_state state = START;
  drawFullScreenImageDMA(background);
  initialize(&player, &b, &enemy);
  int countdown = 1800;
  while (1) {
    currentButtons = BUTTONS;

    char timer[30];
    /*snprintf(timer, sizeof(timer), "score: %d", (int)vBlankCounter % 60);
    waitForVBlank();
    */

    if (countdown > 0) {
    countdown--;
    snprintf(timer, sizeof(timer), "time left: %02d:%02d s", countdown / 60, countdown % 60);
    } else {
      state = LOSE;
    }
    waitForVBlank();
  

    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      state = START;
      fillScreenDMA(GREEN);
      initialize(&player, &b, &enemy);
    }

    switch (state) {
      case START:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY;
          drawFullScreenImageDMA(background);
          vBlankCounter = 0;
        }
        char startScreenText[] = "PRESS ENTER TO START";
        drawCenteredString(HEIGHT / 2, WIDTH / 2, 0, 0, startScreenText, WHITE);
        break;
      case PLAY:
        // Clear the screen
        fillScreenDMA(GREEN);

        if (vBlankCounter % 600 == 0) {
          if (b.velCol > 0) {
            b.velCol++;
          } else {
            b.velCol--;
          }
        }

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          if (player.row - 1 < 0) {
            player.row = 0;
          } else {
            player.row -= 1;
          }
        }

        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          if (player.row + player.height + 1 > HEIGHT) {
            player.row = HEIGHT - player.height;
          } else {
            player.row += 1;
          }
        }

        if (b.row + b.velRow < 0) {
          b.velRow *= -1;
        }

        if (b.row + b.velRow + b.size > HEIGHT) {
          b.velRow *= -1;
        }

        if (b.col + b.velCol < 0) {
          state = LOSE;
          drawFullScreenImageDMA(lose);
        }

        if (b.col + b.velCol + b.size > WIDTH) {
          state = WIN;
          drawFullScreenImageDMA(win);
        }

        if (rectCollision(player, b) || rectCollision(enemy, b)) {
          b.velCol *= -1;
        }

        int enemyCenterRow = enemy.row + enemy.height / 2;
        if (b.row > enemyCenterRow) {
          if (enemy.row + enemy.height + 1 > HEIGHT) {
            enemy.row = HEIGHT - enemy.height;
          } else {
            enemy.row += 1;
          }
        }

        if (b.row < enemyCenterRow) {
          if (enemy.row - 1 < 0) {
            enemy.row = 0;
          } else {
            enemy.row -= 1;
          }
        }

        b.row += b.velRow;
        b.col += b.velCol;

        drawImageDMA(player.row, player.col, player.width, player.height, mario);
        drawImageDMA(enemy.row, enemy.col, enemy.width, enemy.height, avocado);
        drawRectDMA(b.row, b.col, b.size, b.size, WHITE);
    
        drawCenteredString(20, 120, 0, 0, timer, BLACK);
        break;
      case WIN:
        // Handle win state
        break;
      case LOSE:
        // Handle lose state
        break;
    }

    previousButtons = currentButtons;
  }

  UNUSED(previousButtons);

  return 0;
}

int rectCollision(struct rect rect, struct ball b) {
  return rect.col < b.col + b.size &&
         rect.col + rect.width > b.col &&
         rect.row < b.row + b.size &&
         rect.row + rect.height > b.row;
}