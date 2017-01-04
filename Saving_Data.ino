#include <EEPROM.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

uint32_t ui32EEPROMInit;
uint32_t pui32Data[3];
uint32_t pui32Read[3];
uint32_t scoreData[5];
uint32_t nameData [15];
uint32_t a[1];

int scores [5];
char charNames[15];
int emptyIndex = 0;
int indexi;
int minIndex;

void sort( int a[], int n);
void setupOfEEPROM ();




void sort( int a[], int n)
{
  for (int i = 1; i < n; i++)
  {
    int x = a[i];
    int j = i;
    while (j > 0 && a[j - 1] > x)
    {
      a[j] = a[j - 1];
      --j;
    }
    a[j] = x;
  }
}

void setupOfEEPROM ()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);

  while (!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0))
  {
  }
  ui32EEPROMInit = EEPROMInit();
  if (ui32EEPROMInit != EEPROM_INIT_OK)
  {
    while (1)
    {
    }
  }
}

void saveData(int score, char playerName[])
{
  setupOfEEPROM();

  EEPROMRead(scoreData, 0x400, sizeof(scoreData));
  for (int i = 0; i < 5; i++)
  {
    if ((int)scoreData[i] >= 0 && (int)scoreData [i] <= 250)
    {
      scores[i] =  (int)scoreData[i];
    }
    else
    {
      scores[i] = -1;
      emptyIndex = 1;
      indexi = i;
    }
  }

  EEPROMRead(nameData, 0x100, sizeof(nameData));
  for (int i = 0; i < 15; i++)
  {
    if ((char)nameData[i] >= 65 && (char)nameData[i] <= 90)
    {
      charNames [i] = (char) nameData[i];
    }
    else
    {
      charNames [i] = 64;
    }
  }
  int repeat [5] = {0};
  if (emptyIndex == 1)
  {
    scores[indexi] = score;
    charNames[(indexi) * 3] = playerName [0];
    charNames[((indexi) * 3) + 1] = playerName [1];
    charNames[((indexi) * 3) + 2] = playerName [2];
  }
  else
  {
    minIndex = 0;
    for (int i = 1; i < 5; i++)
    {
      if (scores[minIndex] > scores[i])
      {
        minIndex = i;
      }
    }
    if (scores[minIndex] < score)
    {
    scores[minIndex] = score;
    charNames[(minIndex * 3)] = playerName [0];
    charNames[(minIndex * 3) + 1] = playerName [1];
    charNames[(minIndex * 3) + 2] = playerName [2];
    }
  }
  int temporaryScore [5];
  for (int i = 0; i < 5; i++)
  {
    temporaryScore [i] = scores [i];
  }
  sort (scores, 5);
  char storingNames [15];
  for (int i = 0; i < 15; i++)
  {
    storingNames [i] = 64;
  }
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      if (temporaryScore [i] == scores [j] && repeat[j] == 0)
      {
        storingNames [(j * 3)] = charNames [(i * 3)];
        storingNames [(j * 3) + 1] = charNames [(i * 3) + 1];
        storingNames [(j * 3) + 2] = charNames [(i * 3) + 2];
        repeat[j] = 1;
        break;
      }
    }
  }

  scoreData[0] = *(uint32_t *)&scores[0];
  scoreData[1] = *(uint32_t *)&scores[1];
  scoreData[2] = *(uint32_t *)&scores[2];
  scoreData[3] = *(uint32_t *)&scores[3];
  scoreData[4] = *(uint32_t *)&scores[4];

  for (int i = 0; i < 15; i++)
  {
    nameData [i] = *(uint32_t *)&storingNames[i];
  }

  EEPROMProgram(scoreData, 0x400, sizeof(scoreData));
  EEPROMProgram(nameData, 0x100, sizeof(nameData));
}

void readData()
{
  EEPROMRead(scoreData, 0x400, sizeof(scoreData));
  for (int i = 0; i < 5; i++)
  {
    if ((int)scoreData[i] >= 0 && (int)scoreData [i] <= 250)
    {
      highScores[i] =  (int)scoreData[i];
    }
  }
  EEPROMRead(nameData, 0x100, sizeof(nameData));
  for (int i = 0; i < 15; i++)
  {
    if ((char)nameData[i] >= 65 && (char)nameData[i] <= 90)
    {
      highScoreNames[i] =  (char)nameData[i];
    }
  }
}
