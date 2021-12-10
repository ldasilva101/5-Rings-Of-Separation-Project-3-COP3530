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
struct Edge
{
    int src, dest;
    double weight;

    Edge(int src, int dest, double weight)
    {
        this->src = src;
        this->dest = dest;
        this->weight = weight;
    }
};

class Graph
{
public:
    vector<Edge> edges;
    vector<vector<pair<int, double>>> adj_list;
    int vertices;
    int numEdges;
    Graph(int n)
    {
        numEdges = 0;
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

    edges.emplace_back(athleteA, athleteB, weight);

    adj_list[athleteA].push_back(make_pair(athleteB, weight));
    adj_list[athleteB].push_back(make_pair(athleteA, weight));
    numEdges++;
}

// prints athlete info in condensed version, to be used in displaying paths
void printBasicInfo(int athleteID, vector<athlete> &athletes)
{
    cout << "Athlete " << athletes[athleteID].index << ": " << athletes[athleteID].name << ", " << athletes[athleteID].olympicTeam << ", " << athletes[athleteID].sport << ", Total Medals: " << athletes[athleteID].medalCount;
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
void addAthlete(int index, string name, string team, string sport, string event, vector<athlete> &athletes)
{
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

            if(i == 10000)
                break;
            else
            {
                if(athleteAstr != "NA")
                {
                    i++;
                    int athleteA = stoi(athleteAstr);
                    int athleteB = stoi(athleteBstr);

                    pairs.push_back(make_pair(athleteA, athleteB));
                }
            }
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
    vector<double> distance(graph.vertices);
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
            double weight = graph.adj_list[u][i].second;
            if(distance[v] > distance[u] + weight)
            {
                parent[v] = u;
                distance[v] = distance[u] + weight;
                pq.push(make_pair(distance[v], v));
            }
        }
    }

    if(parent[dest] != -1)
    {
        success = true;
        return printSolution(parent, dest, src);
    }
    success = false;
    return parent;
}

