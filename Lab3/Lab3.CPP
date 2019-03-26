#include <stdio.h>
#include <stdlib.h>
#include<dos.h>

char color = 0x00;

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();
void print(int, int, int);
void printstr(int, int, int);

// указатели на старые обработчики прерываний на MASTER
void interrupt(*int8) (...);
void interrupt(*int9) (...);
void interrupt(*intA) (...);
void interrupt(*intB) (...);
void interrupt(*intC) (...);
void interrupt(*intD) (...);
void interrupt(*intE) (...);
void interrupt(*intF) (...);


// указатели на старые обработчики прерываний на SLAVE
void interrupt(*intB0) (...);
void interrupt(*intB1) (...);
void interrupt(*intB2) (...);
void interrupt(*intB3) (...);
void interrupt(*intB4) (...);
void interrupt(*intB5) (...);
void interrupt(*intB6) (...);
void interrupt(*intB7) (...);


// новые обработчики прерываний на MASTER
void interrupt  new8(...) { get_reg(); int8(); }
void interrupt  new9(...) { color++; get_reg(); int9(); }		// обработчик клавиатуры
void interrupt  newA(...) { get_reg(); intA(); }
void interrupt  newB(...) { get_reg(); intB(); }
void interrupt  newC(...) { get_reg(); intC(); }
void interrupt  newD(...) { get_reg(); intD(); }
void interrupt  newE(...) { get_reg(); intE(); }
void interrupt  newF(...) { get_reg(); intF(); }

// новыие обработчики прерываний на SLAVE
void interrupt  newB0(...) { get_reg(); intB0(); }
void interrupt  newB1(...) { get_reg(); intB1(); }
void interrupt  newB2(...) { get_reg(); intB2(); }
void interrupt  newB3(...) { get_reg(); intB3(); }
void interrupt  newB4(...) { color++; get_reg(); intB4(); }		// обработчик мыши
void interrupt  newB5(...) { get_reg(); intB5(); }
void interrupt  newB6(...) { get_reg(); intB6(); }
void interrupt  newB7(...) { get_reg(); intB7(); }


void print(int val, int y, int x)
{
	char temp;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);  // получаем адрес 0 символа 
	screen += (y * 80 + x);								// двигаемся по консоли

	int i;
	for (i = 7; i >= 0; i--) {
		temp = val & 1;
		val /= 2;
		screen->symb = temp + '0';						// заносим символ в ячейку
		color &= 15;
		screen->attr = color;							// заносим цвет в ячейку
		screen++;										// переходим на след символ
	}
}

void printstr(int num, int y, int x)
{
	char str1[] = { 'M', 'A', 'S', 'T', 'E', 'R', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	char str2[] = { 'I', 'S', 'R', '\0' };
	char str3[] = { 'I', 'M', 'R', '\0' };
	char str4[] = { 'S', 'L', 'A', 'V', 'E', ' ', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	int i;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);	// получаем адрес 0 символа
	screen += (y * 80 + x);								// двигаемся по консоли
	switch (num)
	{
	case 1:
	{
		for (i = 0; str1[i] != 0; i++)
		{
			screen->symb = str1[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 2:
	{
		for (i = 0; str2[i] != 0; i++)
		{
			screen->symb = str2[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 3:
	{
		for (i = 0; str3[i] != 0; i++)
		{
			screen->symb = str3[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 4:
	{
		for (i = 0; str4[i] != 0; i++)
		{
			screen->symb = str4[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	}
}

void get_reg()
{
	//получаем данные для регистра запросов ВЕДУЩЕГО
	printstr(1, 13, 25);
	outp(0x20, 0x0A);							// помещаем в порт 0x20 значение регистра запросов
	print(inp(0x20), 14, 34);					// считываем из порта значение регистра запросов

	//получаем данные для регистра обслуживания ВЕДУЩЕГО
	printstr(2, 15, 34);
	outp(0x20, 0x0B);							// помещаем в порт 0x20 значение регистра обслуживания
	print(inp(0x20), 16, 34);					// считываем из порта значение регистра обслуживания

	//регистр масок ВЕДУЩЕГО
	printstr(3, 17, 34);						
	print(inp(0x21), 18, 34);					// считываем из порта 0x21 значение регистра масок

	//все тоже для ведомого
	printstr(4, 19, 25);
	outp(0xA0, 0x0A);
	print(inp(0xA0), 20, 34);

	printstr(2, 21, 34);
	outp(0xA0, 0x0B);
	print(inp(0xA0), 22, 34);
	
	printstr(3, 23, 34);
	print(inp(0xA1), 24, 34);
}

void init() {

	// получаем вектора прерываний MASTER
	int8 = getvect(0x08); //таймер
	int9 = getvect(0x09); //клавиатура
	intA = getvect(0x0A);
	intB = getvect(0x0B);
	intC = getvect(0x0C);
	intD = getvect(0x0D);
	intE = getvect(0x0E);
	intF = getvect(0x0F);

	// получаем вектора прерываний SLAVE
	intB0 = getvect(0x70);
	intB1 = getvect(0x71);
	intB2 = getvect(0x72);
	intB3 = getvect(0x73);
	intB4 = getvect(0x74);
	intB5 = getvect(0x75);
	intB6 = getvect(0x76); 
	intB7 = getvect(0x77);

	// устанавливаем вектора прерываний MASTER
	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	// устанавливаем вектора прерываний SLAVE
	setvect(0xB0, newB0);
	setvect(0xB1, newB1);
	setvect(0xB2, newB2);
	setvect(0xB3, newB3);
	setvect(0xB4, newB4);
	setvect(0xB5, newB5);
	setvect(0xB6, newB6);
	setvect(0xB7, newB7);

	_disable();				// запрещаем прерывания

	// master
	outp(0x20, 0x11);		// ICW1 
	outp(0x21, 0x08);		// ICW2 
	outp(0x21, 0x04);		// ICW3 
	outp(0x21, 0x01);		// ICW4 

	
	// slave
	outp(0xA0, 0x11);		// ICW1
	outp(0xA1, 0xB0);		// ICW2
	outp(0xA1, 0x02);		// ICW3
	outp(0xA1, 0x01);		// ICW4

	_enable();				// разрешаем прерывания

}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG(fp) = _psp;		// получаем сегмент
	FP_OFF(fp) = 0x2c;		// и смещение сегмента данных

	_dos_freemem(*fp);		// освобождаем память 

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);	// оставляем резидентной
	return 0;
}