
#ifndef MCTS_H
#define MCTS_H
#include "Node.hpp"
extern  int MAX;

using namespace std;
template <class T>
class MCTS{
    
    private:
    // the racine of the monte carlo tree
    Node<T>* racine;
    // the maxium number of iteration
    int iteration ;


    Node<T>* selection();
    Node<T>* expansion(Node<T>* bestchild);
    void simulation(Node<T>* child);
    void update(Node<T>* child);

    public:

    MCTS(int it);
    ~MCTS();


    void MCTS_SEARCH();


};


template<class T>
MCTS<T>::MCTS(int it){
    racine = new Node<T>;
    iteration = it;


}
template<class T>
MCTS<T>::~MCTS(){
    racine->deleteNode();
}

template<class T>
Node<T>* MCTS<T>::selection(){

    Node<T>* t= racine;
    while(t->extend && t->level!=MAX) {
        t= t->childs[0];
    }
    return t;



}

template<class T>
Node<T>* MCTS<T>::expansion(Node<T>* bestchild){

    if(bestchild->level ==MAX) return bestchild;
    else{
        return bestchild->create_child();
    }

}

template<class T>
void MCTS<T>::simulation(Node<T>* child){
    child->rollout();

}

template<class T>
void MCTS<T>::update(Node<T>* child)
{
    Node<T>* t=child;
    while(t != racine) {t=t->back_up();t->insert();}

}

template<class T>
void MCTS<T>::MCTS_SEARCH(){
    int n=0;
    Node<T>* t;
    while(n<iteration){
        t=selection();
        t=expansion(t);
        simulation(t);
        update(t);
        n++;
    }


}

#endif



