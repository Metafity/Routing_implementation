#include <iostream>
#include <limits>
using namespace std;

class Router {
    int identifier;
    int num;
    int* Table;
    int* nextHop;
public:
    void Init(int id, int n) {
        identifier = id;
        num = n;
        Table = new int[num];
        nextHop = new int[num];
        for(int i = 0; i < num; i++) {
            Table[i] = INT_MAX;
            nextHop[i] = -1;
        }
    }
    void SetDistance(int i, int d) {
        Table[i] = d;
    }
    void SetHop(int i, int j) {
        nextHop[i] = j;
    }
    int GetDistance(int i) {
        return Table[i];
    }
    int GetMin(bool visited[]) {
        int min = INT_MAX, mindex;
        for(int i = 0; i < num; i++) {
            if( Table[i] < min && visited[i] == false) {
                min = Table[i];
                mindex = i;
            }
        }
        return mindex;
    }
    int* GetTable() {
        return Table;
    }
    void UpdateTable(int* arr[]) {
            for(int i = 0; i < num; i++) {                                                       //for all neighbouring routers
                  if(arr[i] == NULL) continue;
                  for(int j = 0; j < num; j++) {                                                 //for every router except this
                        if(j == identifier) continue;
                        else {
                            if( (*(arr[i] + j) != INT_MAX) && ( *(arr[i] + j) + Table[i] < Table[j])) {     //set table
                                Table[j] = *(arr[i] + j) + Table[i];
                                nextHop[j] = i;
                            }
                        }
                  }
            }
    }
    void Print() {
        for(int i = 0; i < num; i++) {
            if(Table[i] == INT_MAX) cout<<"Can't reach "<<i+1<<"th router"<<endl;
            cout<<"Shortest distance from "<<identifier+1<<"th to "<<i+1<<"th router is "<<Table[i]<<" through hop "<<nextHop[i]<<endl;
        }
    }
};

class Network {
    int num;
    Router* nodes;
    int** graph;

    void FillGraph() {
        for(int i = 0; i < num; i++) {
            for(int j = 0; j < num; j++) {
                graph[i][j] = 0;
            }
        }
        char c='n';
        int x, y, d;
        cout<<"Add an edge?"<<endl;
        cin>>c;
        while(c == 'y') {
            cout<<"Enter the router identifiers and the distance between them"<<endl;
            cin>>x>>y>>d;
            graph[x][y] = graph[y][x] = d;
            cout<<"Enter another edge?"<<endl;
            cin>>c;
        }

    }
public:
    Network(int n) {
        if(n <= 0) throw("No routers, no network\n");
        num = n;
        nodes = new Router[num];
        for(int j = 0; j < num; j++) {
            (nodes + j)-> Init(j, num);
        }
        graph = new int*[num];
        for(int i = 0; i < num; i++) {
            graph[i] = new int[num];
        }
        FillGraph();
    }

    void ShortestPath(int n) {
        if(n >= num) throw("Invalid router\n");

        bool visited[num];
        for(int i = 0; i < num; i++) visited[i] = false;

        (nodes + n)->SetDistance(n, 0);

        for(int j = 0; j < num; j++) {
                int u = (nodes + n)->GetMin(visited);
                visited[u] = true;
                for(int k = 0; k < num; k++ ) {
                    if( !visited[k] && graph[u][k] != 0 && (nodes +n)->GetDistance(u) != INT_MAX &&  ((nodes + n)->GetDistance(u) + graph[u][k] < (nodes + n)->GetDistance(k))) {
                        (nodes + n)->SetDistance(k, ((nodes + n)->GetDistance(u) + graph[u][k])); //hops to do
                    }
                }
        }
    }
    void ComputeShortestPaths() {
        for(int i = 0; i < num; i++) ShortestPath(i);
    }
    void ExchangeTables(Router* node, int it) {
            if( it == num) return;                                      //Base case

            int* arr[num];                                              //Pointer to revieved tables
            for(int i = 0; i < num; i++) {
                if(graph[it][i]) arr[i] = (nodes + i)->GetTable();      //Get table from all neighbouring routera
                else arr[i] = NULL;
            }
            ExchangeTables(node, it + 1);                               //Call function for next router
            (node + it)->UpdateTable(arr);                              //Update table
    }

    void DistanceVector () {
        for(int i = 0; i < num ; i++) {
            for(int j = 0; j < num; j++) {
                if(i == j) {
                        (nodes + i)->SetDistance(i, 0);                 //set distance and hop for self
                        (nodes + i)->SetHop(i, -1);
                }
                else if(graph[i][j] && graph[i][j] < (nodes + i)->GetDistance(j)) {
                        (nodes + i)->SetDistance(j, graph[i][j]);       //set distances and hops for neighbours
                        (nodes + i)->SetHop(j, j);
                }
            }
        }
        ExchangeTables(nodes, 0);
    }

    void Print() {
        for(int i = 0; i < num; i++) (nodes + i)->Print();
        cout<<endl;
    }
};

int main()
{
    Network net(4);
    net.DistanceVector();
    net.DistanceVector();
    //net.ComputeShortestPaths();
    net.Print();
}
