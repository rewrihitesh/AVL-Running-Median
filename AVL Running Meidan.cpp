/* @uthor Hitesh Kumar
    Roll no 2019201039
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
    return -1;    
}

int _converse_inorder(tree* root,int breakPoint){
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
            // ----- I have to test it yet -----
            // if(median<=left_node_count){
            //     return _converse_inorder(avl->leftPtr,left_node_count-median);
            // }else{
            //     return _inorder(avl->rightPtr,median-right_node_count);
            // }
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

int main(){

    ios_base::sync_with_stdio(false);

    cin.tie(NULL);

    int val;

    int n;

    cin>>n;

    tree* avl = NULL;

     for(int i=0;i<n;i++){
        cin>>val;
        avl= insertion(avl,val);
        
        printf("%.1f\n",getMedian(avl));
        }

    return 0;

}