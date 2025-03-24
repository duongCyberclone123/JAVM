#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE) {};

void StackFrame::ArrayAVL::levelTraverse(Node*root, int i){
    if (!root) {
        return;
    }
    else if (i==1){
        cout<<"["<<root->key<<": "<<root->getVal()<<", "<<root->getState()<<"]\t";
    }
    else{
        levelTraverse(root->left,i-1);
        levelTraverse(root->right,i-1);
    }
}

void StackFrame::ArrayAVL::printNode(Node* root){
    if (!root) {
        cout<<"Empty Tree!\n";
        return;
    }
    int h = getHeightRec(root);
    cout<<h<<"\n";
    for (int i=1;i<=h;i++){
        levelTraverse(root,i);
        cout<<"\n";
    }
}

bool isInt(int a, int b){
    return (a==0&&b==0);
}
bool isFloat(int a,int b){
    return (a==1 && b==1);
}
bool isInt(int a){
    return !a;
}

void StackFrame::ArrayAVL::printTree(Node *node, std::string prefix, bool isLeft) {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "|---" : "'---");

        // Print the value of the node
        cout << node->key;
        if (node->state == 0) {
            cout << " " << node->val << " (int)" << endl;
        } else if (node->state == 1) {
            cout << " " << node->val << " (float)" << endl;
        }

        // Enter the next tree level - left and right branch
        printTree(node->left, prefix + (isLeft ? "|\t" : "\t"), true);
        printTree(node->right, prefix + (isLeft ? "|\t" : "\t"), false);
    }
}


void splitString(string st, string &com, double &num, string& key){
    for (int i=st.size()-1;i>=0;i--){
        if (st[i]==' ') st.pop_back();
        else break;
    }
    size_t sp = st.find(' ');
    if (sp != string::npos){
        com=st.substr(0,sp);
        if (com != "istore" && com != "val" && com != "fload" && com != "iload" && com!="fstore" && com!= "par") num = stod(st.substr(sp+1,st.size()));
        else key = st.substr(sp+1,st.size());
    }
    else {com=st;}
}    

void StackFrame::printStack(stack<Stack*> stackFrame){
    stack<Stack*> temp;
    while (!stackFrame.empty()){
        temp.push(stackFrame.top());
        stackFrame.pop();
    }
    cout<<"Stack: [";
    while (!temp.empty()){
        cout<<"("<<temp.top()->getVal()<<", "<<temp.top()->getState()<<"); ";
        temp.pop();
    }
    cout<<"]\n";
}

