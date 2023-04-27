#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include "helper_windows.h"
#define freeSpace printf("\t   ")
#define frameCenter gotoxy(12, 9)

int C; //for print colored c
int score = 0;
int totalSharpNoInOneWord = 0, hardLetter = 0; //for calculate Score and Stop when removing the word containing (#)
int lockedCharacterIndex;
char words[10][30];
int lastLineNumber;
int lastWordIndex;
int flag = 0; //for last wave
float reductionTimeintervalInEachWave;
DWORD intervalBetweenTwoWords; //millisecond

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clearScreen()
{
    for (int satr = 0; satr < 19; satr++)
    {
        gotoxy(1, satr);
        for (int soton = 0; soton < 28; soton++)
        {
            printf(" ");
        }
    }
    gotoxy(1, 0);
}

void get10Words(char words[][30], FILE *filePtr)
{
    for (int wordIndex = 0; wordIndex < 10; wordIndex++)
        fscanf(filePtr, "%s\n", words[wordIndex]);
}

void sharpSleep()
{
    if (totalSharpNoInOneWord > 0)
    {
        Sleep(2 * intervalBetweenTwoWords);
        totalSharpNoInOneWord = 0;
    }
}

int lockedCharacterIndexInPrintRed = 0;
void printLastWord(char *theLastWord)
{
    char *lockedWord;
    lockedWord = theLastWord;
    printf("\033[0;31m");
    for (int j = 0; j < lockedCharacterIndexInPrintRed; j++)
    {
        printf("\033[X%c", *(lockedWord + j));
    }
    printf("\033[0m");
    for (int k = lockedCharacterIndexInPrintRed; k < strlen(lockedWord); k++)
    {
        printf("\033[X%c", *(lockedWord + k));
    }
    printf("\n");
}

void lose(int *waveno, char *level)
{
    clearScreen();
    gotoxy(11, 7);
    printf("GAME OVER");
    gotoxy(12, 8);
    printf("wave(%d)", *waveno + 1);
    gotoxy(11, 9);
    printf("Score: %d", score);
    gotoxy(10, 10);
    printf("level: %s", level);
    Sleep(5000);
    gotoxy(0, 0);
    system("cls");
}

void sleepBefore(){
    if (lastLineNumber != 0)
            Sleep(intervalBetweenTwoWords);
    sharpSleep(); //Stop when removing the word containing (#)
    clearScreen();
}

void timeIntervalDetection(int *waveno){
    for (int i = 0; i < *waveno; i++)
    {
        intervalBetweenTwoWords = intervalBetweenTwoWords - intervalBetweenTwoWords * reductionTimeintervalInEachWave;
        if (intervalBetweenTwoWords < 1000)
            intervalBetweenTwoWords = 1000;
    }
}

int printWords(char words[][30], int *waveno, char *level)
{
    timeIntervalDetection(waveno);
    
    int gameResultAssessor = 1; //1: win
    for (lastLineNumber = 0; lastLineNumber < 20; lastLineNumber++)
    {
        sleepBefore();
        for (int wordIndex = lastLineNumber + lastWordIndex; wordIndex >= lastWordIndex; wordIndex--)
        {
            for (int spaceNo = 0; spaceNo <= (29 - strlen(words[wordIndex])) / 2; spaceNo++)
                printf(" ");
            
            if (wordIndex != lastWordIndex)
            {
                if (lastLineNumber != 19)
                    printf("%s\n", words[wordIndex]);
            }
            else
            {
                if (lastLineNumber != 19)
                    printLastWord(words[lastWordIndex]);
            }
            
            if (lastLineNumber != 19)
                printf("#");
            else
            {
                gotoxy(0, 19);
                for (int i = 0; i < 32; i++)
                    printf("#");
            }

            if (lastWordIndex == 10)
            {
                clearScreen();
                (*waveno) += 1;
                if (intervalBetweenTwoWords <= 1000)
                {
                    flag = 1;
                }
                return 1;
            }
        }
        if (lastLineNumber == 19)
        {
            lose(waveno, level);
            return 0;
        }
        gotoxy(((29 - strlen(words[lastWordIndex])) / 2) + 2 + lockedCharacterIndex, lastLineNumber);
    }
    return gameResultAssessor;
}

void printScore(int score)
{
    gotoxy(0, 20);
    printf("score: %d", score);
}

