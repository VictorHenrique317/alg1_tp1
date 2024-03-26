#include <vector>
#include <list>
#include <iostream>
#include <unordered_set>
#include <stack>
#include <set>
#include <algorithm>

using namespace std;

// Variáveis globais definidas pelo usuario
int n;
int m;
vector<vector<int>> adjacency_list; // Indexação dos vértices começa em 1

// Variáveis globais do DFS para encontrar links de borda
unordered_set<int> border_links;
vector<bool> visited;
int excluded_link;

// Variáveis globais para encontrar clusters
vector<unordered_set<int>> clusters = vector<unordered_set<int>>();
vector<int> dfs_number; 
vector<int> tin; 
vector<int> low; 
vector<int> parent; 
int time_stamp;
int timer;
stack<int> node_stack;

// Variáveis globais para encontrar a floresta de clusters borda
vector<vector<int>> forest_adjacency_list;
vector<vector<int>> forest_edges;

class QuickSort {
private:
    static int partition(vector<int>& arr, int start, int end){
        int pivot = arr[start];
    
        int count = 0;
        for (int i = start + 1; i <= end; i++) {
            if (arr[i] <= pivot)
                count++;
        }
    
        int pivot_index = start + count;
        swap(arr[pivot_index], arr[start]);
    
        int i = start, j = end;
        while (i < pivot_index && j > pivot_index) {
            while (arr[i] <= pivot) { i++; }

            while (arr[j] > pivot) { j--; }
    
            if (i < pivot_index && j > pivot_index) {
                swap(arr[i++], arr[j--]);
            }
        }
    
        return pivot_index;
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

void printVector(vector<int> vec) {
    for (int i = 0; i < vec.size(); i++) {
        printf("%d", vec[i]);

        if(i != vec.size() - 1) {
            printf(" ");
        }
    }
    printf("\n");
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
                border_links.insert(v);
            ++children;
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
        if (!visited[i])
            dfs (i);
    }
}

void listBorderLinks() {
    vector<int> sorted_border_links = vector<int>(border_links.begin(), border_links.end());
    QuickSort::sort(sorted_border_links);

    printf("%ld\n", sorted_border_links.size());
    for(int border_link: sorted_border_links) {
        printf("%d\n", border_link);
    }
}

void tarjanAlgorithm(int current_node) {
  node_stack.push(current_node);
  dfs_number[current_node] = low[current_node] = ++time_stamp;

  for (int v : adjacency_list[current_node]) {
    if (dfs_number[v] == 0) {
      parent[v] = current_node;
      tarjanAlgorithm(v);
      low[current_node] = min(low[current_node], low[v]);
      
      if (low[v] >= dfs_number[current_node]) {
        unordered_set<int> current_cluster;
        int top_node;
        
        do {
          top_node = node_stack.top();
          node_stack.pop();
          current_cluster.insert(top_node);
        } while (top_node != v);
        
        current_cluster.insert(current_node);
        clusters.push_back(current_cluster);
      }
    } else if (v != parent[current_node]) {
      low[current_node] = min(low[current_node], dfs_number[v]);
    }
  }
}

void findClusters(){
    for (int i = 1; i <= n; i++) {
        if (dfs_number[i] == 0) {
            tarjanAlgorithm(i);
        }
    }
}

vector<vector<int>> sortLexicographically(vector<vector<int>> unsorted) {
    vector<vector<int>> sorted = vector<vector<int>>();
    for(vector<int> vec: unsorted){
        sorted.push_back(vec);
    }

    sort(sorted.begin(), sorted.end());
    return sorted;
}

void listClusters(){
    int nb_clusters = clusters.size();
    printf("%d\n", nb_clusters);

    int j = 1;
    vector<vector<int>> clusters_vector = vector<vector<int>>();
    for(unordered_set<int> cluster : clusters){
        vector<int> cluster_vector = vector<int>(cluster.begin(), cluster.end());
        QuickSort::sort(cluster_vector);
        clusters_vector.push_back(cluster_vector);
    }

    vector<vector<int>> sorted_clusters = sortLexicographically(clusters_vector);
    clusters.clear();
    for (vector<int> cluster: sorted_clusters){
        int cluster_identifier = n + j;
        printf("%d ", cluster_identifier);

        int cluster_size = cluster.size();
        printf("%d ", cluster_size);
        printVector(cluster);

        unordered_set<int> cluster_set = unordered_set<int>(cluster.begin(), cluster.end());
        clusters.push_back(cluster_set);
        j++;
    }
}

void findClusterBorderForest(){
    // Indexação começa do 1, primeiro elemento é desconsiderado
    forest_adjacency_list = vector<vector<int>>();
    forest_adjacency_list.push_back(vector<int>());

    int cluster_identifier = n+1;
    for(unordered_set<int> cluster: clusters){

        for(int border_link: border_links){
            if(cluster.find(border_link) != cluster.end()){
                // Esse cluster contem esse link de borda
                int x = min(cluster_identifier, border_link);
                int y = max(cluster_identifier, border_link);
                
                vector<int> edge = vector<int>();
                edge.push_back(x);
                edge.push_back(y);
                forest_edges.push_back(edge);
            }
        }

        cluster_identifier++;
    }
}

void listClusterBorderForest(){
    int forest_vertice_nb = clusters.size() + border_links.size();
    int forest_edge_nb = forest_edges.size();
    printf("%d %d\n", forest_vertice_nb, forest_edge_nb);

    vector<vector<int>> sorted_edges = sortLexicographically(forest_edges);
    for(vector<int> edge: sorted_edges){
        printVector(edge);
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
    dfs_number = vector<int>(n); 
    low = vector<int>(n);
    parent = vector<int>(n);

    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d", &x); // Indexação começa em 1
        scanf("%d", &y); // Indexação começa em 1
        
        adjacency_list[x].push_back(y);
        adjacency_list[y].push_back(x); // Assumindo grafo não dirigido
    }

    findBorderLinks();
    listBorderLinks();

    findClusters();
    listClusters();

    findClusterBorderForest();
    listClusterBorderForest();

    return 0;
}
