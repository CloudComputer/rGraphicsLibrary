#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <vector>
#include <string>

#include <fstream>

#include "StopClock.h"

enum LogLevel{
    Debug   = 0,
    Info    = 1,
    Warning = 2,
    Error   = 3
};

#define LOG(lvl,msg)LogManager::getLogManager()->log(lvl,msg,__FUNCTION__,__FILE__,__LINE__)
#define LOG_DEBUG(msg) {std::stringstream ss; ss << msg;  LOG(Debug,ss.str());}
#define LOG_INFO(msg) {std::stringstream ss; ss << msg;  LOG(Info,ss.str());}
#define LOG_WARN(msg) {std::stringstream ss; ss << msg;  LOG(Warning,ss.str());}
#define LOG_ERROR(msg) {std::stringstream ss; ss << msg;  LOG(Error,ss.str());}

class Logger;

class LogManager{
    LogManager();
    std::vector<Logger*> loggers_;
    bool allowEmptyLogger_;
public:
	virtual ~LogManager();

    void setAllowEmptyLogger(bool allowEmptyLogger = true){allowEmptyLogger_ = allowEmptyLogger;}
    bool getAllowEmptyLogger(){return allowEmptyLogger_;}

    void attatchLogger(Logger *l);

    void log(LogLevel logLevel,std::string msg,std::string function,std::string file,int line);
    //void log(LogLevel logLevel,std::string msg,std::string function,std::string file,int line);

    static std::string logLevelToString(LogLevel l);

    static LogManager *manager_;
    static LogManager* getLogManager();
};

class Logger{
    LogLevel logLevel_;
protected:
    virtual void log_(LogLevel logLevel,std::string msg,std::string function,std::string file,int line) = 0;
public:
    Logger():logLevel_(Warning){}
    virtual void log(LogLevel logLevel,std::string msg,std::string function,std::string file,int line){
        if(logLevel>=logLevel_)
            log_(logLevel,msg,function,file,line);
    }

    void setMinLogLevel(LogLevel logLevel){
        logLevel_ = logLevel;
    }
    LogLevel getMinLogLevel(){
        return logLevel_;
    }
};

class ConsolLogger : public Logger{
    bool useTimmers_;
    StopClock startClock;
    StopClock prevClock;
public:
    ConsolLogger();
    void setUseTimmers(bool useTimmers = true);
    bool getUseTimmers(){return useTimmers_;}
protected:
    virtual void log_(LogLevel logLevel,std::string msg,std::string function,std::string file,int line);
};


class FileLogger : public Logger{
    bool useTimmers_;
    StopClock startClock;
    StopClock prevClock;
    std::ofstream file;
    FileLogger();
public:
    void setUseTimmers(bool useTimmers = true);
    bool getUseTimmers(){return useTimmers_;}
    static FileLogger* CreateLogger(std::string filename);
protected:
    virtual void log_(LogLevel logLevel,std::string msg,std::string function,std::string file,int line);

};

#endif