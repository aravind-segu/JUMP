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
#include <math.h>

const uint32_t SwitchCount = 2;
const uint32_t ButtonCount = 2;
const uint32_t Buttons[ButtonCount] = { PD_2, PE_1 };
const uint32_t Potentiometer = PE_3;
int shakeTime;
int topLeft = 22;
int pressed = 0;
int down = 0;
int once = 0;
int t;
int countGame;
int sum = 0;
const int squareSize = 5;
const int speed = 30;
const int speedJump = 30;
const int speedLev = 100;
char playerName [3];
int jumpTime;
int dropTime;
int alreadyEntered;
int Time;
int Timerecord = 0;
int currentpos = 22;
int currentposlev = 27;
int currentposObstacle = 22;
bool entered = false;
int *dist;
int *typeOfObstacle;
int ycorleft = 22;
int *xCorRectLeft;
int *xCorLevLeft;
int *xCorTriLeft;
int *xCorPlatLeft;
int startPos = 128;
int topRect = 18;
int baseRect = 27;
int triSize = 15;
int indexChar = 0;
int charSelected = 0;
unsigned char selectedChar [3];
int baselev = 27;
int temp;
int tempTime;
double levSpeed;
int tempTimeLev;
int score = 0;
int tempScore;
int penalty = 0;
int highScores [5] = {0};
int i;
int titleLimit = 128;
int sumOfDistances;

void fillTriangle (int base, int x, int y);
void fillInverseTriangle (int base, int x, int y);
void levitator (int x, int y);
void GameUIInit();
static void handlePageWelcome();
static void uiInputTick();
void GameUITick();
void lostPage();
void startL2();
void startGame ();
void setPlayersName();
char chosenCharacter (int input);
void moveBackground(int shakeTime, int obstacles);
void drawFinish (int x, int y);
static void handlePageGame(int shakeTime);

static enum GamePages
{
  Welcome       = 0,
  PlayerName =    1,
  Game          = 2,
  GameStart  = 3,
  DisplayScores  = 4,
  Progress      = 5,
  Lost = 6,
  NumberOfPages = 7,
  LevelTwo = 8,
  LostLevel3 = 9,
  LevelThree = 10,
  LostLevel2 = 11,
  LevelFour = 12,
  LostLevel4 = 13,
  LevelFive = 14,
  LostLevel5 = 15,
  HighScores = 16,
  L2CountDown = 17,
  L3CountDown = 18,
  L4CountDown = 19,
  L5CountDown = 20,
  NextL2 = 21,
  NextL3 = 22,
  NextL4 = 23,
  NextL5 = 24,
} gameUiPage = Welcome;

void fillTriangle (int base, int x, int y)
{
  for (int i = base; i > 0; i -= 2)
  {
    OrbitOledLineTo(x + i, y);
    OrbitOledMoveTo(++x , --y);
  }
}

void fillInverseTriangle (int base, int x, int y)
{
  for (int i = base; i > 0; i -= 2)
  {
    OrbitOledLineTo(x + i, y);
    OrbitOledMoveTo(++x , ++y);
  }
}

void levitator (int x, int y)
{
  int size = 5;
  OrbitOledLineTo(x + size, y);
  if (y < 22)
  {
    baselev = 40;
  }
}

struct ButtonState
{
  bool state;
  bool isRising;
};

static struct InputState
{
  struct ButtonState  buttons[2];
  float               potentiometer;
} gameInputState;

void GameUIInit()
{
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

  gameInputState = { 0 };

  for (int i = 0; i < ButtonCount; ++i )
    pinMode(Buttons[i], INPUT);
  pinMode (LEDR, OUTPUT);
  pinMode (LEDB, OUTPUT);
  pinMode (LEDG, OUTPUT);
}

