#include "Gif.hpp"

Gif::Gif(std::string file_name) : AllClasses(file_name)
{
	little_endian = true;
}

Gif::~Gif() {}

void	Gif::HandlePackedField(std::ifstream &file)
{
	char				buffer;
	unsigned char		packed;
	int					color_res;
	int					table_size;
	std::stringstream	ss;

	if (!file.read(&buffer, 1))
		return ;

	packed = static_cast<unsigned char>(buffer);

	if (packed & 0x80)
		this->data["Global Color Table"] = "Present";
	else
		this->data["Global Color Table"] = "Not Present";

	color_res = ((packed >> 4) & 0x07) + 1;
	ss << color_res << " bits";
	this->data["Color Resolution"] = ss.str();
	
	ss.str("");
	ss.clear();

	if (packed & 0x08)
		this->data["Sort Flag"] = "Sorted";
	else
		this->data["Sort Flag"] = "Not Sorted";

	table_size = 1 << ((packed & 0x07) + 1);
	ss << table_size << " Colors";
	this->data["GCT Size"] = ss.str();
}

void	Gif::parse_continue(std::ifstream &file, unsigned short collector)
{
	std::stringstream	ss;

	collector = read_u16(file);
	if (file.fail())
		return ;
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");
	ss.clear();
	collector = read_u16(file);
	if (file.fail())
		return ;
	ss << collector;
	this->data["Height"] = ss.str();
	HandlePackedField(file);
}

void	Gif::parse()
{
	char			buffer[6];
	unsigned short	collector;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 6))
	{
		if (!(buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F'
			&& buffer[3] == '8' && (buffer[4] == '7' || buffer[4] == '9') && buffer[5] == 'a'))
		{
			std::cout << "[x] " << file_name << " is not a valid GIF." << std::endl;
			return ;
		}
		this->data["Version"] = std::string(buffer, 6);
	}
	parse_continue(file, collector);
	file.close();
}
