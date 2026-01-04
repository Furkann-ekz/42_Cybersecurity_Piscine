#include "Tif.hpp"

TiffTif::TiffTif(std::string file_name) : AllClasses(file_name) {}

TiffTif::~TiffTif() {}

void	TiffTif::ParseContinue(std::ifstream &file)
{
	unsigned int	offset;
	unsigned short	length;
	unsigned short	collector;

	length = 0xFFFF;
	collector = 0;
	offset = read_u32(file);
	if (file.fail())
		return ;
	file.seekg(offset, std::ios::beg);
	Process_IFD(file, collector, length);
}

void	TiffTif::parse()
{
	char			buffer[4];
	unsigned char	b1;
	unsigned char	b2;
	unsigned short	collector;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 2))
	{
		b1 = (unsigned char)buffer[0];
		b2 = (unsigned char)buffer[1];
		if ((b1 == 'I' && b2 == 'I') || (b1 == 'M' && b2 == 'M'))
		{
			if (b1 == 'I' && b2 == 'I')
				little_endian = true;
			else
				little_endian = false;
			collector = read_u16(file);
			if (file.fail())
				return ;
			if (collector != 42)
			{
				std::cout << "[x] " << file_name << " is not a valid TIFF or TIF." << std::endl;
				file.close();
				return ;
			}
		}
		else
		{
			std::cout << "[x] " << file_name << " is not a valid TIFF or TIF." << std::endl;
			file.close();
			return ;
		}
	}
	ParseContinue(file);
	file.close();
}
