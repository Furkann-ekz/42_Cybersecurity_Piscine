#ifndef BMP_HPP
#define BMP_HPP

#include "AllClass.hpp"

class Bmp: public AllClasses
{
	private:
	public:
		Bmp(std::string file_name);
		~Bmp();

		void	parse();
		void	display_info();
};

#endif