#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#define SIZE 4
#define KEY getchar()

/*!
* ---------------------- Прототипы ----------------------------
*/
bool printEndScreen(int matrix[SIZE][SIZE], char name[9], int score, int max);
bool printScreen(int matrix[SIZE][SIZE], int, char name[9]);
bool ElementsUp(int matrix[SIZE][SIZE], int *score);
bool ElementsRight(int matrix[SIZE][SIZE], int);
bool addRandomElement(int matrix[SIZE][SIZE]);
bool verificationEndGame(int matrix[SIZE][SIZE]);
char * gameNumber(int, char numх[5]);
int maxNumber(int matrix[SIZE][SIZE]);
int gen_random(int, int);

/*!
* ------------------- Тело основной функции -------------------
*/
int main() {

Start:

	bool status = true;				// Состояние работы
	bool result;						//Результат выполнения действия
	bool no_char;						//Флаг вводимого значения
	char name[9];				//Строка с именем игрока
	char ch[2];							//Буффер вводимого знака
	char c;								//Буффер зажатой клавиши
	int max;							//Максимальное значение
	int score = 0;						//Очки игрока

	int matrix[SIZE][SIZE] = {			//Игровое поле
		{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 }
	};

	system("cls");										//Очищение окна консоли
	srand((unsigned int)time(NULL));					//Рандом по времени

														//Получение имени игрока
	printf("Enter your name: ");
	scanf("%5s", &name);

	//Подготовка запуска игры
	addRandomElement(matrix);
	addRandomElement(matrix);
	addRandomElement(matrix);
	printScreen(matrix, score, name);

	//Геймплей
	do {
		//Сброс результатов после одного цикла
		result = false;
		no_char = false;

		//Получение зажатой клавиши
		c = _getch();

		//Определение действия по вводимым данным
		switch (c) {

			//При сдвиге вверх
		case 'W': case 'w':
			result = ElementsUp(matrix, &score);
			break;

			//При сдвиге вниз
		case 'S': case 's':
			ElementsRight(matrix, 2);
			result = ElementsUp(matrix, &score);
			ElementsRight(matrix, 2);
			break;

			//При сдвиге влево
		case 'A': case 'a':
			ElementsRight(matrix, 1);
			result = ElementsUp(matrix, &score);
			ElementsRight(matrix, 3);
			break;

			//При сдвиге вправо
		case 'D': case 'd':
			ElementsRight(matrix, 3);
			result = ElementsUp(matrix, &score);
			ElementsRight(matrix, 1);
			break;

			//При нажатии выхода
		case 'B': case 'b':
			result = false;
			status = false;
			break;

			//Если не совершенно никакое действие
		default:
			no_char = true;
		}

		//Проверка конца игры
		max = maxNumber(matrix);												//Получение максимального значения
		if (max == 11)
			status = false;												//При достижении максимального значения
		else if (result && !no_char) {										//При удачном завершении действия и если действие вообще совершено
			addRandomElement(matrix);										//Добавляем после каждого действия доп элемент на поле
			printScreen(matrix, score, name);						//Обновить экран после каждого действия
		}
		else if (!result) if (!verificationEndGame(matrix))				//Проверка конца игры
			status = false;

	} while (status);

	//Конец игры
	printEndScreen(matrix, name, score, max);
	scanf("%1s", &ch);
	if (ch[0] == 'Y' || ch[0] == 'y') goto Start;

	//TODO
	KEY;
	return 0;
}

/*!
* -------------------- Доп. функции ----------------------------
*/

/*! Добавление на поле рандомный элемент в рандомную пустую ячейку
* \param[out] matrix - игровое поле
* \return успешность добавления
*/
bool addRandomElement(int matrix[SIZE][SIZE]) {

	bool success = false;					//Успешность завершения действия
	bool status = false;				//Процесс добавления
	int X, Y;							//Координаты добавляемого элемента

										//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	//Проверка возможности добавить элемент
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (matrix[i][j] == 0) status = true;

	//Добавление рандомного элемента
	while (status && !success) {					//Пока не будет добавлен элемент
														//Генерация рандомных координат
		X = gen_random(0, 3);
		Y = gen_random(0, 3);
		//Проверка что в данных координатах точно есть свободное окно
		if (matrix[Y][X] == 0) {
			matrix[Y][X] = gen_random(1, 2);				//Заполнение свободной ячейки
			success = true;
		}
	}

	return success;
}

/*! Генерация рандомного числа в диапазоне
* \param[in] range_min - минимальное число
* \param[in] range_max - максимальное число
* \return рандомное число из диапазона
*/
int gen_random(int range_min, int range_max) {

	return range_min + rand() % (range_max - range_min + 1);

}

