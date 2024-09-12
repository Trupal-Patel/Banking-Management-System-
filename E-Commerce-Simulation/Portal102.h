#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include<algorithm>

#include "Portal.h"

using namespace std;

#ifndef PORTAL102_H
#define PORTAL102_H

class Portal102 : public Portal {

    // To change portalID to something unique like roll number.
    int lastResponseID = 100, requestID = 100;
    string portalID = "056070102";
    map<int, string> types; //map to store request id with the type of request
    queue<string> sortOrder;    //queue to maintain the sort order

    public:
    
    void processUserCommand(string command);    //method to process user commands

    void checkResponse();   //method to check for response from platform

};

#endif