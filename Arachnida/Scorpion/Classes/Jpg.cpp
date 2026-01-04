#include "Jpg.hpp"

JpegJpg::JpegJpg(std::string file_name) : AllClasses(file_name) {}

JpegJpg::~JpegJpg() {}

void	JpegJpg::StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length)
{
	char			buffer[2];
	unsigned int	offset;

	this->tiff_start = file.tellg();
	if (!file.read(buffer, 2))
		return ;
	if (buffer[0] == 'I' && buffer[1] == 'I')
		little_endian = true;
	else if (buffer[0] == 'M' && buffer[1] == 'M')
		little_endian = false;
	else
		return ;
	collector = read_u16(file);
	if (file.fail())
		return ;
	if (collector != 42)
		return ;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (offset >= 8)
		file.seekg(offset - 8, std::ios::cur);
	else
		return ;
	length -= offset;
	Process_IFD(file, collector, length);
}

void	JpegJpg::parse_exif(std::ifstream &file, unsigned short &collector)
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

void    JpegJpg::UpdateData(std::ifstream &file)
{
	unsigned short	collector;
	unsigned short	length;
	std::streampos	block_start;

	while (!file.eof())
	{
		collector = read_u16(file);

		if (file.fail() || collector == 0xFFDA)
			return ;
		else if (collector == 0xFFE1)
		{
			block_start = file.tellg();
			length = read_u16(file);
			file.seekg(block_start); 
			parse_exif(file, collector);
			file.seekg(block_start + (std::streampos)length, std::ios::beg);
			this->little_endian = false;
		}
		else if (collector == 0xFFC0 || collector == 0xFFC2)
		{
			length = read_u16(file);
			file.seekg(1, std::ios::cur);
			unsigned short height = read_u16(file);
			unsigned short width = read_u16(file);
			std::stringstream ss_h, ss_w;

			ss_h << height;
			ss_w << width;
			this->data["Height"] = ss_h.str();
			this->data["Width"] = ss_w.str();

			file.seekg(length - 7, std::ios::cur);
		}
		else
		{
			if ((collector >> 8) != 0xFF)
				return ;
			length = read_u16(file);
			if (file.fail())
				return ;
			file.seekg(length - 2, std::ios::cur);
		}
	}
}

void	JpegJpg::parse()
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
			std::cout << "[x] " << file_name << " is not a valid JPEG or JPG." << std::endl;
	}
	if (control == true)
		UpdateData(file);
	file.close();
}
