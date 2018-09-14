#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <algorithm>
#include <cmath>
#define ALL(c) (c).begin(),(c).end()
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
#define sqr(a) (a)*(a)
#define ALL(s) s.begin(),s.end()
#define Det(a,b,c) (LL(b.x-a.x)*LL(c.y-a.y)-LL(b.y-a.y)*LL(c.x-a.x))
/// Det przyjmuje wartosc sin -> + jesli (0,180) - jesli (180,360)
#define PointInsideRect(p1,p2,p3) (min(p1.x,p2.x)<p3.x && max(p1.x,p2.x)>p3.x && min(p1.y,p2.y)<p3.y && max(p1.y,p2.y)>p3.y)
#define PointInRect(p1,p2,p3) (min(p1.x,p2.x)<=p3.x && max(p1.x,p2.x)>=p3.x && min(p1.y,p2.y)<=p3.y && max(p1.y,p2.y)>=p3.y)
#define PointInsideSegment(p1,p2,p3) (Det(p1,p2,p3)==0 && PointInsideRect(p1,p2,p3))
#define PointInSegment(p1,p2,p3) (Det(p1,p2,p3)==0 && PointInRect(p1,p2,p3))
#define PointInsideCircle(c,r,p) (sqr(c.x-p.x)+sqr(c.y-p.y) + EPS < sqr(r))
#define PointInCircle(c,r,p) (sqr(c.x-p.x)+sqr(c.y-p.y) - EPS < sqr(r))
using namespace std;
typedef long long LL;
typedef vector<int> VI;
typedef pair<int, int> PII;
const int INF = 1000000000;
const double EPS = 10e-9;
bool IsZero(double x)
{
    return -EPS<=x && x<=EPS;
}
struct POINT{
  int x,y;
  POINT(int x = 0,int y = 0) : x(x), y(y) { }
  bool operator ==(POINT &a)
  {
   return a.x==x && a.y==y;
  }
};
ostream & operator<<(ostream &a, POINT &p)
  {
   a<<"("<<p.x<<", "<<p.y<<")";
  }
