#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "block.h"

//�⺻ ��Ʈ���� �Լ�
void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos(void);
void ShowBlock(char blockInfo[4][4]);
void DeleteBlock(char blockInfo[4][4]);
void RemoveCursor(void);

//�浹�˻�, ����ϰ�, ����̵� �Լ�
int DetectCollision(int posX, int posY, char blockModel[4][4]);
BlockDown();
ShiftLeft();
ShiftRight();
RotateBlock();
ProcessKeyInput();

DrawGameBoard();    //������ �׸��� �Լ�

void AddBlockToBoard(void); //���ǿ� �߰��ϴ� �Լ�
int IsGameOver(void);   //���� ���� Ȯ�� �Լ�

void RemoveFillUpLine(void);
void RedrawBlocks(void);
void printScore();

//�⺻ ���� �̸� ����
#define LEFT 75
#define RIGHT 77
#define UP 72
#define SPACEKEY 32

/* Size of gameboard */
#define GBOARD_WIDTH 10
#define GBOARD_HEIGHT 20

/* Starting point of gameboard */
#define GBOARD_ORIGIN_X 4 
#define GBOARD_ORIGIN_Y 2

//���� ����
int curPosX, curPosY, block_id, speed, score;
int gameBoardInfo[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2] = { 0 };

main()
{

    int x, y;

    printScore();

    for (y = 0;y < GBOARD_HEIGHT;y++) {         //�� ��ġ ���� ������ �迭 �ʱ�ȭ
        gameBoardInfo[y][0] = 1;
        gameBoardInfo[y][GBOARD_WIDTH + 1] = 1;
    }
    for (x = 0;x < GBOARD_WIDTH + 2;x++) {
        gameBoardInfo[GBOARD_HEIGHT][x] = 1;
    }



    speed = 10;
    srand((unsigned int)time(NULL));
    block_id = (rand() % 7) * 4;
    DrawGameBoard();

    curPosX = GetCurrentCursorPos().X;  //���� Ŀ���� ��ġ
    curPosY = GetCurrentCursorPos().Y;


    while (1) {


        RemoveCursor();

        if (IsGameOver())
            break;


        while (1) {

            if (BlockDown() == 0) {
                AddBlockToBoard();
                RemoveFillUpLine();
                
                block_id = (rand() % 7) * 4;
                break;
            }
            ProcessKeyInput();
        }

        SetCurrentCursorPos(14, 3);
        curPosX = GetCurrentCursorPos().X;  //���� Ŀ���� ��ġ ������Ʈ
        curPosY = GetCurrentCursorPos().Y;


    }

    SetCurrentCursorPos(11, 2);
    puts("Game Over!!");

    getchar();
    return 0;

}


void SetCurrentCursorPos(int x, int y)	//Ŀ���� ��ġ�� �����ϴ� �Լ�
{
    COORD positon = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), positon);

}
COORD GetCurrentCursorPos(void) {	//Ŀ���� ��ġ�� �˷��ִ� �Լ�

    COORD curPoint;
    CONSOLE_SCREEN_BUFFER_INFO curInfo;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curPoint.X = curInfo.dwCursorPosition.X;
    curPoint.Y = curInfo.dwCursorPosition.Y;

    return curPoint;
}
void ShowBlock(char blockInfo[4][4]) {	//��Ʈ���� ���� ����ϴ� �Լ�

    int x, y;
    COORD curPos = GetCurrentCursorPos();

    for (y = 0;y < 4;y++) {

        for (x = 0;x < 4;x++) {

            SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

            if (blockInfo[y][x] == 1)
                printf("��");
        }
    }

    SetCurrentCursorPos(curPos.X, curPos.Y);
}
void DeleteBlock(char blockInfo[4][4]) {	//��Ʈ���� ���� �����ϴ� �Լ�

    int x, y;
    COORD curPos = GetCurrentCursorPos();

    for (y = 0;y < 4;y++) {

        for (x = 0;x < 4;x++) {

            SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);

            if (blockInfo[y][x] == 1)
                printf("  ");
        }
    }

    SetCurrentCursorPos(curPos.X, curPos.Y);
}
void RemoveCursor(void) //Ŀ�� ������ ����

{

    CONSOLE_CURSOR_INFO curInfo;

    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

    curInfo.bVisible = 0;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

}







