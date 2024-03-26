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

// Variáveis globais do DFS
int timer;
vector<bool> visited; // Indexação dos vértices começa em 0

// Variáveis globais do DFS para encontrar links de borda
// vector<int> tin, low; // Indexação dos vértices começa em 0
unordered_set<int> border_links;
int excluded_link;

// Variáveis globais para encontrar clusters
vector<unordered_set<int>> clusters = vector<unordered_set<int>>();
set<pair<int, int>> edges;

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

void printVector(vector<int> vec) {
    for (int i = 0; i < vec.size(); i++) {
        printf("%d", vec[i]);

        if(i != vec.size() - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

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

pair<int, int> makeEdge(int x, int y) {
    return make_pair(min(x, y), max(x, y));
}

unordered_set<int> setIntersection(unordered_set<int> set1, unordered_set<int> set2) {
    unordered_set<int> intersection = unordered_set<int>();
    for (int element : set1) {
        if (set2.find(element) != set2.end()) {
            intersection.insert(element);
        }
    }

    return intersection;
}

unordered_set<int> setUnion(unordered_set<int> set1, unordered_set<int> set2) {
    unordered_set<int> union_set = unordered_set<int>();
    for (int element : set1) {
        union_set.insert(element);
    }

    for (int element : set2) {
        union_set.insert(element);
    }

    return union_set;
}

void printUnorderedSet(unordered_set<int> set) {
    vector<int> vec = vector<int>(set.begin(), set.end());
    QuickSort::sort(vec);

    for (int i = 0; i < vec.size(); i++) {
        printf("%d", vec[i]);

        if(i != vec.size() - 1) {
            printf(" ");
        }
    }

    printf("\n");
}

// void dfs(int v, int p = -1) {
//     visited[v] = true;
//     tin[v] = low[v] = timer++;
//     int children=0;

//     for (int to : adjacency_list[v]) {
//         if (to == p) { continue; }

//         if (visited[to]) {
//             low[v] = min(low[v], tin[to]);
//         } else {
//             dfs(to, v);
//             low[v] = min(low[v], low[to]);

//             if (low[to] >= tin[v] && p!=-1)
//                 border_links.insert(v);
            
//             children++;
//         }
//     }
    
//     if(p == -1 && children > 1)
//         border_links.insert(v);
// }

void dfs(int v){
    if(v == excluded_link) { return; }
    visited[v] = true;

    for (int neighbor : adjacency_list[v]) {
        if(!visited[neighbor]){
            dfs(neighbor);
        }
    }
}

// void findBorderLinks() {
//     timer = 0;

//     visited.assign(n, false);
//     tin.assign(n, -1);
//     low.assign(n, -1);

//     for (int i = 1; i <= n; ++i) {
//         if (!visited[i]){ dfs (i); }
//     }
// }

void findBorderLinks() {
    visited.assign(n, false);
    excluded_link = 0; // Inicializando o link excluído para 0 (nenhum link é excluído)
    int minimum_nb_components = 0;
    for (int i = 1; i <= n; ++i) {
        if (!visited[i]){ 
            dfs (i);
            minimum_nb_components++;
        }
    }

    // printf("Componentes minimos: %d\n", minimum_nb_components);
    
    for(int exc = 1; exc <= n; exc++){
        excluded_link = exc;
        visited.assign(n, false);

        int new_component_nb = 0;
        for (int i = 1; i <= n; ++i) {
            if(i == exc) { continue; } // Ignorando o link excluído

            if (!visited[i]){ 
                dfs (i);
                new_component_nb++;
            }
        }

        // printf("Link excluído: %d, Componentes: %d\n", exc, new_component_nb);

        if (new_component_nb > minimum_nb_components){
            border_links.insert(exc);
        }
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

void clusterWiseDfs(unordered_set<int>& cluster, int v) {
    visited[v] = true;
    cluster.insert(v);

    if(border_links.find(v) != border_links.end()) {
        // Se for um link de borda não se deve continuar a busca
        return;
    }

    for (int neighbor : adjacency_list[v]) {
        if (!visited[neighbor]) {
            edges.erase(makeEdge(v, neighbor));
            clusterWiseDfs(cluster, neighbor);
        }
    }
}

void clearBorderLinksColors(){
    // visited.assign(n, false);
    for(int border_link: border_links){
        visited[border_link] = false;
    }
}

bool hasCircuit(int current, int target, list<int>& path) {
    visited[current] = true;
    path.push_back(current);

    // printf("Deepening -> Current: %d, Target: %d\n", current, target);

    if (current == target && path.size() > 1) {
        return true;
    }

    for (int neighbor : adjacency_list[current]) {
        if(neighbor == target && path.size() > 2) {
            // Um dos vizinhos é o target, circuito encontrado
            path.push_back(neighbor);
            return true;
        }

        if (!visited[neighbor]) { // Não permite a repetição de vértices
            if(hasCircuit(neighbor, target, path)){
                return true;
            }
        }
    }

    visited[current] = false;
    path.pop_back();

    // printf("Returning\n");

    return false;
}

vector<int> findCircuit(int vertex) {
    visited.assign(n, false);
    list<int> path;

    if (hasCircuit(vertex, vertex, path)) {
        vector<int> circuit = vector<int>(path.begin(), path.end());
        return circuit;
    } else {
        return vector<int>();
    }
}

bool completeCluster(unordered_set<int> circuit, int u, int v) {
    int nb_contained_links = 0;
    unordered_set<int>* cluster_that_most_contains_circuit_links = nullptr;
    for(int i=0; i<clusters.size(); i++){
        unordered_set<int>* current_cluster_ptr = &clusters[i];
        if(current_cluster_ptr->find(u) == current_cluster_ptr->end() && current_cluster_ptr->find(v) == current_cluster_ptr->end()){
            // Esse cluster não contém os dois links de borda, com certeza ele não pode ser completado
            continue;
        }

        int intersection_size = setIntersection(*current_cluster_ptr, circuit).size();

        if(intersection_size == circuit.size()){
            // Esse cluster já contém todos os links do circuito, ele não será completado
            continue;
        }

        if(intersection_size > nb_contained_links){
            nb_contained_links = intersection_size;
            cluster_that_most_contains_circuit_links = current_cluster_ptr;
        }
    }

    if(cluster_that_most_contains_circuit_links == nullptr){
        // Não existe um cluster que possa ser completado
        return false;
    }
    
    int old_cluster_size = cluster_that_most_contains_circuit_links->size();
    for(int link: circuit){
        cluster_that_most_contains_circuit_links->insert(link);
    }

    return old_cluster_size != cluster_that_most_contains_circuit_links->size();
}

// void findClusters() {
//     visited.assign(n, false); // Resetando o vetor de cores
//     for (int i = 1; i <= n; i++) {
//         if(border_links.find(i) != border_links.end()) {
//             // Se for um link de borda não se deve iniciar a busca
//             continue;
//         }

//         if (!visited[i]) {
//             unordered_set<int> cluster = unordered_set<int>();
//             clearBorderLinksColors(); // Resetando o vetor de cores para que um 
//             // link de borda possa ser visitado multiplas vezes e adicionado em multiplos clusters

//             clusterWiseDfs(cluster, i);
//             if(cluster.size() == 1) { continue; } // Ignorando clusters com somente um link
//             clusters.push_back(cluster);
//         }
//     }

//     // Aqui achamos todos os clusters, menos aqueles que contem somente dois links de borda
//     // Agora precisamos achar os clusters que contem somente dois links de borda

//     unordered_set<int> seen_by_circuit_links = unordered_set<int>();
//     for (pair<int, int> edge : edges) {
//         // Itera sobre todas as arestas que não foram visitadas pelo DFS
//         int u = edge.first;
//         int v = edge.second;
//         // printf("U: %d, V: %d\n", u, v);

//         if(seen_by_circuit_links.find(u) != seen_by_circuit_links.end() && seen_by_circuit_links.find(v) != seen_by_circuit_links.end()){
//             // Se uma aresta percente a um circuito já incorporado não precisamos fazer nada
//             continue;
//         }

//         if(border_links.find(u) != border_links.end() && border_links.find(v) != border_links.end()) {
//             // Aqui ambos os vértices não são links de borda, devemos verificar se existe um cluster que
//             // contenha os dois simultaneamente. Se não existir devemos criar um cluster com esses dois vértices
            
//             bool cluster_exists = false;
//             for (unordered_set<int> cluster : clusters) {
//                 if(cluster.find(u) != cluster.end() && cluster.find(v) != cluster.end()) {
//                     // Esse cluster contem ambos os vértices, não precisamos criar um novo cluster
//                     cluster_exists = true;
//                     break;
//                 }
//             }

//             if(cluster_exists){
//                 // Se existe um cluster que contem ambos os vértices, então não precisamos fazer nada
//                 continue;
//             }

//             // Aqui verificamos se existe um circuito iniciado em u
//             // Se existir, então existe um cluster incompleto e o circuito deve ser adicionado a ele
//             vector<int> circuit = findCircuit(u);
//             bool completed_cluster = false;
//             if(circuit.size() > 0){
//                 // Se existe um circuito que contem o vértice u, então existe um cluster incompleto
//                 // que contem o vértice u e v
//                 // printf("Circuito encontrado: ");
//                 // printVector(circuit);
//                 unordered_set<int> circuit_set = unordered_set<int>(circuit.begin(), circuit.end());
//                 seen_by_circuit_links = setUnion(seen_by_circuit_links, circuit_set);
//                 completed_cluster = completeCluster(circuit_set, u, v);
//             }

//             if (completed_cluster){ continue; } // Os links do circuito foram adicionados a um cluster existente
            
//             // Aqui ou não existe um circuito ou não foi possível completar um cluster com o circuito
//             // Então devemos criar um cluster com os dois vértices
//             unordered_set<int> cluster = unordered_set<int>();
//             cluster.insert(u);
//             cluster.insert(v);
//             clusters.push_back(cluster);
//         }
//     }
// }
const int N = 1e5 + 5;

vector<int> adj[N];
int dfn[N], low[N], parent[N], timeStamp;
stack<int> S;
vector<vector<int>> components;

void dfsBicomponent(int u) {
  dfn[u] = low[u] = ++timeStamp;
  S.push(u);

  for (int v : adjacency_list[u]) {
    if (dfn[v] == 0) {
      parent[v] = u;
      dfsBicomponent(v);
      low[u] = min(low[u], low[v]);
      if (low[v] >= dfn[u]) {
        vector<int> component;
        int w;
        do {
          w = S.top();
          S.pop();
          component.push_back(w);
        } while (w != v);
        component.push_back(u);
        components.push_back(component);
      }
    } else if (v != parent[u]) {
      low[u] = min(low[u], dfn[v]);
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

    scanf("%d", &m);
    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d", &x); // Indexação começa em 1
        scanf("%d", &y); // Indexação começa em 1
        
        adjacency_list[x].push_back(y);
        adjacency_list[y].push_back(x); // Assumindo grafo não dirigido

        edges.insert(makeEdge(x, y));
    }

    findBorderLinks();
    // listBorderLinks();

    for (int i = 1; i <= n; i++) {
        if (dfn[i] == 0) {
            dfsBicomponent(i);
        }
    }

    for (const auto& component : components) {
        for (int v : component) {
        cout << v << " ";
        }
        cout << endl;
    }

    // findClusters();
    // listClusters();

    // findClusterBorderForest();
    // listClusterBorderForest();

    return 0;
}
