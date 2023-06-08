#ifndef ET_H
#define ET_H
// for evaluation the cost of  travel in the probabilistic travel saleman problem 
namespace ET
{
    //evaluation for the 
    double ETtsp(const int* sequence,const int M );


    double ETptsp1(const int* sequence );


    double ETptsp2(const int* sequence );


    double ETptsp3(const int* sequence );
    double ETptsp4(const int* x1,int* CitySequence,int Pair_nb_city );

}



#endif