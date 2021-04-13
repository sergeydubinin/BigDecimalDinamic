#include <iostream>
#include <string>

using namespace std;

class BigDecimal
{
private:
	int length;
	int size;
	char* digits;
	void from_str(const char* str);
	char* operator ~() const;
	void changeSign();
	void copy(const BigDecimal& number);
	void enlarge(int newSize);

public:
	BigDecimal();
	BigDecimal(long number);
	BigDecimal(const char* str);
	BigDecimal(const BigDecimal& number);
	BigDecimal(BigDecimal&& number);
	BigDecimal& operator=(const BigDecimal& number);
	BigDecimal& operator=(BigDecimal&& number);
	void mul10();
	void div10();
	friend std::ostream& operator<< (std::ostream& stream, const BigDecimal& number);
	friend std::istream& operator>> (std::istream& stream, BigDecimal& number);
	bool isPositive() const;
	bool isNegative() const;
	friend BigDecimal operator + (const BigDecimal& a, const BigDecimal& b);
	friend BigDecimal operator - (const BigDecimal& a, const BigDecimal& b);
	~BigDecimal();
};
