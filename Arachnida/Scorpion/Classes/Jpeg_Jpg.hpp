#ifndef JPEGANDJPG_HPP
#define JPEGANDJPG_HPP

#include "AllClass.hpp"

class Jpeg_Jpg: public AllClasses
{
	private:
		void		StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length);
		void		Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length);

		void		UpdateData(std::ifstream &file);
		void		parse_exif(std::ifstream &file, unsigned short &collector);


		void		HandleStringData(std::ifstream &file, unsigned short &collector);
		int			GetTypeSize(std::ifstream &file, unsigned short &collector);

		void		HandleExifSubAndGPS(std::ifstream &file, unsigned short &collector);
		void		HandleValueData(std::ifstream &file, unsigned short &collector);
		void		HandleGPSRef(std::ifstream &file, unsigned short &collector);
		std::string	get_rational(std::ifstream &file);
	public:
		Jpeg_Jpg(std::string file_name);
		~Jpeg_Jpg();

		void		parse();
};

#endif