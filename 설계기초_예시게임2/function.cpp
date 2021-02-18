#include<iostream>
#include<Windows.h>
#include<conio.h>
#include"Text.h"
#include"Maze.h"
#include<string>
#include<fstream>
#include<algorithm>
#include<time.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

#define LEFTSPACE 3     // �̷� ���� �ʱⰪ
#define LIGHTSPACE 3    // �̷� ������ �ʱⰪ
#define UPSPACE 2       // �̷� ���� �ʱⰪ
#define DOWNSPACE 5     // �̷� �Ʒ��� �ʱⰪ
#define MAP 30          // PlayerInform �ʱⰪ

#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define SPACEBAR 32

#define GITEM_OVER_X 30
#define GITEM_OVER_Y 20

#define _X 2
#define _Y 0

#define GITEM_ORIGIN_X 55
#define GITEM_ORIGIN_Y 2
#define GITEM_WIDTH 22
#define GITEM_HEIGHT 17

/* �̷�, �������丮, ���ӿ���, ����Ŭ���� �迭*/
char Hospital[26][31];
char GAME_OVER[20][100];
char Maze[30][31];

int arrowSpeedSet = 2; // ��ǻ�͸��� �ӵ��� ���̰� ���� ��찡 �ִ�. �� ��쿡 ������ �ؼ��ϱ� ���� arrowSpeed�� �����Ѵ�. 2 -> �⺻ 1������ 3 ������ 

/*Ÿ�̸� ���� ������*/
int TimeLimit;
clock_t start;
clock_t t;
clock_t finish;

/*���� ���� ���� ������*/
int locate; // select���� ���.
int quest; // tutorial ����
int death; // death count
int num;
int stage;
int number; // Ƚ��. Ʃ�丮�󿡼� ���.
int storycnt = 0;

/*ȭ�� ���� ������*/
long arrowSpeed; // arrow speed�� stage�� �����ϵ��� �Ѵ�. ����� sleep�� �μ��� ������ ũ�Ⱑ ũ�� ������
POINT arrowPos[15];     // �̷ο��� ��ȭ�ϴ� ȭ���� ��ǥ ����
int arrowCount = 0;         // �̷ο��� ȭ���� ���� ����
char arrowVector[15];      // ȭ���� ���� ���� '>' '<' '+' '-'
POINT arrowStart[15];   // ȭ��ǥ ���� ��ġ ����

POINT npcItemPos[5];
int npcPause = 0;
int npcPauseLimit;
int inpcCount;
int npcItem;


/*PC���� ������*/
POINT tStartPos;
POINT tPlayerPosRED;
POINT tPlayerPosBLUE;

/*��ź ���� ������*/
POINT tBombPos[5];
int Bombflag[5];
POINT BombFirePos[4][5];
POINT BombPoint;
POINT BombItemPos[5];
int BombLimit;
int BombItem;
int BombPause = 0;
int iBombCount;

/*ȸ�� ���� ������*/
int tmp = 0;
int idx = 0;
int RotateCount;


int GameClearScore[3][5] = { 0 };   //���庰 �ɸ��ð�, ���� Ƚ��, ȸ��Ƚ��
char Grade[12][13];

/*����� NPC ���� ������*/
long SpecialarrowSpeed; // arrow speed�� stage�� �����ϵ��� �Ѵ�. ����� sleep�� �μ��� ������ ũ�Ⱑ ũ�� ������
POINT SpecialarrowPos[15];
int SpecialarrowCount = 0;
char SpecialarrowVector[15];
POINT SpecialarrowStart[15];

enum COLOR {
    BLACK, BLUE, GREEN, OC, RED, JAJU, YELLOW, WHITE, GRAY
    , DEEP_BLUE, DEEP_GREEN, DEEP_OC, DEEP_RED, DEEP_JAJU, DEEP_YELLOW,
    DEEP_WHITE, SKY
};

void RemoveCursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void SetCurrentCursorPos(int x, int y) {
    COORD position = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
void SetMaze()
{
    num = 0;
    // death�� �ǵ帮�� �ʴ´�. (1~6 stage���� ���� ��ŷ�� ��� ��) death 0�� Ʃ�丮�� or title���� �� ó����.

    if (locate == 0) // ���� ���� �ƴ� ��
        system("cls");

    while (_kbhit()) _getch();

    switch (stage)
    {
    case -1:
        title(); // Ÿ��Ʋ ������ Ÿ��Ʋ���� ó���ϰ� �ִ�. title() SetMaze() �� stage = -1; SetMaze() SetMaze()
        system("cls");
        printHospital();
        break;

    case 0:
        /* NPC �ӵ� ����*/
        arrowSpeed = 15;
        SpecialarrowSpeed = 15;
        if (RotateCount == 0)
        {
            PlaySound(TEXT("�������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

            /*������ �ʱ���ġ ����*/
            tPlayerPosRED.x = 20;
            tPlayerPosRED.y = 14;
            /*�Ķ��� �ʱ���ġ ����*/
            tPlayerPosBLUE.x = 10;
            tPlayerPosBLUE.y = 1;

            ResetBomb();        // ��ź���� �ʱ�ȭ
            ResetTime();        // Ÿ�̸Ӱ��� �ʱ�ȭ
            ResetNPC();         // NPC �ʱ�ȭ
        }
        if (tmp == 0) {

            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "0GGGGGGGG011110011110011111110");
            strcpy_s(Maze[2], 31, "0GGGGssGG000010010010011ggg110");
            strcpy_s(Maze[3], 31, "0GsGsssGG000010010010011gggbb0");
            strcpy_s(Maze[4], 31, "0GGGssGGG000011110010011ggg110");
            strcpy_s(Maze[5], 31, "0GsGGGGGG0000000000100111111g0");
            strcpy_s(Maze[6], 31, "0GGGG0GGG0111111111110111111g0");
            strcpy_s(Maze[7], 31, "0GGG00GGG011111111111e11111110");
            strcpy_s(Maze[8], 31, "0GG0000GG011111111111e11111110");
            strcpy_s(Maze[9], 31, "0GG1100GG011111111111e11000000");
            strcpy_s(Maze[10], 31, "0GG0000GG011111111111011001100");
            strcpy_s(Maze[11], 31, "0GG1100GG01100000000001ss01100");
            strcpy_s(Maze[12], 31, "0GG0000GG0110000000000ssss0000");
            strcpy_s(Maze[13], 31, "0GG1100GG0111111111110ssss0000");
            strcpy_s(Maze[14], 31, "0GGssssGG00000000010001ss00000");
            strcpy_s(Maze[15], 31, "0GG11ssGG000111111100011001100");
            strcpy_s(Maze[16], 31, "0GGssssGG000100000000011001100");
            strcpy_s(Maze[17], 31, "0GG11ssGG000100000000011000000");
            strcpy_s(Maze[18], 31, "0GGssssGG000111111111011111110");
            strcpy_s(Maze[19], 31, "0GG11ssGG000000000001011111110");
            strcpy_s(Maze[20], 31, "0GGssssGG000000111111011111110");
            strcpy_s(Maze[21], 31, "0GGG00GGG00000010000001111T110");
            strcpy_s(Maze[22], 31, "0GGG00GGG011111111111011TTT110");
            strcpy_s(Maze[23], 31, "0GGG00GGG011555555511011TTTbb0");
            strcpy_s(Maze[24], 31, "0GG0000GG0115555555110111gT110");
            strcpy_s(Maze[25], 31, "0GGGGGGGG01155555551101gggg110");
            strcpy_s(Maze[26], 31, "0GGGGGGGG01155535551101gggbbb0");
            strcpy_s(Maze[27], 31, "0GGGGGGGG0115555555110111g1110");
            strcpy_s(Maze[28], 31, "0GGGGGGGG011555555511011111110");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");

        }
        else if (tmp == 1)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "011b1gg1100000000011111b11b110");
            strcpy_s(Maze[2], 31, "011b1111101100011011111b11b110");
            strcpy_s(Maze[3], 31, "01ggg1111011000110111TTTTgb110");
            strcpy_s(Maze[4], 31, "01ggg1111000ss00001111TTgggg10");
            strcpy_s(Maze[5], 31, "01ggg111100ssss0001111TT1gg110");
            strcpy_s(Maze[6], 31, "01111111111ssss1111111111gg110");
            strcpy_s(Maze[7], 31, "011111111111ss1111111111111110");
            strcpy_s(Maze[8], 31, "0000000eee00000000000000000000");
            strcpy_s(Maze[9], 31, "000000111110010000111011111110");
            strcpy_s(Maze[10], 31, "011111111110010000101011111110");
            strcpy_s(Maze[11], 31, "010000111110011100101015555550");
            strcpy_s(Maze[12], 31, "010000111110010100101015555550");
            strcpy_s(Maze[13], 31, "011110111110010100101015555550");
            strcpy_s(Maze[14], 31, "000010111110010100101115553550");
            strcpy_s(Maze[15], 31, "000010111110010100100015555550");
            strcpy_s(Maze[16], 31, "011110111110010100100015555550");
            strcpy_s(Maze[17], 31, "010000111110010111100015555550");
            strcpy_s(Maze[18], 31, "010000111111110000000011111110");
            strcpy_s(Maze[19], 31, "010000111111110000000011111110");
            strcpy_s(Maze[20], 31, "000000000000000000000000000000");
            strcpy_s(Maze[21], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[22], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[23], 31, "0GssGGGG000000sssssssGGG0GGGG0");
            strcpy_s(Maze[24], 31, "0GsssG00000000sssssss0000GGGG0");
            strcpy_s(Maze[25], 31, "0GGssGG0010101s1s1s1s0000GGGG0");
            strcpy_s(Maze[26], 31, "0GGGGGGG010101s1s1s1sGGG0GGGG0");
            strcpy_s(Maze[27], 31, "0GGsGsGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[28], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 2)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "011111110115555555110GGGGGGGG0");
            strcpy_s(Maze[2], 31, "0111g1110115555555110GGGGGGGG0");
            strcpy_s(Maze[3], 31, "0bbbggg10115553555110GGGGGGGG0");
            strcpy_s(Maze[4], 31, "011gggg10115555555110GGGGGGGG0");
            strcpy_s(Maze[5], 31, "011Tg1110115555555110GG0000GG0");
            strcpy_s(Maze[6], 31, "0bbTTT110115555555110GGG00GGG0");
            strcpy_s(Maze[7], 31, "011TTT110111111111110GGG00GGG0");
            strcpy_s(Maze[8], 31, "011T11110000001000000GGG00GGG0");
            strcpy_s(Maze[9], 31, "011111110111111000000GGssssGG0");
            strcpy_s(Maze[10], 31, "011111110100000000000GGss11GG0");
            strcpy_s(Maze[11], 31, "011111110111111111000GGssssGG0");
            strcpy_s(Maze[12], 31, "000000110000000001000GGss11GG0");
            strcpy_s(Maze[13], 31, "001100110000000001000GGssssGG0");
            strcpy_s(Maze[14], 31, "001100110001111111000GGss11GG0");
            strcpy_s(Maze[15], 31, "00000ss10001000000000GGssssGG0");
            strcpy_s(Maze[16], 31, "0000ssss0111111111110GG0011GG0");
            strcpy_s(Maze[17], 31, "0000ssss0000000000110GG0000GG0");
            strcpy_s(Maze[18], 31, "00110ss10000000000110GG0011GG0");
            strcpy_s(Maze[19], 31, "001100110111111111110GG0000GG0");
            strcpy_s(Maze[20], 31, "00000011e111111111110GG0011GG0");
            strcpy_s(Maze[21], 31, "01111111e111111111110GG0000GG0");
            strcpy_s(Maze[22], 31, "01111111e111111111110GGG00GGG0");
            strcpy_s(Maze[23], 31, "0g1111110111111111110GGG0GGGG0");
            strcpy_s(Maze[24], 31, "0g1111110010000000000GGGGGGsG0");
            strcpy_s(Maze[25], 31, "011ggg110010011110000GGGssGGG0");
            strcpy_s(Maze[26], 31, "0bbggg110010010010000GGsssGsG0");
            strcpy_s(Maze[27], 31, "011ggg110010010010000GGssGGGG0");
            strcpy_s(Maze[28], 31, "011111110011110011110GGGGGGGG0");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 3)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[2], 31, "0GGGGGGGGGGGGGGGGGGGGGGGsGsGG0");
            strcpy_s(Maze[3], 31, "0GGGG0GGGs1s1s1s101010GGGGGGG0");
            strcpy_s(Maze[4], 31, "0GGGG0000s1s1s1s1010100GGssGG0");
            strcpy_s(Maze[5], 31, "0GGGG0000sssssss00000000GsssG0");
            strcpy_s(Maze[6], 31, "0GGGG0GGGsssssss000000GGGGssG0");
            strcpy_s(Maze[7], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[8], 31, "0GGGGGGGGGGGGGGGGGGGGGGGGGGGG0");
            strcpy_s(Maze[9], 31, "000000000000000000000000000000");
            strcpy_s(Maze[10], 31, "011111110000000011111111000010");
            strcpy_s(Maze[11], 31, "011111110000000011111111000010");
            strcpy_s(Maze[12], 31, "055555510001111010011111000010");
            strcpy_s(Maze[13], 31, "055555510001001010011111011110");
            strcpy_s(Maze[14], 31, "055555510001001010011111010000");
            strcpy_s(Maze[15], 31, "055355511101001010011111010000");
            strcpy_s(Maze[16], 31, "055555510101001010011111011110");
            strcpy_s(Maze[17], 31, "055555510101001010011111000010");
            strcpy_s(Maze[18], 31, "055555510101001110011111000010");
            strcpy_s(Maze[19], 31, "011111110101000010011111111110");
            strcpy_s(Maze[20], 31, "011111110111000010011111000000");
            strcpy_s(Maze[21], 31, "00000000000000000000eee0000000");
            strcpy_s(Maze[22], 31, "0111111111111111ss111111111110");
            strcpy_s(Maze[23], 31, "011gg1111111111ssss11111111110");
            strcpy_s(Maze[24], 31, "011gg1TT1111000ssss001111ggg10");
            strcpy_s(Maze[25], 31, "01ggggTT11110000ss0001111ggg10");
            strcpy_s(Maze[26], 31, "011bgTTTT1110110001101111ggg10");
            strcpy_s(Maze[27], 31, "011b11b1111101100011011111b110");
            strcpy_s(Maze[28], 31, "011b11b1111100000000011gg1b110");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        break;
    case 1:
        system("cls");
        /* NPC �ӵ� ����*/
        arrowSpeed = 15;
        if (RotateCount == 0)
        {
            idx = 0;
            system("cls");
            /*������ �ʱ���ġ ����*/
            tPlayerPosRED.x = 1;
            tPlayerPosRED.y = 1;
            /* �Ķ��� �ʱ���ġ ����*/
            tPlayerPosBLUE.x = 28;
            tPlayerPosBLUE.y = 1;
            /*NPC �ʱ���ġ ����*/
            arrowCount = 4;     // 1���忡�� ȭ��ǥ�� ������ 2��
            /*ù��° ȭ��ǥ ��ġ�� ����*/
            arrowStart[0].x = 4;
            arrowStart[0].y = 16;
            arrowPos[0] = arrowStart[0];
            arrowVector[0] = '>';

            /*�ι�° ȭ��ǥ ��ġ�� ����*/
            arrowStart[1].x = 4;
            arrowStart[1].y = 17;
            arrowPos[1] = arrowStart[1];
            arrowVector[1] = '>';

            /*����° ȭ��ǥ ��ġ�� ����*/
            arrowStart[2].x = 15;
            arrowStart[2].y = 19;
            arrowPos[2] = arrowStart[2];
            arrowVector[2] = '<';
            /*�׹�° ȭ��ǥ ��ġ�� ����*/
            arrowStart[3].x = 15;
            arrowStart[3].y = 20;
            arrowPos[3] = arrowStart[3];
            arrowVector[3] = '<';

            /*���庰 ���۽��丮 ���*/
            if (death == 3)
                startStory();
            PlaySound(TEXT("�������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

            ResetBomb();    // ��ź ���� �ʱ�ȭ
            ResetTime();    // Ÿ�̸� ���� �ʱ�ȭ
            ResetNPC();     // NPC �ʱ�ȭ
        }
        if (tmp == 0) {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "010111111110111111111111151110");
            strcpy_s(Maze[2], 31, "010100001010001000100010155550");
            strcpy_s(Maze[3], 31, "010101111011111110111110101110");
            strcpy_s(Maze[4], 31, "010101111011010000100010100010");
            strcpy_s(Maze[5], 31, "010100011111010111111010111110");
            strcpy_s(Maze[6], 31, "011111010000010111101010001000");
            strcpy_s(Maze[7], 31, "000001011111111100101011111010");
            strcpy_s(Maze[8], 31, "011111010000000110101000101010");
            strcpy_s(Maze[9], 31, "010000010111110110111111100010");
            strcpy_s(Maze[10], 31, "011111111101010110100001111110");
            strcpy_s(Maze[11], 31, "000011000101011110111101010000");
            strcpy_s(Maze[12], 31, "011111011101011100001101010110");
            strcpy_s(Maze[13], 31, "000011010001000111101001010110");
            strcpy_s(Maze[14], 31, "000011011111111111101111111110");
            strcpy_s(Maze[15], 31, "000011000000000000000000000000");
            strcpy_s(Maze[16], 31, "0000111111111111e0000000000000");
            strcpy_s(Maze[17], 31, "0000111111111111e0000000000000");
            strcpy_s(Maze[18], 31, "000011111111111100000000000000");
            strcpy_s(Maze[19], 31, "000e11111111111100000000000000");
            strcpy_s(Maze[20], 31, "000e11111111111100000000000000");
            strcpy_s(Maze[21], 31, "000011111111111111111111111110");
            strcpy_s(Maze[22], 31, "000000000000000000000000000110");
            strcpy_s(Maze[23], 31, "011111111111111111111111110110");
            strcpy_s(Maze[24], 31, "010000000000000000000000010110");
            strcpy_s(Maze[25], 31, "010000000000000000015100010110");
            strcpy_s(Maze[26], 31, "010311111115111000010111110110");
            strcpy_s(Maze[27], 31, "010000000000001000010000000110");
            strcpy_s(Maze[28], 31, "011111111111111000011111111110");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 1)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "015111011110111000000111111110");
            strcpy_s(Maze[2], 31, "015101000010111000000111111110");
            strcpy_s(Maze[3], 31, "015101111010001000000100000010");
            strcpy_s(Maze[4], 31, "055001010011111000000101111010");
            strcpy_s(Maze[5], 31, "011111011110001000000101001010");
            strcpy_s(Maze[6], 31, "010000010111111000000101001010");
            strcpy_s(Maze[7], 31, "011111110100001000000101001010");
            strcpy_s(Maze[8], 31, "010100000101101000000101011010");
            strcpy_s(Maze[9], 31, "010101111101111000000101050010");
            strcpy_s(Maze[10], 31, "010101000101000000000101011110");
            strcpy_s(Maze[11], 31, "011111111111011000000101000000");
            strcpy_s(Maze[12], 31, "010001100000011000000101000000");
            strcpy_s(Maze[13], 31, "0101011011110110ee000101000000");
            strcpy_s(Maze[14], 31, "010101111111111011111101000000");
            strcpy_s(Maze[15], 31, "011100010001101011111101001110");
            strcpy_s(Maze[16], 31, "010111110111101011111101001010");
            strcpy_s(Maze[17], 31, "010100010100001011111101001010");
            strcpy_s(Maze[18], 31, "000111010111111011111101005010");
            strcpy_s(Maze[19], 31, "011111010100001011111101001010");
            strcpy_s(Maze[20], 31, "010001010111101011111101001010");
            strcpy_s(Maze[21], 31, "011111010010101011111101001010");
            strcpy_s(Maze[22], 31, "010111111110111011111101001010");
            strcpy_s(Maze[23], 31, "010110000010000011111101001010");
            strcpy_s(Maze[24], 31, "010110111011111111111101001010");
            strcpy_s(Maze[25], 31, "010000101011111111111101001010");
            strcpy_s(Maze[26], 31, "0111111010101000000ee001003010");
            strcpy_s(Maze[27], 31, "000000101010100000000001000010");
            strcpy_s(Maze[28], 31, "011111101110100000000001111110");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 2)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "011111111110000111111111111110");
            strcpy_s(Maze[2], 31, "011000000010000100000000000010");
            strcpy_s(Maze[3], 31, "011011111010000111511111113010");
            strcpy_s(Maze[4], 31, "011010001510000000000000000010");
            strcpy_s(Maze[5], 31, "011010000000000000000000000010");
            strcpy_s(Maze[6], 31, "011011111111111111111111111110");
            strcpy_s(Maze[7], 31, "011000000000000000000000000000");
            strcpy_s(Maze[8], 31, "011111111111111111111111110000");
            strcpy_s(Maze[9], 31, "00000000000000111111111111e000");
            strcpy_s(Maze[10], 31, "00000000000000111111111111e000");
            strcpy_s(Maze[11], 31, "000000000000001111111111110000");
            strcpy_s(Maze[12], 31, "0000000000000e1111111111110000");
            strcpy_s(Maze[13], 31, "0000000000000e1111111111110000");
            strcpy_s(Maze[14], 31, "000000000000000000000000110000");
            strcpy_s(Maze[15], 31, "011111111101111111111110110000");
            strcpy_s(Maze[16], 31, "011010100101111000100010110000");
            strcpy_s(Maze[17], 31, "011010101100001110101110111110");
            strcpy_s(Maze[18], 31, "000010101111011110101000110000");
            strcpy_s(Maze[19], 31, "011111100001011010101111111110");
            strcpy_s(Maze[20], 31, "010001111111011011111010000010");
            strcpy_s(Maze[21], 31, "010101000101011000000010111110");
            strcpy_s(Maze[22], 31, "010111110101001111111110100000");
            strcpy_s(Maze[23], 31, "000100010101111010000010111110");
            strcpy_s(Maze[24], 31, "011111010111111010111110001010");
            strcpy_s(Maze[25], 31, "010001010001000010110111101010");
            strcpy_s(Maze[26], 31, "011101011111011111110111101010");
            strcpy_s(Maze[27], 31, "055551010001000100010100001010");
            strcpy_s(Maze[28], 31, "011151111111111111011111111010");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 3)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "011111100000000001011101111110");
            strcpy_s(Maze[2], 31, "010000100000000001010101000000");
            strcpy_s(Maze[3], 31, "010300100ee0000001010101111110");
            strcpy_s(Maze[4], 31, "010100101111111111110101000010");
            strcpy_s(Maze[5], 31, "010100101111111111110111011010");
            strcpy_s(Maze[6], 31, "010100101111110000010000011010");
            strcpy_s(Maze[7], 31, "010100101111110111011111111010");
            strcpy_s(Maze[8], 31, "010100101111110101010010111110");
            strcpy_s(Maze[9], 31, "010100101111110101111010100010");
            strcpy_s(Maze[10], 31, "010100101111110100001010111110");
            strcpy_s(Maze[11], 31, "010500101111110111111010111000");
            strcpy_s(Maze[12], 31, "010100101111110100001010001010");
            strcpy_s(Maze[13], 31, "010100101111110101111011111010");
            strcpy_s(Maze[14], 31, "011100101111110101100010001110");
            strcpy_s(Maze[15], 31, "000000101111110111111111101010");
            strcpy_s(Maze[16], 31, "000000101000ee0110111101101010");
            strcpy_s(Maze[17], 31, "000000101000000110000001100010");
            strcpy_s(Maze[18], 31, "000000101000000110111111111110");
            strcpy_s(Maze[19], 31, "011110101000000000101000101010");
            strcpy_s(Maze[20], 31, "010050101000000111101111101010");
            strcpy_s(Maze[21], 31, "010110101000000101101000001010");
            strcpy_s(Maze[22], 31, "010100101000000100001011111110");
            strcpy_s(Maze[23], 31, "010100101000000111111010000010");
            strcpy_s(Maze[24], 31, "010100101000000100011110111110");
            strcpy_s(Maze[25], 31, "010111101000000111110010100550");
            strcpy_s(Maze[26], 31, "010000001000000100010111101510");
            strcpy_s(Maze[27], 31, "011111111000000111010000101510");
            strcpy_s(Maze[28], 31, "011111111000000111011110111510");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        break;
    case 2:
        /* NPC �ӵ� ����*/
        arrowSpeed = 2;
        SpecialarrowSpeed = 2;
        if (RotateCount == 0)
        {
            idx = 0;
            /*������ �ʱ���ġ ����*/
            tPlayerPosRED.x = 12;
            tPlayerPosRED.y = 1;
            /* �Ķ��� �ʱ���ġ ����*/
            tPlayerPosBLUE.x = 28;
            tPlayerPosBLUE.y = 1;

            /*���庰 ���۽��丮 ���*/
            if (death == 3)
                startStory();
            PlaySound(TEXT("�������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

            ResetBomb();        // ��ź ���� �ʱ�ȭ
            ResetTime();        // Ÿ�̸� ���� �ʱ�ȭ
            ResetNPC();         // NPC �ʱ�ȭ
        }
        if (tmp == 0)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "011111111111101110111111111110");
            strcpy_s(Maze[2], 31, "010000000000001110100000000000");
            strcpy_s(Maze[3], 31, "010111111111111111111111111e10");
            strcpy_s(Maze[4], 31, "010177777777777777777777771010");
            strcpy_s(Maze[5], 31, "010170000011100000111000001010");
            strcpy_s(Maze[6], 31, "010170GGG01110GGG01110GGG01010");
            strcpy_s(Maze[7], 31, "010170000011100000111000001010");
            strcpy_s(Maze[8], 31, "010170000011100000111000001010");
            strcpy_s(Maze[9], 31, "01017GGGGG111GGGGG111GGGGG1010");
            strcpy_s(Maze[10], 31, "010177777711177777111777771010");
            strcpy_s(Maze[11], 31, "01e177777777777777777777771010");
            strcpy_s(Maze[12], 31, "010111111111111111111111111010");
            strcpy_s(Maze[13], 31, "010000000000100000100000000010");
            strcpy_s(Maze[14], 31, "011111111111111111111111111110");
            strcpy_s(Maze[15], 31, "01000000000e00000e000000000000");
            strcpy_s(Maze[16], 31, "010111101111111511111111111100");
            strcpy_s(Maze[17], 31, "010111101111111511111111111100");
            strcpy_s(Maze[18], 31, "010100101111111511110100000000");
            strcpy_s(Maze[19], 31, "010100101111111511110111111100");
            strcpy_s(Maze[20], 31, "010100101111111511110000000100");
            strcpy_s(Maze[21], 31, "010100101111111511110111111100");
            strcpy_s(Maze[22], 31, "010100101111111511110100000000");
            strcpy_s(Maze[23], 31, "010100101111111511110100000000");
            strcpy_s(Maze[24], 31, "011100111111111511110111111100");
            strcpy_s(Maze[25], 31, "0111000000000e00000e0000000100");
            strcpy_s(Maze[26], 31, "011101111111111111111111111100");
            strcpy_s(Maze[27], 31, "055500001111111111e00000000000");
            strcpy_s(Maze[28], 31, "000000005555511155511111111300");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 1)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "010111111111111000000000000000");
            strcpy_s(Maze[2], 31, "010e00000000001011011100111030");
            strcpy_s(Maze[3], 31, "010111111111101011010100101010");
            strcpy_s(Maze[4], 31, "010170000G77101011010100101010");
            strcpy_s(Maze[5], 31, "010170G00G77101011010100101010");
            strcpy_s(Maze[6], 31, "010170G00G77101011010100101010");
            strcpy_s(Maze[7], 31, "010170G00G77101011010100101010");
            strcpy_s(Maze[8], 31, "010170000G77101011110111101010");
            strcpy_s(Maze[9], 31, "010171111117101011000000001010");
            strcpy_s(Maze[10], 31, "0101711111171010111111111e1010");
            strcpy_s(Maze[11], 31, "011171111117111011111111101e50");
            strcpy_s(Maze[12], 31, "000170000G77101e11111111101150");
            strcpy_s(Maze[13], 31, "011170G00G77101011111111101150");
            strcpy_s(Maze[14], 31, "011170G00G77101055555555501110");
            strcpy_s(Maze[15], 31, "011170G00G77101011111111101110");
            strcpy_s(Maze[16], 31, "000170000G771010111111111e1110");
            strcpy_s(Maze[17], 31, "010171111117111011111111101150");
            strcpy_s(Maze[18], 31, "010171111117101e11111111101150");
            strcpy_s(Maze[19], 31, "010171111117101011111111101150");
            strcpy_s(Maze[20], 31, "010170000G77101011111111101150");
            strcpy_s(Maze[21], 31, "010170G00G77101011111111101150");
            strcpy_s(Maze[22], 31, "010170G00G77101000000000101100");
            strcpy_s(Maze[23], 31, "010170G00G77101011111111101000");
            strcpy_s(Maze[24], 31, "010170000G77101011000000001000");
            strcpy_s(Maze[25], 31, "010177777777101011000000000000");
            strcpy_s(Maze[26], 31, "010111111111101011111111111500");
            strcpy_s(Maze[27], 31, "01000000000e001000000000111500");
            strcpy_s(Maze[28], 31, "011111111111111111111111111500");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 2)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "003111111115551115555500000000");
            strcpy_s(Maze[2], 31, "00000000000e111111111100005550");
            strcpy_s(Maze[3], 31, "001111111111111111111111101110");
            strcpy_s(Maze[4], 31, "0010000000e00000e0000000001110");
            strcpy_s(Maze[5], 31, "001111111011115111111111001110");
            strcpy_s(Maze[6], 31, "000000001011115111111101001010");
            strcpy_s(Maze[7], 31, "000000001011115111111101001010");
            strcpy_s(Maze[8], 31, "001111111011115111111101001010");
            strcpy_s(Maze[9], 31, "001000000011115111111101001010");
            strcpy_s(Maze[10], 31, "001111111011115111111101001010");
            strcpy_s(Maze[11], 31, "000000001011115111111101001010");
            strcpy_s(Maze[12], 31, "001111111111115111111101111010");
            strcpy_s(Maze[13], 31, "001111111111115111111101111010");
            strcpy_s(Maze[14], 31, "000000000000e00000e00000000010");
            strcpy_s(Maze[15], 31, "011111111111111111111111111110");
            strcpy_s(Maze[16], 31, "010000000001000001000000000010");
            strcpy_s(Maze[17], 31, "010111111111111111111111111010");
            strcpy_s(Maze[18], 31, "010177777777777777777777771e10");
            strcpy_s(Maze[19], 31, "010177777111777771117777771010");
            strcpy_s(Maze[20], 31, "0101GGGGG111GGGGG111GGGGG71010");
            strcpy_s(Maze[21], 31, "010100000111000001110000071010");
            strcpy_s(Maze[22], 31, "010100000111000001110000071010");
            strcpy_s(Maze[23], 31, "01010GGG01110GGG01110GGG071010");
            strcpy_s(Maze[24], 31, "010100000111000001110000071010");
            strcpy_s(Maze[25], 31, "010177777777777777777777771010");
            strcpy_s(Maze[26], 31, "01e111111111111111111111111010");
            strcpy_s(Maze[27], 31, "000000000001011100000000000010");
            strcpy_s(Maze[28], 31, "011111111111011101111111111110");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 3)
        {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "005111111111111111111111111110");
            strcpy_s(Maze[2], 31, "005111000000000100e00000000010");
            strcpy_s(Maze[3], 31, "005111111111110101111111111010");
            strcpy_s(Maze[4], 31, "000000000000110101777777771010");
            strcpy_s(Maze[5], 31, "00010000000011010177G000071010");
            strcpy_s(Maze[6], 31, "00010111111111010177G00G071010");
            strcpy_s(Maze[7], 31, "00010100000000010177G00G071010");
            strcpy_s(Maze[8], 31, "05110111111111010177G00G071010");
            strcpy_s(Maze[9], 31, "05110111111111010177G000071010");
            strcpy_s(Maze[10], 31, "051101111111110101711111171010");
            strcpy_s(Maze[11], 31, "05110111111111e101711111171010");
            strcpy_s(Maze[12], 31, "051101111111110111711111171010");
            strcpy_s(Maze[13], 31, "0111e111111111010177G000071000");
            strcpy_s(Maze[14], 31, "01110111111111010177G00G071110");
            strcpy_s(Maze[15], 31, "01110555555555010177G00G071110");
            strcpy_s(Maze[16], 31, "05110111111111010177G00G071110");
            strcpy_s(Maze[17], 31, "05110111111111e10177G000071000");
            strcpy_s(Maze[18], 31, "05e101111111110111711111171110");
            strcpy_s(Maze[19], 31, "0101e1111111110101711111171010");
            strcpy_s(Maze[20], 31, "010100000000110101711111171010");
            strcpy_s(Maze[21], 31, "01010111101111010177G000071010");
            strcpy_s(Maze[22], 31, "01010100101011010177G00G071010");
            strcpy_s(Maze[23], 31, "01010100101011010177G00G071010");
            strcpy_s(Maze[24], 31, "01010100101011010177G00G071010");
            strcpy_s(Maze[25], 31, "01010100101011010177G000071010");
            strcpy_s(Maze[26], 31, "010101001010110101111111111010");
            strcpy_s(Maze[27], 31, "03011100111011010000000000e010");
            strcpy_s(Maze[28], 31, "000000000000000111111111111010");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");

        }
        break;
    case 3:
        system("cls");
        if (RotateCount == 0)
        {
            idx = 0;
            /*������ �ʱ���ġ ����*/
            tPlayerPosRED.x = 1;
            tPlayerPosRED.y = 13;
            /* �Ķ��� �ʱ���ġ ����*/
            tPlayerPosBLUE.x = 1;
            tPlayerPosBLUE.y = 28;

            /*���庰 ���۽��丮 ���*/
            if (death == 3)
                startStory();
            PlaySound(TEXT("�������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

            ResetBomb();        // ��ź ���� �ʱ�ȭ
            ResetTime();        // Ÿ�̸� ���� �ʱ�ȭ
            ResetNPC();         // NPC �ʱ�ȭ
        }
        if (tmp == 0) {

            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "0oo000000001111511111111111130");
            strcpy_s(Maze[2], 31, "0oo00GGBB00111101111111e000000");
            strcpy_s(Maze[3], 31, "0GG00GGBB001111011111110000000");
            strcpy_s(Maze[4], 31, "0GG000000000005000000110111110");
            strcpy_s(Maze[5], 31, "000000000001111011111110100010");
            strcpy_s(Maze[6], 31, "000000gg0005000011111110101010");
            strcpy_s(Maze[7], 31, "000000gg0001111e11111110101010");
            strcpy_s(Maze[8], 31, "0000gggggg00005011111110101010");
            strcpy_s(Maze[9], 31, "0000gggggg01111011111110101010");
            strcpy_s(Maze[10], 31, "000000gg0005000011111110101010");
            strcpy_s(Maze[11], 31, "000000gg0001111011111110101010");
            strcpy_s(Maze[12], 31, "00000000000001101111111e151010");
            strcpy_s(Maze[13], 31, "011011111110111011111110101010");
            strcpy_s(Maze[14], 31, "001010101010111055000000101010");
            strcpy_s(Maze[15], 31, "001111111110100011111110101010");
            strcpy_s(Maze[16], 31, "001011010110111011111110101510");
            strcpy_s(Maze[17], 31, "0011111111001110e1111110101010");
            strcpy_s(Maze[18], 31, "000110101111100011111110101010");
            strcpy_s(Maze[19], 31, "011100100100111000000115101010");
            strcpy_s(Maze[20], 31, "010101101110001111110000000010");
            strcpy_s(Maze[21], 31, "010101000111111001011110111110");
            strcpy_s(Maze[22], 31, "010101110100011111100010500000");
            strcpy_s(Maze[23], 31, "010101010111010101101110111110");
            strcpy_s(Maze[24], 31, "011101011101110101101000e00010");
            strcpy_s(Maze[25], 31, "000101010111010101101011111110");
            strcpy_s(Maze[26], 31, "011111011101110001001011111110");
            strcpy_s(Maze[27], 31, "000001010111010111151111111110");
            strcpy_s(Maze[28], 31, "011111011101110111101111111110");
            strcpy_s(Maze[29], 31, "00000000000000000000000000e000");

        }
        else if (tmp == 1) {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "030011111111111111111101111110");
            strcpy_s(Maze[2], 31, "010010000000000050000101011110");
            strcpy_s(Maze[3], 31, "01001011111111111111010101111e");
            strcpy_s(Maze[4], 31, "010010000000500000000101011110");
            strcpy_s(Maze[5], 31, "010011111111111111110151e11110");
            strcpy_s(Maze[6], 31, "01e000000000e00000050000011110");
            strcpy_s(Maze[7], 31, "011111111111110111110111011110");
            strcpy_s(Maze[8], 31, "011111111111110111110101000110");
            strcpy_s(Maze[9], 31, "011101111111110111100101111110");
            strcpy_s(Maze[10], 31, "011101111111110111101100000500");
            strcpy_s(Maze[11], 31, "011101111111110111101011110110");
            strcpy_s(Maze[12], 31, "011101111111115111101111111110");
            strcpy_s(Maze[13], 31, "01110111111111511e101010000110");
            strcpy_s(Maze[14], 31, "0500000e0000000000001011110110");
            strcpy_s(Maze[15], 31, "011151015101111011011110000000");
            strcpy_s(Maze[16], 31, "011101010101111011010111111110");
            strcpy_s(Maze[17], 31, "011101010101011111110100101010");
            strcpy_s(Maze[18], 31, "011101510151000000100101111110");
            strcpy_s(Maze[19], 31, "000000000000011110101101010100");
            strcpy_s(Maze[20], 31, "00000000gg00010111111111111110");
            strcpy_s(Maze[21], 31, "00BB0000gg00011101101000101010");
            strcpy_s(Maze[22], 31, "00BB00gggggg010111000011111110");
            strcpy_s(Maze[23], 31, "00GG00gggggg011101111010000000");
            strcpy_s(Maze[24], 31, "00GG0000gg00010111001111111110");
            strcpy_s(Maze[25], 31, "00000000gg00011111100000001010");
            strcpy_s(Maze[26], 31, "000000000000000101111111111010");
            strcpy_s(Maze[27], 31, "0ooGG0000000011111010000101010");
            strcpy_s(Maze[28], 31, "0ooGG0000000010000011111101010");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");

        }
        else if (tmp == 2) {
            strcpy_s(Maze[0], 31, "000e00000000000000000000000000");
            strcpy_s(Maze[1], 31, "011111111101111011101110111110");
            strcpy_s(Maze[2], 31, "011111111151111010111010100000");
            strcpy_s(Maze[3], 31, "011111110100100011101110111110");
            strcpy_s(Maze[4], 31, "011111110101101010111010101000");
            strcpy_s(Maze[5], 31, "01000e000101101011101110101110");
            strcpy_s(Maze[6], 31, "011111011101101010111010101010");
            strcpy_s(Maze[7], 31, "000005010001111110001011101010");
            strcpy_s(Maze[8], 31, "011111011110100111111000101010");
            strcpy_s(Maze[9], 31, "010000000011111100011101101010");
            strcpy_s(Maze[10], 31, "010101511000000111001001001110");
            strcpy_s(Maze[11], 31, "010101011111110001111101011000");
            strcpy_s(Maze[12], 31, "0101010111111e0111001111111100");
            strcpy_s(Maze[13], 31, "015101011111110111011010110100");
            strcpy_s(Maze[14], 31, "010101011111110001011111111100");
            strcpy_s(Maze[15], 31, "010101000000550111010101010100");
            strcpy_s(Maze[16], 31, "010101011111110111011111110110");
            strcpy_s(Maze[17], 31, "010151e11111110110000000000000");
            strcpy_s(Maze[18], 31, "0101010111111101111000gg000000");
            strcpy_s(Maze[19], 31, "0101010111111100005000gg000000");
            strcpy_s(Maze[20], 31, "01010101111111011110gggggg0000");
            strcpy_s(Maze[21], 31, "01010101111111050000gggggg0000");
            strcpy_s(Maze[22], 31, "01010101111111e1111000gg000000");
            strcpy_s(Maze[23], 31, "0101010111111100005000gg000000");
            strcpy_s(Maze[24], 31, "010001011111110111100000000000");
            strcpy_s(Maze[25], 31, "011111011000000500000000000GG0");
            strcpy_s(Maze[26], 31, "000000011111110111100BBGG00GG0");
            strcpy_s(Maze[27], 31, "000000e11111110111100BBGG00oo0");
            strcpy_s(Maze[28], 31, "031111111111115111100000000oo0");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        else if (tmp == 3) {
            strcpy_s(Maze[0], 31, "000000000000000000000000000000");
            strcpy_s(Maze[1], 31, "0101011111100000100000000GGoo0");
            strcpy_s(Maze[2], 31, "0101010000101111100000000GGoo0");
            strcpy_s(Maze[3], 31, "010111111111101000000000000000");
            strcpy_s(Maze[4], 31, "01010000000111111000gg00000000");
            strcpy_s(Maze[5], 31, "01111111110011101000gg0000GG00");
            strcpy_s(Maze[6], 31, "000000010111101110gggggg00GG00");
            strcpy_s(Maze[7], 31, "011111110000111010gggggg00BB00");
            strcpy_s(Maze[8], 31, "01010100010110111000gg0000BB00");
            strcpy_s(Maze[9], 31, "01111111111111101000gg00000000");
            strcpy_s(Maze[10], 31, "001010101101011110000000000000");
            strcpy_s(Maze[11], 31, "011111101001000000151015101110");
            strcpy_s(Maze[12], 31, "010101001011111110101010101110");
            strcpy_s(Maze[13], 31, "011111111010110111101010101110");
            strcpy_s(Maze[14], 31, "000000011110110111101510151110");
            strcpy_s(Maze[15], 31, "0110111101000000000000e0000050");
            strcpy_s(Maze[16], 31, "011000010101e11511111111101110");
            strcpy_s(Maze[17], 31, "011111111101111511111111101110");
            strcpy_s(Maze[18], 31, "011011110101111011111111101110");
            strcpy_s(Maze[19], 31, "005000001101111011111111101110");
            strcpy_s(Maze[20], 31, "011111101001111011111111101110");
            strcpy_s(Maze[21], 31, "011000101011111011111111111110");
            strcpy_s(Maze[22], 31, "011110111011111011111111111110");
            strcpy_s(Maze[23], 31, "01111000005000000e000000000e10");
            strcpy_s(Maze[24], 31, "01111e151011111111111111110010");
            strcpy_s(Maze[25], 31, "011110101000000005000000010010");
            strcpy_s(Maze[26], 31, "e11110101011111111111111010010");
            strcpy_s(Maze[27], 31, "011110101000050000000000010010");
            strcpy_s(Maze[28], 31, "011111101111111111111111110030");
            strcpy_s(Maze[29], 31, "000000000000000000000000000000");
        }
        break;

    case 4: //Ending
        PlaySound(TEXT("�¸�.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        printGameSuccess();
        SetMaze();
        SetMaze();
        OutPut();
        return;
    case 5:
        printGameOver();
        //stage = -1;
        SetMaze();
        SetMaze(); // �ٲ� stage���� �̿��Ͽ� �ٽ� ����.
        OutPut();

        char cInput = NULL;

        while (_kbhit()) _getch();

    }
    if (locate > 0) return; // �հ��� �� ��쿡  ���̻� ��� �� ��.
    TextColor(DEEP_WHITE);

    if (stage != 0) // main�޴������� 'm'�� ���Ƶ�.(������ ���� �߻�) �׷��� �������� �ʵ���
    {

        gotoxy(MAP + 5, 3, "������������������������������");
        gotoxy(MAP + 7, 4, "      STAGE : "); printf("%d", stage);
        gotoxy(MAP + 5, 5, "������������������������������");
    }
    else
    {
        gotoxy(MAP + 5, 3, "������������������������������");
        gotoxy(MAP + 7, 4, "     TUTORIAL   ");
        gotoxy(MAP + 5, 5, "������������������������������");
    }

    gotoxy(MAP + 7, 8, "�� : �ݽð� ȸ��");
    gotoxy(MAP + 7, 10, "�� : �ð� ȸ��");

    TextColor(RED);
    gotoxy(MAP + 7, 14, "     <ITEM>");
    TextColor(WHITE);

    gotoxy(MAP + 7, 16, "SPACE : ��ź"); //printf("  %d", BombItem);

    gotoxy(MAP + 14, 16, "");

    TextColor(DEEP_OC);
    if (BombItem == 0) {
        printf("           ");
    }
    else if (BombItem == 1) {
        printf("��           ");
    }
    else if (BombItem == 2) {
        printf("���         ");
    }
    else if (BombItem == 3) {
        printf("����       ");
    }
    else if (BombItem == 4) {
        printf("�����     ");
    }
    else if (BombItem == 5) {
        printf("������   ");
    }
    TextColor(WHITE);


    finish = clock();
    t = finish - start;

    gotoxy(30 + 10, 26, ""); printf("  %.f   ", TimeLimit - (((float)t) / CLOCKS_PER_SEC));



    //gotoxy(MAP + 16, 18, ""); printf(" %d", npcPauseLimit - inpcCount);

    gotoxy(MAP + 14, 18, "");

    TextColor(DEEP_YELLOW);
    if (npcPauseLimit - inpcCount == 0) {
        printf("           ");
    }
    else if (npcPauseLimit - inpcCount == 1) {
        printf("           ");
    }
    else if (npcPauseLimit - inpcCount == 2) {
        printf("����         ");
    }
    else if (npcPauseLimit - inpcCount == 3) {
        printf("������       ");
    }
    else if (npcPauseLimit - inpcCount == 4) {
        printf("��������     ");
    }
    else if (npcPauseLimit - inpcCount == 5) {
        printf("����������   ");
    }
    TextColor(WHITE);


    gotoxy(MAP + 14, 24, ""); printf("%d", RotateCount);

    gotoxy(MAP + 7, 28, "LIFE : ");
    if (death == 3) {
        gotoxy(MAP + 11, 28, "��  ��  ��");
    }
    else if (death == 2) {
        gotoxy(MAP + 11, 28, "��  ��      ");
    }
    else if (death == 1) {
        gotoxy(MAP + 11, 28, "��          ");
    }


    TextColor(WHITE);
    gotoxy(MAP + 7, 18, "N : NPC ����");

    TextColor(DEEP_JAJU);
    gotoxy(MAP + 7, 24, "ȸ�� Ƚ��  : ");

    TextColor(DEEP_GREEN);
    gotoxy(MAP + 7, 26, "TIMER");

    ////�� stage ���� ������ ���� ��ǥ�� startPoint �� �ȴ�. (���߿� ����� ) 
    //if (stage != -1)
    //    tStartPos = tPlayerPosRED;

    while (_kbhit()) _getch();
}

void OutPut()
{
    /* ��ź ���� �̷� ����*/
    for (int i = 0; i < iBombCount; i++) {


        BombPoint.x = BombFirePos[tmp][i].x;
        BombPoint.y = BombFirePos[tmp][i].y;


        if (Maze[BombPoint.y + 1][BombPoint.x] == '5')
        {
            Maze[BombPoint.y + 1][BombPoint.x] = '1';
        }

        if (Maze[BombPoint.y - 1][BombPoint.x] == '5')
        {

            Maze[BombPoint.y - 1][BombPoint.x] = '1';
        }
        if (Maze[BombPoint.y][BombPoint.x + 1] == '5')
        {
            Maze[BombPoint.y][BombPoint.x + 1] = '1';

        }

        if (Maze[BombPoint.y][BombPoint.x - 1] == '5')
        {
            Maze[BombPoint.y][BombPoint.x - 1] = '1';

        }

    }


    for (int i = 0; i < 30; i++) //�� y
    {
        for (int j = 0; j < 30; j++) //�� x
        {
            /* ���� ���۽ÿ�*/
            if (RotateCount == 0)
            {
                /* ��ź ��ǥ�� '*'ǥ�� */
                for (int k = 0; k < BombLimit; k++)
                    Maze[BombItemPos[k].y][BombItemPos[k].x] = '*';

                /* PC ��ǥ�� ���� ǥ�� */
                Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '6';
                Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '2';

                for (int i = 0; i < arrowCount; i++)
                    Maze[arrowPos[i].y][arrowPos[i].x] = arrowVector[i];
                for (int i = 0; i < SpecialarrowCount; i++)
                    Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = SpecialarrowVector[i];

                /* ������ ������� ������ ������ �Ķ��� ������� �ʱ�*/
                if (stage == -1 || stage == 4 || stage == 5)
                {
                    Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '1';
                    Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '1';
                    for (int k = 0; k < BombLimit; k++)
                        Maze[BombItemPos[k].y][BombItemPos[k].x] = '1';
                    if (stage == -1)
                        titlePicture();
                }
            }

            switch (Maze[i][j])
            {
                //�ٹ̱� ��//
            case 's':   //sky �ϴû�
                TextColor(DEEP_OC);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'G':   //Gray ȸ��
                TextColor(GRAY);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'g':   //green �ʷϻ� ���ѳ���
                TextColor(GREEN);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'T':   //Tree ���ѳ���
                TextColor(DEEP_GREEN);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'o':   //�˾� ���� ��Ȳ
                TextColor(DEEP_YELLOW);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'b':   //brown ����
                TextColor(DEEP_JAJU);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
            case 'B':
                TextColor(DEEP_BLUE);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 't':   //title text
                TextColor(DEEP_WHITE);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case 'R':   //brown ����
                TextColor(DEEP_RED);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case ' ':
                if (num == 0)
                    gotoxy(LEFTSPACE + j, UPSPACE + i, " ");
                break;
            case '0':
            case 'e':
                TextColor(DEEP_WHITE);
                if (num == 0)
                {
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    // title�� ��� ���� �ڸ��� �� �����. (�ӵ��� ������ )
                    if (stage == -1 || stage == 7)
                    {
                        TextColor(DEEP_WHITE);
                        gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    }
                }
                break;
            case '2':
                // TextColor(BLUE);
                TextColor(DEEP_OC);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case '3':
                TextColor(DEEP_YELLOW);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case '5':
                TextColor(DEEP_OC);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case '6':
                TextColor(RED);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;
            case '9':
                TextColor(DEEP_OC);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;

            case '<':
            case '>':
            case '-':
            case '+':
                TextColor(DEEP_WHITE);
                switch (Maze[i][j])
                {
                case '+':
                case 'u':
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    break;
                case '-':
                case 'd':
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    break;
                case '>':
                case 'r':
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    break;
                case '<':
                case 'l':
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                    break;
                }

                break;

            case '#':           // special NPC >
            case '@':           // special NPC <
            case '&':           // special NPC +
            case '=':           // special NPC -
                TextColor(DEEP_WHITE);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");

                break;

            case '*':
                TextColor(DEEP_OC);
                gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                break;



            default: // 1 4 2�� �Էµ� ��쿡�� if������ ó�����ֱ�.    //  �н� PC ���� ��� ����@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                if (Maze[i][j] == '1' && tPlayerPosRED.x == j && tPlayerPosRED.y == i
                    || Maze[i][j] == '2' && tPlayerPosRED.x == j && tPlayerPosRED.y == i
                    || Maze[i][j] == '4' && tPlayerPosRED.x == j && tPlayerPosRED.y == i)
                {
                    TextColor(DEEP_RED);
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                }// ��ó�� �Ϸ�. ���Ŀ� �ٸ� ���� ��� ? ...
                if (Maze[i][j] == '1' || Maze[i][j] == '2')
                    gotoxy(LEFTSPACE + j, UPSPACE + i, " ");

                if (Maze[i][j] == '4')
                {
                    TextColor(DEEP_JAJU);
                    gotoxy(LEFTSPACE + j, UPSPACE + i, "��");
                } // 1,2,4 �κ��� if�� �켱������ ���� ó���ϵ��� �Ѵ�.
            }
        }

    }
    if (num == 0) // ó�� set���Ŀ� num == 1�̴�. 
        num++;
}
void CreateBomb()
{

    // FMOD
    //Fmod->update();

    int block = 0;
    int cnt = 0;

    if (BombItem == 0) // max = 5 ������ ������ ��Ʈ�� ������ ������ ��Ʈ���� ����!
        return;

    Sleep(50);
    // ��ź��ġ�� ��ġ�� �ʰų� �ִ���� ���� ���� ��쿡�� ���� count�� �µ��� �÷��̾� ��ġ ���� 

    tBombPos[iBombCount] = tPlayerPosBLUE;

    //Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '4';

    if (Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '5')
    {
        PlaySound(TEXT("29.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        cnt++;
    }

    if (Maze[tPlayerPosBLUE.y - 1][tPlayerPosBLUE.x] == '5')
    {
        cnt++;
        PlaySound(TEXT("29.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    }
    if (Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x + 1] == '5')
    {
        PlaySound(TEXT("29.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

        cnt++;
    }

    if (Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x - 1] == '5')
    {
        PlaySound(TEXT("29.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        cnt++;
    }


    if (cnt > 0) {

        if (tmp == 0) {

            BombFirePos[0][iBombCount] = tPlayerPosBLUE;   //   0�� �� �������� ����

            BombFirePos[1][iBombCount].x = BombFirePos[0][iBombCount].y;
            BombFirePos[1][iBombCount].y = 30 - BombFirePos[0][iBombCount].x - 1;

            BombFirePos[2][iBombCount].x = BombFirePos[1][iBombCount].y;
            BombFirePos[2][iBombCount].y = 30 - BombFirePos[1][iBombCount].x - 1;

            BombFirePos[3][iBombCount].x = BombFirePos[2][iBombCount].y;
            BombFirePos[3][iBombCount].y = 30 - BombFirePos[2][iBombCount].x - 1;

        }
        else if (tmp == 1) {


            BombFirePos[1][iBombCount] = tPlayerPosBLUE;   //   0�� �� �������� ����

            BombFirePos[2][iBombCount].x = BombFirePos[1][iBombCount].y;
            BombFirePos[2][iBombCount].y = 30 - BombFirePos[1][iBombCount].x - 1;

            BombFirePos[3][iBombCount].x = BombFirePos[2][iBombCount].y;
            BombFirePos[3][iBombCount].y = 30 - BombFirePos[2][iBombCount].x - 1;

            BombFirePos[0][iBombCount].x = BombFirePos[3][iBombCount].y;
            BombFirePos[0][iBombCount].y = 30 - BombFirePos[3][iBombCount].x - 1;



        }
        else if (tmp == 2) {


            BombFirePos[2][iBombCount] = tPlayerPosBLUE;   //   0�� �� �������� ����

            BombFirePos[3][iBombCount].x = BombFirePos[2][iBombCount].y;
            BombFirePos[3][iBombCount].y = 30 - BombFirePos[2][iBombCount].x - 1;

            BombFirePos[0][iBombCount].x = BombFirePos[3][iBombCount].y;
            BombFirePos[0][iBombCount].y = 30 - BombFirePos[3][iBombCount].x - 1;

            BombFirePos[1][iBombCount].x = BombFirePos[0][iBombCount].y;
            BombFirePos[1][iBombCount].y = 30 - BombFirePos[0][iBombCount].x - 1;



        }
        else if (tmp == 3) {

            BombFirePos[3][iBombCount] = tPlayerPosBLUE;   //   0�� �� �������� ����

            BombFirePos[0][iBombCount].x = BombFirePos[3][iBombCount].y;
            BombFirePos[0][iBombCount].y = 30 - BombFirePos[3][iBombCount].x - 1;

            BombFirePos[1][iBombCount].x = BombFirePos[0][iBombCount].y;
            BombFirePos[1][iBombCount].y = 30 - BombFirePos[0][iBombCount].x - 1;

            BombFirePos[2][iBombCount].x = BombFirePos[1][iBombCount].y;
            BombFirePos[2][iBombCount].y = 30 - BombFirePos[1][iBombCount].x - 1;

        }
        BombItem--;
        ++iBombCount;
        gotoxy(MAP + 16, 16, "");

        TextColor(DEEP_OC);
        if (BombItem == 0) {
            printf("           ");
        }
        else if (BombItem == 1) {
            printf("��           ");
        }
        else if (BombItem == 2) {
            printf("���         ");
        }
        else if (BombItem == 3) {
            printf("����       ");
        }
        else if (BombItem == 4) {
            printf("�����     ");
        }
        else if (BombItem == 5) {
            printf("������   ");
        }
        TextColor(WHITE);

    }

    // FMOD
    //Fmod->update();


}

void gravityRED()
{
    if (Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '3')
    {
        Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '1';
        ++tPlayerPosBLUE.y;
        Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '6';

        RotateCount = 0;
        tmp = 0;
        SetMaze();

        return;
    }

    if (Maze[tPlayerPosRED.y + 1][tPlayerPosRED.x] == '0'
        || Maze[tPlayerPosRED.y + 1][tPlayerPosRED.x] == 'w'
        || Maze[tPlayerPosRED.y + 1][tPlayerPosRED.x] == 'e'
        || Maze[tPlayerPosRED.y + 1][tPlayerPosRED.x] == '5')
        return;

    Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '1';
    ++tPlayerPosRED.y;
    Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '6';

    //���
    OutPut();

    //Sleep(arrowSpeed);
    Sleep(1);
}
void gravityBLUE()
{
    for (int i = 0; i < BombLimit; i++)
    {
        if (BombItemPos[i].x == tPlayerPosBLUE.x && tPlayerPosBLUE.y == BombItemPos[i].y) {
            if (Bombflag[i] == 0)
            {
                Bombflag[i] = 1;    // idx 0���� ��ź�� �Ծ����ϴ� flag
                BombItem++;
                PlaySound(TEXT("������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
                BombPause++;
            }
        }
    }

    if (tPlayerPosBLUE.x == tPlayerPosRED.x && (tPlayerPosBLUE.y + 1 == tPlayerPosRED.y || tPlayerPosBLUE.y == tPlayerPosRED.y))
    {
        death--;
        PlaySound(TEXT("death.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
        if (death == 0)
        {
            if (stage == 0)
            {
                death = 3;
            }
            // GameOver!
            else
                stage = 5;
        }
        RotateCount = 0;
        tmp = 0;
        idx = 0;
        SetMaze();
    }


    if (Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '0'
        || Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == 'w'
        || Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == 'e'
        || Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '5'
        || Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == 'G')
        return;

    Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '1';
    ++tPlayerPosBLUE.y;
    Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '2';

    if (Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '3')
    {
        if (stage != 0)
        {
            stagePlus();
            SetMaze();
            OutPut();
        }
    }

    //���
    OutPut();
    Sleep(arrowSpeed);
}
void MovePlayer(char cInput)
{
    npcItem = npcPauseLimit - inpcCount;
    switch (cInput)
    {
    case 'n':
    case 'N':
        if (npcItem == 0) // max = 5 ������ ������ ��Ʈ�� ������ ������ ��Ʈ���� ����!
            break;

        npcPause++;
        inpcCount++;
        gotoxy(MAP + 16, 18, ""); printf(" %d", npcItem);
        break;

    case LEFT:
        PlaySound(TEXT("click1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

        ++RotateCount;
        idx++;

        if (idx < 0)
        {
            tmp = abs(idx);
            tmp = tmp % 4;
            tmp = (4 - tmp) % 4;
        }
        else  tmp = idx % 4;

        Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '1';
        Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '1';

        for (int i = 0; i < BombLimit; i++)
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '1';

        SetMaze();
        RotateLEFT();   // �ε��� ����
        break;

    case RIGHT:
        PlaySound(TEXT("click1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

        ++RotateCount;
        --idx;

        if (idx < 0)
        {
            tmp = abs(idx);
            tmp = tmp % 4;
            tmp = (4 - tmp) % 4;
        }
        else  tmp = idx % 4;

        Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '1';
        Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '1';


        for (int i = 0; i < BombLimit; i++)
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '1';

        SetMaze();
        RotateRIGHT();
        break;
    }
}
//void ResetNPC() {
//
//    inpcCount = 0;
//    if (stage == 1) {
//        npcLimit = 2;
//    }
//    else if (stage == 2) {
//        npcLimit = 2;
//    }
//    else if (stage == 3) {
//        npcLimit = 3;
//    }
//}
void arrowMove()
{
    if (npcPause > 0)
    {
        npcPause++;
        if (npcPause > 40) {
            gotoxy(12, 1, "                          "); printf("                             ");
            npcPause = 0;
        }
        return;
    }

    for (int i = 0; i < arrowCount; i++)
    {
        if ((arrowPos[i].x == tPlayerPosBLUE.x && arrowPos[i].y == tPlayerPosBLUE.y) // arrow��ġ�� player��ġ�� ������ ���� ��ġ��. 
            || (arrowPos[i].x == tPlayerPosBLUE.x && arrowPos[i].y == tPlayerPosBLUE.y))
        {
            death--;
            PlaySound(TEXT("death.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            if (death == 0)
            {
                if (stage == 0)
                {
                    death = 3;
                }
                // GameOver!
                else
                    stage = 5;
            }
            ResetBomb();
            RotateCount = 0;
            tmp = 0;
            idx = 0;
            SetMaze();
            continue;
        }

        if (Maze[arrowPos[i].y][arrowPos[i].x] == '0') continue;

        switch (arrowVector[i])
        {
        case '>':
            Maze[arrowPos[i].y][arrowPos[i].x] = '1';
            arrowPos[i].x++;

            if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
                arrowPos[i] = arrowStart[i];
            if (Maze[arrowPos[i].y][arrowPos[i].x] == '6')
                arrowPos[i].x--;

            Maze[arrowPos[i].y][arrowPos[i].x] = '>';
            break;


        case '<':
            Maze[arrowPos[i].y][arrowPos[i].x] = '1';
            --arrowPos[i].x;

            if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
                arrowPos[i] = arrowStart[i];
            if (Maze[arrowPos[i].y][arrowPos[i].x] == '6')
                arrowPos[i].x++;
            Maze[arrowPos[i].y][arrowPos[i].x] = '<';
            break;

        case '-':
            Maze[arrowPos[i].y][arrowPos[i].x] = '1';
            ++arrowPos[i].y;

            if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
                arrowPos[i] = arrowStart[i];
            if (Maze[arrowPos[i].y][arrowPos[i].x] == '6')
                arrowPos[i].y--;
            Maze[arrowPos[i].y][arrowPos[i].x] = '-';
            break;

        case '+':
            Maze[arrowPos[i].y][arrowPos[i].x] = '1';
            --arrowPos[i].y;

            if (Maze[arrowPos[i].y][arrowPos[i].x] == 'e')
                arrowPos[i] = arrowStart[i];
            if (Maze[arrowPos[i].y][arrowPos[i].x] == '6')
                arrowPos[i].y++;
            Maze[arrowPos[i].y][arrowPos[i].x] = '+';

            break;
        }

        Sleep(arrowSpeed);
    }
}

void specialArrowMove() {

    /*ȭ��ǥNPC�� �����ϸ� �׸�ŭ �����NPC�� �ӵ��� �������� ���⼭ �ӵ� ����*/

    if (npcPause > 0)
    {
        SpecialarrowSpeed = arrowCount * arrowSpeed;
    }
    else
        SpecialarrowSpeed = arrowSpeed;

    for (int i = 0; i < SpecialarrowCount; i++)
    {
        if (SpecialarrowPos[i].x == tPlayerPosRED.x && SpecialarrowPos[i].y == tPlayerPosRED.y) // arrow��ġ�� player��ġ�� ������ ���� ��ġ��. 
        {
            continue;
        }

        if (SpecialarrowPos[i].x == tPlayerPosBLUE.x && SpecialarrowPos[i].y == tPlayerPosBLUE.y) // arrow��ġ�� player��ġ�� ������ ���� ��ġ��. 
        {
            death--;
            PlaySound(TEXT("death.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            if (death == 0)
            {
                if (stage == 0)
                    death = 3;
                // GameOver!
                else stage = 5;
            }
            ResetBomb();
            RotateCount = 0;
            tmp = 0;
            idx = 0;
            SetMaze();
            continue;
        }

        if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == '0') continue;

        switch (SpecialarrowVector[i]) {
        case '#':
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '1';
            SpecialarrowPos[i].x++;

            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == 'e')
                SpecialarrowPos[i] = SpecialarrowStart[i];
            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == '6')
                SpecialarrowPos[i].x--;

            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '#';
            break;
        case '@':
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '1';
            --SpecialarrowPos[i].x;

            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == 'e')
                SpecialarrowPos[i] = SpecialarrowStart[i];
            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == '6')
                SpecialarrowPos[i].x++;
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '@';
            break;

        case'&':
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '1';
            --SpecialarrowPos[i].y;

            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == 'e')
                SpecialarrowPos[i] = SpecialarrowStart[i];
            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == '6')
                SpecialarrowPos[i].y++;
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '&';

            break;

        case '=':
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '1';
            ++SpecialarrowPos[i].y;

            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == 'e')
                SpecialarrowPos[i] = SpecialarrowStart[i];
            if (Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] == '6')
                SpecialarrowPos[i].y--;
            Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = '=';
            break;


        }
        Sleep(SpecialarrowSpeed);
    }
}
void stagePlus()
{
    PlaySound(TEXT("Ŭ����.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    GameClearScore[0][stage] = (int)(((float)t) / CLOCKS_PER_SEC);
    GameClearScore[1][stage] = death;
    GameClearScore[2][stage] = RotateCount;


    RotateCount = 0;
    tmp = 0;
    idx = 0;

    ResetBomb();
    ResetTime();
    storycnt = 0;
    stage++;
}

void title() // Ÿ��Ʋ�� stage = -1���� �׸� �׸��� ���� �����̴�.
{
    PlaySound(TEXT("�������.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    for (int i = 0; i < arrowCount; i++)
        arrowPos[i] = { 0,0 };
    for (int i = 0; i < SpecialarrowCount; i++)
        SpecialarrowPos[i] = { 0,0 };


    while (_kbhit()) _getch();

    titlePicture();
    num = 0;
    stage = -1;
    death = 3;

    TextColor(DEEP_WHITE);
    gotoxy(MAP + 5, 9, "������������������������������");
    gotoxy(MAP + 5, 11, "������������������������������");


    TextColor(DEEP_YELLOW);
    gotoxy(MAP + 5, 10, "��     �˾��� Ż����Ѷ�   ��");
    TextColor(DEEP_WHITE);
    gotoxy(MAP + 5, 14, "        TUTORIAL       ");
    // ó�� ���ý� ���ο� ������ ��� ����.

    gotoxy(MAP + 5, 16, "        GAME START       ");
    gotoxy(MAP + 5, 18, "        END GAME       ");

    TextColor(WHITE);
    gotoxy(MAP + 5, 22, "��  press ENTER to select  ��");
    gotoxy(MAP + 5, 24, "   ��  ��  : up     ��   ");
    gotoxy(MAP + 5, 25, "   ��  �� : down    ��    ");

    // ICON �� ��ġ�� �ٲ��� �ʴ´�.
    titleIcon();

    int locate = 0; // ���� ������ ��������, ��� ��ġ ������. // ���⼭ ����ϴ� locate�� select���� ����ϴ� locate�� �ٸ���.
    char str[40];// ��й�ȣ�� �������� ����� �迭. �ʹ� �����ϰ� �ָ� ���ڸ� ���� �Է��� �� ��̾�����.

    TextColor(DEEP_RED);
    gotoxy(MAP + 5, 14, "  ��");
    // ��ǻ� ó������ �Է��� ���ٸ� ���� ���� �����Ƿ� �̸� ��Ƶ־� �Ѵ�.

    //�츮�� title���� while������ Ű���� ��
    // ���Ŀ� ���� �� ���� ó�� ..
    OutPut();

    while (_kbhit()) _getch(); // ����.

    while (true)
    {
        if (_kbhit())
        {
            char input = _getch();
            while (_kbhit()) input = _getch();
            switch (input)
            {
            case UP:
                titleIcon();
                switch (locate) // ������ ���� 4���� up -> 3���� �̵��� ���������� 3����ġ�� ���� ��. (���Ŀ� --ó��)
                {
                case 0:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 14, "  ��"); //�� �̻� �ö� ���� �����Ƿ� �ڽ��� �ڸ�.
                    break;
                case 1:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 14, "  ��");
                    break;
                case 2:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 16, "  ��");
                    break;
                }
                locate--;
                if (locate == -1)
                    locate = 0; // �� ��(0)���� ���� ���� ���. 
                 //���� �̵�
                break;
            case DOWN:
                titleIcon();
                switch (locate) // ������ ���� 0���� �Ʒ��� �̵� 1�� ��ġ�� ��� ���Ŀ� ++ ó��
                {
                case 0:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 16, "  ��");
                    break;
                case 1:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 18, "  ��");
                    break;
                case 2:
                    TextColor(DEEP_RED);
                    gotoxy(MAP + 5, 18, "  ��");
                    break;
                }
                locate++;
                if (locate == 3)
                    locate = 2; // �� �Ʒ����� �Ʒ��� ����. 
                break;
            case SPACEBAR:
            case ENTER:
                switch (locate)
                {
                case 0: // ���� �ϱ� (�⺻ ���� ���)
                    locate = 5;
                    stage = 0;
                    break;

                case 1: //Ʃ�丮�� 
                    locate = 5;
                    stage = 1; // ���Ŀ� Ʃ�丮�� ������������ Ŭ����� 1stage�� �̵�. Ÿ��Ʋ�� case -1:
                    break;
                case 2:// ����
                    exit(0);
                    break;
                }
                while (_kbhit()) _getch(); // ����. enter���Ŀ� ���� ���� �ϴ� ����. ( �������� ������ �̻��ϰ� ��)
                break;
            case ESC:
                exit(0);
                break;
            }
        }
        if (locate == 5)
        {
            while (_kbhit()) _getch(); // ����. 

            return;
        }// ���Ŀ� main -> set 1������������ . . .  // Ʃ�丮�� ����
    }//while true
}
void titleIcon()
{
    TextColor(DEEP_WHITE);

    gotoxy(MAP + 5, 14, "  ��");
    gotoxy(MAP + 5, 16, "  ��");
    gotoxy(MAP + 5, 18, "  ��");

}
void titlePicture()
{
    strcpy_s(Maze[0], 31, "  0000000000000000000000000000");
    strcpy_s(Maze[1], 31, " 0                           0");
    strcpy_s(Maze[2], 31, "0 ttt t t ttt                0");      // THE VACCINE
    strcpy_s(Maze[3], 31, "0  t  t t t                  0");
    strcpy_s(Maze[4], 31, "0  t  ttt ttt                0");
    strcpy_s(Maze[5], 31, "0  t  t t t       BBBB       0");
    strcpy_s(Maze[6], 31, "0  t  t t ttt    BGBBBB      0");
    strcpy_s(Maze[7], 31, "0               BGBBBBB      0");
    strcpy_s(Maze[8], 31, "0              BGBBBBBB      0");
    strcpy_s(Maze[9], 31, "0             BGBBBBBBB      0");
    strcpy_s(Maze[10], 31, "0            BGBBBBBBB       0");
    strcpy_s(Maze[11], 31, "0           RBBBBBBBB        0");
    strcpy_s(Maze[12], 31, "0          RRRBBBBBB         0");
    strcpy_s(Maze[13], 31, "0         RGRRRBBBB          0");
    strcpy_s(Maze[14], 31, "0        RGRRRRRBB           0");
    strcpy_s(Maze[15], 31, "0       RGRRRRRRB            0");
    strcpy_s(Maze[16], 31, "0      RGRRRRRRR             0");
    strcpy_s(Maze[17], 31, "0      RGRRRRRR              0");
    strcpy_s(Maze[18], 31, "0      RRRRRRR               0");
    strcpy_s(Maze[19], 31, "0      RRRRRR                0");
    strcpy_s(Maze[20], 31, "0       RRRR                 0");
    strcpy_s(Maze[21], 31, "0                            0");
    strcpy_s(Maze[22], 31, "0 t t  t   tt  tt t t  t ttt 0");
    strcpy_s(Maze[23], 31, "0 t t t t t   t   t tt t t   0");
    strcpy_s(Maze[24], 31, "0 t t ttt t   t   t tt t ttt 0");
    strcpy_s(Maze[25], 31, "0 t t t t t   t   t t tt t   0");
    strcpy_s(Maze[26], 31, "0 t t t t t   t   t t tt t   0");
    strcpy_s(Maze[27], 31, "0  t  t t  tt  tt t t  t ttt 0");
    strcpy_s(Maze[28], 31, "0                           0 ");
    strcpy_s(Maze[29], 31, "0000000000000000000000000000  ");
}
void cheat()
{
    stagePlus();
    SetMaze();
    OutPut();
}
void playerInform()
{
    finish = clock();
    t = finish - start;

    gotoxy(30 + 10, 26, ""); printf("  %.f   ", TimeLimit - (((float)t) / CLOCKS_PER_SEC));


    if ((TimeLimit - (((float)t) / CLOCKS_PER_SEC) <= 1)) {

        system("cls");
        printTimeOver();

        BombPause = 0;
        iBombCount;

        tmp = 0;
        idx = 0;
        RotateCount = 0;

        stage = -1; // title
        SetMaze();  // title�Լ��� �� => �޴��������� stage�� ����

        SetMaze(); // �ٲ� stage���� �̿��Ͽ� �ٽ� ����.
        OutPut();

        char cInput = NULL;
        while (_kbhit()) _getch();



        return;
    }



    gotoxy(MAP + 14, 16, "");

    TextColor(DEEP_OC);
    if (BombItem == 0) {
        printf("           ");
    }
    else if (BombItem == 1) {
        printf("��           ");
    }
    else if (BombItem == 2) {
        printf("���         ");
    }
    else if (BombItem == 3) {
        printf("����       ");
    }
    else if (BombItem == 4) {
        printf("�����     ");
    }
    else if (BombItem == 5) {
        printf("������   ");
    }
    TextColor(WHITE);

    //TextColor(DEEP_JAJU);
    //gotoxy(MAP + 7, 24, "ȸ�� Ƚ��  : "); printf("%d", RotateCount);
    //gotoxy(MAP + 16, 18, ""); printf(" %d", npcPauseLimit - inpcCount);

    gotoxy(MAP + 14, 18, "");

    TextColor(DEEP_YELLOW);
    if (npcPauseLimit - inpcCount == 0) {
        printf("           ");
    }
    else if (npcPauseLimit - inpcCount == 1) {
        printf("��          ");
    }
    else if (npcPauseLimit - inpcCount == 2) {
        printf("����         ");
    }
    else if (npcPauseLimit - inpcCount == 3) {
        printf("������       ");
    }
    else if (npcPauseLimit - inpcCount == 4) {
        printf("��������     ");
    }
    else if (npcPauseLimit - inpcCount == 5) {
        printf("����������   ");
    }
    TextColor(WHITE);

    gotoxy(MAP + 14, 24, ""); printf("%d", RotateCount);

    gotoxy(MAP + 7, 28, "LIFE : ");
    if (death == 3) {
        gotoxy(MAP + 11, 28, "��  ��  ��");
    }
    else if (death == 2) {
        gotoxy(MAP + 11, 28, "��  ��      ");
    }
    else if (death == 1) {
        gotoxy(MAP + 11, 28, "��          ");
    }

    if (BombPause > 0)
    {
        if (BombPause < 20)
            gotoxy(12, 1, "�� ��ź�� ȹ���ϼ̽��ϴ�! ��");
        BombPause++;
        if (BombPause >= 20)
        {
            gotoxy(12, 1, "                             ");
            BombPause = 0;
        }
    }

}
void tutorial(char input)
{
    switch (quest)
    {
    case 0: //0. Backspace�� �������ƿ� 0�϶� �ѹ� ����ϰ� �ϱ�. ���Ŀ� �̼� �����ÿ� �Ѿ�ִ� ��.
        if (number == 0)
        {
            gotoxy_C(5, 1, "�̷����� ȸ����Ű�� �Ķ����� Ż����Ѿ� �մϴ�", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
            Sleep(600);
            gotoxy(5, 1, "                                                ");

            gotoxy_C(5, 1, "Ÿ���� �ʰ��ǰų� ��(life)�� ��� �������           ", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
            Sleep(600);
            gotoxy(5, 1, "                                                ");

            gotoxy_C(5, 1, "GAME OVER �Դϴ�!                              ", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
            Sleep(600);
            gotoxy(5, 1, "                                                ");

            gotoxy_C(5, 1, "��/�� ����Ű�� ������ �̷θ� ȸ����Ű����.", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
            Sleep(600);
            gotoxy(5, 1, "                                                ");

            gotoxy_C(5, 1, "�̷ΰ� ȸ���Կ� ����", DEEP_WHITE); number = 1; //number =1 number�� ���ؼ� �ѹ��� ������ֵ��� ����.
            Sleep(600);
            gotoxy(5, 1, "                                                                  ");

            gotoxy_C(5, 1, "�Ķ���� �������� �߷��� �������� �������ϴ�.", DEEP_WHITE); number = 1;
            Sleep(600);
            gotoxy(5, 1, "                                                ");


            while (_kbhit()) _getch(); // ����. 
            number = 1;
        }
        if (tPlayerPosBLUE.x == 13 && tPlayerPosBLUE.y == 4)
        {
            quest++;
            number = 0;
        }
        break;
    case 1: // ��ź�� ȹ���ϼ���
        if (number == 0)
        {
            gotoxy_C(5, 1, "��ź�� ȹ���ϼ���                    ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                             ");

            gotoxy_C(5, 1, "�����ʿ� ȹ���� ��ź�� ǥ�õ˴ϴ�           ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                ");

            number = 1;
        }
        if (tPlayerPosBLUE.x == 28 && tPlayerPosBLUE.y == 19)
        {
            quest++;
            number = 0;
        }
        break;
    case 2://2. ȭ�쿡 ������ �׾��.
        if (number == 0)
        {
            gotoxy_C(5, 1, "N Ű�� ������ ȭ��ǥ NPC(->)�� �����ؿ�.                     ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                            ");
            //while (_kbhit()) _getch(); // ����. 

            gotoxy_C(5, 1, "������ �� ���۹��� NPC�� �������� �ʽ��ϴ�                  ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                             ");

            gotoxy_C(5, 1, "ȭ��ǥ NPC�� ������ ����, ���۹��� NPC �� ���� ����������.  ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                                ");

            gotoxy_C(5, 1, "NPC�� ������ ���� ������� ������ �ʱ�ȭ�˴ϴ�      ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                    ");

            gotoxy_C(5, 1, "Ʃ�丮�󿡼��� ���� �������� �־����ϴ�               ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                      ");

            number = 1;
        }
        if (tPlayerPosBLUE.y == 10 && (tPlayerPosBLUE.x == 19 || tPlayerPosBLUE.x == 20))
        {
            quest++;
            number = 0;
        }
        break;

    case 3:
        //3. �����̽��ٸ� ���� ��ź�� ��ġ
        if (number == 0)
        {

            number = 1;
            gotoxy_C(5, 1, "�������� ������ �ʰ� �̷θ� ȸ����Ű����.              ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                            ");

            gotoxy_C(5, 1, "������� ������ ���� �ϳ� ������� ������ �ʱ�ȭ�˴ϴ�.", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                           ");

            while (_kbhit()) _getch(); // ����. 

        }
        if (tPlayerPosBLUE.x == 18 && tPlayerPosBLUE.y == 15)
        {
            quest++;
            number = 0;
        }
        break;
    case 4:
        if (number == 0)
        {
            number = 1;
            gotoxy_C(5, 1, "�̷� ȸ���� �ؼ� Ż�ⱸ�� �ִ� ������ �̵��ϼ���.", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                    ");
        }
        if (tPlayerPosBLUE.y == 22 && tPlayerPosBLUE.x == 15)
        {
            quest++;
            number = 0;
        }
        break;
    case 5:
        if (number == 0)
        {
            number = 1;
            gotoxy_C(5, 1, "�����̽� �ٸ� ������ ��ź�� ����� �� �ֽ��ϴ�. ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                     ");
            gotoxy_C(5, 1, "��ź�� �����¿�, 4���� �������� �����ϴ�. ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                     ");

        }
        if (Maze[tPlayerPosBLUE.y + 1][tPlayerPosBLUE.x] == '3')
        {
            quest++;
            number = 0;
        }
        break;
    case 6:
        if (number == 0)
        {
            system("cls");
            gotoxy_C(15, 15, "Ʃ�丮�� Ż�⿡ �����ϼ̽��ϴ�.    ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                     ");

            gotoxy_C(15, 15, "���� ������ ������ ����� �ѢѢѢ� ", DEEP_WHITE);
            Sleep(600);
            gotoxy(5, 1, "                                                     ");

            Sleep(1000);

            stagePlus();
            SetMaze();
            OutPut();
        }

    }

}
void RotateLEFT()
{
    int tmp_y;
    int tmp_x;

    /* ������, �Ķ��� ��ǥ Rotate*/
    tmp_x = tPlayerPosRED.y;  // j
    tmp_y = 30 - tPlayerPosRED.x - 1;  // i
    tPlayerPosRED.y = tmp_y;
    tPlayerPosRED.x = tmp_x;

    tmp_x = tPlayerPosBLUE.y;
    tmp_y = 30 - tPlayerPosBLUE.x - 1;
    tPlayerPosBLUE.y = tmp_y;
    tPlayerPosBLUE.x = tmp_x;



    /* ��ź ��ǥ Rotate*/
    for (int i = 0; i < BombLimit; i++)
    {
        tmp_x = BombItemPos[i].y;
        tmp_y = 30 - BombItemPos[i].x - 1;
        BombItemPos[i].y = tmp_y;
        BombItemPos[i].x = tmp_x;
        if (Bombflag[i] == 1)   // ��ź�� ���� ����
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '1';
        else
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '*';
    }

    /* ȭ��ǥ NPC Rotate*/
    for (int i = 0; i < arrowCount; i++)
    {
        switch (arrowVector[i])
        {
        case '>':
            arrowVector[i] = '+';
            break;
        case '-':
            arrowVector[i] = '>';
            break;
        case '<':
            arrowVector[i] = '-';
            break;
        case '+':
            arrowVector[i] = '<';
            break;
        }
        tmp_x = arrowStart[i].y;
        tmp_y = 30 - arrowStart[i].x - 1;
        arrowStart[i].y = tmp_y;
        arrowStart[i].x = tmp_x;
        tmp_x = arrowPos[i].y;
        tmp_y = 30 - arrowPos[i].x - 1;
        arrowPos[i].y = tmp_y;
        arrowPos[i].x = tmp_x;
        Maze[arrowPos[i].y][arrowPos[i].x] = arrowVector[i];
    }

    /* ���׶��  NPC Rotate*/
    for (int i = 0; i < SpecialarrowCount; i++)
    {
        switch (SpecialarrowVector[i])
        {
        case '#':
            SpecialarrowVector[i] = '&';
            break;
        case '=':
            SpecialarrowVector[i] = '#';
            break;
        case '@':
            SpecialarrowVector[i] = '=';
            break;
        case '&':
            SpecialarrowVector[i] = '@';
            break;
        }
        tmp_x = SpecialarrowStart[i].y;
        tmp_y = 30 - SpecialarrowStart[i].x - 1;
        SpecialarrowStart[i].y = tmp_y;
        SpecialarrowStart[i].x = tmp_x;
        tmp_x = SpecialarrowPos[i].y;
        tmp_y = 30 - SpecialarrowPos[i].x - 1;
        SpecialarrowPos[i].y = tmp_y;
        SpecialarrowPos[i].x = tmp_x;
        Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = SpecialarrowVector[i];
    }

    Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '6';
    Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '2';
    OutPut();
}
void RotateRIGHT()
{
    int tmp_y;
    int tmp_x;

    /* ������, �Ķ��� ��ǥ Rotate*/
    tmp_y = tPlayerPosRED.x;  // j
    tmp_x = 30 - tPlayerPosRED.y - 1;  // i
    tPlayerPosRED.y = tmp_y;
    tPlayerPosRED.x = tmp_x;

    tmp_y = tPlayerPosBLUE.x;
    tmp_x = 30 - tPlayerPosBLUE.y - 1;
    tPlayerPosBLUE.y = tmp_y;
    tPlayerPosBLUE.x = tmp_x;


    /* ��ź ��ǥ Rotate*/
    for (int i = 0; i < BombLimit; i++)
    {
        tmp_y = BombItemPos[i].x;
        tmp_x = 30 - BombItemPos[i].y - 1;
        BombItemPos[i].y = tmp_y;
        BombItemPos[i].x = tmp_x;
        if (Bombflag[i] == 1)   // ��ź�� ���� ����
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '1';
        else
            Maze[BombItemPos[i].y][BombItemPos[i].x] = '*';
    }
    /* ȭ��ǥ NPC Rotate*/
    for (int i = 0; i < arrowCount; i++)
    {
        switch (arrowVector[i])
        {
        case '>':
            arrowVector[i] = '-';
            break;
        case '-':
            arrowVector[i] = '<';
            break;
        case '<':
            arrowVector[i] = '+';
            break;
        case '+':
            arrowVector[i] = '>';
            break;
        }
        tmp_y = arrowStart[i].x;
        tmp_x = 30 - arrowStart[i].y - 1;
        arrowStart[i].y = tmp_y;
        arrowStart[i].x = tmp_x;
        tmp_y = arrowPos[i].x;
        tmp_x = 30 - arrowPos[i].y - 1;
        arrowPos[i].y = tmp_y;
        arrowPos[i].x = tmp_x;
        Maze[arrowPos[i].y][arrowPos[i].x] = arrowVector[i];
    }
    /* ���׶��  NPC Rotate*/
    for (int i = 0; i < SpecialarrowCount; i++)
    {
        switch (SpecialarrowVector[i])
        {
        case '#':
            SpecialarrowVector[i] = '=';
            break;
        case '=':
            SpecialarrowVector[i] = '@';
            break;
        case '@':
            SpecialarrowVector[i] = '&';
            break;
        case '&':
            SpecialarrowVector[i] = '#';
            break;
        }
        tmp_y = SpecialarrowStart[i].x;
        tmp_x = 30 - SpecialarrowStart[i].y - 1;
        SpecialarrowStart[i].y = tmp_y;
        SpecialarrowStart[i].x = tmp_x;
        tmp_y = SpecialarrowPos[i].x;
        tmp_x = 30 - SpecialarrowPos[i].y - 1;
        SpecialarrowPos[i].y = tmp_y;
        SpecialarrowPos[i].x = tmp_x;
        Maze[SpecialarrowPos[i].y][SpecialarrowPos[i].x] = SpecialarrowVector[i];
    }

    Maze[tPlayerPosRED.y][tPlayerPosRED.x] = '6';
    Maze[tPlayerPosBLUE.y][tPlayerPosBLUE.x] = '2';
    OutPut();
}
void DrawItemBoard() {
    int x, y;
    SetCurrentCursorPos(GITEM_ORIGIN_X, GITEM_ORIGIN_Y);

    for (y = 0; y <= GITEM_HEIGHT; y++) {
        SetCurrentCursorPos(GITEM_ORIGIN_X, GITEM_ORIGIN_Y + y);

        if (y == 0 || y == GITEM_HEIGHT) {
            if (y == 0) printf("��");
            if (y == GITEM_HEIGHT) printf("��");
        }
        else printf("��");
    }
    for (y = 0; y <= GITEM_HEIGHT; y++) {
        SetCurrentCursorPos(GITEM_ORIGIN_X + (GITEM_WIDTH + 1) * 2, GITEM_ORIGIN_Y + y);

        if (y == 0 || y == GITEM_HEIGHT) {
            if (y == 0) printf("��");
            if (y == GITEM_HEIGHT) printf("��");
        }
        else printf("��");
    }
    for (x = 1; x < GITEM_WIDTH; x++) {
        SetCurrentCursorPos(GITEM_ORIGIN_X + x * 2, GITEM_ORIGIN_Y + GITEM_HEIGHT);
        printf("����");
    }
    for (x = 1; x < GITEM_WIDTH; x++) {
        SetCurrentCursorPos(GITEM_ORIGIN_X + x * 2, GITEM_ORIGIN_Y);
        printf("����");
    }

    TextColor(DEEP_WHITE);
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 1);
    printf("            < ��潺�丮 >");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 3);
    printf("�������� ���� ������ �ִ� ȯ�ڵ��� \n");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 4);
    printf("�Կ����̾���. �����, �� ������ �ڷγ�");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 5);
    printf("���̷����� ������ �Ǹ鼭 ������\n");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 6);
    printf("���ü���� �����ϰ� �ȴ�.         ");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 8);
    printf("ȯ�ڵ��� ������ �ڷγ��� ġ���ϱ� ���ؼ� ");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 9);
    printf("��� �Ķ��� "); TextColor(DEEP_BLUE); printf("��"); TextColor(DEEP_WHITE); printf("�� ���ߵǾ���");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 10);
    printf("������ ���赵�� ���ۿ��� �����ϴ� ������"); TextColor(DEEP_RED); printf("��"); TextColor(DEEP_WHITE);
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 11);
    printf("�� ��Ÿ���� ���Ҵ�. �Ķ���"); TextColor(DEEP_BLUE); printf("��"); TextColor(DEEP_WHITE); printf("��");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 12);
    printf("������"); TextColor(DEEP_RED); printf("��"); TextColor(DEEP_WHITE); printf("�� ������ ȿ���� �Ҵ´�");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 14);
    printf("�̷ο��� ������"); TextColor(DEEP_RED); printf("��"); TextColor(DEEP_WHITE); printf("�� ���� �Ķ���"); TextColor(DEEP_BLUE); printf("��"); TextColor(DEEP_WHITE); printf("����");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 15);
    printf("Ż����� ȯ�ڵ鿡�� �ǰ��� ��ã������\n");

    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 20);
    printf("��");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 19);
    printf("������������������������������������������");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 20);
    printf("��         Press Enter to START         ��");
    SetCurrentCursorPos(GITEM_ORIGIN_X + 3, GITEM_ORIGIN_Y + 21);
    printf("������������������������������������������");
}
void printHospital() {

    strcpy_s(Hospital[0], 31, "000000000000000000000000000");
    strcpy_s(Hospital[1], 31, "000000000022200000000000000");
    strcpy_s(Hospital[2], 31, "000000000022200000000000000");
    strcpy_s(Hospital[3], 31, "000000000022200000000000000");
    strcpy_s(Hospital[4], 31, "000000022222222200000000000");
    strcpy_s(Hospital[5], 31, "000000022222222200000000000");
    strcpy_s(Hospital[6], 31, "111111122222222211111110000");
    strcpy_s(Hospital[7], 31, "133333333322233333333310000");
    strcpy_s(Hospital[8], 31, "133333333322233333333310000");
    strcpy_s(Hospital[9], 31, "133333333322233333333310000");
    strcpy_s(Hospital[10], 31, "133333333333333333333310000");
    strcpy_s(Hospital[11], 31, "133111133111133111133310000");
    strcpy_s(Hospital[12], 31, "133100133100133100133310000");
    strcpy_s(Hospital[13], 31, "133100133100133100133310000");
    strcpy_s(Hospital[14], 31, "133111133111133111133310000");
    strcpy_s(Hospital[15], 31, "13333333333333333333331000");
    strcpy_s(Hospital[16], 31, "13333333331133333333331000");
    strcpy_s(Hospital[17], 31, "13311113311113311113331000");
    strcpy_s(Hospital[18], 31, "13310013331133310013331000");
    strcpy_s(Hospital[19], 31, "13310013111111310013331000");
    strcpy_s(Hospital[20], 31, "13311113100001311113331000");
    strcpy_s(Hospital[21], 31, "13333333100001333333331000");
    strcpy_s(Hospital[22], 31, "13333333100001333333331000");
    strcpy_s(Hospital[23], 31, "13333333100001333333331000");
    strcpy_s(Hospital[24], 31, "11111111111111111111111");
    strcpy_s(Hospital[25], 31, "000000000000000000000");

    for (int i = 0; i < 26; i++) //�� y
    {
        for (int j = 0; j < 31; j++) //�� x
        {
            switch (Hospital[i][j])
            {
            case '0':
                gotoxy(_X + j, _Y + i, " ");
                break;
            case '1':
                TextColor(DEEP_WHITE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '2':
                TextColor(DEEP_GREEN);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '3':
                TextColor(GRAY);
                TextColor(DEEP_BLUE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '4':
                TextColor(DEEP_RED);
                gotoxy(_X + j, _Y + i, "��");
                break;
            }
        }
    }
    DrawItemBoard();
    getchar();
}
void startStory()
{
    ScorePage();

    PlaySound(TEXT("Ű����5.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    storycnt++;



    if (stage == 1)
    {


        system("cls");
        SetCurrentCursorPos(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 1);
        TextColor(DEEP_WHITE);
        printf(" �� MISSION :");
        TextColor(DEEP_WHITE);

        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 3, "���⿡ �ɸ� �Ҿƹ������� Ÿ���÷縦 �����ؾ� �Ѵ�!", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 5, "  ġ�Ḧ ���� �Ķ� �˾��� �����ض�!", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 7, " �� ���� �˾��� ���ۿ��� �ʷ��Ѵ�!", DEEP_WHITE);

        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 13, "Press Enter to skip!");

        _getch();
        system("cls");

    }
    else if (stage == 2)
    {


        system("cls");

        SetCurrentCursorPos(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 1);
        TextColor(DEEP_WHITE);
        printf(" �� MISSION :");
        TextColor(DEEP_WHITE);

        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 3, "ġ�ſ� �ɸ� �ҸӴϿ��� ġ�� ġ������ �����ؾ� �Ѵ�.", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 5, "�ҸӴ��� ġ�Ű� ��������", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 7, "��������� �߾��� �������� ����� �� �ֵ��� ��������.", DEEP_WHITE);

        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 13, "Press Enter to skip!");

        _getch();
        system("cls");
    }
    else if (stage == 3)
    {
        system("cls");

        SetCurrentCursorPos(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 1);
        TextColor(DEEP_WHITE);
        printf(" �� MISSION :");
        TextColor(DEEP_WHITE);



        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 3, "�ڷγ��� �ɸ� �˹ٻ����� �ڷγ� ġ������ �����ؾ� �Ѵ�.", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 5, "�̷��� NPC�� �� ���ϰ� ���� �������� �� Ȱ���ؾ� �Ѵ�.", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 7, "�˹ٻ��� �ٽ� �˹ٸ� ������ �˹ٺ� ����", DEEP_WHITE);
        gotoxy_C(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 9, "�ϻ����� ���ư� �� �ְ� ��������.", DEEP_WHITE);

        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 13, "Press Enter to skip!");

        _getch();
        //Sleep(4000);

        system("cls");
    }
    else
        return;

}

