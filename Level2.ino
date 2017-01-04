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

int levSize = 5;
bool enteredlev = false;
int penaltyL2 = 0;
int speedL2 = 20;
int scoreRecord = 0;


void moveDown (int tempTime);
void jump (int tempTime, int maxHeight);
void moveBackgroundL2(int shakeTime, int obstacles);



void lostPageL2() {
  OrbitOledMoveTo(50, 2);
  OrbitOledDrawString ("You Lost!!");
  for (int i = 0; i < 3; i++) {
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
  enteredlev = false;
  shakeTime = (int) millis();
  gameUiPage = LevelTwo;
}

static void handlePageGameL2(int shakeTime)
{
  OrbitOledMoveTo(0, 27);
  OrbitOledFillRect(128, 32);

  OrbitOledMoveTo (0, 27);
  OrbitOledLineTo (0, 0);

  OrbitOledMoveTo (128, 27);
  OrbitOledLineTo (128, 0);

  OrbitOledMoveTo (0, 0);
  OrbitOledLineTo (128, 0);

  if (pressed == 0 && down == 0 && enteredlev == false)
  {
    OrbitOledMoveTo(5, topLeft);
    OrbitOledFillRect(5 + squareSize, topLeft + squareSize);
  }

  int  Time = ((int) ((millis() - shakeTime) / speedL2));
  OrbitOledMoveTo(5, 22);

  if (gameInputState.buttons[0].isRising && pressed == 0 && down == 0)
  {
    pressed = 1;
  }

  if (pressed == 1 && down == 0)
  {
    jump(tempTime, 4, speedJump);
  }


  if (pressed == 0 && down == 1)
  {
    moveDown (tempTime, speedJump);
  }

  moveBackgroundL2(shakeTime, 8);
  OrbitOledUpdate();
  delay(50);
  OrbitOledClear();
}

void moveDown (int tempTime, int speedJump)
{
  if (Timerecord == 0)
  {
    dropTime = ((int) (millis()));
    Timerecord = 1;
  }
  tempTime = ((int) ((millis () - dropTime) / speedJump) % 28);
  if ((currentpos + tempTime) <= 22)
  {
    OrbitOledMoveTo(5, currentpos + tempTime - 0.1 * (tempTime ^ 2));
    ycorleft = currentpos + tempTime;
    OrbitOledFillRect(5 + squareSize, (currentpos + tempTime) + squareSize - 0.1 * (tempTime ^ 2));
  }
  else
  {
    pressed = 0;
    down = 0;
    currentpos = 22;
    Timerecord = 0;
  }
}

void jump (int tempTime, int maxHeight, int speedJump)
{
  if (Timerecord == 0)
  {
    jumpTime = ((int) (millis()));
    Timerecord = 1;
  }

  tempTime = ((int) ((millis () - jumpTime) / speedJump) % 28);
  if ((currentpos - tempTime) >= maxHeight)
  {
    OrbitOledMoveTo(5, currentpos - tempTime + 0.1 * (tempTime ^ 2));
    ycorleft = currentpos - tempTime;
    OrbitOledFillRect(5 + squareSize, (currentpos - tempTime) + squareSize + 0.1 * (tempTime ^ 2));
  }
  else
  {
    pressed = 0;
    down = 1;
    Timerecord = 0;
    currentpos = maxHeight;
  }
}

void moveBackgroundL2(int shakeTime, int obstacles)
{
  Time = ((int) ((millis() - shakeTime) / speedL2));
  i = 0;
  sumOfDistances = 0;

  if (entered == false)
  {
    dist = (int *) malloc (((obstacles + 1) * sizeof(int)));
    typeOfObstacle =  (int *) malloc (((obstacles + 1) * sizeof(int)));
    xCorRectLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorTriLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorLevLeft = (int *) malloc ((obstacles * sizeof(int)));
    for (int j = 0; j <  (obstacles + 1); j++)
    {
      dist [j] = 80 + (rand() % 30);
      sum += dist [j];
      typeOfObstacle [j] = rand() % 3;
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
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, topRect);
        xCorRectLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorRectLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((ycorleft + squareSize) > topRect)
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL2 <= 50)
            {
              penaltyL2 += 10;
            }
            entered = false;
            gameUiPage = LostLevel2;
          }
        }
        OrbitOledFillRect(startPos + (dist[i] + sumOfDistances) + 5 - Time, baseRect);
      }
      else if (typeOfObstacle[i] == 1)
      {
        if (baselev == 27)
        {
          t = (int) millis();
          baselev--;
        }
        tempTimeLev = ((int) ((millis () - t) / speedLev) % 28);
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, currentposlev - tempTimeLev);
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + levSize, currentposlev - tempTimeLev);
        if (currentposlev - tempTimeLev < 20)
        {
          baselev = 27;
        }

        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, currentposlev - tempTimeLev + 3);
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + levSize, currentposlev - tempTimeLev + 3);

        if (currentposlev - tempTimeLev + 3 < 20)
        {
          baselev = 27;
        }

        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, currentposlev - tempTimeLev + 6);
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + levSize, currentposlev - tempTimeLev + 6);

        if (currentposlev - tempTimeLev + 6 < 20)
        {
          baselev = 27;
        }

        xCorLevLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorLevLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((ycorleft + squareSize) > 20)
          {
            enteredlev = true;
            pressed = 0;
            down = 0;
            currentpos = 22;
            Timerecord = 0;
          }
        }
        if (enteredlev == true)
        {
          jump(tempTime, -5, 15);
        }

        if (ycorleft < 0)
        {
          digitalWrite (LEDR, HIGH);
          delay (2000);
          digitalWrite (LEDR, LOW);
          OrbitOledClearBuffer();
          OrbitOledClear();
          if (penaltyL2 <= 50)
          {
            penaltyL2 += 10;
          }
          pressed = 0;
          down = 0;
          currentpos = 22;
          Timerecord = 0;
          entered = false;
          enteredlev = false;
          ycorleft = 22;
          gameUiPage = LostLevel2;
        }
      }
      else if (typeOfObstacle[i] == 2)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 27);
        fillTriangle(triSize, startPos + (dist[i] + sumOfDistances) - Time, 27);
        xCorTriLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorTriLeft [i];

        if ( temp > 2 && temp <= 10)
        {
          if ( (ycorleft + squareSize) > 27 - (10 - temp) )
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL2 <= 50)
            {
              penaltyL2 += 10;
            }
            entered = false;
            gameUiPage = LostLevel2;
          }
        }
        else if ( temp >= -10 && temp <= 2)
        {
          if ( (ycorleft + squareSize) > 19 + (-2 - temp))
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL2 <= 50)
            {
              penaltyL2 += 10;
            }
            entered = false;
            gameUiPage = LostLevel2;
          }
        }
      }
      else if (typeOfObstacle[i] == 5)
      {
        drawFinish(startPos + (dist[i] + sumOfDistances) - Time, 27);
        if (startPos + (dist[i] + sumOfDistances) - Time < 0)
        {
          if (scoreRecord == 0)
          {
          score += 50 - penaltyL2;
          scoreRecord = 1;
          }
          OrbitOledClear();
          penaltyL2 = 0;
          startL3();
        }
      }
      sumOfDistances += dist[i];
    }
  }
}

void startL3()
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
  tempScore = score;
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
  gameUiPage = NextL3;
  
}

void Lvl3CountDown(){
  
  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 3 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 3 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 3 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  shakeTime = (int) millis();
  entered = false;
  gameUiPage = LevelThree;
}
