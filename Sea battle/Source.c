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
	enum squareStatus field[FIELD_SIZE][FIELD_SIZE];//���� ������
	initField(field);
	printField(field, 1);

	shipsArrangement(field);//����������� �������

	enum squareStatus botField[FIELD_SIZE][FIELD_SIZE];//���� ����
	initField(botField);
	fillBotField(botField);//��������� ���� ����
	int playerShipsCount[] = { 4,3,2,1 };//���������� ������� ������
	int botShipsCount[] = { 4,3,2,1 };//���������� ������� ����������
	printGameSpace(field, botField, botShipsCount, playerShipsCount);

	gameLoop(playerShipsCount, botShipsCount, field, botField);//������� ����

	getResult(playerShipsCount);//���������� ����� ����

	exitGame();//����������� ������ ��� ������

	return 0;
}

