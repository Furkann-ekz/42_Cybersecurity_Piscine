#include "Png.hpp"

Png::Png(std::string file_name) : AllClasses(file_name)
{
	this->little_endian = false;
}

Png::~Png() {}

void	Png::HandlePhys(std::ifstream &file, unsigned int length)
{
	unsigned int		ppu_x;
	unsigned int		ppu_y;
	unsigned char		unit;
	std::stringstream	ss;
	double				dpi_x;

	if (length != 9)
	{
		SkipChunk(file, length);
		return ;
	}
	ppu_x = read_u32(file);
	ppu_y = read_u32(file);
	if (!file.read((char *)&unit, 1))
		return ;
	
	if (unit == 1 && ppu_x > 0)
	{
		dpi_x = (double)ppu_x * 0.0254;
		ss << (int)dpi_x << " DPI";
		this->data["Resolution"] = ss.str();
	}
	file.seekg(4, std::ios::cur);
}

void	Png::HandleText(std::ifstream &file, unsigned int length)
{
	char		*buffer;
	std::string	raw_data;
	std::string	key;
	std::string	val;
	size_t		null_pos;

	buffer = new char[length + 1];
	if (!buffer)
	{
		SkipChunk(file, length);
		return ;
	}
	if (file.read(buffer, length))
	{
		buffer[length] = '\0';
		raw_data = std::string(buffer, length);
		
		null_pos = raw_data.find('\0');
		if (null_pos != std::string::npos)
		{
			key = raw_data.substr(0, null_pos);
			if (null_pos + 1 < length)
				val = raw_data.substr(null_pos + 1);

			if (!key.empty() && !val.empty())
				this->data[key] = val;
		}
	}
	delete[] buffer;
	file.seekg(4, std::ios::cur);
}

void	Png::HandleTime(std::ifstream &file, unsigned int length)
{
	unsigned short		year;
	unsigned char		month, day, hour, minute, second;
	std::stringstream	ss;
	char				buf[5];

	if (length != 7)
	{
		SkipChunk(file, length);
		return ;
	}
	year = read_u16(file);
	file.read(buf, 5);
	month = (unsigned char)buf[0];
	day = (unsigned char)buf[1];
	hour = (unsigned char)buf[2];
	minute = (unsigned char)buf[3];
	second = (unsigned char)buf[4];

	ss << year << ":" << (int)month << ":" << (int)day << " "
	   << (int)hour << ":" << (int)minute << ":" << (int)second;
	this->data["Last Modification"] = ss.str();
	
	file.seekg(4, std::ios::cur);
}

void	Png::SkipChunk(std::ifstream &file, unsigned int length)
{
	file.seekg(length + 4, std::ios::cur);
}

void	Png::ProcessRemainingChunks(std::ifstream &file)
{
	unsigned int	length;
	char			type_buf[5];
	std::string		type;

	while (true)
	{
		length = read_u32(file);
		if (file.fail())
			break;
		
		if (!file.read(type_buf, 4))
			break;
		type_buf[4] = '\0';
		type = type_buf;

		if (type == "IEND")
			break ;
		else if (type == "tEXt")
			HandleText(file, length);
		else if (type == "pHYs")
			HandlePhys(file, length);
		else if (type == "tIME")
			HandleTime(file, length);
		else
			SkipChunk(file, length);
	}
}

void	Png::ParseIHDR_Body(std::ifstream &file)
{
	unsigned char		buffer[5];
	std::stringstream	ss;
	std::string			color_type;

	if (!file.read((char *)buffer, 5))
		return ;
	ss << (int)buffer[0];
	this->data["Bit Depth"] = ss.str();
	ss.str("");
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
	
	this->data["Compression"] = (buffer[2] == 0) ? "Deflate" : "Unknown";
	this->data["Filter Method"] = (buffer[3] == 0) ? "Adaptive" : "Unknown";
	
	if (buffer[4] == 0) this->data["Interlace"] = "No Interlace";
	else if (buffer[4] == 1) this->data["Interlace"] = "Adam7 (Progressive)";
	else this->data["Interlace"] = "None";

	file.seekg(4, std::ios::cur);
}

void	Png::ParseIHDR(std::ifstream &file, unsigned int &collector)
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
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");

	collector = read_u32(file);
	ss << collector;
	this->data["Height"] = ss.str();

	ParseIHDR_Body(file);

	ProcessRemainingChunks(file);
}

void	Png::parse()
{
	unsigned char	buffer[8];
	unsigned int	length;
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
			length = read_u32(file);
			if (file.fail()) return ;
			ParseIHDR(file, length);
		}
		else
			std::cout << "[x] " << file_name << " is not a valid PNG." << std::endl;
	}
	file.close();
}