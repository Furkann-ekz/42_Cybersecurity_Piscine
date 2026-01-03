#include "Jpeg_Jpg.hpp"

Jpeg_Jpg::Jpeg_Jpg(std::string file_name) : AllClasses(file_name) {}

Jpeg_Jpg::~Jpeg_Jpg() {}

int	Jpeg_Jpg::GetTypeSize(std::ifstream &file)
{
	unsigned short	collector;
	if ((collector = read_u16(file)) == 0)
		return (0);
	switch (collector)
	{
		case 1:
			return (1); // BYTE
		case 2:
			return (1); // ASCII
		case 3:
			return (2); // SHORT
		case 4:
			return (4); // LONG
		case 5:
			return (8); // RATIONAL
		case 6:
			return (1); // SBYTE (Çok nadir)
		case 7:
			return (1); // UNDEFINED (Exif'te sıkça çıkar)
		case 8:
			return (2); // SSHORT
		case 9:
			return (4); // SLONG
		case 10:
			return (8); // SRATIONAL (Pozitif/Negatif kesirli)
		case 11:
			return (4); // FLOAT
		case 12:
			return (8); // DOUBLE
		default:
			return (0); // Bilinmeyen tip
	}
}

void	Jpeg_Jpg::HandleStringData(std::ifstream &file, unsigned short &collector)
{
	unsigned int	type_size;
	unsigned int	count;
	unsigned int	offset;
	std::streampos	base;
	std::string		result_value;

	type_size = GetTypeSize(file);
	count = read_u32(file);
	if (file.fail())
		return ;
	count *= type_size;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (count == 0)
		return ;
	if (count > 4)
	{
		char	*buffer;
		
		base = file.tellg();
		file.seekg(this->tiff_start + (std::streampos)offset, std::ios::beg);
		buffer = new char[count + 1];
		if (!buffer)
		{
			file.seekg(base);
			return ;
		}
		if (file.read(buffer, count))
		{
			buffer[count] = '\0';
			result_value = buffer;
		}
		delete[] buffer;
		file.seekg(base);
	}
	else
	{
		char	buffer[5];

		file.seekg(-4, std::ios::cur);
		if (file.read(buffer, count))
		{
			buffer[count] = '\0';
			result_value = buffer;
		}
		file.seekg(4 - count, std::ios::cur);
	}
	if (!result_value.empty())
	{
		if (collector == 0x010F)
			this->data["Camera Make"] = result_value;
		else if (collector == 0x0110)
			this->data["Camera Model"] = result_value;
		else if (collector == 0x0132)
			this->data["Date/Time"] = result_value;
	}
}

void	Jpeg_Jpg::HandleExifSubAndGPS(std::ifstream &file)
{
	unsigned short	temp_collector;
	unsigned short	temp_length;
	unsigned int	type_size;
	unsigned int	count;
	unsigned int	offset;
	std::streampos	base;

	temp_collector = 0;
	temp_length = 0;
	type_size = GetTypeSize(file);
	count = read_u32(file);
	if (file.fail())
		return ;
	count *= type_size;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (count == 0)
		return ;
	base = file.tellg();
	file.seekg(this->tiff_start + (std::streampos)offset, std::ios::beg);
	Process_IFD(file, temp_collector, temp_length);
	file.seekg(base);
}

void	Jpeg_Jpg::HandleValueData(std::ifstream &file, unsigned short &collector)
{
	unsigned int	type_size;
	unsigned int	count;
	unsigned int	offset;
	std::streampos	base;
	std::string		temp;

	type_size = GetTypeSize(file);
	count = read_u32(file);
	if (file.fail())
		return ;
	count *= type_size;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (count == 0)
		return ;
	base = file.tellg();
	if (count > 4)
	{
		file.seekg(this->tiff_start + (std::streampos)offset, std::ios::beg);
		if (collector == 0x829D)
			this->data["FNumber"] = get_rational(file);
		else if (collector == 0x829A)
			this->data["ExposureTime"] = get_rational(file);
		else if (collector == 0x0002 || collector == 0x0004)
		{
			temp = get_rational(file) + " " + get_rational(file) + " " + get_rational(file);
			if (collector == 0x0002)
				this->data["GPS Latitude"] = temp;
			else
				this->data["GPS Longitude"] = temp;
		}
		else if (collector == 0x0006)
			this->data["GPS Altitude"] = get_rational(file);
		file.seekg(base);
	}
	else
	{
		file.seekg(-4, std::ios::cur);
		if (collector == 0x8827)
		{
			unsigned int	val = 0;
			if (count == 2)
			{
				val = read_u16(file);
				if (file.fail())
					return ;
			}
			else
			{
				val = read_u32(file);
				if (file.fail())
					return ;
			}
			std::stringstream ss;
			ss << val;
			this->data["ISO Speed"] = ss.str();
		}
		file.seekg(4 - count, std::ios::cur);
	}
}

std::string	Jpeg_Jpg::get_rational(std::ifstream &file)
{
	unsigned int		num;
	unsigned int		den;
	std::stringstream	ss;

	num = read_u32(file);
	if (file.fail())
		return ("");
	den = read_u32(file);
	if (file.fail())
		return ("");

	if (den == 0)
		return ("0");
	if (num % den == 0)
	{
		ss << (num / den);
		return (ss.str());
	}
	if (num > den)
	{
		double val = (double)num / (double)den;
		ss << val;
	}
	else
		ss << num << "/" << den;
	return (ss.str());
}

