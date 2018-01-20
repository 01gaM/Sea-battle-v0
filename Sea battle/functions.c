#include "Header.h"
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

void initField(enum squareStatus field [FIELD_SIZE][FIELD_SIZE]) {
	for (int i = 0; i < FIELD_SIZE; i++) {
		for (int j = 0; j < FIELD_SIZE; j++) {
			field[i][j] = EMPTY;//"��������" ����
		}
	}
}

void printBorder() {
	int stripeLen = 41;
	printf_s("\n  ");
	for (int i = 0; i < stripeLen; i++) {
		printf_s("-");
	}
}

void printField(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int mode) {
	char ch = 'A';
	printf_s("  ");
	for (int i = 1; i <= FIELD_SIZE; i++) {
		printf_s("  %d ", i);
	}
	printBorder();
	printf_s("\n");
	for (int i = 0; i < FIELD_SIZE; i++) {
		printf_s("%c |", ch);
		for (int j = 0; j < FIELD_SIZE; j++) {
			if ((mode == 2) && (field[i][j] == SHIP_PART)) {
				printf_s("   |");
			}
			else {
				switch (field[i][j]) {
				case EMPTY:
					printf_s("   |");
					break;
				case CHECKED:
					printf_s(" . |");
					break;
				case SHIP_PART:
					printf_s(" o |");
					break;
				case DAMAGED:
					printf_s(" x |");
					break;
				case DESTROYED:
					printf_s(" * |");
					break;
				}
			}
		}
		printBorder();
		printf_s("\n");
		ch++;
	}
}

int getSumm(int arr[], int size) {
	int s = 0;
	for (int i = 0; i < size; i++) {
		s += arr[i];
	}
	return s;
}

void printShipsCount(int shipsCount[]) {
	printf_s("���������� �������:\n");
	for (int i = 0; i < 4; i++) {
		printf_s("��� %d: %d - ", i + 1, shipsCount[i]);
		for (int j = 0; j <= i; j++) {
			printf_s("o");
		}
		printf_s("\n");
	}
}

int isOnField(int checkCoordinate1, int checkCoordinate2) {
	return ((checkCoordinate1 >= 0) && (checkCoordinate1 <= 9) && (checkCoordinate2 >= 0) && (checkCoordinate2 <= 9)) ? 1 : 0;//1 - ���������� �� ����, 0 - ���������� �� �� ����
}

int checkNeighbours(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ) {	
	for (int i = currI-1; i <= currI+1; i++) {
		for (int j = currJ-1; j <= currJ+1; j++) {
			if ((isOnField(i, j) == 1) && (map[i][j] != EMPTY)) {
				return 0;
			}
		}
	}
	return 1;//����� �� ������������ � ���������
}

int addShip(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], struct Coordinate b, struct Coordinate e, enum shipType type) {//��������� �������
	if (type == TYPE1) {
		int num2 = b.ch - 'A';
		if (checkNeighbours(map, num2, b.num - 1) == 0) {
			return 0;//������ ��������� ������� � ���� �����
		}
		map[num2][b.num - 1] = SHIP_PART;//��������� �������
	}
	else {
		int num1 = b.ch - 'A';
		int num2 = e.ch - 'A';

		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				if (checkNeighbours(map, i, j - 1) == 0) {
					return 0;//������ ��������� ������� � ���� �����
				}
			}
		}
		for (int i = num1; i <= num2; i++) {
			for (int j = b.num; j <= e.num; j++) {
				map[i][j - 1] = SHIP_PART;//��������� �������
			}
		}
	}
	return 1;//������� ��� �������� �� ����
}

int getShipLen(struct Coordinate b, struct Coordinate e) {//�������� ����� �������
	int s1 = e.ch - b.ch;
	int s2 = e.num - b.num;
	return (s1 > s2) ? s1 : s2;
}

