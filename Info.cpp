#include "Info.hpp"

#include<cmath>
#define INF 1000000;


double Info::Cp = 0.3;
Info::Info()
{
    X=INF;
    N=0;

}

void Info::updateX_min(const double x)
{
    X=((X<x)? X:x);

}

void Info::updateX_av(const double x){

    X = N*X+ x;
    N++;
    X /= N;
}

double Info::UCT(const int n,const double Xmax, const double Xmin)const {

    return (X - Xmin) / (Xmax - Xmin) - 2 * Cp * std::sqrt(log(n) / N);
}
