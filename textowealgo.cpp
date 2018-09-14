    #include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
#define VAR(a,c) __typeof(c) a = (c)
#define SIZE(g) g.size()
#define PB push_back
#define REP(x,n) for(int x=0; x<(n); ++x)
#define ND second
#define ST first
#define FOREACH(x,s) for(VAR(x,s.begin()); x!=s.end(); ++x)
using namespace std;
typedef vector<int> VI;
void KMP(const char* wzo,const char* str)
{
    #define KMPH(z) while(k > 0 && wzo[k]!=z[q]) k = p[k]; if(wzo[k] == z[q]) k++;
    int* p = new int [strlen(wzo)+1];
    int k=0, q, m;
    p[1] = 0;
    for(q = 1;wzo[q];q++)
    {
        KMPH(wzo);
        p[q + 1] = k;
    }
    m = q;
    k = 0;
    for(q = 0; str[q]; q++)
    {
        KMPH(str);
        if(m == k) /// found a match
        {
            cout<<q - m + 1<<endl;
            k = p[k];
        }
    }
    delete[] p;
}
int MinPeroid(const char* s)
{
    int *p = new int [strlen(s--) + 1];///s-- decrementing pointer value
    int k = 0, q;
    p [1] = 0;
    for( q = 2; s[q]; q++)
    {
        while(k > 0 && s[k+1] != s[q])k = p[k];
        if(s[k+1] == s[q]) k++;
        p[q] = k;
    }
    delete[] p;
    return q - p[q - 1] - 1;
}
struct mkmp ///Aho-Corasick
{
    struct leaf{
     map<char, leaf*> son;
     leaf *lnk, *wo;
     /// lnk-> wartosc funkcji prefiksowej
     /// wo-> wskaznik na najdluzszy wzorzec bedacy sufiksem tego wierzholka
     int el; ///numer wzorca wierzcholka
     leaf(): el(-1){}
    };
    VI len; ///d³ugosc poszczegolnych wzorcow
    leaf root;
    void AddWord(const char *s)
    {
        int l=strlen(s);
        leaf *p = &root;
        for(; *s; ++s) {
            VAR(e, p->son.find(*s));
            p = (e == p->son.end())? p->son[*s] = new leaf : e->ND;
        }
        if(p->el == -1)
        {
            p->el = SIZE(len); ///jesli wierzcholek nie reprezentuje wzorca, przypisz mu   nowy identyfikator i zapamietaj dlugosc
            len.PB(l);
        }
    }
    leaf *mv(leaf *w, char l)
    {
        while(w != &root && w->son.find(l) == w->son.end()) w = w->lnk;
        if(w->son.find(l) != w->son.end()) w = w.son[l];
    }
    /// funkcja prefiksowa, uzyc po dodaniu wzorcow
    void calcLink()
    {
        vector<leaf*> l;
        leaf *w;
        root.lnk = root.wo = 0; /// root nie uczestniczy w poszukiwaniach wzorcaa
        FOREACH(it, root.son)/// dla kazdego syna korzenia ustawiamy funkcje prefiskowa jego na korzen i wstawiamy do kolejki
        {
            l.PB(it->ND);
            it->ND->lnk = &root;
        }
        REP(x,SIZE(l))
        {
            l[x]->wo = (l[x]->lnk->el != -1) ? l[x]->lnk : l[x]->lnk->wo;
            FOREACH(it,l[x].son)
            {
                l.PB(it->ND);
                w = l[x]->lnk;
                w = mv(w, it->ST);
                ///ustalamy wierzcholek jako wartosc funkcji prefiksowej
                it->ND->lnk = w;
            }
        }
    }
    /*
            ////////////////////////////////////
            ///////      DOKONCZYC TO    ///////
            ////////////////////////////////////
    */
};
int main(){
    string pattern,text;
    cin>>pattern>>text;
    cout<<MinPeroid(pattern.c_str());
    KMP(pattern.c_str(),text.c_str());
}
