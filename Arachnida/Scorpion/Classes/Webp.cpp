#include "Webp.hpp"

Webp::Webp(std::string file_name) : AllClasses(file_name)
{
	this->little_endian = true;
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
		return ;
	
	collector = read_u16(file) & 0x3fff;
	if (file.fail()) return ;
	ss << collector;
	this->data["Width"] = ss.str();
	ss.str("");
	
	collector = read_u16(file) & 0x3fff;
	if (file.fail()) return ;
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

void	Webp::ParseEXIF(std::ifstream &file, unsigned int len)
{
	char			header[6];
	char			tiff_head[8];
	unsigned short	dummy_collector;
	unsigned short	exif_len;
	unsigned int	ifd_offset;

	if (len < 6)
	{
		file.seekg(len, std::ios::cur);
		return ;
	}
	
	file.read(header, 6);
	if (header[0] == 'E' && header[1] == 'x' && header[2] == 'i' && header[3] == 'f')
	{
		this->tiff_start = file.tellg();

		file.read(tiff_head, 8);
		if (tiff_head[0] == 'I' && tiff_head[0] == 'I')
			this->little_endian = true;
		else
			this->little_endian = false;
		
		if (this->little_endian)
			 ifd_offset = (unsigned char)tiff_head[4] | ((unsigned char)tiff_head[5] << 8) | ((unsigned char)tiff_head[6] << 16) | ((unsigned char)tiff_head[7] << 24);
		else
			 ifd_offset = (unsigned char)tiff_head[7] | ((unsigned char)tiff_head[6] << 8) | ((unsigned char)tiff_head[5] << 16) | ((unsigned char)tiff_head[4] << 24);

		if (ifd_offset >= 8)
			file.seekg(ifd_offset - 8, std::ios::cur);
		
		dummy_collector = 0;
		if (len > 14)
		{
			exif_len = (unsigned short)(len - 14); 
			Process_IFD(file, dummy_collector, exif_len);
		}
	}
	else
		file.seekg(len - 6, std::ios::cur);
}

void	Webp::ProcessChunks(std::ifstream &file)
{
	char			chunk_tag[5];
	unsigned int	chunk_size;
	std::string		tag;

	while (file.read(chunk_tag, 4))
	{
		chunk_tag[4] = '\0';
		tag = chunk_tag;
		
		chunk_size = read_u32(file);
		if (file.fail())
			break;

		if (tag == "EXIF")
		{
			ParseEXIF(file, chunk_size);
			if (chunk_size % 2 != 0)
				file.seekg(1, std::ios::cur);
		}
		else
		{
			file.seekg(chunk_size, std::ios::cur);
			if (chunk_size % 2 != 0)
				file.seekg(1, std::ios::cur);
		}
	}
}

void	Webp::parse_continue(std::ifstream &file, unsigned int collector, char *buffer)
{
	std::string		format_type;
	unsigned int	chunk_size;

	(void)collector;
	
	format_type = std::string(buffer, 4);
	this->data["Format Type"] = format_type;

	chunk_size = read_u32(file);
	if (file.fail())
		return;

	if (format_type == "VP8X")
		ParseVP8X(file, chunk_size);
	else if (format_type == "VP8 ")
		ParseVP8(file, chunk_size);
	else if (format_type == "VP8L")
		ParseVP8L(file, chunk_size);
	else
		file.seekg(chunk_size, std::ios::cur);

	ProcessChunks(file);
}

void	Webp::parse()
{
	char			buffer[5];
	unsigned int	riff_size;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}

	if (file.read(buffer, 4))
	{
		if (buffer[0] == 'R' && buffer[1] == 'I' && buffer[2] == 'F' && buffer[3] == 'F')
		{
			riff_size = read_u32(file);
			
			if (file.read(buffer, 4))
			{
				if (buffer[0] == 'W' && buffer[1] == 'E' && buffer[2] == 'B' && buffer[3] == 'P')
				{
					if (file.read(buffer, 4))
						parse_continue(file, riff_size, buffer);
				}
				else
					std::cout << "[x] " << file_name << " is not a valid WebP." << std::endl;
			}
		}
		else
			std::cout << "[x] " << file_name << " is not a valid WebP." << std::endl;
	}
	file.close();
}
