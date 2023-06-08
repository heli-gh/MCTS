#include <iostream>
#include <sstream>
#include <fstream>
#include "string.h"
#include <math.h>
using namespace std;

#define Inf_Cost           1000000000

class DataSource{
    private:
        int **Distance;
        int nb_city;
        int Start_City;
        double *Coordinate_X; 
	    double *Coordinate_Y;

	  

    public:
        DataSource();
        int getNbCity() const;
        int** getDistance() const;
        bool Read_Instance_Info(string Input_Inst_Name);
        void Calculate_All_Pair_Distance();
        int Calculate_Int_Distance(int First_City,int Second_City);
        double Calculate_Double_Distance(int First_City,int Second_City);
};