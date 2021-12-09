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
using namespace std::chrono;

// graph implementation will be an adjacency list
class Graph
{
public:
    vector<vector<pair<int, double>>> adj_list;
    int vertices;
    Graph(int n)
    {
        adj_list.resize(n);
    }
    void addEdge(int athleteA, int athleteB, int medalCount);
};

void Graph::addEdge(int athleteA, int athleteB, int medalCount)
{
    // taking the inverse of total medal count across athletes to use for weight-based algorithms
    double weight;
    if(medalCount == 0)
        weight = 2.0;
    else
        weight = 1.0 / medalCount;

    adj_list[athleteA].push_back(make_pair(athleteB, weight));
    adj_list[athleteB].push_back(make_pair(athleteA, weight));
}

// prints athlete info in condensed version, to be used in displaying paths
void printBasicInfo(int athleteID, vector<athlete> &athletes)
{
    cout << "Athlete " << athletes[athleteID].index << ": " << athletes[athleteID].name << ", " << athletes[athleteID].olympicTeam << ", " << athletes[athleteID].sport;
}

// prints the full athlete information upon user's request
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

// used in reading in the athlete data to create an athlete object in the right format
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

// reads in athlete data from CSV file
void readAthleteCSV(string filename, vector<athlete> &athletes)
{
    ifstream inFile(filename);

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

// reads in pairs data from CSV file
// NOTE: this will take 7-10 minutes to read in as it is a LARGE FILE, but it only gets read in once upon initial start of the program
void readPairsCSV(string filename, vector<pair<int, int>> &pairs)
{
    ifstream inFile(filename);

    int i = 0;
    if(inFile.is_open())
    {
        string fileLine;
        getline(inFile, fileLine);

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

            if(athleteAstr != "NA")
            {
                int athleteA = stoi(athleteAstr);
                int athleteB = stoi(athleteBstr);

                pairs.push_back(make_pair(athleteA, athleteB));
            }
            i++;
            if(i == 500)
                break;
        }
    }
}

void printPath(vector<int> parent, int j, vector<int> &path)
{

    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

    printPath(parent, parent[j], path);

    path.push_back(j);
}

vector<int> printSolution(vector<int> parent, int dest, int src)
{
    vector<int> path;
    path.push_back(src);
    printPath(parent, dest, path);
    return path;
}

vector<int> dijkstra(const Graph& graph, vector<int> &dist, vector<int> &prev, int src, int dest, bool &success)
{
    vector<int> distance(graph.vertices);
    bool visited[graph.vertices];
    vector<int> parent(graph.vertices);
    for(int i = 0; i < graph.vertices; i++)
    {
        parent[i] = -1;
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
                parent[v] = u;
                distance[v] = distance[u] + weight;
                pq.push(make_pair(distance[v], v));
            }
        }
    }

    if(parent[dest] != -1){
        success = true;
        return printSolution(parent, dest, src);
    }
    success = false;
    return parent;
}

