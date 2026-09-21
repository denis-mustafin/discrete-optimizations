#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    auto input = freopen(argv[1], "r", stdin);
    int n, W;
    cin >> n >> W;
    vector<array<int, 2>> items(n);
    for (int i = 0; i < n; ++i) {
        int v, w;
        cin >> v >> w;
        items[i] = {v, w};
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
            cout << "Presentation Error: Not enough items in the output\nScore = 0\n";
            return 0;
        }
    }

    sort(ans.begin(), ans.end());
    int real_cost = 0, w = 0;
    for (int i = 0; i < size; ++i) {
        if (ans[i] < 0 || ans[i] >= n) {
            cout << "Presentation Error: Incorrect item id\nScore = 0\n";
            return 0;
        }
        real_cost += items[ans[i]][0];
        w += items[ans[i]][1];
        if (i > 0 && ans[i] == ans[i - 1]) {
            cout << "Wrong Answer: An item is used twice\nScore = 0\n";
            return 0;
        }
    }
    if (w > W) {
        cout << "Wrong Answer: The weight exceeds capacity\nScore = 0\n";
    }
    if (real_cost != participant_cost) {
        cout << "Wrong Answer: Participant claims cost is " << participant_cost << " but it is " << real_cost << "\nScore = 0\n";
        return 0;
    }

    cout << "OK\nScore = " << participant_cost << "\n";

    return 0;
}