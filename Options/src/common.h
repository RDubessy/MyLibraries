#ifndef COMMON_H
#define COMMON_H
#include <string>
#include <map>
using std::string;
using std::map;
using std::cerr;
using std::endl;
using std::ifstream;
/*! \brief This type contains the options, as a map of string pairs, keys and
 * values */
typedef map<string,string> ConfigMap;
/*! \brief This method reads a string and add an entry in the map. */
bool readString(string,ConfigMap &,string &);
/*! \brief This method reads the cmd lines options and trigger the program
 * execution. */
bool parseOptions(const int, char *[],ConfigMap &);
/*! \brief This method displays a convevience 'usage' screen. */
void printUsage(char []);
/*! \brief This method reads a string and remove any blanks or comments. */
string strip(string);
/*! \brief This method populates the map. */
int parseConfig(ConfigMap &);
int getConfig(ConfigMap &,string,int);
double getConfig(ConfigMap &,string,double);
#endif
/* common.h */
