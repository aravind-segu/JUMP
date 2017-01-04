int platformSize = 40;
bool onPlatform = false;
int pressedL4 = 0;
int downL4 = 0;
int TimeRecordL4 = 0;
int currentposL4 = 22;
bool enteredPlatform = false;
bool nowOnPlatform = false;
bool gettingOffPlatform = false;
int minHeight = 27;
int penaltyL4  = 0;
int speedL4 = 15;
int scoreRecordL4 = 0;

void jumpL5 (int tempTime, int maxHeight);
void drawPlatform(int size, int x, int y);
void movedown (int tempTime);
void moveDownL4 (int tempTime, int minHeight, int maximum);
void moveOffPlatform (int tempTime);

void moveBackgroundL4(int shakeTime, int obstacles);
void startL5();


void lostPageL4()
{
  OrbitOledMoveTo(50, 2);
  OrbitOledDrawString ("You Lost!!");
  for (int i = 0; i < 3; i++)
  {
    OrbitOledMoveTo ((10 * i) + 5, 2);
    OrbitOledDrawChar (playerName[i]);
  }

  OrbitOledUpdate();
  delay(500);
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
  delay(1000);
  OrbitOledClear();

  shakeTime = (int) millis();
  gameUiPage = LevelFour;
}

void jumpL5 (int tempTime, int maxHeight)
{
  if (TimeRecordL4 == 0)
  {
    jumpTime = ((int) (millis()));
    TimeRecordL4 = 1;
  }

  tempTime = ((int) ((millis () - jumpTime) / speedJump) % 28);
  if ((currentposL4 - tempTime) >= maxHeight)
  {
    OrbitOledMoveTo(5, currentposL4 - tempTime + 0.1 * (tempTime ^ 2));
    ycorleft = currentposL4 - tempTime;
    OrbitOledFillRect(5 + squareSize, (currentposL4 - tempTime) + squareSize + 0.1 * (tempTime ^ 2));
  }
  else
  {
    pressedL4 = 0;
    downL4 = 1;
    TimeRecordL4 = 0;
    currentposL4 = maxHeight;
  }
}

void drawPlatform(int size, int x, int y)
{
  OrbitOledMoveTo(x, y);
  OrbitOledFillRect(x + platformSize, y - 5);

  for (int i = 0; i < size / 5; i++)
  {
    OrbitOledMoveTo(x + i * 5, 27);
    fillTriangle(5, x + i * 5, 27);
  }
}

void movedown (int tempTime)
{
  if (TimeRecordL4 == 0)
  {
    dropTime = ((int) (millis()));
    TimeRecordL4 = 1;
  }
  tempTime = ((int) ((millis () - dropTime) / speedJump) % 28);
  if ((currentposL4 + tempTime) <= 22)
  {
    OrbitOledMoveTo(5, currentposL4 + tempTime - 0.1 * (tempTime ^ 2));
    ycorleft = currentposL4 + tempTime;
    OrbitOledFillRect(5 + squareSize, (currentposL4 + tempTime) + squareSize - 0.1 * (tempTime ^ 2));
  }
  else
  {
    pressedL4 = 0;
    downL4 = 0;
    currentposL4 = 22;
    TimeRecordL4 = 0;
  }
}


void moveDownL4 (int tempTime, int minHeight, int maximum)
{
  if (TimeRecordL4 == 0)
  {
    dropTime = ((int) (millis()));
    TimeRecordL4 = 1;
  }
  tempTime = ((int) ((millis () - dropTime) / speedJump) % 28);
  if ((minHeight + tempTime) <= maximum && nowOnPlatform == false)
  {
    OrbitOledMoveTo(5, minHeight - squareSize + tempTime - 0.1 * (tempTime ^ 2));
    ycorleft = minHeight - squareSize + tempTime;
    OrbitOledFillRect(5 + squareSize, (minHeight - squareSize + tempTime) + squareSize - 0.1 * (tempTime ^ 2));
  }

}

void moveOffPlatform (int tempTime)
{
  if (TimeRecordL4 == 0)
  {
    dropTime = ((int) (millis()));
    TimeRecordL4 = 1;
  }
  tempTime = ((int) ((millis () - dropTime) / speedJump) % 28);
  if ((11 + tempTime) <= 22)
  {
    OrbitOledMoveTo(5, 11 - squareSize + (tempTime) );
    ycorleft = 11 - squareSize + (tempTime);
    OrbitOledFillRect(5 + squareSize, (11 - squareSize + (tempTime)) + squareSize );
  }
  else
  {
    pressedL4 = 0;
    downL4 = 0;
    currentposL4 = 22;
    TimeRecordL4 = 0;
    ycorleft = 22;
    gettingOffPlatform = false;
    onPlatform = false;
  }
}

static void handlePageGameL4(int shakeTime)
{
  OrbitOledMoveTo(0, 27);
  OrbitOledFillRect(128, 32);

  OrbitOledMoveTo (0, 27);
  OrbitOledLineTo (0, 0);

  OrbitOledMoveTo (128, 27);
  OrbitOledLineTo (128, 0);

  OrbitOledMoveTo (0, 0);
  OrbitOledLineTo (128, 0);

  if (pressedL4 == 0 && downL4 == 0 && enteredlev == false && onPlatform == false && gettingOffPlatform == false)
  {
    OrbitOledMoveTo(5, 22);
    OrbitOledFillRect(5 + squareSize, 27);
  }
  else if (onPlatform)
  {
    OrbitOledMoveTo(5, 11);
    OrbitOledFillRect(5 + squareSize, 16);
  }

  Time = ((int) ((millis() - shakeTime) / speedL4));
  OrbitOledMoveTo(5, 22);

  if (gameInputState.buttons[0].isRising && pressedL4 == 0 && downL4 == 0)
  {
    pressedL4 = 1;
  }

  if (pressedL4 == 1 && downL4 == 0 && onPlatform == false)
  {
    jumpL5(tempTime, 4);
  }


  if (pressedL4 == 0 && downL4 == 1 && onPlatform == false && gettingOffPlatform == false)
  {
    movedown(tempTime);
  }

  if (onPlatform == true)
  {
    moveDownL4(tempTime, 4 , 11);
    nowOnPlatform = true;
  }

  if (gettingOffPlatform == true)
  {
    moveOffPlatform(tempTime);
  }

  moveBackgroundL4(shakeTime, 12);
  OrbitOledUpdate();
  delay(50);
  OrbitOledClear();
}