/*! Выполнить игровой сдвиг вверх
* \param[out] score - очки игрока
* \param[out] matrix - матрица игрового поля
* \return успешность операции
*/
bool ElementsUp(int matrix[SIZE][SIZE], int *score) {

	bool success = false;															//Успешность изменений

																					//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	// -------- Сдвиг вверх перед склеиванием -------- 
	//Алгоритм сдвига вверх к стене
	for (int i = 0; i < SIZE; i++)														//Перебор ячеек
		for (int j = 0; j < SIZE - 1; j++)												//Игрового поля

			if (matrix[j][i] == 0)												//Если пустая ячейка найдена
				for (int k = j + 1; k < SIZE; k++) if (matrix[k][i] != 0)			//Поиск по столбцу не пустую ячейку 
				{	//Сдвиг вверх найденной заполненной ячейки
					matrix[j][i] = matrix[k][i];
					matrix[k][i] = 0;
					success = true;
					break;
				}
	// -------- Склеивание -------- 
	//Алгрримт склеивание схожих элементов
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE - 1; j++)
			//Склейка схожего элемента
			if (matrix[j][i] == matrix[j + 1][i] && matrix[j][i] != NULL) {				//Если на ячейку ниже находится такой же элемент и если этот элемент не 0.
				*score += matrix[j][i] * 10;												//Начисление очков
				matrix[j][i]++;
				matrix[j + 1][i] = 0;
				success = true;
			}

	// -------- Повторный сдвиг, после склеивания -------- 
	//Алгоритм сдвига вверх к стене
	for (int i = 0; i < SIZE; i++)														//Перебор ячеек
		for (int j = 0; j < SIZE - 1; j++)												//Игрового поля

			if (matrix[j][i] == 0)														//Если пустая ячейка найдена
				for (int k = j + 1; k < SIZE; k++) if (matrix[k][i] != 0)					//Поиск по столбцу не пустую ячейку 
				{	//Сдвиг вверх найденной заполненной ячейки
					matrix[j][i] = matrix[k][i];
					matrix[k][i] = 0;
					success = true;
					break;
				}

	return success;

}

/*! Поворот матрици n раз
* \param[in] count - кол-во поворотов на 90 градусов.
* \param[out] matrix - матрица игрового поля
* \return успешность операции
*/
bool ElementsRight(int matrix[SIZE][SIZE], int count) {

	bool success = false;											//Успешность переворота
	int buffer;													//Буфер значения

																//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	// -------- Поворот матрицы на COUNT раз, по часовой стрелке --------
	for (int k = 0; k < count; k++)								//Счетчик поворотов
																//Алгоритм поворота матрици по часовой стрелке на 90 градусов
		for (int i = 0; i<SIZE / 2 && matrix != NULL; i++)		//Перебор элементов...
			for (int j = i; j<SIZE - 1 - i; j++)					//...Матрици
			{
				buffer = matrix[i][j];
				matrix[i][j] = matrix[SIZE - j - 1][i];
				matrix[SIZE - j - 1][i] = matrix[SIZE - i - 1][SIZE - j - 1];
				matrix[SIZE - i - 1][SIZE - j - 1] = matrix[j][SIZE - i - 1];
				matrix[j][SIZE - i - 1] = buffer;
				success = true;
			}

	return success;

}

/*! Поиск максимального значения игрового поля
* \param[in] matrix - матрица игрового поля
* \return максимальное значение игрового поля
*/
int maxNumber(int matrix[SIZE][SIZE]) {

	//Завершение с ошибкой если игровое поле не существует.
	if (!matrix) return false;

	int maxNumber = 0;

	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (maxNumber < matrix[i][j]) maxNumber = matrix[i][j];

	return maxNumber;
}

