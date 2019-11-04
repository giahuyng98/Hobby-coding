#include <bits/stdc++.h>
using namespace std;

const int INF = numeric_limits<int>::max();

struct Job{     
    int id, time, cost, crTime, crCost;
    vector<int> edgeList;

    Job(int id) : id(id), time(0), cost(0),crTime(0), crCost(0) {}

    Job(int id, stringstream &ss) : id(id){
        ss >> time >> cost >> crTime >> crCost;
    }

    bool canCrash() const {
        return crTime < time;
    }

    int getCrashCost() const {
        return crCost - cost;
    }
};

class JobList{
private:
    vector<Job> jobs;
    vector<string> idToName;

    void addDependList(int jobId, const vector<string> &edgeList, 
                       const unordered_map<string, int> &nameToId){
        for(const string &dependJobName : edgeList){
            jobs[nameToId.at(dependJobName)].edgeList.emplace_back(jobId);
        }
    }
public:
    Job& operator[](int idx){
        return jobs[idx];
    }

    string getName(int idx){
        return idToName[idx];
    }

    int size() const {
        return jobs.size();
    }

    void input(istream &in){
        vector<vector<string>> dependsAllJobList{
            {}
        };
        unordered_map<string, int> nameToId{
            {"START", 0},
        };

        string line;
        jobs.emplace_back(Job(0));
        idToName.emplace_back("START");

        int n;
        in >> n >> ws;
        while (n--) {
            getline(in, line);
            stringstream ss(line);
            string jobName;
            ss >> jobName;
            nameToId[jobName] = jobs.size();
            jobs.emplace_back(Job(jobs.size(), ss));
            vector<string> edgeList;
            string depend;
            while (ss >> depend){
                edgeList.emplace_back(depend);
            }
            if (edgeList.size() == 0){
                edgeList.emplace_back("START");
            }
            dependsAllJobList.emplace_back(edgeList);
            idToName.emplace_back(jobName);
        }

        const int FINISH = jobs.size();
        nameToId.insert({"FINISH", FINISH});
        jobs.emplace_back(Job(FINISH));
        dependsAllJobList.push_back({});

        for(int i = 0; i < FINISH; ++i){
            addDependList(i, dependsAllJobList[i], nameToId);
        }
        for(int i = 0; i < FINISH; ++i){
            if (jobs[i].edgeList.empty())
                jobs[i].edgeList.emplace_back(FINISH);
        }
    }
};

class Crashing{
private:
    JobList jobList;
    int accTime;
    vector<int> topo;

    struct Event{
        int id, et, te, tl;

        Event(int id = 0) : id(id), et(0), te(0), tl(INF) {}

        bool isCrashable() const {
            return te == tl;
        }

        bool operator<(const Event &rhs) const {
            return id < rhs.id;
        }

        bool cmpTime(const Event &rhs) const {
            return te < rhs.te;
        }
    };

    int calculateTime(vector<Event> &events){
        for(int id : topo){
            for(int next : jobList[id].edgeList){
                events[next].te = max(events[next].te, 
                        events[id].te + events[next].et);
            }
        }
        events.back().tl = events.back().te;
        for(auto it = topo.rbegin(); it != topo.rend(); ++it){
            int id = *it;
            for(int next : jobList[id].edgeList){
                events[id].tl = min(events[id].tl, 
                        events[next].tl - events[next].et);
            }
        }
        return events[topo.back()].te;
    }

    int getCost(const set<Event> &crashList){
        int cost = 0;
        for(const Event &evt : crashList){
            cost += jobList[evt.id].getCrashCost();
        }
        return cost;
    }

    string getListName(const set<Event> &crashedList) {
        string listName;
        for(const Event &evt : crashedList){
            listName += jobList.getName(evt.id);
        }
        return listName;
    }

    pair<int,set<Event>> getNextCrashList(const set<Event> &crashedList){
        const int n = jobList.size();
        vector<Event> events(n);
        for(int i = 0; i < n; ++i){
            events[i].id = i;
            if (crashedList.count(Event(i))){
                events[i].et = jobList[i].crTime;
            } else {
                events[i].et = jobList[i].time;
            }
        }

        int te = calculateTime(events);
        set<Event> mayBeNextCrashList;
        for(Event &evt : events){
            if (evt.isCrashable() && jobList[evt.id].canCrash()){
                mayBeNextCrashList.emplace(move(evt));
            }
        }

        set<Event> nextCrashList;
        set_difference(mayBeNextCrashList.begin(), mayBeNextCrashList.end(),
                       crashedList.begin(), crashedList.end(),
                       inserter(nextCrashList,nextCrashList.begin())
                );

        return {te, nextCrashList};
    }

    void doCrash(){
        preprocess();

        using T = pair<int, set<Event>>;
        priority_queue<T, vector<T>, greater<T>> queue;
        queue.push({0, {}});

        map<set<Event>,bool> isCrashed;
        isCrashed[{}] = true;

        cout << "+------+----------+---------+-------+---------------+\n";
        cout << "| Step |  Crashed | Cr.Cost | T.End |      Next     |\n";
        cout << "+------+----------+---------+-------+---------------+\n";

        int step = 0;
        while (!queue.empty()){
            int currentCost = queue.top().first;
            auto crashedList = queue.top().second;
            queue.pop();

            int te;
            set<Event> nextCrashList;
            tie(te, nextCrashList) = getNextCrashList(crashedList);

            cout << "|" << setw(6);
            cout << step++;
            cout << "|" << setw(10);
            cout << getListName(crashedList);
            cout << "|" << setw(9);
            cout << currentCost;

            cout << "|" << setw(7);
            cout << te;

            cout << "|" << setw(15);
            cout << getListName(nextCrashList);
            cout << "|\n+------+----------+---------+-------+---------------+\n";

            if (te <= accTime){
                break;
            }

            for(const Event &evt : nextCrashList){
                set<Event> s = crashedList;
                s.insert(evt);
                if (!isCrashed[s]){
                    isCrashed[s] = true;
                    int cost = getCost(s);
                    queue.push({cost, s});
                }
            }
        }
    }

    void preprocess(){
        vector<bool> visited(jobList.size(), false);
        visited[0] = true;
        dfs(0, visited);
        reverse(topo.begin(), topo.end());
    }

    void dfs(int id, vector<bool> &visited){
        for(int next : jobList[id].edgeList){
            if (!visited[next]){
                visited[next] = true;
                dfs(next, visited);
            }
        }
        topo.push_back(id);
    }

public:
    void input(istream &in){
        in >> accTime >> ws;
        jobList.input(in);
        doCrash();
    }
};

int main(int argc, char *argv[]){
    Crashing crash;
    if (argc < 2){
        crash.input(cin);
    } else {
        ifstream in(argv[1]);
        crash.input(in);
    }
    return 0;
}
