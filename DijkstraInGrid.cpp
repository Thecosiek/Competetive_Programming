//We are given n islands in a grid. Each island is reprezented by point in grid (a, b). Our tast is to find shortest path from island 1 to 
//island n. However there is strange way we are counting distance between two island - it is minimum of diffrence of their X and Y coordinates.

//solve - dijkstra with appliend difference in counting distance

//Maksymilian Piątek
#include <bits/stdc++.h>
using namespace std;

struct Wyspa {
    int x;
    int y;
    int index;
};

struct Edge {
    int kierunek;
    int koszt;
};

int dijkstra(int n, vector<vector<Edge>> &Graf, int start, int end) {
    vector<int> dist(n, INT_MAX);
    dist[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        pair<int, int> current = pq.top();
        pq.pop();

        int current_dist = current.first;
        int u = current.second;

        if (current_dist > dist[u]) {
            continue;
        }

        for (long long i = 0; i < Graf[u].size(); ++i) {
            int v = Graf[u][i].kierunek;
            int koszt = Graf[u][i].koszt;

            if (dist[u] + koszt < dist[v]) {
                dist[v] = dist[u] + koszt;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    return dist[end];
}

bool compareByX(const Wyspa& a, const Wyspa& b) {
    return a.x < b.x;
}

bool compareByY(const Wyspa& a, const Wyspa& b) {
    return a.y < b.y;
}

void add_edge(vector<vector<Edge>> &Graf, int u, int v, int koszt) {
    Graf[u].push_back(Edge{v, koszt});
    Graf[v].push_back(Edge{u, koszt});
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    cin >> n;

    vector<Wyspa> wyspy(n);
    
    for (int i = 0; i < n; ++i) {
        cin >> wyspy[i].x >> wyspy[i].y;
        wyspy[i].index = i;
    }

    vector<Wyspa> x_sasiad = wyspy;
    vector<Wyspa> y_sasiad = wyspy;
    
    std::sort(x_sasiad.begin(), x_sasiad.end(), compareByX);
    std::sort(y_sasiad.begin(), y_sasiad.end(), compareByY);

    vector<vector<Edge>> Graf(n);

    for (int i = 0; i < n - 1; ++i) {
        int u = x_sasiad[i].index;
        int v = x_sasiad[i + 1].index;
        int koszt = min(abs(x_sasiad[i].x - x_sasiad[i + 1].x), abs(x_sasiad[i].y - x_sasiad[i + 1].y));
        add_edge(Graf, u, v, koszt);

        u = y_sasiad[i].index;
        v = y_sasiad[i + 1].index;
        koszt = min(abs(y_sasiad[i].x - y_sasiad[i + 1].x), abs(y_sasiad[i].y - y_sasiad[i + 1].y));
        add_edge(Graf, u, v, koszt);
    }

    int result = dijkstra(n, Graf, 0, n - 1);
    cout << result;

    return 0;
}
