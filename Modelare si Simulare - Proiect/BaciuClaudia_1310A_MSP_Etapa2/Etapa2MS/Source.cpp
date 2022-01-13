#include"Header.h"
using namespace std;

double genExp(double lambda_1)
{
	double u = 1;
	//log(0)=-inf, deci u<1
	u = (double)rand() / (RAND_MAX + 1);
	double x = (-1 / lambda_1) * log(1 - u);
	return x;
}
double genGauss(double m, double sigma)
{
	double S = 0;
	for (int i = 1; i <= 12; ++i)
		S += (double)rand() / RAND_MAX;
	return m + sigma * (S - 6);
}
double genTR() // functia de generare a timpilor de remediere
{
	double U, Tr;
	U = (double)rand() / ((double)RAND_MAX + 1);
	if (U < pA)
	{
		Tr = genExp(miuA);
	}
	else
	{
		Tr = genExp(miuB);
	}
	return Tr;
}
double genTR2() //geneartor valori aleatorii cu rep normala
{
	double U, Tr;
	U = (double)rand() / ((double)RAND_MAX + 1);
	if (U < pA)
	{
		Tr = genGauss(1 / miuA, 1 / (4 * miuA));
	}
	else
	{
		Tr = genGauss(1 / miuB, 1 / (4 * miuB));
	}
	return Tr;
}
int main()
{
	cout << "Numarul sistemelor (s):";
	int s;//nr sisteme
	cin >> s;
	double DS = 0;//durata simularii
	double Tr = 0;//timp remediere
	double STf = 0, STr = 0;//suma timp functionare/remediere
	double ceas = 0;//ceasul simularii
	int nf = s;//nr sisteme fuctionare
	int no = 0;//nr sisteme oprite
	int No = 0;
	int Nr = 0;//contor numar opriri/remedieri
	//generare valoare corespunzatoare primei opriri
	double TP0 = genExp(nf * (lamA + lamB));
	do {
		if ((nf == s) || ((nf > 0) && (TP0 < Tr)))
		{
			//o oprire accidentala
			No++;
			ceas += TP0;//actualizare ceas simulare la momentul ultimului eveniment
			if (no > 0)
				Tr -= TP0;//reducerea timpului de remediere odata cu avansul variabilei ceas
			STf += nf * TP0;//actualizare statistici
			nf--;//trecerea la o noua stare a sistemului
			no++;//no++ diferit de No++ !!
			if (no == 1)
			{
				//genTR2 pentru repartitia normal
				//genTR pentru reparitia exponential negativa
				Tr = genTR2();//muncitorul e obligat sa intervina imediat la oprirea primului sistem
				STr += Tr;//statistica
			}
		}
		else {
			//o remediere terminata
			Nr++; //incrementam numarul de remedieri
			ceas += Tr;//actualizare ceas la momentul ultimului eveniment

			if (nf > 0)
				TP0 -= Tr;
			//determinare exacta a mom opririi relativ la var ceas actualizata
			STf += nf * Tr;
			nf++;
			no--;
			if (no > 0)
			{
				Tr = genTR2();
				STr += Tr;//trecerea muncitorului la urmatorul sistem blocat
			}
		}
		if (nf > 0)
		{
			TP0 = genExp(nf * (lamA + lamB));
		}
	} while (No < 9 * lim_max);
	//verificarea preliminara a determinarii corecte a ev urmator
	cout << "Verificari preliminare:\n";
	cout << "\tNumarul total al opririlor: " << No << endl;
	cout << "\tSuma dintre nr sistemelor remediate si a celor ramase oprite la oprirea simularii: " << Nr + no << endl;
	cout << "\tSTr/Nr= " << double(STr / Nr) << " si valoarea teoretica=" << (double)(1 / (lamA + lamB) * (lamA / miuA + lamB / miuB)) << endl << endl;
	DS = ceas;//durata simularii este egala cu var ceas
	double D = (double)STf / (s * DS) * 100;//(%)-afisat in procente -->gradul de disponibilitate a sistemelor
	double O = (double)STr / DS * 100;//(%)-afisat in procente -->gradul de ocupare al muncitorului
	if (s == 1)
	{
		cout << "Valoare teoretica a gradului de disponibilitate: " << D_t << " % " << endl;
	}
	cout << "Gradul de disponibilitate a sistemelor: " << D << " % " << endl;
	cout << "Gradul de ocupare al muncitorului: " << O << " % " << endl;
	cout << "\n\t Verificare cantitativa a numarului de opriri\n";
	cout << "\t\t No= " << No << " si valoare teoretica este : " << DS * s * D / 100 * (lamA + lamB) << endl;
}


