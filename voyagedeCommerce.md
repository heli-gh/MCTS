

[TOC]



## Monte-Carlo Tree search pour voyage de commerce

## Introduction

MCTS = trees search + monte carlo methode

 le processus de base des MCTS commencent généralement par un arbre contentant uniquement le noeud de racine, à partir de là , l'arbre se développe progressivement par 4 étape avant de prendre le décision:

-  Sélection : en partant du noeud racine, on choisir le plus "intéresante" noeud existe L.
-  Expansion: si L n'est contient pas une solution, alors on crée un ou plusieurs noeuds enfants de  (si l'enfant sont déjà existé on créer pas) et sélectionne un noeud enfant C.
-  Simulation : à partir de C, effectuer une simulation aléatoire.(monte carlo)
-  Mise à jour: avec le résultats de simulation obtenus, mettre à jour la séquence de déplacement actuel.

### Représentation de la solution

la solution de voyage de commerce est représentée par la séquence des villes qui correspond à l'ordre de la villes. donc pour notre arbre , chaque noeud est représente une ville choisi et les noeuds suivant représente la ville prochaine choisie . 

### Sélection 

la fonction d'évaluation pour la noeud N définie par:
$$
h(N) = \frac{x_j-x_{min} }{x_{max}-x_{min}}- 2C_p \sqrt[2]{\frac{ln(n)}{n_j}}
$$
donc le noeud le plus interessant est qui minimise le $h(N)$ 

pour sélection  de $C_p$ on fixe d'abord (après tester notre code, on choisie $C_p$ entre $[0.2,0.3]$ pour équilibrer les explorations et exploitations)

$x_j$ : le minimum de la coût de simulation précédant.

$n$ : le nombre totale de simulation pour le noeud parent.

$x_{min}$ le cout minimum de le noeud parent.

$x_{max}$ le cout maximum de le noeud parent.

$n_j$ : le nombre de simulation pour cette noeud N.



le premier terme pour uniformiser le $x_j$ entre 0 et 1. on fait une référence de wikipédia [1](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search) qui nous montrons que probabilité de gagner un jeux est entre 0 et 1. 



### Expansion

on creer les noeud enfants pour le noeud N (le union de solution des noeud enfants contient tous les solution de N), **on choisir aléatoirement un parmi les noeuds enfants C** . mais il y a une contrainte on ne peux pas choisir les villes qui est déjà choisie

dans notre version de code, on créer un noeud enfant par une fois.

### Simulation

on choisir une séquence des villes arbitrairement mais on fixe le ville qui est représenté par noeud avant C.

mais quand on test le méthode qui choisir une séquence des villes arbitrairement. c'est très   moins performant. donc on choisie une méthode de rollout, qui choisir le séquence des villes suivi la lois de probabilité lié à distance entre eux. pour former les probabilités on utilise le fonction softmax. 



### Mise à jour

mise à jour $n$, $n_j$, $x_j$(si on trouve la coût de cette séquence est inférieur que ancien)  et $x_{max}$ pour les noeuds de cette branche 

il y a un limite peut être mise à jour. le coût minimum on trouve.



### Implémentation

#### classe Node

```c++
//parent node of the tree
	Node* parent;
    //the information of the node 
    Info content;
//the city number

int number;

// the location information of the node 
int level;

// the extend status whether or not fully developped

bool extend;
//the  path infomation of the node
int*  tableux;
Node<T> ** childs;

// the number of childs
int nc;

//the maximum travel cost of the childs
double Xmax;
```



**create _child():** pour créer une nouvelle fils qui représente ajoute une nouvelle ville dans le parcours sans répétition.

si on peut créer le nouvelle fils 

​		tout d'abord on chercher les numéros des villes déjà passé, numéro de ville déjà passé par le fils. parmi eux, on choisie aléatoirement . et allouer le espace pour nouvelle noeud.



**rollout (): **pour faire une simulation aléatoire à partir de ce noeud.*

on va simuler trois fois pour récupérer le valeur minimum.

pour chaque fois 

​	d'abord on va cumuler les numéros de citées qui n'ai pas arrivée.

