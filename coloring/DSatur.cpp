#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1100;
vector<int> g[MAXN];
bool banned_color[MAXN][MAXN];
int banned_cnt[MAXN];
int uncolored_n[MAXN];
int colors[MAXN];

int greedy(int n, bool hot = false, int max_colors = -1) {
    if (max_colors == -1) max_colors = n;
    if (!hot) {
        fill(colors, colors + n, -1);
    }
    set<array<int, 3>> st;
    int total = 0;
    for (int i = 0; i < n; ++i) {
        if (colors[i] != -1) {
            total = max(total, colors[i] + 1);
            continue;
        }
        uncolored_n[i] = 0;
        fill(banned_color[i], banned_color[i] + max_colors, 0);
        banned_cnt[i] = 0;

        for (int u : g[i]) {
            if (colors[u] == -1) {
                ++uncolored_n[u];
            } else {
                if (!banned_color[i][colors[u]]) {
                    ++banned_cnt[i];
                    banned_color[i][colors[u]] = true;
                }
            }
        }
        st.insert({-banned_cnt[i], -uncolored_n[i], i});
    }

    while (!st.empty()) {
        auto elem = *st.begin();
        st.erase(st.begin());
        int v = elem[2];
        colors[v] = 0;
        while (banned_color[v][colors[v]]) ++colors[v];
        total = max(total, colors[v] + 1);
        if (total > max_colors) return MAXN;

        for (int u : g[v]) {
            if (colors[u] != -1) continue;
            st.erase({-banned_cnt[u], -uncolored_n[u], u});
            --uncolored_n[u];
            if (!banned_color[u][colors[v]]) {
                ++banned_cnt[u];
                banned_color[u][colors[v]] = true;
            }
            st.insert({-banned_cnt[u], -uncolored_n[u], u});
        }
    }
    return total;
}

void solve_() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    int total = greedy(n);
    cout << total << "\n";
    for (int i = 0; i < n; ++i) {
        cout << colors[i] << " ";
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