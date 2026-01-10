#include "Tif.hpp"

Tif::Tif(std::string file_name) : AllClasses(file_name) {}

Tif::~Tif() {}

void	Tif::ParseContinue(std::ifstream &file)
{
	unsigned int	offset;
	unsigned short	collector;
	unsigned short	length;

	length = 0xFFFF; 
	collector = 0;

	offset = read_u32(file);
	if (file.fail())
		return ;
	
	file.seekg(offset, std::ios::beg);
	
	Process_IFD(file, collector, length);
}

void	Tif::parse()
{
	char			buffer[2];
	unsigned short	magic_num;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}

	if (file.read(buffer, 2))
	{
		if (buffer[0] == 'I' && buffer[1] == 'I')
			this->little_endian = true;
		else if (buffer[0] == 'M' && buffer[1] == 'M')
			this->little_endian = false;
		else
		{
			std::cout << "[x] " << file_name << " is not a valid TIFF." << std::endl;
			file.close();
			return ;
		}
	}
	else
	{
		file.close();
		return ;
	}

	magic_num = read_u16(file);
	if (file.fail() || magic_num != 42)
	{
		std::cout << "[x] " << file_name << " is not a valid TIFF (Magic number mismatch)." << std::endl;
		file.close();
		return ;
	}

	ParseContinue(file);
	file.close();
}