void moveBackgroundL4(int shakeTime, int obstacles)
{
  Time = ((int) ((millis() - shakeTime) / speedL4));
  i = 0;
  sumOfDistances = 0;

  if (entered == false)
  {
    dist =  (int *) malloc (((obstacles + 1) * sizeof(int)));
    typeOfObstacle =  (int *) malloc (((obstacles + 1) * sizeof(int)));
    xCorRectLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorTriLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorLevLeft = (int *) malloc ((obstacles * sizeof(int)));
    xCorPlatLeft = (int *) malloc ((obstacles * sizeof(int)));
    for (int j = 0; j <  (obstacles + 1); j++)
    {
      dist [j] = 100 + (rand() % 30);
      sum += dist [j];
      typeOfObstacle [j] = rand() % 4;
    }
    typeOfObstacle [(rand() % obstacles)] = 3;
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
            if (penaltyL4 <= 50)
            {
              penaltyL4 += 10;
            }
            entered = false;
            gameUiPage = LostLevel4;
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
        int size = 5;
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + size, currentposlev - tempTimeLev);
        if (currentposlev - tempTimeLev < 20)
        {
          baselev = 27;
        }

        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, currentposlev - tempTimeLev + 3);
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + size, currentposlev - tempTimeLev + 3);

        if (currentposlev - tempTimeLev + 3 < 20)
        {
          baselev = 27;
        }

        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, currentposlev - tempTimeLev + 6);
        OrbitOledLineTo(startPos + (dist[i] + sumOfDistances) - Time + size, currentposlev - tempTimeLev + 6);

        if (currentposlev - tempTimeLev + 6 < 20)
        {
          baselev = 27;
        }

        xCorLevLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorLevLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((ycorleft + squareSize) > 20)
          {
            enteredlev = true;
            pressedL4 = 0;
            downL4 = 0;
            currentposL4 = 22;
            TimeRecordL4 = 0;
          }
        }
        if (enteredlev == true)
        {
          jump(tempTime, -5, speedJump);
        }
        if (ycorleft < 0)
        {
          digitalWrite (LEDR, HIGH);
          delay (2000);
          digitalWrite (LEDR, LOW);
          OrbitOledClearBuffer();
          OrbitOledClear();
          if (penaltyL4 <= 50)
          {
            penaltyL4 += 10;
          }
          pressedL4 = 0;
          downL4 = 0;
          currentposL4 = 22;
          TimeRecordL4 = 0;
          entered = false;
          enteredlev = false;
          ycorleft = 22;
          gameUiPage = LostLevel4;
        }
      }
      else if (typeOfObstacle[i] == 2)
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
            if (penaltyL4 <= 50)
            {
              penaltyL4 += 10;
            }
            entered = false;
            gameUiPage = LostLevel4;
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
            if (penaltyL4 <= 50)
            {
              penaltyL4 += 10;
            }
            entered = false;
            gameUiPage = LostLevel4;
          }
        }
      }
      else if (typeOfObstacle[i] == 3)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 27);
        drawPlatform(platformSize, startPos + (dist[i] + sumOfDistances) - Time, 21);
        xCorPlatLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorPlatLeft [i];

        if ( temp > -35 && temp <= 10)
        {
          if (ycorleft > 11)
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL4 <= 50)
            {
              penaltyL4 += 10;
            }
            entered = false;
            onPlatform = false;
            gettingOffPlatform = true;
            gameUiPage = LostLevel4;
          }
          onPlatform = true;
        }
        if (temp < -35 && temp > -40)
        {
          downL4 = 0;
          gettingOffPlatform = true;
          onPlatform = false;
        }
        else
        {
          minHeight = 27;
        }
      }
      else if (typeOfObstacle[i] == 5)
      {
        drawFinish(startPos + (dist[i] + sumOfDistances) - Time, 27);
        if (startPos + (dist[i] + sumOfDistances) - Time < 0)
        {
         if (scoreRecordL4 == 0)
          {
          score += 50 - penaltyL2;
          scoreRecordL4 = 1;
          }
          readData();
          OrbitOledClear();
          penaltyL4 = 0;
          startL5();
        }
      }
      sumOfDistances += dist[i];
    }
  }
}

void startL5()
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
   OrbitOledUpdate();

  OrbitOledUpdate();
  delay(2000);
  OrbitOledClear();
  gameUiPage = NextL5;
}

void Lvl5CountDown(){
  
  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 5 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('3');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 5 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('2');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  OrbitOledMoveTo(0, 0);
  OrbitOledDrawString("LVL 5 Starts In:");
  OrbitOledMoveTo (64, 13);
  OrbitOledDrawChar('1');
  OrbitOledUpdate();
  delay(1000);
  OrbitOledClear();

  shakeTime = (int) millis();
  entered = false;
  gameUiPage = LevelFive;
}
