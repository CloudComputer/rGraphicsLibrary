#include "Logger.h"

#include <assert.h>
#include <util/Macros.h>
#include <iostream>
#include <ctime>


std::ostream& operator<<(std::ostream& os,const glm::vec2 &v){
	os << "[" << v.x << "," << v.y << "]";
	return os;
}
std::ostream& operator<<(std::ostream& os,const glm::vec3 &v){
	os << "[" << v.x << "," << v.y <<  "," << v.z <<  "]";
	return os;
}
std::ostream& operator<<(std::ostream& os,const glm::vec4 &v){
	os << "[" << v.x << "," << v.y <<  "," << v.z <<  "," << v.w <<  "]";
	return os;
}
std::ostream& operator<<(std::ostream& os,const glm::ivec2 &v){
	os << "[" << v.x << "," << v.y << "]";
	return os;
}
std::ostream& operator<<(std::ostream& os,const glm::ivec3 &v){
	os << "[" << v.x << "," << v.y <<  "," << v.z <<  "]";
	return os;
}
std::ostream& operator<<(std::ostream& os,const glm::ivec4 &v){
	os << "[" << v.x << "," << v.y <<  "," << v.z <<  "," << v.w <<  "]";
	return os;
}


LogManager* LogManager::manager_ = 0;
LogManager* LogManager::getLogManager(){
    if(!manager_){
        manager_ = new LogManager();
    }
    return manager_;
}

LogManager::LogManager():allowEmptyLogger_(false){}

LogManager::~LogManager(){
    IT_FOR(loggers_,logger){
        delete *logger;
    }
}

void LogManager::attatchLogger(Logger *l){
    loggers_.push_back(l);
}

void LogManager::log(LogLevel logLevel,std::string msg,std::string function,std::string file,int line){
    if(loggers_.empty() && !allowEmptyLogger_){
        loggers_.push_back(new ConsolLogger());
#ifdef _DEBUG
        loggers_[0]->setMinLogLevel(Debug);
#else
        loggers_[0]->setMinLogLevel(Warning);
#endif
    }
    IT_FOR(loggers_,logger){
        (*logger)->log(logLevel,msg,function,file,line);
    }
}

std::string LogManager::logLevelToString(LogLevel l){
    switch(l){
    case Debug:
        return "Debug";
    case Info:
        return "Info";
    case Warning:
        return "Warning";
    case Error:
        return "Error";
    default:
        assert(false && "Unknown loglevel");
        return "Unknown logLevel";
    }
}

ConsolLogger::ConsolLogger(){
    startClock.start();
    useTimmers_ = false;
}

void ConsolLogger::setUseTimmers(bool useTimmers){
    useTimmers_ = useTimmers;
}

void ConsolLogger::log_(LogLevel logLevel,std::string msg,std::string function,std::string file,int line){
    std::ostream &out = (logLevel==Error) ? std::cerr : std::cout;
    
    out << LogManager::logLevelToString(logLevel) << "\t";
    if(useTimmers_){
        out << prevClock.getFractionElapsedSeconds() << "s," << startClock.getFractionElapsedSeconds() << "s\t"; 
    }
    out << function << " in " << file << " line " << line  << "\t";
    out << msg << std::endl;
    prevClock.restart();
}


FileLogger::FileLogger(){
    startClock.start();
    useTimmers_ = false;
}

void FileLogger::setUseTimmers(bool useTimmers){
    useTimmers_ = useTimmers;
}


FileLogger* FileLogger::CreateLogger(std::string filename){
    FileLogger *logger = new FileLogger();
    logger->file.open(filename.c_str(),std::ios::out);
    if(!logger->file.is_open()){
        delete logger;
        return 0;
    }
    return logger;
}

void FileLogger::log_(LogLevel logLevel,std::string msg,std::string function,std::string srcFile,int line){
    file << LogManager::logLevelToString(logLevel) << "\t";
    if(useTimmers_){
        file << prevClock.getFractionElapsedSeconds() << "s," << startClock.getFractionElapsedSeconds() << "s\t"; 
    }
    file << function << " in " << srcFile << " line " << line  << "\t";
    file << msg << std::endl;
    prevClock.restart();
}