vector<int> bellmanFord(const Graph& graph, int src, int dest, bool &success)
{
    //Step 1 initalize everything
    double dist[graph.vertices];
    vector<int> prev(graph.vertices);

    for (int i = 0; i < graph.vertices; i++)
    {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }

    //Step 2 Relax Edges
    dist[src] = 0;
    for (int i = 1; i < graph.vertices; i++)
    {
        for (int j = 0; j < graph.numEdges; j++)
        {
            auto u = graph.edges[j].src; //start vertex
            auto v = graph.edges[j].dest; //to vertex
            double w = graph.edges[j].weight; //weight of edge
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }

    //Step 3, negative cycles
    for (int j = 0; j < graph.numEdges; j++) {
        auto u = graph.edges[j].src; //start vertex
        auto v = graph.edges[j].dest; //to vertex
        double w = graph.edges[j].weight; //weight of edge
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
        {
            cout << "Graph contains a negative cycle." << endl;
            success = false;
            return prev;
        }
    }

    if(prev[dest] != -1)
    {
        success = true;
        return printSolution(prev, dest, src);
    }
    success = false;
    return prev;
}

// performs breadth-first search to find shortest path between two athletes (does not factor in weights)
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
        int top = order.front();
        order.pop_front();
        for (int i = 0; i < graph.adj_list[top].size(); i++)
        {
            if (!visited[graph.adj_list[top][i].first])
            {
                prev[graph.adj_list[top][i].first] = top;
                visited[graph.adj_list[top][i].first] = true;
                dist[graph.adj_list[top][i].first] = dist[top] + 1;
                order.push_back(graph.adj_list[top][i].first);

                if (graph.adj_list[top][i].first == end)
                {
                    success = true;
                    break;
                }
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
    cout << "Building full graph of Olympic connections..." << endl << endl;
    vector<pair<int, int>> pairs;
    readPairsCSV("pairs.csv", pairs);
    auto olympicGraph = Graph(athletes.size());
    olympicGraph.vertices = athletes.size();

    // build the graph
    for(int i = 0; i < pairs.size(); i++)
        olympicGraph.addEdge(pairs[i].first, pairs[i].second, athletes[pairs[i].first].medalCount + athletes[pairs[i].second].medalCount);

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
            << "    : ~  5 R O S  ~ :" << endl
            << "    : ~ A W A R D ~ :" << endl
            << "     :  *       *  :" << endl
            << "      `.  * * *  .'" << endl
            << "        `-.....-'" << endl
            << "   art by joan g stark" << endl;

    cout << endl << "Welcome to the Five Rings of Separation!" << endl;
    cout << "----------------------------------------" << endl;
    string keepgoing = "yes";
    while(keepgoing == "yes")
    {
        string athlete1;
        string athlete2;
        cout << "Enter the indices of the athletes you'd like to connect: (0 - " << athletes.size() - 1 << ")" << endl;
        cout << "Athlete 1: ";
        cin >> athlete1;
        cout << "Athlete 2: ";
        cin >> athlete2;

        if(stoi(athlete1) < 0 || stoi(athlete1) > 99999 || stoi(athlete2) < 0 || stoi(athlete2) > 99999)
        {
            cout << "Invalid athlete indices. Please try again." << endl << endl;
            continue;
        }

        cout << endl << "Connecting " << athletes[stoi(athlete1)].name << " to " << athletes[stoi(athlete2)].name << "..." << endl << endl;

        // BREADTH-FIRST SEARCH
        //  since this is the quickest, it will be used to check if there's a full path between two athletes before the longer algorithms even run
        bool success = false;
        auto start3 = high_resolution_clock::now();
        int pred[olympicGraph.vertices], dist[olympicGraph.vertices];
        vector<int> path = BFS(olympicGraph, stoi(athlete1), stoi(athlete2), olympicGraph.vertices, pred, dist, success);
        auto stop3 = high_resolution_clock::now();

        if(!success) // only prints a path if there is a full one between two athletes from BFS
            cout << "It looks like there is no path that fully connects " <<  athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << "." << endl << endl;

        else
        {
            cout << "A path was found between your athletes!" << endl;
            cout << "Breadth-First Search found the following shortest path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;
            for(int i = path.size() - 1; i >= 0; i--)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;

            auto duration3 = duration_cast<microseconds>(stop3 - start3);

            cout << endl
                 << "  .__." << endl
                 << " (|  |)" << endl
                 << "  (  )" << endl
                 << "  _)(_  " << endl
                 << "art by sjw" << endl;

            cout << endl << "Now looking for the path with the most medals!" << endl;
            // DIJKSTRA'S ALGORITHM
            cout << endl << "Dijkstra's Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;
            vector<int> dist(olympicGraph.vertices), prev(olympicGraph.vertices);
            int begin = stoi(athlete1);
            int end = stoi(athlete2);
            success = false;
            auto start1 = high_resolution_clock::now();
            path = dijkstra(olympicGraph, dist, prev, begin, end, success);
            auto stop1 = high_resolution_clock::now();
            for(int i = 0; i < path.size(); i++)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
            auto duration1 = duration_cast<microseconds>(stop1 - start1);
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;

            // BELLMAN-FORD ALGORITHM
            cout << endl << "The Bellman-Ford Algorithm found the following path between " << athletes[stoi(athlete1)].name << " and " << athletes[stoi(athlete2)].name << ":" << endl;
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;
            auto start2 = high_resolution_clock::now();
            success = false;
            path = bellmanFord(olympicGraph, stoi(athlete1), stoi(athlete2), success);
            auto stop2 = high_resolution_clock::now();
            for(int i = 0; i < path.size(); i++)
            {
                printBasicInfo(path[i], athletes);
                cout << endl;
            }
            auto duration2 = duration_cast<microseconds>(stop2 - start2);
            cout << ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\" << endl;

            // comparing the performance of the algorithms
            cout << endl << "PERFORMANCE DIAGNOSTICS" << endl;
            cout << "-----------------------" << endl;
            cout << "Breadth-First Search: " << duration3.count() << " microseconds" << endl;
            cout << "Dijkstra's Algorithm: " << duration1.count() << " microseconds" << endl;
            cout << "Bellman-Ford Algorithm: " << duration2.count() << " microseconds" << endl;
            cout << "-----------------------" << endl;
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
            cout << "~*~*~*~*~*~*~*~*~*~*~" << endl;
            printFullInfo(i, athletes);
            cout << "~*~*~*~*~*~*~*~*~*~*~" << endl;
            cout << endl << "Would you like more information about another athlete? (yes/no): ";
            cin >> moreInfo;
        }

        // if users want to search for another pair, they can continue to do so
        cout << endl << "Would you like to connect another pair of athletes? (yes/no): ";
        cin >> keepgoing;
    }
}