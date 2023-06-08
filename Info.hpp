#ifndef INFO_H
#define INFO_H

class Info{
    //the minimum value among the previous tirals
    double X;
    //the weight
    static double Cp;
    //the number of trials 
    int N;

    public :
    //constructor
    Info();

    //update the minimum 
    void updateX_min(const double x);

    //update the averge value
    void updateX_av(const double x);

    

    //Upper Confidential Bound for Trees
    double UCT(const int n,const double Xmax=0, const double Xmin=0)const ;

    template<class T>
    friend class Node;

};
#endif