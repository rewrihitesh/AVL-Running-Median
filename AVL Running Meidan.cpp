/* @uthor Hitesh Kumar
    roll no 2019201039
    Assignment 2 Question 3
*/

#include<bits/stdc++.h>
using namespace std;

static int counter=0;

typedef struct _avl {
    int count_nodes;
    int height;
    int data;
    struct _avl* leftPtr;
    struct _avl* rightPtr;
} tree;


tree* rr(tree* root);
tree* ll(tree* root);
tree* rl(tree* root);
tree* lr(tree* root);
tree* getNode(int val);
tree* insertion(tree* root,int val);
tree* find_K(tree* root, int k);
tree* find_kth_largest(tree* root, int k);
tree* _delete(tree* root,int val);
tree* closest_key_node(tree* root, int k,tree* closest_key_root,int min_val);
void update_count(tree *root);
void print_tree(tree* root);
void inverse_print_tree(tree* root);
bool isEmpty(tree* root);
bool getKey(tree* avl, int key);
int getHeight(tree* root);
int getCount(tree* root);
int leftCount(tree* root);
int rightCount(tree* root);
int update_height(tree* root);
int _inorder(tree* root,int breakPoint);
int _converse_inorder(tree* root,int breakPoint);
int closest_k(tree* root, int k,int closest_key,int min_val);
int count_range_val(tree* root,int x, int y);
int pos_k(tree* root, int key);
float getMedian(tree* avl);


bool isEmpty(tree* root){
    if(root->rightPtr == NULL && root->leftPtr == NULL)
        return true;
    return false;
}
int getHeight(tree* root){
    if(root == NULL)
        return 0;
    return root->height;
}

int getCount(tree* root){
    if(root == NULL)
        return 0;
    return root->count_nodes;
}

void update_count(tree *root){
    root->count_nodes = getCount(root->leftPtr) + getCount(root->rightPtr) + 1;
}

tree* rr(tree* root){
    tree* temp= root->rightPtr;
    if(!temp)
        return root;
    root->rightPtr= temp->leftPtr;
    temp->leftPtr= root;
    update_count(root);
    update_count(temp);
    root->height= std::max(getHeight(root->rightPtr),getHeight(root->leftPtr));
    temp->height= std::max(getHeight(temp->rightPtr),getHeight(temp->leftPtr));
    temp->height+=1;
    root->height+=1;
    return temp;
}

tree* ll(tree* root){
    tree* temp= root->leftPtr;
    if(!temp)
        return root;
    root->leftPtr= temp->rightPtr;
    temp->rightPtr= root;
    update_count(root);
    update_count(temp);
    root->height= std::max(getHeight(root->rightPtr),getHeight(root->leftPtr));
    temp->height= std::max(getHeight(temp->rightPtr),getHeight(temp->leftPtr));
    temp->height+=1;
    root->height+=1;
    return temp;
}

tree* rl(tree* root){
        root->rightPtr=ll(root->rightPtr);
        return rr(root);
}

tree* lr(tree* root){
        root->leftPtr=rr(root->leftPtr);
        return ll(root);
}


int leftCount(tree* root){
    if(root==NULL)
        return 0;
    if(root->leftPtr == NULL)
        return 0;
    return root->leftPtr->count_nodes;
}

int rightCount(tree* root){
    if(root==NULL)
        return 0;
    if(root->rightPtr == NULL)
        return 0;
    return root->rightPtr->count_nodes;
}

int update_height(tree* root){
    if(root == NULL)
        return 0;
    if(root->leftPtr == NULL && root->rightPtr == NULL)
        return 0;
    return std::max(getHeight(root->rightPtr),getHeight(root->leftPtr));
}

tree* getNode(int val){
    tree *newNode= (struct _avl*)malloc(sizeof(struct _avl));
    newNode->rightPtr=NULL;
    newNode->leftPtr=NULL;
    // etiher make nodd node height -1 as discussed in class or
    // height 1
    newNode->height=1;
    newNode->count_nodes=1; //count of single node is 1
    newNode->data=val;
    return newNode;
}

void print_tree(tree* root){
        if(root==NULL){
            return ;
        }
        print_tree(root->leftPtr);
        int data = root->data;
        std::cout<<data<<" ";
        print_tree(root->rightPtr);
}


void inverse_print_tree(tree* root){
        if(root==NULL){
            return ;
        }
        inverse_print_tree(root->rightPtr);
        int data = root->data;
        std::cout<<data<<" ";
        inverse_print_tree(root->leftPtr);
}

tree* insertion(tree* root,int val){
    if(root==NULL){
        tree *newNode=getNode(val);
        return newNode;
    }
    if(root->data <= val){//rr
        root->rightPtr=insertion(root->rightPtr,val);
    }else
    if(root->data > val){
        root->leftPtr=insertion(root->leftPtr,val);
    }
    root->height = update_height(root) + 1;

    int bf = getHeight(root->leftPtr) - getHeight(root->rightPtr);
    // +ve left skewd else right

    if(bf>=2){
        if(val>root->leftPtr->data){//lr rotaion
            return lr(root);
        }else{//ll
            return ll(root);
        }
    }
    if(bf<=-2){
        if(val<root->rightPtr->data){//rl
            return rl(root);
        }else{//rr
            return rr(root);
        }
    }
    
    update_count(root);
    return root;
}

