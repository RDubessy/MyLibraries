#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <map>
/*! \brief This type contains the options, as a map of string pairs, keys and
 * values */
typedef std::map<std::string,std::string> ConfigMap;
/*! \brief This method reads a string and add an entry in the map. */
bool readString(std::string,ConfigMap &,std::string &);
/*! \brief This method reads the cmd lines options and trigger the program
 * execution. */
bool parseOptions(const int, char *[],ConfigMap &);
/*! \brief This method displays a convevience 'usage' screen. */
void printUsage(char []);
/*! \brief This method reads a string and remove any blanks or comments. */
std::string strip(std::string);
/*! \brief This method populates the map. */
int parseConfig(ConfigMap &);
int getConfig(ConfigMap &,std::string,int);
double getConfig(ConfigMap &,std::string,double);
template<class T> struct range {
    T min;
    T max;
    T incr;
};
range<int> getConfig(ConfigMap &,std::string,range<int>);
range<double> getConfig(ConfigMap &,std::string,range<double>);
#endif
/* common.h */
