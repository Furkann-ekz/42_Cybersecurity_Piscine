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
		std::string	file_name;
		bool		control;
		std::map<std::string, std::string>	data;
	public:
		AllClasses(std::string f_image);
		virtual ~AllClasses();

		virtual void	parse() = 0;
		void			display_info();
		void			set_filename(std::string f_name);
		std::string		get_filename();
		template <typename T>
		std::string AllClasses::to_string(T value)
		{
			std::stringstream ss;
			ss << value;
			return ss.str();
		}
};

#endif