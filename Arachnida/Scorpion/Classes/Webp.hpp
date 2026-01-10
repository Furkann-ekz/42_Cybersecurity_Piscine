#ifndef WEBP_HPP
#define WEBP_HPP

#include "AllClass.hpp"

class Webp: public AllClasses
{
	public:
		Webp(std::string file_name);
		~Webp();

		void			parse();
		void			parse_continue(std::ifstream &file, unsigned int collector, char *buffer);
		void			ProcessChunks(std::ifstream &file);
		
		void			ParseVP8X(std::ifstream &file, unsigned int &collector);
		void			ParseVP8(std::ifstream &file, unsigned int &collector);
		void			ParseVP8L(std::ifstream &file, unsigned int &collector);
		void			ParseEXIF(std::ifstream &file, unsigned int len);
		
		void			HandleVP8XFlags(std::ifstream &file);
		unsigned int	read_u24(std::ifstream &file);
};

#endif