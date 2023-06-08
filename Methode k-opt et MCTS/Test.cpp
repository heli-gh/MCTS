#include "Markov.h"
#include "string.h"

//Créer les distances entre villes par hasard
void Give_Distance(){
    for(int i=0;i<nb_city;i++){
        for(int j=0;j<nb_city;j++){
            if(i==j){
                Distance[i][j] = Inf_Cost;//Distance i,i = infinie
            }
            else{
                if(i<j){
                    Distance[i][j] = rand()%10;
                    Distance[i][j]+=1; //entier aléatoirement entre 1 et 10
                }
                else{
                    Distance[i][j] = Distance[j][i];
                }
            }
        }
    }
}

//Afficher les distances entre villes
void Afficher_Distance(){
    for(int i=0;i<nb_city;i++){
        for(int j=0;j<nb_city;j++){
           cout<<"Dis: " << Distance[i][j]<<"";
        }
        cout<<"\n";
    }   
}

//Obtenir les données de TSPLIB instances
bool Read_Instance_Info(string Input_Inst_Name)
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
  	Start_City--; //correspent le ville avec l'indice de tableau
  	  
  	Allocate_Memory(nb_city);
  	    
  	int Cur_City;
  	double Cur_X;
  	double Cur_Y;
  
  	for(int i=0;i<nb_city;i++)
  	{
    	FIC >> Cur_City;  
    	FIC >> Cur_X;  
    	FIC >> Cur_Y; 
     
		Coordinate_X[Cur_City-1]=Cur_X;  //correspent le ville avec l'indice de tableau
		Coordinate_Y[Cur_City-1]=Cur_Y;   
 	}
	    
  	FIC.close();   
	   
  	return true;    
}

//Enregistrer les résultats dans le fichier .txt pour les instances TSPlib
bool Save_Solution(const char* Output_File_Name)
{
	FILE *fp; 
  	fp=fopen(Output_File_Name, "w+"); 
  
   	fprintf(fp,"%d\n",nb_city);
  	fprintf(fp,"%d\n",Start_City+1);
    
    int Cur_City=Start_City;
	do
	{
		fprintf(fp,"%d\n ",Cur_City+1);
		Cur_City=All_Node[Cur_City].Next_City;		
	}while(Cur_City != Null && Cur_City != Start_City);
	
	fprintf(fp,"%d\n ",Get_Solution_Total_Distance());
			
    fclose(fp); 
  	return true;    
}

int main(){
    //Tester les instances de TSPlib
    string Name = "Instances/a280.tsp";
    const char* Res = "Resultats/Res_a280.txt";
    double Overall_Begin_Time=(double)clock();
    Read_Instance_Info(Name);
    Calculate_All_Pair_Distance();
    Current_Instance_Best_Distance = Inf_Cost;
    Markov_Decision_Process();
    Save_Solution(Res);
    /*Pour tester les cas de petit dimension
    double Overall_Begin_Time=(double)clock();
    nb_city=5;
    Start_City = 0;
    Allocate_Memory(nb_city);
    Give_Distance();
    Afficher_Distance();
    Current_Instance_Best_Distance = Inf_Cost;
    Markov_Decision_Process();
    Convert_All_Node_To_Solution();
    for(int j=0;j<nb_city;j++){
        cout<<"City: " << Solution[j]<<"";
        cout<<"City_Pre: " << All_Node[j].Pre_City<<"";
        cout<<"City_Next: " << All_Node[j].Next_City<<"";
        cout<<"\n";
    }*/
    
    return 0;
}