
#include<iostream>
using namespace std; 
// program to implement B tree

template <typename T>
class Btree
{
private:
    struct   node
    {
        T *keys; 
        node **C;
        int n; 
        int t;
        bool leaf;
        node(int t, bool leaf)
        {
            this->t = t; this->leaf = leaf;
            keys = new T[2*t-1];
            C = new node *[2*t];
  
            // Initialize the number of keys as 0
            n = 0;
        }
    };
    node    * root;
 
    int     t;
    void    insert_not_full(T k,node *&x);
    void    split_child(int i , node *&y, node*&x);
    void    traverse(node *& root);
    node    *search(T k,node *&root)
    {
        if(root == NULL)
        {
            return NULL;
        }    
        else
        {
            // Find the first key greater than or equal to k
            int i = 0;
            while (i < root->n && k > root->keys[i])
                i++;
  
            // If the found key is equal to k, return this node
            if (root->keys[i] == k)
                return root;
  
            // If key is not found here and this is a leaf node
            if (root->leaf == true)
                return NULL;
  
            // Go to the appropriate child
                return search(k,root->C[i]);
        }
    }
    void    insert(T k, node *& root);
    void    remove(T k, node *& root);
    int     findKey(T k, node *& root);
    void    removeFromLeaf(int idx, node *& root);
    void    removeFromNonLeaf(int idx, node *& root);
    int     getPred(int idx, node *& root);
    int     getSucc(int idx, node *& root);
    void    merge(int idx, node *& root);
    void    borrowFromNext(int idx, node *& root);
    void    borrowFromPrev(int idx, node *& root);
    void    fill(int idx, node *& root);
public:
    typedef node *Bnode;
    Btree (int t)
    {
        root = NULL;
        this ->t = t;
    }
    void insert(T k);
    void traverse();
    int search(T k)
    {
        if (search(k,root) == NULL)
            return 0;            
        else 
            return 1;       
    }
    void remove(T k);
    
};