void GameItemBoard()
{
    int x, y;

    TextColor(GRAY);
    SetCurrentCursorPos(GITEM_OVER_X, GITEM_OVER_Y + 4);
    printf(" �� PRESS ENTER TO RESTART GAME\n");
}

void ResetNPC() {
    inpcCount = 0;

    if (stage == 0) {
        /*NPC �ʱ���ġ ����*/
        arrowCount = 2;     // Ʃ�丮�󿡼� ȭ��ǥ�� ������ 2��
        npcPauseLimit = 2; // ���������� ����

        /*ù��° ȭ��ǥ ��ġ�� ����*/
        arrowStart[0].x = 10;
        arrowStart[0].y = 7;
        arrowPos[0] = arrowStart[0];
        arrowVector[0] = '>';
        /*�ι�° ȭ��ǥ ��ġ�� ����*/
        arrowStart[1].x = 10;
        arrowStart[1].y = 9;
        arrowPos[1] = arrowStart[1];
        arrowVector[1] = '>';

        /*special NPC ��ġ ����*/
        SpecialarrowCount = 1;
        SpecialarrowStart[0].x = 10;
        SpecialarrowStart[0].y = 8;
        SpecialarrowPos[0] = SpecialarrowStart[0];
        SpecialarrowVector[0] = '#';

    }
    else if (stage == 1) {
        /*NPC �ʱ���ġ ����*/
        arrowCount = 4;     // 1���忡�� ȭ��ǥ�� ������ 2��
        npcPauseLimit = 1; // ���������� ����

          /*ù��° ȭ��ǥ ��ġ�� ����*/
        arrowStart[0].x = 4;
        arrowStart[0].y = 16;
        arrowPos[0] = arrowStart[0];
        arrowVector[0] = '>';

        /*�ι�° ȭ��ǥ ��ġ�� ����*/
        arrowStart[1].x = 4;
        arrowStart[1].y = 17;
        arrowPos[1] = arrowStart[1];
        arrowVector[1] = '>';

        /*����° ȭ��ǥ ��ġ�� ����*/
        arrowStart[2].x = 15;
        arrowStart[2].y = 19;
        arrowPos[2] = arrowStart[2];
        arrowVector[2] = '<';
        /*�׹�° ȭ��ǥ ��ġ�� ����*/
        arrowStart[3].x = 15;
        arrowStart[3].y = 20;
        arrowPos[3] = arrowStart[3];
        arrowVector[3] = '<';

        /*special NPC ��ġ ����*/
        SpecialarrowCount = 0;

    }
    else if (stage == 2) {
        /*NPC �ʱ���ġ ����*/
        arrowCount = 2;     // 2���忡�� ȭ��ǥ�� ������ 3��
        npcPauseLimit = 2; // ���������� ����

        /*ù��° ȭ��ǥ ��ġ�� ����*/
        arrowStart[0].x = 11;
        arrowStart[0].y = 23;
        arrowPos[0] = arrowStart[0];
        arrowVector[0] = '+';
        /*�ι�° ȭ��ǥ ��ġ�� ����*/
        arrowStart[1].x = 13;
        arrowStart[1].y = 16;
        arrowPos[1] = arrowStart[1];
        arrowVector[1] = '-';

        /*special NPC ��ġ ����*/
        SpecialarrowCount = 4;

        SpecialarrowStart[0].x = 8;
        SpecialarrowStart[0].y = 27;
        SpecialarrowPos[0] = SpecialarrowStart[0];
        SpecialarrowVector[0] = '#';

        SpecialarrowStart[1].x = 3;
        SpecialarrowStart[1].y = 3;
        SpecialarrowPos[1] = SpecialarrowStart[1];
        SpecialarrowVector[1] = '#';

        SpecialarrowStart[2].x = 26;
        SpecialarrowStart[2].y = 11;
        SpecialarrowPos[2] = SpecialarrowStart[2];
        SpecialarrowVector[2] = '@';

        /*����° ȭ��ǥ ��ġ�� ����*/
        SpecialarrowStart[3].x = 17;
        SpecialarrowStart[3].y = 23;
        SpecialarrowPos[3] = SpecialarrowStart[3];
        SpecialarrowVector[3] = '&';
    }
    else if (stage == 3) {
        /*NPC �ʱ���ġ ����*/
        arrowCount = 3;     // 2���忡�� ȭ��ǥ�� ������ 3��
        npcPauseLimit = 3;   // ���������� ����

        /*ù��° ȭ��ǥ ��ġ�� ����*/
        arrowStart[0].x = 16;
        arrowStart[0].y = 2;
        arrowPos[0] = arrowStart[0];
        arrowVector[0] = '>';

        /*�ټ���° ȭ��ǥ ��ġ�� ����*/
        arrowStart[1].x = 26;
        arrowStart[1].y = 25;
        arrowPos[1] = arrowStart[1];
        arrowVector[1] = '-';
        /*������° ȭ��ǥ ��ġ�� ����*/
        arrowStart[2].x = 24;
        arrowStart[2].y = 28;
        arrowPos[2] = arrowStart[2];
        arrowVector[2] = '+';

        /*special NPC ��ġ ����*/
        SpecialarrowCount = 3;

        /*�ι�° ȭ��ǥ ��ġ�� ����*/
        SpecialarrowStart[0].x = 17;
        SpecialarrowStart[0].y = 12;
        SpecialarrowPos[0] = SpecialarrowStart[0];
        SpecialarrowVector[0] = '#';
        /*����° ȭ��ǥ ��ġ�� ����*/
        SpecialarrowStart[1].x = 22;
        SpecialarrowStart[1].y = 7;
        SpecialarrowPos[1] = SpecialarrowStart[1];
        SpecialarrowVector[1] = '@';
        /*�׹�° ȭ��ǥ ��ġ�� ����*/
        SpecialarrowStart[2].x = 22;
        SpecialarrowStart[2].y = 17;
        SpecialarrowPos[2] = SpecialarrowStart[2];
        SpecialarrowVector[2] = '@';
    }
}
void ResetBomb() {

    iBombCount = 0; //�� ��ź

    for (int i = 0; i < 5; i++)
        Bombflag[i] = 0;

    if (stage == 0) {

        BombItemPos[0].x = 15;  //���� �� �ִ� ��ź �������� ��ġ
        BombItemPos[0].y = 4;
        BombItemPos[1].x = 12;
        BombItemPos[1].y = 17;
        BombItemPos[2].x = 20;
        BombItemPos[2].y = 19;

        BombItem = 0;   // ȹ���� ��ź ����
        BombLimit = 3;  //�־����� ��ź ����
    }
    else if (stage == 1) {
        BombItemPos[0].x = 26;  //���� �� �ִ� ��ź �������� ��ġ
        BombItemPos[0].y = 1;
        BombItemPos[1].x = 11;
        BombItemPos[1].y = 9;
        BombItemPos[2].x = 10;
        BombItemPos[2].y = 18;
        BombItem = 0;           //ȹ���� ��ź ����
        BombLimit = 3;          //�־����� ��ź ����
    }
    else if (stage == 2) {
        BombItemPos[0].x = 28;  //���� �� �ִ� ��ź �������� ��ġ
        BombItemPos[0].y = 3;
        BombItemPos[1].x = 26;
        BombItemPos[1].y = 12;
        BombItemPos[2].x = 5;
        BombItemPos[2].y = 26;
        BombItemPos[3].x = 14;
        BombItemPos[3].y = 16;
        BombItem = 0;   // ȹ���� ��ź ����
        BombLimit = 4;  //�־����� ��ź ����
    }
    else if (stage == 3) {
        BombItemPos[0].x = 4;  //���� �� �ִ� ��ź �������� ��ġ
        BombItemPos[0].y = 28;

        BombItemPos[1].x = 9;
        BombItemPos[1].y = 26;

        BombItemPos[2].x = 8;
        BombItemPos[2].y = 13;

        BombItemPos[3].x = 13;
        BombItemPos[3].y = 11;

        BombItemPos[4].x = 26;
        BombItemPos[4].y = 6;

        BombItem = 0;   // ȹ���� ��ź ����
        BombLimit = 5;  //�־����� ��ź ����
    }


    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            BombFirePos[i][j].x = 0;
            BombFirePos[i][j].y = 0;
        }
    }
}
void ResetTime() {

    start = clock();

    if (stage == 0) {
        TimeLimit = 1000;
    }
    else if (stage == 1) {
        TimeLimit = 150;
    }
    else if (stage == 2) {
        TimeLimit = 190;
    }
    else if (stage == 3) {

        TimeLimit = 200;
    }
}

