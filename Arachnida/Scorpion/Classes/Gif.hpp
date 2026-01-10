#ifndef GIF_HPP
#define GIF_HPP

#include "AllClass.hpp"

class Gif: public AllClasses
{
	public:
		Gif(std::string file_name);
		~Gif();

		void	parse();
		void	parse_continue(std::ifstream &file);
		void	HandlePackedField(std::ifstream &file);

};

#endif