/* 
author :- 
     _____       _____     __    __     __    _____     __        _____      __    __
    /     \     |     \   |  |  |  |   |  |  |     \   |  |      /     \     \ \  / /
   /  /_\  \    |  |\  \  |  |  |  |   |  |  |  |\  \  |  |     /  /_\  \     \ \/ /
  /   __    \   |  | \  \ |  |  |  |   |  |  |  | \  \ |  |    /   __    \     |  |
 /   /  \    \  |  |  \  \|  |  |  |___|  |  |  |  \  \|  |   /   /  \    \    |  |
/___/    \____\ |__|   \_____|  |_________|  |__|   \_____|  /___/    \___ \   |__|

created :- Jun 3,2020.
*/

/*
* deletion method will be added soon.
*/

#include<iostream>
#include<vector>
#include<math.h>

using namespace std;
#define ORDER 4

/*
* Standard B-tree's definition
*/
const int MIN_CHILDREN = ceil(ORDER/2.0);
const int MAX_CHILDREN = ORDER;
const int MIN_KEYS = MIN_CHILDREN-1;
const int MAX_KEYS = MAX_CHILDREN-1;
const int MID = (ORDER - 1)/2;


/*
* The structure of a B-tree node.
* We have keys and the pointers to
* the child node.
*/
struct Node{
    vector<int>keys;
    vector<Node *>children;
    bool is_leaf;
    
    Node(bool is_leaf):is_leaf(is_leaf){}
    
    Node(int val,bool is_leaf):is_leaf(is_leaf){
        keys.insert(keys.begin(),val);
    }
};
using pNode = Node *;

/*
* This structure is a tuple containing
* the boolean,the node of the key and
* the index in the node
*/
struct Search_res{
    bool found;
    pNode node;
    int key_ind;
};

/*
* Each method for our B-tree is 
* implemented in this namespace;
*/
namespace Btree{

    /*
    * To get the index of an element.
    * Params:
    *     (i) the current node we are exploring,
    *     (ii) the index, initially set to 0,
    *     (iii) the element, we are exploring for.
    */
    inline void getInd(pNode node,int &idx,int val){
        while(idx < node->keys.size() && node->keys[idx] < val){
            idx++;                
        }
    }

    /*
    * The auxiliary function for the splitting a node.
    * Params:
    *     (i) the current node we are splitting,
    *     (ii) the new node that will be formed,
    *     (iii),(iv) the range of the values to be assigned.
    */
    void assignValues(pNode node,pNode temp,int range1,int range2){
        vector<int>::iterator it1 = temp->keys.begin();
        vector<pNode>::iterator it2 = temp->children.begin();

        temp->keys.insert(it1,node->keys.begin()+range1,node->keys.begin()+range2);
        if(!temp->is_leaf){
            temp->children.insert(it2,node->children.begin()+range1,node->children.begin()+range2+1);
        }
    }

    /*
    * The main split method.
    * Params:
    *     (i) the parent node of the node to be splitted,
    *     (ii) the node to be splitted,
    *     (iii) the index of the parent node at which the MID key of the curr. node will be placed.
    */
    void split(pNode parNode,pNode node,int idx){

        pNode temp1 = new Node(node->is_leaf);
        pNode temp2 = new Node(node->is_leaf);

        assignValues(node,temp1,0,MID);
        assignValues(node,temp2,MID+1,MAX_KEYS);

        if(parNode->children.size()) parNode->children.erase(parNode->children.begin()+idx);
        parNode->children.insert(parNode->children.begin()+idx,temp1);
        parNode->children.insert(parNode->children.begin()+idx+1,temp2);
    }

    /*
    * The main insert method.
    * Params:
    *     (i) the node we are currently exploring,initially set to root,
    *     (ii) the value to be inserted.
    *
    * Return:
    *       returns the root node.
    */
    pNode insert(pNode node,int val){
        if(!node) return (node = new Node(val,true));

        if(node->keys.size() == MAX_KEYS){
            pNode parNode = new Node(node->keys[MID],false);
            split(parNode,node,0);
            insert(parNode,val);
            return parNode;
        }

        pNode temp = node;
        int idx;

        while(!node->is_leaf){
            idx = 0;
            getInd(node,idx,val);
            if(node->children[idx]->keys.size() == MAX_KEYS){
                node->keys.insert(node->keys.begin()+idx,node->children[idx]->keys[MID]);
                split(node,node->children[idx],idx);
                continue;
            }
            node = node->children[idx];
        }

        idx = 0;
        getInd(node,idx,val);
        node->keys.insert(node->keys.begin()+idx,val);
        return temp;
    }

    /*
    * The main search method.
    * Params:
    *     (i) the node we are currently exploring,initially set to root,
    *     (ii) the key to be searched.
    *
    * Return:
    *       returns the search result, having a bool,
    *       curresponding node and the index of the element.
    */
    Search_res search(pNode node,int key){
        if(!node) return (Search_res){false};
        int idx;
        while(!node->is_leaf){
            idx = 0;
            getInd(node,idx,key);
            if(node->keys[idx] == key) return (Search_res){true,node,idx};
            node = node->children[idx];
        }

        idx = 0;
        getInd(node,idx,key);
        if(node->keys[idx] == key) return (Search_res){true,node,idx};
        return (Search_res){false};
    }

    /*
    * The main printing method(inorder).
    * It prints the key and its depth seperated by a whitespace.
    *
    * Params:
    *     (i) the node we are currently exploring,initially set to root,
    *     (ii) the depth of a curr. node (root has depth 0).
    */
    void inorder(pNode node,int level = 0){
        if(!node) return;
        int i = 0;
        for(;i<node->keys.size();i++){
            if(!node->is_leaf) inorder(node->children[i],level+1);
            cout<<node->keys[i]<<" "<<level<<"\n";
        }
        if(!node->is_leaf) inorder(node->children[i],level+1);        
    }
};

/*
* The main function.
* 'ar' contains the data to be inserted.
*/
int main(){
    pNode root = NULL;
    using namespace Btree;

    int ar[20] = {1,3,9,8,7,2,4,10,6,5,13,17,11,19,15,12,18,14,20,16};
    for(int i = 0;i < 20;i++){
        root = insert(root,ar[i]);
    }
    inorder(root);
    if(search(root,17).found) cout<<"found\n";
    if(!search(root,31).found) cout<<"not found\n";
}
