#ifndef SCORPION_HPP
#define SCORPION_HPP

#include "./Classes/AllClass.hpp"
#include "./Classes/Bmp.hpp"
#include "./Classes/Gif.hpp"
#include "./Classes/JpegAndJpg.hpp"
#include "./Classes/Png.hpp"
#include "./Classes/Tiff.hpp"
#include "./Classes/Webp.hpp"

class Scorpion
{
	private:
		std::vector<AllClasses *>	classes;
		std::vector<std::string>	files;
	public:
		Scorpion(char **av);
		~Scorpion();

		std::string	get_extension(std::string f_name);
};

#endif