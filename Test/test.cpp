#include "pch.h"
#include "..\BigDecimal2\BigDecimal.cpp"

TEST(BigDecimalConstructor, DefaultConstructor)
{
	BigDecimal a;
	ASSERT_EQ(a, 0);
}

TEST(BigDecimalConstructor, LongConstructor)
{
	BigDecimal a(12345);
	ASSERT_EQ(a, "12345");
	BigDecimal b(-5786);
	ASSERT_EQ(b, "-5786");
	int zero = -0;
	BigDecimal c(zero);
	ASSERT_EQ(c, "0");
}

TEST(BigDecimalConstructor, StringConstructor)
{
	BigDecimal a("12345");
	BigDecimal b("-8963");
	BigDecimal c("+78696");
	BigDecimal d("-0");
	ASSERT_EQ(a, 12345);
	ASSERT_EQ(b, -8963);
	ASSERT_EQ(c, 78696);
	ASSERT_EQ(d, 0);
	char* test = nullptr;
	ASSERT_THROW(BigDecimal d(test), invalid_argument);
	ASSERT_THROW(BigDecimal(""), invalid_argument);
	ASSERT_THROW(BigDecimal("jdfhksdj"), invalid_argument);
	ASSERT_THROW(BigDecimal("-dededede"), invalid_argument);
}

TEST(BigDecimalConstructor, CopyConstructor) 
{
	BigDecimal a(12345);
	BigDecimal b(a);
	ASSERT_EQ(b, "12345");
	ASSERT_EQ(a, "12345");
	BigDecimal c("-12345");
	BigDecimal d(c);
	ASSERT_EQ(c, -12345);
	ASSERT_EQ(d, -12345);
}

TEST(BigDecimalConstructor, MoveConstructor) 
{
	BigDecimal a(BigDecimal(1556));
	ASSERT_EQ(a, "1556");
	BigDecimal b(BigDecimal("-754502"));
	ASSERT_EQ(b, -754502);
}

TEST(BigDecimalMetods, Addition)
{
	BigDecimal a(1234);
	BigDecimal b(11111);
	ASSERT_EQ(a + b, 12345);
	a = 7654;
	b = 3111;
	ASSERT_EQ(a + b, 10765);
	a = -7123;
	b = -4321;
	ASSERT_EQ(a + b, -11444);
	a = 55123;
	b = -55012;
	ASSERT_EQ(a + b, 111);
	a = "456978";
	b = "0";
	ASSERT_EQ(a - b, 23568);
	a = "-898926";
	b = "0";
	ASSERT_EQ(a + b, 456978);
	a = "325689";
	b = "-325689";
	ASSERT_EQ(a + b, 0);
}

TEST(BigDecimalMetods, Subtraction)
{
	BigDecimal a(11111);
	BigDecimal b(1111);
	ASSERT_EQ(a - b, 10000);
	a = -11111;
	b = 1456;
	ASSERT_EQ(a - b, -12567);
	a = -58796;
	b = -2569;
	ASSERT_EQ(a - b, -56227);
	a = "23568";
	b = "0";
	ASSERT_EQ(a + b, 456978);
	a = "-546454";
	b = "0";
	ASSERT_EQ(a - b, 23568);
	a = "78638941";
	b = "78638941";
	ASSERT_EQ(a - b, 0);
}

TEST(BigDecimalMetods, Mul10)
{
	BigDecimal a(1234);
	a.mul10();
	ASSERT_EQ(a, 12340);
	BigDecimal b("0");
	b.mul10();
	ASSERT_EQ(b, 0);
	BigDecimal c("-8978");
	c.mul10();
	ASSERT_EQ(c, -89780);
}

TEST(BigDecimalMetods, Div10)
{
	BigDecimal a("1234");
	a.div10();
	ASSERT_EQ(a, 123);
	BigDecimal b("0");
	b.div10();
	ASSERT_EQ(b, 0);
	BigDecimal c("-65874");
	c.div10();
	ASSERT_EQ(c, -6587);
}

TEST(IOMetods, Input)
{
	stringstream ss;
	ss << "12345\n";
	BigDecimal a;
	ss >> a;
	ASSERT_EQ(a, 12345);
	ASSERT_FALSE(ss.fail());
	ss << "-12345\n";
	ss >> a;
	ASSERT_EQ(a, -12345);
	ASSERT_FALSE(ss.fail());
	ss << "rtrtttfs\n";
	ss >> a;
	ASSERT_TRUE(ss.fail());
}

TEST(IOMetods, Output)
{
	stringstream ss;
	BigDecimal a(12345);
	ss << a;
	ASSERT_EQ(ss.str(), "12345");
	ss.str(string());
	a = -12345;
	ss << a;
	ASSERT_EQ(ss.str(), "-12345");
}
