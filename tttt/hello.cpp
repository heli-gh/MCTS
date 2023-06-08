#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <iostream>
using namespace std;
int main(){

    int a[2];
    a[0]=1;a[1]=6;
    swap(a[0],a[1]);
    srand((int)time(0));
    cout<<a[0]<<a[1]<< rand()%3<<rand()%3<<rand()%6;
    return 0;

}
