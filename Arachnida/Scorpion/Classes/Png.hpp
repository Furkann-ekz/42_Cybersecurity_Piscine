#ifndef PNG_HPP
#define PNG_HPP

#include "AllClass.hpp"

class Png: public AllClasses
{
	public:
		Png(std::string file_name);
		~Png();

		void	parse();
		void	ParseIHDR(std::ifstream &file, unsigned int &collector);
		void	ParseIHDR_Body(std::ifstream &file);
		
		void	ProcessRemainingChunks(std::ifstream &file);
		void	HandleText(std::ifstream &file, unsigned int length);
		void	HandlePhys(std::ifstream &file, unsigned int length);
		void	HandleTime(std::ifstream &file, unsigned int length);
		void	SkipChunk(std::ifstream &file, unsigned int length);
};

#endif