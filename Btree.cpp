
#include<iomanip>
#include<iostream>
#include<cstring>
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
    node  *root; 
	int    t;
    void   insert_not_full(T k,node *&x);
	void   split_child(int i , node *&y, node*&x);
    void   insert(T k, node *& root);
    
	void   traverse(node *& root);
        
    node  *search(T k,node *&root, int &index)
    {
        if(root == NULL)
        {
            //printf("No records found\n");
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
    void  insert(T k);
    void  traverse();
    int   search(T k)
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
	void  remove(T k);
    
};

int S_no=1;

template <typename T>
void Btree<T>::insert(T k)
{
    insert(k,root);
}
 
template <typename T>
void Btree<T>::traverse()
{
    if (root == NULL)
	{
		printf("\t\t<><><><><><><><><><><>\n");
		printf("\t\t| No record to show  |\n");
		printf("\t\t<><><><><><><><><><><>\n");
	}
	else
	{
		S_no = 1;
		
		char s_no[10] = "S Number";
		char roll_no[40] = "Roll Number";
		char name[40] = "Name";
		char phone_no[40] = "Phone Number";
		cout<<setfill('-')<<setw(139)<<"\n";
		printf("|%10s||%40s||%40s||%40s|\n",s_no,roll_no,name,phone_no);
		cout<<setfill('-')<<setw(139)<<"\n";
		traverse(root);
	}
}

// The main function that inserts a new key in this B-Tree
template <typename T>
void Btree<T>::insert(T k , node *& root)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new node(t, true);
        root->keys[0] = k;  // Insert key
		root->data[0].get();
        root->n = 1;  // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            node *s = new node(t, false);
  
            // Make old root as child of new root
            s->C[0] = root;
  
            // Split the old root and move 1 key to the new root
            split_child(0, root, s);
  
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < k)
                i++;
            insert_not_full(k,s->C[i]);
  
            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
            insert_not_full(k,root);
    }

}

template <typename T>
void Btree<T>::split_child(int i , node *& y, node *& x)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    node *z = new node(y->t, y->leaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
	{
        z->keys[j] = y->keys[j+t];
		z->data[j] = y->data[j+t];
	}
    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
  
    // Reduce the number of keys in y
    y->n = t - 1;
  
    // Since this node is going to have a new child,
    // create space of new child
//    printf("x->n = %d i = %d\n",x->n,i);
    for (int j = x->n; j >= i+1; j--)
    {
//        printf("j = %d j + 1 = %d\n" , j,j+ 1 );
       x->C[j+1] = x->C[j];
    }
  
    // Link the new child to this node
    x->C[i+1] = z;
  
    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = x->n-1; j >= i; j--)
	{
        x->keys[j+1] = x->keys[j];
		x->data[j+1] = x->data[j]; 
	}
    // Copy the middle key of y to this node
    x->keys[i] = y->keys[t-1];
	x->data[i] = y->data[t-1];
    // Increment count of keys in this node
    x->n = x->n + 1;

}


template <typename T>
void Btree<T>::insert_not_full(T k, node *& x)
{

    // Initialize index as index of rightmost element
    int i = x->n-1;
  
    // If this is a leaf node
    if (x->leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && x->keys[i] > k)
        {
            x->keys[i+1] = x->keys[i];
			x->data[i+1] = x->data[i];
            i--;
        }
  
        // Insert the new key at found location
        x->keys[i+1] = k;
		x->data[i+1].get();
        x->n = x->n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && x->keys[i] > k)
            i--;
  
        // See if the found child is full
        if ((x->C[i+1])->n == 2*t-1)
        {
            // If the child is full, then split it
        
            split_child(i+1, x->C[i+1], x);
  
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (x->keys[i+1] < k)
                i++;
        }
        //go to right
        insert_not_full(k,x->C[i+1]);
    }

}