void fillBotField(enum squareStatus map[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate begin;
	struct Coordinate end;
	srand(time(NULL));//������������� rand
	int botShipsCount[] = { 4,3,2,1 };//o - 4, oo - 3, ooo - 2, oooo - 1
	while (getSumm(botShipsCount, 4) != 0) {
		for (int currType = 3; currType >= 0; currType--) {
			while (botShipsCount[currType] > 0) {//���� ������� ����� ���� �� �����������
				begin.ch = rand() % 10 + 'A';//�������� �� 'A' �� 'J'
				begin.num = rand() % 10 + 1;//�������� �� 1 �� 10
				end.ch = '-';//"�������� ���������� �����"
				end.num = -1;
				int currI = begin.ch - 'A';
				if (currI + currType < 9) {
					end.ch = begin.ch + currType;//������������ ������������ �������
					end.num = begin.num;
				}
				else {
					if (begin.num + currType < 9) {
						end.num = begin.num + currType;
						end.ch = begin.ch;
					}
				}
				if ((end.ch != '-') && (end.num != -1)) {//���� ������� ������ � ����
														 //�������� �������� ������� �� ����
					if (addShip(map, begin, end, currType + 1) == 1) {//���� ������
						botShipsCount[currType]--;//������� ������� �� ������ ���������
					}
				}
			}
		}
	}

}

void coordinateToInt(struct Coordinate c, int* c1, int* c2) {//��������� ���������� � �������� �� 0 �� 9
	*c1 = c.ch - 'A';
	*c2 = c.num - 1;
}


int isEmpty(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int c1, int c2) {
	return (field[c1][c2] == EMPTY) || (field[c1][c2] == CHECKED) ? 1 : 0;//1 - ������ ������, 0 - ������ �� ������
}

int checkCoordinate(int c1, int c2,  int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int checkCoordinate1 = c1 + vertical;
	int checkCoordinate2 = c2 + horizontal;
	while ((isOnField(checkCoordinate1, checkCoordinate2) == 1) && (isEmpty(field, checkCoordinate1, checkCoordinate2) == 0)) {//���� �� ����� �� ����� ���� ��� �� ����� �������� �������
		if (field[checkCoordinate1][checkCoordinate2] == SHIP_PART) {
			return 0;//������� ������������� ����� �������
		}
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
	return -1;//�� ������� ������������� ����� �������
}

int isKilled(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	//��������� ����, ���, ���� � ����� �� ������� ������������� ������ �������
	//1 - ������� "����", 0 - ������� �� ��������� ��������
	return ((checkCoordinate(c1, c2, -1, 0, field) == 0) || (checkCoordinate(c1, c2, 1, 0, field) == 0) || (checkCoordinate(c1, c2, 0, -1, field) == 0) || (checkCoordinate(c1, c2, 0, 1, field) == 0)) ? 0 : 1;
}

void markAsChecked(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int checkCoordinate1, int checkCoordinate2, int isHorizontal, int isVertical, int horizontal, int vertical, int* shipLen) {
	while ((isOnField(checkCoordinate1, checkCoordinate2) == 1) && (isEmpty(field, checkCoordinate1, checkCoordinate2) == 0)) {//���� �� ����� �� ����� ���� ��� �� ����� �������� �������
		field[checkCoordinate1][checkCoordinate2] = DESTROYED;

		if ((isOnField(checkCoordinate1 - isHorizontal, checkCoordinate2 - isVertical) == 1) && (isEmpty(field, checkCoordinate1 - isHorizontal, checkCoordinate2 - isVertical) == 1)) {
			field[checkCoordinate1 - isHorizontal][checkCoordinate2 - isVertical] = CHECKED;//����� ������ ��� ������������� � ������� ��� ���������������
		}
		if ((isOnField(checkCoordinate1 + isHorizontal, checkCoordinate2 + isVertical) == 1) && (isEmpty(field, checkCoordinate1 + isHorizontal, checkCoordinate2 + isVertical) == 1)) {
			field[checkCoordinate1 + isHorizontal][checkCoordinate2 + isVertical] = CHECKED;//������ ������ ��� ������������� � ������ ��� ���������������
		}
		//���� ����� �� ����� �������, �� �������� ������ �� ���������
		if ((isOnField(checkCoordinate1 + vertical, checkCoordinate2 + horizontal) == 1) && (isEmpty(field, checkCoordinate1 + vertical, checkCoordinate2 + horizontal) == 1)) {
			field[checkCoordinate1 + vertical][checkCoordinate2 + horizontal] = CHECKED;//��� ������������� - ������ ������ ��� �����, ��� ��������������� - ������ ����� ��� ������ (� ����������� �� �����������, ��������� � ���������� �������)
			if (isOnField(checkCoordinate1 + vertical - isHorizontal, checkCoordinate2 + horizontal - isVertical) == 1) {
				field[checkCoordinate1 + vertical - isHorizontal][checkCoordinate2 + horizontal - isVertical] = CHECKED;//����� ������ ��� ������������� � ������� ��� ���������������
			}
			if (isOnField(checkCoordinate1 + vertical + isHorizontal, checkCoordinate2 + horizontal + isVertical) == 1) {
				field[checkCoordinate1 + vertical + isHorizontal][checkCoordinate2 + horizontal + isVertical] = CHECKED;//������ ������ ��� ������������� � ������ ��� ���������������
			}
		}
		*shipLen = *shipLen + 1;
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
}

int destroyLine(int c1, int c2, int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int isHorizontal, int* shipLen) {
	int checkCoordinate1 = c1;
	int checkCoordinate2 = c2;
	int isVertical = 0;
	if (isHorizontal == 0) {//��������� ����������� �������
		isVertical = 1;
	}
	markAsChecked(field, checkCoordinate1, checkCoordinate2, isHorizontal, isVertical, horizontal, vertical, shipLen);//�������� �������� ������, ��� ����������� ������
}

void destroyShip(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	int checkCoordinate1;
	int checkCoordinate2;
	int shipLen = -4;//����� ������� - 4 (�.�. ��������� �������� ���������� � 4-� ������������)
	field[c1][c2] = DAMAGED;
	//��������� ����
	destroyLine(c1, c2, -1, 0, field, 0, &shipLen);
	//��������� ���
	destroyLine(c1, c2, 1, 0, field, 0, &shipLen);
	//��������� ����
	destroyLine(c1, c2, 0, -1, field, 1, &shipLen);
	//��������� �����
	destroyLine(c1, c2, 0, 1, field, 1, &shipLen);
	shipCount[shipLen]--;//������� ������� �� ������ ����������
}

int nextTurn(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	///���� � �������� ������� �� ��� ����� ������� ����������, �� �������� ������ �� "x"
	//���� � �������� ������� ��� ����� ������� ����������(��� �� ���� 1), �� �������� ��� ������ ������� �� "*"
	int c1;
	int c2;
	coordinateToInt( aim, &c1, &c2);

	if (field[c1][c2] == EMPTY) {//��������� ����
		field[c1][c2] = CHECKED;
		return 0;
	}
	if (field[c1][c2] != SHIP_PART) {//��������� ����� ������ ��� �����
		return -1;
	}

	if (isKilled(aim, field) == 0) {
		field[c1][c2] = DAMAGED;
		return 1;//������� '�����'
	}
	else {
		destroyShip(aim, field, shipCount);///�������� ��� ������ ������� �� *
	}
	return getSumm(shipCount, 4) == 0 ? 3 : 2;//3 - ���� ���������, 2 - ������� ����
}

void printGameSpace(enum squareStatus player1[FIELD_SIZE][FIELD_SIZE], enum squareStatus player2[FIELD_SIZE][FIELD_SIZE], int botCount[], int playerCount[]) {
	printf_s("���� ����:\n");
	printField(player1, 1);// 1 - ����� 1
	printShipsCount(playerCount);
	printf_s("\n���� ����������:\n");
	printField(player2, 2);//2 - ����� 2
	printShipsCount(botCount);
}


int hitLine(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], int vertical, int horizontal) {
	int x;//��������� ����
	int c1;
	int c2;
	coordinateToInt(aim, &c1, &c2);
	if ((isOnField(c1, c2) == 1) && ((field[c1][c2] == SHIP_PART) || (field[c1][c2] == EMPTY))) {
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (isOnField(c1 + vertical, c2 + horizontal) == 1)) {
			aim.ch += vertical;
			c1 += vertical;
			aim.num += horizontal;
			c2 += horizontal;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		return x == 2 ? 1 : -1;//1 - success, 2 - ������� �� ��������� ��������
	}
	return 0;
}

int hitTarget(struct Coordinate target, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	struct Coordinate aim;
	aim.num = target.num;
	aim.ch = target.ch - 1;
	int x;//��������� ����
	//����
	x = hitLine(aim, field, shipCount, -1, 0);
	if (x != 0) {
		return x;
	}
	aim.ch = target.ch + 1;
	//���
	x = hitLine(aim, field, shipCount, 1, 0);
	if (x != 0) {
		return x;
	}
	aim.num = target.num - 1;
	aim.ch = target.ch;
	//����
	x = hitLine(aim, field, shipCount, 0, -1);
	if (x != 0) {
		return x;
	}
	aim.num = target.num + 1;
	//�����
	x = hitLine(aim, field, shipCount, 0, 1);
	if (x != 0) {
		return x;
	}
}

int checkDiagonal(int currX, int currY, struct Coordinate* aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int x;
	while (currY < 10) {//������ ���������(����� ������� ����� 4)
		aim->ch = currX + 'A';
		aim->num = currY + 1;
		x = nextTurn(*aim, field, shipCount);
		if (x != -1) {
			printf_s("\n%d%c\n", aim->num, aim->ch);
		}
		if ((x != -1) && (x != 2)) {//���������� �����, ������ ���� ����� ������� ��� �������� ��� ����������� ������
			return x;
		}
		if (currX + 4 < 10) {
			currX += 4;
		}
		else {
			currX--;
			currX -= 4 * (int)(currX / 4);
			currY++;
		}
	}
	return -2;//����� �� �����
}

int search(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[], struct Coordinate* aim) {
	int x;//��������� ����
	x = checkDiagonal(3, 0, aim, field, shipCount);//������ ���������(����� ������� ����� 4)
	if (x != -2) {
		return x;
	}
	x = checkDiagonal(1, 0, aim, field, shipCount); {//������ ��������� (����� �������� ����� 3 � 2)
		if (x != -2) {
			return x;
		}
		for (int i = 0; i < 10; i++) {//����� �������� ����� 1
			for (int j = 0; j < 10; j++) {
				if ((field[i][j] == SHIP_PART) || (field[i][j] == EMPTY)) {
					aim->ch = i + 'A';
					aim->num = j + 1;
					x = nextTurn(*aim, field, shipCount);
					printf_s("\n%d%c\n", aim->num, aim->ch);
					if ((x != -1) && (x != 2)) {//���������� �����, ���� ����� ������� ��� �������� ��� ����������� ������
						return x;
					}

				}
			}
		}
	}
}

struct Coordinate botTurn(enum squareStatus enemyField[FIELD_SIZE][FIELD_SIZE], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int botShipCount[], int playerShipCount[], struct Coordinate currAim) {
	int y = 0;//��������� ��������� �������
	int x;
	x = ((currAim.ch == '0') && (currAim.num == 0)) ? search(enemyField, playerShipCount, &currAim) : 1;//���� ��� ������� ����, �� ������� �

	if (x == 1) {//���� ���� ���������(���� ����)
		y = hitTarget(currAim, enemyField, playerShipCount);//�������� ������ ���� �������
	}
	else {
		currAim.ch = '0';//�������� ������� ����, �.�. ��������� ����
		currAim.num = 0;
	}

	if (y == 1) {//���� ���� ����������
		currAim.ch = '0';//�������� ������� ����
		currAim.num = 0;
		return botTurn(enemyField, field, botShipCount, playerShipCount, currAim);//������ ���� �.�. ��������� 
	}
	printGameSpace(enemyField, field, botShipCount, playerShipCount);
	//���� �� ����������, �� ��������� ������� ���� ��� ���������
	return currAim;
}

struct Coordinate getCoordinate(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {/////////////!!!!
	struct Coordinate c;
	char ch = '0';//������ ����� ���������
	int x = scanf_s("%d%c", &c.num, &c.ch);
	if (c.ch != '\n') {//���� ���� ������� �� ������ �����
		ch = getchar();//�������� ������ ����� �������� ���������
	}
	int c1;
	int c2;
	coordinateToInt(c, &c1, &c2);
	while ((isOnField(c1, c2) == 0) || (ch != '\n') || (x != 2)) {
		printf_s("\n���������� ������� ������ �������� �� ������ ����� �� 1 �� 10 � ����� �� 'A' �� 'J'.\n������� ������ ����������: ");
		//���������� �������� ����� ������
		if ((ch != '\n') && (c.ch != '\n') || (x != 2)) {//���� ���� ������� ���-�� ����� ����� � �����
			x = getchar();
			while (x != '\n') {
				x = getchar();
			}
		}
		//��������� ����� ������
		x = scanf_s("%d%c", &c.num, &c.ch);
		if (c.ch != '\n') {//���� ���� ������� �� ������ �����
			ch = getchar();//�������� ������ ����� �������� ���������
		}
		else {
			ch = '0';//"��������" ������
		}
		coordinateToInt(c, &c1, &c2);
	}
	if ((field[c1][c2] != SHIP_PART) && (field[c1][c2] != EMPTY)) {
		printf_s("\n������ ����� ��� ���������. ������� ����� ����������: ");
		return getCoordinate(field);
	}
	return c;
}

int getType(enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipsCount[]) {
	enum Type currType;
	char ch;
	int x = scanf_s("%d%c", &currType, &ch);
	while ((x != 2) || (currType < 1) || (currType > 4) || (ch != '\n')) {//���� ����� �� �������� ���������� ��� ���� ��� ���� ������� �� ������ �����
		printf_s("������������ �������� ����. ������� ����� �� 1 �� 4: ");
		if ((ch != '\n') || (x != 2)) {//���� ����� ���� ������� �� ������ ���� ����� (�������� ������ �������� ������)
			x = getchar();
			while (x != '\n') {//���������� �� �������� �����
				x = getchar();
			}
		}
		x = scanf_s("%d%c", &currType, &ch);//��������� ����� ������
	}
	if (shipsCount[currType - 1] == 0) {
		printf_s("������� ������� ���� �����������. �������� ������ ���: ");
		return getType(field, shipsCount);
	}
	return currType;
}

void get2Coordinates(struct Coordinate* beginning, struct Coordinate* ending, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int currType) {//��������� ���������� ������ � �����
	printf_s("������� ���������� ������ �������: ");
	*beginning = getCoordinate(field);

	printf_s("������� ���������� ����� �������: ");
	*ending = getCoordinate(field);

	//////���������� ������ ���� �� ����� �����
	if ((beginning -> ch != ending -> ch) && (beginning -> num != ending -> num)) {
		printf_s("���������� ������ � ����� ������� ������ ���� �� ����� �����.\n");
		get2Coordinates(beginning, ending, field, currType);
	}
	/////////beginning<ending
	while ((beginning -> ch > ending -> ch) || (beginning -> num > ending -> num)) {
		printf_s("���������� ������ ������ ���� ������ ��������� ����� �������.\n");
		get2Coordinates(beginning, ending, field, currType);
	}
	//////////�������� ����� ������� 
	while (getShipLen(*beginning, *ending) + 1 != currType) {
		printf_s("����� ������ ��������������� ���� %d, ������� ������ ����������.\n", currType);
		get2Coordinates(beginning, ending, field, currType);
	}
}

void shipsArrangement(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {//����������� �������� �� ���� ������
	int shipsCount[] = { 4, 3, 2, 1 };
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
}

void gameLoop(int playerShipsCount[], int botShipsCount[], enum squareStatus field[FIELD_SIZE][FIELD_SIZE], enum squareStatus botField[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate aim;
	struct Coordinate botAim;
	botAim.ch = '0';//�������� ������� ���� ���� 
	botAim.num = 0;
	while ((getSumm(playerShipsCount, 4) > 0) && (getSumm(botShipsCount, 4) > 0)) {
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
}

void getResult(int playerShipsCount[]) {
	if (getSumm(playerShipsCount, 4) == 0) {
		printf_s("\n�� ���������!");
	}
	else {
		printf_s("\n�� ��������!");
	}
}

void exitGame() {
	printf_s("\n������� ����� ������ ��� ������: ");
	getchar();
}