#ifndef WEBP_HPP
#define WEBP_HPP

#include "AllClass.hpp"

class Webp: public AllClasses
{
	private:
	public:
		Webp(std::string file_name);
		~Webp();

		void	parse();
		void	display_info();
};

#endif