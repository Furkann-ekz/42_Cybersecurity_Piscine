#ifndef BMP_HPP
#define BMP_HPP

#include "AllClass.hpp"

class Bmp: public AllClasses
{
	private:
		bool	UpDown;
	public:
		Bmp(std::string file_name);
		~Bmp();

		void	parse();
		void	ParseContinue(std::ifstream &file);
};

#endif