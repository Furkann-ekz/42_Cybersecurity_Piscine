#include "Spider.hpp"

int	main(int ac, char **av)
{
	Spider	spider;
	if (ac < 2)
		return (cout << "Argument number is wrong." << endl, 1);
	spider.set_data();
}
