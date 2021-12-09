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

void bellmanFord(Graph& graph, int src, int dest) {
    //Step 1 initialize everything
    int dist[graph.vertices];
    bool sptSet[graph.vertices];
    int prev[graph.vertices];

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
                return;
            }
        }
    }
    printSolution(dist, graph.vertices, prev, dest);
}

// performs breadth-first search to find a path between two athletes
vector<int> bfs(const Graph& graph, pair<int, double> src, pair<int, double> end, vector<athlete> &athletes, bool &success) {
    vector<int> path;

    success = false;

    set<pair<int,double>> visited;
    queue<pair<int,int>> search;

    visited.insert(src);
    search.push(src);

    while(!search.empty())
    {
        int element = search.front().first;
        path.push_back(element);
        if(element == end.first)
        {
            success = true;
            break;
        }
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

        cout << "Welcome to the Five Rings of Separation!" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter the indices of the athletes you'd like to connect: (0 - " << athletes.size() - 1 << ")" << endl;
        cout << "Athlete 1: ";
        cin >> athlete1;
        cout << "Athlete 2: ";
        cin >> athlete2;

        cout << endl << "Connecting " << athletes[stoi(athlete1)].name << " to " << athletes[stoi(athlete2)].name << "..." << endl << endl;

        /*vector<pair<int, int>> pairs;
        int athlete2int = stoi(athlete2);
        map<int, bool> checked;
        makeGraphPairs(checked, stoi(athlete1), athlete2int, pairs, athletes);*/

        // DIJKSTRA'S ALGORITHM
        cout << "Finding the connections between  " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << " using Dijkstra's Algorithm:" << endl;
        // dijkstra's
        //int dist[olympicGraph.vertices], prev[olympicGraph.vertices];
       // int begin = stoi(athlete1);
       // int end = stoi(athlete2);
        auto start1 = high_resolution_clock::now();
       // dijkstra(olympicGraph, begin, end);
        auto stop1 = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(stop1 - start1);

        // BELLMAN-FORD ALGORITHM
        cout << "Finding the connections between  " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << " using the Bellman-Ford Algorithm:" << endl;
        auto start2 = high_resolution_clock::now();
        bellmanFord(olympicGraph, stoi(athlete1), stoi(athlete2));
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);

        // BREADTH-FIRST SEARCH
        cout << "Finding the connections between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << " using Breadth-First Search:" << endl;
        pair<int, double> bfsSrc = make_pair(stoi(athlete1), athletes[stoi(athlete1)].weight);
        pair<int, double> bfsEnd = make_pair(stoi(athlete2), athletes[stoi(athlete2)].weight);
        bool success = false;
        auto start3 = high_resolution_clock::now();
        vector<int> path = bfs(olympicGraph, bfsSrc, bfsEnd, athletes, success);
        auto stop3 = high_resolution_clock::now();
        if(success) // only prints a path if there is a full one between two athletes from BFS
        {
            cout << "A path was found between your athletes!" << endl;
            for(int i = 0; i < path.size(); i++)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
        }
        else // will not print a partial path from BFS
            cout << "Based on BFS, it looks like there are no connections that reach " <<  athletes[stoi(athlete2)].name << " from " << athletes[stoi(athlete1)].name << "." << endl;
        auto duration3 = duration_cast<microseconds>(stop3 - start3);

        // comparing the performance of the algorithms
        cout << endl << "The diagnostics for the performance of these algorithms was: " << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "Dijkstra's Algorithm: " << duration1.count() << " microseconds" << endl;
        cout << "Bellman-Ford Algorithm: " << duration2.count() << " microseconds" << endl;
        cout << "Breadth-First Search: " << duration3.count() << " microseconds" << endl;

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
