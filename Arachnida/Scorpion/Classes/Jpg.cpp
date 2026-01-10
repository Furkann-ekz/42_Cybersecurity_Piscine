#include "Jpg.hpp"

Jpg::Jpg(std::string file_name) : AllClasses(file_name)
{
	this->little_endian = false; 
}

Jpg::~Jpg() {}

void	Jpg::StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length)
{
	char			buffer[2];
	unsigned int	offset;

	this->tiff_start = file.tellg();
	if (!file.read(buffer, 2))
		return ;
	if (buffer[0] == 'I' && buffer[1] == 'I')
		this->little_endian = true;
	else if (buffer[0] == 'M' && buffer[1] == 'M')
		this->little_endian = false;
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

void	Jpg::parse_exif(std::ifstream &file, unsigned short &collector)
{
	char			exif[6];
	unsigned short	length;

	length = read_u16(file);
	if (length == 0 || length == 1)
		return ;
	length -= 2;
	if (!file.read(exif, 6))
		return ;
	if (!(exif[0] == 'E' && exif[1] == 'x' && exif[2] == 'i' 
		&& exif[3] == 'f' && exif[4] == '\0' && exif[5] == '\0'))
		return ;
	length -= 6;
	StartParsing(file, collector, length);
}

void	Jpg::UpdateData(std::ifstream &file)
{
	unsigned short		collector;
	unsigned short		length;
	unsigned short		height;
	unsigned short		width;
	char				precision_byte;
	char				comp_byte;
	int					precision;
	int					components;
	std::streampos		block_start;
	std::stringstream	ss;

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
			
			file.read(&precision_byte, 1);
			precision = (unsigned char)precision_byte;
			
			height = read_u16(file);
			width = read_u16(file);
			
			file.read(&comp_byte, 1);
			components = (unsigned char)comp_byte;

			ss << height; this->data["Height"] = ss.str(); ss.str("");
			ss << width; this->data["Width"] = ss.str(); ss.str("");
			ss << precision << " bits"; this->data["Bit Depth"] = ss.str(); ss.str("");
			ss << components; this->data["Components"] = ss.str(); ss.str("");

			if (components == 1) this->data["Color Space"] = "Grayscale";
			else if (components == 3) this->data["Color Space"] = "YCbCr (Standard)";
			else if (components == 4) this->data["Color Space"] = "CMYK";

			if (collector == 0xFFC0)
				this->data["Compression"] = "Baseline (Standard)";
			else
				this->data["Compression"] = "Progressive";

			file.seekg(length - 8, std::ios::cur);
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

void	Jpg::parse()
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
			UpdateData(file);
		else
			std::cout << "[x] " << file_name << " is not a valid JPEG or JPG." << std::endl;
	}
	file.close();
}