#include<iostream>
#include<conio.h>
#include<Windows.h>
#include"Text.h"
#include"Maze.h"
extern float speaker; // speaker ��ȯ.
extern int death;
extern int stage;
extern int create;
extern int quest;
extern int hansei;
extern int titleOn;
extern int arrowSpeedSet;
using namespace std;

#define ESC 27
#define SPACEBAR 32
#define BACKSPACE 8
#define LEFT 75
#define RIGHT 77

struct _tagPoint
{
    int x;
    int y;
};

int main()
{

    system("mode con cols=110 lines=34  "); //CLI ũ��
    system("THE VACCINE");//��� �̸� 

    /* Ŀ�� ���ֱ� */
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = false;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
    stage = -1; // title
    SetMaze();  // title�Լ��� �� => �޴��������� stage�� ����

    SetMaze(); // �ٲ� stage���� �̿��Ͽ� �ٽ� ����.
    char cInput = NULL;
    while (_kbhit()) _getch();
    while (true)
    {
        OutPut();
        //�̷� ���


        if (_kbhit())
        {
            cInput = _getch();
            while (_kbhit()) cInput = _getch();
            // ����Ű�� ���� ó���� ������ ��� �Ǿ� ���ֹ���. �����ν� X ,  m���� �ν�. ����Ű�� ��츦 ����Ͽ� _getch()
            switch (cInput)
            {
            case ESC:
                exit(0);
                break;
            case SPACEBAR:  // ��ź�� ��Ʈ��
                CreateBomb();
                break;
            case BACKSPACE: // �� ���� �����߿� back�� ���� ���̴�. ���� title���� ����.
                stage = -1;
                SetMaze();   //Ʃ�丮���̸� return
                SetMaze();   //Ʃ�丮���̸� return
                OutPut();
                if (stage != 0) // Ʃ�丮���� �ƴ� ����.
                    death++; // backspace ���� ������ ó��
                break;
            case '0':
                cheat();
                break;
            }
            MovePlayer(cInput);
            arrowMove();
            specialArrowMove();
            gravityRED();
            gravityBLUE();
        }
        else
        {
            arrowMove();
            specialArrowMove();
            gravityRED();
            gravityBLUE();
        }
        // �Է��� ������ ���� �޾� ó���ϰ� �����̴� ���ÿ� ȭ�쵵 �����̵��� �Ѵ�.
        // �Է��� ������ ȭ�츸 �����̵��� �Ѵ�.

        playerInform(); // �÷��̾� inform
        if (stage == 0) // tutorial
            tutorial(cInput);
    }
    system("pause");
    return 0;

}