void identifyWordType_calculateScore(int *score, int *hardLetter, int *totalSharpNoInOneWord, char *lockedWord)
{
    if (*hardLetter > 0)
    {
        (*score) += 3;
        *hardLetter = 0;
    }
    else if (strlen(lockedWord) > 10)
        (*score) += 2;
    else
        (*score)++;
}
int sharpN = 0;
void my_callback_on_key_arrival(char c)
{
    int halateEftekhari = 0;
    char *lockedWord;
    lockedWord = words[lastWordIndex];

    if (c == *(lockedWord + lockedCharacterIndex))
    {
        lockedCharacterIndexInPrintRed++;
        lockedCharacterIndex++;
        printf("\033[0;31m");
        printf("\033[X%c", c);
        printf("\033[0m");
        if (c == '#')
            sharpN++;
        else if (c == '@' || c == '$' || c == '%' || c == '&' || c == '!' || c == '^')
        {
            hardLetter++;
        }
    }
    if (lockedCharacterIndexInPrintRed == strlen(lockedWord))
    {
        lockedCharacterIndexInPrintRed = 0;
        gotoxy(((29 - strlen(words[lastWordIndex])) / 2) + 2, lastLineNumber - 1);
        for (int i = 0; i < strlen(lockedWord); i++)
        {
            printf(" ");
        }
    }
    //raftan be ebtedaye kalame ghabl va mohasebe score
    if (strlen(lockedWord) == lockedCharacterIndex)
    {
        totalSharpNoInOneWord = sharpN;
        sharpN = 0;
        identifyWordType_calculateScore(&score, &hardLetter, &totalSharpNoInOneWord, lockedWord);
        printScore(score);
        lockedCharacterIndex = 0;
        lastLineNumber--;
        lastWordIndex++;
        gotoxy(((29 - strlen(words[lastWordIndex])) / 2) + 2, lastLineNumber - 1);
    }
    //raftan be ebtedaye kalame ghabl va mohasebe score
}

int isLower(char ch)
{
    if (ch <= 'z' && ch >= 'a')
        return 1;
    return 0;
}

void toUpper(char *s)
{
    while (*s)
    {
        if (isLower(*s))
            *s += 'A' - 'a';
        s++;
    }
}
void win()
{
    gotoxy(12, 5);
    printf("YOU WIN");
    gotoxy(7, 7);
    printf("\033[0;33m");
    printf("\"You have reached");
    gotoxy(7, 8);
    printf(" a good degree of");
    gotoxy(8, 9);
    printf("  typing speed\"");
    printf("\033[0m");
    gotoxy(11, 11);
    printf("score: %d", score);
    Sleep(5000);
    system("cls");
}
int main()
{
    //select level
    printf("Please select the level you want:   Easy    Medium    Hard\n");
    char level[7];
    scanf("%s", level);
    toUpper(level);
    int levelno;
    if (strcmp(level, "EASY") == 0)
    {
        levelno = 0;
        intervalBetweenTwoWords = 10000;
        reductionTimeintervalInEachWave = 0.2;
    }
    else if (strcmp(level, "MEDIUM") == 0)
    {
        levelno = 1;
        intervalBetweenTwoWords = 8000;
        reductionTimeintervalInEachWave = 0.3;
    }
    else if (strcmp(level, "HARD") == 0)
    {
        levelno = 2;
        intervalBetweenTwoWords = 5000;
        reductionTimeintervalInEachWave = 0.4;
    }
    //select level
    //Game page design
    system("cls");
    int i = 0;
    for (i = 0; i < 19; i++)
    {
        printf("#                              #\n");
    }
    for (i = 0; i < 32; i++)
        printf("#");
    gotoxy(0, 0);
    //Game page design
    strcpy(words[10], "      ");

    HANDLE thread_id = start_listening(my_callback_on_key_arrival);
    FILE *filePtr;
    int gameResultAssessor = 1;
    int waveno = 0;
    switch (levelno)
    {
    case 0:
        filePtr = fopen("words.txt", "r");
        while (gameResultAssessor == 1)
        {
            lockedCharacterIndex = 0;
            lastLineNumber = 0;
            lastWordIndex = 0;
            if (flag == 1)
            {
                win();
                return 0;
            }
            get10Words(words, filePtr);
            frameCenter;
            printf("WAVE(%d)", waveno + 1);
            Sleep(2500);
            gameResultAssessor = printWords(words, &waveno, level);
            if (gameResultAssessor == 0)
            {
                return 0;
            }
        }
        break;
    case 1:
        filePtr = fopen("words.txt", "r");
        while (gameResultAssessor == 1)
        {
            lockedCharacterIndex = 0;
            lastLineNumber = 0;
            lastWordIndex = 0;
            if (flag == 1)
            {
                win();
                return 0;
            }
            get10Words(words, filePtr);
            frameCenter;
            printf("WAVE(%d)", waveno + 1);
            Sleep(2500);
            gameResultAssessor = printWords(words, &waveno, level);
            if (gameResultAssessor == 0)
            {
                return 0;
            }
        }
        break;
    case 2:
        filePtr = fopen("words.txt", "r");
        while (gameResultAssessor == 1)
        {
            lockedCharacterIndex = 0;
            lastLineNumber = 0;
            lastWordIndex = 0;
            if (flag == 1)
            {
                win();
                return 0;
            }
            get10Words(words, filePtr);
            frameCenter;
            printf("WAVE(%d)", waveno + 1);
            Sleep(2500);
            gameResultAssessor = printWords(words, &waveno, level);
            if (gameResultAssessor == 0)
            {
                return 0;
            }
        }

        break;
    default:
        system("cls");
        printf("invalid answer");
        return 0;
        break;
    }
    WaitForSingleObject(thread_id, INFINITE);
    return 0;
}