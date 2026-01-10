#ifndef SVG_HPP
#define SVG_HPP

#include "AllClass.hpp"

class Svg: public AllClasses
{
	private:
		std::string	text;
		std::string	get_attribute(std::string key);
		std::string get_element_content(std::string tag);
	public:
		Svg(std::string file_name);
		~Svg();

		void	parse();
		void	parse_continue();

};

#endif