static void handlePageProgressBar()
{
  static uint32_t progress = 0;
  if (0 == (progress = (1 + progress) % 100))
  {
    OrbitOledClear();
  }
  OrbitOledMoveTo(30, 24);
  OrbitOledDrawString("Loading...");

  OrbitOledMoveTo(14, 14);
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnBlank));
  OrbitOledSetDrawMode(modOledAnd);
  OrbitOledFillRect(14 + progress, 16);

  OrbitOledMoveTo(14 + progress, 14);
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
  OrbitOledFillRect(14 + progress + 10, 16);
}

static void handlePageWelcome()
{
  int switchDirection = 1;
  Time = ((int) ((millis() / 50)));
  OrbitOledMoveTo(56, 5);
  OrbitOledDrawString ("JUMP");

  OrbitOledUpdate();
  delay(100);
  OrbitOledClear();
  OrbitOledMoveTo(18 + 2 * sin(Time), 18 + 2 * sin(Time));
  OrbitOledDrawString("Shake to Play");

  if (ShakeIsShaking())
  {
    OrbitOledClearBuffer();
    OrbitOledClear();
    shakeTime = (int) millis();
    gameUiPage = PlayerName;
  }
}

static void uiInputTick()
{
  for (int i = 0; i < ButtonCount; ++i )
  {
    bool previousState = gameInputState.buttons[i].state;
    gameInputState.buttons[i].state = digitalRead(Buttons[i]);
    gameInputState.buttons[i].isRising = (!previousState && gameInputState.buttons[i].state);
  }
  gameInputState.potentiometer = analogRead(Potentiometer);
}

void GameUITick()
{
  uiInputTick();
  switch (gameUiPage)
  {
    case Welcome:
      handlePageWelcome();
      break;

    case PlayerName:
      setPlayersName();
      break;

    case Game:
      handlePageGame(shakeTime);
      break;

    case GameStart:
      startGame();
      break;

    case Lost:
      lostPage();
      break;

    case LostLevel2:
      lostPageL2();
      break;

    case LostLevel3:
      lostPageL3();
      break;

    case LostLevel4:
      lostPageL4();
      break;

    case LostLevel5:
      lostPageL5();
      break;

    case LevelTwo:
      handlePageGameL2(shakeTime);
      break;

    case LevelThree:
      handlePageGameL3(shakeTime);
      break;

    case LevelFour:
      handlePageGameL4(shakeTime);
      break;

    case LevelFive:
      handlePageGameL5(shakeTime);
      break;

    case HighScores:
      highScoresPrint();
      break;
      
    case L2CountDown:
      Lvl2CountDown();
      break;
      
    case L3CountDown:
      Lvl3CountDown();
      break;
      
    case L4CountDown:
      Lvl4CountDown();
      break;
      
    case L5CountDown:
      Lvl5CountDown();
      break;
      
    case NextL2:
      next(2);
      break;
      
    case NextL3:
      next(3);
      break;
      
    case NextL4:
      next(4);
      break;
      
    case NextL5:
      next(5);
      break;
      
    default:
      handlePageProgressBar();
      break;
  }
  OrbitOledUpdate();
}

void lostPage()
{
  OrbitOledMoveTo(50, 13);
  OrbitOledDrawString ("You Lost!!");
  for (int i = 0; i < 3; i++)
  {
    OrbitOledMoveTo ((10 * i) + 5, 13);
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
  gameUiPage = Game;
}
void startL2()
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
  gameUiPage = NextL2;

}

int next (int level) {
  int inputOption = analogRead(Potentiometer);
  int selected = (int)(inputOption / 2047);
  OrbitOledMoveTo(0, 2);
  OrbitOledDrawString("Press BTN 1");
  if (selected == 0) {
    OrbitOledMoveTo (30, 16);
    OrbitOledDrawString("Continue");
    if (gameInputState.buttons[0].isRising) {
      OrbitOledClear();
      if (level == 2) {
        gameUiPage = L2CountDown;
      } else if (level == 3) {
        gameUiPage = L3CountDown;
      } else if (level == 4) {
        gameUiPage = L4CountDown;
      } else if (level == 5) {
        gameUiPage = L5CountDown;
      }

    }
  } else {
    OrbitOledMoveTo (30, 16);
    OrbitOledDrawString ("Quit      ");
    if (gameInputState.buttons[0].isRising) {
      OrbitOledClear();
      saveData (score, playerName);
      gameUiPage = HighScores;
    }
  }

  OrbitOledUpdate();
}

