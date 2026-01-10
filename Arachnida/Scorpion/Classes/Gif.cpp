#include "Gif.hpp"

Gif::Gif(std::string file_name) : AllClasses(file_name)
{
	this->little_endian = true;
}

Gif::~Gif() {}

void	Gif::HandlePackedField(unsigned char packed)
{
	int					color_res;
	int					table_size;
	std::stringstream	ss;

	if (packed & 0x80)
		this->data["Global Color Table"] = "Present";
	else
		this->data["Global Color Table"] = "Not Present";

	color_res = ((packed >> 4) & 0x07) + 1;
	ss << color_res << " bits";
	this->data["Color Resolution"] = ss.str();
	ss.str("");

	if (packed & 0x08)
		this->data["Sort Flag"] = "Sorted";
	else
		this->data["Sort Flag"] = "Not Sorted";

	table_size = 1 << ((packed & 0x07) + 1);
	ss << table_size << " Colors";
	this->data["GCT Size"] = ss.str();
}

void	Gif::ReadComment(std::ifstream &file)
{
	unsigned char	block_size;
	char			*buffer;
	std::string		comment;

	while (true)
	{
		file.read((char *)&block_size, 1);
		if (file.fail() || block_size == 0)
			break ;
		buffer = new char[block_size + 1];
		if (buffer)
		{
			file.read(buffer, block_size);
			buffer[block_size] = '\0';
			comment += buffer;
			delete[] buffer;
		}
		else
			file.seekg(block_size, std::ios::cur);
	}
	if (!comment.empty())
		this->data["Comment"] = comment;
}

void    Gif::ReadGraphicControl(std::ifstream &file)
{
	unsigned char		block_size;
	unsigned short		delay;
	unsigned char		packed;
	unsigned char		trans_index;
	std::stringstream	ss;

	file.read((char *)&block_size, 1);
	if (file.fail())
		return ;

	file.read((char *)&packed, 1);
	
	delay = read_u16(file);
	if (delay > 0)
	{
		ss << delay * 10 << " ms";
		this->data["Frame Delay"] = ss.str();
	}

	file.read((char *)&trans_index, 1);

	if (packed & 0x01)
	{
		ss.str("");
		ss << (int)trans_index;
		this->data["Transparent Index"] = ss.str();
	}

	file.seekg(1, std::ios::cur); 
}

void	Gif::ReadApplication(std::ifstream &file)
{
	unsigned short		loops;
	unsigned char		block_size;
	char				app_id[12];
	char				sub_block_size;
	std::stringstream	ss;

	file.read((char *)&block_size, 1);
	if (block_size == 11)
	{
		file.read(app_id, 11);
		app_id[11] = '\0';
		this->data["Application"] = std::string(app_id);

		if (std::string(app_id) == "NETSCAPE2.0")
		{
			file.read(&sub_block_size, 1);
			file.seekg(1, std::ios::cur);
			loops = read_u16(file);
			
			if (loops == 0)
				ss << "Infinite";
			else
				ss << loops;
			this->data["Loop Count"] = ss.str();
			file.seekg(1, std::ios::cur);
		}
		else
			SkipBlock(file);
	}
	else
	{
		file.seekg(block_size, std::ios::cur);
		SkipBlock(file);
	}
}

void	Gif::SkipBlock(std::ifstream &file)
{
	unsigned char	size;

	while (true)
	{
		file.read((char *)&size, 1);
		if (file.fail() || size == 0)
			break ;
		file.seekg(size, std::ios::cur);
	}
}

void	Gif::HandleExtension(std::ifstream &file)
{
	unsigned char	label;

	if (!file.read((char *)&label, 1))
		return ;

	switch (label)
	{
		case 0xF9:
			ReadGraphicControl(file);
			break ;
		case 0xFE:
			ReadComment(file);
			break ;
		case 0xFF:
			ReadApplication(file);
			break ;
		default:
			SkipBlock(file);
	}
}

void	Gif::ProcessBlocks(std::ifstream &file)
{
	unsigned char	separator;
	unsigned char	packed;
	int				size;

	while (file.read((char *)&separator, 1))
	{
		if (separator == 0x3B)
			break ;
		else if (separator == 0x21)
			HandleExtension(file);
		else if (separator == 0x2C)
		{
			file.seekg(8, std::ios::cur);
			
			file.read((char *)&packed, 1);
			if (packed & 0x80)
			{
				size = 1 << ((packed & 0x07) + 1);
				file.seekg(3 * size, std::ios::cur);
			}
			file.seekg(1, std::ios::cur);
			SkipBlock(file);
		}
	}
}

void	Gif::parse_continue(std::ifstream &file)
{
	std::stringstream	ss;
	unsigned short		width;
	unsigned short		height;
	char				packed_byte;
	char				bg_index;
	char				aspect_byte;
	int					size;

	width = read_u16(file);
	height = read_u16(file);
	
	ss << width; this->data["Width"] = ss.str(); ss.str("");
	ss << height; this->data["Height"] = ss.str(); ss.str("");

	file.read(&packed_byte, 1);
	HandlePackedField((unsigned char)packed_byte);

	file.read(&bg_index, 1);
	file.read(&aspect_byte, 1);

	if (packed_byte & 0x80)
	{
		size = 1 << ((packed_byte & 0x07) + 1);
		file.seekg(3 * size, std::ios::cur);
	}
	ProcessBlocks(file);
}

void	Gif::parse()
{
	char			buffer[6];
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 6))
	{
		if (buffer[0] == 'G' && buffer[1] == 'I' && buffer[2] == 'F')
		{
			buffer[6] = '\0';
			this->data["Version"] = std::string(buffer);
			parse_continue(file);
		}
		else
			std::cout << "[x] " << file_name << " is not a valid GIF." << std::endl; 
	}
	file.close();
}
