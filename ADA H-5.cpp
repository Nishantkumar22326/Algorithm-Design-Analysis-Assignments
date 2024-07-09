#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int INF = 1e9;

struct Edge {
    int u, v, cap, flow;
    Edge(int u, int v, int cap) : u(u), v(v), cap(cap), flow(0) {}
};

int n;
vector<vector<int>> adj;
vector<Edge> edges;

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int idx : adj[cur]) {
            Edge& e = edges[idx];
            if (parent[e.v] == -1 && e.cap > e.flow) {
                parent[e.v] = idx;
                int new_flow = min(flow, e.cap - e.flow);
                if (e.v == t)
                    return new_flow;
                q.push({e.v, new_flow});
            }
        }
    }

    return 0;
}

int max_flow(int s, int t) {
    int flow = 0;
    vector<int> parent(n);

    int new_flow;
    while ((new_flow = bfs(s, t, parent))) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            edges[prev].flow += new_flow;
            edges[prev ^ 1].flow -= new_flow;
            cur = edges[prev].u;
        }
    }

    return flow;
}

int main() {
    // Read input (number of boxes and their dimensions)
    cin >> n;
    vector<vector<int>> boxes(n, vector<int>(3));
    for (int i = 0; i < n; i++) {
        cin >> boxes[i][0] >> boxes[i][1] >> boxes[i][2];
    }

    // Construct the flow network
    adj.resize(n + 2);
    int s = n, t = n + 1;
    for (int i = 0; i < n; i++) {
        edges.emplace_back(s, i, 1);  // Edge from source to box
        adj[s].push_back(edges.size() - 1);
        adj[i].push_back(edges.size() - 1);

        edges.emplace_back(i, t, 1);  // Edge from box to sink
        adj[i].push_back(edges.size() - 1);
        adj[t].push_back(edges.size() - 1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (boxes[i][0] < boxes[j][0] && boxes[i][1] < boxes[j][1] && boxes[i][2] < boxes[j][2]) {
                edges.emplace_back(i, j, 1);  // Edge from box i to box j
                adj[i].push_back(edges.size() - 1);
                adj[j].push_back(edges.size() - 1);
            }
        }
    }

    // Run Ford-Fulkerson's algorithm to find the maximum flow
    int max_flow_val = max_flow(s, t);

    // The minimum number of visible boxes is n - max_flow_val
    cout << "Minimum number of visible boxes: " << n - max_flow_val << endl;
    
    return 0;
}
