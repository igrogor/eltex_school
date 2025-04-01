#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>
#include <stdlib.h>

struct Func
{
   char *name_func;
   int (*function)(int a, int b);
} typedef Func;

void menu();

int addition(int arg1, int arg2);

int subtraction(int arg1, int arg2);

int multiplication(int arg1, int arg2);

int division(int arg1, int arg2);
#endif