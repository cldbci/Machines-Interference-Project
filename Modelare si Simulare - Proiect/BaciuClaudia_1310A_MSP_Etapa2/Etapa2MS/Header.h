#pragma once
#include<iostream>
#include<cmath>
using namespace std;
#define lim_max 1000000  
//definirea parametrilor variabilelor aleatoare
#define lamA 0.2105
#define lamB 0.1626
#define miuA 3.8533
#define miuB 3.4218
#define pA lamA/(lamA+lamB)
#define pB lamB/(lamA+lamB)
#define D_t 100/(1+lamA/miuA+lamB/miuB)//val teoretica a gradului de disponibilitate pt un singur sistem
//definitii functii de generare valori de selectie
double genExp(double lambda_1);
double genGauss(double m, double sigma);
double genTR();//generator timp remediere simulare 1
double genTR2();//generator timp remediere simulare 2

