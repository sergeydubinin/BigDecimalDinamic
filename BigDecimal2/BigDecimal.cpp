#include <cstring>
#include <algorithm>
#include "BigDecimal.h"

BigDecimal::BigDecimal() : length(1), size(1)
{
	digits = new char[size + 2]();

	for (int i = 0; i <= size; i++)
	{
		digits[i] = '0';
	}
}

BigDecimal::BigDecimal(long number)
{
	if (number == 0)
	{
		length = 1;
		size = 1;
		digits = new char[size + 2]();
		digits[0] = '0';
		digits[1] = '0';
		return;
	}

	size = std::ceil(std::log10(std::abs(number) + 0.2));
	digits = new char[size + 2]();

	if (number < 0)
	{
		digits[size] = '9';
		number *= -1;
	}
	else
	{
		digits[size] = '0';
	}

	int i = 0;
	for (; number != 0; i++)
	{
		int rem = number % 10;
		digits[i] = '0' + rem;
		number = number / 10;
	}
	length = i;
}

BigDecimal::BigDecimal(const BigDecimal& number)
{
	digits = nullptr;
	copy(number);
}

BigDecimal::BigDecimal(BigDecimal&& number) : digits(nullptr)
{
	*this = std::move(number);
}

BigDecimal& BigDecimal::operator=(const BigDecimal& number)
{
	if (this == &number)
	{
		return *this;
	}
	copy(number);
	return *this;
}

BigDecimal& BigDecimal::operator=(BigDecimal&& number)
{
	if (this != &number)
	{
		delete[] digits;

		digits = number.digits;
		length = number.length;
		size = number.size;

		number.digits = nullptr;
	}
	return *this;
}

void BigDecimal::from_str(const char* str)
{
	if (str == nullptr) 
	{
		throw ("Initialization by nullptr");
	}

	delete[] digits;

	size = strlen(str);
	if (size && str[0] == '-')
	{
		size--;
	}

	digits = new char[size + 2]();

	int n = strlen(str);
	int i = n - 1, j = 0;
	if (strcmp(str, "0") == 0)
	{
		digits[size] = '0';
		length = 1;
		return;
	}
	else if (n && str[0] == '-')
	{
		digits[size] = '9';
		n--;
	}
	else
	{
		digits[size] = '0';
	}

	for (; i >= 0 && str[i] >= '0' && str[i] <= '9'; i--, j++)
	{
		digits[j] = str[i];
	}
	length = j;

	if (length == 0 || length != n)
	{
		throw ("Invalid string passed to constructor");
	}
}

BigDecimal::BigDecimal(const char* str)
{
	digits = nullptr;
	from_str(str);
}

void BigDecimal::mul10()
{
	if (!isPositive() && !isNegative())
	{
		return;
	}

	if (length == size)
	{
		enlarge(size + 1);
	}

	for (int i = length; i > 0; i--)
	{
		digits[i] = digits[i - 1];
	}
	digits[0] = '0';
	length++;
}

void BigDecimal::div10()
{
	if (!isPositive() && !isNegative())
	{
		return;
	}

	for (int i = 0; i < length; i++)
	{
		digits[i] = digits[i + 1];
	}
	length--;
	digits[length] = '0';
}

std::ostream& operator<< (std::ostream& stream, const BigDecimal& number)
{
	if (number.digits[number.size] == '9')
	{
		stream << '-';
	}
	for (int i = number.length - 1; i >= 0; i--)
	{
		stream << number.digits[i];
	}

	return stream;
}

std::istream& operator>> (std::istream& stream, BigDecimal& number)
{
	std::string str;
	stream >> str;
	try
	{
		number.from_str(str.data());
	}
	catch (std::exception& error)
	{
		stream.setstate(std::ios::failbit);
		return stream;
	}
	return stream;
}

bool BigDecimal::isPositive() const
{
	if (digits[size] != '0')
	{
		return false;
	}

	if (length == 1 && digits[0] == '0')
	{
		return false;
	}

	return true;
}

bool BigDecimal::isNegative() const
{
	return digits[size] == '9';
}

char* BigDecimal::operator ~ () const
{
	auto complement = new char[size + 2]();
	for (int i = 0; i <= size; i++)
	{
		complement[i] = '0';
	}

	if (isPositive())
	{
		//������������� ����� - ������ ������ �� ����
		//������ ������������� ������
		for (int i = size; i > 0; i--)
		{
			complement[i] = digits[size - i];
		}
	}
	else if (isNegative())
	{
		//������ ���� 9, ���������� ��� ����� �������������
		complement[0] = '9';
		for (int i = 1; i <= size; i++)
		{
			//��������� 1
			if (i == size)
			{
				complement[i] = '9' - digits[size - i] + '1';
			}
			else
			{
				complement[i] = '9' - digits[size - i] + '0';
			}
		}
		//���������� ������� - ��������� �����
		if (complement[size] == '9' + 1)
		{
			complement[size] = '0';
			bool isActive = true;
			for (int i = size - 1; i >= 0 && isActive; i--)
			{
				if (isActive)
				{
					if (complement[i] == '9')
					{
						complement[i] = '0';
					}
					else
					{
						isActive = false;
						complement[i]++;
					}
				}
			}
		}
	}

	return complement;
}

void BigDecimal::changeSign()
{
	if (isPositive())
	{
		digits[size] = '9';

	}
	else if (isNegative())
	{
		digits[size] = '0';

	}
}

