#ifndef ALLCLASS_HPP
#define ALLCLASS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <arpa/inet.h>
#include <algorithm>
#include <sstream>

class AllClasses
{
	protected:
		bool								control;
		bool								little_endian; // false ise big; true ise little endian
		std::string							file_name;
		std::map<std::string, std::string>	data;
	public:
		AllClasses(std::string f_image);
		virtual ~AllClasses();

		virtual void	parse() = 0;
		void			display_info();
		void			set_filename(std::string f_name);
		std::string		get_filename();
		unsigned short	read_u16(std::ifstream &file);
		unsigned int	read_u32(std::ifstream &file);

		template <typename T>
		std::string		to_string(T value)
		{
			std::stringstream ss;
			ss << value;
			return ss.str();
		}
};

#endif