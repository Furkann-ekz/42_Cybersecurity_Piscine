#include "Scorpion.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
		return (0);
	try
	{
		av = av + 1;
		Scorpion	scorpion(av);
		scorpion.Run();
	}
	catch (std::exception)
	{
		std::cout << "exception handling" << std::endl;
	}
	return (0);
}
