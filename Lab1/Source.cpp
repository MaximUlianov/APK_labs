#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<math.h>
#include<conio.h>
#include<time.h>

#include"Foo.h"


void calculate() {
    float interval_beg;
    float interval_end;
    float gap;
    float result = 0;
    int num_1 = 1;
    int num_4 = 4;

    clock_t beg, end, total_beg;

    printf("Enter beg:\n");
    scanf("%f", &interval_beg);
    printf("Enter end:\n");
    scanf("%f", &interval_end);
    printf("Enter gap:\n");
    scanf("%f", &gap);

    float num = interval_beg;

    beg = total_beg = clock();

    for (num ; num <= interval_end; num += gap) {
        result += ((cos(num) + 1) / (sin(num) + 4));
    }

    end = clock();

    printf("C - result: %f\n", result);
    printf("C - runtime: %f\n", (double)(end - beg) / CLK_TCK);

    beg = clock();
    result = 0;
    num = interval_beg;
   
        _asm {
                finit
        loop_start :
                fld num

                fcomp interval_end
                fstsw ax
                and ah, 01000101b
                jz loop_end

                fld num
                fcos
                fiadd num_1

                fld num
                fsin
                fiadd num_4


                fdivp st(1), st

                fld result
                faddp st(1), st
                fstp result

                 fld num
                 fld gap
                 faddp st(1),st
                 fstp num
                 jmp loop_start
        loop_end:
                 fwait
        }
  
    end = clock();
    printf("ASM - result: %f\n",result);
    printf("ASM - runtime: %f\n", (double)(end - beg) / CLK_TCK);
    printf("Total runtime: %f\n", (double)(end - total_beg) / CLK_TCK);
}