BigDecimal operator + (const BigDecimal& a, const BigDecimal& b)
{
	auto cur = a;
	auto number = b;

	int size = cur.size;
	// ������ ���������� �����
	if (cur.size != number.size)
	{
		size = std::max(cur.size, number.size);
		cur.enlarge(size);
		number.enlarge(size);
	}

	if (cur.digits[cur.size] == number.digits[number.size]
		&& (cur.digits[cur.size - 1] - '0' + number.digits[number.size - 1] - '0') > 9)
	{
		size++;
		cur.enlarge(size);
		number.enlarge(size);
	}

	//�������� �������������� ���
	auto c1 = ~cur;
	auto c2 = ~number;

	char* str = new char[cur.size + 2]();
	char* temp = new char[cur.size + 1]();

	// ���������� �����
	if (cur.digits[cur.size] == number.digits[number.size])
	{
		// ������������� �����
		if (cur.isPositive() && number.isPositive())
		{
			bool hasAdd = false;//���������� ���� �� ��� 1
			for (int i = 0; i < cur.size; i++)
			{
				str[i] = c1[cur.size - i] + c2[cur.size - i] - '0' + hasAdd;
				if (str[i] > '9')
				{
					str[i] = str[i] - 10;
					hasAdd = true;
				}
				else
				{
					hasAdd = false;
				}
			}
			//������������� ������
			for (int i = 0; i < cur.size; i++)
			{
				temp[i] = str[cur.size - i - 1];
			}

			BigDecimal number;
			if (hasAdd)
			{
				number = BigDecimal((std::string("1") + temp).data());
			}
			else
			{
				auto ptr = temp;
				//������� ������ ����
				while (*ptr == '0')
				{
					++ptr;
				}
				number = BigDecimal(ptr);
			}

			delete[] str;
			delete[] temp;
			return number;
		}
		else if (cur.isNegative() && number.isNegative()) // ��� �������������
		{
			bool hasAdd = false;
			for (int i = 0; i < cur.size; i++)
			{
				str[i] = c1[cur.size - i] + c2[cur.size - i] - '0' + hasAdd;
				if (str[i] > '9')
				{
					str[i] = str[i] - 10;
					hasAdd = true;
				}
				else
				{
					hasAdd = false;
				}

				temp[i] = '0';
			}
			// �������� ��������� (������� ��������� � �������)
			for (int i = 0; i < cur.size; i++)
			{
				auto c = str[i];
				temp[cur.size - 1 - i] = '9' - c + '0' + hasAdd;
				if (temp[cur.size - 1 - i] > '9')
				{
					hasAdd = true;
					temp[cur.size - 1 - i] = '0';
				}
				else
				{
					hasAdd = false;
				}
			}
			auto ptr = temp;
			while (*ptr == '0')
			{
				++ptr;
			}

			auto number = BigDecimal(std::string(std::string("-") + ptr).data());
			delete[] str;
			delete[] temp;
			return number;
		}
		else
		{
			//�������� � 0
			if (cur.isPositive())
			{
				return cur;
			}
			else
			{
				return number;
			}
		}

	}
	else // ������ �����
	{
		bool hasAdd = false;
		for (int i = 0; i < cur.size; i++)
		{
			str[i] = c1[cur.size - i] + c2[cur.size - i] - '0' + hasAdd;
			if (str[i] > '9')
			{
				str[i] = str[i] - 10;
				hasAdd = true;
			}
			else
			{
				hasAdd = false;
			}
		}
		//��������� - ������������� �����
		if (hasAdd)
		{
			//������������� ������
			for (int i = 0; i < cur.size; i++)
			{
				temp[i] = str[cur.size - i - 1];
			}
			auto ptr = temp;
			while (*ptr == '0')
			{
				++ptr;
			}
			auto number = BigDecimal(ptr);
			delete[] str;
			delete[] temp;
			return number;
		}
		else //������������� �����
		{
			hasAdd = true; //�������� ������
			for (int i = 0; i < cur.size; i++)
			{
				auto c = str[i];
				temp[cur.size - 1 - i] = '9' - c + '0' + hasAdd;
				if (temp[cur.size - 1 - i] > '9')
				{
					hasAdd = true;
					temp[cur.size - 1 - i] = '0';
				}
				else
				{
					hasAdd = false;
				}
			}
			auto ptr = temp;
			while (*ptr == '0')
			{
				++ptr;
			}
			auto number = BigDecimal(std::string(std::string("-") + ptr).data());
			delete[] str;
			delete[] temp;
			return number;
		}
	}

	auto t_number = BigDecimal(str);
	delete[] str;
	delete[] temp;
	return t_number;
}

BigDecimal operator - (const BigDecimal& a, const BigDecimal& b)
{
	auto number = b;
	number.changeSign();
	return a + number;
}

BigDecimal::~BigDecimal()
{
	delete[] digits;
}

void BigDecimal::copy(const BigDecimal& number)
{
	delete[] digits;

	length = number.length;
	size = number.size;

	digits = new char[size + 2]();

	for (int i = 0; i <= size; i++)
	{
		digits[i] = number.digits[i];
	}
}

void BigDecimal::enlarge(int newSize)
{
	if (newSize == size)
	{
		return;
	}
	auto temp = new char[newSize + 2];
	for (int i = 0; i < size; i++)
	{
		temp[i] = digits[i];
	}
	for (int j = size; j < newSize; j++)
	{
		temp[j] = '0';
	}
	temp[newSize] = digits[size];
	temp[newSize + 1] = '\0';
	size = newSize;
	delete[] digits;
	digits = temp;
}

