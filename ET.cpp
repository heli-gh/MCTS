#include "ET.hpp"
#include <ctime>
#include <cstdlib>
#define min(a,b) a>b?b:a
#define random(a,b) (rand()%(b-a)+a)
extern  int MAX;
extern int** d;
extern float* P;


namespace ET
{
    //evaluation for the 
    double ETtsp(const int* sequence, const int M=MAX){

        double cout =0;
        for (int i=0;i<M;i++){
            cout +=d[sequence[i]][sequence[i+1]];
        }
        return cout;
    }


    double ETptsp1(const int* sequence ){
        double cout =0;

        for(int i=0; i<MAX;i++){
            for(int j=i+1;j<=MAX;j++)
            {
                float t=d[sequence[i]][sequence[j]]*P[sequence[i]]*P[sequence[j]];
                float f=0;
                for(int k = i+1;k<=j-1;k++){
                    f*=(1-P[sequence[k]]);
                }
                cout+=t*f;
            }
        }

        return cout;

    }


    double ETptsp2(const int* sequence ){
        double cout =0;
        int lambda=20;
        for(int i=0; i<MAX;i++){
            int l= min(MAX,i+lambda);
            for(int j=i+1;j<=l;j++)
            {
                float t=d[sequence[i]][sequence[j]]*P[sequence[i]]*P[sequence[j]];
                float f=1;
                for(int k = i+1;k<=j-1;k++){
                    f*=(1-P[sequence[k]]);
                }
                cout+=t*f;
            }
        }

        return cout;

    }


    double ETptsp3(const int* sequence){
        double cout=0;
        int it =100;
        int n=0;
        while(n++<it){
            int om[MAX];
            int c=0;
            for(int i=0;i< MAX;i++){
                float r = random(0,1000)/999;
                if(r<P[i]) {om[i]=1;c++;}
                else{om[i]=0;}
            }
            int* pc= new int[c+1];
            
            int l=0;
            for (int i=0;i<=MAX;i++){
                if(om[sequence[i]]) pc[l++]=sequence[i];
            }


            cout +=ETtsp(pc,c);

            delete[] pc;


        }

        return cout/it;
        
    }

    
    double ETptsp4(const int* x1,int* CitySequence,int Pair_nb_city ){
        double diff=0;
        
        int it =100;
        int n=0;
        while(n++<it){

            int om[MAX];
            int c=0;
            for(int i=0;i< MAX;i++){
                float r = random(0,1000)/999;
                if(r<P[i]) {om[i]=1;c++;}
                else{om[i]=0;}
            }
            for(int i=0;i<Pair_nb_city;i++)
            {
                int a=0;int b=0;int c=0;int t=0;
                if(om[CitySequence[2*i]]) a=CitySequence[2*i];
                else {
                    int j=0;
                    for(;j<MAX;j++)
                    {
                        if(x1[j]==CitySequence[2*i])break;
                    }
                    while( om[x1[j]]==0 && j!=0){
                        j--;

                    }
                    a = x1[j];
                }

                if(om[CitySequence[2*i+1]]){b=CitySequence[2*i+1];c=CitySequence[2*i+1];}
                else{
                    int j=0;
                    for(;j<MAX;j++)
                    {
                        if(x1[j]==CitySequence[2*i+1])break;
                    }
                    int l=j;
                    while( om[x1[l]]==0 && j!=MAX){
                        l++;

                    }
                    b = x1[l];
                    l= j;
                    while(om[x1[l]] == 0 && j!=0){
                        l--;
                    }
                    c= x1[l];


                }
                if(om[CitySequence[2*i+2]])t=CitySequence[2*i+2];
                else{int j=0;
                    for(;j<MAX;j++)
                    {
                        if(x1[j]==CitySequence[2*i+2])break;
                    }
                    while( om[x1[j]]==0 && j!=MAX){
                        j++;

                    }
                    t = x1[j];

                }

                diff+=(d[c][t]-d[a][b]);
            }
            
        }

        return diff/it;

    }
}
