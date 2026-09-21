#include <bits/stdc++.h>
using namespace std;

#define DEBUG

mt19937 rnd(0);

vector<vector<int>> sets, in;
vector<int> cost;
vector<double> utility;

const double INF = 1e18;

struct State {
    vector<bool> covered;
    vector<int> taken;
    vector<bool> set_used;
    vector<double> utility;
    vector<int> cur_size;
    set<pair<double, int>> st;
    int score;

    void add_set(int i) {
        taken.push_back(i);
        score += cost[i];
        set_used[i] = true;

        for (int x : sets[i]) {
            if (!covered[x]) {
                covered[x] = true;
                for (int j : in[x]) {
                    if (!set_used[j]) {
                        --cur_size[j];
                        st.erase({utility[j], j});
                        if (cur_size[j] > 0) {
                            utility[j] = (double)cost[j] / cur_size[j];
                            st.insert({utility[j], j});
                        }
                    }
                }
            }
        }
    }
};

bool operator<(const State& a, const State& b) {
    return false;
}

int best_value;
vector<int> ans;
int n;

int pilot_complete(State s) {
    while (!s.st.empty()) {
        int i = (*s.st.begin()).second;
        s.st.erase(s.st.begin());
        s.add_set(i);
    }
    for (auto x : s.covered) {
        if (!x) return (int)1e9;
    }
    if (s.score < best_value) {
        best_value = s.score;
        ans = s.taken;
    }
    return s.score;
}

void solve_() {
    int m;
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
    vector<bool> covered(n, false);
    vector<int> cur_size(m);
    for (int i = 0; i < m; ++i) {
        cur_size[i] = sets[i].size();
    }
    vector<int> greedy_ans;
    State start(covered, {}, vector<bool>(m, 0), utility, cur_size, st, 0);
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
    best_value = value;
    ans = greedy_ans;

    for (int i = 0; i < n; ++i) {
        sort(in[i].begin(), in[i].end(), [&](int x, int y) { return utility[x] < utility[y]; });
    }


    auto start_time = clock();

#ifndef DEBUG

    while (clock() - start_time < 50 * CLOCKS_PER_SEC) {
        shuffle(item_perm.begin(), item_perm.end(), rnd);
        for (int i = 0; i < n; ++i) {
            id_item[item_perm[i]] = i;
        }
#endif

        const int QUEUE_SIZE = 5;
        set<pair<int, State>> beam_queue;
        beam_queue.insert({pilot_complete(start), start});
#ifdef DEBUG
        int iter = 0;
#endif
        while (!beam_queue.empty()) {
#ifdef DEBUG
            if ((++iter) % 100 == 0) {
                cerr << ++iter << " extract; ans = " << best_value << endl;
            }
#endif
            auto elem = *beam_queue.begin();
            beam_queue.erase(beam_queue.begin());
            if (elem.first > (int)5e8) continue;
            State s = elem.second;
            /// cerr << s.st.size() << "\n";
            if (s.st.empty()) continue;

            int i = (*s.st.begin()).second;

            State s1 = s;
            s1.add_set(i);
            beam_queue.insert({pilot_complete(s1), s1});
            s.set_used[i] = true;
            s.st.erase(s.st.begin());
            beam_queue.insert({pilot_complete(s), s});

            while (beam_queue.size() > QUEUE_SIZE) {
                auto it = beam_queue.end();
                --it;
                beam_queue.erase(it);
            }
        }
#ifndef DEBUG
    }
#endif

    cout << best_value << "\n";
    cout << ans.size() << "\n";
    for (int i : ans) {
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