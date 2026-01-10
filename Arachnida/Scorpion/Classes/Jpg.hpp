#ifndef JPG_HPP
#define JPG_HPP

#include "AllClass.hpp"

class Jpg: public AllClasses
{
	public:
		Jpg(std::string file_name);
		~Jpg();

		void	parse();
		void	StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length);
		void	UpdateData(std::ifstream &file);
		void	parse_exif(std::ifstream &file, unsigned short &collector);
};

#endif