//void npc_Limit() {
//
//
//    if (stage == 1) {
//        npcLimit = 2;
//    }
//    else if (stage == 2) {
//        npcLimit = 2;
//    }
//    else if (stage == 3) {
//        npcLimit = 3;
//    }
//}
void printGameOver() {
    PlaySound(TEXT("�й�.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    strcpy_s(GAME_OVER[0], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[1], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[2], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[3], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[4], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[5], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[6], 100, "0000000004444000444400440004404444440000000000000000000000000000000");
    strcpy_s(GAME_OVER[7], 100, "0000000040000004000040404040404000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[8], 100, "0000000440000004000040400400404000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[9], 100, "0000000040004404444440400000404444440000000000000000000000000000000");
    strcpy_s(GAME_OVER[10], 100, "000000004444400400004040000040400000000000000000000000000000000000");
    strcpy_s(GAME_OVER[11], 100, "000000000000400400004040000040444444000000000000000000000000000000");
    strcpy_s(GAME_OVER[12], 100, "000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[13], 100, "000000000444440004000040044444004444440000000000000000000000000000");
    strcpy_s(GAME_OVER[14], 100, "000000004000004004000040040000004000004000000000000000000000000000");
    strcpy_s(GAME_OVER[15], 100, "000000004000004004000040044444004444440000000000000000000000000000");
    strcpy_s(GAME_OVER[16], 100, "000000004000004000400400040000004000040000000000000000000000000000");
    strcpy_s(GAME_OVER[17], 100, "000000000444440000044000044444404000004000000000000000000000000000");
    strcpy_s(GAME_OVER[18], 100, "000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[19], 100, "000000000000000000000000000000000000000000000000000000000000000000");


    for (int i = 0; i < 20; i++) //�� y
    {
        for (int j = 0; j < 100; j++) //�� x
        {
            switch (GAME_OVER[i][j])
            {
            case '0':
                gotoxy(_X + j, _Y + i, " ");
                break;
            case '1':
                TextColor(DEEP_WHITE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '2':
                TextColor(DEEP_GREEN);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '3':
                TextColor(GRAY);
                TextColor(DEEP_BLUE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '4':
                TextColor(DEEP_RED);
                gotoxy(_X + j, _Y + i, "��");
                break;
            }
        }
    }
    GameItemBoard();
    getchar();
    stage = -1;
}
void printGameSuccess() {

    system("cls");
    system("Color 7");
    strcpy_s(GAME_OVER[0], 100, "000000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[0], 100, "000000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[2], 100, "000000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[3], 100, "000000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[4], 100, "000000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[5], 100, "000000003333300033330033000330333333000000000000000000000000000000000");
    strcpy_s(GAME_OVER[6], 100, "000000030000000300003030303030300000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[7], 100, "000000330000000300003030030030300000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[8], 100, "000000030003300333333030000030333333300000000000000000000000000000000");
    strcpy_s(GAME_OVER[9], 100, "000000033333300300003030000030300000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[10], 100, "00000000000300300003030000030333333300000000000000000000000000000000");
    strcpy_s(GAME_OVER[11], 100, "00000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[12], 100, "00000033333030000033333300033300033333000000000000000000000000000000");
    strcpy_s(GAME_OVER[13], 100, "00000300000030000030000000030030030000300000000000000000000000000000");
    strcpy_s(GAME_OVER[14], 100, "00000300000030000033333300333333030333300000000000000000000000000000");
    strcpy_s(GAME_OVER[15], 100, "00000300000030000030000000300003030000300000000000000000000000000000");
    strcpy_s(GAME_OVER[16], 100, "00000033333033333033333300300003030000300000000000000000000000000000");



    int totalTime = 0;
    int totalCount = 0;
    int total;

    for (int i = 1; i < 4; i++) {

        totalTime += GameClearScore[0][i];
        totalCount += GameClearScore[2][i];

    }

    total = totalTime + totalCount;

    /*for (int i = 1; i < 4; i++) {
        gotoxy(10 + i * 10, GITEM_ORIGIN_Y + 17, ""); printf("%d ����", i );
        gotoxy(10 + i * 10, GITEM_ORIGIN_Y + 19, "�� �ɸ� �ð� : "); printf("%d��", GameClearScore[0][i ]);
        gotoxy(10 + i * 10, GITEM_ORIGIN_Y + 21, "�� ���� Ƚ�� : "); printf("%d��", 3 - GameClearScore[1][i]);
        gotoxy(10 + i * 10, GITEM_ORIGIN_Y + 23, "�� ȸ�� Ƚ�� : "); printf("%d��", GameClearScore[2][i]);

    }*/

    gotoxy(10 + 16, GITEM_ORIGIN_Y + 18, ""); printf("------- �ջ� ���� -------");
    gotoxy(10 + 17, GITEM_ORIGIN_Y + 20, "�� �� �ɸ� �ð� : "); printf("%d��", totalTime);
    gotoxy(10 + 17, GITEM_ORIGIN_Y + 22, "�� �� ���� Ƚ�� : "); printf("%d��", 3 - death);
    gotoxy(10 + 17, GITEM_ORIGIN_Y + 24, "�� �� ȸ�� Ƚ�� : "); printf("%d��", totalCount);


    if (death == 3) {

        strcpy_s(Grade[0], 13, "000000000000");     //S
        strcpy_s(Grade[2], 13, "000444444000");
        strcpy_s(Grade[3], 13, "003333333300");
        strcpy_s(Grade[4], 13, "005500005500");
        strcpy_s(Grade[5], 13, "002200000000");
        strcpy_s(Grade[6], 13, "006666666000");
        strcpy_s(Grade[7], 13, "000444444400");
        strcpy_s(Grade[8], 13, "000000003300");
        strcpy_s(Grade[9], 13, "005500005500");
        strcpy_s(Grade[10], 13, "002222222200");
        strcpy_s(Grade[11], 13, "000666666000");

    }

    else if (death == 2 && total <= 500) {

        strcpy_s(Grade[0], 13, "000000000000");   //A   :4
        strcpy_s(Grade[2], 13, "000004400000");
        strcpy_s(Grade[3], 13, "000044440000");
        strcpy_s(Grade[4], 13, "000444444000");
        strcpy_s(Grade[5], 13, "004440044400");
        strcpy_s(Grade[6], 13, "004400004400");
        strcpy_s(Grade[7], 13, "004444444400");
        strcpy_s(Grade[8], 13, "004444444400");
        strcpy_s(Grade[9], 13, "004400004400");
        strcpy_s(Grade[10], 13, "004400004400");
        strcpy_s(Grade[11], 13, "000000000000");

    }


    else if (death == 2 && total > 500) {
        strcpy_s(Grade[0], 13, "00000000000");  //B   :2
        strcpy_s(Grade[2], 13, "00222222000");
        strcpy_s(Grade[3], 13, "00222222220");
        strcpy_s(Grade[4], 13, "002200002200");
        strcpy_s(Grade[5], 13, "002200002200");
        strcpy_s(Grade[6], 13, "002222222000");
        strcpy_s(Grade[7], 13, "002222222000");
        strcpy_s(Grade[8], 13, "002200002200");
        strcpy_s(Grade[9], 13, "002200002200");
        strcpy_s(Grade[10], 13, "002222222200");
        strcpy_s(Grade[11], 13, "002222222000");

    }


    else if (death == 1 && total <= 500) {
        strcpy_s(Grade[0], 13, "000000000000");     //C   :3
        strcpy_s(Grade[2], 13, "000333333000");
        strcpy_s(Grade[3], 13, "003333333300");
        strcpy_s(Grade[4], 13, "003300003300");
        strcpy_s(Grade[5], 13, "003300000000");
        strcpy_s(Grade[6], 13, "003300000000");
        strcpy_s(Grade[7], 13, "003300000000");
        strcpy_s(Grade[8], 13, "003300003300");
        strcpy_s(Grade[9], 13, "003333333300");
        strcpy_s(Grade[10], 13, "000333333000");
        strcpy_s(Grade[11], 13, "000000000000");
    }

    else {
        strcpy_s(Grade[0], 13, "000000000000");     //D   :5
        strcpy_s(Grade[2], 13, "005555555000");
        strcpy_s(Grade[3], 13, "005555555500");
        strcpy_s(Grade[4], 13, "000550005500");
        strcpy_s(Grade[5], 13, "000550005500");
        strcpy_s(Grade[6], 13, "000550005500");
        strcpy_s(Grade[7], 13, "000550005500");
        strcpy_s(Grade[8], 13, "000550005500");
        strcpy_s(Grade[9], 13, "005555555500");
        strcpy_s(Grade[10], 13, "00555555000");
        strcpy_s(Grade[11], 13, "00000000000");

    }


    for (int i = 0; i < 20; i++) //�� y
    {
        for (int j = 0; j < 100; j++) //�� x
        {
            switch (GAME_OVER[i][j])
            {
            case '0':
                gotoxy(_X + j, _Y + i, " ");
                break;
            case '1':
                TextColor(DEEP_WHITE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '2':
                TextColor(DEEP_OC);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '3':
                TextColor(GRAY);
                TextColor(DEEP_BLUE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '4':
                TextColor(DEEP_RED);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '5':
                TextColor(DEEP_YELLOW);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '6':
                TextColor(DEEP_JAJU);
                gotoxy(_X + j, _Y + i, "��");
                break;
            }

        }
        Sleep(50);
    }


    gotoxy(10 + 10, GITEM_ORIGIN_Y + 17, "");

    for (int i = 0; i < 12; i++) //�� y
    {
        for (int j = 0; j < 13; j++) //�� x
        {
            switch (Grade[i][j])
            {
            case '0':
                gotoxy(_X + j + 10, _Y + i + 17, " ");
                break;
            case '1':
                TextColor(DEEP_WHITE);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            case '5':
                TextColor(DEEP_GREEN);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            case '2':
                TextColor(GRAY);
                TextColor(DEEP_OC);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            case '4':
                TextColor(DEEP_RED);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            case '3':
                TextColor(DEEP_YELLOW);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            case '6':
                TextColor(DEEP_JAJU);
                gotoxy(_X + j + 10, _Y + i + 17, "��");
                break;
            }

        }
    }




    //GameItemBoard();
    getchar();
    stage = -1;
}
void ScorePage()
{

    if (stage == 2)
    {
        system("cls");
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 7, "�ɸ� �ð� : "); printf("%d��", GameClearScore[0][stage - 1]);
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 9, "���� Ƚ�� : "); printf("%d��", 3 - GameClearScore[1][stage - 1]);
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 11, "ȸ�� Ƚ�� : "); printf("%d��", GameClearScore[2][stage - 1]);

        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 20, "                  \"Enter\" to next stage ��");


        _getch();
        system("cls");
    }
    else if (stage == 3)
    {
        system("cls");
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 7, "�ɸ� �ð� : "); printf("%d��", GameClearScore[0][stage - 1]);
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 9, "���� Ƚ�� : "); printf("%d��", 3 - GameClearScore[1][stage - 1]);
        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 11, "ȸ�� Ƚ�� : "); printf("%d��", GameClearScore[2][stage - 1]);




        gotoxy(GITEM_ORIGIN_X / 2 - 10, GITEM_ORIGIN_Y + 20, "                  \"Enter\" to next stage ��");




        _getch();
        system("cls");
    }
    else
        return;
}
void printTimeOver() {
    PlaySound(TEXT("�й�.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

    strcpy_s(GAME_OVER[0], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[1], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[2], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[3], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[4], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[5], 100, "0000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[6], 100, "0000000044444404444440440004404444440000000000000000000000000000000");
    strcpy_s(GAME_OVER[7], 100, "0000000000040000040000404040404000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[8], 100, "0000000000040000040000400400404000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[9], 100, "0000000000040000040000400000404444440000000000000000000000000000000");
    strcpy_s(GAME_OVER[10], 100, "000000000004000004000040000040400000000000000000000000000000000000");
    strcpy_s(GAME_OVER[11], 100, "000000000004000444444040000040444444000000000000000000000000000000");
    strcpy_s(GAME_OVER[12], 100, "000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[13], 100, "000000000444440004000040044444004444440000000000000000000000000000");
    strcpy_s(GAME_OVER[14], 100, "000000004000004004000040040000004000004000000000000000000000000000");
    strcpy_s(GAME_OVER[15], 100, "000000004000004004000040044444004444440000000000000000000000000000");
    strcpy_s(GAME_OVER[16], 100, "000000004000004000400400040000004000040000000000000000000000000000");
    strcpy_s(GAME_OVER[17], 100, "000000000444440000044000044444404000004000000000000000000000000000");
    strcpy_s(GAME_OVER[18], 100, "000000000000000000000000000000000000000000000000000000000000000000");
    strcpy_s(GAME_OVER[19], 100, "000000000000000000000000000000000000000000000000000000000000000000");


    for (int i = 0; i < 20; i++) //�� y
    {
        for (int j = 0; j < 100; j++) //�� x
        {
            switch (GAME_OVER[i][j])
            {
            case '0':
                gotoxy(_X + j, _Y + i, " ");
                break;
            case '1':
                TextColor(DEEP_WHITE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '2':
                TextColor(DEEP_GREEN);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '3':
                TextColor(GRAY);
                TextColor(DEEP_BLUE);
                gotoxy(_X + j, _Y + i, "��");
                break;
            case '4':
                TextColor(DEEP_RED);
                gotoxy(_X + j, _Y + i, "��");
                break;

            }
        }
    }
    GameItemBoard();
    getchar();
    stage = -1;
}