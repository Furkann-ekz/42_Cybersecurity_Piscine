#ifndef TIF_HPP
#define TIF_HPP

#include "AllClass.hpp"

class Tif: public AllClasses
{
	public:
		Tif(std::string file_name);
		~Tif();

		void	parse();
		void	ParseContinue(std::ifstream &file);
};

#endif