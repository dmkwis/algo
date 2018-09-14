#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;
struct BSTNode
{
    int value;
    BSTNode *left, *right,*up;
};
string cr,cl,cp;
void printBT(string sp, string sn, BSTNode * v)
{
  string s;
  cr = cl = cp = "  ";
  cr[0] = 218; cr[1] = 196;
  cl[0] = 192; cl[1] = 196;
  cp[0] = 179;
  if(v)
  {
    s = sp;
    if(sn == cr) s[s.length() - 2] = ' ';
    printBT(s + cp, cr, v->right);

    s = s.substr(0,sp.length()-2);
    cout << s << sn << v->value << endl;

    s = sp;
    if(sn == cl) s[s.length() - 2] = ' ';
    printBT(s + cp, cl, v->left);
  }
}
void DFSRelease(BSTNode *v)
{
    if(v)
    {
        DFSRelease(v->left);
        DFSRelease(v->right);
        delete v;
    }
}
void DFSVisit(BSTNode*v)
{
    if(v)
    {
        DFSVisit(v->left);
        cout<<v->value<<endl;//in-order=sortowanie
        DFSVisit(v->right);
    }
}
void BSTAdd(BSTNode *&root,int val)
{
    BSTNode *next,*tmp;
    next=new BSTNode;
    next->value=val;
    next->right=next->left=NULL;
    tmp=root;
    if(!tmp)
    {
        root=next;
        root->up=NULL;
    }
    else
    {
        while(true)
        {
            if(val<tmp->value)
            {
                if(tmp->left==NULL)
                {
                    tmp->left=next;
                    break;
                }
                else
                {
                    tmp=tmp->left;
                }
            }
            else
            {
                if(tmp->right==NULL)
                {
                    tmp->right=next;
                    break;
                }
                else
                {
                    tmp=tmp->right;
                }
            }
        }
        next->up=tmp;
    }
}
void R_rot(BSTNode *&root,BSTNode *A)
{
    BSTNode *p=A->up,*B=A->left;
    if(!B)return;//jesli lewe poddrzewo istnieje
    A->left=B->right;
    if(A->left)A->left->up=A;
    B->right=A;
    A->up=B;
    B->up=p;
    if(p)
    {
        if(p->right==A)p->right=B;else p->left=B;
    }else root=B;
}
void L_rot(BSTNode *&root,BSTNode *A)
{
    BSTNode *p=A->up,*B=A->right;
    if(!B)return;//jesli prawe poddrzewo istnieje
    A->right=B->left;
    if(A->right)A->right->up=A;
    B->left=A;
    A->up=B;
    B->up=p;
    if(p)
    {
        if(p->right==A)p->right=B;else p->left=B;
    }else root=B;
}
unsigned int log2(unsigned int x)
{
  unsigned int y = 1;
  while((x >>= 1)) y <<= 1;
  return y;
}
void DSWRebalance(BSTNode*& root)
{
    int n=0;
    BSTNode *current=root;
    while(current!=NULL)
    {
        if(current->left!=NULL)
        {
            R_rot(root,current);
            current=current->up;
        }else{
        n++;
        current=current->right;
        }
    }
    int s=n+1-log2(n+1);
    current=root;
    for(int i = 0; i < s; i++)
    {
        L_rot(root,current);
        current = current->up->right;
    }
    n = n - s;
  while(n > 1)
  {
    n >>= 1;
    current = root;
        for(int i = 0; i < n; i++)
    {
      L_rot(root,current);
      current = current->up->right;
    }
  }
}
int main()
{
    BSTNode* root=NULL;
    srand(time(NULL));
    int n=40;

    for(int i=1; i<=n; i++)
    {
        int add;
        add=i;
        BSTAdd(root,add);
    }
    printBT("","",root);
    DSWRebalance(root);
    printBT("","",root);
    DFSVisit(root);
    DFSRelease(root);
    return 0;
}
