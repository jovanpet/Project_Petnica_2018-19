#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <cstring>
#include <limits.h>
#include <iostream>
#include <sstream>
#include <climits>
#include <iterator> 
#include <vector>

using namespace std;

int Tij[100][100];       //najbrzi put izmedju i->j
float IVT[100][100];     //x
int time1[100][100];     //vreme izmedju svake dir povezane rute
int need[100][100];      //need izmedju i j
string rute[100][100];   //sve rute izmedju dve (t znaci da ne postoji ruta izmedju njih !!!)
string rfilter[10000];   //niz  
int ruta[100];           //ness pomocno
int n;  int duzina;      //n-broj noda duzina-ness pomocno
float K; float xm;       //K,xm-const          menjam po slucaju!!!
int brute=5;               //broj ruta u grafu   menjam po slucaju!!!
float af2=1;             //konstanta za F2
int transfer[100][100];  //matrica transfera
int U=7;                   //penalty
int pop=20;         	//populacija koju koriatim
int generacion=1;		//broj generacija
int m=5;				//povecanje N

string coverter_int_array_to_string(int niz[], int broj,int src)
{
	ostringstream os;
	os<<src; os<< ' ';
    for (int i=0; i<broj; i++) {
        os << niz[i];
        ruta[i]=0;
        if(i!=broj-1) os<< ' ';
    }

    string str(os.str());
	return str;
}

string covert_int_to_string(int a)
{
    ostringstream temp;
    temp<<a;
    return temp.str();
}

int minDistance(int dist[], bool sptSet[], int V) //za dijkstra
{
     
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false &&
                   dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
 
void printPath(int parent[], int j,int needer)  //vraca nazad put koji je tacno najkraci
{
    if (parent[j] == - 1)
    {
    	return;
	}
 
    printPath(parent, parent[j], needer);
    ruta[duzina++]=j;
    /*if(duzina>0)
    {
    	needer+=need[ruta[duzina-1]][ruta[duzina]];
	}*/
}
 
int printSolution(int dist[], int n, int parent[], int V, int src)  //vraca node najkraceg puta i vreme izmedju
{
    for (int i = src+1; i < V; i++)
    {
    	Tij[src][i]=dist[i]; //vreme izmedju najkracih puteva
    	Tij[i][src]=dist[i];
        printPath(parent, i,0); //vraca najkraci niz noda u nizu
		string p=coverter_int_array_to_string(ruta,duzina,src);  //konvertuje niz u string(p je string najkraceg puta)
		rute[src][i]=p;
		rute[i][src]=p;
        duzina=0;
    }
}
 
void dijkstra(int graph[100][100], int src, int V) //od nekog time matrix(koliko vremena do cega) vraca matrcu najbrzih nacina da stignes od i j
{
    int dist[V]; 
 
    bool sptSet[V];
 
    int parent[V];
 
    for (int i = 0; i < V; i++)
    {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
 
    dist[src] = 0;

    for (int count = src; count < V - 1; count++)
    {
   
        int u = minDistance(dist, sptSet, n);
 
        sptSet[u] = true;
 
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            } 
    }

    printSolution(dist, V, parent,n, src);
}

int propability(int zbir,int num) //biranje broja po nekoj verovatnoci
{
	int i = rand() % zbir; //zbir elemenata
	int c = 0;
	int sum = 0;
	while(c < num) //broj elemenata
	{
    	//sum += act[c]; ovo je u komentaru jer act nepostoji :)
    	if(i < sum)	
		{
			return c;
    	}
    	c += 1;
	}
}

void generate_time()  //generise vreme izmedju i j
{
	int k; 
	k=1;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n; j++)
		{
			time1[i][j]=rand() % 16;
			time1[j][i]=time1[i][j];
		}
		k++;
	}
}

float beta1_generator()  //generise beta1
{
	 float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/-(K/xm))); 
	 return r2;
}

void generate_need()                //generise potrebu izmedju i j
{
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			need[i][j]=rand() % 20+1;
		}
	}
}

