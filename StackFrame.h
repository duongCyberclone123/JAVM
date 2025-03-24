#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
#include <stack>



/*
StackFrame declaration
*/
enum{
    S, E, G
};
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    class Stack{
        private:
            double val;
            int state;
        public:
            Stack(double val=0.0,int state=0){
                this->val=val;
                this->state = state;
            }
            double getVal(){
                return val;
            }   
            int getState(){
                return state;
            }
            void setVal(double val){
                this->val=val;
            } 
            void setState(int state){
                this->state=state;
            }
            ~Stack(){}
    };
    void printStack(std::stack<Stack*> stackFrame);
    class ArrayAVL{
        public: class Node;
        private:
            Node*root;
            int num_node;
        public:
            class Node{
                private:        
                    double val;
                    int state;
                    std::string key;
                    friend class ArrayAVL;
                public:
                Node* left;
                Node*right;
                
                Node(double val = 0.0,int state = -1, std::string key = ""){
                    left=nullptr;
                    right=nullptr;
                    this->val = val;
                    this->state = state;
                    this->key = key;
                }

                double getVal(){
                return val;
                }   
                int getState(){
                    return state;
                }
                void setVal(double val){
                    this->val=val;
                } 
                void setState(int state){
                    this->state=state;
                }
                std::string getKey(){return key;}
            };
                ArrayAVL(){
                    root=NULL;
                    num_node=0;
                }
                //fix
                int getHeightRec(Node *node)
                {
                    if (node == NULL)
                        return 0;
                    int lh = this->getHeightRec(node->left);
                    int rh = this->getHeightRec(node->right);
                    return (lh > rh ? lh : rh) + 1;
                }
                int getHeight()
                {
                    return this->getHeightRec(this->root);
                }

                int getBalance(Node* root) {
                    if (!root) return 0;
                    return (getHeightRec(root->left)-getHeightRec(root->right));
                }

                // 1st str to be smaller
                int strcmp(std::string a, std::string b){
                    int n = a.size() > b.size()?b.size():a.size();
                    for (int i = 0; i < n; i++){
                        if (a[i] < b[i]) return S;
                        else if (a[i] > b[i]) return G;
                    }
                    if (a.size() > b.size()) return G;
                    else if (a.size() == b.size()) return E;
                    else return S;
                }

                Node* rotateRight(Node* root) {
                //TODO: Rotate and return new root after rotate
                    Node* temp=root->left;
                    root->left=temp->right;
                    temp->right=root;
                    return temp;
                }

                Node* rotateLeft(Node* root) {
                //TODO: Rotate and return new root after rotate
                    Node*temp=root->right;
                    root->right=temp->left;
                    temp->left=root;
                    return temp;
                }

                Node* insertNode(Node*root,double val, int state, std::string key) {
                    if (!root) {
                        return new Node(val, state, key);
                    }
                
                    if (strcmp(key,root->key) == S) {
                        root->left= insertNode(root->left, val, state, key);
                    } else {
                        root->right= insertNode(root->right, val,state, key);
                    } 
                   
                    int balance=getBalance(root);

                    if (balance > 1 && strcmp(key, root->left->key) == S) {
                        return rotateRight(root);
                    }
                    //right right case
                    if (balance<-1 && (strcmp(key, root->right->key) == G || strcmp(key, root->right->key) == E)) {
                        return rotateLeft(root);
                    }
                    //left right case
                    if (balance>1 && (strcmp(key,root->left->key) == G || strcmp(key,root->left->key) == E)) {
                        root->left=rotateLeft(root->left);
                        return rotateRight(root);
                    }
                    if (balance<-1 && strcmp(key, root->right->key) == S) {
                        root->right=rotateRight(root->right);
                        return rotateLeft(root);
                    }
                    return root;
                }

                void insert(double val, int state, std::string key){
                    //TODO
                    if (!root) {
                        num_node++;
                        
                        root=new Node(val, state, key);

                    }
                    Node* existNode = searchNode(key);
                    if (existNode){
                        existNode->setState(state);
                        existNode->setVal(val);
                        return;
                    }
                    else {
                        root=insertNode(root,val, state, key);
                        num_node++;
                    }
                }

                //end fix
                int getNum(){return num_node;}

                Node* getRoot(){return root;}

                void setRoot(Node*node){root = node;}

                Node* levelTraverse(Node* root, int level, std::string key) {
                    if (!root) return NULL;
                    if (level == 1) {
                        if (root->key == key) return root;
                    } else {
                        Node* leftResult = levelTraverse(root->left, level - 1, key);
                        if (leftResult) return leftResult;  
                        return levelTraverse(root->right, level - 1, key);  
                    }
                    return NULL;
                }

                Node* parentNode(Node* root, std::string key){
                    if (!root) return NULL;
                    if (!root->left && !root->right) return NULL;
                    else if ((root->left && root->left->getKey() == key) || (root->right && root->right->getKey() == key)) return root;
                    else if (!root->left && root->right) return parentNode(root->right, key);
                    else if (root->left && !root->right) return parentNode(root->left, key) ;
                    else {
                        int comp = strcmp(root->getKey(), key);
                        if (comp == G) return parentNode(root->left, key);
                        else return parentNode(root->right, key);
                    }
                }

                Node* searchNode(std::string key) {
                    int h = getHeight();
                    for (int i = 1; i <= h; i++) {
                        Node* temp = levelTraverse(root, i, key);
                        if (temp) {
                            return temp;
                        }  // Return immediately if node is found
                    }
                    return NULL;  // Return NULL if node is not found
                }

                void levelTraverse(Node*root, int i);

                void printNode(Node*root);
                void printTree(Node *node, std::string prefix, bool isLeft);
    };
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);

    void runAsCLI();
};

#endif // !__STACK_FRAME_H__
