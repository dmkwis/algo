#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#define ALL(c) (c).begin(),c.end()
#define SIZE(x) ((int)(x).size())
#define REP(x,n) for(int x=0;x<(n);++x)
#define VAR(v,n) __typeof(n) v=(n)
#define FOREACH(i,c) for(VAR(i,(c).begin());i!=(c).end();++i)
#define FOR(i,b,e) for(int i=(b); i <= (e);++i)
#define FORD(i,b,e) for(int i=(b);i>=e;i--)
#define PB push_back
#define ST first
#define ND second
#define MP make_pair
using namespace std;
typedef vector<int> VI;
typedef pair<int, int> PII;
const int INF = 1000000000;
const double EPS = 10e-9;
template <class V,class E> struct Graph
{
    struct Ed : E
    {
        int v;
        Ed(E p, int w) : E(p), v(w) { }
    };
    struct Ve : V, vector<Ed> { };
    vector <Ve> g;
    Graph(int n = 0) : g(n) { }
    void EdgeD(int f, int t, E d = E())
    {
        g[f].PB(Ed(d,t));
    }
    void EdgeU(int f, int t, E d = E())
    {
        Ed eg(d,t);
        eg.rev = SIZE(g[t]) + (f==t);
        g[f].PB(eg);
        eg.rev = SIZE(g[eg.v = f]) - 1;
        g[t].PB(eg);
    }
    void Write()
    {
        REP(x,SIZE(g))
        {
            cout<<x<<":";
            FOREACH(it,g[x])cout<<" "<<it->v;
            cout<<endl;
        }
    }
    void BFS(int s)
    {
        FOREACH(it, g) it->t = it->s = -1;
        g[s].t = 0;
        queue<int> qu;
        qu.push(s);
        while(!qu.empty())
        {
            s=qu.front();
            qu.pop();
            FOREACH(it,g[s])
            if(g[it->v].t==-1)
            {
                qu.push(it->v);
                g[it->v].t = g[s].t + 1;
                g[it->v].s = s;
            }
        }
    }
    void Dfs(int e = -1)
    {
        stack<int> Stos;
        int t = -1, b = 0;
        e == -1 ? e = SIZE(g) - 1 : b = e;//jesli nie podamy parametru e -> dfs dla wszystkich wierzcholkow grafu
        // -> przedzial od [b..e], inaczej to b=e -> przedzial [e..e]
        REP(x,SIZE(g))g[x].d = g[x].f = g[x].s = -1;
        FOR(s,b,e)
        if(g[s].d == -1)
        {
            Stos.push(s);
            g[s].d = ++t;
            g[s].f = SIZE(g[s]); // f jako tymczasowy licznik nieprzetworzonych krawedzi
            while(!Stos.empty())
            {
                int s = Stos.top();
                if(!g[s].f)
                {
                    g[s].f=++t;
                    Stos.pop();
                }
                else
                {
                    if (g[s = g[s][--g[s].f].v].d == -1)//jesli wierzcholek do ktorego prowadzi krawedz jest nieodwiedzony
                    {
                        g[s].s = Stos.top();// s - ojciec w drzewie DFS
                        g[s].f = SIZE(g[s]);
                        g[s].d = ++t;
                        Stos.push(g[s]);
                    }
                }
            }
        }
    }
    int nr;
    void SccSDfs (int v)
    {
        if(g[v].t == -1)
        {
            g[v].t = nr;
            FOREACH(it, g[v]) SccSDfs(it->v);
            if(nr < 0) g[v].t = -(--nr)-3; // NUMER SIE ZMNIEJSZA CALY CZAS CZYLI CALY CZAS < 0 TO TYLKO 1 FAZA ALGORYTMU
        }
    }
    // tylko wyznacza numery silnie spojnych skladowych w grafie
    void SccS()
    {
        Graph<V,E> gt(SIZE(g));
        REP(x,SIZE(g))
        {
            g[x].t = gt.g[x].t = -1; //ustalanie czasu przetworzenia dla wierzcholkow grafow
            FOREACH(it,g[x]) gt.EdgeD(it->v, x); // tworzenie grafu transponowanego
        }
        gt.nr = -2;
        nr = 0;
        VI v(SIZE(g));// v -> vector kolejnosci wyjscia z wierzcholkow
        REP(x, SIZE(g))
        {
            gt.SccSDfs(x);
            v[gt.g[x].t] = x;
        }
        FORD(x, SIZE(g) - 1, 0)
        {
            SccSDfs(v[x]);
            nr++;
        }
    }
    VI vis; // vis->visited
    Graph<V, E> *sccRes; // wskaznik do konstruowanego grafu SCC
    void SccDfs(int v, int nr, bool phase)
    {
        g[v].t = 1;
        if(!phase) vis[v] = nr; // jesli druga faza alg to ustawiamy dla wierzcholka numer SCC
        FOREACH(it, g[v])
        if(g[it->v].t == -1)
            SccDfs(it->v, nr, phase);
        else if(!phase && nr > vis[it->v])
            sccRes->EdgeD(g[it->v].t, vis[it->v] = nr);
        if(phase) vis.PB(v);
        else g[v].t = nr;
    }
    Graph<V,E> Scc()
    {
        Graph<V,E> gt(SIZE(g)), res(SIZE(g)), *tab[] = {this, &gt};
        gt.sccRes = &res;
        gt.vis.resize(SIZE(g), -1);
        vis.clear();
        REP(i, SIZE(g)) FOREACH(it, g[i]) gt.EdgeD(it->v, i);
        // zaczynamy dwie fazy DFS
        REP(i,2)
        {
            //zaznaczamy wierzcholki jako nieodwiedzone
            FOREACH(it, tab[i]->g) it->t = -1;
            int comp = 0, v;
            //Dla kolejnych nieodwiedzonych wierzcholkow przeszukujemy graf
            FORD(j, SIZE(g) - 1, 0)
            {
                if(tab[i]->g[v = (i ? vis[j] : j)].t == -1)
                {
                    tab[i]->SccDfs(v, comp++, 1 - i);
                }
            }
            if(i) res.g.resize(comp);
        }
        REP(i, SIZE(g)) g[i].t = gt.g[i].t;
        return res;
    }
    int topo;
    void TopoDfs(int v)
    {
        if(!g[v].t)
        {
            g[v].t = 1;
            FOREACH(it, g[v]) TopoDfs(it->v);
            g[v].t = --topo;
        }
    }
    void TopoSort()
    {
        FOREACH(it, g) it->t = 0;
        topo = SIZE(g);
        FORD(x, topo - 1, 0) TopoDfs(x);
    }
    VI TopoSortV()
    {
        VI res(SIZE(g));
        TopoSort();
        REP(x,SIZE(g))res[g[x].t]=x;
    }
    bool AcyclicD()
    {
        TopoSort();
        FOREACH(it, g) FOREACH(it2, *it) if(it->t >= g[it2->v].t) return false;
        return true;
    }
    int acyc;
    bool *viz;//vizited
    void AcDfs(int v,Ed *p)
    {
        if(!viz[v])
        {
            FOREACH(it,g[v]) if(&(*it)!=p) AcDfs(it->v, &g[it->v][it->rev]); ///poza ta po ktorej przyszedles
        }
        else acyc=0;
    }
    bool AcyclicU()
    {
        acyc=1;
        viz=new bool[SIZE(g)];
        REP(x,SIZE(g))vis[x]=0;
        REP(x,SIZE(g))AcDfs(x,0);
        delete [] viz;
        return acyc;
    }
    // Algorytm Prima - zwraca wage MinSpanTree
    int MinSpanTree()
    {
        int r=0; // suma wag
        int *d;
        bool *o;
        d=new int[SIZE(g)]; // d[x] -> najmniejsza odleglosc do wierzcholka x z jakiego kolwiek innego przerobionego wierzcholka
        o=new bool[SIZE(g)];
        REP(x,SIZE(g))
        {
            d[x]=INF;
            o[x]=0;
        }
        set<PII> s;
        s.insert(MP(d[0]=0, 0));
        while(!s.empty())
        {
            int v=(s.begin())->ND;
            s.erase(s.begin());
            bool t=0;
            o[v]=1;
            r+=d[v];
            FOREACH(it, g[v])
            {
                it->span=0;
                if(!t && o[it->v] && it->l == d[v])
                    t = it->span = g[it->v][it->rev].span = 1;
                else if(!o[it->v] && d[it->v] > it->l)
                {
                    s.erase(MP(d[it->v],it->v));
                    s.insert(MP(d[it->v] = it->l, it->v));
                }

            }
        }
        delete[] o;
        delete[] d;
        return r;
    }
    struct djcmp
    {
        bool operator() (const Ve *a, const Ve *b) const
        {
            return (a->t == b->t) ? a < b : a->t < b->t;
        }
    };
    void Dijkstra (int s)
    {
        set<Ve*, djcmp> k; /// kolejka priorytetowa
        FOREACH(it, g)
        {
            it->t = INF;
            it->s = -1;
        }
        g[s].t = 0; ///odleglosc
        g[s].s = -1; ///ojciec
        k.insert(&g[s]);
        while(!k.empty())
        {
            Ve *y =*(k.begin());
            k.erase(k.begin());
            FOREACH(it, *y)
            {
                if(g[it->v].t > ((y->t) + (it->l)))
                {
                    k.erase(&g[it->v]);
                    g[it->v].t = y->t + it->l;
                    g[it->v].s = y - &g[0]; /// y to wskaznik na element w wektorze g dlatego trzeba odjac &g[0] zeby miec ilosc elementow ->index
                    k.insert(&g[it->v]);
                }

            }
        }
    }
    bool BellmanFord (int v)
    {
        FOREACH(it, g) it->t = INF;
        g[v].t = 0;
        g[v].s = -1;
        int n = 0;
        bool change;
        do{
            change = 0;
            REP(i,SIZE(g))
            {
                FOREACH(it, g[i])
                {
                    if(g[it->v].t > it->l + g[i].t)
                    {
                        g[it->v].t = it->l + g[i].t;
                        g[it->v].s = i;
                        change = 1;
                    }
                }
            }
        } while(change && n++ < SIZE(g));
        return change;
    }
    int AMatrix[300][300];///umowny size bo nie wiadomo gdzie delete?
    /// dla grafow rzadkich (E~V)
    bool Johnson()
    {
        Ve q;
        g.push_back(q);
        E k;
        k.l = 0;
        REP(x, SIZE(g) - 1)
        {
            EdgeD(SIZE(g) - 1, x, k);
        }
        bool B = BellmanFord(SIZE(g) - 1);
        if(B) return 1;
        g.pop_back();
        int *h = new int [SIZE(g)];
        REP(x,SIZE(g)) h[x] = g[x].t;
        REP(x, SIZE(g)) REP(y, SIZE(g)) AMatrix[x][y] = INF;
        REP(i, SIZE(g))
        FOREACH(it, g[i])/// *******
        {
            it->l = it->l + h[i] - h[it->v]; /// Johnson zmienia wagi krawedzi (zawsze testowac na koncu)
        }
        REP(x, SIZE(g))
        {
            Dijkstra(x);
            REP(y, SIZE(g))if(g[y].t != INF)AMatrix[x][y] = g[y].t - h[x] + h[y];
        }
        delete[] h;
        return 0;
    }
    void FloydWarshall()
    {
       REP(x, SIZE(g)) REP(y, SIZE(g)) AMatrix[x][y] = INF;
       REP(x, SIZE(g)) AMatrix[x][x] = 0;
       REP(x, SIZE(g)) FOREACH(it, g[x]) if(AMatrix[x][it->v] > it->l) AMatrix[x][it->v] = it->l;
       REP(k, SIZE(g)) REP(v1, SIZE(g)) REP(v2, SIZE(g)) if(AMatrix[v1][v2] > AMatrix[v1][k] + AMatrix[k][v2]) AMatrix[v1][v2] = AMatrix[v1][k] + AMatrix[k][v2];
    }

    /// vis -> wyzej zainicjowany VI (sprawdzenie czy V jest visited + trzyma nr warstwy dla kazdego V
    /// EdgeU
    #define ITER typename vector<Ed>::iterator
    ///typename bo Ed jest z template
    int out;

    vector<ITER> itL; /// dla kazdego wierzcholka wskaznik na aktualnie przetwarzana krawedz
    /*

                DO ANALIZY

    int FlowDfs(int x, int fl) ///fl-> current flow
    {
        int r=0, f;
        ///jesli mamy wierzcholek ujscia lub fl==0 czyli nie mozna zwiekszyc przeplywu
        if(x == out || !fl) return fl;
        for(ITER &it = itL[x]; it!=g[x].end();++it)
        {
            ///jesli krawedz jest miedzy kolejnymi warstwami i nie jest nasycona
            if(vis[x] + 1 == vis[it->v] && it->c > it->f) {
                ///wyznaczmy wartosc przeplywu dla tej krawedzi jaki mozna przez nia przepuscic
                    it->f += f = FlowDfs(it->v, min(fl,it->c - it->f)); /// ogarnac
                ///aktualizacja wartosci
                    g[it->v][it->rev].f -= f; ///przeplyw w przeciwna strone (graf nieskierowany)
                    r+=f;
                    fl-=f;
                ///jesli nie mozna powiekszyc sciezki
                    if(fl==0)break;

            }
        }
        return r;
    }
    */
    int DinicMaxFlow(int s, int f)///O(V^2*E)
    {
        int res = 0, n = SIZE(g);
        vis.resize(n);
        itL.resize(n);
        out=f;
        REP(x,n) FOREACH(it,g[x]) it->f=0; ///current flow = 0 dla kazdej krawedzi
        int *q = new int [n]; ///tablicowa kolejka do BFS
        int b,e;
        while(1)
        {
            ///Alg BFS zmodyfikowany
            REP(x,n) {vis[x] = -1; itL[x] = g[x].begin();}///ustawiamy wierzcholki jako nieodwiedzone + ustawiamy poczatkowa krawedz do przetwazania dla kolejnych wierzcholkow
            ///analizujemy tylko nienasycone krawedzie w BFS
            for(q[vis[s] = b = e = 0] = s; b <= e; ++b)
            {
                FOREACH(it, g[q[b]]) if(vis[it->v] == -1 && it->c > it->f) ///JESLI it->v jest nieodwiedzony oraz przetwarzana krawedz nie jest nasycona
                vis[q[++e] = it->v] = vis[q[b]] + 1; /// dodajemy wierzcholek do przetworzenia do kolejki oraz ustalamy ten wierzcholek na odwiedzony
            }
                if(vis[f] == -1) break; ///jesli nie istnieje sciezka ujscia do f
                res+= FlowDfs(s,INF);
        }
        delete[] q;
        return res;
    }
};
struct Vs
{
    int d,f,t,s;
};
struct Ve
{
    int w,rev,l,c,f;
    bool span;
};
int main()
{
    int n,m,f,t;
    cin>>n>>m;
    Ve ed;
    Graph<Vs,Ve> g(n);
    REP(x,m)
    {
        cin>>f>>t>>ed.l;
        g.EdgeD(f,t,ed);
    }
    g.FloydWarshall();
    REP(x,n)
    {
        REP(y,n)cout<<g.AMatrix[x][y]<<" ";
        cout<<endl;
    }
    g.Johnson();
    cout<<endl;
    REP(x,n)
    {
        REP(y,n)cout<<g.AMatrix[x][y]<<" ";
        cout<<endl;
    }
}