int * preklapanje(string k, string t) //vraca niz gde se preklapaju
{
	std::stringstream test(k);
	std::string segment;
	std::vector<std::string> seglist;
	static int ro[105];                           //niz noda koje preseca
	ro[0]=-1;
	while(std::getline(test, segment, ' '))
	{
   		seglist.push_back(segment);
	}
	int g=0;
	
	for(int i=0; i<seglist.size(); i++)
	{
		if (t.find(seglist[i]) != std::string::npos)
		{
			if(i+1==seglist.size())
			{
				ro[g]=atoi(seglist[i].c_str());
    			g++;
    			ro[g]=-1;
    		}
    		else if(t.find(seglist[i+1]) != std::string::npos)
    		{
    			//:)
			}
			else
			{
				ro[g]=atoi(seglist[i].c_str());
    			g++;
    			ro[g]=-1;
			}
		}
	}
	
	return ro;
}

bool pronadji(int i, string k)      //pronalazi odredjenu nodu u stringu noda
{
	string s=covert_int_to_string(i);
	if (k.find(s) != std::string::npos)
	{
		return true;
	}
	return false;
}

int mini(int min, int u)            //odredjuje min
{
	if(min>u)
	{
		return u;
	}
	else 
	{
		return min;
	}
}

int IVTnum(int i,int j,string rset[])                              //racuna vreme koje je potrebno i j koristeci set ruta r
{
	int odradjen[100];                                             //niz koriscenih ruta
	int min=INT_MAX;                                               //ako ostane INT_MAX nije povezan sa j
	int ukupno;                                                    //ukupno vreme
	
	for(int no=0; no<brute; no++)                                  //postavi niz na nulu
	{
		odradjen[no]=0; 
	}
	
	for(int p=0; p<brute ; p++)
	{ 
		if(pronadji(i,rset[p])==true)                              //pronadji pocetnu nodu u ruti
		{
			ukupno=0;
			odradjen[p]=1;                                         //postavi je na predjenu
			if(pronadji(j,rset[p])==true)                          //da li je u njoj j kraj
			{
					ukupno+=Tij[i][j];                              
					min=mini(min,ukupno);
					ukupno-=Tij[i][j];
			}
			else
			{	
				for(int o=0; o<brute ; o++)                          //gleda da li ima preklapanja sa nekom rutom
				{
					if(odradjen[o]!=1)                               //da li je vec iskoriscen
					{
						int *jol;
						jol=preklapanje(rset[p],rset[o]);
						for(int ko=0; *(jol+ko)!=-1; ko++)
						{
							odradjen[o]=1;
							ukupno+=Tij[i][*(jol+ko)]+U;
							if(pronadji(j,rset[o])==true)             //da li je u njoj i kraj
							{
								ukupno+=Tij[*(jol+ko)][j];                 
								min=mini(min,ukupno);
								ukupno-=Tij[*(jol+ko)][j];
							}
							else
							{	
								for(int c=0; c<brute ; c++)              //gleda da li ima preklapanja sa nekom rutom
								{
									if(odradjen[c]!=1)                   //da li je vec iskoriscen
									{
										int *pol;
										pol=preklapanje(rset[o],rset[c]);
										for(int ho=0;*(pol+ho)!=-1; ho++ )
										{
											ukupno+=Tij[*(jol+ko)][*(pol+ho)]+U;
											if(pronadji(j,rset[c])==true)         //da li je u njoj kraj
											{
												ukupno+=Tij[*(pol+ho)][j];
												min=mini(min,ukupno);
												ukupno-=Tij[*(pol+ho)][j];
											}
											ukupno-=(Tij[*(jol+ko)][*(pol+ho)]+U);
										}
									}
								}
							}
							odradjen[o]=0;
							ukupno-=(Tij[i][*(jol+ko)]+U); 
						}
					}
				}
			}
			odradjen[p]=0;	//vrati na nulu ako je zavrsen rad sa tom rutom
		}
	}
	if(min==INT_MAX)
	{
		return 0;
	}
	else 
	{
		return min;
	}
}