int DetectCollision(int posX, int posY, char blockModel[4][4]) { //�� �浹 �˻� �Լ�

    int x, y;
    int arrX, arrY;

    arrX = (posX - GBOARD_ORIGIN_X) / 2;
    arrY = (posY - GBOARD_ORIGIN_Y);

    for (x = 0;x < 4;x++) {
        for (y = 0;y < 4;y++) {
            if (blockModel[y][x] == 1 && gameBoardInfo[arrY + y][arrX + x] == 1)
                return 0;
        }
    }
    return 1;
}
BlockDown() {  //�Ʒ��� ��ĭ �̵� �Լ�

    if (!DetectCollision(curPosX, curPosY + 1, blockModel[block_id]))
        return 0;

    DeleteBlock(blockModel[block_id]);
    curPosY += 1;
    SetCurrentCursorPos(curPosX, curPosY);
    ShowBlock(blockModel[block_id]);

}
ShiftLeft() {  //�������� ��ĭ �̵� �Լ�

    if (!DetectCollision(curPosX - 2, curPosY, blockModel[block_id]))  //���� ���� �浹�ϸ� �̵�X
        return;

    DeleteBlock(blockModel[block_id]);
    curPosX -= 2;
    SetCurrentCursorPos(curPosX, curPosY);
    ShowBlock(blockModel[block_id]);

}
ShiftRight() {  //�������� ��ĭ �̵� �Լ�


    if (!DetectCollision(curPosX + 2, curPosY, blockModel[block_id]))  //������ ���� �浹�ϸ� �̵�X
        return;

    DeleteBlock(blockModel[block_id]);
    curPosX += 2;
    SetCurrentCursorPos(curPosX, curPosY);
    ShowBlock(blockModel[block_id]);

}
RotateBlock() {  //�ð� �ݴ���� 90�� ȸ��

    int block_senior = block_id - block_id % 4;
    int block_rotated = block_senior + (block_id + 1) % 4;

    if (!DetectCollision(curPosX, curPosY, blockModel[block_rotated])) //ȸ���� ����� �浹���� ������ ����
        return 0;


    DeleteBlock(blockModel[block_id]);
    block_id = block_rotated;
    ShowBlock(blockModel[block_id]);
}
void SpaceDown() {

    while (BlockDown());

}





ProcessKeyInput() {     //�ð��� �°� ����� �̵������ִ� �Լ�
    int i, key;

    for (i = 0;i < 20;i++) {
        if (_kbhit() != 0) {
            key = _getch();
            switch (key) {
            case LEFT:
                ShiftLeft();
                break;
            case RIGHT:
                ShiftRight();
                break;
            case UP:
                RotateBlock();
                break;
            case SPACEKEY:
                SpaceDown();
                break;
            }
        }
        Sleep(speed);
    }
}

DrawGameBoard() {   //�������� �׸��� �Լ�

    int x, y;
    for (y = 0; y <= GBOARD_HEIGHT;y++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT)
            printf("��");
        else
            printf("��");

        SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1) * 2, GBOARD_ORIGIN_Y + y);
        if (y == GBOARD_HEIGHT)
            printf("��");
        else
            printf("��");

    }

    for (x = 1; x < GBOARD_WIDTH + 1;x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
        printf("��");
    }

    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH, 0);

}


void AddBlockToBoard(void) {

    int x, y, arrCurX, arrCurY;

    for (y = 0;y < 4;y++) {
        for (x = 0;x < 4;x++) {

            arrCurX = (curPosX - GBOARD_ORIGIN_X) / 2;
            arrCurY = curPosY - GBOARD_ORIGIN_Y;

            if (blockModel[block_id][y][x] == 1)
                gameBoardInfo[arrCurY + y][arrCurX + x] = 1;

        }
    }

}
int IsGameOver(void) {

    if (!DetectCollision(curPosX, curPosY, blockModel[block_id])) {
        return 1;
    }
    return 0;
}
void RemoveFillUpLine(void) {

    int line;
    int x, y;

    for (y = GBOARD_HEIGHT - 1;y > 0;y--) {
        
        for (x = 1;x < GBOARD_WIDTH + 1;x++) {
            if (gameBoardInfo[y][x] != 1)
                break;
        }
        if (x == (GBOARD_WIDTH+1)) {
            score += 10;
            for (line = 0; y - line > 0;line++) {
                memcpy(&gameBoardInfo[y - line][1], &gameBoardInfo[(y - line)-1][1], GBOARD_WIDTH * sizeof(int));
            }
            y++;
        }
    }

    printScore();
    RedrawBlocks();

}

void RedrawBlocks(void){
    int x, y;

    int cursX, cursY;

    for (y = 0; y < GBOARD_HEIGHT; y++)

    {
        for (x = 1; x < GBOARD_WIDTH + 1; x++)

        {
            cursX = x * 2 + GBOARD_ORIGIN_X;

            cursY = y + GBOARD_ORIGIN_Y;

            SetCurrentCursorPos(cursX, cursY);

            if (gameBoardInfo[y][x] == 1)

            {
                printf("��");

            }

            else { printf("  "); }

        }

    }

}

void printScore() {

    SetCurrentCursorPos(30, 10);
    printf("Score: %d", score);

}