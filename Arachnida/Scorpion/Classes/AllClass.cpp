#include "AllClass.hpp"

AllClasses::AllClasses(std::string f_image)
{
	file_name = f_image;
	control = false;
}

AllClasses::~AllClasses()
{}

void	AllClasses::set_filename(std::string f_name)
{
	file_name = f_name;
}

std::string AllClasses::get_filename()
{
	return (file_name);
}

template <typename T>
std::string AllClasses::to_string(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}
