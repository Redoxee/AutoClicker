#include "pch.h"
#include "Clicker.h"


Clicker::Clicker()
{
}


Clicker::~Clicker()
{
}

void Clicker::Tick()
{
	this->clickerScore += this->speed;
}

bool Clicker::IsOver()
{
	return this->clickerScore >= this->targetScore;
}

std::ostream& operator<<(std::ostream& stream, const Clicker* c)
{
	stream << "Score : " << c->clickerScore << " Speed : " << c->speed << " Target : " << c->targetScore;
	return stream;
}

