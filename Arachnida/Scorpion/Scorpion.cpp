#include "Scorpion.hpp"

Scorpion::Scorpion() {}

Scorpion::~Scorpion()
{
	for (size_t i = 0; i < files.size(); i++)
		delete files[i];
	files.clear();
}
