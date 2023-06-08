#include "Fonctions.h"

// Initialiser les paramètres utilisé dans le processus MCTS
void MCTS_Init()
{
	for(int i=0;i<nb_city;i++)
		for(int j=0;j<nb_city;j++)
		{
			Weight[i][j]=1;
			Chosen_Times[i][j]=0;	
		}
	
	Total_Simulation_Times=0;	
}

//Obtenir le poids moyen pour tous les arêtes part de Cur_City
double Get_Avg_Weight(int Cur_City)
{
	double Total_Weight=0;
    double moyen;
	for(int i=0;i<nb_city;i++)	
	{
		if(i != Cur_City)
			Total_Weight+=Weight[Cur_City][i];						
	}
    moyen = Total_Weight/nb_city-1;
		
	return moyen;
}

//Estimer le potentiel(pour obtenir le probabilité pour connecter b_i et a_i+1)
double Get_Potential(int First_City, int Second_City)
{	
	double Potential = Weight[First_City][Second_City]/Avg_Weight+Alpha*sqrt( log(Total_Simulation_Times+1) / ( log(2.718)*(Chosen_Times[First_City][Second_City]+1) ) );
	
	return Potential;	
}

//Obtenir les 10 villes le plus proche que le Cur_City et éviter de connecter à premier ville et Pre_City
void Select_10_nearestCity(int Cur_City,int Begin_City){
    int Smallest_Distance;
    int Smallest_indice;
    for(int i=0;i<nb_city;i++){
        If_City_Selected[i] = false;
    }
    int nb = 0;
	If_City_Selected[Cur_City] = true;
	If_City_Selected[Begin_City] = true;
    while(nb<Max_Candidate_Num){
		int ind;
		for(int k=0;k<nb_city;k++){
			if(If_City_Selected[k] == false){
				ind = k;
				break;
			}
		}
		Smallest_Distance = Distance[Cur_City][ind];
		Smallest_indice = ind;
        for(int j=ind;j<nb_city;j++){
            if((Distance[Cur_City][j]<=Smallest_Distance)&&(If_City_Selected[j]==false)){
                Smallest_Distance = Distance[Cur_City][j];
                Smallest_indice = j;
            }
        }
        Candi_city[nb]=Smallest_indice;
        If_City_Selected[Smallest_indice] = true;
        nb++;
    }
}

//Obtenir les candidats possibles de a_i+1 pour connecter b_i
void Get_Candidate_city(int Cur_City,int Begin_City){
    int nb = 0;
    if(nb_city-3 < Max_Candidate_Num){
        for(int i=0;i<nb_city;i++){
            if((i!=Cur_City)&&(i!=All_Node[Cur_City].Pre_City)&&(i!=Begin_City)){
                Candi_city[nb] = i;
                nb++;
            }
        }
        Candi_nb_city = nb;
    }
    else{
        Select_10_nearestCity(Cur_City,Begin_City); //Si on peut avoir plus de 10 villes peut connecter à Cur_city on choisit les 10 plus proche(comme indiquer dans le rapport)
        Candi_nb_city = Max_Candidate_Num;
    }
}


// Obtenir le pobabilité (enregistré dans le Probabilistic[]) pour selectionner chaque ville de candidat
bool Get_Probabilistic(int Cur_City){	
	double Total_Potential=0;
	for(int i=0;i<Candi_nb_city;i++)	Total_Potential+=Get_Potential(Cur_City, Candi_city[i]);	
		
	Probabilistic[0]=(int)(1000*Get_Potential(Cur_City, Candi_city[0])/Total_Potential);
	for(int i=1;i<Candi_nb_city-1;i++)	Probabilistic[i]=Probabilistic[i-1]+(int)(1000*Get_Potential(Cur_City, Candi_city[i])/Total_Potential);	
	Probabilistic[Candi_nb_city-1]=1000;	
	
	return true;
}

// Choisir la ville à connecter(controlé par le Probabilistic[])
int Probabilistic_Get_City_To_Connect()
{
	int Random_Num = Get_Random_Int(1000);
	for(int i=0;i<Candi_nb_city;i++)
		if(Random_Num < Probabilistic[i])
			return Candi_city[i];
	
	return Null;
}

