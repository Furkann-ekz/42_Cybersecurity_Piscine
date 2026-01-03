#include "AllClass.hpp"

AllClasses::AllClasses(std::string f_image)
{
	file_name = f_image;
	control = false;
	little_endian = false;
}

AllClasses::~AllClasses()
{}

void	AllClasses::set_filename(std::string f_name)
{
	file_name = f_name;
}

std::string	AllClasses::get_filename()
{
	return (file_name);
}

void	AllClasses::display_info()
{
	std::map<std::string, std::string>::iterator iterator;
	if (control == false)
		return ;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "File: " << file_name << std::endl;
}

unsigned short	AllClasses::read_u16(std::ifstream &file)
{
	unsigned char	buffer[2];

	if (!file.read((char *)buffer, 2))
		return (0);

	if (this->little_endian)
		return ((buffer[1] << 8) | buffer[0]);

	return ((buffer[0] << 8) | buffer[1]);
}

unsigned int	AllClasses::read_u32(std::ifstream &file)
{
	unsigned char	buffer[4];

	if (!file.read((char *)buffer, 4))
		return (0);

	if (this->little_endian)
		return ((buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0]);

	return ((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3]);
}

void	AllClasses::print_data()
{
	if (this->data.empty())
	{
		std::cout << ">> Herhangi bir veri bulunamadi." << std::endl;
		return ;
	}
	std::map<std::string, std::string>::iterator it = this->data.begin();
	
	while (it != this->data.end())
	{
		std::cout << "[" << it->first << "] : " << it->second << std::endl;
		it++;
	}
}
