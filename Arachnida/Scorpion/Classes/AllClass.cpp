#include "AllClass.hpp"

AllClasses::AllClasses(std::string f_image)
{
	file_name = f_image;
	control = false;
	little_endian = false;
	tiff_start = 0;
}

AllClasses::~AllClasses()
{}

void	AllClasses::set_filename(std::string f_name)
{
	file_name = f_name;
}

std::string	AllClasses::get_filename()
{
	return (file_name);
}

void	AllClasses::display_info()
{
	std::map<std::string, std::string>::iterator iterator;
	if (control == false)
		return ;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "File: " << file_name << std::endl;
}

unsigned short	AllClasses::read_u16(std::ifstream &file)
{
	unsigned char	buffer[2];

	if (!file.read((char *)buffer, 2))
		return (0);

	if (this->little_endian)
		return ((buffer[1] << 8) | buffer[0]);

	return ((buffer[0] << 8) | buffer[1]);
}

unsigned int	AllClasses::read_u32(std::ifstream &file)
{
	unsigned char	buffer[4];

	if (!file.read((char *)buffer, 4))
		return (0);

	if (this->little_endian)
		return ((buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0]);

	return ((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3]);
}

void    AllClasses::print_data()
{
	if (this->data.empty())
	{
		std::cout << RED << ">> Herhangi bir veri bulunamadi." << RESET << std::endl;
		return ;
	}
	
	std::map<std::string, std::string>::iterator it = this->data.begin();
	
	while (it != this->data.end())
	{
		std::cout << CYAN << "[" << it->first << "] " << RESET << ": " 
				  << GREEN << it->second << RESET << std::endl;
		it++;
	}
}

void	AllClasses::HandleSubIFD(std::ifstream &file)
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

void	AllClasses::HandleValueData(std::ifstream &file, unsigned short &collector)
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
		if (collector == 0x8827 || collector == 0x0100 || collector == 0x0101 || collector == 0x0103)
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
			
			std::stringstream	ss;
			ss << val;

			if (collector == 0x8827)
				this->data["ISO Speed"] = ss.str();
			else if (collector == 0x0100)
				this->data["Width"] = ss.str();
			else if (collector == 0x0101)
				this->data["Height"] = ss.str();
			else if (collector == 0x0103)
				this->data["Compression"] = ss.str();
		}        
		file.seekg(4 - count, std::ios::cur);
	}
}

std::string	AllClasses::get_rational(std::ifstream &file)
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

void	AllClasses::HandleGPSRef(std::ifstream &file, unsigned short &collector)
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

void	AllClasses::Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length)
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
			HandleSubIFD(file);
		else if (collector == 0x8827 || collector == 0x829A || collector == 0x829D
			|| collector == 0x0002 || collector == 0x0004 || collector == 0x0006
			|| collector == 0x0100 || collector == 0x0101 || collector == 0x0103) // 103=Compression
			HandleValueData(file, collector);
		else if (collector == 0x0001 || collector == 0x0003)
			HandleGPSRef(file, collector);
		else
			file.seekg(10, std::ios::cur);
		length -= 12;
		i++;
	}
}

void	AllClasses::HandleStringData(std::ifstream &file, unsigned short &collector)
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

int	AllClasses::GetTypeSize(std::ifstream &file)
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
