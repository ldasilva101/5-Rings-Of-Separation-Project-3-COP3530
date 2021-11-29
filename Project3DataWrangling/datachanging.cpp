#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <map>
#include "athlete.h"
using namespace std;

class Graph{
public:
    vector<pair<int, int>> *adj_list;
    int vertices;
    Graph(int n){
        vertices = n;
        adj_list = new vector<pair<int, int>>[n];
    }
    void addEdge(int athleteA, int athleteB, int medCount);
};

void Graph::addEdge(int athleteA, int athleteB, int medCount){
    adj_list[athleteA].emplace_back(athleteB, medCount);
    adj_list[athleteB].emplace_back(athleteA, medCount);
}


void addAthlete(string name, string team, string sport, string event, string medal, vector<athlete> &athletes){
    bool duplicate = false;
    vector<pair<string, string>> events;
    for(int i = 0; i < athletes.size(); i++){
        if(athletes[i].name == name && athletes[i].olympicTeam == team && athletes[i].sport == sport){
            athletes[i].events.emplace_back(event, medal);
            if(!medal.empty()){
                athletes[i].medalCount++;
            }
            duplicate = true;
        }
    }

    if(!duplicate){
        events.emplace_back(event, medal);
        int medalCount = 0;
        if(!medal.empty()){
            medalCount++;
        }
        athletes.emplace_back(name, team, sport, medalCount, events);
    }

}

void readCSV(string filename, vector<athlete> &athletes){
    ifstream inFile(filename);

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

            addAthlete(name, team, sport, event, medal, athletes);
        }
    }
}

int main() {
    vector<athlete> athletes;
    readCSV("olympicdata.csv", athletes);
    for(int i = 0; i < athletes.size(); i++){
        cout << athletes[i].name << endl;
        cout << athletes[i].medalCount << endl;
    }

    auto *olympicGraph = new Graph(athletes.size());

    for(int i = 0; i < athletes.size(); i++){
        for(int j = 0; j < athletes.size(); j++){
            if(i != j && (athletes[i].olympicTeam == athletes[j].olympicTeam || athletes[i].sport == athletes[j].sport)){
                olympicGraph->addEdge(i, j, athletes[i].medalCount + athletes[j].medalCount);
            }
        }
    }
}