void Lvl2CountDown() {

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 2 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 2 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 2 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  shakeTime = (int) millis();
  entered = false;
  gameUiPage = LevelTwo;
}

void startGame ()
{
  OrbitOledMoveTo(0, 2);
  OrbitOledDrawString ("Welcome: ");
  for (int i = 0; i < 3; i++) {
    OrbitOledMoveTo ((10 * i) + 75, 2);
    OrbitOledDrawChar (playerName[i]);
  }
  
  OrbitOledMoveTo (5, 22);
  OrbitOledDrawString("Game Starts In:");
  OrbitOledUpdate();
  delay(2000);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(500);
  OrbitOledClear();

  shakeTime = (int) millis();
  gameUiPage = Game;
}

void setPlayersName()
{
  OrbitOledMoveTo(0, 2);
  OrbitOledDrawString("Enter Your Name:");

  OrbitOledMoveTo(39, 25);
  OrbitOledFillRect (49, 27);

  OrbitOledMoveTo(59, 25);
  OrbitOledFillRect (69, 27);

  OrbitOledMoveTo(79, 25);
  OrbitOledFillRect (89, 27);

  float input = analogRead(Potentiometer);
  char chosenChar = chosenCharacter((int)(input / 157));
  if (input >= 13)
  {
    OrbitOledMoveTo(41 + indexChar, 17);
    OrbitOledDrawChar(chosenChar);
  }

  if (gameInputState.buttons[0].isRising)
  {
    if (charSelected == 2)
    {
      playerName[charSelected] = chosenChar;
      charSelected = 0;
      indexChar = 0;
      shakeTime = (int) millis();
      //store (playerName, 80);
      OrbitOledClear();
      gameUiPage = GameStart;
    }
    else
    {
      playerName[charSelected] = chosenChar;
      charSelected++;
      indexChar += 20;
    }
  }
}

char chosenCharacter (int input)
{
  switch (input)
  {
    case 1: return 'A';
    case 2: return 'B';
    case 3: return 'C';
    case 4: return 'D';
    case 5: return 'E';
    case 6: return 'F';
    case 7: return 'G';
    case 8: return 'H';
    case 9: return 'I';
    case 10: return 'J';
    case 11: return 'K';
    case 12: return 'L';
    case 13: return 'M';
    case 14: return 'N';
    case 15: return 'O';
    case 16: return 'P';
    case 17: return 'Q';
    case 18: return 'R';
    case 19: return 'S';
    case 20: return 'T';
    case 21: return 'U';
    case 22: return 'V';
    case 23: return 'W';
    case 24: return 'X';
    case 25: return 'Y';
    case 26: return 'Z';
    default: return ' ';
  }
}

void moveBackground(int shakeTime, int obstacles)
{
  //EEPROMMassErase();
  int  Time = ((int) ((millis() - shakeTime) / speed));

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
      // Serial.println (sum);
      typeOfObstacle [j] = rand() % 2;
    }
    entered = true;
    typeOfObstacle[obstacles] = 5;
  }

  i = 0;
  sumOfDistances = 0;
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
            if (penalty <= 50)
            {
              penalty += 10;
            }
            gameUiPage = Lost;
          }
        }
        OrbitOledFillRect(startPos + (dist[i] + sumOfDistances) + 5 - Time, baseRect);
      }

      else if (typeOfObstacle[i] == 1)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 27);
        fillTriangle(triSize, startPos + (dist[i] + sumOfDistances) - Time, 27);
        xCorTriLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorTriLeft [i];

        if ( temp > 2 && temp <= 10)
        {
          if ( (ycorleft + squareSize) > 27 - (10 - temp) )
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penalty <= 50) {
              penalty += 10;
            }
            gameUiPage = Lost;
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
            if (penalty <= 50) {
              penalty += 10;
            }
            gameUiPage = Lost;
          }
        }
      }
      else if (typeOfObstacle[i] == 5)
      {
        drawFinish(startPos + (dist[i] + sumOfDistances) - Time, 27);
        if (startPos + (dist[i] + sumOfDistances) - Time < 0)
        {
          score = 50 - penalty;
          OrbitOledClear();
          penalty = 0;
          startL2();

        }
      }
      sumOfDistances += dist[i];
    }
  }
}

