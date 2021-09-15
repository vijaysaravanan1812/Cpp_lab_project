
#include<iostream>
using namespace std; 
// program to implement B tree

template <typename T>
class Btree
{
private:
    struct info
    {
        char name[40];
        long int phone_no;
        void get()
        {
            cout<<"Name :\t\t";
            cin>>name;
            cout<<"Phone no :\t";
            cin>>phone_no;
        }
        void display()
        {
            cout<<name<<" "<<phone_no;
        }
    };
    struct  node
    {
        T *keys;
        info data[5]; 
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
        node        *root; 
         int        t;
        void        insert_not_full(T k,node *&x);
        void        split_child(int i , node *&y, node*&x);
        void        insert(T k, node *& root);
        
        void        traverse(node *& root);
        
    node  *search(T k,node *&root, int &index)
    {
        if(root == NULL)
        {
            printf("No records found\n");
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
            {
                index = i;
                return root;
            }
            // If key is not found here and this is a leaf node
            if (root->leaf == true)
                return NULL;
  
            // Go to the appropriate child
                return search(k,root->C[i],index);
        }
    }
      
        void        remove(T k, node *& root);
        int         findKey(T k, node *& root);
        void        removeFromLeaf(int idx, node *& root);
        void        removeFromNonLeaf(int idx, node *& root);
        int         getPred(int idx, node *& root, node *& temp);
        int         getSucc(int idx, node *& root, node *& temp);
        void        merge(int idx, node *& root);
        void        borrowFromNext(int idx, node *& root);
        void        borrowFromPrev(int idx, node *& root);
        void        fill(int idx, node *& root);
        long int    count_digit(long int number) ;
public:
    
    Btree (int t = 3)
    {
        root = NULL;
        this ->t = t;
    }
        void        insert(T k);
        void        traverse();
    int search(T k)
    {
        int index;
        if (search(k,root, index) == NULL)
        {
            printf("No such Key Found\n");
            getchar();
            return 0;
        }            
        else
        { 
            node *temp = search(k,root,index);
            printf("Enter the New Roll Number\t");
            cin>>temp->keys[index];
            printf("Enter the Data \n");
            temp->data[index].get();
            return 1;
        }       
    }
        void        remove(T k);
    
};

