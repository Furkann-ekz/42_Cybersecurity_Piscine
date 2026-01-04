#ifndef TIF_HPP
#define TIF_HPP

#include "AllClass.hpp"

class TiffTif: public AllClasses
{
	public:
		TiffTif(std::string file_name);
		~TiffTif();

		void	parse();
		void	ParseContinue(std::ifstream &file);
};

#endif