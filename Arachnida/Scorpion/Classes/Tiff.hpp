#ifndef TIFF_HPP
#define TIFF_HPP

#include "AllClass.hpp"

class Tiff: public AllClasses
{
	private:
	public:
		Tiff(std::string file_name);
		~Tiff();

		void	parse();
		void	display_info();
};

#endif