template <typename T>
void Btree<T>::traverse(node *&root)
{
        // There are n keys and n+1 children, traverse through n keys
        // and first n children
		int i;

        for (i = 0; i < root->n; i++)
        {
            // If this is not leaf, then before printing key[i],
            // traverse the subtree rooted with child C[i].
            if (root->leaf == false)
                traverse(root->C[i]);
			printf("|%10d||%40d||%40s||%40ld|\n",S_no++,root->keys[i],root->data[i].name,root->data[i].phone_no);
			cout<<setfill('-')<<setw(139)<<"\n";
        }
  
        // Print the subtree rooted with last child
        if (root->leaf == false)
        {
        
            traverse(root->C[i]);
            printf("\n");
        }

}

template <typename T>
void Btree<T>::remove(T k)
{
	if (root == NULL)
	{
		printf("\t\t<><><><><><><><><><><>\n");
		cout << "\t\t| Nothing To Delete! |\n";
		printf("\t\t<><><><><><><><><><><>\n");
		getchar();
		return;
	}

	// Call the remove function for root
	remove(k, root);

	// If the root node has 0 keys, make its first child as the new root
	// if it has a child, otherwise set root as NULL
	if (root->n==0)
	{
		node *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// Free the old root
		delete tmp;
	}
	return;
}

// A function to remove the key k from the sub-tree rooted with this node
template <typename T>
void Btree<T>::remove(T k, node *& root)
{
	int idx = findKey(k,root);

	// The key to be removed is present in this node
	if (idx < root->n && root->keys[idx] == k)
	{

		// If the node is a leaf node - removeFromLeaf is called
		// Otherwise, removeFromNonLeaf function is called
		if (root->leaf)
			removeFromLeaf(idx,root);
		else
			removeFromNonLeaf(idx,root);
	}
	else
	{

		// If this node is a leaf node, then the key is not present in tree
		if (root->leaf)
		{
			cout<<"\t\t"<<setfill('-')<<setw(40 + count_digit(k))<<"\n";
			cout << "\t\t| The Roll Number "<< k <<" is does not exist  |\n";
			cout<<"\t\t"<<setfill('-')<<setw(40 + count_digit(k))<<"\n";			
			getchar();
			return;
		}

		// The key to be removed is present in the sub-tree rooted with this node
		// The flag indicates whether the key is present in the sub-tree rooted
		// with the last child of this node
		bool flag = ( (idx==root->n)? true : false );

		// If the child where the key is supposed to exist has less that t keys,
		// we fill that child
		if ((root->C[idx])->n < t)
			fill(idx,root);

		// If the last child has been merged, it must have merged with the previous
		// child and so we recurse on the (idx-1)th child. Else, we recurse on the
		// (idx)th child which now has atleast t keys
		if (flag && idx > root->n)
			remove(k,root->C[idx-1]);
		else
			remove(k,root->C[idx]);
	}
	return;
}

template <typename T>
// A utility function that returns the index of the first key that is
// greater than or equal to k
int Btree<T>::findKey(T k , node *& root)
{
	int idx=0;
	while (idx< root->n && root->keys[idx] < k)
		++idx;
	return idx;
}


template <typename T>
// A function to remove the idx-th key from this node - which is a leaf node
void Btree<T>::removeFromLeaf(int idx,node *& root)
{

	// Move all the keys after the idx-th pos one place backward
	for (int i=idx+1; i< root->n; ++i)
	{
		root->keys[i-1] = root->keys[i];
		root->data[i-1] = root->data[i];
	}
	// Reduce the count of keys
	root->n--;

	return;
}

