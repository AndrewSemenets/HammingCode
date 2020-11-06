#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <iostream>
#include <math.h> 

using namespace std;

vector<bool> vect_bits;
bool **table;

void encode(char* bits);	
void decode(char* bits);
int minPow(int errNum);

int main(int argc, char *argv[])
{
	
	if (argc < 3)
	{
		printf("\n Bad arguments \nTry: HammingCode e or d (bits)\n");
	}
	if (argv[1][0] == 'e') encode(argv[2]);
	else if(argv[1][0] == 'd') decode(argv[2]);
	else throw "Invalid first argument";

	cout << endl;
	system("pause");
	return 0;
}

void encode(char * bits)
{
	//------------------------------------формируем вектор входных данных и добавляем контрольные биты 
	for (int i = 0; i < strlen(bits); i++)
	{
		if(bits[i] == '0') vect_bits.push_back(0);
		else if(bits[i] == '1') vect_bits.push_back(1);
		else throw "Invalid bits string";
	}

	int cb_count;//control bit count
	for (int i = 0; pow(2, i) < vect_bits.size(); i++)
	{
		vect_bits.insert(vect_bits.begin() + pow(2, i)-1, false);
		cb_count = i;
	}
	cb_count++;


	//-----------------------------------добавляем контрольную матрицу
	table = new bool*[cb_count];
	for (int i = 0; i < cb_count; i++)
	{
		table[i] = new bool[vect_bits.size()];
		for (int j = 0; j < vect_bits.size(); j++)
		{
			table[i][j] = 0;
		}
	}
	for (int i = 0; i < cb_count; i++)
	{
		bool flag = 1;
		int count = 0;
		for (int j = pow(2, i)-1; j < vect_bits.size(); j++)
		{
			table[i][j] = flag;
			count++;
			if (count == pow(2, i))
			{
				flag = !flag;
				count = 0;
			}
		}
	}

	//---------------------------------------------------высчитываем контрольные биты
	for (int i = 0; i < cb_count; i++)
	{
		int strok_sum = 0;
		for (int j = pow(2, i) - 1; j < vect_bits.size(); j++)
		{
			
			if (vect_bits[j] && table[i][j])
			{
				strok_sum++;
			}
		}
		vect_bits[pow(2, i) - 1] = strok_sum % 2;
	}

	cout << "Encoded: ";
	for (bool bit : vect_bits)
	{
		cout << bit;
	}
	cout << endl;
}

void decode(char* bits)
{
	vector<bool> out_bits;
	vector<bool> contr_bits;
	//------------------------------------формируем вектор входных данных и добавляем контрольные биты 
	int pow_num = 0;
	int cb_count = 0;//control bit count
	for (int i = 0; i < strlen(bits); i++)
	{
		if (i == pow(2, pow_num) - 1)
		{
			pow_num++;
			if (bits[i] == '0') contr_bits.push_back(0);
			else if (bits[i] == '1') contr_bits.push_back(1);
			else throw "Invalid bits string";
			vect_bits.push_back(0);
			cb_count++;
			continue;
		}
		if (bits[i] == '0')
		{
			out_bits.push_back(0);
			vect_bits.push_back(0);
		}
		else if (bits[i] == '1')
		{
			out_bits.push_back(1);
			vect_bits.push_back(1);
		}
		else throw "Invalid bits string"; 
	}
	

	//-----------------------------------добавляем контрольную матрицу
	table = new bool* [cb_count];
	for (int i = 0; i < cb_count; i++)
	{
		table[i] = new bool[vect_bits.size()];
		for (int j = 0; j < vect_bits.size(); j++)
		{
			table[i][j] = 0;
		}
	}
	for (int i = 0; i < cb_count; i++)
	{
		bool flag = 1;
		int count = 0;
		for (int j = pow(2, i) - 1; j < vect_bits.size(); j++)
		{
			table[i][j] = flag;
			count++;
			if (count == pow(2, i))
			{
				flag = !flag;
				count = 0;
			}
		}
	}

	//---------------------------------------------------высчитываем контрольные биты и проверяем на наличие ошибки
	int error_num = 0;
	for (int i = 0; i < cb_count; i++)
	{
		int strok_sum = 0;
		for (int j = pow(2, i) - 1; j < vect_bits.size(); j++)
		{

			if (vect_bits[j] && table[i][j])
			{
				strok_sum++;
			}
		}
		vect_bits[pow(2, i) - 1] = strok_sum % 2;
		if (vect_bits[pow(2, i) - 1] != contr_bits[i])		//находим номер бита с ошибкой
		{
			error_num += pow(2, i);
		}
	}

	if (error_num != 0)
	{
		for (int i = 0; pow(2, i) < vect_bits.size(); i++)
		{
			if (error_num == pow(2, i))
			{
				cout << "The message contains an error in a control bit" << endl;
				error_num = 0;
			}
		}
	}
	if (error_num) error_num--;
	error_num = minPow(error_num);
	cout << "Recieved message: " << endl;
	for (bool bit : out_bits)
	{
		cout << bit;
	}
	cout << endl;

	//---------------------------------------------------исправляем ошибку(если есть)
	if (!error_num) cout << "The message contains no errors" << endl;
	else
	{
		cout << "The message contains an error in position No " << error_num + 1 << "\nCorrected version:";
		out_bits[error_num] = !out_bits[error_num];
		for (bool bit : out_bits)
		{
			cout << bit;
		}
		cout << endl;
	}

}

int minPow(int errNum)
{
	int out = errNum;
	for (int i = 0; pow(2, i) <= errNum; i++)
	{
		if (errNum > pow(2, i) - 1)
		{
			out--;
		}
	}
	return out;
}
//111000111000111000111
//111000111000111000111
//00111100001110000111000111
//00111110001110000111000111
//00111100001110000111000101

//0100010000111101
//100110000100001011101
//100110000110001011101

//wik
//100100101110001
//11110010001011110001
//11110110001011110001