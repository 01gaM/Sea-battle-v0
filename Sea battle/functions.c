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

int checkNeighbours(enum squareStatus map[FIELD_SIZE][FIELD_SIZE], int currI, int currJ) {	
	for (int i = currI-1; i <= currI+1; i++) {
		for (int j = currJ-1; j <= currJ+1; j++) {
			if ((i >= 0) && (i <= 9) && (j >= 0) && (j <= 9) && (map[i][j] != EMPTY)) {
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
	if (s1 > s2) {
		return s1;
	}
	return s2;

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

int checkCoordinate(int c1, int c2,  int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int checkCoordinate1 = c1 + vertical;
	int checkCoordinate2 = c2 + horizontal;
	while ((checkCoordinate1 >= 0) && (checkCoordinate1 <= 9) && (checkCoordinate2 >= 0) && (checkCoordinate2 <= 9) && (field[checkCoordinate1][checkCoordinate2] != EMPTY) && (field[checkCoordinate1][checkCoordinate2] != CHECKED)) {//���� �� ����� �� ����� ���� ��� �� ����� �������� �������
		if (field[checkCoordinate1][checkCoordinate2] == SHIP_PART) {
			return 0;//������� ������������� ����� �������
		}
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
	return -1;//�� ������� ������������� ����� �������
}

int isKilled(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;
	//��������� ����, ���, ���� � ����� �� ������� ������������� ������ �������
	if ((checkCoordinate(c1, c2, -1, 0, field) == 0) || (checkCoordinate(c1, c2, 1, 0, field) == 0) || (checkCoordinate(c1, c2, 0, -1, field) == 0) || (checkCoordinate(c1, c2, 0, 1, field) == 0)) {
		return 0;
	}
	return 1;//������� ����
}

int destroyLine(int c1, int c2, int vertical, int horizontal, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int isHorizontal, int* shipLen) {
	int checkCoordinate1 = c1 + vertical;
	int checkCoordinate2 = c2 + horizontal;
	while ((checkCoordinate1 >= 0) && (checkCoordinate1 <= 9) && (checkCoordinate2 >= 0) && (checkCoordinate2 <= 9) && (field[checkCoordinate1][checkCoordinate2] != EMPTY) && (field[checkCoordinate1][checkCoordinate2] != CHECKED)) {//���� �� ����� �� ����� ���� ��� �� ����� �������� �������
		if (field[checkCoordinate1][checkCoordinate2] == DAMAGED) {
			field[checkCoordinate1][checkCoordinate2] = DESTROYED;
			if (isHorizontal == 0) {//��� �� ���������
				if (checkCoordinate2 - 1 >= 0) {
					field[checkCoordinate1][checkCoordinate2 - 1] = CHECKED;
				}
				if (checkCoordinate2 + 1 <= 9) {
					field[checkCoordinate1][checkCoordinate2 + 1] = CHECKED;
				}

				if ((checkCoordinate1 + vertical >= 0) && ((field[checkCoordinate1 + vertical][checkCoordinate2] == EMPTY) || (field[checkCoordinate1 + vertical][checkCoordinate2] == CHECKED))) {
					field[checkCoordinate1 + vertical][checkCoordinate2] = CHECKED;
					if (checkCoordinate2 - 1 >= 0) {
						field[checkCoordinate1 + vertical][checkCoordinate2 - 1] = CHECKED;
					}
					if (checkCoordinate2 + 1 <= 9) {
						field[checkCoordinate1 + vertical][checkCoordinate2 + 1] = CHECKED;
					}
				}
			}
			else {//��� �� �����������
				if (checkCoordinate1 - 1 >= 0) {
					field[checkCoordinate1 - 1][checkCoordinate2] = CHECKED;
				}
				if (checkCoordinate1 + 1 <= 9) {
					field[checkCoordinate1 + 1][checkCoordinate2] = CHECKED;
				}
				if ((checkCoordinate2 + horizontal >= 0) && ((field[checkCoordinate1][checkCoordinate2 + horizontal] == EMPTY) || (field[checkCoordinate1][checkCoordinate2 + horizontal] == CHECKED))) {
					field[checkCoordinate1][checkCoordinate2 + horizontal] = CHECKED;
					if (checkCoordinate1 - 1 >= 0) {
						field[checkCoordinate1 - 1][checkCoordinate2 + horizontal] = CHECKED;
					}
					if (checkCoordinate1 + 1 <= 9) {
						field[checkCoordinate1 + 1][checkCoordinate2 + horizontal] = CHECKED;
					}
				}
			}
			*shipLen = *shipLen + 1;
		}
		checkCoordinate1 += vertical;
		checkCoordinate2 += horizontal;
	}
}

void destroyShip(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;
	int checkCoordinate1;
	int checkCoordinate2;
	int isHorizontal;
	int shipLen = 0;//����� ������� - 1
	field[c1][c2] = DESTROYED;
	if (((field[c1 - 1][c2] == DAMAGED) && (c1 - 1 >= 0)) || ((field[c1 + 1][c2] == DAMAGED) && (c1 + 1 <= 9))) {
		isHorizontal = 0;
	}
	else {
		isHorizontal = 1;
	}
	if (isHorizontal == 0) {
		//��������� ����
		destroyLine(c1, c2, -1, 0, field, isHorizontal, &shipLen);
		//��������� ���
		destroyLine(c1, c2, 1, 0, field, isHorizontal, &shipLen);
	}
	else {
		//��������� ����
		destroyLine(c1, c2, 0, -1, field, isHorizontal, &shipLen);
		//��������� �����
		destroyLine(c1, c2, 0, 1, field, isHorizontal, &shipLen);
	}
	shipCount[shipLen]--;//������� ������� �� ������ ����������
	//�������� ������� � ������ ������� ������ ��� ������
	if (isHorizontal == 1) {
		if (c1 - 1 >= 0) {
			field[c1 - 1][c2] = CHECKED;
		}
		if (c1 + 1 <= 9) {
			field[c1 + 1][c2] = CHECKED;
		}
		if ((c2 - 1 >= 0) && (field[c1][c2 - 1] != DESTROYED)) {
			field[c1][c2 - 1] = CHECKED;
			if (c1 - 1 >= 0) {
				field[c1 - 1][c2 - 1] = CHECKED;
			}
			if (c1 + 1 <= 9) {
				field[c1 + 1][c2 - 1] = CHECKED;
			}
		}
		if ((c2 + 1 <= 9) && (field[c1][c2 + 1] != DESTROYED)) {
			field[c1][c2 + 1] = CHECKED;
			if (c1 - 1 >= 0) {
				field[c1 - 1][c2 + 1] = CHECKED;
			}
			if (c1 + 1 <= 9) {
				field[c1 + 1][c2 + 1] = CHECKED;
			}
		}
	}
	else {
		if (c2 - 1 >= 0) {
			field[c1][c2 - 1] = CHECKED;
		}
		if (c2 + 1 <= 9) {
			field[c1][c2 + 1] = CHECKED;
		}
		if ((c1 - 1 >= 0) && (field[c1 - 1][c2] != DESTROYED)) {
			field[c1 - 1][c2] = CHECKED;
			if (c2 - 1 >= 0) {
				field[c1 - 1][c2 - 1] = CHECKED;
			}
			if (c2 + 1 <= 9) {
				field[c1 - 1][c2 + 1] = CHECKED;
			}
		}
		if ((c1 + 1 <= 9) && (field[c1 + 1][c2] != DESTROYED)) {
			field[c1 + 1][c2] = CHECKED;
			if (c2 - 1 >= 0) {
				field[c1 + 1][c2 - 1] = CHECKED;
			}
			if (c2 + 1 <= 9) {
				field[c1 + 1][c2 + 1] = CHECKED;
			}
		}
	}
}

int nextTurn(struct Coordinate aim, enum squareStatus field[FIELD_SIZE][FIELD_SIZE], int shipCount[]) {
	///���� � �������� ������� �� ��� ����� ������� ����������, �� �������� ������ �� "x"
	//���� � �������� ������� ��� ����� ������� ����������(��� �� ���� 1), �� �������� ��� ������ ������� �� "*"
	int c1 = aim.ch - 'A';
	int c2 = aim.num - 1;

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
		///�������� ��� ������ ������� �� *
		destroyShip(aim, field, shipCount);
	}
	if (getSumm(shipCount, 4) == 0) {
		return 3;//game over
	}
	return 2;//������� '����'
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
	if ((aim.ch - 'A' >= 0) && (aim.ch - 'A' <= 9) && (aim.num > 0) && (aim.num <= 10) && ((field[aim.ch - 'A'][aim.num - 1] == SHIP_PART) || (field[aim.ch - 'A'][aim.num - 1] == EMPTY))) {
		x = nextTurn(aim, field, shipCount);
		printf_s("\n%d%c\n", aim.num, aim.ch);
		while ((x == 1) && (aim.ch - 'A' + vertical >= 0) && (aim.ch - 'A' + vertical <= 9) && (aim.num + horizontal > 0) && (aim.num + horizontal <= 10)) {
			aim.ch += vertical;
			aim.num += horizontal;
			x = nextTurn(aim, field, shipCount);
			printf_s("\n%d%c\n", aim.num, aim.ch);
		}
		if (x == 2) {
			return 1;//success
		}
		return -1;//������� �� ��������� ��������
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
	if ((currAim.ch == '0') && (currAim.num == 0)) {//���� ��� ������� ����
		x = search(enemyField, playerShipCount, &currAim);//������� �
	}
	else {
		x = 1;
	}

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

struct Coordinate getCoordinate(enum squareStatus field[FIELD_SIZE][FIELD_SIZE]) {
	struct Coordinate c;
	char ch = '0';//������ ����� ���������
	int x = scanf_s("%d%c", &c.num, &c.ch);
	if (c.ch != '\n') {//���� ���� ������� �� ������ �����
		ch = getchar();//�������� ������ ����� �������� ���������
	}
	while ((c.ch < 'A') || (c.ch > 'J') || (c.num < 1) || (c.num > 10) || (ch != '\n') || (x != 2)) {
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
	}
	if ((field[c.ch - 'A'][c.num - 1] != SHIP_PART) && (field[c.ch - 'A'][c.num - 1] != EMPTY)) {
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