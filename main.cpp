#include <vector>
#include <iostream>
#include <unordered_set>

using namespace std;

// Variáveis globais
int n;
vector<vector<int>> adjacency_list; // Indexação dos vértices começa em 1
vector<bool> visited; // Indexação dos vértices começa em 0
vector<int> tin, low; // Indexação dos vértices começa em 0
int timer;

unordered_set<int> border_links;

void printAdjacencyList(vector<vector<int>> adjacency_list) {
    printf("Size: %ld\n", adjacency_list.size());

    for (int i = 1; i < adjacency_list.size(); i++) {
        printf("%d -> ", i);
        for (int j = 0; j < adjacency_list[i].size(); j++) {
            printf("%d ", adjacency_list[i][j]);
        }
        printf("\n");
    }
}

class QuickSort {
private:
    static int partition(vector<int>& arr, int start, int end){
        int pivot = arr[start];
    
        int count = 0;
        for (int i = start + 1; i <= end; i++) {
            if (arr[i] <= pivot)
                count++;
        }
    
        int pivotIndex = start + count;
        swap(arr[pivotIndex], arr[start]);
    
        int i = start, j = end;
        while (i < pivotIndex && j > pivotIndex) {
            while (arr[i] <= pivot) { i++; }

            while (arr[j] > pivot) { j--; }
    
            if (i < pivotIndex && j > pivotIndex) {
                swap(arr[i++], arr[j--]);
            }
        }
    
        return pivotIndex;
    }

    static void sortR(vector<int>& arr, int start, int end){
        if (start >= end){ return; }
    
        int p = partition(arr, start, end);
    
        sortR(arr, start, p - 1);
        sortR(arr, p + 1, end);
    }

public:
    static void sort(vector<int>& arr){
        int start = 0;
        int end = arr.size() - 1;
        sortR(arr, start, end);
    }
};

void dfs(int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;

    for (int to : adjacency_list[v]) {
        if (to == p) { continue; }

        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        } else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);

            if (low[to] >= tin[v] && p!=-1)
                border_links.insert(v);
            
            children++;
        }
    }
    
    if(p == -1 && children > 1)
        border_links.insert(v);
}

void findBorderLinks() {
    timer = 0;

    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);

    for (int i = 0; i < n; ++i) {
        if (!visited[i]){ dfs (i); }
    }
}

void listBorderLinks(unordered_set<int> border_links) {
    vector<int> sorted_border_links = vector<int>(border_links.begin(), border_links.end());
    QuickSort::sort(sorted_border_links);

    printf("%ld\n", sorted_border_links.size());
    for(int border_link: sorted_border_links) {
        printf("%d", border_link);

        if(border_link != sorted_border_links.back()) {
            printf(" ");
        }
    }
    printf("\n");
}

vector<vector<int>> removeVertices(vector<vector<int>> adjacency_list, unordered_set<int> vertices_to_remove) {
    vector<vector<int>> cutted_graph = vector<vector<int>>();

    int current_vertex = 0;
    for(vector<int> neighbors : adjacency_list) {
        vector<int> new_neighbors = vector<int>();

        if((current_vertex == 0) || (vertices_to_remove.find(current_vertex) != vertices_to_remove.end())){
            // É o primeiro elemento ou um vértice a ser removido
            current_vertex++;
            cutted_graph.push_back(new_neighbors);
            continue;
        }

        for(int neighbor : neighbors) {
            if(vertices_to_remove.find(neighbor) == vertices_to_remove.end()) {
                // Vizinho não esta na lista de vértices a serem removidos, então adiciona
                new_neighbors.push_back(neighbor);
            }
        }

        current_vertex++;
        cutted_graph.push_back(new_neighbors);
    }

    return cutted_graph;
}

vector<vector<int>> findClusters(vector<vector<int>> cutted_graph) {
    vector<vector<int>> clusters = vector<vector<int>>();

    visited.assign(n, false); // Resetando o vetor de visitados
    for (int i = 0; i < n; ++i) {
        if (!visited[i]){
            vector<int> cluster = vector<int>();
            dfs(i);
            
            for(bool is_visited : visited) { // TODO: Errado??
                if(is_visited) { cluster.push_back(i); }
            }

            vector<int> sorted_cluster = vector<int>(cluster.begin(), cluster.end());
            QuickSort::sort(sorted_cluster);
            clusters.push_back(sorted_cluster);
        }
    }

    return clusters;
}

void listClusters(vector<vector<int>> clusters){
    int nb_clusters = clusters.size();
    printf("%d\n", nb_clusters);

    for(int j=0; j < nb_clusters; j++) {
        vector<int> cluster = clusters[j];

        int cluster_identifier = n + j;
        printf("%d ", cluster_identifier);

        int cluster_size = cluster.size();
        printf("%d ", cluster_size);

        for(int i=0; i < cluster_size; i++) {
            printf("%d", cluster[i]);

            if(i != cluster_size - 1) { printf(" "); }
        }
    }
}

int main() {
    adjacency_list = vector<vector<int>>();

    scanf("%d", &n); // Número de links
    for (int i = 0; i <= n; i++) {
        // Inicializando a lista de adjacência com vetores vazios
        // o primeiro elemento não irá ser utilizado porque a indexação começa em 1
        adjacency_list.push_back(vector<int>());
    }

    int m; // Número de conexões
    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d", &x); // Indexação começa em 1
        scanf("%d", &y); // Indexação começa em 1
        
        adjacency_list[x].push_back(y);
        adjacency_list[y].push_back(x); // Assumindo grafo não dirigido
    }

    findBorderLinks();
    listBorderLinks(border_links);

    vector<vector<int>> cut_graph = removeVertices(adjacency_list, border_links);
    vector<vector<int>> clusters = findClusters(cut_graph);
    listClusters(clusters);

    return 0;
}
