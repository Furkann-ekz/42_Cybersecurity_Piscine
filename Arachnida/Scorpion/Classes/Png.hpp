#ifndef PNG_HPP
#define PNG_HPP

#include "AllClass.hpp"

class Png: public AllClasses
{
	private:
	public:
		Png(std::string file_name);
		~Png();

		void	parse();
		void	ParseStarting(std::ifstream &file, unsigned int &collector);
		void	Continue(std::ifstream &file);
};

#endif