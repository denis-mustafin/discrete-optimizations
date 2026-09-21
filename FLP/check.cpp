#include <bits/stdc++.h>
using namespace std;

using ld = long double;

struct Vector {
    ld x;
    ld y;

    Vector(ld x = 0, ld y = 0): x(x), y(y) {}

    ld sqlen() const { return x * x + y * y; }
    ld len() const { return hypotl(x, y); }
};

istream& operator>>(istream& in, Vector& v) {
    in >> v.x >> v.y;
    return in;
}

Vector operator-(const Vector& a, const Vector& b) {
    return {a.x - b.x, a.y - b.y};
}

const int MAXN = 1e4;
Vector shop_pos[MAXN];
int c[MAXN];
ld cost[MAXN];
int d[MAXN];
Vector person_pos[MAXN];

int main(int argc, char* argv[]) {
    cout << fixed << setprecision(15);

    auto input = freopen(argv[1], "r", stdin);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        cin >> cost[i] >> c[i] >> shop_pos[i];
    }
    for (int i = 0; i < m; ++i) {
        cin >> d[i] >> person_pos[i];
    }
    fclose(input);

    auto output = freopen(argv[2], "r", stdin);
    ld participant_score;
    if (!(cin >> participant_score)) {
        cout << "Presentation Error: No score in the output\nScore = 0\n";
        return 0;
    }

    int shop_count;
    if (!(cin >> shop_count)) {
        cout << "Presentation Error: No shop count in the output\nScore = 0\n";
        return 0;
    }
    if (shop_count < 0 || shop_count > n) {
        cout << "Presentation Error: Invalid shop count\nScore = 0\n";
        return 0;
    }

    vector<bool> used_people(m), used_shops(n);
    ld score = 0;
    int total = 0;

    for (int i = 0; i < shop_count; ++i) {
        int id;
        if (!(cin >> id)) {
            cout << "Presentation Error: Not enough shops\nScore = 0\n";
            return 0;
        }
        if (id < 0 || id >= n) {
            cout << "Presentation Error: Invalid shop id\nScore = 0\n";
            return 0;
        }
        if (used_shops[id]) {
            cout << "Wrong Answer: A shop is used twice\nScore = 0\n";
            return 0;
        }
        used_shops[id] = true;
        score += cost[id];
        int cnt;
        int sum = 0;
        if (!(cin >> cnt)) {
            cout << "Presentation Error: No people count\nScore = 0\n";
            return 0;
        }
        if (cnt < 0 || cnt > m) {
            cout << "Presentation Error: Invalid number of people\nScore = 0\n";
            return 0;
        }

        total += cnt;
        for (int j = 0; j < cnt; ++j) {
            int jd;
            if (!(cin >> jd)) {
                cout << "Presentation Error: Not enough people\nScore = 0\n";
                return 0;
            }
            if (used_people[jd]) {
                cout << "Wrong Answer: A person is used twice\nScore = 0\n";
                return 0;
            }
            used_people[jd] = true;
            sum += d[jd];
            score += (shop_pos[id] - person_pos[jd]).len();
        }
        if (sum > c[id]) {
            cout << "Wrong Answer: A shop exceeds capacity\nScore = 0\n";
            return 0;
        }
    }

    if (total < m) {
        cout << "Wrong Answer: Not all people are used\nScore = 0\n";
        return 0;
    }

    if (abs(score - participant_score) > 1e-6 && abs(score - participant_score) / score > 1e-6) {
        cout << "Wrong Answer: participant claims the length is " << participant_score << " but it is actually " << score << "\nScore = 0\n";
        return 0;
    }

    cout << "OK\nScore = " << participant_score << "\n";

    return 0;
}