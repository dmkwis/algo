#include <iostream>
using namespace std;
struct dpotegowe
{
    int *t, n;
    dpotegowe(int s)
    {
        n = s;
        t = new int [s+1];
    }
    ~dpotegowe()
    {
        delete[] t;
    }
    int pot(int x)
    {
        return x&(-x); /// to samo co x&(x^(x-1))
    }
    void update(int x, int krot)
    {
        while(x <= n)
        {
            t[x] += krot;
            x += pot(x);
        }
    }
    int beforeeq(int x)
    {
        int sum = 0;
        while (x > 0)
        {
            sum += t[x];
            x-= pot(x);
        }
        return sum;
    }
    int nth(int i)
    {
        int x = 0, l = n;
        while(l > 0)
        {
            if(x+l <= n && t[x+l] < i)
            {
                i = i - t[x + l];
                x += l;
            }else
            l >>= 1;
        }
        return x+1;
    }
};

int main()
{
    dpotegowe k(8);
    for(int i=1;i<9;++i)k.update(i,1);
}
