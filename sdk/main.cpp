#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#include "contestant_code.hpp"

using namespace std;

string input() {
    string line;
    getline(cin, line);
    return line;
}

int main() {
    FILE* logfile = fopen("ai.log", "w");
    string line;

    string start_obs_str = input();
    json start_obs = json::parse(start_obs_str);

    try {
        init(start_obs);
    } catch(exception& e) {
        fprintf(logfile, "init failed\n");
        fprintf(logfile, "%s\n", start_obs_str.c_str());
        fprintf(logfile, "%s\n", e.what());
        fflush(logfile);
    }

    while (true) {
        string obs_str = input();
        json obs = json::parse(obs_str);
        json action;
        try {
            action = get_action(obs);
        } catch(exception& e) {
            action = json({{"type", "Idle"}});
            fprintf(logfile, "get_action failed\n");
            fprintf(logfile, "%s\n", obs_str.c_str());
            fprintf(logfile, "%s\n", e.what());
            fflush(logfile);
        }

        action["frame"] = obs["frame"];
        cout << action.dump() << endl;
    }
}
