#include "TSP_IO.h"

// Retourner un entier entre [0,nb)
int Get_Random_Int(int nb)
{ 
  return rand()%nb;
}

//Calculer le distance entre 2 villes, et convertir à entier le plus proche
int Calculate_Int_Distance(int First_City,int Second_City)
{
  	return (int)(0.5 + sqrt( (Coordinate_X[First_City]-Coordinate_X[Second_City])*(Coordinate_X[First_City]-Coordinate_X[Second_City]) +
                (Coordinate_Y[First_City]-Coordinate_Y[Second_City])*(Coordinate_Y[First_City]-Coordinate_Y[Second_City]) ) );
}

//Calculer le distance entre 2 ville (double)
double Calculate_Double_Distance(int First_City,int Second_City)
{
  	return sqrt( (Coordinate_X[First_City]-Coordinate_X[Second_City])*(Coordinate_X[First_City]-Coordinate_X[Second_City]) +
                   (Coordinate_Y[First_City]-Coordinate_Y[Second_City])*(Coordinate_Y[First_City]-Coordinate_Y[Second_City]) );
}

//Calculer les distances entre les villes(entier) et stocker dans le tableau Distance[][]
void Calculate_All_Pair_Distance()
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

// Retourner le distance entre 2 ville dans Distance[][] 
Distance_Type Get_Distance(int First_City,int Second_City)
{
	return Distance[First_City][Second_City];
}

// Utiliser l'information dans Solution[] pour renouveler l'information dans le Struct_Node *All_Node
void Convert_Solution_To_All_Node()
{
  	int Temp_Cur_City;
  	int Temp_Pre_City;
  	int Temp_Next_City;
  
  	for(int i=0;i<nb_city;i++)
  	{
  		Temp_Cur_City = Solution[i];  	
  		Temp_Pre_City = Solution [(i-1+nb_city)%nb_city];
  		Temp_Next_City = Solution [(i+1+nb_city)%nb_city];	
  		
  		All_Node[Temp_Cur_City].Pre_City=Temp_Pre_City;
  		All_Node[Temp_Cur_City].Next_City=Temp_Next_City;   
  	} 
}

// Utiliser l'information dans le Struct_Node *All_Node pour renouveler l'information dans le  Solution[]
bool Convert_All_Node_To_Solution()
{
	for(int i=0;i<nb_city;i++)
		Solution[i]=Null;
	
	int Cur_Index=0;
	Solution[Cur_Index]=Start_City;
	
	int Cur_City=Start_City;
	do
	{
		Cur_Index++;
		
		Cur_City=All_Node[Cur_City].Next_City;
		if(Cur_City == Null || Cur_Index >= nb_city)
			return false;
		
		Solution[Cur_Index]=Cur_City;			
	}while(All_Node[Cur_City].Next_City != Start_City);
	
	return true;
}

//Vérifier le solution dans Struct_Node *All_Node est disponible ou pas pour TSP tour
bool Check_Solution_Feasible()
{
	int Cur_City=Start_City;
	int Visited_City_Num=0;
	while(true)
	{	
		Cur_City = All_Node[Cur_City].Next_City;
		if(Cur_City == Null)	
		{
			printf("\nThe current solution is unvalid. Current city is Null\n");
			return false;
		}
		  		
		Visited_City_Num++;
		if(Visited_City_Num > nb_city)
		{
			printf("\nThe current solution is unvalid. Loop may exist\n");
			getchar();
			return false;
		}
				
		if(Cur_City == Start_City && Visited_City_Num == nb_city)	
			return true;			 	
	}
}

//Retourner le total distance(Entier) de chemin à partir de Struct_Node *All_Node
Distance_Type Get_Solution_Total_Distance()
{
  	Distance_Type Solution_Total_Distance=0;
  	for(int i=0;i<nb_city;i++)
  	{
  		int Temp_Next_City=All_Node[i].Next_City;
  		if(Temp_Next_City != Null)
  	  		Solution_Total_Distance += Get_Distance(i,Temp_Next_City); 
  		else
  		{
  			printf("\nGet_Solution_Total_Distance() fail!\n");
  			getchar();
  			return Inf_Cost;
		}  	  		
  	}	
  
  	return Solution_Total_Distance;
}

// Pour chaque ville entre Second_City et First_City, reverser leur Pre_City et Next_City(utiliser dans k-opt)
void Reverse_Sub_Path(int Second_City,int First_City)
{
	int Cur_City=First_City;
	int Temp_Next_City = All_Node[Cur_City].Next_City;
	
	while(true)
	{	
		int Temp_City = All_Node[Cur_City].Pre_City;
		All_Node[Cur_City].Pre_City=All_Node[Cur_City].Next_City;
		All_Node[Cur_City].Next_City=Temp_City;
		
		if(Cur_City==Second_City)
			break;
		
		Cur_City=Temp_Next_City;
		Temp_Next_City=All_Node[Cur_City].Next_City;	
	}	
} 

// Copier l'information de Struct_Node *All_Node à Struct_Node *Best_All_Node
void Store_Best_Solution()
{
	for(int i=0;i<nb_city;i++)
	{
		Best_All_Node[i].Next_City=All_Node[i].Next_City;
		Best_All_Node[i].Pre_City=All_Node[i].Pre_City;
	}	 
}

// Copier l'information de Struct_Node *Best_All_Node à Struct_Node *All_Node 
void Restore_Best_Solution()
{
	for(int i=0;i<nb_city;i++)
	{
		All_Node[i].Next_City=Best_All_Node[i].Next_City;
		All_Node[i].Pre_City=Best_All_Node[i].Pre_City;
	}	 
}