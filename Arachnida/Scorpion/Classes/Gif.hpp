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
		
		void	ProcessBlocks(std::ifstream &file);
		void	HandleExtension(std::ifstream &file);
		void	ReadComment(std::ifstream &file);
		void	ReadGraphicControl(std::ifstream &file);
		void	ReadApplication(std::ifstream &file);
		void	SkipBlock(std::ifstream &file);
		void	HandlePackedField(unsigned char packed);
};

#endif