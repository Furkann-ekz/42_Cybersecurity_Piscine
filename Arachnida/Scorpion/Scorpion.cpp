#include "Scorpion.hpp"

static std::string	get_extension(std::string f_name)
{
	size_t		i;

	i = f_name.rfind('.');
	if (i != std::string::npos)
		return (f_name.substr(i));
	return ("");
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
	size_t		i;
	std::string	extension;
	std::string	current_file;

	i = 0;
	while (i < files.size())
	{
		extension = to_lower(get_extension(files[i]));
		if (extension == ".jpg" || extension == ".jpeg")
			classes.push_back(new JpegJpg(files[i]));
		else if (extension == ".bmp")
			classes.push_back(new Bmp(files[i]));
		else if (extension == ".gif")
			classes.push_back(new Gif(files[i]));
		else if (extension == ".png")
			classes.push_back(new Png(files[i]));
		else if (extension == ".tiff")
			classes.push_back(new TiffTif(files[i]));
		else if (extension == ".webp")
			classes.push_back(new Webp(files[i]));
		else if (extension == ".svg")
			classes.push_back(new Svg(files[i]));
		else
			std::cout << "Warning: Unknown format file: " << files[i] << std::endl;
		i++;
	}
}

Scorpion::Scorpion(char **av)
{
	size_t	i;

	i = 0;
	while (av[i])
	{
		files.push_back(av[i]);
		i++;
	}
	SetClasses();
}

Scorpion::~Scorpion()
{
	size_t	i;

	i = 0;
	while (i < classes.size())
	{
		delete classes[i];
		i++;
	}
	classes.clear();
}

void	Scorpion::Run(void)
{
	size_t	i;

	i = 0;
	while (i < classes.size())
	{
		classes[i]->parse();
		classes[i]->display_info();
		i++;
	}
}
