#include <iostream>
#include <common.h>
using namespace std;
int main(int argc, char *argv[]) {
    ConfigMap config;
    if(!parseOptions(argc,argv,config)) {        //Parse cmd line options
        cerr << "==> Try '" << argv[0] << " --usage'" << endl;
        return -1;
    }
    if(config["usage"].size()>0||config["help"].size()>0) {
        printUsage(argv[0]);
        return -1;
    }
    if(!parseConfig(config)) {
        cerr << "==> Try 'man " << argv[0] << "'" << endl;
        return -1;
    }
    return 0;
}
/* main.cpp */
