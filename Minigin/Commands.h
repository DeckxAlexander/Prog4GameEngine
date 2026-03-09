#pragma once
#include <iostream>


class Command 
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
	
};


class TestCommand : public Command 
{
public:
	void Execute() override { std::cout << "Test"; }
};