void drawFinish (int x, int y)
{
  //Draw Flags
  OrbitOledMoveTo(x, y);
  OrbitOledLineTo(x, y - 25);
  OrbitOledMoveTo(x - 4, y - 25);
  fillInverseTriangle(9, x - 4, y - 25);
  OrbitOledMoveTo(x - 4, y - 20);
  fillTriangle(9, x - 4, y - 25);
  OrbitOledMoveTo(x + 42, y);
  OrbitOledLineTo(x + 42, y - 25);
  OrbitOledMoveTo(x + 38, y - 25);
  fillInverseTriangle(9, x + 38, y - 25);
  OrbitOledMoveTo(x + 38, y - 20);
  fillTriangle(9, x + 38, y - 25);
  //Draw Castle
  OrbitOledMoveTo(x + 8, y);
  OrbitOledFillRect(x + 16, y - 11);
  OrbitOledMoveTo(x + 26, y);
  OrbitOledFillRect(x + 34, y - 11);
  OrbitOledMoveTo(x + 8, y - 11);
  OrbitOledFillRect(x + 34, y - 15);
  //Draw Mini Towers
  OrbitOledMoveTo(x + 12, y - 15);
  OrbitOledFillRect(x + 18, y - 21);
  OrbitOledMoveTo(x + 24, y - 15);
  OrbitOledFillRect(x + 30, y - 21);
  //Draw Mini-mini towers
  OrbitOledMoveTo(x + 12, y - 21);
  OrbitOledFillRect(x + 14, y - 23);
  OrbitOledMoveTo(x + 16, y - 21);
  OrbitOledFillRect(x + 18, y - 23);
  OrbitOledMoveTo(x + 24, y - 21);
  OrbitOledFillRect(x + 26, y - 23);
  OrbitOledMoveTo(x + 28, y - 21);
  OrbitOledFillRect(x + 30, y - 23);
}

static void handlePageGame(int shakeTime)
{
  OrbitOledMoveTo(0, 27);
  OrbitOledFillRect(128, 32);

  OrbitOledMoveTo (0, 27);
  OrbitOledLineTo (0, 0);

  OrbitOledMoveTo (128, 27);
  OrbitOledLineTo (128, 0);

  OrbitOledMoveTo (0, 0);
  OrbitOledLineTo (128, 0);

  if (pressed == 0 && down == 0)
  {
    OrbitOledMoveTo(5, topLeft);
    OrbitOledFillRect(5 + squareSize, topLeft + squareSize);
  }
  int  Time = ((int) ((millis() - shakeTime) / speed));
  OrbitOledMoveTo(5, 22);

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
    if ((currentpos - tempTime) >= 4) {
      OrbitOledMoveTo(5, currentpos - tempTime + 0.1 * (tempTime ^ 2));
      ycorleft = currentpos - tempTime;
      OrbitOledFillRect(5 + squareSize, (currentpos - tempTime) + squareSize + 0.1 * (tempTime ^ 2));
    } else {
      pressed = 0;
      down = 1;
      Timerecord = 0;
      currentpos = 4;
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

  moveBackground(shakeTime, 5);
  OrbitOledUpdate();
  delay(50);
  OrbitOledClear();
}

