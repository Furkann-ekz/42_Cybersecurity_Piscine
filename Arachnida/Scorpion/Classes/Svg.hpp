#ifndef SVG_HPP
#define SVG_HPP

#include "AllClass.hpp"

class Svg: public AllClasses
{
	private:
	public:
		Svg(std::string file_name);
		~Svg();

		void	parse();
		void	display_info();
};

#endif