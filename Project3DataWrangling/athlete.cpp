#include "athlete.h"

athlete::athlete(string _name, string _olympicTeam, string _sport, int _medalCount, vector<pair<string, string>> _events)
{
    name = _name;
    olympicTeam = _olympicTeam;
    sport = _sport;
    medalCount = _medalCount;
    events = _events;
}