// aop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <string>
#include "has_member.h"
#include "aspect.h"

using namespace std;

class Test
{
private:
	void foo(int)
	{}

	int i;
};

HAS_MEMBER_FUNCTION(foo);

struct TimeElapsedAspect
{
	void Before(int i)
	{
		std::cout << "start timer" << std::endl;
		timer = std::chrono::system_clock::now();
	}

	void After(int i)
	{
		cout << "time elapsed: " << std::chrono::system_clock::duration(std::chrono::system_clock::now() - timer).count() << endl;
	}

private:
	std::chrono::system_clock::time_point timer{ std::chrono::system_clock::now() };
};

struct LoggingAspect
{
	LoggingAspect()
	{
		static int id = 0;
		std::cout << "LoggingAspect::construct -- id:" << id++ << std::endl;
	}

	void Before(int i)
	{
		std::cout << "entering" << std::endl;
	}

	void After(int i)
	{
		std::cout << "leaving" << std::endl;
	}
};

struct NoBeforeOrAfter
{
	NoBeforeOrAfter()
	{
		static int id = 0;
		std::cout << "NoBeforeOrAfter::construct -- id:" << id++ << std::endl;
	}

	void Before(const std::string&)
	{
	}

	void After(const std::string&)
	{
	}
};

void foo(int a)
{
	cout << "real HT function: " << a << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	auto value = has_member_foo<Test, int>::value;

	std::cout << value << std::endl;

	aop::Invoke<NoBeforeOrAfter, LoggingAspect, TimeElapsedAspect>(&foo, 1);
	cout << "-----------------------" << endl;
	aop::Invoke<TimeElapsedAspect, NoBeforeOrAfter, LoggingAspect>(&foo, 1);
	cout << "-----------------------" << endl;
	aop::Invoke<LoggingAspect, LoggingAspect, NoBeforeOrAfter>(&foo, 1);
	cout << "-----------------------" << endl;

	LoggingAspect logging;
	TimeElapsedAspect timer;
	NoBeforeOrAfter no_before_or_after;

	aop::InvokeWithAspects<LoggingAspect, LoggingAspect, TimeElapsedAspect, NoBeforeOrAfter>(std::forward<LoggingAspect>(logging),
		std::forward<LoggingAspect>(logging), 
		std::forward<TimeElapsedAspect>(timer),
		std::forward<NoBeforeOrAfter>(no_before_or_after),
		&foo, 1);

	system("pause");

	return 0;
}

