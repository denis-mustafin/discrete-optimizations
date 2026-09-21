#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    auto input = freopen(argv[1], "r", stdin);
    int n, m;
    cin >> n >> m;
    vector<array<int, 2>> ed(m);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        ed[i] = {u, v};
    }
    fclose(input);

    auto output = freopen(argv[2], "r", stdin);
    double participant_colors;
    if (!(cin >> participant_colors)) {
        cout << "Presentation Error: No cost in the output\nScore = 0\n";
        return 0;
    }

    vector<int> colors(n);
    for (int i = 0; i < n; ++i) {
        if (!(cin >> colors[i])) {
            cout << "Presentation Error: not enough colors\nScore = 0\n";
        }
        if (colors[i] < 0 || colors[i] >= participant_colors) {
            cout << "Presentation Error: invalid color\nScore = 0\n";
        }
    }

    for (int i = 0; i < m; ++i) {
        if (colors[ed[i][0]] == colors[ed[i][1]]) {
            cout << "Wrong Answer: vertices " << ed[i][0] << " and " << ed[i][1] << " are connected by an edge but have the same color\nScore = 0\n";
        }
    }

    cout << "OK\nScore = " << participant_colors << "\n";

    return 0;
}