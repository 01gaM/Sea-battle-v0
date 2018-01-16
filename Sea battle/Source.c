#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "Header.h"

int main() {
	setlocale(LC_ALL, "RUS");
	printf_s("�������� �����������:\n");
	printf_s("o - ����� �������\n");
	printf_s(". - ������ ������\n");
	printf_s("x - ����������� ����� �������\n");
	printf_s("* - ����������� �������\n");
	enum squareStatus field[FIELD_SIZE][FIELD_SIZE];
	initField(field);
	printField(field, 1);
	int shipsCount[] = { 4,3,2,1 };//o - 4, oo - 3, ooo - 2, oooo - 1

	while (getSumm(shipsCount, 4) != 0) {////////////////���� ����������� ��������
		printShipsCount(shipsCount);
		enum shipType currType;//��� �������� �������
		struct Coordinate beginning;//���������� ������ �������
		struct Coordinate ending;//���������� ����� �������
		printf_s("�������� ��� �������: ");
		currType = getType(field, shipsCount);
		
		if (currType == TYPE1) {//����������� ���� ���������� ��� TYPE1
			printf_s("������� ��� ����������: ");
			beginning = getCoordinate(field);
		}
		else {//����������� 2 ���������� ��� ��������� �����
			get2Coordinates(&beginning, &ending, field, currType);			
		}

		////////�������� �� ������������ ��������
		if (addShip(field, beginning, ending, currType) == 0) {
			printf_s("������� ������������ � ���������. �������� ������ �������.\n");
		}
		else {
			shipsCount[currType - 1]--;
		}
		printField(field, 1);
	}

	enum squareStatus botField[FIELD_SIZE][FIELD_SIZE];
	initField(botField);
	fillBotField(botField);//��������� ���� ����
	struct Coordinate aim;
	struct Coordinate botAim;
	botAim.ch = '0';//�������� ������� ���� ���� 
	botAim.num = 0;
	int playerShipsCount[] = { 4,3,2,1 };//���������� ������� ������
	int botShipsCount[] = { 4,3,2,1 };//���������� ������� ����������
	printGameSpace(field, botField, botShipsCount, playerShipsCount);
	//������� ����
	while ((getSumm(playerShipsCount, 4) != 0) && (getSumm(botShipsCount, 4) != 0)) {
		//��� ������
		printf_s("\n������� ���������� ����: ");
		aim = getCoordinate(botField);
		int t = nextTurn(aim, botField, botShipsCount);//��������� ���� ������
		while ((t == 1) || (t == 2)) {//���� ����� ����� �� ����, �� ��������� ��� ��������� ���
			printGameSpace(field, botField, botShipsCount, playerShipsCount);
			printf_s("\n������� ���������� ����: ");
			aim = getCoordinate(botField);
			t = nextTurn(aim, botField, botShipsCount);
		}

		printGameSpace(field, botField, botShipsCount, playerShipsCount);
		if (t == 3) {//���� ��� ������� ���������� �����
			break;
		}

		//��� ����
		printf_s("\n��� ����������\n");
		botAim = botTurn(field, botField, botShipsCount, playerShipsCount, botAim);

	}
	if (getSumm(playerShipsCount, 4) == 0) {
		printf_s("\n�� ���������!");
	}
	else {
		printf_s("\n�� ��������!");
	}

	int x = getchar();
	while (x != '\n') {//���������� ������, �������� �����
		x = getchar();
	}
	printf_s("\n������� ����� ������ ��� ������: ");//��������� ����� ������
	getchar();
	return 0;
}

