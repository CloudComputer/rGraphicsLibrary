#include "HTML.h"

#include <fstream>


HTML::HTML(){


}


HTML::~HTML(){


}


void HTML::appendToHead(std::string code){
	head << code << std::endl;
}
void HTML::appendToBody(std::string code){
	body << code << std::endl;
}


void HTML::save(const char *filename)const{
	std::ofstream html(filename);
	html << "<html>" <<std::endl;
	html << "<head>" <<std::endl;
	html << head.str() << std::endl; 
	html << "</head>"<<std::endl;
	html << "<body>" << std::endl;
	html << body.str() << std::endl;
	html << "</body>"<<std::endl<<"</html>";
	html.close();
}