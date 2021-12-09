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
    double weight;
    if(medalCount == 0)
        weight = 2.0;
    else
        weight = 1.0 / medalCount;

    adj_list[athleteA].push_back(make_pair(athleteB, weight));
    adj_list[athleteB].push_back(make_pair(athleteA, weight));
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

    int i = 0;
    if(inFile.is_open())
    {
        string fileLine;
        getline(inFile, fileLine);

        i = 0;
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
            if(i == 50)
                break;
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

/*vector<int> dijkstra(const Graph& graph, int src)
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
}*/

/*void dijkstra(const Graph& graph, int *dist, int *prev, int start)
{
    int n = graph.vertices;

    for(int u = 0; u<n; u++) {
        dist[u] = 9999;   //set as infinity
        prev[u] = -1;    //undefined previous
    }

    dist[start] = 0;   //distance of start is 0
    set<int> S;       //create empty set S
    list<int> Q;

    for(int u = 0; u<n; u++) {
        Q.push_back(u);    //add each node into queue
    }

    while(!Q.empty()) {
        list<int> :: iterator i;
        i = min_element(Q.begin(), Q.end());
        int u = *i; //the minimum element from queue
        Q.remove(u);
        S.insert(u); //add u in the set
        for(int k = 0; k < graph.adj_list[u].size(); k++) {
            pair<int, int> it = graph.adj_list[u][k];
            if((dist[u]+(it.second)) < dist[it.first]) { //relax (u,v)
                dist[it.second] = (dist[u]+(it.first));
                prev[it.first] = u;
            }
        }
    }
}*/

int minDistance(int dist[], bool sptSet[], int V)
{

    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false &&
            dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printPath(int parent[], int j)
{

    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

    printPath(parent, parent[j]);

    printf("%d ", j);
}

void printSolution(int dist[], int n,
                   int parent[], int dest)
{
    int src = 0;
    printf("Vertex\t Distance\tPath");
    printf("\n%d -> %d \t\t %d\t\t%d ",
           src, dest, dist[dest], src);
    printPath(parent, dest);
}

// Function that implements Dijkstra's
// single source shortest path
// algorithm for a graph represented
// using adjacency matrix representation
void dijkstra(const Graph& graph, int src, int dest)
{

    // The output array. dist[i]
    // will hold the shortest
    // distance from src to i
    int dist[graph.vertices];

    // sptSet[i] will true if vertex
    // i is included / in shortest
    // path tree or shortest distance
    // from src to i is finalized
    bool sptSet[graph.vertices];

    // Parent array to store
    // shortest path tree
    int parent[graph.vertices];

    // Initialize all distances as
    // INFINITE and stpSet[] as false
    for (int i = 0; i < graph.vertices; i++)
    {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    // Distance of source vertex
    // from itself is always 0
    dist[src] = 0;

    // Find shortest path
    // for all vertices
    for (int count = 0; count < graph.vertices - 1; count++)
    {
        // Pick the minimum distance
        // vertex from the set of
        // vertices not yet processed.
        // u is always equal to src
        // in first iteration.
        int u = minDistance(dist, sptSet, graph.vertices);

        // Mark the picked vertex
        // as processed
        sptSet[u] = true;

        // Update dist value of the
        // adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < graph.vertices; v++)

            // Update dist[v] only if is
            // not in sptSet, there is
            // an edge from u to v, and
            // total weight of path from
            // src to v through u is smaller
            // than current value of
            // dist[v]
            if (!sptSet[v] && graph.adj_list[u][v].second && dist[u] + graph.adj_list[u][v].second < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph.adj_list[u][v].second;
            }
    }

    // print the constructed
    // distance array
    printSolution(dist, graph.vertices, parent, dest);
}

// not totally correct :)
void bfs(const Graph& graph, int src, int end) {
    set<int> visited;
    queue<int> search;

    visited.insert(src);
    search.push(src);

    while(!search.empty())
    {
        int element = search.front();
        cout << element << " ";
        search.pop();

        vector<pair<int, double>> connections = graph.adj_list[element];
        sort(connections.begin(), connections.begin() + connections.size());

        for(int i = 0; i < connections.size(); i++)
        {
            if(visited.count(connections[i]) == 0)
            {
                visited.insert(connections[i]);
                search.push(connections[i]);
            }
        }
    }
}


int main()
{
    vector<athlete> athletes;
    readAthleteCSV("olympicdata.csv", athletes);

    vector<pair<int, int>> pairs;
    readPairsCSV("pairs.csv", pairs);
    auto olympicGraph = Graph(athletes.size());
    cout << "here";

    for(int i = 0; i < pairs.size(); i++)
    {
        olympicGraph.addEdge(pairs[i].first, pairs[i].second, athletes[pairs[i].first].medalCount + athletes[pairs[i].second].medalCount);
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

        cout << endl << "Connecting " << athletes[stoi(athlete1)].name << " to " << athletes[stoi(athlete2)].name << "..." << endl;

        /*vector<pair<int, int>> pairs;
        int athlete2int = stoi(athlete2);
        map<int, bool> checked;
        makeGraphPairs(checked, stoi(athlete1), athlete2int, pairs, athletes);*/

        cout << "Dijkstra's Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
        // dijkstra's
        int dist[olympicGraph.vertices], prev[olympicGraph.vertices];
        int begin = stoi(athlete1);
        int end = stoi(athlete2);
        auto start = high_resolution_clock::now();
       // dijkstra(olympicGraph, begin, end);
        auto stop = high_resolution_clock::now();
        auto duration1 = duration_cast<seconds>(stop - start);

        cout << "The Bellman-Ford Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
        // bellman ford
        start;
        stop;
        auto duration2 = duration_cast<seconds>(stop - start);

        cout << "Breadth-First Search found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
        // bfs or something
        start;
        bfs(olympicGraph, begin, end);
        stop;
        auto duration3 = duration_cast<seconds>(stop - start);

        cout << "The diagnostics for the performance of these algorithms was: " << endl;
        cout << "Dijkstra's Algorithm: " << duration1.count() << " seconds" << endl;
        cout << "Bellman-Ford Algorithm: " << duration2.count() << " seconds" << endl;
        cout << "Breadth-First Search: " << duration3.count() << " seconds" << endl;
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
