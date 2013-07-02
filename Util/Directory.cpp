#include "Directory.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <assert.h>

using namespace boost::filesystem;

std::vector<std::string> Directory::getAllFilesInFolder(std::string dir){
	std::vector<std::string> files;

	assert(is_directory(dir));

	for(directory_iterator file(dir);file!=directory_iterator();++file){
		if(is_directory(file->path())) continue;
		files.push_back(file->path().generic_string());
	}

	return files;
}
std::vector<std::string> Directory::getAllFilesInFolder(std::string dir,std::string pattern){
	std::vector<std::string> files;

	assert(is_directory(dir));
	boost::xpressive::sregex regx = boost::xpressive::sregex::compile(pattern);
	boost::xpressive::smatch what;

	for(directory_iterator file(dir);file!=directory_iterator();++file){
		if(is_directory(file->path())) continue;
		if(boost::xpressive::regex_search(file->path().filename().generic_string(),what,regx))
			files.push_back(file->path().generic_string());
	}

	return files;
}

void Directory::createFolder(std::string dir){
	create_directory(dir);
}

bool Directory::exists(std::string path){
	return boost::filesystem::exists(path);
}