#pragma once
#include <string>
#include <utility>
#include <iostream>
#include <vector>
using namespace std;

struct athlete {
    string name;
    string olympicTeam;
    string sport;
    int medalCount;
    vector<pair<string, string>> events;
    athlete(string _name, string _olympicTeam, string _sport, int _medalCount, vector<pair<string, string>> _events);
};
