#include <bits/stdc++.h>
using namespace std;

vector<int> d;
vector<array<double, 2>> people;

double len(int i, int j) {
    return hypot(people[i][0] - people[j][0], people[i][1] - people[j][1]);
}

int main(int argc, char* argv[]) {
    auto input = freopen(argv[1], "r", stdin);
    int n, v, c;
    cin >> n >> v >> c;
    people.resize(n + 1);
    d.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> d[i] >> people[i][0] >> people[i][1];
    }
    people[n] = {0, 0};
    fclose(input);

    auto output = freopen(argv[2], "r", stdin);
    double participant_cost;
    if (!(cin >> participant_cost)) {
        cout << "Presentation Error: No cost in the output\nScore = 0\n";
        return 0;
    }

    vector<int> cnt(n);

    double real_cost = 0;

    for (int i = 0; i < v; ++i) {
        int sz;
        if (!(cin >> sz)) {
            cout << "Presentation Error: not enough vehicle descriptions\nScore = 0\n";
            return 0;
        }
        if (sz < 0 || sz > n) {
            cout << "Presentation Error: incorrect size " << sz << "\nScore = 0\n";
            return 0;
        }
        vector<int> cur(sz);
        int sum = 0;
        for (int j = 0; j < sz; ++j) {
            if (!(cin >> cur[j])) {
                cout << "Presentation Error: not enough customers for a vehicle\nScore = 0\n";
                return 0;
            }
            --cur[j];
            if (cur[j] < 0 || cur[j] >= n) {
                cout << "Presentation Error: incorrect person index\nScore = 0\n";
                return 0;
            }
            ++cnt[cur[j]];
            if (j == 0) {
                real_cost += len(cur[j], n);
            } else {
                real_cost += len(cur[j], cur[j - 1]);
            }
            sum += d[cur[j]];
        }
        if (sum > c) {
            cout << "Wrong Answer: vehicle exceeds capacity\nScore = 0\n";
            return 0;
        }
        if (sz > 0) {
            real_cost += len(n, cur[sz - 1]);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (cnt[i] != 1) {
            cout << "Wrong Answer: some person is not in one set\nScore = 0\n";
            return 0;
        }
    }

    if (abs(real_cost - participant_cost) > 0.001) {
        cout << "Wrong Answer: participant's cost is incorrect\nScore = 0\n";
        return 0;
    }

    cout << fixed << setprecision(3);
    cout << "OK\nScore = " << participant_cost << "\n";

    return 0;
}