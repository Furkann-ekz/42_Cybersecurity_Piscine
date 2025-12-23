#ifndef SPIDER_HPP
#define SPIDER_HPP

#include "iostream"
#include "cstring"

using std::string;
using std::endl;
using std::cout;

typedef struct s_data
{
	bool	r;
	bool	p;
	bool	l;
}	t_data;

class Spider
{
	private:
		t_data	data;
	public:
		Spider();
		~Spider();
		void	set_data();
};

#endif