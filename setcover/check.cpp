#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> sets;
vector<int> cost;

int main(int argc, char* argv[]) {
    auto input = freopen(argv[1], "r", stdin);
    int n, m;
    cin >> n >> m;
    sets.resize(m);
    cost.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> cost[i];
        string s;
        getline(cin, s);
        istringstream is(s);
        vector<int> v((istream_iterator<int>(is)), istream_iterator<int>());
        sets[i] = v;
    }
    fclose(input);

    auto output = freopen(argv[2], "r", stdin);
    int participant_cost;
    if (!(cin >> participant_cost)) {
        cout << "Presentation Error: No cost in the output\nScore = 0\n";
        return 0;
    }
    int size;
    if (!(cin >> size)) {
        cout << "Presentation Error: No size in the output\nScore = 0\n";
        return 0;
    }
    vector<int> ans(size);
    for (int i = 0; i < size; ++i) {
        if (!(cin >> ans[i])) {
            cout << "Presentation Error: Not enough sets in the output\nScore = 0\n";
            return 0;
        }
    }

    vector<int> covered(n);
    int real_cost = 0;
    for (int i : ans) {
        if (i < 0 || i >= m) {
            cout << "Presentation Error: Incorrect set index\nScore = 0\n";
        }
        for (int x : sets[i]) {
            if (x >= 0 && x < n) {
                covered[x] = 1;
            }
        }
        real_cost += cost[i];
    }
    for (int i = 0; i < n; ++i) {
        if (!covered[i]) {
            cout << "Wrong Answer: Element " << i << " not covered\nScore = 0\n";
        }
    }
    if (real_cost != participant_cost) {
        cout << "Wrong Answer: Participant claims cost is " << participant_cost << " but it is " << real_cost << "\nScore = 0\n";
    }

    cout << "OK\nScore = " << participant_cost << "\n";

    return 0;
}