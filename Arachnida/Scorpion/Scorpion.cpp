#include "Scorpion.hpp"

std::string	Scorpion::get_extension(std::string f_name)
{
	size_t		i;

	i = f_name.rfind('.');
	if (i != std::string::npos)
		return (f_name.substr(i));
	return ("");
}

Scorpion::Scorpion(char **av)
{
	size_t	i;

	i = 0;
	while (av[++i])
		files.push_back(av[i]);
}

Scorpion::~Scorpion()
{
	for (size_t i = 0; i < classes.size(); i++)
		delete classes[i];
	classes.clear();
}