struct POINTD{
  double x,y;
  POINTD(double x = 0,double y = 0) : x(x), y(y) { }
  bool operator ==(POINT &a)
  {
   return IsZero(a.x - x) && IsZero(a.y-y);
  }
};
ostream & operator<<(ostream &a, POINTD &p)
  {
   a<<"("<<p.x<<", "<<p.y<<")";
  }
  bool ordXY (POINT *a, POINT *b)
{
    return a->x==b->x ? a->y < b->y : a->x < b->x;
}
bool ordYX (POINT *a, POINT *b)
{
    return a->y==b->y ? a->x < b->x : a->y < b->y;
}
double PointLineDist(POINTD p1, POINTD p2, POINTD p)
{
    double A = p2.y - p1.y, B = p2.x - p1.x;
    return fabs(A * (p1.x - p.x)+ B * (p1.y - p.y)) / sqrt(A*A + B*B);
}
double PolygonArea(vector<POINT> &p) {
    double area = 0;
    int s = SIZE(p);
    REP(x, s) area += (p[x].x + p[(x+1)%s].x) * (p[x].y - p[(x+1)%s].y);
    return abs(area) / 2;
}
int sgn (double x)
{
    if(x>0)return 1;
    if(x<0)return -1;
    return 0;
}
inline bool SegmentCross(POINT &p1, POINT &p2, POINT &l1, POINT &l2)
{
    return sgn(Det(p1,p2,l1))*sgn(Det(p1,p2,l2))==-1 && sgn(Det(l1,l2,p1))*sgn(Det(l1,l2,p2))==-1;
}
bool PointInsidePolygon(vector<POINT> &l, POINT p)
{
    int v = 0, s = SIZE(l);
    POINT d(INF,p.y + 1);
    REP(x, s) if(PointInSegment(l[x], l[(x+1)%s], p)) return false;
    REP(x,s)  v+=SegmentCross(l[x], l[(x+1)%s],p,d);
    return v & 1;
}
bool PointInPolygon(vector<POINT> &l, POINT p)
{
    int v = 0, s = SIZE(l);
    POINT d(INF,p.y + 1);
    REP(x, s) if(PointInSegment(l[x], l[(x+1)%s], p)) return true;
    REP(x,s)  v+=SegmentCross(l[x], l[(x+1)%s],p,d);
    return v & 1;
}
bool PointInsideConvexPolygon(vector <POINT> &l, POINT p)
{
    int a = 1, b = SIZE(l) - 1, c;
    if(Det(l[0],l[a],l[b])>0) swap(a,b); // a to odcinek po lewej od b
    if(Det(l[0],l[a],p)>0 || Det(l[0],l[b],p)<0) return false; //jesli lezy po lewej od a lub na prawo od b
    while(abs(a-b) > 1)
    {
        c = (a+b)/2;
        Det(l[0],l[c],p)>0 ? b=c : a=c;
    }
    return Det(l[a],l[b],p) < 0;
}
bool PointInConvexPolygon(vector <POINT> &l, POINT p)
{
    int a = 1, b = SIZE(l) - 1, c;
    if(Det(l[0],l[a],l[b])>0) swap(a,b); // a to odcinek po lewej od b
    if(Det(l[0],l[a],p)>0 || Det(l[0],l[b],p)<0) return false; //jesli lezy po lewej od a lub na prawo od b
    while(abs(a-b) > 1)
    {
        c = (a+b)/2;
        Det(l[0],l[c],p)>0 ? b=c : a=c;
    }
    return Det(l[a],l[b],p) <= 0;
}
POINT *RSK;
bool AngleCompare(POINT *a, POINT *b)
{
    LL w = Det((*RSK), (*a), (*b));
    if(w==0) return abs(RSK->x - a->x) + abs(RSK->y - a->y) < abs(RSK->x - b->x) + abs(RSK->y - b->y);
    return w > 0;
}
vector<POINT*> AngleSort (vector<POINT> &p, POINT s, POINT k) {
    RSK=&s;
    vector<POINT*> l,r;
    FOREACH(it, p)
    {
        LL d = Det(s,k,(*it));
        (d > 0 || (d==0 && (s.x==it->x ? s.y < it->y : s.x < it->x))) ? l.PB(&*it) : r.PB(&*it);
        sort(ALL(l), AngleCompare);
        sort(ALL(r), AngleCompare);
        FOREACH(it,r)l.PB(*it);
    }
    return l;
}
vector <POINT*> ConvexHull (vector<POINT> &o)
{
 #define XCAL {while(SIZE(w)>m && Det((*w[SIZE(w)-2]),(*w[SIZE(w)-1]),(*s[x]))<=0)w.pop_back(); w.PB(s[x]);}
 vector<POINT*> s, w;
 FOREACH(i,o) s.PB(&*i);
 sort(ALL(s), ordXY);
 unsigned int m=1;
 REP(x, SIZE(s)) XCAL
 m=SIZE(w);
 FORD(x, SIZE(s) - 2, 0) XCAL
 w.pop_back();
 return w;
}
struct NearestPoint{
    POINT *p1, *p2;
    double dist;
    vector<POINT*> l;
    void Filter(vector<POINT*> &k, double p)
    {
        int s=0;
        REP(x,SIZE(k))if(sqr(k[x]->x - p)<=dist) k[s++] = k[x];
        k.resize(s);
    }
    void Calc(int p, int k, vector<POINT*> &ys)
    {
         if(k - p > 1)
         {
             vector<POINT*> lp, rp;
             int c = (k + p - 1)/2;
             FOREACH(it,ys) ordXY(l[c],*it) ? rp.PB(*it) : lp.PB(*it);
             Calc(p, c + 1, lp);
             Calc(c + 1, k, rp);
             Filter(lp, l[c]->x);
             Filter(rp, l[c]->x);
             int p = 0; double k;
             FOREACH(it, lp) {
                 /// w dowolnym kwadracie o boku dist moze byc conajwyzej 5 pkt ze zbioru (lp+rp)
                 /// dla kazdego punktu lp sprawdzamy najblizsze 4 pod wzgledem y ze zbioru rp
                while(p < SIZE(rp) - 1 && rp[p + 1]->y < (*it) -> y) p++;
                FOR(x,max(0,p-2),min(p+1,SIZE(rp)-1))
                    if(dist > (k = sqr((*it)->x - rp[x]->x) + sqr((*it)->y - rp[x]->y)))
                    {
                        dist = k;
                        p1 = *it;
                        p2 = rp[x];
                    }
             }
         }
    }
    NearestPoint(vector<POINT> &p) {
        FOREACH(it,p) l.PB(&(*it));
        sort(ALL(l),ordXY);
        FOR(x,1,SIZE(l) - 1)
        if(l[x-1]->x == l[x]->x && l[x-1]->y == l[x]->y)
        {
            dist = 0;
            p1 = l[x-1];
            p2 = l[x];
            return;
        }
        dist = double(INF)*double(INF);
        vector<POINT*> v = l;
        sort(ALL(v),ordYX);
        Calc(0,SIZE(l),v);
    }
};
int main()
{
  int n;
  cin>>n;
  vector<POINT> v;
  POINT tmp;
  REP(i,n)
  {
      cin>>tmp.x>>tmp.y;
      v.push_back(tmp);
  }
  vector<POINT*> Hull = ConvexHull(v);
  FOREACH(it,Hull)cout<<" "<<*(*it);
}
