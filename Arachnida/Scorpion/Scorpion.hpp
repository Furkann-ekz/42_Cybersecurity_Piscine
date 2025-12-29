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
		std::vector<AllClasses *>	files;
	public:
		Scorpion();
		~Scorpion();
};

#endif