void StackFrame::runAsCLI(){
    vector<stack<Stack*>> lstPage;
    vector<ArrayAVL*> lstArray;
    stack<Stack*> initstackFrame;
    ArrayAVL* initArrayTree = new ArrayAVL();
    lstPage.push_back(initstackFrame);
    lstArray.push_back(initArrayTree);
    int page = 0;
    string cmd = "init";
    int size = 0;
    int num_line=0;

    while (cmd != "exit"){
        stack<Stack*> stackFrame = lstPage[page];
        ArrayAVL* ArrayTree = lstArray[page];
        StackFrame::printStack(stackFrame);
        ArrayTree->printTree(ArrayTree->getRoot(),"",false);
        cout<<"Number of Pages: "<<lstArray.size()<<"\n";
        cout<< "At page: "<<page<<"\n";
        cout<<"<< ";
        getline(cin,cmd);
        double num=0.0;
        string com="";
        string key="";
        splitString(cmd,com,num,key);
        //cout<<"Command line no."<<num_line<<" with command \""<<com<<"\""<<"\n";
        if (com=="iconst"){
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(int(num),0));
        }
        else if (com=="fconst"){
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(num,1));
        }
        else if (com=="iadd"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal())+int(b->getVal()),0));
        }
        else if (com=="fadd"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(a->getVal()+b->getVal(),1));
        }
        else if (com=="isub"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(b->getVal()) - int(a->getVal()),0));
        }
        else if (com=="fsub"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(b->getVal() - a->getVal(),1));
        }
        else if(com=="idiv"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line); 
            stackFrame.push(new Stack(int(int(b->getVal()) / int(a->getVal())),0));
        }
        else if(com=="fdiv"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line); 
            stackFrame.push(new Stack(b->getVal() / a->getVal(),1));
        }
        else if (com=="irem"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line);
            stackFrame.push(new Stack(int(b->getVal()) - int(int(b->getVal()) / int(a->getVal())*int(a->getVal())),0));
        }
        else if (com=="ineg"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if(!isInt(a->getState())) throw TypeMisMatch(num_line);
            if (!a->getState()){
                stackFrame.push(new Stack(-int(a->getVal()),0));
            }
        }
        else if (com=="fneg"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(-double(a->getVal()),1));
        }
        else if (com=="iand"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) & int(b->getVal()),0));
        }
        else if (com=="ior"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) | int(b->getVal()),0));
        }
        else if (com=="ieq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) == int(b->getVal()),0));
        }
        else if (com=="feq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) == (b->getVal()),0));
        }
        else if (com=="ineq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) != int(b->getVal()),0));
        }
        else if (com=="fneq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) != (b->getVal()),0));
        }
        else if (com=="ilt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) > int(b->getVal()),0));
        }
        else if (com=="flt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) > (b->getVal()),0));
        }
        else if (com=="igt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) < int(b->getVal()),0));
        }
        else if (com=="fgt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) < (b->getVal()),0));
        }
        else if (com=="ibnot"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()){
                int b = a->getVal(); 
                stackFrame.push(new Stack(b==0,0));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="imul"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal())*int(b->getVal()),0));
        }
        else if (com=="fmul"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(a->getVal()*b->getVal(),1));
        }
        else if (com == "istore"){
            //cout<<ArrayTree->getNum()<<" \n";
            if (ArrayTree->getNum()*2 >= localVarSpaceSize) throw LocalSpaceFull(num_line);
            if (stackFrame.empty()) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (a->getState()) throw TypeMisMatch(num_line);
            ArrayTree->insert(a->getVal(),0,key);
        }
        else if (com == "fstore"){
            if (ArrayTree->getNum()*2 >= localVarSpaceSize) throw LocalSpaceFull(num_line);
            if (stackFrame.empty()) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()) throw TypeMisMatch(num_line);
            ArrayTree->insert(a->getVal(),1,key);
        }
        else if (com == "iload"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            StackFrame::ArrayAVL::Node* temp = ArrayTree->searchNode(key);
            if (temp->getState()) throw TypeMisMatch(num_line);
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(temp->getVal(),temp->getState()));
        }
        else if (com == "fload"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            StackFrame::ArrayAVL::Node* temp = ArrayTree->searchNode(key);
            if (!temp->getState()) throw TypeMisMatch(num_line);
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(temp->getVal(),temp->getState()));
        }
        else if (com=="i2f"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()){
                int b = a->getVal(); 
                stackFrame.push(new Stack(double(b),1));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="f2i"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (a->getState()){
                double b = a->getVal(); 
                stackFrame.push(new Stack(int(b),0));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="val"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            cout<<ArrayTree->searchNode(key)->getVal()<<"\n";
        }
        else if (com=="top"){
            if (size<=0) throw StackEmpty(num_line);
            cout<<stackFrame.top()->getVal()<<"\n";
        }
        else if (com == "par"){
            if (!ArrayTree->parentNode(ArrayTree->getRoot(), key)) cout<<"null\n";
            else cout<<ArrayTree->parentNode(ArrayTree->getRoot(), key)->getKey()<<"\n";
        }
        size = stackFrame.size()*2;
        lstPage[page] = stackFrame;
        lstArray[page] = ArrayTree;
        if (com == "exit"){
            return;
        }
        else if (com == "reset"){
            while (!stackFrame.empty()){
                stackFrame.pop();
            }
            ArrayTree->setRoot(nullptr);
        }
        else if (com == "new"){
            
            stack<Stack*> newStackFrame;
            ArrayAVL* newArrayTree = new ArrayAVL();
            lstPage.push_back(newStackFrame);
            lstArray.push_back(newArrayTree);
            page = lstPage.size() - 1;
        }
        else if (com == "switch"){
            if (num >= lstPage.size()) cout<<"Page is not exist!\n";
            else {
                page = num;
            }
        }
        else if (com == "back"){
            if (page == 0) page = 0;
            else {
                page--;
            }
        }
        else if (com == "rm"){
            if (num >= lstPage.size()) cout<<"Page is not exist!\n";
            else {
                lstPage.erase(lstPage.begin()+num);
                lstArray.erase(lstArray.begin()+num);
                page = page > 0 ? page-1 : 0;
            }
        }
        else if (com == "clear"){
            lstArray.clear();
            lstPage.clear();
            page = 0;
        }
        else if (com == "clscr"){
            system("cls");
        }
        else if (com == "print"){
            printStack(stackFrame);
        }
        else if (com == "printTree"){
            ArrayTree->printNode(ArrayTree->getRoot());
        }
        else {
            cout<<"Invalid command\n";
        }
    }
    //ArrayTree->printTree(ArrayTree->getRoot(),"",false);
}

