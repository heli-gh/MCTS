#include "MCTS.hpp"
#include "DataSource.hpp"
#include "Info.hpp"
#include <iostream>
#include "Node.hpp"
#include <ctime>
extern  int MAX;
extern int** d;
extern int MINIMUM;
extern int ctt;
extern int* parcours;
extern float* P;


int main(){
    
    srand((int)time(0));
    int a =(int)time(0);

    string Name = "Instances/a280.tsp";
    DataSource* Test = new DataSource();
    Test->Read_Instance_Info(Name);
    Test->Calculate_All_Pair_Distance();

    MAX = Test->getNbCity();
    d = Test->getDistance();
    MINIMUM = 10000000;
    ctt = 0;
    parcours = new int[MAX];

    P = new float[MAX];
    P[0]=1.0;
    for(int i=1;i<MAX;i++){
        P[i] =0.8;
    }
    
    
    int it=10000;
    MCTS<Info>* test= new MCTS<Info>(it);

    test->MCTS_SEARCH();
    std::cout<< MINIMUM<<endl;
    for(int i=0;i< MAX;i++) std::cout << parcours[i]<<' ';
    std::cout<<endl;
    int b=(int) time(0);
    std::cout<<b-a<<endl;
    delete[] P;
    delete[] parcours;
    delete test;
    return MINIMUM;

}