/*! Отпечатать экран игры
* \param[in] matrix - матрица игрового поля
* \param[in] score - очки игрока
* \param[in] name - имя игрока
* \return успешность отрисовки
*/
bool printScreen(int matrix[SIZE][SIZE], int score, char name[9]) {

	char num[16][5];													//Переменная для отображения числа ячейки
	system("cls");														//Очищение окна консоли

																		//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	// ---------- Отрисовка экрана ----------
	//Шапка информации
	printf("_____________________________________\n");
	printf("|             GAME 2048             |\n");
	printf("|    Name: %5s   Score: %7d   |\n", name, score);
	printf("|___________________________________|\n");
	printf("|        |        |        |        |\n");
	//1-я строка игрового поля
	printf("|  %4s  |  %4s  |  %4s  |  %4s  |\n",
		gameNumber(matrix[0][0], num[0]), gameNumber(matrix[0][1], num[1]),
		gameNumber(matrix[0][2], num[2]), gameNumber(matrix[0][3], num[3]));
	//2-я строка игрового поля
	printf("|        |        |        |        |\n");
	printf("|________|________|________|________|\n");
	printf("|        |        |        |        |\n");
	printf("|  %4s  |  %4s  |  %4s  |  %4s  |\n",
		gameNumber(matrix[1][0], num[4]), gameNumber(matrix[1][1], num[5]),
		gameNumber(matrix[1][2], num[6]), gameNumber(matrix[1][3], num[7]));
	//3-я строка игрового поля
	printf("|        |        |        |        |\n");
	printf("|________|________|________|________|\n");
	printf("|        |        |        |        |\n");
	printf("|  %4s  |  %4s  |  %4s  |  %4s  |\n",
		gameNumber(matrix[2][0], num[8]), gameNumber(matrix[2][1], num[9]),
		gameNumber(matrix[2][2], num[10]), gameNumber(matrix[2][3], num[11]));
	//4-я строка игрового поля
	printf("|        |        |        |        |\n");
	printf("|________|________|________|________|\n");
	printf("|        |        |        |        |\n");
	printf("|  %4s  |  %4s  |  %4s  |  %4s  |\n",
		gameNumber(matrix[3][0], num[12]), gameNumber(matrix[3][1], num[13]),
		gameNumber(matrix[3][2], num[14]), gameNumber(matrix[3][3], num[15]));
	//Информативный блок
	printf("|        |        |        |        |\n");
	printf("|________|________|________|________|\n");
	printf("#===================================#\n");
	printf("#    W,A,S,D - Move; B - Exit       #\n");
	printf("#___________________________________#\n");

	return true;

}

/*! Выводит строку num для игрового окна
* \param[in] n - номер
* \param[out] num - строка с номером
* \return указатель на num
*/
char * gameNumber(int n, char num[5]) {

	//Перевод в строку
	if (n == 0) {							//При 0 степени
		num[0] = ' ';
		num[1] = '\0';
	}
	else {
		int buff = (int)pow(2.0, n);			//Получение 2 возведенного в степень числа n
		_itoa(buff, num, 10);				//Перевод в с	троку
	}

	return num;
}

/*! Проверка конца игры
* \param[out] matrix - игровое поле
* \return успешность проверки конца игры, при отсутсвии конца - true, при наличии - false
*/
bool verificationEndGame(int matrix[SIZE][SIZE]) {

	bool success = false;															//Успешность проверки

																					//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	//Проверка концца
	for (int k = 0; k < 4; k++) {														//Проверка каждой стороны

																						//Алгоритм проверки конца, проверка смещений
		for (int i = 0; i < SIZE && !success; i++)									//Перебор ячеек
			for (int j = 0; j < SIZE - 1 && !success; j++)							//Игрового поля

				if (matrix[j][i] == 0)												//Если пустая ячейка найдена
					for (int k = j + 1; k < SIZE; k++) if (matrix[k][i] != 0)			//Поиск по столбцу не пустую ячейку 
					{	//Сдвиг вверх найденной заполненной ячейки
						success = true;
						break;
					}

		//Алгоритм проверки конца, проверка склеивания
		for (int i = 0; i < SIZE && k < 3 && !success; i++)
			for (int j = 0; j < SIZE - 1 && !success; j++)
				//Склейка схожего элемента
				if (matrix[j][i] == matrix[j + 1][i] && matrix[j][i] != NULL) success = true;

		//Поворот матрици
		ElementsRight(matrix, 1);
	}

	return success;
}

/*! Отпечатать экран завершения игры
* \param[in] matrix - матрица игрового поля
* \param[in] score - очки игрока
* \param[in] name - имя игрока
* \param[in] max - максимальное значение
* \return успешность отрисовки
*/
bool printEndScreen(int matrix[SIZE][SIZE], char name[9], int score, int max) {

	system("cls");														//Очищение окна консоли

																		//Завершение с ошибкой если игровое поле не существует
	if (!matrix) return false;

	// ------------------- Отрисовка экрана ---------------
	printf("_____________________________________\n");
	printf("|             GAME 2048             |\n");
	printf("|    Name: %5s   Score: %7d   |\n", name, score);
	printf("|___________________________________|\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|            %9s              |\n", (max == 11) ? "YOU  WIN!" : "GAME OVER");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|      You want play new game?      |\n");
	printf("|                                   |\n");
	printf("|            Press Y\\N.             |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|                                   |\n");
	printf("|___________________________________|\n");
	printf("#===================================#\n");
	printf("#                                   #\n");
	printf("#___________________________________#\n");

	return true;

}
