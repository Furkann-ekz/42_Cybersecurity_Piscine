#include "Scorpion.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	try
	{
		Scorpion	scorpion();
	}
	catch (std::exception)
	{
		std::cout << "exception handling" << std::endl;
	}
}
