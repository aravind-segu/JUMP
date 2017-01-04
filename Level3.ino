#include <EEPROM.h>
#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#define LEDR RED_LED
#define LEDB BLUE_LED
#define LEDG GREEN_LED
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int topLeftL3 = 5;
int currentposL3 = 5;
int currentposlevL3 = 0;
int topRectL3 = 14;
int baseRectL3 = 5;
int ycorleftL3 = 5;
int baselevL3 = 0;
int speedL3 = 20;
int penaltyL3 = 0;
int scoreRecordL3 = 0;

void moveBackgroundL3(int shakeTime, int obstacles);
void startL4();
void drawFinishL3 (int x, int y);

void lostPageL3()
{
  OrbitOledMoveTo(50, 2);
  OrbitOledDrawString ("You Lost!!");
  for (int i = 0; i < 3; i++)
  {
    OrbitOledMoveTo ((10 * i) + 5, 2);
    OrbitOledDrawChar (playerName[i]);
  }
  OrbitOledUpdate();
  delay(2000);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  shakeTime = (int) millis();
  gameUiPage = LevelThree;
}

static void handlePageGameL3(int shakeTime)
{
  OrbitOledMoveTo(0, 0);
  OrbitOledFillRect(128, 5);

  OrbitOledMoveTo (0, 32);
  OrbitOledLineTo (0, 0);

  OrbitOledMoveTo (128, 32);
  OrbitOledLineTo (128, 0);

  OrbitOledMoveTo (0, 32);
  OrbitOledLineTo (128, 32);

  if (pressed == 0 && down == 0)
  {
    OrbitOledMoveTo(5, topLeftL3);
    OrbitOledFillRect(5 + squareSize, topLeftL3 + squareSize);
  }

  Time = ((int) ((millis() - shakeTime) / speedL3));
  OrbitOledMoveTo(5, 5);

  if (gameInputState.buttons[0].isRising && pressed == 0 && down == 0)
  {
    pressed = 1;
  }

  if (pressed == 1 && down == 0)
  {
    if (Timerecord == 0)
    {
      jumpTime = ((int) (millis()));
      Timerecord = 1;
    }

    tempTime = ((int) ((millis () - jumpTime) / speedJump) % 28);
    if ((currentposL3 + tempTime) <= 23)
    {
      OrbitOledMoveTo(5, currentposL3 + tempTime - 0.1 * (tempTime ^ 2));
      ycorleftL3 = currentposL3 + tempTime;
      OrbitOledFillRect(5 + squareSize, (currentposL3 + tempTime) + squareSize - 0.1 * (tempTime ^ 2));
    }
    else
    {
      pressed = 0;
      down = 1;
      Timerecord = 0;
      currentposL3 = 23;
    }
  }

  if (pressed == 0 && down == 1)
  {
    if (Timerecord == 0)
    {
      dropTime = ((int) (millis()));
      Timerecord = 1;
    }
    tempTime = ((int) ((millis () - dropTime) / speedJump) % 28);
    if ((currentposL3 - tempTime) > 5)
    {
      OrbitOledMoveTo(5, currentposL3 - tempTime + 0.1 * (tempTime ^ 2));
      ycorleftL3 = currentposL3 - tempTime;
      OrbitOledFillRect(5 + squareSize, (currentposL3 - tempTime) + squareSize + 0.1 * (tempTime ^ 2));
    }
    else
    {
      pressed = 0;
      down = 0;
      currentposL3 = 5;
      Timerecord = 0;
    }
  }

  moveBackgroundL3(shakeTime, 10);
  OrbitOledUpdate();
  delay(50);
  OrbitOledClear();
}

