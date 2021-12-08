#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <queue>
#include "athlete.h"
using namespace std;

class Graph
{
public:
    vector<vector<pair<int, int>>> adj_list;
    int vertices;
    Graph(int n)
    {
        vertices = n;
    }
    void addEdge(int athleteA, int athleteB, int medCount);
};

void Graph::addEdge(int athleteA, int athleteB, int medCount)
{
    adj_list[athleteA].push_back(make_pair(athleteA, athleteB));
    adj_list[athleteB].push_back(make_pair(athleteA, athleteB));
}

void printBasicInfo(int athleteID, vector<athlete> &athletes)
{
    cout << "Athlete " << athletes[athleteID].index << ": " << athletes[athleteID].name << ", " << athletes[athleteID].olympicTeam << ", " << athletes[athleteID].sport;
}

void printFullInfo(int athleteID, vector<athlete> &athletes)
{
    cout << "Name: " << athletes[athleteID].name << endl;
    cout << "Team: " << athletes[athleteID].olympicTeam << endl;
    cout << "Sport: " << athletes[athleteID].sport << endl;
    cout << "Total Medal Count: " << athletes[athleteID].medalCount << endl;
    cout << "Events:" << endl;
    for(int i = 0; i < athletes[athleteID].events.size(); i++)
    {
        cout << athletes[athleteID].events[i] << endl;
    }
}

void addAthlete(int index, string name, string team, string sport, string event, vector<athlete> &athletes){
    vector<string> events;

    size_t start;
    size_t end = 0;
    while((start = event.find_first_not_of("$", end)) != string::npos)
    {
        end = event.find("$", start);
        events.push_back(event.substr(start, end - start));
    }

    int medalCount = 0;
    for(int i = 0; i < events.size(); i++)
    {
        if(events[i].find("( NA )") == string::npos)
            medalCount++;
    }

    index = index - 1;
    athletes.emplace_back(index, name, team, sport, medalCount, events);
}

void readAthleteCSV(string filename, vector<athlete> &athletes)
{
    ifstream inFile(filename);

    if(!inFile.is_open())
        cout << "Error: could not open file." << endl;

    if(inFile.is_open())
    {
        string fileLine;
        getline(inFile, fileLine);

        while (getline(inFile, fileLine))
        {
            istringstream stream(fileLine);

            string row;
            string name;
            string team;
            string sport;
            string event;
            int index;

            getline(stream, row, ',');
            getline(stream, name, ',');
            getline(stream, team, ',');
            getline(stream, sport, ',');
            getline(stream, event, ',');

            row = row.substr(1, row.size() - 2);
            index = stoi(row);
            name = name.substr(1, name.size() - 2);
            team = team.substr(1, team.size() - 2);
            sport = sport.substr(1, sport.size() - 2);
            event = event.substr(1, event.size() - 2);

            addAthlete(index, name, team, sport, event, athletes);
        }
    }
}

void readPairsCSV(string filename, vector<pair<int, int>> &pairs)
{
    ifstream inFile(filename);

    if(!inFile.is_open())
        cout << "Error: could not open file." << endl;

    if(inFile.is_open())
    {
        string fileLine;
        getline(inFile, fileLine);

        int i = 0;
        while (getline(inFile, fileLine))
        {
            istringstream stream(fileLine);

            string line;
            string row;
            string athleteAstr;
            string athleteBstr;

            getline(stream, line, ',');
            getline(stream, row, ',');
            getline(stream, athleteAstr, ',');
            getline(stream, athleteBstr, ',');

            cout << i << endl;
            i++;
            if(athleteAstr != "NA")
            {
                int athleteA = stoi(athleteAstr);
                int athleteB = stoi(athleteBstr);

                pairs.push_back(make_pair(athleteA, athleteB));
            }
        }
    }
}

/*void makeGraphPairs(map<int, bool> &checked, int athlete1, int &athlete2, vector<pair<int, int>> &pairs, vector<athlete> &athletes)
{
    checked[athlete1] = true;

    for(int i = 0; i < athletes.size(); i++)
    {
        if(i != athlete1 && (athletes[i].olympicTeam == athletes[athlete1].olympicTeam || athletes[i].sport == athletes[athlete1].sport))
        {
            pairs.push_back(make_pair(athlete1, i));
            if(i == athlete2)
            {
                cout << "goal reached" << endl;
                break;
            }
            else if(!checked[i])
                makeGraphPairs(checked, i, athlete2, pairs, athletes);
        }
    }
}*/

vector<int> dijkstra(const Graph& graph, int src)
{
    vector<int> distance(graph.vertices);
    bool visited[graph.vertices];

    for(int i = 0; i < graph.vertices; i++)
    {
        distance[i] = numeric_limits<int>::max();
        visited[i] = false;
    }

    distance[src] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, src));
    while(!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        for(int i = 0; i < graph.adj_list[u].size(); i++)
        {
            int v = graph.adj_list[u][i].first;
            int weight = graph.adj_list[u][i].second;

            if(distance[v] > distance[u] + weight)
            {
                distance[v] = distance[u] + weight;
                pq.push(make_pair(distance[v], v));
            }
        }
    }
    return distance;
}

int main()
{
    vector<athlete> athletes;
    readAthleteCSV("olympicdata.csv", athletes);

    vector<pair<int, int>> pairs;
    readPairsCSV("pairs.csv", pairs);
    auto olympicGraph = new Graph(athletes.size());

    for(int i = 0; i < pairs.size(); i++)
    {
        olympicGraph->addEdge(pairs[i].first, pairs[i].second, athletes[pairs[i].first].medalCount + athletes[pairs[i].second].medalCount);
    }

    string keepgoing = "yes";
    while(keepgoing == "yes")
    {
        string athlete1;
        string athlete2;

        cout << "Welcome to the Olympic Athlete Connection Center!" << endl;
        cout << "Enter the indices of the athletes you'd like to connect: " << endl;
        cout << "Athlete 1: ";
        cin >> athlete1;
        cout << "Athlete 2: ";
        cin >> athlete2;

        cout << "Connecting " << athletes[stoi(athlete1)].name << " to " << athletes[stoi(athlete2)].name << "..." << endl;

        /*vector<pair<int, int>> pairs;
        int athlete2int = stoi(athlete2);
        map<int, bool> checked;
        makeGraphPairs(checked, stoi(athlete1), athlete2int, pairs, athletes);*/

        // dijkstra's
        // bellman ford
        // bfs or something
        // print basic info in these

        // after doing the paths between athletes, ask user if they'd like some in-depth information about any of them
        string moreInfo;
        cout << "Would you like more in-depth information about any given athlete? (yes/no): ";
        cin >> moreInfo;

        while(moreInfo == "yes")
        {
            cout << "Enter the ID of the athlete you'd like more information for: ";
            string index;
            cin >> index;
            int i = stoi(index);
            cout << endl;
            printFullInfo(i, athletes);
            cout << endl << "Would you like more information about another athlete? (yes/no): ";
            cin >> moreInfo;
        }

        cout << endl << "Would you like to connect another pair of athletes? (yes/no): ";
        cin >> keepgoing;
    }
}
