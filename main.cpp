#include <vector>
#include <iostream>

using namespace std;

// Variáveis globais
int n;
vector<vector<int>> adjacency_list; 
vector<bool> visited;
vector<int> tin, low;
int timer;

void printAdjacencyList(vector<vector<int>> adjacency_list) {
    cout << "Size: " << adjacency_list.size() << endl;

    for (int i = 1; i < adjacency_list.size(); i++) {
        cout << i << " -> ";
        for (int j = 0; j < adjacency_list[i].size(); j++) {
            cout << adjacency_list[i][j] << " ";
        }
        cout << endl;
    }
}

void IS_CUTPOINT(int v){ //TODO: Pode repetir o mesmo vértice
    cout << v << endl;
}

void dfs(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;
    for (int to : adjacency_list[v]) {
        if (to == p) continue;
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] >= tin[v] && p!=-1)
                IS_CUTPOINT(v);
            ++children;
        }
    }
    
    if(p == -1 && children > 1)
        IS_CUTPOINT(v);
}

void find_cutpoints() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs (i);
    }
}

int main() {
    adjacency_list = vector<vector<int>>();

    cin >> n; // Número de links
    for (int i = 0; i <= n; i++) {
        // Inicializando a lista de adjacência com vetores vazios
        // o primeiro elemento não irá ser utilizado porque a indexação começa em 1
        adjacency_list.push_back(vector<int>());
    }

    int m; // Número de conexões
    cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x; // Indexação começa em 1
        cin >> y; // Indexação começa em 1
        
        adjacency_list[x].push_back(y);
        adjacency_list[y].push_back(x); // Assumindo grafo não dirigido
    }

    // printAdjacencyList(adjacency_list);

    find_cutpoints();

    return 0;
}
