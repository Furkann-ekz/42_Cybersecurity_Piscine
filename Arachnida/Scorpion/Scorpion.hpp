#ifndef SCORPION_HPP
#define SCORPION_HPP

#include "./Classes/AllClass.hpp"
#include "./Classes/Bmp.hpp"
#include "./Classes/Gif.hpp"
#include "./Classes/Jpeg_Jpg.hpp"
#include "./Classes/Png.hpp"
#include "./Classes/Tiff.hpp"
#include "./Classes/Webp.hpp"
#include "./Classes/Svg.hpp"
#include <cctype>

class Scorpion
{
	private:
		std::vector<AllClasses *>	classes;
		std::vector<std::string>	files;
	public:
		Scorpion(char **av);
		~Scorpion();

		void		SetClasses(void);
		std::string	to_lower(std::string str);
		void		Run(void);
};

#endif