void moveBackgroundL3(int shakeTime, int obstacles)
{
  Time = ((int) ((millis() - shakeTime) / speedL3));
  i = 0;
  sumOfDistances = 0;

  if (entered == false)
  {
    dist =  (int *) malloc (((obstacles + 1) * sizeof(int)));
    typeOfObstacle =  (int *) malloc (((obstacles + 1) * sizeof(int)));
    xCorRectLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorTriLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorLevLeft = (int *) malloc ((obstacles * sizeof(int)));
    for (int j = 0; j <  (obstacles + 1); j++)
    {
      dist [j] = 60 + (rand() % 30);
      sum += dist [j];
      typeOfObstacle [j] = rand() % 2;
    }
    entered = true;
    typeOfObstacle[obstacles] = 5;
  }

  if (i != obstacles)
  {
    for (i = 0; i < (obstacles + 1); i++)
    {
      if (typeOfObstacle[i] == 0)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, topRectL3);
        xCorRectLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorRectLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((ycorleftL3) < topRectL3)
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL3 <= 50) {
              penaltyL3 += 10;
            }
            entered = false;
            gameUiPage = LostLevel3;
          }
        }
        OrbitOledFillRect(startPos + (dist[i] + sumOfDistances) + 5 - Time, baseRectL3);
      }

      else if (typeOfObstacle[i] == 1)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 5);
        fillInverseTriangle(triSize, startPos + (dist[i] + sumOfDistances) - Time, 5);
        xCorTriLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorTriLeft [i];

        if ( temp > 2 && temp <= 10)
        {
          if ( (ycorleftL3) < (16 - temp) )
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL3 <= 50)
            {
              penaltyL3 += 10;
            }
            entered = false;
            gameUiPage = LostLevel3;
          }
        }
        else if ( temp >= -10 && temp <= 2)
        {
          if ( (ycorleftL3) < (12 + temp))
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL3 <= 50)
            {
              penaltyL3 += 10;
            }
            entered = false;
            gameUiPage = LostLevel3;
          }
        }
      }
      else if (typeOfObstacle[i] == 5)
      {
        drawFinishL3(startPos + (dist[i] + sumOfDistances) - Time, 0);
        if (startPos + (dist[i] + sumOfDistances) - Time < 0)
        {
          if (scoreRecordL3 == 0)
          {
          score += 50 - penaltyL2;
          scoreRecordL3 = 1;
          }
          OrbitOledClear();
          penaltyL3 = 0;
          startL4();
        }
      }
      sumOfDistances += dist[i];
    }
  }
}

void startL4()
{
  OrbitOledMoveTo(50, 2);
  OrbitOledDrawString ("You Won!!");
  for (int i = 0; i < 3; i++)
  {
    OrbitOledMoveTo ((10 * i) + 5, 2);
    OrbitOledDrawChar (playerName[i]);
  }

  OrbitOledMoveTo (5, 22);
  OrbitOledDrawString("Your Score:");
  OrbitOledMoveTo (120, 22);
  countGame = 0;
  int tempScore = score;
  do {
    char printingNumber = chosenNumber (tempScore % 10);
    OrbitOledDrawChar(printingNumber);
    OrbitOledMoveTo((120 - 10 * (countGame + 1)), 22);
    countGame++;
    tempScore = tempScore / 10;
  } while (tempScore > 0);
  OrbitOledUpdate();
  delay(2000);
  OrbitOledClear();
  gameUiPage = NextL4;
}

void Lvl4CountDown(){
  
  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 4 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 4 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 4 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  shakeTime = (int) millis();
  entered = false;
  gameUiPage = LevelFour;
}
void drawFinishL3 (int x, int y)
{
  //Draw Flags
  OrbitOledMoveTo(x, y);
  OrbitOledLineTo(x, y + 25);
  OrbitOledMoveTo(x - 4, y + 25);
  fillTriangle(9, x - 4, y + 25);
  OrbitOledMoveTo(x - 4, y + 20);
  fillInverseTriangle(9, x - 4, y + 25);
  OrbitOledMoveTo(x + 42, y);
  OrbitOledLineTo(x + 42, y + 25);
  OrbitOledMoveTo(x + 38, y + 25);
  fillTriangle(9, x + 38, y + 25);
  OrbitOledMoveTo(x + 38, y + 20);
  fillInverseTriangle(9, x + 38, y + 25);
  //Draw Castle
  OrbitOledMoveTo(x + 8, y);
  OrbitOledFillRect(x + 16, y + 11);
  OrbitOledMoveTo(x + 26, y);
  OrbitOledFillRect(x + 34, y + 11);
  OrbitOledMoveTo(x + 8, y + 11);
  OrbitOledFillRect(x + 34, y + 15);
  //Draw Mini Towers
  OrbitOledMoveTo(x + 12, y + 15);
  OrbitOledFillRect(x + 18, y + 21);
  OrbitOledMoveTo(x + 24, y + 15);
  OrbitOledFillRect(x + 30, y + 21);
  //Draw Mini-mini towers
  OrbitOledMoveTo(x + 12, y + 21);
  OrbitOledFillRect(x + 14, y + 23);
  OrbitOledMoveTo(x + 16, y + 21);
  OrbitOledFillRect(x + 18, y + 23);
  OrbitOledMoveTo(x + 24, y + 21);
  OrbitOledFillRect(x + 26, y + 23);
  OrbitOledMoveTo(x + 28, y + 21);
  OrbitOledFillRect(x + 30, y + 23);
}