​	ensuite nous allons tirer les cité par rapport la probabilité lié son distance.

​	à la fin calculer la distance total;

à la fin de cet fonction, content.X et Xmax est mis à jour.



**distribution(*permit,debut,fin):** pour remplir le chemin hamiltonien suivi le probabilité par rapport de distance(calule par softmax).



**insert(): ** pour  que tous les fils  forme une tas

si tous les files est tous développé , c'est à dire on ne peut pas créer plus de fils,comme on choisir que le premier fils pour faire une simulation donc on appliquer downAjust().

sinon les fils n'est pas encore tous développé. on appliquer upAjust().



**downAjust(int low ,int high)** pour ajuster le tas de tête à pied.

choisir i=low et j=2i+1 son fils gauche(dans le tas) pour vérifier si les fils j,j+1 un est plus petite que racine i  on échange le fils plus petite et racine et commence vérifier par la suite i=j,j=2i+1 jusqu'à les fils sont plus grande que racine ou touche le fonds c'est à dire il admet pas la fils.

**upAjuste(int low , int high) **pour ajuster le tas de pied à tête

même principe que downAjuste() mais on commence à la fin de tas, et vérifier son parent est plus petite que lui ou pas.



**back_up() **pour mise à jour le content de parent et coût maximum de parent.



#### classe Info 

```c++
//the minimum value among the previous tirals
double X;
//the weight
static double Cp;
//the number of trials 
int N;
```

**UCT(int n, double Xmin , double Xmax)**
$$
\frac{X - Xmin} {Xmax - Xmin} - 2 Cp \sqrt\frac{log(n) }{ N};
$$



#### classe MCTS

**selection():** pour déterminer quel Node faire développé.

commence à racine, si le noeud est développé et le noeud touch pas le fond.

on refaire le vérifier de son fils childs[0] qui trier par tas par rapport de Info::UCT .

sinon  on retourner lui même.



**expansion(Node bestchild)** pour créer une nouvelle fil de cet noeud .

si il touche le fond alors retourner lui même.

sinon créer une fils pour lui et le retourner.



**simulation(Node child)** pour estimer la parcours minimum de cet noeud.

exécuter le rollout pour obtenir un parcours aléatoire et remplir le Info



**update(Node child)** commence à child mis à jour les information de son ancêtres.



#### méthode1

pour une racine fixé, on fait

```c++
while(n<iteration){
        t=selection();
        t=expansion(t);
        simulation(t);
        update(t);
        n++;
    }
```

#### méthode 2

pareil que le méthode 1 sauf que on déplace le racine vers le plus meilleure fils, après certaine nombre de itération. 



## Monte carlo Tree Search pour Voyage de commerce probaliste

### Introduction

Le problème probabiliste du voyageur de commerce (PTSP) a été introduit par Jaillet [2] comme une extension du problème classique du voyageur de commerce (TSP). Dans le PTSP, le vendeur ne doit pas nécessairement visiter chaque nœud, mais une probabilité d'exiger une visite est donnée. Probabilité d'exiger une visite est donnée pour chaque nœud. L'objectif est de trouver une tournée a-priori
qui inclut chaque nœud et minimise la longueur attendue d'une tournée a-posteriori qui contient les noeuds avec les probabilités données et saute les autres. Une application réelle du PTSP consisterait, par exemple, à planifier une tournée quotidienne pour le facteur. tournée quotidienne pour le facteur. Après chaque livraison, il se rend toujours à l'adresse suivante de la tournée planifiée pour laquelle une livraison est requise.

Pour implémentation dans MCTS, on ne change que le méthode pour évaluer le $x_j$ .

### Méthode déterministe

