#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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
            string event;
            string medal;
            string team;
            string sport;

            getline(stream, row, ',');
            getline(stream, name, ',');
            getline(stream, event, ',');
            getline(stream, medal, ',');
            getline(stream, team, ',');
            getline(stream, sport, ',');
        }
    }
}

