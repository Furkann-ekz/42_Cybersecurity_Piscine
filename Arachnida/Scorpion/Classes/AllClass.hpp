#ifndef ALLCLASS_HPP
#define ALLCLASS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <arpa/inet.h>
#include <algorithm>
#include <sstream>

class AllClasses
{
	protected:
		std::string	file_name;
	public:
		AllClasses(std::string f_image);
		virtual ~AllClasses();

		virtual void parse() = 0;
		virtual void display_info() = 0;
		void	set_filename(std::string f_name);
		std::string get_filename();
};

#endif