Formellement, on nous donne un graphe complet $G = <V, E>$ avec $V$ contenant n nœuds et$ E$ contenant m arêtes. Chaque arête$ (i, j) \in E$ est affectée d'un coût $d_{ij} > 0$ et chaque noeud $v \in V$ a une probabilité $ p(v)$ d'être visité. Une solution est une tournée a-priori $T = <v_1 , . . . , v_n >$ sur tous les nœuds avec $v_n$ connecté à $v_1$ et ses coûts attendus sont définis comme suit:
$$
c(T) = \sum_{i=1}^{2^n}p(R_i)\times L(R_i) 
$$
où $R_i$ est une réalisation possible, c'est-à-dire un tour a posteriori possible, et $p(R_i )$ sa probabilité d'occurrence. Comme il existe un nombre exponentiel de réalisations différentes, il n'est pas pratique d'obtenir la valeur objective de cette manière. C'est pourquoi Jaillet [2] a montré que la longueur attendue peut être calculée en $O(n^3 )$ en énumérant explicitement toutes les réalisations à l'aide du terme suivant :

$$
c(T) = \sum_{i=1}^n \sum_{j=i+1}^{n}d_{v_iv_j}p_{v_i}p_{v_j}\prod_{k=i+1}^{j-1}(1-p_{v_k}) + \sum_{j=1}^n \sum_{i=1}^{j-1}d_{v_iv_j}p_{v_i}p_{v_j}\prod_{k=j+1}^n(1-p_{v_k})\prod_{k=1}^{i-1}(1-p_{v_k})
$$
le premier terme signifie le coût moyen de passe $v_i$ vers $v_j$.

le deuxième terme au contraire, signifie le coût moyen de passe $v_j$ vers $v_i$ 

Dans notre cas, par défaut, on supposons que le ville départ (0) est fixé avec son probabilité d'occurrence égale 1.

Par conséquence, il n'aura pas le cas de passe $v_j$ vers $v_i$ qui sont dans l'ordre $i<j$ . donc on reformule terme:
$$
c(T) = \sum_{i=1}^n \sum_{j=i+1}^{n+1}d_{v_iv_j}p_{v_i}p_{v_j}\prod_{k=i+1}^{j-1}(1-p_{v_k})
$$


où $v_1= v_{n+1} =0 $ .

### Méthode approximation

