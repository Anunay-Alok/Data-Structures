/*
The implementation of a persistent segment tree
to answer a range minimum query.

author :- 
     _____       _____     __    __     __    _____     __        _____      __    __
    /     \     |     \   |  |  |  |   |  |  |     \   |  |      /     \     \ \  / /
   /  /_\  \    |  |\  \  |  |  |  |   |  |  |  |\  \  |  |     /  /_\  \     \ \/ /
  /   __    \   |  | \  \ |  |  |  |   |  |  |  | \  \ |  |    /   __    \     |  |
 /   /  \    \  |  |  \  \|  |  |  |___|  |  |  |  \  \|  |   /   /  \    \    |  |
/___/    \____\ |__|   \_____|  |_________|  |__|   \_____|  /___/    \___ \   |__|

created :- Jun 14,2020.
*/

#include<iostream>
#include<vector>
using namespace std;


/*
    structure of the tree's node
    data : (a)the value at the current node
           (b)pointers to left and right nodes after spliiting the curr node
           (c) constructor
*/
struct Node{
    int val;
    Node * l;
    Node * r;
    Node(int val,Node* l,Node* r):val(val),l(l),r(r){}
};

using pNode = Node *;
const int INF = 0x7f7f7f7f;
int n;


/*
    the namespace for all segtree methods
*/
namespace PersistentSegtree{
    
    /*
        initial function to build a tree structure.
        requires just the main array on which we will build the tree

        params: (a) the main/data array
                (b) the range, the node may answer for

        returns: the root for the first segtree.
    */
    pNode build(int * ar,int rangeL = 0,int rangeR = n-1){
        pNode temp,tempL,tempR;
        if(rangeL == rangeR) return temp = new Node(ar[rangeL],0,0);
        int rangeM = (rangeL + rangeR) >> 1;
        tempL = build(ar,rangeL,rangeM);
        tempR = build(ar,rangeM+1,rangeR);
        return temp = new Node(min(tempL->val,tempR->val),tempL,tempR);
    }

    /*
        our method to update the value at an index.
        requires the previous structure of the segtree,
        the index and the new value it should be altered with.

        params: (a) root of the prev status
                (b) the index
                (c) the new value

        returns: the root for the new segtree.
    */
    pNode update(pNode prev,int idx,int newVal,int rangeL = 0,int rangeR = n-1){
        pNode temp,tempL,tempR;
        if(rangeL == rangeR) return temp = new Node(newVal,0,0);
        int rangeM = (rangeL + rangeR) >> 1;
        if(idx <= rangeM) tempR = prev->r,tempL = update(prev->l,idx,newVal,rangeL,rangeM);
        else tempL = prev->l,tempR = update(prev->r,idx,newVal,rangeM+1,rangeR);
        return temp = new Node(min(tempL->val,tempR->val),tempL,tempR);
    }

    /*
        query method returns the range min.
        params: (a) the root of the segtree to be looked
                (b) the range to be searched for

        returns: the minimum of the range.
    */
    int query(pNode node,int l,int r,int rangeL = 0,int rangeR = n-1){
        if(l > r) return INF;
        if(rangeL == l && rangeR == r) return node->val;
        int rangeM = (rangeL + rangeR) >> 1;
        int minLeft = query(node->l,l,min(r,rangeM),rangeL,rangeM);
        int minRight = query(node->r,max(rangeM+1,l),r,rangeM+1,rangeR);
        return min(minLeft,minRight);
    }

    /*
        utility function to know the status of the tree
        requires only the root node
        params: (a) the root of the segtree to be looked
                (b) the node having the data of the range l,r
    */
    void preorder(pNode temp,int l = 0,int r = n-1){
        if(!temp) return;
        cout<<l<<" "<<r<<" "<<temp->val<<"\n";
        preorder(temp->l,l,(l+r)>>1);
        preorder(temp->r,1+(l+r)>>1,r);
    }
};

int main(){
    using namespace PersistentSegtree;

    n = 8;
    int ar[n] = {1,4,7,3,8,6,2,5};  //data
    vector<pNode> roots;            //vector having the root for all updates
    roots.emplace_back(build(ar));
    roots.emplace_back(update(roots[0],3,0));

    preorder(roots[0]);
    cout<<endl<<"\n";
    preorder(roots[1]);
    cout<<endl<<"\n";

    cout<<query(roots[0],3,4)<<"\n";
    cout<<query(roots[1],3,4)<<"\n";
}
