#ifndef JPEGANDJPG_HPP
#define JPEGANDJPG_HPP

#include "AllClass.hpp"

class Jpeg_Jpg: public AllClasses
{
	private:
	public:
		Jpeg_Jpg(std::string file_name);
		~Jpeg_Jpg();

		void	parse();
};

#endif