template <typename T>
// A function to remove the idx-th key from this node - which is a non-leaf node
void Btree<T>::removeFromNonLeaf(int idx, node *& root)
{

	int k = root->keys[idx];

	// If the child that precedes k (C[idx]) has atleast t keys,
	// find the predecessor 'pred' of k in the subtree rooted at
	// C[idx]. Replace k by pred. Recursively delete pred
	// in C[idx]
	if (root->C[idx]->n >= t)
	{
		node *temp;
		int pred = getPred(idx, root, temp);
		root->keys[idx] = pred;
		root->data[idx] = temp->data[temp->n -1];
		remove(pred,root->C[idx]);
	}

	// If the child C[idx] has less that t keys, examine C[idx+1].
	// If C[idx+1] has atleast t keys, find the successor 'succ' of k in
	// the subtree rooted at C[idx+1]
	// Replace k by succ
	// Recursively delete succ in C[idx+1]
	else if ((root->C[idx+1])->n >= t)
	{
		node *temp;
		int succ = getSucc(idx,root,temp);
		root->keys[idx] = succ;
		root->data[idx] = temp->data[0];
		remove(succ,root->C[idx+1]);
	}

	// If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
	// into C[idx]
	// Now C[idx] contains 2t-1 keys
	// Free C[idx+1] and recursively delete k from C[idx]
	else
	{
		merge(idx,root);
		remove(k,root->C[idx]);
	}
	return;
}

template <typename T>
// A function to get predecessor of keys[idx]
int Btree<T>::getPred(int idx, node *& root, node *& temp)
{
	// Keep moving to the right most node until we reach a leaf
	node *cur   =  root->C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	// Return the last key of the leaf
	temp = cur;
	return cur->keys[cur->n-1];
}

template <typename T>
int Btree<T>::getSucc(int idx, node *& root, node *& temp)
{

	// Keep moving the left most node starting from C[idx+1] until we reach a leaf
	node *cur = root->C[idx+1];
	while (!cur->leaf)
		cur = cur->C[0];

	// Return the first key of the leaf
	temp = cur;
	return cur->keys[0];
}

template <typename T>
// A function to merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
void Btree<T>::merge(int idx, node *& root)
{
	node *child = root->C[idx];
	node *sibling = root->C[idx+1];

	// Pulling a key from the current node and inserting it into (t-1)th
	// position of C[idx]
	child->keys[root->t-1] = root->keys[idx];
	child->data[root->t - 1] = root->data[idx];

	// Copying the keys from C[idx+1] to C[idx] at the end
	for (int i=0; i<sibling->n; ++i)
	{
		child->keys[i+root->t] = sibling->keys[i];
		child->data[i+root->t] = sibling->data[i];
	}
	// Copying the child pointers from C[idx+1] to C[idx]
	if (!child->leaf)
	{
		for(int i=0; i<=sibling->n; ++i)
			child->C[i+root->t] = sibling->C[i];
	}

	// Moving all keys after idx in the current node one step before -
	// to fill the gap created by moving keys[idx] to C[idx]
	for (int i=idx+1; i<root->n; ++i)
	{
		root->keys[i-1] = root->keys[i];
		root->data[i-1] = root->data[i];
	}
	// Moving the child pointers after (idx+1) in the current node one
	// step before
	for (int i=idx+2; i<=root->n; ++i)
		root->C[i-1] = root->C[i];

	// Updating the key count of child and the current node
	child->n += sibling->n+1;
	root->n--;

	// Freeing the memory occupied by sibling
	delete(sibling);
	return;
}

template <typename T>
// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void Btree<T>::borrowFromNext(int idx, node *& root)
{

	node *child     =   root->C[idx];
	node *sibling   =   root->C[idx+1];

	// keys[idx] is inserted as the last key in C[idx]
	child->keys[(child->n)] = root->keys[idx];
	child->data[(child->n)] = root->data[idx];

	// Sibling's first child is inserted as the last child
	// into C[idx]
	if (!(child->leaf))
		child->C[(child->n)+1] = sibling->C[0];

	//The first key from sibling is inserted into keys[idx]
	root->keys[idx] = sibling->keys[0];
	root->data[idx] = sibling->data[0];
	// Moving all keys in sibling one step behind
	for (int i=1; i < sibling->n; ++i)
	{
		sibling->keys[i-1] = sibling->keys[i];
		sibling->data[i-1] = sibling->data[i];
	}
	// Moving the child pointers one step behind
	if (!sibling->leaf)
	{
		for(int i=1; i<=sibling->n; ++i)
			sibling->C[i-1] = sibling->C[i];
	}

	// Increasing and decreasing the key count of C[idx] and C[idx+1]
	// respectively
	child->n += 1;
	sibling->n -= 1;

	return;
}

