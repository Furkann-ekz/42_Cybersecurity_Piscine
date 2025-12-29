#ifndef GIF_HPP
#define GIF_HPP

#include "AllClass.hpp"

class Gif: public AllClasses
{
	private:
	public:
		Gif(std::string file_name);
		~Gif();

		void	parse();
		void	display_info();
};

#endif