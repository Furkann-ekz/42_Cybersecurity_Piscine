#ifndef ALLCLASS_HPP
#define ALLCLASS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cctype>

#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[1;32m"
#define CYAN	"\033[1;36m"
#define YELLOW	"\033[1;33m"

class AllClasses
{
	protected:
		std::streampos						tiff_start;
		bool								control;
		bool								little_endian;
		std::string							file_name;
		std::map<std::string, std::string>	data;

	public:
		AllClasses(std::string f_image);
		virtual	~AllClasses();

		virtual void	parse() = 0;
		void			set_filename(std::string f_name);
		std::string		get_filename();
		void			display_info();
		void			print_data();

		unsigned short	read_u16(std::ifstream &file);
		unsigned int	read_u32(std::ifstream &file);

		void			Process_IFD(std::ifstream &file, unsigned short &collector, unsigned short &length);
		void			DispatchTag(std::ifstream &file, unsigned short collector);
		
		void			HandleStringData(std::ifstream &file, unsigned short &collector);
		void			HandleSubIFD(std::ifstream &file);
		
		void			HandleValueData(std::ifstream &file, unsigned short &collector);
		void			HandleLongValues(std::ifstream &file, unsigned short collector, unsigned int val_offset);
		void			HandleShortValues(std::ifstream &file, unsigned short collector, unsigned int type_size, unsigned int count);
		
		void			HandleGPSRef(std::ifstream &file, unsigned short &collector);
		
		int				GetTypeSize(std::ifstream &file);
		std::string		get_rational(std::ifstream &file);

		template <typename T>
		std::string		to_string(T value)
		{
			std::stringstream	ss;
			ss << value;
			return ss.str();
		}
};

#endif