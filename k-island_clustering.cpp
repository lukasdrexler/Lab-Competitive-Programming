#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

/*Graph Class
 * Nested classes are Vertex and Edge
 * already equipped with UNION-FIND structure
 * Graph basically consists of a vector of pointers to its vertices
 * Vertices have adjacency lists with pointers to their neighbours
 * Edges are also stored as explicit objects (needed for Kruskal)
 */


class Graph{
public:

    class Vertex{
    public:
        int id;
        std::list<Graph::Vertex*> neighbours;
        Graph::Vertex *parent;                  //only for UNION-FIND

        Vertex(int ident);

    };


    class Edge{                                 //Edges consist of pointers to the two endpoints and a weight
    public:
        Graph::Vertex *head;
        Graph::Vertex *tail;
        int weight;

        Edge(Graph::Vertex *h, Graph::Vertex *t, int w);
    };



    Graph(int n, int m);





    std::vector<Graph::Vertex*> vertices;       //Vector of (pointers to) all vertices
    std::vector<Graph::Edge*> edges;            //Vector of (pointers to) all edges
    std::vector<int> sizes;                     //for UNION-FIND only (keeping track of size of components)


    void add_vertex(Vertex *v);
    void add_edge(Edge *e,int i);               //Vector of (pointers to) all edges
    void sort_edges();


    Graph::Vertex* find(Graph::Vertex *v);
    void unify(Graph::Vertex* u, Graph::Vertex* v);



    static bool compare_edges(Graph::Edge *e, Graph::Edge *f){
        return (e->weight < f->weight);
    }






};
//Constructors for Vertex, Edge and Graph
Graph::Vertex::Vertex(int ident) {
    id = ident;
    parent = this;
}


Graph::Edge::Edge(Graph::Vertex *h, Graph::Vertex *t, int w) {
    head = h;
    tail = t;
    weight = w;
}


Graph::Graph(int n, int m){               //constructor only initializes vectors of vertices and sizes, edges has m NULL pointers
    vertices.resize(n);
    sizes.resize(n);
    edges.resize(m);
    for(int i = 0; i < n; i++){
        Vertex* v = new Vertex(i);
        vertices[i] = v;
        sizes[i] = 1;
    }
};



//Member functions of Graph

void Graph::add_vertex(Vertex *v){              //not needed as of now
    vertices.push_back(v);
    return;
}

void Graph::add_edge(Graph::Edge *e,int i){
    Vertex* h = e->head;
    Vertex* t = e->tail;
    vertices[h->id]->neighbours.push_back(t);   //add each vertex to the other vertex' adjacency list
    vertices[t->id]->neighbours.push_back(h);
    edges[i] = e;                               //store edge as actual object in edge list
    return;
}

void Graph::sort_edges() {
    std::sort(edges.begin(),edges.end(),Graph::compare_edges);
    return;
}


//These functions are only for the UNION-FIND structure (Kruskal etc.)

Graph::Vertex* Graph::find(Graph::Vertex *v) {      //finds root of connected component containing v
    if (v->parent->id != v->id){
        v->parent = find(v->parent);                //makes all parent pointers of vertices on the path to the root point to the root
    }
    return v->parent;
}


void Graph::unify(Graph::Vertex* u, Graph::Vertex* v){      //joins two connected components
    Graph::Vertex* root_u = find(u);
    Graph::Vertex* root_v = find(v);
    if(sizes[root_u->id] <= sizes[root_v->id]){              //to keep paths short, the smaller component is appended to the larger
        root_u->parent = v;
        sizes[root_v->id] = sizes[root_v->id] + sizes[root_u->id];
    }
    if(sizes[root_v->id] < sizes[root_u->id]){
        root_v->parent = u;
        sizes[root_u->id] = sizes[root_v->id] + sizes[root_u->id];
    }

}




/* partial_Kruskal receives a graph and an integer k. It then performs (n-k) iterations of Kruskal's algorithm
(i.e. until k connected components are left). It returns a vector of all chosen edges.
To perform the "regular" Kruskal, one can call partial_Kruskal with k = 1.
(It returns a vector of pointers to the chosen edges. This is not necessary for this task, but might be of use for later tasks)
*/

std::vector<Graph::Edge*> partial_Kruskal(Graph &g, int k){
    int number_edges = 0;                                       //we start without any edges
    std::vector<Graph::Edge*> min_weight_forest(g.vertices.size()-k);

    g.sort_edges();                                             //sort edges by weight (non-decreasing)

    for(int i = 0; i < g.edges.size();i++){
        Graph::Edge* current_edge = g.edges[i];
        Graph::Vertex* root_h = g.find(current_edge->head);
        Graph::Vertex* root_t = g.find(current_edge->tail);

        if(root_h != root_t){                                   //if endpoints of current edge are in different connected components
            g.unify(current_edge->head,current_edge->tail);     //join those connected components
            min_weight_forest[number_edges] = current_edge;     //and add the edge to output
            number_edges += 1;
        }
        if(number_edges == g.vertices.size()-k){                //if we have chosen n-k edges
            for(int j = i; j < g.edges.size(); j++){            //go through all remaining edges
                Graph::Edge* e = g.edges[j];
                if(g.find(e->head) != g.find(e->tail)){
                    std::cout << e->weight;                     //and print weight of cheapest edge between to connected components
                    return min_weight_forest;                   //not relevant for this task, but might come in handy later
                }
            }

        }
    }

}



int main(){

    int n,k,m;

    std::cin >> n;
    std::cin >> k;
    std::cin >> m;
    Graph g = Graph(n,m);



    for (int i = 0; i < m; ++i) {           //read edges and add them to graph
        int head_id, tail_id, weight;
        std::cin >> head_id >> tail_id >> weight;
        Graph::Vertex* head = g.vertices[head_id];
        Graph::Vertex* tail = g.vertices[tail_id];
        Graph::Edge* e = new Graph::Edge(head,tail,weight);
        g.add_edge(e,i);
    }


    //KRUSKAL PART
    std::vector<Graph::Edge*> min_weight_forest = partial_Kruskal(g,k);     //output happens in function partial_Kruskal


}
