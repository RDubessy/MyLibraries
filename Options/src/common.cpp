#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "common.h"
bool readString(string s, ConfigMap &config, string &appKey) {
    bool res=true;
    bool verbose=(config["verbose"].size()>0);
    if(s.size()==0)                              //Empty line, skipping
        return true;
    if(s[0]=='[') {                              //Marker
        int index=s.find(']');
        switch(index) {
            case -1:
            case 1:
                if(verbose)
                    cerr << "[E] Bad marker : '" << s << "' !" << endl;
                res=false;
                break;
            default:
                appKey=s.substr(1,index-1);
                if(verbose)
                    cerr << "[I] Found marker : '" << appKey << "'." << endl;
                appKey+="::";
        }
    } else {
        //if(s[0]=='<') {                       //Option
        //int indexA=s.find(',');
        int indexB=s.find('=');
        //if(indexA>indexB)
        //    res=false;
        /*
        switch(indexA) {
            case -1:
            case 1:
                res=false;
                break;
            default:
                break;
        }
        */
        switch(indexB) {
            case -1:
            //case 2:
                res=false;
                break;
            default:
                break;
        }
        if(res) {
            //string key=s.substr(1,indexA-1);
            string key=s.substr(0,indexB);
            if(verbose)
                cerr << "[I] Found option : '" << key << "'." << endl;
            string value=s.substr(indexB+1);
            config[appKey+key]=value;
            if(verbose)
                cerr << "[I] '" << appKey+key << "=" << value << "'." << endl;
        }
        else
            cerr << "[E] Bad option : '" << s << "' !" << endl;
    }
    return res;
}
bool parseOptions(const int argc, char *argv[], ConfigMap &config) {
    bool res=true;
    if(argc==1) {
        cerr << "[W] Need at least one argument !" << endl;
        res=false;
    }
    int index;
    for(int i=1;i<argc;i++) {
        string s(argv[i]);
        if(s[0]=='-') {
            switch(s[1]) {
                case '-':                        //Long style option
                    s=s.substr(2);               //Remove '--'
                    index=s.find('=');
                    switch(index) {
                        case -1:
                            if(s.compare("usage")||s.compare("help"))
                                config[s]=" ";
                            break;
                        case 0:
                            cerr << "[W] Bad cmd line option : ' --" << s
                                << "' ! [ignored]" << endl;
                            res=false;
                            break;
                        default:
                            string key=s.substr(0,index);
                            string value=s.substr(index+1);
                            config[key]=value;
                    }
                    break;
                case 'v':
                    config["verbose"]=" ";
                    break;
                default:
                    cerr << "[W] Bad cmd line option : ' --" << s
                        << "' ! [ignored]" << endl;
                    res=false;
            }
        }
        else {                               //Valid config file ?
            if(config["configFile"].size()>0) { //Conflict !
                cerr << "[W] Conflicting config file names !" << endl;
                res=false;
            }
            else {
                config["configFile"]=s;
            }
        }
    }
    return res;
}
void printUsage(char argv[]) {
    cerr << "This program is based on the 'progTemplate' files, developped by "
        << "R. Dubessy.\n"
        << "More information at www-link\n"
        << "Usage :\n"
        << "%" << argv << " options filename\n"
        << "Where 'filename' is the name of a valid config file\n"
        << "and 'options' stand for :\n"
        << "\t--usage, --help : display this screen and exit."
        << endl;
    return;
}
string strip(string s) {
    string res("");
    int n=s.size();
    for(int i=0;i<n;i++) {
        if(s[i]=='#')
            break;
        if(s[i]!=' ')
            res+=s[i];
    }
    return res;
}
int parseConfig(ConfigMap &config) {
    string appKey("");
    ifstream file(config["configFile"].c_str(),std::ios::in);
    if(!file.good()) {
        cerr << "[E] Error opening the configuration file : '" 
            << config["configFile"] << "' !" << endl;
        return false;
    }
    bool res=true;
    while(file) {
        string s;
        getline(file,s);
        s=strip(s);
        res=res && readString(s,config,appKey);
    }
    file.close();
    return res;
}
int getConfig(ConfigMap &config, string name, int def) {
    if(config[name].size()>0)
        return atoi(config[name].c_str());
    cerr << "[W] Key : '" << name.c_str() 
        << "' not found, using default value : " << def << endl;
    return def;
}
double getConfig(ConfigMap &config, string name, double def) {
    if(config[name].size()>0)
        return atof(config[name].c_str());
    cerr << "[W] Key : '" << name.c_str() 
        << "' not found, using default value : "
        << def << endl;
    return def;
}
/* common.cpp */
