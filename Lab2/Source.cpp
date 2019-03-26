#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <random>



const int N = 64;
const int measured_times = 1000000;

using word = short;

word A[N];
word B[N];
word Res[N];
int Sum;

void randInitArray(word X[N]) {
    std::random_device rd;
    for (int i = 0; i < N; i++)
        X[i] = rd() % 8;
}

void cArraysAverage() {
    for (int j = 0; j < measured_times; j++) {
        for (int i = 0; i < N; i++) {
            Res[i] = (A[i] + B[i]) / 2;
        }
        Sum += Res[N-1];
    }
}


void mmxArraysAverage() {
    Sum = 0;
    word tmp = 0;
    int repTimes = N;
    int i = 0;
    while (i < measured_times) {
        _asm {
                pusha
                pxor MM7, MM7
                xor esi, esi
            loop1:
                movq MM0, A[esi]
                movq MM1, B[esi]
                pavgw MM0, MM1
                movq Res[esi], MM0
                add esi, 8
                sub repTimes, 4
                jnz loop1

                psrlq MM0, 48
                movq tmp, MM0

                emms

                popa
        }
        ++i;
        repTimes = N;
        Sum += tmp;
    }
}


void asmArraysAverage() {
    word tmp = 0;
    Sum = 0;
    int rep_times = N;
    for (int i = 0; i < measured_times; ++i) {
        _asm {
                pusha

                xor esi, esi
            loop1 :
                xor eax, eax
                xor ebx, ebx
                xor ecx, ecx
                mov ax, word ptr A[esi]
                mov bx, word ptr B[esi]
                add ax, bx
                shr eax, 1
                mov cx, ax

                mov Res[esi], cx
                add esi, 2
                sub rep_times, 1
                jnz loop1

                mov tmp, cx

                popa
        }
        Sum += tmp;
        rep_times = N;
    }
}


void time_measure_wrapper(void(*fun)(), const char *label) {
    LARGE_INTEGER frequency, start, finish;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    fun();
    QueryPerformanceCounter(&finish);
    float delay = (finish.QuadPart - start.QuadPart) * 1000.0f / frequency.QuadPart;
    printf("%s time elapsed : %f ms\n", label, delay);
    printf("Sum: %d\n", Sum);
    
}




int main() {
    randInitArray(A);
    randInitArray(B);
       

   
    time_measure_wrapper(&cArraysAverage, "C function: ");
    time_measure_wrapper(&asmArraysAverage, "ASM function: ");
    time_measure_wrapper(&mmxArraysAverage, "MMX function: ");


    system("pause");
    return 0;
}