#include "Svg.hpp"

Svg::Svg(std::string file_name) : AllClasses(file_name) {}

Svg::~Svg() {}

std::string	Svg::get_attribute(std::string key)
{
	std::string	target;
	size_t		pos = 0;
	size_t		quote_start;
	size_t		quote_end;
	char		quote_type;

	target = key + "=";
	while ((pos = this->text.find(target, pos)) != std::string::npos)
	{
		if (pos > 0 && !isspace(this->text[pos - 1]) && this->text[pos - 1] != ',')
		{
			pos += target.length();
			continue ;
		}

		quote_start = this->text.find_first_of("\"'", pos + target.length());

		if (quote_start == std::string::npos)
			return ("");

		quote_type = this->text[quote_start];
		quote_end = this->text.find(quote_type, quote_start + 1);

		if (quote_end == std::string::npos)
			return ("");

		return (this->text.substr(quote_start + 1, quote_end - quote_start - 1));
	}
	return ("");
}

std::string Svg::get_element_content(std::string tag)
{
	std::string open_tag = "<" + tag + ">";
	std::string close_tag = "</" + tag + ">";

	size_t start = this->text.find(open_tag);
	if (start == std::string::npos)
		return ("");
	
	start += open_tag.length();

	size_t end = this->text.find(close_tag, start);
	if (end == std::string::npos)
		return ("");

	return (this->text.substr(start, end - start));
}

void	Svg::parse_continue()
{
	std::string			width;
	std::string			height;
	std::string			viewBox;
	std::string			trash;
	std::string			target;
	std::stringstream	ss;

	width = get_attribute("width");
	height = get_attribute("height");
	if (!width.empty() && !height.empty())
	{
		this->data["Width"] = width;
		this->data["Height"] = height;
		return ;
	}
	viewBox = get_attribute("viewBox");
	if (!viewBox.empty())
	{
		ss << viewBox;
		ss >> trash >> trash;
		ss >> target;
		this->data["Width"] = target;
		ss >> target;
		this->data["Height"] = target;
		this->data["Info"] = "Extracted from viewBox";
	}
	else
		std::cout << "[x] No dimensions found in SVG." << std::endl;
	target = get_element_content("title");
    if (!target.empty())
        this->data["Title"] = target;

    target = get_element_content("desc");
    if (!target.empty())
        this->data["Description"] = target;

    target = get_attribute("version");
    if (!target.empty())
        this->data["Version"] = target;
    else
        this->data["Version"] = "1.1 (Default)";
}

void	Svg::parse()
{
	std::string		line;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	while (std::getline(file, line))
		this->text += line + "\n";
	parse_continue();
	file.close();
}