void StackFrame::run(string filename) {
    stack<Stack*> stackFrame;
    ArrayAVL* ArrayTree = new ArrayAVL();
    
    fstream file(filename,ios::in);
    string line;
    int num_line=0;
    int size=0;
    if (!file.is_open()){
        cout<<"Not open!";
        return;
    }
    
    while (!file.eof()){
        getline(file,line);
        double num=0.0;
        string com="";
        string key="";
        splitString(line,com,num,key);
        num_line++;
        //cout<<"Command line no."<<num_line<<" with command \""<<com<<"\""<<"\n";
        if (com=="iconst"){
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(int(num),0));
        }
        else if (com=="fconst"){
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(num,1));
        }
        else if (com=="iadd"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal())+int(b->getVal()),0));
        }
        else if (com=="fadd"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(a->getVal()+b->getVal(),1));
        }
        else if (com=="isub"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(b->getVal()) - int(a->getVal()),0));
        }
        else if (com=="fsub"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(b->getVal() - a->getVal(),1));
        }
        else if(com=="idiv"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line); 
            stackFrame.push(new Stack(int(int(b->getVal()) / int(a->getVal())),0));
        }
        else if(com=="fdiv"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line); 
            stackFrame.push(new Stack(b->getVal() / a->getVal(),1));
        }
        else if (com=="irem"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            if (a->getVal() == 0.0) throw DivideByZero(num_line);
            stackFrame.push(new Stack(int(b->getVal()) - int(int(b->getVal()) / int(a->getVal())*int(a->getVal())),0));
        }
        else if (com=="ineg"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if(!isInt(a->getState())) throw TypeMisMatch(num_line);
            if (!a->getState()){
                stackFrame.push(new Stack(-int(a->getVal()),0));
            }
        }
        else if (com=="fneg"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(-double(a->getVal()),1));
        }
        else if (com=="iand"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) & int(b->getVal()),0));
        }
        else if (com=="ior"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) | int(b->getVal()),0));
        }
        else if (com=="ieq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) == int(b->getVal()),0));
        }
        else if (com=="feq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) == (b->getVal()),0));
        }
        else if (com=="ineq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) != int(b->getVal()),0));
        }
        else if (com=="fneq"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) != (b->getVal()),0));
        }
        else if (com=="ilt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) > int(b->getVal()),0));
        }
        else if (com=="flt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) > (b->getVal()),0));
        }
        else if (com=="igt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal()) < int(b->getVal()),0));
        }
        else if (com=="fgt"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            //if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack((a->getVal()) < (b->getVal()),0));
        }
        else if (com=="ibnot"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()){
                int b = a->getVal(); 
                stackFrame.push(new Stack(b==0,0));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="imul"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            if (!(isInt(a->getState(), b->getState()))) throw TypeMisMatch(num_line);
            stackFrame.push(new Stack(int(a->getVal())*int(b->getVal()),0));
        }
        else if (com=="fmul"){
            if (size-1<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            Stack* b = stackFrame.top();
            stackFrame.pop();
            stackFrame.push(new Stack(a->getVal()*b->getVal(),1));
        }
        else if (com == "istore"){
            //cout<<ArrayTree->getNum()<<" \n";
            if (ArrayTree->getNum()*2 >= localVarSpaceSize) throw LocalSpaceFull(num_line);
            if (stackFrame.empty()) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (a->getState()) throw TypeMisMatch(num_line);
            ArrayTree->insert(a->getVal(),0,key);
        }
        else if (com == "fstore"){
            if (ArrayTree->getNum()*2 >= localVarSpaceSize) throw LocalSpaceFull(num_line);
            if (stackFrame.empty()) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()) throw TypeMisMatch(num_line);
            ArrayTree->insert(a->getVal(),1,key);
        }
        else if (com == "iload"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            StackFrame::ArrayAVL::Node* temp = ArrayTree->searchNode(key);
            if (temp->getState()) throw TypeMisMatch(num_line);
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(temp->getVal(),temp->getState()));
        }
        else if (com == "fload"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            StackFrame::ArrayAVL::Node* temp = ArrayTree->searchNode(key);
            if (!temp->getState()) throw TypeMisMatch(num_line);
            if (size >= opStackMaxSize) throw StackFull(num_line);
            stackFrame.push(new Stack(temp->getVal(),temp->getState()));
        }
        else if (com=="i2f"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (!a->getState()){
                int b = a->getVal(); 
                stackFrame.push(new Stack(double(b),1));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="f2i"){
            if (size<=0) throw StackEmpty(num_line);
            Stack* a = stackFrame.top();
            stackFrame.pop();
            if (a->getState()){
                double b = a->getVal(); 
                stackFrame.push(new Stack(int(b),0));
            }
            else throw TypeMisMatch(num_line);
        }
        else if (com=="val"){
            if (ArrayTree->getNum()==0 || !ArrayTree->searchNode(key)){
                throw UndefinedVariable(num_line);
            }
            cout<<ArrayTree->searchNode(key)->getVal()<<"\n";
        }
        else if (com=="top"){
            if (size<=0) throw StackEmpty(num_line);
            cout<<stackFrame.top()->getVal()<<"\n";
        }
        else if (com == "par"){
            if (!ArrayTree->parentNode(ArrayTree->getRoot(), key)) cout<<"null\n";
            else cout<<ArrayTree->parentNode(ArrayTree->getRoot(), key)->getKey()<<"\n";
        }
        size = stackFrame.size()*2;
    }
    //ArrayTree->printTree(ArrayTree->getRoot(),"",false);
    file.close();
}
