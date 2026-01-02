#ifndef JPEGANDJPG_HPP
#define JPEGANDJPG_HPP

#include "AllClass.hpp"

class Jpeg_Jpg: public AllClasses
{
	private:
		void	StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length);
		void	Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length);
	public:
		Jpeg_Jpg(std::string file_name);
		~Jpeg_Jpg();

		void	parse();
		void	UpdateData(std::ifstream &file);
		void	parse_exif(std::ifstream &file, unsigned short &collector);


		void	HandleMakeData(std::ifstream &file, unsigned short &collector);
		int		GetTypeSize(std::ifstream &file, unsigned short &collector);

		void	HandleExifSub(std::ifstream &file, unsigned short &collector);

		void	HandleModelData(std::ifstream &file, unsigned short &collector);

		void	HandleTimeData(std::ifstream &file, unsigned short &collector);

		void	HandleGPS(std::ifstream &file, unsigned short &collector);
};

#endif