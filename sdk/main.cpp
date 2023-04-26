#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

#include "contestant_code.hpp"

using namespace std;

class Consumer {
public:
    Consumer(int timeout = 1) : timeout(timeout) {
        task = std::thread(&Consumer::run, this);
    }

    string get_init() {
        while (lines.empty()) {};
        return lines[0];
    }

    string get_observation() {
        time_t begin = clock();
        while (lines.size() == last_length)
            if (clock() - begin > timeout * CLOCKS_PER_SEC)
                exit(0);

        last_length = lines.size();
        return lines.back();
    }

private:
    std::thread task;
    void run() {
        string line;
        while (getline(cin, line)) lines.push_back(line);
        cerr << "consumer thread exited" << endl;
    }

    vector<string> lines;
    int last_length = 1;
    int timeout;
};

int main() {
    FILE* logfile = fopen("ai.log", "w");
    Consumer consumer;

    string start_obs_str = consumer.get_init();
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
        string obs_str = consumer.get_observation();
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
