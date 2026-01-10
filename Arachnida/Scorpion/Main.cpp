#include "Scorpion.hpp"

int	main(int ac, char **av)
{
	Scorpion	*app;

	if (ac < 2)
	{
		std::cout << "Usage: ./scorpion [file1] [file2] ..." << std::endl;
		return (1);
	}

	app = new Scorpion(av + 1);

	app->Run();

	delete app;

	return (0);
}