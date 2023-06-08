#ifndef NODE_H
#define NODE_H


using namespace std;
template <class  T>
class Node
{
private:
    //parent node of the tree
    Node<T>* parent;
    //the information of the node 
    T content;

    //the city number

    int number;

    // the location information of the node 
    int level;

    // the extend status whether or not fully developped

    bool extend;
    //the  path infomation of the node
    int*  tableux;



    Node<T> ** childs;

    // the number of childs
    int nc;

    //the maximum of the childs
    double Xmax;

public:
    //constructor for racine
    Node();


    //constructor for child
    Node(Node<T>* parent,int n);





    //destructor
    ~Node();

    //delete nodes
    void deleteNode();

    //comparison
    bool operator >(const Node<T> & other)const;

    

    //heap table to ajuste from bottom to top
    void upAjust(int low , int high);
    //heap table to ajuste frome top to bottom
    void downAjust(int low , int high);

    //add a child

    void insert();


    //create a child.
    Node<T>* create_child();


    //create a number of childs(version2)
    void create_childs();

    //distribution
    void distribution(int* permit,const int debut ,const int fin);
    void distribution_al(int* permit,const int debut ,const int fin);
    
    //rollout simulation with probability
    void rollout();

    //to 
    Node<T>* back_up();

    //friend
    template <class U>
    friend class MCTS;


};
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include "ET.hpp"

#define INF 1000000;

#define random(a,b) (rand()%(b-a)+a)

extern int MAX;
extern int ctt;


//
extern int MINIMUM;

//the table of distance 
extern int** d;
extern float* P;
extern int* parcours;


template<class T>
Node<T>::Node():content(),level(1),extend(false),number(0),nc(0),Xmax(0){
    //parent
    parent =0;    
    //table
    tableux = new int[level];
    tableux[0]=0;
    //childs
    childs = new Node<T>*[MAX-level];
    for(int i=0;i<MAX-level;i++){
        childs[i]=0;
    }
    ctt--;
}

template <class T>
Node<T>::Node(Node<T>* p,int n):content(),extend(false),number(n),nc(0),Xmax(0)
{
    //parent
    parent = p;

    p->childs[p->nc] = this;

    //level
    level=p->level +1;
    //table
    tableux = new int[level];
    for(int i =0 ; i< level-1;i++) tableux[i]= p->tableux[i];
    tableux[level-1] = n;
    //childs
    if (level!=MAX) 
    {childs=new Node<T>*[MAX-level];

        for(int i=0;i<MAX-level;i++){
            childs[i]=0;
        }
    }
    else{childs =0;}
    ctt--;
}

template<class T>
Node<T>::~Node(){
    
    
    if (childs) {delete[] childs;childs=0;}
    if (tableux){delete[] tableux; tableux=0;}
    
    
    
}

template<class T>
void Node<T>::deleteNode(){
    if(this){
        for (int i=0;i<nc;i++){
            if (childs[i]) childs[i]->deleteNode();
            if(parent) parent->childs[i] =0;
        }
    }

    delete this;

}




template<class T>
void Node<T>::upAjust(int low, int high){
    int i=high,j=i/2-0.5;
    while(j>=low){
        if(*childs[j] > *childs[i]){
            swap(childs[j],childs[i]);
            i=j;
            j=i/2-0.5;
        }else{break;}
    }

}
template <class T>
void Node<T>::downAjust(int low , int high){
    int i=low,j=2*i+1;
    while(j<=high){
        if(j+1<=high && !(*childs[j+1] > *childs[j])) j+=1;
        if(!(*childs[j] > *childs[i])){
            swap(childs[j],childs[i]);
            i=j;
            j=2*i+1;
        }else{break;}
    }

}


template<class T>
void Node<T>::insert(){
    if (extend) downAjust(0,nc-1);
    else upAjust(0,nc-1);
}

template<class T>
Node<T>* Node<T>::create_child(){
 if(!extend){
    int* tabou =new int[MAX];
    int i;

    for(i =0;i<MAX;i++)tabou[i]=0;    
    for(i =0;i<level;i++)tabou[tableux[i]]=1;    
    
    for(i=0;i<nc;i++)tabou[childs[i]->number]=1;
    int* v = new int[MAX -nc - level];
    int n=0;
    for(i=0;i<MAX;i++){
        if(tabou[i] !=1 ) {v[n]=i;n++;}
        
    }
    int dd=random(0,MAX-nc-level);
    int a=v[dd];
    Node<T>* node= new Node<T>(this , a);
    delete[] tabou;
    delete[] v;
    nc++;
    if(nc+level == MAX) extend=true;
    return node;
 }
 else{
    cout <<"error!";exit(0);return NULL;
 }
}

template <class T>
bool Node<T>::operator > (const  Node<T> & other)const{
    int n= parent->content.N ;
    return content.UCT(n,parent->Xmax,parent->content.X)>other.content.UCT(n,parent->Xmax,parent->content.X);
}

template<class T>
void Node<T>::distribution(int* permit,const  int debut,const int fin){
    int dis[ fin -debut+1];
    int c=0;
    for(int i=debut; i<= fin ;i++){
        dis[c] = d[permit[debut-1]][permit[i]];
        c++;
    }
   

    float proba[fin-debut+1];
    double n=0;
    for (int i =0;i<=fin-debut ;i++ ){
        proba[i]= 1.0/exp(dis[i]/100.0);
        n+=proba[i];
    }
    double t= 0;
    for(int i=0;i<=fin-debut;i++){
        proba[i] /=n;
        t+=proba[i];
        proba[i]=t;

    }
    float k=random(0,10000)/9999.0;

    for( int i=0;i<=fin-debut;i++){
        if (proba[i] >= k) {
            swap(permit[i+debut],permit[debut]);
            break;
        }
        
    }

    

}
template<class T>
void Node<T>::distribution_al(int* permit,const  int debut,const int fin){
    
    int k=random(debut,fin+1);
    swap(permit[k],permit[debut]);
      

}

template <class T>
void Node<T>::rollout(){
    //3 times rollout get the best  
    int times=0;
    double ct =INF;
    while (times++<3){
    
        int tabou[MAX];
        for(int i=0;i<MAX;i++) tabou[i]=0;
        for(int i=0;i<level;i++) tabou[tableux[i]]=1;
    
        

        int permit[MAX-level+1];
        permit[0] = number;
        int n =1;
        for(int i =0;i < MAX;i++){
            if(tabou[i]!=1) {
                permit[n]=i;
                n++;
            }
        }
    
        for(int i=1 ; i<MAX-level;i++){
            distribution(permit,i,MAX-level);
        }
        int cc[MAX+1];
        cc[MAX]=tableux[0];
        for(int i=0;i<level;i++){
            cc[i]=tableux[i];
        }
        for(int i = 1;i<MAX-level+1;i++){cc[i+level-1] =permit[i];}
        
        double ref = ET::ETptsp2(cc);
    
        if(ref<ct) ct=ref;

        if(ct < MINIMUM) {
            MINIMUM=ct;
            for(int i=0;i<MAX;i++){
                parcours[i] = cc[i];
            }
        }

    }
    if(ct>Xmax) Xmax = ct;
    content.updateX_min(ct);
    content.N++;

    
    
}

template <class T>
Node<T>* Node<T>::back_up(){
    if(parent){
    if(parent->content.X > content.X) parent->content.X=content.X  ;
    (parent->content.N) ++;
    if(parent->Xmax < Xmax)  parent->Xmax =Xmax;
    }
    
    return parent;
    
}





#endif