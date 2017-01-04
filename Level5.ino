int flyingInitial = 16;
int penaltyL5 = 0;
int speedL5; //= 10;
int enteredBoost = 0;
int boostTime;


void moveBackgroundL5(int shakeTime, int obstacles);
void finishGame();
void jumpFlying (int tempTime, int maxHeight, int jumpSpeed);



static void handlePageGameL5(int shakeTime)
{
  OrbitOledMoveTo(0, 27);
  OrbitOledFillRect(128, 32);

  OrbitOledMoveTo (0, 27);
  OrbitOledLineTo (0, 0);

  OrbitOledMoveTo (128, 27);
  OrbitOledLineTo (128, 0);

  OrbitOledMoveTo (0, 0);
  OrbitOledLineTo (128, 0);

  input = analogRead(Potentiometer);
  temp = ((int)(input / 186));
  if (temp > 0)
  {
    flyingInitial = temp;
  }
  if (input >= 3 && enteredlev == false)
  {
    OrbitOledMoveTo(5, flyingInitial);
    OrbitOledFillRect(5 + squareSize, flyingInitial + squareSize);
  }
  if (gameInputState.buttons[0].isRising)
  {
    if (enteredBoost == 0)
    {
       boostTime = (int) millis();
       enteredBoost = 1;
    }
  }
   if (enteredBoost == 1)
    {
      moveBackgroundL5(boostTime, 10, 15);
    }
    else
   {
      moveBackgroundL5(shakeTime, 10, 15);
    }
   
  
  OrbitOledUpdate();
  delay(50);
  OrbitOledClear();
}

void moveBackgroundL5(int shakeTime, int obstacles, int speedL5)
{
  Time = ((int) ((millis() - shakeTime) / speedL5));
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
      dist [j] = 30 + (rand() % 30);
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
      if (typeOfObstacle[i] == 0 && i % 2 == 0)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, topRect);
        xCorRectLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        temp = xCorRectLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((flyingInitial + squareSize) > topRect)
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
        OrbitOledFillRect(startPos + (dist[i] + sumOfDistances) + 5 - Time, baseRect);
      }
      else if (typeOfObstacle[i] == 0 && i % 2 == 1)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, topRectL3);
        xCorRectLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorRectLeft [i];

        if ( temp >= 0 && temp <= 10)
        {
          if ((flyingInitial) < topRectL3)
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
        OrbitOledFillRect(startPos + (dist[i] + sumOfDistances) + 5 - Time, 0);
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
          if ((flyingInitial + squareSize) > 20)
          {
            enteredlev = true;
          }
        }
        if (enteredlev == true)
        {
          jumpFlying(tempTime, -5, 10);
        }

        if (flyingInitial < 0)
        {
          digitalWrite (LEDR, HIGH);
          delay (2000);
          digitalWrite (LEDR, LOW);
          OrbitOledClearBuffer();
          OrbitOledClear();
          if (penaltyL5 <= 50)
          {
            penaltyL5 += 10;
          }
          gameUiPage = LostLevel5;
        }
      }

      else if (typeOfObstacle[i] == 2 && i % 2 == 0)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 27);
        fillTriangle(triSize, startPos + (dist[i] + sumOfDistances) - Time, 27);
        xCorTriLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorTriLeft [i];

        if ( temp > 2 && temp <= 10)
        {
          if ( (flyingInitial + squareSize) > 27 - (10 - temp) )
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
        else if ( temp >= -10 && temp <= 2)
        {
          if ( (flyingInitial + squareSize) > 19 + (-2 - temp))
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
      }
      else if (typeOfObstacle[i] == 2 && i % 2 == 1)
      {
        OrbitOledMoveTo(startPos + (dist[i] + sumOfDistances) - Time, 5);
        fillInverseTriangle(triSize, startPos + (dist[i] + sumOfDistances) - Time, 0);
        xCorTriLeft[i] = startPos + (dist[i] + sumOfDistances) - Time;

        int temp = xCorTriLeft [i];

        if ( temp > 2 && temp <= 10)
        {
          if ( (flyingInitial) < (16 - temp) )
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
        else if ( temp >= -10 && temp <= 2)
        {
          if ( (flyingInitial) < (12 + temp))
          {
            digitalWrite (LEDR, HIGH);
            delay (2000);
            digitalWrite (LEDR, LOW);
            OrbitOledClearBuffer();
            OrbitOledClear();
            if (penaltyL5 <= 50)
            {
              penaltyL5 += 10;
            }
            entered = false;
            gameUiPage = LostLevel5;
          }
        }
      }
      else if (typeOfObstacle[i] == 5)
      {

        drawFinish(startPos + (dist[i] + sumOfDistances) - Time, 27);
        if (startPos + (dist[i] + sumOfDistances) - Time < 0)
        {
          score += 50 - penaltyL5;
          saveData (score, playerName);
          OrbitOledClear();
          finishGame();
        }
      }
      sumOfDistances += dist[i];
    }
  }
}

void finishGame()
{
  OrbitOledClear();
  OrbitOledMoveTo(10, 2);
  OrbitOledDrawString("                           ");
  OrbitOledMoveTo(10,2);
  OrbitOledDrawString ("Congratz!!!");
  for (int i = 0; i < 3; i++)
  {
    OrbitOledMoveTo ((50 + i) + 5, 2);
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
  penaltyL5 = 0;
  gameUiPage = HighScores;
 
}
void jumpFlying (int tempTime, int maxHeight, int jumpSpeed)
{
  if (Timerecord == 0)
  {
    jumpTime = ((int) (millis()));
    Timerecord = 1;
  }

  tempTime = ((int) ((millis () - jumpTime) / jumpSpeed) % 28);
  if ((currentpos - tempTime) >= maxHeight)
  {
    OrbitOledMoveTo(5, currentpos - tempTime + 0.1 * (tempTime ^ 2));
    flyingInitial = currentpos - tempTime;
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

void lostPageL5()
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
  enteredlev = false;
  shakeTime = (int) millis();
  enteredBoost = 0;
  gameUiPage = LevelFive;
}
