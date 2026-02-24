#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> sets, in;
vector<int> cost;
vector<double> utility;

const double INF = 1e18;

void solve_() {
    int n, m;
    cin >> n >> m;
    in.resize(n);
    sets.resize(m);
    cost.resize(m);
    utility.resize(m);
    for (int i = 0; i < m; ++i) {
        cin >> cost[i];
        /// was it really that difficult to include the size of the set?
        string s;
        getline(cin, s);
        istringstream is(s);
        vector<int> v((istream_iterator<int>(is)), istream_iterator<int>());
        for (int x : v) {
            if (x < 0 || x >= n) continue;
            in[x].push_back(i);
            sets[i].push_back(x);
        }
        if (sets[i].empty()) {
            utility[i] = INF;
        } else {
            utility[i] = (double)cost[i] / sets[i].size();
        }
    }


    set<pair<double, int>> st;
    for (int i = 0; i < m; ++i) {
        if (utility[i] > INF / 2) continue;
        st.insert({utility[i], i});
    }
    vector<int> covered(n, 0);
    vector<int> cur_size(m);
    for (int i = 0; i < m; ++i) {
        cur_size[i] = sets[i].size();
    }
    vector<int> greedy_ans;
    int value = 0;
    while (!st.empty()) {
        int i = (*st.begin()).second;
        st.erase(st.begin());
        value += cost[i];
        greedy_ans.push_back(i);
        for (int x : sets[i]) {
            if (!covered[x]) {
                covered[x] = 1;
                for (int j : in[x]) {
                    st.erase({utility[j], j});
                    --cur_size[j];
                    if (cur_size[j] > 0) {
                        utility[j] = (double)cost[j] / cur_size[j];
                        st.insert({utility[j], j});
                    }
                }
            }
        }
    }
    cout << value << "\n";
    cout << greedy_ans.size() << "\n";
    for (int i : greedy_ans) {
        cout << i << " ";
    }
    cout << "\n";
}

/// #define MULTITEST

signed main() {
#ifdef LOCAL
    freopen("../input.txt", "r", stdin);
    freopen("../output.txt", "w", stdout);
    auto start = clock();
#else
    ios_base::sync_with_stdio(false);
    cin.tie(0);
#endif


    int tst = 1;
#ifdef MULTITEST
    cin >> tst;
#endif // MULTITEST
    while (tst--) {
        solve_();
    }

#ifdef LOCAL
    cerr << "\ntime: ";
    cerr << setprecision(4) << (clock() - start) * 1.0 / CLOCKS_PER_SEC << endl;
#endif // LOCAL
    return 0;
}