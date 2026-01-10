#include "Webp.hpp"

Webp::Webp(std::string file_name) : AllClasses(file_name)
{
	little_endian = true;
}

Webp::~Webp() {}

unsigned int	Webp::read_u24(std::ifstream &file)
{
	unsigned char	buffer[3];
	unsigned int	value;

	if (!file.read((char *)buffer, 3))
		return (0);

	value = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16);
	return (value);
}

void	Webp::HandleVP8XFlags(std::ifstream &file)
{
	char			buffer;
	unsigned char	flags;

	if (!file.read(&buffer, 1))
		return ;

	flags = (unsigned char)buffer;

	if (flags & 0x20)
		this->data["ICC Profile"] = "Present";

	if (flags & 0x10)
		this->data["Alpha Channel"] = "Present (Transparency)";
	else
		this->data["Alpha Channel"] = "Not Present";

	if (flags & 0x08)
		this->data["EXIF Metadata"] = "Present";

	if (flags & 0x04)
		this->data["XMP Metadata"] = "Present";

	if (flags & 0x02)
		this->data["Animation"] = "Yes (Animated WebP)";
	else
		this->data["Animation"] = "No";
}

void	Webp::ParseVP8X(std::ifstream &file, unsigned int &collector)
{
	std::stringstream	ss;

	HandleVP8XFlags(file);
	file.seekg(3, std::ios::cur);
	collector = read_u24(file) + 1;
	if (file.fail())
		return ;
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");
	collector = read_u24(file) + 1;
	if (file.fail())
		return ;
	ss << collector;
	this->data["Height"] = ss.str();
}

void	Webp::ParseVP8(std::ifstream &file, unsigned int &collector)
{
	unsigned char		buffer[3];
	std::stringstream	ss;

	if (!file.read((char *)buffer, 3))
		return ;
	if ((buffer[0] & 1) != 0)
	{
		this->data["Frame Type"] = "Interframe (No Size Info)";
		return ;
	}
	this->data["Frame Type"] = "Keyframe";
	if (!file.read((char *)buffer, 3))
		return ;
	if (buffer[0] != 0x9D || buffer[1] != 0x01 || buffer[2] != 0x2A)
	{
		std::cout << "[x] Invalid VP8 Start Code" << std::endl;
		return ;
	}
	collector = read_u16(file) & 0x3fff;
	if (file.fail())
		return ;
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");
	collector = read_u16(file) & 0x3fff;
	if (file.fail())
		return ;
	ss << collector;
	this->data["Height"] = ss.str();
}

void	Webp::ParseVP8L(std::ifstream &file, unsigned int &collector)
{
	bool				has_alpha;
	char				buff;
	std::stringstream	ss;

	if (!file.read(&buff, 1))
		return ;
	if (buff != 0x2F)
		return ;
	collector = read_u32(file);
	if (file.fail())
		return ;
	ss << (collector & 0x3FFF) + 1;
	this->data["Width"] = ss.str();
	ss.str("");
	ss << ((collector >> 14) & 0x3FFF) + 1;
	this->data["Height"] = ss.str();
	has_alpha = ((collector >> 28) & 1);
	if (has_alpha)
		this->data["Alpha Channel"] = "Present";
	else
		this->data["Alpha Channel"] = "Not Present";
}

void	Webp::parse_continue(std::ifstream &file, unsigned int &collector, char *buffer)
{
	std::stringstream	ss;
	std::string			string_buffer;

	ss << collector;
	this->data["File Size"] = ss.str();
	if (!file.read(buffer, 4))
		return ;
	string_buffer = std::string(buffer, 4);
	this->data["Format Type"] = string_buffer;
	collector = read_u32(file);
	if (file.fail())
		return ;
	if (string_buffer == "VP8X")
		ParseVP8X(file, collector);
	else if (string_buffer == "VP8 ")
		ParseVP8(file, collector);
	else if (string_buffer == "VP8L")
		ParseVP8L(file, collector);
	else
		std::cout << "[x] Unknown WebP Format: " << string_buffer << std::endl;
}

void	Webp::parse()
{
	char			buffer[4];
	unsigned int	collector;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 4))
	{
		if ((buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F'))
		{
			collector = read_u32(file);
			if (file.fail())
				return (file.close());
			if (file.read(buffer, 4))
			{
				if (buffer[0] == 'W' && buffer[1] == 'E' && buffer[2] == 'B' && buffer[3] == 'P')
					parse_continue(file, collector, buffer);
			}
			else
				std::cout << "[x] " << file_name << " is not a valid WebP." << std::endl;
		}
		else
			std::cout << "[x] " << file_name << " is not a valid WebP." << std::endl;
	}
	file.close();
}
