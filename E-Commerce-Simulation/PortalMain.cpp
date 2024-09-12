#include<iostream>
#include "Portal102.h"
using namespace std;

int main(){
    Portal102 portal;
    while(true){
        string s;
        getline(cin, s);
        if(s == "End") break;
        else portal.processUserCommand(s);
    }

    std::ofstream ofs;
    ofs.open("PortalToPlatform.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    
    return 0;
}