// Le processus complet pour chosir la ville (a_i+1 dans le rapport) pour connecter Cur_City (b_i dans le rapport)
int Choose_City_To_Connect(int Cur_City,int Begin_City)
{
	Avg_Weight = Get_Avg_Weight(Cur_City);		
	Get_Candidate_city(Cur_City,Begin_City);
	Get_Probabilistic(Cur_City);	
	
	return Probabilistic_Get_City_To_Connect();
}

//Obtenir le différence de distance entre permutation de ville actuel et initial
Distance_Type Get_Simulated_Action_Delta(int Begin_City)
{
	// Stocker le solution actuel à Solution[]
	if(Convert_All_Node_To_Solution()==false)
		return -Inf_Cost;		
	
	int Pre_City=All_Node[Begin_City].Pre_City;   // a_1=Begin city, b_1=Next_City
	
	//Casser arête (a_1,b_1)
	All_Node[Begin_City].Pre_City=Null;           
	All_Node[Pre_City].Next_City=Null;				

	// Les éléments pour un action est stocké dans City_Sequence[] (a_{i+1}=City_Sequence[2*i], b_{i+1}=City_Sequence[2*i+1])
	City_Sequence[0]=Begin_City;                     
	City_Sequence[1]=Pre_City;	

    Gain[0] = -Get_Distance(Begin_City,Pre_City);
	Real_Gain[0] = 0;   // Real_Gain[i] stocker le delta (après connecter à a_1 forme le boucle) en le (i+1)ème fois d'itération
	int nb = 0;     // nb indiquer le nombre de fois (k de k_opt dans le rapport) pour un action	
	
	bool If_Changed=false;				
	int Cur_City = Pre_City;

	while((Real_Gain[nb]>=0)&&(nb<=Max_Depth)){		

		int Next_City_To_Connect = Choose_City_To_Connect(Cur_City,Begin_City);	//Choisir a_{i+1}
		
		//Ronouveler le nombre de fois choisie, utilisé dans le MCTS	
		Chosen_Times[Cur_City][Next_City_To_Connect] ++;
		Chosen_Times[Next_City_To_Connect][Cur_City] ++;	
		
		int Next_City_To_Disconnect = All_Node[Next_City_To_Connect].Next_City; // Determiner b_{i+1}

		// Renouveler City_Sequence[], Gain[], Real_Gain[] et Pair_nb_city
		City_Sequence[2*(nb+1)] = Next_City_To_Connect;
		City_Sequence[2*(nb+1)+1] = Next_City_To_Disconnect;	

		Gain[nb+1] = Gain[nb] - Get_Distance(Next_City_To_Connect,Next_City_To_Disconnect) + Get_Distance(Cur_City,Next_City_To_Connect);
		Real_Gain[nb+1] = Gain[nb+1] + Get_Distance(Next_City_To_Disconnect,Begin_City);	
		nb++;	

		// Reverser les villes entre b_i et b_{i+1}	
		Reverse_Sub_Path(Cur_City,Next_City_To_Disconnect);			
		All_Node[Next_City_To_Connect].Next_City = Cur_City;
		All_Node[Cur_City].Pre_City = Next_City_To_Connect;
		All_Node[Next_City_To_Disconnect].Next_City=Null;

        Cur_City = Next_City_To_Disconnect;	
		If_Changed=true;
		
	}


    // Reenregistrer le solution avant simulation(on manipule toujours dans All_node et ne change pas le tableau Solution[], donc le Solution[] reste toujours le permutation de ville originale)	
	if(If_Changed)	
		Convert_Solution_To_All_Node();			
	else
	{
		All_Node[Begin_City].Pre_City = Cur_City;
	    All_Node[Cur_City].Next_City=Begin_City;	
	}	
	
	// Obtenir delta
	int Real_Gain_value = 0;
    if(Real_Gain[nb]<0){
        Real_Gain_value = Real_Gain[nb];
		Pair_nb_city = nb+1;
    }
	
	return Real_Gain_value;	
}

