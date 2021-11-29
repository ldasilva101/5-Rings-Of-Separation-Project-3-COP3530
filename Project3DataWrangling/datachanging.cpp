#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "athlete.h"
using namespace std;

int main() {
    vector<string> athleteNames;
    ifstream inFile("olympicdata.csv");

    if(!inFile.is_open())
    {
        cout << "Error: could not open file." << endl;
    }
    if(inFile.is_open())
    {
        string fileLine;
        getline(inFile, fileLine);

        while (getline(inFile, fileLine)) {
            istringstream stream(fileLine);

            string row;
            string name;
            string team;
            string sport;
            string event;
            string medal;

            getline(stream, row, ',');
            getline(stream, name, ',');
            getline(stream, team, ',');
            getline(stream, sport, ',');
            getline(stream, event, ',');
            getline(stream, medal, ',');

            name = name.substr(1, name.size() - 2);
            team = team.substr(1, team.size() - 2);
            sport = sport.substr(1, sport.size() - 2);
            event = event.substr(1, event.size() - 2);
            medal = medal.substr(1, medal.size() - 2);
            athleteNames.push_back(event);
        }
    }

    for(int i = 0; i < athleteNames.size(); i++)
    {
        cout << athleteNames[i] << endl;
    }
}

