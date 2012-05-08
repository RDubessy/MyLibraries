#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "common.h"
bool readString(std::string s, ConfigMap &config, std::string &appKey) {
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
                    std::cerr << "[E] Bad marker : '" << s << "' !" << std::endl;
                res=false;
                break;
            default:
                appKey=s.substr(1,index-1);
                if(verbose)
                    std::cerr << "[I] Found marker : '" << appKey << "'." << std::endl;
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
            std::string key=s.substr(0,indexB);
            if(verbose)
                std::cerr << "[I] Found option : '" << key << "'." << std::endl;
            key=appKey+key;
            std::string value=s.substr(indexB+1);
            if(config[key].size()==0)     //Do not erase previous value.
                config[key]=value;
            if(verbose)
                std::cerr << "[I] '" << appKey+key << "=" << value << "'." << std::endl;
        }
        else
            std::cerr << "[E] Bad option : '" << s << "' !" << std::endl;
    }
    return res;
}
bool parseOptions(const int argc, char *argv[], ConfigMap &config) {
    bool res=true;
    if(argc==1) {
        std::cerr << "[W] Need at least one argument !" << std::endl;
        res=false;
    }
    int index;
    for(int i=1;i<argc;i++) {
        std::string s(argv[i]);
        if(s[0]=='-') {
            switch(s[1]) {
                case '-':                        //Long style option
                    s=s.substr(2);               //Remove '--'
                    index=s.find('=');
                    switch(index) {
                        case -1:
                            config[s]="";
                            break;
                        case 0:
                            std::cerr << "[W] Bad cmd line option : ' --" << s
                                << "' ! [ignored]" << std::endl;
                            res=false;
                            break;
                        default:
                            std::string key=s.substr(0,index);
                            std::string value=s.substr(index+1);
                            config[key]=value;
                    }
                    break;
                case 'v':
                    config["verbose"]=" ";
                    break;
                default:
                    std::cerr << "[W] Bad cmd line option : ' --" << s
                        << "' ! [ignored]" << std::endl;
                    res=false;
            }
        }
        else {                               //Valid config file ?
            if(config["configFile"].size()>0) { //Conflict !
                std::cerr << "[W] Conflicting config file names !" << std::endl;
                res=false;
            } else {
                config["configFile"]=s;
            }
        }
    }
    return res;
}
void printUsage(char argv[]) {
    std::cerr << "This program is based on the 'progTemplate' files, developped by "
        << "R. Dubessy.\n"
        << "More information at www-link\n"
        << "Usage :\n"
        << "%" << argv << " options filename\n"
        << "Where 'filename' is the name of a valid config file\n"
        << "and 'options' stand for :\n"
        << "\t--usage, --help : display this screen and exit."
        << std::endl;
    return;
}
std::string strip(std::string s) {
    std::string res("");
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
    std::string appKey("");
    std::ifstream file(config["configFile"].c_str(),std::ios::in);
    if(!file.good()) {
        std::cerr << "[E] Error opening the configuration file : '" 
            << config["configFile"] << "' !" << std::endl;
        return false;
    }
    bool res=true;
    while(file) {
        std::string s;
        getline(file,s);
        s=strip(s);
        res=res && readString(s,config,appKey);
    }
    file.close();
    return res;
}
int getConfig(ConfigMap &config, std::string name, int def) {
    if(config[name].size()>0)
        return atoi(config[name].c_str());
    std::cerr << "[W] Key : '" << name.c_str() 
        << "' not found, using default value : " << def << std::endl;
    return def;
}
double getConfig(ConfigMap &config, std::string name, double def) {
    if(config[name].size()>0)
        return atof(config[name].c_str());
    std::cerr << "[W] Key : '" << name.c_str() 
        << "' not found, using default value : "
        << def << std::endl;
    return def;
}
range<int> getConfig(ConfigMap &config,std::string name,range<int> def) {
    std::string rval=config[name];
    if(rval.size()==0)
        return def;
    range<int> res=def;
    int index=rval.find(':');
    switch(index) {
        case 0:
            break;
        case -1:
            res.min=res.max=atoi(rval.c_str());
            res.incr=1;
            break;
        default:
            {
                res.min=atoi(rval.substr(0,index).c_str());
                rval=rval.substr(index+1);
                if(rval.size()==0) {
                    res.max=res.min;
                    res.incr=1;
                    break;
                }
                int index2=rval.find(':');
                switch(index2) {
                    case 0:
                        res.max=atoi(rval.substr(1).c_str());;
                        res.incr=1;
                        break;
                    case -1:
                        res.max=atoi(rval.c_str());
                        res.incr=1;
                        break;
                    default:
                        res.incr=atoi(rval.substr(0,index2).c_str());
                        res.max=atoi(rval.substr(index2+1).c_str());
                }
            }
    }
    return res;
}
range<double> getConfig(ConfigMap &config,std::string name,range<double> def) {
    std::string rval=config[name];
    if(rval.size()==0)
        return def;
    range<double> res=def;
    int index=rval.find(':');
    switch(index) {
        case 0:
            break;
        case -1:
            res.min=res.max=atof(rval.c_str());
            res.incr=1;
            break;
        default:
            {
                res.min=atof(rval.substr(0,index).c_str());
                rval=rval.substr(index+1);
                if(rval.size()==0) {
                    res.max=res.min;
                    res.incr=1;
                    break;
                }
                int index2=rval.find(':');
                switch(index2) {
                    case 0:
                        res.max=atof(rval.substr(1).c_str());;
                        res.incr=1;
                        break;
                    case -1:
                        res.max=atof(rval.c_str());
                        res.incr=1;
                        break;
                    default:
                        res.incr=atof(rval.substr(0,index2).c_str());
                        res.max=atof(rval.substr(index2+1).c_str());
                }
            }
    }
    return res;
}
/* common.cpp */
