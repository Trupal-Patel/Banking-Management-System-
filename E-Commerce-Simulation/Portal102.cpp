#include "Portal102.h"
using namespace std;

void Portal102 :: processUserCommand(string command) {
    requestID++;
    ofstream myFile("PortalToPlatform.txt", ios::app);
    //writinf to file with coressponding commands and mapping request id with types
    if(command == "Start"){
        myFile << portalID << " " << requestID << " " << command << endl;
        types[requestID] = "Start";
    }
    else if(command == "Check"){
        types[requestID] = "Check";
        checkResponse();
    }
    else if(command.substr(0, 3) == "Buy"){
        myFile << portalID << " " << requestID << " " << command << endl;
        types[requestID] = "Buy";
    }
    else if(command.substr(0, 4) == "List"){
        for(int i=command.size()-1; i>=0; i--){
            if(command[i] == ' '){
                myFile << portalID << " " << requestID << " " << command.substr(0, i) << endl;
                sortOrder.push(command.substr(i+1, command.length()-i-1));
                break;
            }
        }
        types[requestID] = "List";
    }
    myFile.close();
}

void Portal102 :: checkResponse() {
    while(lastResponseID < requestID) {
        lastResponseID++;   //maintaining a last read response id counter
        string text, sortorder;
        string qtype = types[lastResponseID];
        types.erase(lastResponseID);
        vector<pair<string, vector<string>>> toSortName;    //vectors for sorting
        vector<pair<int, vector<string>>> toSortPrice;

        if(qtype=="List"){
            sortorder = sortOrder.front();
            sortOrder.pop();
        }

        ifstream myFile("PlatformToPortal.txt");
        while(getline(myFile, text)){
            int s=0, l=0;
            for(int i=0; i<text.length(); i++){
                if(text[i]==' ' && s==0){
                    s=i+1;
                }
                else if(text[i]==' '){
                    l=i-s;
                    break;
                }
            }
            int currResponseID = stoi(text.substr(s, l));
            if(currResponseID < lastResponseID) continue;
            if(currResponseID > lastResponseID) break;
            //extracting every word of the line in a vector
            vector<string> tmp;
            string word = "";
            for (auto x : text)
            {
                if (x == ' ')
                {
                    tmp.push_back(word);
                    word = "";
                }
                else {
                    word = word + x;
                }
            }
            tmp.push_back(word);

            //logic coressponding to each query type

            if(qtype == "Start"){
                cout << endl << "-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x" << endl;
                cout << endl << "Hi! Here are the list of categories you would want to go through!" << endl << endl;
                for(int i = 2; i < tmp.size(); i++){
                    cout << tmp[i] << endl;
                }

            }
            else if(qtype == "Buy"){
                if(tmp[2] == "Success"){
                    cout << endl << "-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x" << endl;
                    cout << endl << "Congratulations! Your purchase was successful" << endl;
                }
                else{
                    cout << endl << "-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x" << endl;
                    cout << endl << "Oops! We might not have those many of the product or the ProductID is incorrect!" << endl;
                }
            }
            else if(qtype == "List"){
                
                toSortName.push_back({tmp[2], tmp});
                toSortPrice.push_back({stoi(tmp[4]), tmp});
            }
        }
        myFile.close();
        //additional printing if the query type was List
        if(qtype == "List"){
            if(sortorder == "Name"){
                sort(toSortName.begin(), toSortName.end());
                cout << endl << "-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x" << endl;
                cout << endl << "There you go, all the products you would want to see sorted by name." << endl << endl;
                for(auto i : toSortName){
                    cout << i.second[2] << " " << i.second[3] << " " << i.second[4] << " " << i.second[5] << endl;
                }
            }
            else if(sortorder == "Price"){
                cout << endl << "-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x" << endl;
                cout << endl << "There you go, all the products you would want to see sorted by price." << endl << endl;
                sort(toSortPrice.begin(), toSortPrice.end());
                for(auto i : toSortPrice){
                    cout << i.second[2] << " " << i.second[3] << " " << i.second[4] << " " << i.second[5] << endl;
                }
            }
        }
    }
}