int _inorder(tree* root,int breakPoint){
        counter=0;
        stack<tree*> custom_stack;
        tree* backup_root= root;
        //cout<<"inorder: "<<breakPoint<<endl;
        while(backup_root || !custom_stack.empty()){
            while(backup_root){
                custom_stack.push(backup_root);
                backup_root=backup_root->leftPtr;
            }
            backup_root= custom_stack.top();
            custom_stack.pop();
            //cout<<backup_root->data<<endl;
            if(breakPoint==counter){
                return backup_root->data;
            }
            counter++;
            backup_root= backup_root->rightPtr;
        } 
        //handle -1 here
    return -1;  
}



int _converse_inorder(tree* root,int breakPoint){
        counter=0;
        stack<tree*> custom_stack;
        tree* backup_root= root;
        while(backup_root || !custom_stack.empty()){
            while(backup_root){
                custom_stack.push(backup_root);
                backup_root= backup_root->rightPtr;
            }
            backup_root= custom_stack.top();
            custom_stack.pop();
        
            if(breakPoint==counter){
                return backup_root->data;
            }
            counter++;
            backup_root=backup_root->leftPtr;
        } 
        return -1;
    }

tree* find_K(tree* root, int k){
    if(!root)
        return NULL;
    if(getCount(root->leftPtr)+1 == k)
        return root;
    if(k<=getCount(root->leftPtr)){
        return find_K(root->leftPtr,k);
    }else{
        return find_K(root->rightPtr,(k-getCount(root->leftPtr)-1));
    }
}

tree* find_kth_largest(tree* root, int k){
    int total_size_tree=getCount(root);
    return find_K(root, total_size_tree-k+1);
}

float getMedian(tree* avl){
        counter=0;
        double tree_total_node= avl->count_nodes;
        
        int right_node_count = getCount(avl->rightPtr);
        int left_node_count = getCount(avl->leftPtr);

        if(right_node_count == left_node_count)
            return avl->data;
        if((avl->count_nodes)%2){
            double double_mid=tree_total_node/2;
            int median=std::ceil(double_mid);
            float x=(float)(find_K(avl,median)->data);
            return x;
        }else{
                int median = tree_total_node/2;
                float x=(float)(find_K(avl,median)->data);
                float y=(float)(find_K(avl,median+1)->data);
                return (x+y)/2;

        }
}

bool getKey(tree* avl, int key){
        if(!avl)
            return false;
        if(avl->data == key){
            return true;  
        }
        if(key<avl->data)
            return getKey(avl->leftPtr,key);
        if(key>avl->data)
            return getKey(avl->rightPtr,key);
    return false;
}

int closest_k(tree* root, int k,int closest_key= -1 ,int min_val=INT_MAX){

        if(!root)
            return closest_key;
        if(abs(root->data-k)<=min_val){
            //update the key_closest and min_val
            min_val=abs(root->data-k);
            closest_key=root->data;
        }

        if(root->data == k)
            return k;
        if(k>root->data)// go left
            return closest_k(root->rightPtr,k,closest_key,min_val);
        if(k<root->data)// go left
            return closest_k(root->leftPtr,k,closest_key,min_val);
        return closest_key;
}

tree* closest_key_node(tree* root, int k,tree* closest_key_root= NULL ,int min_val=INT_MAX){

        if(!root)
            return closest_key_root;
        if(abs(root->data-k)<min_val){
            //update the key_closest and min_val
            min_val=abs(root->data-k);
            closest_key_root=root;
        }

        if(root->data == k)
            return root;
        if(k>root->data)// go left
            return closest_key_node(root->rightPtr,k,closest_key_root,min_val);
        if(k<root->data)// go left
            return closest_key_node(root->leftPtr,k,closest_key_root,min_val);
        
        return closest_key_root;
}

int pos_k(tree* root, int key){
    if(root->data == key)
        return getCount(root->leftPtr) + 1;
    if(key<root->data){
        return pos_k(root->leftPtr,key);
    }
    if(key>root->data){
        return (getCount(root)-getCount(root->rightPtr)) + pos_k(root->rightPtr,key);
    }
}

int count_range_val(tree* root,int x, int y){
            int balance_out_factor = 0;
            if(x>y){
                //swap
                int temp=x;
                x=y;
                y=temp;
            }
            int temp_x = closest_k(root, x);
            if(temp_x<x){
                balance_out_factor+=1;
            }
            int temp_pos_x=pos_k(root,temp_x);
            int temp_y = closest_k(root, y);
            if(temp_y>y){
                balance_out_factor+=1;
            }
            int temp_pos_y=pos_k(root,temp_y);
            return (temp_pos_y - temp_pos_x)+1 - balance_out_factor;
}

int main(){

    int val;

    int n;
    cin>>n;

    //std::fstream testfile("testq2.txt", std::ios_base::in);
    //testfile>>n;

    tree* avl = NULL;

    for(int i=0;i<n;i++){
        cin>>val;
       // testfile>>val;
        avl= insertion(avl,val);
        cout<<"Running Median: "<<getMedian(avl)<<endl;
        }
    return 0;
}
    