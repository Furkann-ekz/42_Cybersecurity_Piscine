#include "AllClass.hpp"

AllClasses::AllClasses(std::string f_image)
{
	this->file_name = f_image;
	this->control = false;
	this->little_endian = false;
	this->tiff_start = 0;
}

AllClasses::~AllClasses()
{
}

void	AllClasses::set_filename(std::string f_name)
{
	this->file_name = f_name;
}

std::string AllClasses::get_filename()
{
	return (this->file_name);
}

void	AllClasses::display_info()
{
	if (this->data.empty())
		return ;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "File: " << this->file_name << std::endl;
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

void	AllClasses::print_data()
{
	std::map<std::string, std::string>::iterator	it;

	if (this->data.empty())
	{
		std::cout << RED << ">> No valid metadata found." << RESET << std::endl;
		return ;
	}
	it = this->data.begin();
	while (it != this->data.end())
	{
		std::cout << CYAN << "[" << it->first << "] " << RESET << ": " 
				  << GREEN << it->second << RESET << std::endl;
		it++;
	}
}

void	AllClasses::HandleLongValues(std::ifstream &file, unsigned short collector, unsigned int val_offset)
{
	std::string		temp_str;
	std::streampos	base;

	base = file.tellg();
	file.seekg(this->tiff_start + (std::streampos)val_offset, std::ios::beg);
	if (collector == 0x829D)
		this->data["F-Number"] = get_rational(file);
	else if (collector == 0x829A)
		this->data["Exposure Time"] = get_rational(file);
	else if (collector == 0x9202)
		this->data["Aperture Value"] = get_rational(file);
	else if (collector == 0x9201)
		this->data["Shutter Speed"] = get_rational(file);
	else if (collector == 0x9204)
		this->data["Exposure Bias"] = get_rational(file);
	else if (collector == 0x0002)
	{
		temp_str = get_rational(file) + " " + get_rational(file) + " " + get_rational(file);
		this->data["GPS Latitude"] = temp_str;
	}
	else if (collector == 0x0004)
	{
		temp_str = get_rational(file) + " " + get_rational(file) + " " + get_rational(file);
		this->data["GPS Longitude"] = temp_str;
	}
	else if (collector == 0x0006)
		this->data["GPS Altitude"] = get_rational(file);
	file.seekg(base);
}

void	AllClasses::HandleShortValues(std::ifstream &file, unsigned short collector, unsigned int type_size, unsigned int count)
{
	unsigned int		val;
	int					remaining;
	std::stringstream	ss;

	file.seekg(-4, std::ios::cur);
	val = (type_size == 2) ? read_u16(file) : read_u32(file);
	if (file.fail())
		return ;
	ss << val;
	if (collector == 0x8827)
		this->data["ISO Speed"] = ss.str();
	else if (collector == 0x0100)
		this->data["Image Width"] = ss.str();
	else if (collector == 0x0101)
		this->data["Image Height"] = ss.str();
	else if (collector == 0x0103)
		this->data["Compression"] = ss.str();
	else if (collector == 0x0112)
		this->data["Orientation"] = ss.str();
	else if (collector == 0x9209)
		this->data["Flash"] = ss.str();
	else if (collector == 0x9207)
		this->data["Metering Mode"] = ss.str();
	else if (collector == 0xA405)
		this->data["Focal Length"] = ss.str();
	remaining = 4 - (type_size * count);
	if (remaining > 0)
		file.seekg(remaining, std::ios::cur);
	else
		file.seekg(0, std::ios::cur);
}

void	AllClasses::HandleValueData(std::ifstream &file, unsigned short &collector)
{
	unsigned int	type_size;
	unsigned int	count;
	unsigned int	val_offset;

	type_size = GetTypeSize(file);
	count = read_u32(file);
	if (file.fail())
		return ;
	val_offset = read_u32(file);
	if (file.fail())
		return ;
	if ((count * type_size) > 4)
		HandleLongValues(file, collector, val_offset);
	else
		HandleShortValues(file, collector, type_size, count);
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

std::string AllClasses::get_rational(std::ifstream &file)
{
	unsigned int		num;
	unsigned int		den;
	double				val;
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
		ss << (num / den);
	else
	{
		if (num < den)
			ss << num << "/" << den;
		else
		{
			val = (double)num / (double)den;
			ss << std::fixed << std::setprecision(2) << val;
		}
	}
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
	offset = read_u32(file);
	if (file.fail())
		return ;
	if (count > 4)
		return ;
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

void	AllClasses::DispatchTag(std::ifstream &file, unsigned short collector)
{
	if (collector == 0x010F || collector == 0x0110 || collector == 0x0132
		|| collector == 0x0131 || collector == 0x9003 || collector == 0x9004)
		HandleStringData(file, collector);
	else if (collector == 0x8769 || collector == 0x8825)
		HandleSubIFD(file);
	else if (collector == 0x8827 || collector == 0x829A || collector == 0x829D
		|| collector == 0x0002 || collector == 0x0004 || collector == 0x0006
		|| collector == 0x0100 || collector == 0x0101 || collector == 0x0103
		|| collector == 0x0112 || collector == 0x9201 || collector == 0x9202
		|| collector == 0x9204 || collector == 0x9207 || collector == 0x9209
		|| collector == 0xA405)
		HandleValueData(file, collector);
	else if (collector == 0x0001 || collector == 0x0003)
		HandleGPSRef(file, collector);
	else
		file.seekg(10, std::ios::cur);
}

void	AllClasses::Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length)
{
	unsigned short	num_entries;
	size_t			i;

	num_entries = read_u16(file);
	if (num_entries == 0)
		return ;
	i = 0;
	while (i < num_entries)
	{
		collector = read_u16(file);
		if (file.fail())
			return ;
		DispatchTag(file, collector);
		(void)length;
		i++;
	}
}

void	AllClasses::HandleStringData(std::ifstream &file, unsigned short &collector)
{
	unsigned int	count;
	unsigned int	offset;
	std::streampos	base;
	std::string		res;
	char			*buf;
	char			s_buf[5];

	(void)GetTypeSize(file); 
	count = read_u32(file);
	offset = read_u32(file);
	if (file.fail() || count == 0)
		return ;
	if (count > 4)
	{
		base = file.tellg();
		file.seekg(this->tiff_start + (std::streampos)offset, std::ios::beg);
		buf = new char[count + 1];
		if (buf && file.read(buf, count))
		{
			buf[count] = '\0';
			res = buf;
		}
		delete[] buf;
		file.seekg(base);
	}
	else
	{
		file.seekg(-4, std::ios::cur);
		if (file.read(s_buf, count))
		{
			s_buf[count] = '\0';
			res = s_buf;
		}
		file.seekg(4 - count, std::ios::cur);
	}
	if (!res.empty())
	{
		if (collector == 0x010F)
			this->data["Camera Make"] = res;
		else if (collector == 0x0110)
			this->data["Camera Model"] = res;
		else if (collector == 0x0131)
			this->data["Software"] = res;
		else if (collector == 0x0132)
			this->data["Modify Date"] = res;
		else if (collector == 0x9003)
			this->data["Date/Time Original"] = res;
		else if (collector == 0x9004)
			this->data["Create Date"] = res;
	}
}

int	AllClasses::GetTypeSize(std::ifstream &file)
{
	unsigned short	type;

	type = read_u16(file);
	switch (type)
	{
		case 1:
			return (1);
		case 2:
			return (1);
		case 3:
			return (2);
		case 4:
			return (4);
		case 5:
			return (8);
		case 6:
			return (1);
		case 7:
			return (1);
		case 8:
			return (2);
		case 9:
			return (4);
		case 10:
			return (8);
		case 11:
			return (4);
		case 12:
			return (8);
		default:
			return (0);
	}
}