void	Jpeg_Jpg::HandleGPSRef(std::ifstream &file, unsigned short &collector)
{
	unsigned int	type_size;
	unsigned int	count;
	unsigned int	offset;
	char			buffer[5];

	type_size = GetTypeSize(file);
	count = read_u32(file);
	if (file.fail())
		return ;
	count *= type_size;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (count == 0)
		return ;
	if (count <= 4)
	{
		file.seekg(-4, std::ios::cur);
		file.read(buffer, count);
		if (file.fail())
			return ;
		buffer[count] = '\0';
		if (collector == 0x0001)
			this->data["GPS Latitude Ref"] = std::string(buffer);
		else if (collector == 0x0003)
			this->data["GPS Longitude Ref"] = std::string(buffer);
		file.seekg(4 - count, std::ios::cur);
	}
}

void	Jpeg_Jpg::Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length)
{
	unsigned short	num_entries;
	size_t			i;

	if ((num_entries = read_u16(file)) == 0)
		return ;
	i = 0;
	while (i < num_entries)
	{
		collector = read_u16(file);
		if (file.fail())
			return ;
		if (collector == 0x010F || collector == 0x0110 || collector == 0x0132)
			HandleStringData(file, collector);
		else if (collector == 0x8769 || collector == 0x8825)
			HandleExifSubAndGPS(file);
		else if (collector == 0x8827 || collector == 0x829A || collector == 0x829D
			|| collector == 0x0002 || collector == 0x0004 || collector == 0x0006)
			HandleValueData(file, collector);
		else if (collector == 0x0001 || collector == 0x0003)
			HandleGPSRef(file, collector);
		else
			file.seekg(10, std::ios::cur);
		length -= 12;
		i++;
	}
}

void	Jpeg_Jpg::StartParsing(std::ifstream &file, unsigned short &collector, unsigned short length)
{
	char			buffer[2];
	unsigned int	offset;

	this->tiff_start = file.tellg();
	if (!file.read(buffer, 2))
		return ;
	if (buffer[0] == 'I' && buffer[1] == 'I')
		little_endian = true;
	else if (buffer[0] == 'M' && buffer[1] == 'M')
		little_endian = false;
	else
		return ;
	collector = read_u16(file);
	if (file.fail())
		return ;
	if (collector != 42)
		return ;
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (offset >= 8)
		file.seekg(offset - 8, std::ios::cur);
	else
		return ;
	length -= offset;
	Process_IFD(file, collector, length);
}

void	Jpeg_Jpg::parse_exif(std::ifstream &file, unsigned short &collector)
{
	char			exif[6];
	unsigned short	length;

	length = read_u16(file);
	if (length == 0 || length == 1)
		return ;
	length -= 2;
	if (!file.read(exif, 6))
		return ;
	if (!(exif[0] == 'E' && exif[1] == 'x' && exif[2] == 'i' && exif[3] == 'f' && exif[4] == '\0' && exif[5] == '\0'))
		return ;
	length -= 6;
	StartParsing(file, collector, length);
}

void    Jpeg_Jpg::UpdateData(std::ifstream &file)
{
	unsigned short	collector;
	unsigned short	length;
	std::streampos	block_start;

	while (!file.eof())
	{
		collector = read_u16(file);

		if (file.fail() || collector == 0xFFDA)
			return ;
		else if (collector == 0xFFE1)
		{
			block_start = file.tellg();
			length = read_u16(file);
			file.seekg(block_start); 
			parse_exif(file, collector);
			file.seekg(block_start + (std::streampos)length, std::ios::beg);
			this->little_endian = false;
		}
		else if (collector == 0xFFC0 || collector == 0xFFC2)
		{
			length = read_u16(file);
			file.seekg(1, std::ios::cur);
			unsigned short height = read_u16(file);
			unsigned short width = read_u16(file);
			std::stringstream ss_h, ss_w;

			ss_h << height;
			ss_w << width;
			this->data["Height"] = ss_h.str();
			this->data["Width"] = ss_w.str();

			file.seekg(length - 7, std::ios::cur);
		}
		else
		{
			std::cout << "Atlanan Marker: 0x" << std::hex << collector << std::dec << std::endl;
			if ((collector >> 8) != 0xFF)
				return ;
			length = read_u16(file);
			if (file.fail())
				return ;
			file.seekg(length - 2, std::ios::cur);
		}
	}
}

void	Jpeg_Jpg::parse()
{
	char			buffer[2];
	unsigned char	b1;
	unsigned char	b2;
	std::ifstream	file(file_name.c_str(), std::ios::in | std::ios::binary);
	
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file: " << file_name << std::endl;
		return ;
	}
	if (file.read(buffer, 2))
	{
		b1 = (unsigned char)buffer[0];
		b2 = (unsigned char)buffer[1];
		if (b1 == 0xFF && b2 == 0xD8)
			control = true;
		else
			std::cout << "[x] " << file_name << " is not a valid JPEG." << std::endl;
	}
	if (control == true)
		UpdateData(file);
	file.close();
}
