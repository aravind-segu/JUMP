char highScoreNames [15];
char printingNumber;
float input;
int Score;
int selectedScore;

void highScoresPrint()
{
  readData();

  input = analogRead(Potentiometer);
  Score = (int)(input / 1023);
  selectedScore = 4 - Score;

  if (selectedScore < 5 && highScoreNames[(selectedScore * 3)] != 64)
  {
    countGame = 0;
    OrbitOledMoveTo(0, 2);
    OrbitOledDrawString("Highscores");
    OrbitOledMoveTo (10, 16);
    OrbitOledDrawChar((char)(Score + 49));
    OrbitOledMoveTo (18, 16);
    OrbitOledDrawChar('.');

    OrbitOledMoveTo (30, 16);
    OrbitOledDrawChar (highScoreNames[selectedScore * 3]);

    OrbitOledMoveTo (40, 16);
    OrbitOledDrawChar (highScoreNames[(selectedScore * 3) + 1]);

    OrbitOledMoveTo (50, 16);
    OrbitOledDrawChar (highScoreNames[(selectedScore * 3) + 2]);

    tempScore = highScores[selectedScore];
    OrbitOledMoveTo (120, 16);
    OrbitOledDrawChar ((char)32);
    OrbitOledMoveTo(110, 16);
    OrbitOledDrawChar ((char)32);
    OrbitOledMoveTo(100, 16);
    OrbitOledDrawChar ((char)32);
    OrbitOledMoveTo(90, 16);
    OrbitOledDrawChar ((char)32);
    OrbitOledMoveTo(120, 16);

    do {
      printingNumber = chosenNumber (tempScore % 10);
      OrbitOledDrawChar(printingNumber);
      OrbitOledMoveTo((120 - 10 * (countGame + 1)), 16);
      countGame++;
      tempScore = tempScore / 10;
    } while (tempScore > 0);
  }
  OrbitOledUpdate();
  if (ShakeIsShaking()){
    entered = false; 
    handlePageWelcome();
  }
}

char chosenNumber (int number)
{
  for (int i = 0; i < 10; i++)
  {
    if (number == i)
      return i + 48;
  }
}