//Si le delta pour un action est moins que zéro, utiliser l'information de cet action(Stocké dans le City_Sequence) pour renouveler les paramètres par rétropropagation
void Back_Propagation(Distance_Type Before_Simulation_Distance, Distance_Type Action_Delta)
{
	for(int i=0;i<Pair_nb_city;i++)  
	{
		int First_City = City_Sequence[2*i];
		int Second_City = City_Sequence[2*i+1];
		int Third_City;
		if(i<Pair_nb_city-1)	
			Third_City = City_Sequence[2*i+2];
		else
			Third_City = City_Sequence[0];
		
		if(Action_Delta >0)
		{
			double Increase_Rate = Beta*(pow(2.718, (double) (-Action_Delta) / (double)(Before_Simulation_Distance) )-1);
			Weight[Second_City][Third_City] += Increase_Rate;
			Weight[Third_City][Second_City] += Increase_Rate;							
		}		
	}		
}

// Simuler le plus Max_Simulation_Times actions
Distance_Type Simulation(int Max_Simulation_Times)
{
	Distance_Type Best_Action_Delta = 0;
	for(int i=0;i<Max_Simulation_Times;i++)
	{
		int Begin_City = Get_Random_Int(nb_city);
		Distance_Type Action_Delta = Get_Simulated_Action_Delta(Begin_City);		

		Total_Simulation_Times++;
		
		if(Action_Delta<Best_Action_Delta){
			Best_Action_Delta = Action_Delta;
			break;
		}					
	}	

	return Best_Action_Delta;	
}

//Executer le meilleur action enregistré dans City_Sequence[] avec profondeur Pair_nb_city
bool Execute_Best_Action()
{
	int Begin_City = City_Sequence[0];
	int Cur_City = City_Sequence[1];	
	All_Node[Begin_City].Pre_City=Null;
	All_Node[Cur_City].Next_City=Null;	

	for(int i=1;i<Pair_nb_city;i++)  
	{
		int Next_City_To_Connect = City_Sequence[2*i];
		int Next_City_To_Disconnect = City_Sequence[2*i+1];	
		
		Reverse_Sub_Path(Cur_City,Next_City_To_Disconnect);		
				
		All_Node[Cur_City].Pre_City = Next_City_To_Connect;
		All_Node[Next_City_To_Connect].Next_City = Cur_City;
		All_Node[Next_City_To_Disconnect].Next_City = Null;
		
		Cur_City = Next_City_To_Disconnect;				
	}

	All_Node[Begin_City].Pre_City = Cur_City;
	All_Node[Cur_City].Next_City = Begin_City;	
	
	if(Check_Solution_Feasible()==false)
	{
		printf("\nError! The solution after applying action from %d is unfeasible\n",Begin_City+1);
		return false;		
	}	

	return true;
}

// Processus MCTS
void MCTS()
{	 
	while(true)
	{
		Distance_Type Before_Simulation_Distance = Get_Solution_Total_Distance();
		
		//Simuler n fois(controlé par Param_H) actions
		Distance_Type Best_Delta=Simulation(Param_H*nb_city);
		
		// Utiliser l'information dans le meilleur action pour mise à jour les paramètres de MCTS par le rétropropagation
		Back_Propagation(Before_Simulation_Distance,Best_Delta);
				
		if(Best_Delta < 0)
		{
			// Selectionner le meilleur action pour executer
			Execute_Best_Action();

			// Stocker le meilleur solution dans Struct_Node *Best_All_Node				
			Distance_Type Cur_Solution_Total_Distance=Get_Solution_Total_Distance();

			if(Cur_Solution_Total_Distance < Current_Instance_Best_Distance)
			{
				Current_Instance_Best_Distance = Cur_Solution_Total_Distance;
				cout<<"CHANGE "<<Current_Instance_Best_Distance;//Afficher chaque chemin diminuée
				cout<<"\n";
				Store_Best_Solution(); 
			}					
			Convert_All_Node_To_Solution();
		}		
		else
			break;	  // Le MCTS terminer si il n'existe plus d'action amélioré	
	}	
} 