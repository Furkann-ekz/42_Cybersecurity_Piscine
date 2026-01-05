#include "Png.hpp"

Png::Png(std::string file_name) : AllClasses(file_name) {}

Png::~Png() {}

void	Png::Continue(std::ifstream &file)
{
	unsigned char		buffer[5];
	std::stringstream	ss;
	std::string			color_type;

	if (!file.read((char *)buffer, 5))
		return ;
	ss << (int)buffer[0];
	this->data["Bit Depth"] = ss.str();
	ss.str("");
	ss.clear();
	switch ((int)buffer[1])
	{
		case 0:
			color_type = "Grayscale";
			break ;
		case 2:
			color_type = "Truecolor (RGB)";
			break ;
		case 3:
			color_type = "Indexed";
			break ;
		case 4:
			color_type = "Grayscale + Alpha";
			break ;
		case 6:
			color_type = "Truecolor + Alpha (RGBA)";
			break ;
		default:
			color_type = "Unknown";
	}
	this->data["Color Type"] = color_type;
	ss.str(""); ss.clear();
	ss << (int)buffer[2];
	this->data["Compression"] = (buffer[2] == 0) ? "Deflate" : "Unknown";
	this->data["Filter Method"] = (buffer[3] == 0) ? "Adaptive" : "Unknown";
	if (buffer[4] == 0)
		this->data["Interlace"] = "No Interlace";
	else if (buffer[4] == 1)
		this->data["Interlace"] = "Adam7 (Progressive)";
	else
		this->data["Interlace"] = "None";
}

void	Png::ParseStarting(std::ifstream &file, unsigned int &collector)
{
	std::stringstream	ss;
	char				buffer[4];

	if (!file.read(buffer, 4))
		return ;
	if (buffer[0] != 'I' || buffer[1] != 'H' || buffer[2] != 'D' || buffer[3] != 'R')
	{
		std::cout << "[x] Invalid PNG: First chunk must be IHDR." << std::endl;
		return ;
	}
	if (collector != 13)
	{
		std::cout << "[x] Invalid PNG: IHDR length must be 13." << std::endl;
		return ;
	}
	collector = read_u32(file);
	if (file.fail())
		return ;
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");
	ss.clear();
	collector = read_u32(file);
	if (file.fail())
		return ;
	ss << collector;
	this->data["Height"] = ss.str();
	Continue(file);
}

void	Png::parse()
{
	unsigned char	buffer[8];
	unsigned int		collector;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read((char *)buffer, 8))
	{
		if (buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4E && buffer[3] == 0x47
			&& buffer[4] == 0x0D && buffer[5] == 0x0A && buffer[6] == 0x1A && buffer[7] == 0x0A)
		{
			collector = read_u32(file);
			if (file.fail())
				return ;
			ParseStarting(file, collector);
		}
		else
			std::cout << "[x] " << file_name << " is not a valid PNG." << std::endl;
	}
	file.close();
}
