//27.	F(x) = (cos(x) + 1) / (sin(x) + 4)
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<conio.h>

#include"Foo.h"


int main(){
    char a;
    do {

        system("CLS");
        calculate();
        printf("Do you want to continue? (default - yes / exit - n)\n");
        rewind(stdin);

    }while ((a = getchar()) != 'n');
    system("pause");
    return 0;
}