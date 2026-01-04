#include "Bmp.hpp"

Bmp::Bmp(std::string file_name) : AllClasses(file_name)
{
	little_endian = true;
	UpDown = false;
}

Bmp::~Bmp() {}

void	Bmp::ParseContinue(std::ifstream &file)
{
	int					width;
	int					height;
	std::stringstream	ss;

	file.seekg(18, std::ios::beg);
	width = read_u32(file);
	if (file.fail())
		return ;
	height = read_u32(file);
	if (file.fail())
		return ;
	if (height > 0)
		UpDown = true;
	else
		UpDown = false;
	ss << width;
	this->data["Width"] = ss.str();
	ss.clear();
	ss.str("");
	if (height < 0)
		height *= -1;
	ss << height;
	this->data["Height"] = ss.str();
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
