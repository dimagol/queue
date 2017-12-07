//
// Created by dima on 01/12/17.
//

#ifndef TCP_SHMAFKA_LOGGER_H
#define TCP_SHMAFKA_LOGGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include <ctime>
#include <thread>
#include <sstream>

using namespace std;




template <typename T>
string strBuilder(T t)
{
    stringstream ss("");
    ss << t;
    return ss.str();
}

template<typename T, typename... Args>
string strBuilder(T t, Args... args) // recursive variadic function
{
    stringstream ss("");
    ss << t;
    return ss.str() + strBuilder(args...) ;
}


class TSLogger {
public:
    enum Severity{TRACE =1,DEBUG =2,INFO =3,WARN =4,ERROR =5};


    bool init(const string &a_logFileName,
              bool a_print = false,
              Severity a_logSev = WARN) {

        print = a_print;
        logSev = a_logSev;
        logFile = new  ofstream(a_logFileName);

    }

    void logMsg(string msg, Severity severity, string srcFileName, uint64_t line) {
        string sevStr;
        switch (severity) {
            case TRACE:
                sevStr = "TRACE";
                break;
            case DEBUG:
                sevStr = "DEBUG";
                break;
            case INFO:
                sevStr = "INFO";
                break;
            case WARN:
                sevStr = "WARN";
                break;
            case ERROR:
                sevStr = "ERROR";
                break;
        }

        lock.lock();
        stringstream ss("");
        ss << sevStr
           << " "
           << getTimeStr()
           << " "
           << getFileName(srcFileName)
           << ":"
           << line
           << " ["
           << this_thread::get_id()
           << "] - "
           << msg << endl;

        string fullMsg = ss.str();
        if (logFile != nullptr) {
            (*logFile) << fullMsg;
            logFile->flush();
            lock.unlock();
        }

        if (print){
            cout << fullMsg;
            cout.flush();
        }

    }

    virtual ~TSLogger() {
        close();
    }

private:
    void close(){
        lock.lock();
        logFile->close();
        lock.unlock();
    }
    string getTimeStr(){
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
        std::string str(buffer);
        return str;
    }
    string getFileName(const string& s) {

        char sep = '/';

        size_t i = s.rfind(sep, s.length());
        if (i != string::npos) {
            return(s.substr(i+1, s.length() - i));
        }

        return("");
    }


public:
    Severity logSev = INFO;
private:
    ofstream  *logFile = nullptr;
    bool print = false;
    mutex lock;

};

static TSLogger globalLogger;


#define LOG_ERROR(...)\
    if (globalLogger.logSev <= TSLogger::Severity::ERROR){\
           std::string str1112 = strBuilder(__VA_ARGS__); \
           globalLogger.logMsg(strBuilder(__VA_ARGS__),TSLogger::ERROR,__FILE__,__LINE__);\
    }

#define LOG_WARN(...)\
    if (globalLogger.logSev <= TSLogger::Severity::WARN){\
           std::string str1112 = strBuilder(__VA_ARGS__); \
           globalLogger.logMsg(strBuilder(__VA_ARGS__),TSLogger::WARN,__FILE__,__LINE__);\
    }

#define LOG_INFO(...)\
    if (globalLogger.logSev <= TSLogger::Severity::INFO){\
           std::string str1112 = strBuilder(__VA_ARGS__); \
           globalLogger.logMsg(strBuilder(__VA_ARGS__),TSLogger::INFO,__FILE__,__LINE__);\
    }

#define LOG_DEBUG(...)\
    if (globalLogger.logSev <= TSLogger::Severity::DEBUG){\
           std::string str1112 = strBuilder(__VA_ARGS__); \
           globalLogger.logMsg(strBuilder(__VA_ARGS__),TSLogger::DEBUG,__FILE__,__LINE__);\
    }

#define LOG_TRACE(...)\
    if (globalLogger.logSev <= TSLogger::Severity::TRACE){\
           std::string str1112 = strBuilder(__VA_ARGS__); \
           globalLogger.logMsg(strBuilder(__VA_ARGS__),TSLogger::TRACE,__FILE__,__LINE__);\
    }

#endif //TCP_SHMAFKA_LOGGER_H