void generate_IVT(string rset[])  //generisanje x za F1
{
	int k=1;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n; j++)
		{
			IVT[i][j]=IVTnum(i,j,rset);
			//cout<<IVT[i][j]<<" ";
			IVT[i][j]-=(float)Tij[i][j];
		}
		k++;
		//cout<<endl;
	}
}

float fij(int i, int j,float beta) //racuna fij za F1 (pogledaj skriptu)
{
	float k=beta/xm;
	float l=K/(xm*xm);
	return -(k+l)*IVT[i][j]*IVT[i][j]+ beta*IVT[i][j]+K;
}

float F1()                           //1 od 3
{
	float beta=beta1_generator();
	float u1=0;
	float u2=0;
	int k=1;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n ; j++)
		{
			if(IVT[i][j]>=0 && IVT[i][j]<=xm)
			{
				u1+=((float)need[i][j])*fij(i,j,beta);
				u2+=(float)need[i][j];
			}
		}
		k++;
	}
	return u1/u2;
}

float F2()                           //2 od 3
{
	float beta=beta1_generator();
	int k=1;
	float demand=0;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n ; j++)
		{
			if(IVT[i][j]>=0)
			{
				demand+=(float)need[i][j];
			}
		}
		k++;
	}
	float oj=((K-beta*af2)/(af2*af2));
	oj=oj*(demand*demand)+beta*demand;
	return oj;
}

float F3()                           //3 od 3
{
	float beta=beta1_generator();
	int k=1;
	float demand=0;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n ; j++)
		{
			if(IVT[i][j]<0)
			{
				demand+=(float)need[i][j];
			}
		}
		k++;
	}
	float oj=-((K+beta)*(demand*demand));
	oj=oj+beta*demand+K;
	return oj;
}

float F()
{
	return F1()+F2()+F3();
}

bool sorti (int i,int j) 
{ return (i>j); }

void filter()   //filter od matrice skidamo preklapanje :)
{
	int k=1;
	for(int p=0; p<5; p++)
{
	for(int pov=k; pov<5 ; pov++)
	{
		if(rute[p][pov]!="t")
		{
		int h=1;
		for(int i=0; i<5; i++)
		{
			for(int j=h; j<5 ; j++)
			{
				if((i!=p || j!=pov) && rute[i][j]!="t")
				{
				//cout<<i<<" "<<j<<" "<<endl;
    				if (rute[i][j].find(rute[p][pov]) != std::string::npos)
					{
    					//std::cout << "found!"<<i<<" "<<j<<endl;
    					rute[p][pov]="t"; break;
					}
				}
			}
			h++;
		}
		}
	}
	k++;
}
}

int convert()   //prebacuje maricu ruta u niz bez "t"
{
	int k=1;
	int y=0;
	for(int i=0; i<n; i++)
	{
		for(int j=k ; j<n; j++)
		{
			if(rute[i][j]!="t")
			{
				rfilter[y]=rute[i][j];
				y++;
			}
		}
		k++;
	}
	return y;
}

string crossover_s(string a, string b,int p)
{
	std::stringstream test1(a);
	std::string segment;
	std::vector<std::string> seglist;
	while(std::getline(test1, segment, ' '))
	{
   		seglist.push_back(segment);
	}
	std::stringstream test2(b);
	std::vector<std::string> seglist1;
	while(std::getline(test2, segment, ' '))
	{
   		seglist1.push_back(segment);
	}
	int ind=0; string s;
	for(int i=0; i<seglist.size(); i++)
	{
		for(int j=0; j<seglist1.size(); j++)
		{
			if(seglist[i]==seglist1[j] && atoi(seglist[i].c_str())==p)
			{
				 int k;
				for(int lo=0; lo<=j; lo++)
				{
					s+=seglist1[lo];
				}
				for(int lo=i+1; lo<seglist.size(); lo++)
				{
					s+=seglist[lo];
				}
				break; ind =1;
			}
			if(ind==1)
			{
				break;
			}
		}
		if(ind==1)
		{
			break;
		}
	}
	return s;
}

