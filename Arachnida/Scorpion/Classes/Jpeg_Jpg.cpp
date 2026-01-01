#include "Jpeg_Jpg.hpp"

Jpeg_Jpg::Jpeg_Jpg(std::string file_name) : AllClasses(file_name) {}

Jpeg_Jpg::~Jpeg_Jpg() {}

void	Jpeg_Jpg::Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length)
{
	unsigned short	num_entries;
	unsigned int	four_byte;

	if ((num_entries = read_u16(file)) == 0)
		return ;
	length -= num_entries;
	while (num_entries >= 0)
	{
		length -= num_entries;
		num_entries -= 12;
		if ((collector = read_u16(file)) == 0)
			return ;
		if (collector == 3)
			;
	}
}

void	Jpeg_Jpg::StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length)
{
	char			buffer[2];
	unsigned int	offset;

	if (!file.read(buffer, 2))
		return ;
	if (buffer[0] == 'I' && buffer[1] == 'I')
		little_endian = true;
	else if (buffer[0] == 'M' && buffer[1] == 'M')
		little_endian = false;
	else
		return ;
	if ((collector = read_u16(file)) == 0)
		return ;
	if (collector != 42)
		return ;
	if ((offset = read_u32(file)) == 0)
		return ;
	if (offset >= 8)
		file.seekg(offset - 8, std::ios::cur);
	else
		return ;
	length -= offset;
	Process_IFD(file, collector, length);
}

void	Jpeg_Jpg::parse_exif(std::ifstream &file, unsigned short &collector)
{
	char			exif[6];
	unsigned short	length;

	length = read_u16(file);
	if (length == 0 || length == 1)
		return ;
	length -= 2;
	if (!file.read(exif, 6))
		return ;
	if (!(exif[0] == 'E' && exif[1] == 'x' && exif[2] == 'i' && exif[3] == 'f' && exif[4] == '\0' && exif[5] == '\0'))
		return ;
	length -= 6;
	StartParsing(file, collector, length);
}

void	Jpeg_Jpg::UpdateData(std::ifstream &file)
{
	unsigned short	collector;
	unsigned short	length;

	while (!file.eof())
	{
		if ((collector = read_u16(file)) == 0 || collector == 0xFFDA)
			return ;
		else if (collector == 0xFFE1)
		{
			parse_exif(file, collector);
			return ;
		}
		//else if (collector == 0xFFC0)
			//
		else
		{
			if ((collector >> 8) != 0xFF)
				return ;
			if ((length = read_u16(file))  == 0)
				return ;
			file.seekg(length - 2, std::ios::cur);
		}
	}
}

void	Jpeg_Jpg::parse()
{
	char			buffer[2];
	unsigned char	b1;
	unsigned char	b2;
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
		if (b1 == 0xFF && b2 == 0xD8)
			control = true;
		else
			std::cout << "[x] " << file_name << " is not a valid JPEG." << std::endl;
	}
	if (control == true)
		UpdateData(file);
	file.close();
}
