#include "Bmp.hpp"

Bmp::Bmp(std::string file_name) : AllClasses(file_name)
{
	this->little_endian = true;
	this->UpDown = false;
}

Bmp::~Bmp() {}

void	Bmp::GetCompression(std::ifstream &file)
{
	unsigned int	compression;
	std::string		comp_str;

	compression = read_u32(file);
	switch (compression)
	{
		case 0:
			comp_str = "BI_RGB (Uncompressed)";
			break;
		case 1:
			comp_str = "BI_RLE8 (8-bit RLE)";
			break;
		case 2:
			comp_str = "BI_RLE4 (4-bit RLE)";
			break;
		case 3:
			comp_str = "BI_BITFIELDS (Raw)";
			break;
		case 4:
			comp_str = "BI_JPEG (Embedded JPEG)";
			break;
		case 5:
			comp_str = "BI_PNG (Embedded PNG)";
			break;
		case 6:
			comp_str = "BI_ALPHABITFIELDS (RGBA)";
			break;
		default:
			comp_str = "Unknown";
	}
	this->data["Compression"] = comp_str;
}

void	Bmp::ParseContinue(std::ifstream &file)
{
	int					width;
	int					height;
	int					x_ppm;
	int					y_ppm;
	unsigned short		planes;
	unsigned short		bpp;
	unsigned int		image_size;
	std::stringstream	ss;

	file.seekg(18, std::ios::beg);
	width = read_u32(file);
	if (file.fail())
		return ;
	height = read_u32(file);
	if (file.fail())
		return ;

	if (height < 0)
	{
		height *= -1;
		this->data["Scan Direction"] = "Top-Down (Standard)";
	}
	else
		this->data["Scan Direction"] = "Bottom-Up (Reversed)";

	ss << width;
	this->data["Width"] = ss.str();
	ss.str("");
	ss << height;
	this->data["Height"] = ss.str();
	ss.str("");

	planes = read_u16(file);
	ss << planes;
	this->data["Planes"] = ss.str();
	ss.str("");

	bpp = read_u16(file);
	ss << bpp << " bits";
	this->data["Bits Per Pixel"] = ss.str();
	ss.str("");

	GetCompression(file);

	image_size = read_u32(file);
	ss << image_size << " bytes";
	this->data["Raw Image Size"] = ss.str();

	x_ppm = read_u32(file);
	y_ppm = read_u32(file);
	if (x_ppm > 0 && y_ppm > 0)
	{
		ss.str("");
		ss << (int)(x_ppm / 39.37) << " DPI";
		this->data["Resolution"] = ss.str();
	}
}

void	Bmp::parse()
{
	char			buffer[2];
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 2))
	{
		if (buffer[0] == 'B' && buffer[1] == 'M')
			ParseContinue(file);
		else
			std::cout << "[x] " << file_name << " is not a valid BMP." << std::endl;
	}
	file.close();
}
