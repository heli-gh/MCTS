#include "DataSource.hpp"

DataSource::DataSource(){

}

bool DataSource::Read_Instance_Info(string Input_Inst_Name)
{
  	ifstream FIC;   
  	FIC.open(Input_Inst_Name);   
   
 	if (FIC.fail())
  	{     
    	cout<<"Read_Instance_Info() fail! Fail to open the input file "<<Input_Inst_Name<<endl;    
    	return false;     
  	}  

  	FIC >> nb_city;

  	FIC >> Start_City;
  	Start_City--; 
  	  
  	Distance = new int *[nb_city];
	for(int i=0;i<nb_city;i++)
		Distance[i]=new int [nb_city];  
  	    
  	int Cur_City;
  	double Cur_X;
  	double Cur_Y;

	Coordinate_X = new double[nb_city];
	Coordinate_Y = new double[nb_city];
  
  	for(int i=0;i<nb_city;i++)
  	{
    	FIC >> Cur_City;  
    	FIC >> Cur_X;  
    	FIC >> Cur_Y; 
    
		Coordinate_X[Cur_City-1]=Cur_X;  
		Coordinate_Y[Cur_City-1]=Cur_Y;   
 	}
	    
  	FIC.close();   
	   
  	return true;    
}

//Calculate the distance between two cities, rounded up to the nearest integer 
int DataSource::Calculate_Int_Distance(int First_City,int Second_City)
{
  	return (int)(0.5 + sqrt( (Coordinate_X[First_City]-Coordinate_X[Second_City])*(Coordinate_X[First_City]-Coordinate_X[Second_City]) +
                (Coordinate_Y[First_City]-Coordinate_Y[Second_City])*(Coordinate_Y[First_City]-Coordinate_Y[Second_City]) ) );
}

//Calculate the distance between two cities
double DataSource::Calculate_Double_Distance(int First_City,int Second_City)
{
  	return sqrt( (Coordinate_X[First_City]-Coordinate_X[Second_City])*(Coordinate_X[First_City]-Coordinate_X[Second_City]) +
                   (Coordinate_Y[First_City]-Coordinate_Y[Second_City])*(Coordinate_Y[First_City]-Coordinate_Y[Second_City]) );
}

//Calculate the distance (integer) between any two cities, stored in Distance[][]
void DataSource::Calculate_All_Pair_Distance()
{
  	for(int i=0;i<nb_city;i++)
  		for(int j=0;j<nb_city;j++)
  		{
	  		if(i!=j)
	    		Distance[i][j]=Calculate_Int_Distance(i,j);  
	  		else
	    		Distance[i][j]=Inf_Cost;  	  
		}  	 
}

int DataSource::getNbCity() const{
	return nb_city;
}

int** DataSource::getDistance() const{
	return Distance;
}

