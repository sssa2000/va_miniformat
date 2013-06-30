// test_tinyformat.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "miniformat_va.h"
#include <string>
#include <vector>

template <typename T>
void test()
{
	T str;
	//被注释的代码都是使用变长模板参数以后，不被支持的代码

	//mini::format(str, "String: %1 Int: %0, Float: %(.3)2\n", 100, "JJ", 3.141592);
	//assert(std::string(str) == "String: JJ Int: 100, Float: 3.142\n");
	//mini::format(str, "%1 %1 %1\n", 100, "JJ", 3.141592);
	//assert(std::string(str) == "JJ JJ JJ\n");
	//mini::format(str, "%(.2)2 %(.3)2 %(.4)2\n", 100, "JJ", 3.141592);
	//assert(std::string(str) == "3.14 3.142 3.1416\n");
	//mini::format(str, "%2 %1 %0 %0 %1 %2\n", 100, "JJ", 3.141592);
	//assert(std::string(str) == "3.141592 JJ 100 100 JJ 3.141592\n");
	mini::format(str, "%0\n", "P1");
	assert(std::string(str) == "P1\n");
	mini::format(str, "%0\n", 7);
	assert(std::string(str) == "7\n");
	mini::format(str, "%0\n", 3.14);
	assert(std::string(str) == "3.140000\n");
	mini::format(str, "%0 %1\n", "one", "two");
	assert(std::string(str) == "one two\n");
	mini::format(str, "%0 %1 %2 %3\n", "one", "two", "three", "four");
	assert(std::string(str) == "one two three four\n");
	mini::format(str, "%0 %1 %2 %3 %4\n", "one", "two", "three", "four", "five");
	assert(std::string(str) == "one two three four five\n");
	mini::format(str, "%0 %1 %2 %3 %4 %5\n", "one", "two", "three", "four", "five", "six");
	assert(std::string(str) == "one two three four five six\n");
	mini::format(str, "%0 %1 %2 %3 %4 %5 %6\n", "one", "two", "three", "four", "five", "six", "seven");
	assert(std::string(str) == "one two three four five six seven\n");
	mini::format(str, "%0 %1 %2 %3 %4 %5 %6 %7\n", "one", "two", "three", "four", "five", "six", "seven", "eight");
	assert(std::string(str) == "one two three four five six seven eight\n");
	mini::format(str, "%0 %1 %2 %3 %4 %5 %6 %7 %8\n", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine");
	assert(std::string(str) == "one two three four five six seven eight nine\n");
	//mini::format(str, "%0 %%, %%0\n", "Literal");
	//assert(std::string(str) == "Literal %, %0\n");
	mini::format(str, "%(6)0\n", 100);
	assert(std::string(str) == "   100\n");
	mini::format(str, "%(2)0\n", 100);
	assert(std::string(str) == "100\n");
	mini::format(str, "%(3)0\n", 100);
	assert(std::string(str) == "100\n");
	mini::format(str, "%(6.2)0\n", 3.14);
	assert(std::string(str) == "  3.14\n");
	mini::format(str, "%(6.2)0\n", -3.14);
	assert(std::string(str) == " -3.14\n");
	// Followings should assert in debug.
	//mini::format(std::string(str), "%0 %n\n", 3);
	//mini::format(std::string(str), "%(.3)1\n", 3.141592);
	//mini::format(std::string(str), "String: %1 Int: %0, Float: %(.3)3\n", 100, "JJ", 3.141592);
}
int _tmain(int argc, _TCHAR* argv[])
{
	test<std::string>();
	system("pause");
	return 0;
}

	