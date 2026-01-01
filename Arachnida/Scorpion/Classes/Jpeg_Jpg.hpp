#ifndef JPEGANDJPG_HPP
#define JPEGANDJPG_HPP

#include "AllClass.hpp"

class Jpeg_Jpg: public AllClasses
{
	private:
		void	Jpeg_Jpg::StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length);
		void	Jpeg_Jpg::Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length);
	public:
		Jpeg_Jpg(std::string file_name);
		~Jpeg_Jpg();

		void	parse();
		void	UpdateData(std::ifstream &file);
		void	parse_exif(std::ifstream &file, unsigned short &collector);
};

#endif