vector<int> bellmanFord(Graph& graph, int src, int dest, bool &success) {
    //Step 1 initialize everything
    int dist[graph.vertices];
    bool sptSet[graph.vertices];
    vector<int> prev(graph.vertices);

    for (int i = 0; i < graph.vertices; i++) {
        dist[i] = INT_MAX;
        prev[0] = -1;
        sptSet[i] = false;
    }
    //Step 2 Relax Edges
    dist[src] = 0;
    for (int i = 0; i < graph.vertices; i++) {
        for (int j = 0; j < graph.adj_list[i].size(); j++) {
            auto u = i;
            auto v = graph.adj_list[i][j].first; //to vertex
            double w = graph.adj_list[i][j].second; //weight of edge
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }
    //Step 3, negative cycles
    for (int i = 0; i < graph.vertices; i++) {
        for (int j = 0; j < graph.adj_list[i].size(); j++) {
            int u = i;
            int v = graph.adj_list[i][j].first;
            double w = graph.adj_list[i][j].second;
            if (dist[u] + w < dist[v]) {
                cout << "Graph contains a negative";
                success = false;
                return prev;
            }
        }
    }

    if(prev[dest] != -1){
        success = true;
        return printSolution(prev, dest, src);
    }
    success = false;
    return prev;
}

// performs breadth-first search to find shortest path between two athletes
vector<int> BFS(Graph &graph, int start, int end, int vertices, int prev[], int dist[], bool &success)
{
    success = false;
    list<int> order;

    bool visited[vertices];

    for (int i = 0; i < vertices; i++)
    {
        visited[i] = false;
        dist[i] = INT_MAX;
        prev[i] = -1;
    }

    visited[start] = true;
    dist[start] = 0;
    order.push_back(start);

    while (order.size() != 0) {
        int u = order.front();
        order.pop_front();
        for (int i = 0; i < graph.adj_list[u].size(); i++)
        {
            if (!visited[graph.adj_list[u][i].first])
            {
                visited[graph.adj_list[u][i].first] = true;
                dist[graph.adj_list[u][i].first] = dist[u] + 1;
                prev[graph.adj_list[u][i].first] = u;
                order.push_back(graph.adj_list[u][i].first);

                if (graph.adj_list[u][i].first == end)
                    success = true;
            }
        }
    }

    vector<int> path;
    int check = end;
    path.push_back(check);
    while(prev[check] != -1)
    {
        path.push_back(prev[check]);
        check = prev[check];
    }

    return path;
}

int main()
{
    // read in athlete data
    vector<athlete> athletes;
    readAthleteCSV("olympicdata.csv", athletes);

    // read in edges
    vector<pair<int, int>> pairs;
    readPairsCSV("pairs.csv", pairs);
    auto olympicGraph = Graph(athletes.size());
    olympicGraph.vertices = athletes.size();

    // build the graph
    for(int i = 0; i < pairs.size(); i++)
    {
        olympicGraph.addEdge(pairs[i].first, pairs[i].second, athletes[pairs[i].first].medalCount + athletes[pairs[i].second].medalCount);
    }

    string keepgoing = "yes";
    while(keepgoing == "yes")
    {
        string athlete1;
        string athlete2;
        cout
                << "     _______________" << endl
                << "    |@@@@|     |####|" << endl
                << "    |@@@@|     |####|" << endl
                << "    |@@@@|     |####|" << endl
                << "    \\@@@@|     |####/" << endl
                << "     \\@@@|     |###/" << endl
                << "      `@@|_____|##'" << endl
                << "           (O)" << endl
                << "        .-'''''-." << endl
                << "      .'  * * *  `." << endl
                << "     :  *       *  :" << endl
                << "    : ~  O A C C  ~ :" << endl
                << "    : ~ A W A R D ~ :" << endl
                << "     :  *       *  :" << endl
                << "      `.  * * *  .'" << endl
                << "        `-.....-'" << endl;

        cout << "Welcome to the Five Rings of Separation!" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter the indices of the athletes you'd like to connect: (0 - " << athletes.size() - 1 << ")" << endl;
        cout << "Athlete 1: ";
        cin >> athlete1;
        cout << "Athlete 2: ";
        cin >> athlete2;

        cout << endl << "Connecting " << athletes[stoi(athlete1)].name << " to " << athletes[stoi(athlete2)].name << "..." << endl << endl;

        // BREADTH-FIRST SEARCH
        //  since this is the quickest, it will be used to check if there's a full path between two athletes before the longer algorithms even run
        bool success = false;
        auto start3 = high_resolution_clock::now();
        int pred[olympicGraph.vertices], dist[olympicGraph.vertices];
        vector<int> path = BFS(olympicGraph, stoi(athlete1), stoi(athlete2), olympicGraph.vertices, pred, dist, success);
        auto stop3 = high_resolution_clock::now();
        if(!success) // only prints a path if there is a full one between two athletes from BFS
        {
            cout << "It looks like there is no path that fully connects " <<  athletes[stoi(athlete2)].name << " and " << athletes[stoi(athlete1)].name << "." << endl << endl;
        }
        else
        {
            cout << "A path was found between your athletes!" << endl;
            cout << "Breadth-First Search found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            for(int i = path.size() - 1; i >= 0; i--)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }

            auto duration3 = duration_cast<microseconds>(stop3 - start3);

            // DIJKSTRA'S ALGORITHM
            cout << endl << "Dijkstra's Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            // dijkstra's
            //int dist[olympicGraph.vertices], prev[olympicGraph.vertices];
            // int begin = stoi(athlete1);
            // int end = stoi(athlete2);
            vector<int> dist(olympicGraph.vertices), prev(olympicGraph.vertices);
            int begin = stoi(athlete1);
            int end = stoi(athlete2);
            bool success = false;
            auto start1 = high_resolution_clock::now();
            vector<int> path = dijkstra(olympicGraph, dist, prev, begin, end, success);
            auto stop1 = high_resolution_clock::now();
            for(int i = 0; i < path.size(); i++)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
            auto duration1 = duration_cast<microseconds>(stop1 - start1);

            // BELLMAN-FORD ALGORITHM
            cout << endl << "The Bellman-Ford Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            auto start2 = high_resolution_clock::now();
            success = false;
            path = bellmanFord(olympicGraph, stoi(athlete1), stoi(athlete2), success);
            for(int i = 0; i < path.size(); i++)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<microseconds>(stop2 - start2);

            // comparing the performance of the algorithms
            cout << endl << endl << "The diagnostics for the performance of these algorithms was: " << endl;
            cout << "-------------------------------------------------------------" << endl;
            cout << "Breadth-First Search: " << duration3.count() << " microseconds" << endl;
            cout << "Dijkstra's Algorithm: " << duration1.count() << " microseconds" << endl;
            cout << "Bellman-Ford Algorithm: " << duration2.count() << " microseconds" << endl;
        }

        // users can ask for more detailed information about any athlete from the path
        string moreInfo;
        cout << "Would you like more in-depth information about any given athlete? (yes/no): ";
        cin >> moreInfo;

        // allow users to ask for more information about as many athletes as they'd like
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

        // if users want to search for another pair, they can continue to do so
        cout << endl << "Would you like to connect another pair of athletes? (yes/no): ";
        cin >> keepgoing;
    }
}
