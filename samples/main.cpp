#include "polynomials.h"
#include <locale>

int main()  
{
	try {
		setlocale(LC_ALL, "Rus");
		int quan_input, action, operation;
		bool need_a = true, loop = true;
		Polynomial res, a, b;
		while (loop)
		{
			cout << "�������� �������� � ������� � �����:\n1)��������� ��������� �� ���������\n2)�������� ���������\n3)��������� ���������\n4)��������� ���������\n5)��������� �������� �� �����\n";
			cin >> operation;
			if (operation < 5 && operation >0)
			{
				if (need_a) {
					cout << "������� ���������� ������� � ������ ��������, � ����� ������� ������: ����������� ����� �������, ������� x, ������� y, ������� z: ";
					cin >> a;
				}
				cout << "������� ���������� ������� �� ������ ��������, � ����� ������� ������: ����������� ����� �������, ������� x, ������� y, ������� z: ";
				cin >> b;
				switch (operation)
				{
				case 1: {
					if (a == b) cout << "�������� �����\n"; else cout << "�������� �� �����\n";
					break;
				}
				case 2: {
					res = a + b;
					cout << "��������� ��������: " << res << endl;
					break;
				}
				case 3: {
					res = a - b;
					cout << "��������� ���������: " << res << endl;
					break;
				}
				case 4: {
					res = a * b;
					cout << "��������� ���������: " << res << endl;
					break;
				}
				}

			}
			else if (operation == 5)
			{
				double factor;
				if (need_a) {
					cout << "������� ���������� ������� � ��������, � ����� ������� ������: ����������� ����� �������, ������� x, ������� y, ������� z: ";
					cin >> a;
				}
				cout << "������� �����: ";
				cin >> factor;
				res = a * factor;
				cout << "��������� ���������: " << res << endl;
			}
			else throw invalid_argument("Invalid operation input ");
			cout << "�������� �������� � ������� ��� �����:\n1)��������� ���������\n2)���������� ���������� ��������� � ������ �������� �������\n";
			if(operation!=1) cout << "3)���������� ���������� ��������� � ���������� �����������\n";
			cin >> action;
			switch (action)
			{
			case 2: {
				need_a = true;
				break;
			}
			case 3: {
				if (operation == 1) throw invalid_argument("Invalid operation input ");
				need_a = false;
				a = res;
				break;
			}
			case 1: {
				loop = false;
				break;
			}
			default: {
				throw invalid_argument("Invalid operation input ");
			}
			}
		}
	}
	catch (const exception& e)
	{
		cout << e.what();
	}
}