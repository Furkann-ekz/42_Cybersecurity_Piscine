#include "Scorpion.hpp"

static std::string	get_extension(std::string f_name)
{
	size_t	i;

	i = f_name.rfind('.');
	if (i != std::string::npos)
		return (f_name.substr(i));
	return ("");
}

Scorpion::Scorpion(char **av)
{
	size_t	i;

	i = 0;
	while (av[i])
	{
		this->files.push_back(av[i]);
		i++;
	}
	this->SetClasses();
}

Scorpion::~Scorpion()
{
	size_t	i;

	i = 0;
	while (i < this->classes.size())
	{
		delete this->classes[i];
		i++;
	}
	this->classes.clear();
}

std::string	Scorpion::to_lower(std::string str)
{
	size_t	i;

	i = 0;
	while (i < str.length())
	{
		str[i] = std::tolower(str[i]);
		i++;
	}
	return (str);
}

void	Scorpion::SetClasses(void)
{
	std::string	extension;
	size_t		i;

	i = 0;
	while (i < this->files.size())
	{
		extension = this->to_lower(get_extension(this->files[i]));
		
		if (extension == ".jpg" || extension == ".jpeg")
			this->classes.push_back(new Jpg(this->files[i]));
		else if (extension == ".bmp")
			this->classes.push_back(new Bmp(this->files[i]));
		else if (extension == ".gif")
			this->classes.push_back(new Gif(this->files[i]));
		else if (extension == ".png")
			this->classes.push_back(new Png(this->files[i]));
		else if (extension == ".tiff" || extension == ".tif")
			this->classes.push_back(new Tif(this->files[i]));
		else if (extension == ".webp")
			this->classes.push_back(new Webp(this->files[i]));
		else if (extension == ".svg")
			this->classes.push_back(new Svg(this->files[i]));
		else
			std::cout << "Warning: Unknown format file: " << this->files[i] << std::endl;
		i++;
	}
}

void	Scorpion::Run(void)
{
	size_t	i;

	i = 0;
	while (i < this->classes.size())
	{
		this->classes[i]->parse();
		this->classes[i]->display_info();
		this->classes[i]->print_data();
		i++;
	}
}