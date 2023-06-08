#include <time.h>
#include <ctime>
#include "MCTS.h"

//Choisir une ville par hasard
int Select_Random_City(int Cur_City) 
{	
	int Random_Index=Get_Random_Int(nb_city);
	for(int i=0;i<nb_city;i++)	
	{
		int Candi_city=(Random_Index+i)%nb_city;		
		if(!If_City_Selected[Candi_city])			
			return Candi_city;
	}
	
	return Null;
}

//Créer une solution(chemin) par hasard et transformer dans le Structure *All_Node
bool Generate_Initial_Solution()
{
	for(int i=0;i<nb_city;i++)	
	{
		Solution[i] = Null;
		If_City_Selected[i] = false;	
	}
	
	int choix_nb = 0;

	Solution[choix_nb] = Start_City;
	int Next_City;
	int Cur_City = Start_City;
	If_City_Selected[Cur_City]=true;

	while(choix_nb < nb_city-1)
	{	
		Next_City=Select_Random_City(Cur_City);	 		
		if(If_City_Selected[Next_City]==false)
		{
			Cur_City = Next_City;
			If_City_Selected[Cur_City]=true;	
			choix_nb++;
			Solution[choix_nb] = Cur_City;								
		}		
	};

	Convert_Solution_To_All_Node();
	return Check_Solution_Feasible();		
}

//Même que Generate_Initial_Solution()
void Jump_To_Random_State()
{
	Generate_Initial_Solution(); 	
}

//Répéter le MCTS jusqu'à temps limite
Distance_Type Markov_Decision_Process() 
{
	MCTS_Init();                      // Initialiser MCTS parametres
	Generate_Initial_Solution();      // Initialiser un chemin
	MCTS();		                      // MCTS

	while(((double)clock()-Current_Instance_Begin_Time) /CLOCKS_PER_SEC<Param_T*nb_city)
	{
		Jump_To_Random_State();
		MCTS();
	}
	
	//Copier le meilleur solution dans le Struct_Node *Best_All_Node à Struct_Node *All_Node 
	Restore_Best_Solution();
	
	if(Check_Solution_Feasible())
		return Get_Solution_Total_Distance();
	else
		return Inf_Cost;
}