bool moze(string ga[500][500],int i)				//da li moze da se izvrsi instra cross over na odredjeni set
{
			int *g;
			for(int j=0; j<brute; j++)      
			{
				for(int ho=0; ho<brute; ho++)
				{
					if(ho!=j)
					{
   						g=preklapanje(ga[i][j],ga[i][ho]);
						if(*g!=-1)
						{
							return true;
						}
					}
				}
			}
			return false;
}

main()
{   string rset[500];
	float popularity[10000];
	srand(time(0));
	scanf("%d",&n);
	generate_time();
	generate_need();
	for(int i=0; i<n-1 ;i++) //pokretanje dijkstre za const
	{
    	dijkstra(time1, i,n);
	}	
	
	filter();
	int br=convert();   
	
	string ga[500][500];
	int t;
    int g;
    int ind;
	int mesta[br];
	
	for(int i=0; i<br; i++)
	{
		mesta[i]=i;
	}

	for(int i=0; i<pop; i++)                  //generisemo pocetne route setove
	{
		for(int j=0; j<brute; j++)
		{
   			t=rand()%(br-j);
   			g=mesta[t];
   			mesta[t]=mesta[br-1-j];
   			mesta[br-1-j]=g;
   			ga[i][j]=rfilter[g];
   			g=0; t=0;
		}
	}

	for(int fo=0; fo<generacion; fo++)       //geneticki
	{
		for(int i=0; i<pop; i=i+2)
		{
			for(int j=0; j<brute; j++)      //inter-string crossover sa koef 0.5
			{
				if(rand()%2==0)
				{
					string pomocni=ga[i][j];
					ga[i][j]=ga[i+1][j];
					ga[i+1][j]=pomocni;
				}
			}
		}
		cout<<"J";
		for(int moe=2; moe<=m; moe++)					//prosiruje matricu ga na N*m
		{
			for(int i=0; i<pop; i++)                  //generisemo pocetne route setove
			{
				for(int j=0; j<brute; j++)
				{
					ga[i*moe][j]=ga[i][j];
				}
			}
		}
		cout<<"o";
		for(int i=0; i<brute; i++)
		{
			mesta[i]=i;
		}
		
		for(int i=0; i<pop*m; i++)					//inter-string crossover sa koef 0.5
		{	
			if(moze(ga,i))
			{
					for(int plo=0; plo<2; plo++)
  					{
   						t=rand()%(brute-plo);
   						g=mesta[t];
   						mesta[t]=mesta[brute-1-plo];
   						mesta[brute-1-plo]=g;
   					}
   					int *lol;
   					lol=preklapanje(ga[i][mesta[brute-1]],ga[i][mesta[brute-2]]);
   					while(*lol==-1)
   					{
   						for(int plo=0; plo<2; plo++)
  						{
   							t=rand()%(brute-plo);
   							g=mesta[t];
   							mesta[t]=mesta[brute-1-plo];
   							mesta[brute-1-plo]=g;
   						}
							lol=preklapanje(ga[i][mesta[brute-1]],ga[i][mesta[brute-2]]);
					}
						int kol;
						for(kol=0; *(lol+kol)!=-1; kol++);
						int ran=rand()%kol;
						ran=*(lol+ran);
						string koj;
						koj=ga[i][mesta[brute-1]];
						cout<<koj<<"| "<<ga[i][mesta[brute-2]]<<" "<<rand<<" "<<kol;
						ga[i][mesta[brute-1]]=crossover_s(koj,ga[i][mesta[brute-2]],ran);
						ga[i][mesta[brute-2]]=crossover_s(ga[i][mesta[brute-2]],koj,ran);
   					
   				
			}
		}
		cout<<"v";
		for(int i=0; i<pop*m; i++)
		{
			for(int j=0; j<brute; j++)      		//inter-string crossover sa koef 0.5
			{
				rset[j]=ga[i][j];
			}
			generate_IVT(rset);
			
			popularity[i]=F();
		}
	}
	
	//do ovde je za IRSA
}