template <typename T>
// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void Btree<T>::borrowFromPrev(int idx, node *& root)
{

	node *child     =   root->C[idx];
	node *sibling   =   root->C[idx-1];

	// The last key from C[idx-1] goes up to the parent and key[idx-1]
	// from parent is inserted as the first key in C[idx]. Thus, the loses
	// sibling one key and child gains one key

	// Moving all key in C[idx] one step ahead
	for (int i=child->n-1; i>=0; --i)
	{
		child->keys[i+1] = child->keys[i];
		child->data[i+1] = child->data[i];
	}
	// If C[idx] is not a leaf, move all its child pointers one step ahead
	if (!child->leaf)
	{
		for(int i=child->n; i>=0; --i)
			child->C[i+1] = child->C[i];
	}

	// Setting child's first key equal to keys[idx-1] from the current node
	child->keys[0] = root->keys[idx-1];
	child->data[0] = root->data[idx-1];
	// Moving sibling's last child as C[idx]'s first child
	if(!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// Moving the key from the sibling to the parent
	// This reduces the number of keys in the sibling
	root->keys[idx-1] = sibling->keys[sibling->n-1];
	root->data[idx-1] = sibling->data[sibling->n -1];
	child->n += 1;
	sibling->n -= 1;

	return;
}

template <typename T>
// A function to fill child C[idx] which has less than t-1 keys
void Btree<T>::fill(int idx,node *& root)
{

	// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
	// from that child
	if (idx!=0 && (root->C[idx-1])->n>=t)
		borrowFromPrev(idx,root);

	// If the next child(C[idx+1]) has more than t-1 keys, borrow a key
	// from that child
	else if (idx!=root->n && (root->C[idx+1])->n>=t)
		borrowFromNext(idx,root);

	// Merge C[idx] with its sibling
	// If C[idx] is the last child, merge it with with its previous sibling
	// Otherwise merge it with its next sibling
	else
	{
		if (idx != root->n)
			merge(idx,root);
		else
			merge(idx-1,root);
	}
	return;
}

template <typename T>
long int Btree<T>::count_digit(long int number) {
   int count = 0;


   while(number != 0) {
      number = number / 10;
      count++;
   }
 
   return count;
}


// Driver program to test above functions
int main()
{
	system("clear");
	Btree <int> t; // A B-Tree with minium degree 3

	int count = 1; 
	while (count)
	{
		system("clear");
		int choice;
		printf("Insert - 1 \tDisplay -2 \tRemove -3 \tEdit -4\n");
		
		cin>>choice;
		if (choice == 1)
		{
			int time;
			
			printf("How Many records you need to insert\t");
			cin>>time;
			for (int i = 1; i <= time; i++)
			{		
				int roll_no;   
				printf("Roll Number should be Unique\nStudent %d roll number \t",i);
				cin>>roll_no;
				t.insert(roll_no);
			}
			
		}
		else if (choice == 2)
		{
			t.traverse();
			getchar();
		}
		else if (choice == 3)
		{
			int roll_no;
			printf("Enter roll no to remove that record\t");
			cin>>roll_no;
			t.remove(roll_no);
			
		}
		else if (choice == 4)
		{
			int roll_no;
			printf("Enter Old Roll_Number\t");
			cin>>roll_no;
			t.search(roll_no);
		}
		else
		{
			char ch;
			printf("\t\t<><><><><><><><><><><><><><><><>\n");
			printf("\t\t| Are you sure do want to exix |\n");
			printf("\t\t<><><><><><><><><><><><><><><><>\n");
			printf("\t\t|%-30s|\n","   Yes - y/Y  |  No - n/N  ");
			printf("\t\t<><><><><><><><><><><><><><><><>\n\t\t");
			cin>>ch;
			if (ch == 'Y' || ch == 'y')
				count = 0;
			else if(ch == 'n' || ch == 'N')
				count = 1;
			else
			{
				printf("Invalid Input!!\n");
				getchar();
			}
		}

		getchar();
		
	}
	
	return 0;
}