Même si le terme $c(T)$ produit un temps d'évaluation polynomial pour le PTSP, le temps $O(n^3 )$ résultant pour calculer coût de PTSP est encore très très long, en particulier pour les méthodes métaheuristiques qui doivent évaluer à plusieurs reprises la valeur de la fonction objective $c(T)$. Dans cette étude, l'algorithme proposé doit comparer plusieurs fois deux solutions (c'est-à-dire la nouvelle solution avant et après la recherche locale) sur la base de la valeur de la fonction objective. Par conséquent, l'évaluation approximative a été utilisée pour augmenter l'efficacité de calcul de l'algorithme proposé.
$$
E(T) = \sum_{i=1}^n \sum_{j=i+1}^{\min (n+1,i+\lambda )}d_{v_iv_j}p_{v_i}p_{v_j}\prod_{k=i+1}^{j-1}(1-p_{v_k})
$$
La seule différence entre $c(T)$ et $E(T)$ est le choix de la position de troncature $\lambda $ dans $E(T)$. L'équation $E(T)$ aura une complexité de calcul de $O(n  \lambda^2 )$ au lieu de $O(n^3 )$ dans $c(T)$. Il est facile de voir que $E(T)$ devient plus précis lorsque $\lambda$ augmente. Cependant, une valeur plus élevée de $\lambda $ nécessite plus d'efforts de calcul  $E(T)$. L'équation E(T) peut donner une très bonne approximation de $c(T)$ avec une valeur petite de  $\lambda$ lorsque le probabilité $p(v_k)$ devient grande, parce que $\prod_{k=i+1}^{j-1}(1-p_{v_k})$produira un très faible valeur et peut être omise. Néanmoins, l'équation $E(T)$ aura besoin d'une plus grande valeur de $\lambda$ pour effectuer une bonne approximation lorsque la valeur de $p (v_k)$ est petite.

Pour une tournée donnée, $E(T)$ est toujours inférieur à la valeur de la valeur de $c(T)$ en raison de la troncature dans le calcul de E. Laissons $b$ et $a$ désigner respectivement la tournée a priori avant et après l'application d'une méthode de recherche locale spécifique. Cela signifie qu'aucune amélioration n'a été trouvée après la recherche locale si $E(a)\ge E(b)$. $c(T)$ est utilisé pour évaluer exactement la solution après la recherche locale si $E(a) < E(b) $. Si la recherche locale donne une meilleure valeur $c(b)$ que celle de la solution originale (c'est-à-dire $c(a) < c(b)$), la nouvelle solution a remplacera la solution originale. Si aucune amélioration n'a été trouvée après la recherche locale, aucun remplacement n'est effectué.

### Méthode estimation

faire référence de [3].

Nous considérons le PTSP comme un problème d'optimisation combinatoire stochastique d'optimisation combinatoire stochastique qui peut être décrit comme suit : Minimiser $F (x) = E (f (x, \Omega))$, sous réserve que $x \in S$, où x est une solution a priori, $S$ est l'ensemble des solutions réalisables, l'opérateur $E$ représente l'espérance mathématique, et $f (x, \Omega)$  est le coût de la solution a posteriori qui dépend d'une variable aléatoire $\Omega$, qui est une distribution de Bernoulli à n variables, et d'une réalisation $\omega$ de la solution a posteriori. et une réalisation $\omega$ de $\Omega$ prescrit quels nœuds doivent être visités. Un estimateur sans biais de $F (x)$ d'une solution $x $ peut être calculé sur la base d'un échantillon de coûts de solutions a posteriori obtenues à partir de M solutions indépendantes de la variable aléatoire Ω.

Dans les algorithmes d'amélioration itérative pour le PTSP, nous devons comparer deux solutions voisines $x$ et $x'$ afin de sélectionner celle dont le coût est le plus faible. Pour $x$ , un estimateur sans biais de $F (x' )$ peut être estimé analogiquement $ F (x)$ en utilisant un ensemble diﬀérent de M' réalisations indépendantes de $\Omega$. Cependant, afin d'augmenter la précision de cet estimateur, la technique bien connue de réduction de la variance appelée la méthode des nombres aléatoires communs peut être adoptée.
Méthode des nombres aléatoires communs, dans le contexte du PTSP, cette technique consiste à
à utiliser le même ensemble de réalisations de $\Omega$ pour estimer les coûts $F (x')$ et $F (x)$. Par conséquent,
nous avons M = M' et l'estimateur $\hat{F}_M (x') - \hat{F}_M (x)$ de la diﬀérence de coût est donné par :
$$
\hat{F}_M (x') - \hat{F}_M (x) = \frac{1}{M}\sum_{r=1}^M(f(x',w_r)-f(x,w_r))
$$
Nous avons mis en œuvre des algorithmes d'amélioration itérative qui utilisent cette façon d'estimer les diﬀérences de coût en exploitant une structure de voisinage qui utilise 2-opt. Pour rendre le calcul des diﬀérences de coût aussi eﬃcient que possible, étant donné deux solutions a priori voisines et une réalisation $\omega$, l'algorithme doit identifier les arêtes qui ne sont pas communes aux deux solutions a posteriori. Ceci est réalisé comme suit : 

pour chaque arête $<i, j >$ qui est supprimée de $x$, il faut ﬁnaliser l'arête correspondant à la solution a posteriori. pour chaque arête$ <i, j>$ supprimée dans $x$, il faut trouver l'arête correspondante $<i' , j'>$ qui est supprimée dans la solution a posteriori de $x$. Nous appelons cette arête l'arête a posteriori et nous l'obtenons comme suit :  Si le nœud $i$ nécessite une visite, alors $i ' =i $, sinon, $i'$ est le premier prédécesseur de $i $ dans $x $ tel que $\omega[i']= 1$, c'est-à-dire le premier prédécesseur pour lequel la réalisation est un, ce qui indique qu'il nécessite une visite. Si le nœud $j$ nécessite une visite, alors $j' = j$, sinon, $j'$ est le premier successeur de $j$ tel que $\omega [j'] = 1$. Rappelons que dans un mouvement 2-opt, les arêtes $<a, b>$ et $<c, d>$ sont supprimées de $x $ et remplacées par $<a, c>$et$< b, d>$. Pour une réalisation $\omega$ donnée et les arêtes a-posteriori correspondantes, $<a' , b'> $, $<c' , d'>$ , la diﬀérence de coût entre les deux solutions a posteriori est donnée par $d(a' ,c' ) + d( b' ,d' ) - d( a' ,b ' ) - d( c' ,d' ) $, où $d (i,j)$ est le coût de l'arête $<i, j>.



### Implémentation

```c++
 
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
```





## MCTS + Réseaux profonds + Apprentissage Renforcement

faire référence de [4]

### MCTS

#### Selection

(Kocsis et Szepesvri 2006) ont proposé une stratégie de sélection appelée Upper Confidence bounds applied to Trees (UCT), qui a remporté un grand succès dans le jeu. Il existe certaines différences entre le jeu et les problèmes d'optimisation combinatoire. 

Tout d'abord, une branche avec le taux moyen de gain le plus élevé est préférée dans le jeu, alors que l'optimisation combinatoire vise à trouver l'extrême, qui peut se situer dans la direction sans une bonne valeur moyenne. Ainsi, étant donné un nœud $s$, nous modifions la politique de l'UCT en sélectionnant l'enfant $i$ de s qui maximise la la formulation suivante,
$$
arg\max_{\hat{Q}_i}(\hat{Q}_i+2*C_p*\sqrt\frac{\ln N_s}{N_i})
$$
où $\hat{Q}_i = -f (i)$ , $f$ est défini comme suit:
$$
f(v)=g(v)+h(v)
$$
où $g(v)$ est connu et représente la longueur réelle de la séquence ordonnée $S$ du premier au dernier nœud, $h(v)$ est inconnu et supposé être la longueur optimale de $v$ au début ville 0 ( cycle) . Dans notre cadre, $h(v)$ est évalué par un réseau neuronal profond, qui sera décrit dans la section suivante. $\hat{Q}_i$ est la meilleure récompense trouvée sous la sous-arborescence du nœud $i$ . $N_s $ et $N_i$ sont respectivement le nombre de visites du nœud $s $et du nœud $i$. $C_p > 0$ est un paramètre utilisé pour équilibrer l'exploitation et l'exploration

De plus, l'étendue de la valeur $\hat{Q}$ est différente entre les problèmes de jeu et les problèmes d'optimisation combinatoire. Dans les jeux, le résultat d'un jeu est composé d'une perte, d'un match nul et d'une victoire, c'est-à-dire ${0, 0,5, 1}$. La récompense moyenne d'un nœud reste toujours comprise entre $[0 , 1]$. Dans les problèmes d'optimisation combinatoire, une récompense arbitraire peut ne pas se situer dans l'intervalle prédéfini. dans l'intervalle prédéfini. Nous normalisons donc la meilleure récompense de chaque nœud $c$ dont le parent est le nœud $p$ à $ [0,1] $ avec la formulation suivante,
$$
\hat{Q}_c = \frac{\hat{Q}_c-\hat{Q}_{min}}{\hat{Q}_{max}-\hat{Q}_{min}}
$$
où $\hat{Q}_{max}$ et $\hat{Q}_{min}$ sont respectivement le maximum et le minimum parmi tous les nœuds enfants du nœud $p$, respectivement.

#### Expansion

Lorsqu'un nœud feuille $I$ est atteint, nous développons le nœud jusqu'à ce que son nombre de visites atteigne un seuil prédéfini (nous avons fixé ce seuil à $40$). Cette méthode permet d'éviter de générer un trop grand nombre de branches, ce qui distrait la recherche et économise les ressources de calcul. Comme l'algorithme A* (Hart, Nilsson et Raphael 1968), nous développons tous les nœuds enfants du nœud feuille l en même temps.

#### Simulation

Nous utilisons la fonction de valeur $h$ pour évaluer tous les nœuds enfants qui sont développés dans l'étape d'expansion

#### rétro-propagation

nous choisissons d'utiliser la meilleure récompense. de simulation parmi tous les nœuds enfants pour la rétro-propager vers la racine.

### graphes convolutions réseaux

Inspiré par le graph embedding network, nous proposons d'utiliser des convolutions de graphe pour extraire des caractéristiques du graphe. Chaque nœud du le graphe est représenté par un vecteur de caractéristiques et fusionne les informations de ses nœuds voisins de manière récursive en fonction de la topologie du graphe. Pour chaque nœud, la caractéristique est exprimée sous la forme d'un vecteur à 9 dimensions. Nous utilisons un élément 0 ou 1 pour indiquer si un nœud a été traversée ou non. En outre, les informations sur le nœud actuel (état de la traversée, coordonnées x, coordonnées y), nous prenons particulièrement en compte le premier et le dernier nœud du chemin parcouru. dernier nœud du chemin parcouru car le chemin de la solution est le chemin hamiltonien. De plus, nous utilisons le poids des arêtes(distance) comme caractéristique supplémentaire

Nous décrivons maintenant la paramétrisation des convolutions de graphes à l'aide de graphes embedding. Nous faisons correspondre les caractéristiques de chaque nœud $v$ du graphe à l'espace dimension plus large en utilisant la formule suivante:
$$
H_v^{t+1}= \sigma(\theta_1 x_v+\theta_2\sum_{u\in \mathcal{N(v)}}H_u^t+\theta_3\sum_{u\in\mathcal{N(v)}}\sigma(\theta_4d_{v,u}))
$$
où $\theta_1 ∈ \mathbb{R}^l , \theta_2,\theta_3 ∈ \mathbb{R}^ {l×l}$ et $θ 4 ∈ \mathbb{R}^ l$ sont les paramètres, et $\sigma$ est l'unité linéaire rectifiée (relu).$ x_v$ et $d_ {v,u}$ sont les caractéristiques du nœud et la distance entre les deux nœuds mentionnés ci-dessus, respectivement. Et $\mathcal{N (v)}$ désigne les nœuds voisins du nœud $v$.

Après T itérations, chaque nœud est intégré dans le graphe, nous utiliserons ces informations d'intégration pour définir h(v), nous calculons h(v) comme suit,
$$
h(v)= \theta_5 \sigma([\theta_6\sum_{u\in V}H_u^T,\theta_7H_v^T])
$$
où $\theta_5 ∈ \mathbb{R}^{2l} ,\theta_6,\theta_7 ∈ \mathbb{R}^ {l\times l}   $ et $[-, -] $désigne l'opérateur de concaténation.

on le suggère T=4;

### Auto-apprentissage

#### Formulation de l'apprentissage par renforcement

Nous définissons les états, les actions et les récompenses dans le cadre de l'apprentissage par renforcement de la manière suivante : 

- États : un état $S$ est une séquence ordonnée de nœuds parcourus sur un graphe $G$. Nous utilisons graphe embedding pour coder chaque état sous la forme d'un vecteur dans l'espace à $l$ dimension. L'état terminal $\hat{S}$ signifie que nous avons parcouru tous les nœuds.
- Transition : la transition est déterministe dans le problème du voyageur de commerce, et correspond à l'ajout du nœud sélectionné $v ∈ \bar{S}$ à $S$, où $\bar{S}$ et $S $ sont respectivement la séquence parcourue et la séquence non parcourue, respectivement.
- Actions : une action $v$ est un nœud de $G$ dans la séquence $\bar{S}$ non parcourue.
- Récompenses : Lorsque tous les nœuds de G sont parcourus, la longueur $d$ de la séquence ordonnée $\hat{S}= \{v_1 , v_2 , .., v_n \}$ peut être calculée selon la formule suivante:

​		
$$
d = \sum_{i=1}^{|\hat{S}|-1}d_{v_i,v_{i+1}} +d_{v_{\hat{S}},v_1}
$$
​		Nous pouvons également calculer la longueur de la séquence partielle $S_p$ =$S ∩ v$ lorsque le nœud $v$ 		est ajouté à $S$ comme suit,
$$
g(v) =\sum_{i=1}^{|\hat{S}^p|-1}d_{v_i,v_{i+1}}
$$
​		Nous définissons la fonction de récompense $ r(s, v)$ à l'état $s$ comme la longueur de la séquence

​		partielle ordonnée de $\hat{S}$ où le nœud de départ est $v$ .C'est-à-dire

​		
$$
r(s,v)=d-g(v)
$$

- Politique : Sur la base de la fonction de valeur $h$ du réseau neurone, nous utilisons la recherche arborescente de Monte Carlo comme politique par défaut pour sélectionner l'action $v$ suivante. Après avoir répété t fois la lecture, nous choisissons une action $v$ parmi toutes les actions valides de l'état racine s par la la formulation suivante,

​			
$$
v=arg\max_{v_i\in V_c} \hat{Q}_{v_i}
$$
où $V_c$ est l'ensemble de toutes les actions valides de l'état racine $s$, et $ \hat{Q}_{v_i}$ est la récompense de l'état, qui est obtenue en effectuant l'action $v_i$ de l'état racine.

#### Algorithme d'apprentissage

Tout d'abord, les paramètres du réseau neuronal sont initialisés à des poids aléatoires $\theta_0$ . Lorsqu'un épisode se termine et que tous les nœuds ont été parcourus, les données pour chaque étape temporelle t sont stockées sous forme de $(s_t , v_t , r_t )$, où $r_t$ peut être calculé selon $r(s,v)$. Le réseau neuronal est formé à partir d'un échantillonnage uniforme de tous les pas de temps $(s, v, t)$. En particulier, les paramètres $\theta$ sont appris par descente de gradient sur une fonction de perte $I$ sur l'erreur quadratique moyenne:
$$
I = (r-h)^2 +c||\theta||^2
$$
où c est un paramètre qui contrôle le niveau.

Notre algorithme de formation, décrit dans l'algorithme 1,

![](https://s2.loli.net/2023/06/07/6X2uJ1yFzi5soKT.png)



## Résultats

Instance a280tsp

pour probabilité d'occurrence égale 0.8 sauf que la probabilité de le ville(0) de départ égale  1.

| nombre de itération | temps   | coût/chemin                                                  |
| ------------------- | ------- | ------------------------------------------------------------ |
| 1000                | 3.81s   | 2870<br/>0 248 251 252 253 254 255 256 257 258 259 260 261 262 263 264 265 139 140 147 146 145 144 143 199 200 195 196 193 194 197 198 142 141 138 137 136 135 267 268 269 133 134 266 148 149 177 176 150 151 155 152 154 153 128 127 126 125 123 122 121 120 119 118 156 157 158 159 174 160 161 162 163 164 165 166 167 168 100 99 98 97 92 93 96 95 94 77 76 74 75 73 72 71 70 66 69 68 67 57 58 43 44 55 56 54 53 52 51 50 49 48 47 46 45 40 39 38 34 35 36 37 33 32 30 31 28 27 26 25 21 24 22 23 13 12 11 10 9 7 6 8 275 274 273 272 271 270 15 14 16 17 18 131 132 130 19 20 129 124 29 41 42 59 60 117 61 62 63 64 65 84 85 115 114 116 113 112 86 83 82 81 80 88 108 107 103 102 101 90 91 89 79 78 87 111 110 109 104 105 172 106 173 171 170 169 187 188 189 190 191 192 185 186 184 183 182 181 180 179 178 175 201 202 203 204 205 206 207 208 209 210 213 214 217 218 221 220 219 216 215 212 211 228 229 250 249 246 244 239 238 245 230 231 232 235 234 233 226 225 224 223 222 227 236 237 243 240 241 242 247 277 278 3 276 5 4 2 279 1 |
| 10000               | 34.02 s | 2838<br/>0 272 7 9 10 11 12 14 13 23 22 24 21 25 26 27 28 30 31 32 33 34 37 35 36 38 39 40 41 42 59 60 117 116 114 115 85 84 64 65 63 62 61 58 57 56 55 44 45 46 54 53 52 51 50 49 48 47 43 67 68 66 69 70 71 72 73 75 74 76 77 78 80 79 89 108 88 81 82 83 86 112 111 87 110 113 109 107 103 104 105 106 172 173 161 162 163 164 165 166 167 168 169 171 170 102 101 100 99 98 97 92 93 96 95 94 91 90 160 174 159 158 157 156 118 119 120 121 122 123 124 29 125 126 127 20 19 130 131 132 17 18 129 128 153 154 152 155 151 150 176 175 180 181 182 183 184 186 185 188 187 189 190 191 192 193 194 195 196 197 200 199 143 142 141 140 139 265 137 136 266 264 263 262 261 260 259 258 257 256 253 252 251 208 207 206 209 210 211 212 213 214 215 216 219 218 221 220 217 222 223 224 225 226 227 228 229 250 249 246 244 239 238 237 230 231 236 232 233 234 235 245 243 240 241 242 247 248 255 254 205 204 203 202 201 198 144 145 146 147 138 148 149 177 178 179 135 267 268 269 133 134 16 15 270 271 273 274 275 276 3 278 277 2 279 1 4 5 6 8 |
| 100000              | 353s    | 2846<br/>0 255 227 228 229 250 249 246 244 239 240 241 242 243 245 238 237 230 231 236 235 232 233 234 226 225 224 223 222 218 221 220 219 216 215 212 211 210 213 214 217 209 208 251 254 253 256 257 258 259 260 261 262 263 264 265 137 136 266 267 268 269 133 134 135 138 147 146 145 142 141 140 139 148 149 177 150 176 175 180 179 178 181 182 183 184 186 185 189 188 187 164 163 162 161 160 174 159 158 157 156 118 119 120 121 122 123 124 29 30 31 28 27 26 25 21 24 22 23 13 14 12 11 10 9 7 6 8 275 274 273 272 271 270 15 16 17 132 131 18 19 20 129 130 128 127 126 125 153 154 152 155 151 173 172 105 104 103 102 101 100 99 98 97 92 93 96 95 94 77 76 74 73 72 71 70 69 66 65 64 63 62 61 117 60 59 42 41 40 39 38 37 36 35 34 33 32 48 47 52 51 50 49 46 53 45 54 55 44 43 56 57 58 67 68 84 85 115 114 116 113 112 86 83 82 81 88 80 79 78 75 87 111 110 109 107 108 89 90 91 168 169 170 171 166 167 165 106 190 191 192 193 196 195 194 200 199 143 144 198 197 201 202 203 204 205 206 207 252 248 247 278 277 3 276 5 4 2 279 1 |

Les résultats expérimentaux montrent que l'utilisation des MCTS en combinaison avec rollout pour résoudre le problème PTSP n'est pas aussi efficace. Elle prend plus de temps et ne permet pas d'atteindre la solution optimale de manière récursive aussi rapidement. Cependant, avec moins de temps, les MCTS peuvent donner un résultat plus satisfaisant.

## Conclusion

Dans le présent document, MCTS sont d'abord proposés et développés pour résoudre le PTSP. Bien que les résultats numériques ne confirment pas la supériorité prédominante de le méthode MCTS, l'incorporation de la notion de diversification toutefois améliore de ses performances(rollout),

elle a permis au chercheur d'être conscient de la possibilité de combiner le MCTS avec d'autre méthode par exmaple (deep networks) ou améliore le étape simulaiton, dans notre cadre , les ville est distribué par la loi formé par le softmax qui restreins la l'aléatoirété parce que le différence de distance entre les villes est très grande.donc le probailité de choisir le ville plus proche est très grande , ce qui ne permets pas de sortir de la solution optimale locale.





## référence

- [1]  [wikipédia](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search)
- [2] Jaillet, P.: Probabilistic Traveling Salesman Problems. Ph.D. thesis, Massachusetts
Institute of Technology (1985)

- [3] Birattari M., Balaprakash P., Stützle T., and Dorigo M. (2007): “Estimation-based local search for stochastic combinatorial optimization”. Technical Report [TR/IRIDIA/2007-003]. IRIDIA, Université Libre de Bruxelles, Brussels, Belgium.

- [4] Solve Traveling Salesman Problem by Monte Carlo Tree Search and Deep Neural Network Zhihao Xing, Shikui Tu, Lei Xu . Department of Computer Science and Engineering, Shanghai Jiao Tong University, Shanghai, China