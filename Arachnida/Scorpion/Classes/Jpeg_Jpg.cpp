#include "Jpeg_Jpg.hpp"

Jpeg_Jpg::Jpeg_Jpg(std::string file_name) : AllClasses(file_name) {}

Jpeg_Jpg::~Jpeg_Jpg() {}

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
	file.close();
}
