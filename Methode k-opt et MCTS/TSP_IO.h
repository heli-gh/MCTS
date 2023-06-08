#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <ctime>
#include <vector>
#include <math.h>
using namespace std;


#define Null               -1 
#define Inf_Cost           1000000000
#define Max_Candidate_Num  10
#define Max_Depth  		   10

typedef int    Distance_Type; //définir une type pour distance entre 2 ville(pas vraiement utile)

//Hyper paramètres 
double Alpha=1;       //utiliser pour estimer le potentiel pour chaque arête
double Beta=10;       //utiliser dans le  rétropropagation
double Param_H=10;   //utiliser pour controler le nombre d'actions tester
double Param_T=1;  	 //utiliser pour controler le condition de termination


//#définir Default_Random_Seed  (unsigned)time(NULL);
#define Default_Random_Seed  489663920;    
unsigned Random_Seed=Default_Random_Seed;

//Utiliser pour enregistrer le input information pour un instance donnée
int Start_City;      //Ville de départ
int nb_city;       //Nombre de ville
double *Coordinate_X;  // Coordonnée x de ville(pour instance)
double *Coordinate_Y;  //Coordonnée y de ville(pour instance)
Distance_Type **Distance; //Tableau pour enregistrer les distance entre villes

double Current_Instance_Begin_Time; //Distance de chemin au début
Distance_Type Current_Instance_Best_Distance;  //Le plus court chemin pendant le simulation(change)

//Utiliser pour enregistrer une ville lien
struct Struct_Node
{
  int Pre_City;
  int Next_City;
};

Struct_Node *All_Node;        //Enregistrer un tour
Struct_Node *Best_All_Node;   //Enregistrer le meilleur tour

//Utiliser pour enregistrer un solution dans un tableau(permutation des villes)
int *Solution;  

//Utiliser pour stocker les candidats pour chaque ville(pour choisir ai+1 dans k-opt)
int Candi_nb_city; //Nombre de villes peut être connecté par bi (nombre de ai+1 peut être choisit)
int *Candi_city;  //Ensemble de villes peut être connecté par bi (villes peut être ai+1)
bool *If_City_Selected; //Pour quelque fonction

//Utiliser pour stocker un action
int Pair_nb_city;
int *City_Sequence;
Distance_Type *Gain;
Distance_Type *Real_Gain;

//Utiliser dans le MCTS
double **Weight; //Poids de chaque arête
double Avg_Weight;
int    **Chosen_Times; //Nombre de fois choisit pour chaque ville
int    *Probabilistic;
int    Total_Simulation_Times;



void Allocate_Memory(int nb_city)
{   
	Coordinate_X = new double [nb_city];  
	Coordinate_Y = new double [nb_city]; 

	Distance = new Distance_Type *[nb_city];
	for(int i=0;i<nb_city;i++)
		Distance[i]=new Distance_Type [nb_city];  

	All_Node = new Struct_Node [nb_city]; 
	Best_All_Node = new Struct_Node [nb_city]; 
	Solution = new int [nb_city];  
		
	Candi_city=new int [nb_city];
	If_City_Selected=new bool [nb_city];
	 
	City_Sequence=new int [3*nb_city];
	Gain=new Distance_Type [3*nb_city];
	Real_Gain=new Distance_Type [3*nb_city];
		
	Weight=new double *[nb_city];
	for(int i=0;i<nb_city;i++)
		Weight[i]=new double [nb_city];  
	
	Chosen_Times=new int *[nb_city];
	for(int i=0;i<nb_city;i++)
		Chosen_Times[i]=new int [nb_city];

	Probabilistic=new int [nb_city];	
}

void Release_Memory(int nb_city)
{
  	delete []Coordinate_X;  
	delete []Coordinate_Y;

	for(int i=0;i<nb_city;i++)
		delete []Distance[i];  
	delete []Distance;
    	   
	delete []All_Node;   
	delete []Best_All_Node;	
	delete []Solution;  
			
	delete []Candi_city;
	delete []If_City_Selected;	

	delete []City_Sequence;
	delete []Gain;
	delete []Real_Gain;

	for(int i=0;i<nb_city;i++)
		delete []Weight[i];
	delete []Weight;
	
	for(int i=0;i<nb_city;i++)
		delete []Chosen_Times[i];
	delete []Chosen_Times;
		
	delete []Probabilistic;	
}

