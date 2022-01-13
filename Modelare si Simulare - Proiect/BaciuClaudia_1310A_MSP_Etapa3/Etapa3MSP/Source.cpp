#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

#define lambdaA 0.2105
#define lambdaB 0.1626
#define miuA 3.8533
#define miuB 3.4218

//#define alfa 1 //alfa=0 sau alfa=0.5

#define pA lambdaA/(lambdaA + lambdaB)
#define pB 1-pA

//#define MFR B

#define mA 1/miuA
#define mB 1/miuB

#define sigmaA 1/(3.5*miuA)
#define sigmaB 1/(3.5*miuB)

enum Module { A, B };
enum Stare { O, F };


double genExp(double lambda)
{
	double u, x;
	u = (double)rand() / (RAND_MAX + 1);
	x = -1 / lambda * log(1 - u);
	return x;
}

double genTr(Module m)
{
	double Tr;
	if (m == A)
	{
		Tr = genExp(miuA);
	}
	else
	{
		Tr = genExp(miuB);
	}

	return Tr;
}

void Simulare(int& S, double& D, double& Oc)
{
	double NS = 1e+7; //numarul de simulari
	double DS;
	double STr = 0; //suma timpilor de remediere
	double STf = 0; //suma timpilor de functionare
	double ceas = 0; //ceasul simularii
	int Nd = 0; //numarul sistemelor decfectate pe parcursul simularii
	int Nr = 0; //numarul remedierilor efectuate
	int nf = S; //numarul de sisteme in functiune
	int nmd = 0; // numarul modulelor defecte, indiferent de tipul lor si de sistemele de care apartin
	int nmf[2][100];

	double Tf[100]; //pentru un sistem in functiune, timpul pana la aparitia primei intreruperi accidentale
	Stare St[100]; //starea sistemului i

	double Tpd; //timpul pana la prima intrerupere accidentala(defectare) la sistemele aflate in functiune
	int sd;//sistemul la care va aparea prima defectare
	int sr; //sistemul la care se face remedierea;

	Module md; //modulul afectat de aceasta intrerpere accidentala
	Module mr; //modulul de remediat in curs

	double Tr = 0; //timpul pana la terminarea remedierii in curs
	int No = 0; //numarul de sisteme oprite

	for (int i = 1; i <= S; i++)
	{
		//nmf[A][i] = 1;///modulul A este fara modul de rezerva(pentru verificare etapa II)
		nmf[A][i] = 2;//modulul A este prevazut cu modul de rezerva
		nmf[B][i] = 1;
	}
	for (int i = 1; i <= S; i++)
	{
		St[i] = F;
		Tf[i] = genExp(lambdaB + lambdaA );
	}

	double min = 1e+7;
	int ind;

	for (int i = 1; i <= S; i++)
	{
		if (St[i] == F && Tf[i] < min)
		{
			min = Tf[i];
			ind = i;
		}
	}
	Tpd = min;
	sd = ind;

	//determina modulul afectat de intrerupere
	double u = (double)rand() / RAND_MAX;
	if (u < pA)
	{
		md = A;
	}
	else
	{
		if (u < (pA + pB))
			md = B;
	}
	do {
		//Determinarea evenimentului urmator
		if (nmd == 0 || ((nf > 0) && Tpd < Tr))
		{
			//defectare
			Nd++;
			ceas += Tpd;
			if (nmd > 0)
			{
				Tr -= Tpd;
			}
			for (int i = 1; i <= S; i++)
			{
				if (St[i] == F)
				{
					Tf[i] -= Tpd;
				}
			}
			STf += nf * Tpd;
			nmf[md][sd]--;
			nmd++;
			if (nmf[md][sd] == 0) //oprire sistem
			{
				St[sd] = O;
				nf--;
				No++;
			}
			else
			{
				if (nmf[md][sd] > 0)
					Tf[sd] = genExp(lambdaB + lambdaA );
				else
				{
					St[sd] = O;
					--nf;
					++No;
				}
			}
			if (nmd == 1)
			{
				sr = sd;
				mr = md;
				Tr = genTr(mr);
				STr += Tr;
			}
			/*
			if ((nmd == 1) || (St[sr] == F && St[sd] == O) || (sd == sr && md == MFR))
			{
				sr = sd;
				mr = md;

				if (nmd > 1)
				{
					STr -= Tr;
				}

				Tr = genTr(mr);
				STr += Tr;
			}*/

		}
		else
		{
			//remediere
			Nr++;
			ceas += Tr;

			for (int i = 1; i <= S; i++)
			{
				if (St[i] == F)
				{
					Tf[i] -= Tr;
				}
			}
			STf += nf * Tr;
			nmf[mr][sr]++;
			nmd--;
			if (nmf[A][sr] >= 1 && nmf[B][sr] >= 1)
			{
				if (St[sr] == O)
				{
					St[sr] = F;
					nf++;
				}
				Tf[sr] = genExp(lambdaB + lambdaA );
			}
			if (nmd > 0)
			{
				//inceputul unei noi remedieri
				//Actualizare sr, mr
				bool gasit = false;

				for (int i = 1; i <= S; i++)
				{
					if (nmf[B][i] == 0) //starea S2 sau S3(se remediaza B)
					{
						mr = B;
						sr = i;
						gasit = true;
						break;
					}
				}
				if (gasit == false) //nu s a gasit modulul remediat
				{
					for (int i = 1; i <= S; i++)
					{
						if (nmf[A][i] == 0) //starea 4 (ambele module A defecte)
						{
							mr = A;
							sr = i;
							gasit = true;
							break;
						}
					}
				}
				if (gasit == false)
				{
					for (int i = 1; i <= S; i++)
					{
						if (nmf[A][i] == 1 && nmf[B][i] == 1) //starea 1
						{
							mr = B;
							sr = i;
							gasit = true;
						}
					}
				}
				Tr = genTr(mr);
				STr += Tr;
			}

		}

		if (nf > 0)
		{ //Actualizare Tpd,sd
			min = 1e+6;
			for (int i = 1; i <= S; i++)
			{
				if (Tf[i] < min && St[i] == F)
				{
					min = Tf[i];
					ind = i;
				}
			}
			Tpd = min;
			sd = ind;
			//Actualizare md
			double u = (double)rand() / RAND_MAX;

			if (u < pA)
			{
				md = A;
			}
			else
			{
				if (u < (pA + pB))
					md = B;
			}
		}

	} while (Nd < NS);
	DS = ceas;

	//calcul statistici
	D = (STf / (DS * S)) * 100;
	Oc = (STr / DS) * 100;

	cout << "D = " << D << endl;
	cout << "O = " << Oc << endl;

	//Verificarea programului de simulare (Modulul B cu rezerva)
	cout << "Nd = " << Nd << endl;
	cout << "DS * S * (D/100) (lambdaA + lambdaB) = " << DS * S * (D / 100) * (lambdaA + lambdaB) << endl;
	cout << "STf *(lambdaA + lambdaB) = " << STf * (lambdaA + lambdaB) << endl << endl;

	S++;

}

int main(void)
{
	double Oc = 0, D;
	int S = 1;

	while (Oc <= 99)
	{
		cout << "S = " << S << endl;
		Simulare(S, D, Oc);
	}
}


