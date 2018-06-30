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
#include <iterator>

using namespace std;

int Tij[100][100];           //najbrzi put izmedju i->j
float IVT[100][100];         //x
int time1[100][100];         //vreme izmedju svake dir povezane rute
int need[100][100];          //need izmedju i j
string rute[100][100];       //sve rute izmedju dve (t znaci da ne postoji ruta izmedju njih !!!)
string rfilter[10000];       //niz  
int ruta[100];               //ness pomocno
int n;  int duzina;          //n-broj noda duzina-ness pomocno
int K=10; int xm=35;         //K,xm-const          menjam po slucaju!!!
int brute=4;                 //broj ruta u grafu   menjam po slucaju!!!
float af2=1;                 //konstanta za F2
int U=5;                     //penalty
int pop=20;         	    //populacija koju koriatim
int generacion=500;		//broj generacija
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
	float i = 0, d = 0, j=0;
    i = rand() %(K+1); 
    j=rand()% xm+1;
    d = -i / j; 
    return d;
}

float beta2_generator()  //generise beta1
{
	float i = 0;
    i = rand() %K+(2*K); 
    return i; 
}

float beta3_generator()  //generise beta1
{
	float i = 0, d = 0, j=0;
    i = rand() %K; 
    return -i;
}

void generate_need()                //generise potrebu izmedju i j
{
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			need[i][j]=rand() % 10+1;
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
	beta=1;
	float k=beta/xm;
	float l=K/(xm*xm);
	return -(k+l)*IVT[i][j]*IVT[i][j]+ beta*IVT[i][j]+K;
}

float F1()                           //1 od 3
{
	float beta=beta1_generator();
	beta=1;
	float u1=0;
	float u2=0;
	int k=1;
	for(int i=0; i<n; i++)
	{
		for(int j=k; j<n ; j++)
		{
			if(IVT[i][j]>=0 && IVT[i][j]<=xm)
			{
				u1+=(need[i][j])*fij(i,j,beta);
				u2+=need[i][j];
			}
		}
		k++;
	}
	if(u2!=0)
	{
		return u1/u2;
	}
	else return 0;
}

float F2()                           //2 od 3
{
	float beta=beta2_generator();
	beta=1;
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
	float beta=beta3_generator();
	beta=1;
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
	return 20*F1()+5*F2()+F3();
}

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

void podeli(vector<string>& seglist,string a)
{
	std::stringstream test1(a);
	std::string segment;
	while(std::getline(test1, segment, ' '))
	{
   		seglist.push_back(segment);
	}
}

int lociraj(vector<string> seglist,string p)
{
	for(int i=0; i<seglist.size(); i++)
	{
		if(seglist[i]==p)
		{
			return i;
		}
	}
}

string crossover_s(string a, string b,int p)
{
	std::vector<std::string> seglist;
	std::vector<std::string> seglist1;
	podeli(seglist,a);
	podeli(seglist1,b);
	string s="";
	int c; int d; 
	string inti;
	inti=covert_int_to_string(p);
	c=lociraj(seglist,inti);
	d=lociraj(seglist1,inti);
	if(c!=0 && d+1!=seglist1.size())
	{
		for(int i=0; i<c; i++)
		{
			s+=seglist[i]+" ";
		}
		if(d+1==seglist1.size())
		{
			s+=inti;
		}
		else
		{
			s+=inti+" ";
			for(int i=d+1; i<seglist1.size(); i++)
			{

				if(i+1!=seglist1.size())
				{
					s+=seglist1[i]+" ";
				}
				else 
				{
					s+=seglist1[i];
				}
			}
		}
	return s;
	}
	return a;
}

bool moze(string ga[700][100],int i)				//da li moze da se izvrsi instra cross over na odredjeni set
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

void generisi_B(vector<float>& B)
{
	for (int i = 0; i < pop*m; i++)
    {
        B[i] = i;
    }
}

void generisi_A(vector<float>& B, float popi[])
{
	for (int i = 0; i < pop*m; ++i)
    {
        B[i] = popi[i];
    }
}

bool iste_rute(float i, int j,string ga[700][100])			//gldea da li su iste rute neke dve iz ganeticke matrice
{
	int joca=0; 
	
	for(int ho=0; ho<brute; ho++)
	{
		for(int l=0; l<brute; l++)
		{
			if(ga[(int)i][ho]==ga[j][l])
			{
				joca++;
				l=brute;
			}
			
		}
	}
	if(joca==brute)
	{
		return true;
	}	
	return false;
}

bool vec_se_nalazi(vector<float> B, int g, string ga[700][100], int i)
{
	for(int j=0; j<g; j++)
	{
			if(iste_rute(B[j],i,ga) || B[j]==i)
			{
				return true;
			}
	}
	
	return false;
}

void filter2(string ga[700][100], vector<float> glow,vector<float>& B,vector<float>& C)
{
	int g=0;
	vector<float> D(500);
	for(int i=0; i<pop*m; i++)
	{
		C[i]=B[i];
	}
	B[0]=glow[pop*m-1];
	D[0]=glow[pop*m-1];
	g++;
	for(int i=pop*m-2; i>=0; i--)
	{
		if(vec_se_nalazi(D,g,ga,glow[i])==false)
		{
			B[g]=glow[i];
			D[g]=glow[i];
			g++;
		}
	}
	if(B[pop]<0)
	{
		cout<<"JOJ";
		for(int i=0; i<pop; i++)
		{
			if(B[i]<0)
			{
				B[i]=rand()% pop*m;
			}
		}
	}
}

bool ima_ponavljanja(string a)
{
	std::vector<std::string> seglist;
	podeli(seglist,a);
	for(int i=0; i<seglist.size(); i++)
	{
		for(int j=0; j<seglist.size(); j++)
		{
			if(i!=j && seglist[i]==seglist[j])
			{
				return true;
			}
		}
	}
	return false;
}

main()
{   string rset[500];
	vector<float> A(500), B(500); 
	vector<float> C(500);
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
	
	string ga[700][100];
	string gapom[pop][brute];
	int t;
    int g;
    int ind;
	int mesta[br];
	float satis;
	
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
		//cout<<fo<<" ";
		for(int i=0; i<pop; i=i+2)
		{
			for(int j=0; j<brute; j++)      //inter-string crossover sa koef 0.5
			{
				if(rand()%2==0)
				{
					if(i+1!=pop)
					{
						string pomocni=ga[i][j];
						ga[i][j]=ga[i+1][j];
						ga[i+1][j]=pomocni;
					}
					else
					{
						string pomocni=ga[i][j];
						ga[i][j]=ga[i-1][j];
						ga[i-1][j]=pomocni;
					}
				}
			}
		}
	
		for(int i=0; i<br; i++)					//refresh za mesta
		{
			mesta[i]=i;
		}
	
		for(int moe=1; moe<m; moe++)				//uvecaj matricu
		{				
			for(int i=0; i<pop; i++)                 
			{
				for(int j=0; j<brute; j++)
				{
   					t=rand()%(br-j);
   					g=mesta[t];
   					mesta[t]=mesta[br-1-j];
   					mesta[br-1-j]=g;
   					ga[pop*moe+i][j]=rfilter[g];
   					g=0; t=0;
   					//ga[pop*moe+i][j]=ga[i][j];
				}			
			}
		}
		
		for(int i=0; i<brute; i++)				//refesh za mesta
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
   					while(*lol==-1 )
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
						//cout<<koj<<"| "<<ga[i][mesta[brute-2]]<<" "<<ran<<endl;
						ga[i][mesta[brute-1]]=crossover_s(koj,ga[i][mesta[brute-2]],ran);
						ga[i][mesta[brute-2]]=crossover_s(ga[i][mesta[brute-2]],koj,ran);
			}
		}
		
		for(int i=0; i<br; i++)					//refresh za mesta
		{
			mesta[i]=i;
		}
		
		for(int i=0; i<pop*m; i++)				//unistavanje ponavljanja
		{
			for(int j=0; j<brute; j++)
			{
				if(ima_ponavljanja(ga[i][j]))
				{
					t=rand()%(br-j);
   					g=mesta[t];
   					mesta[t]=mesta[br-1-j];
   					mesta[br-1-j]=g;
   					ga[i][j]=rfilter[g];
   					g=0; t=0;
				}
			}
		}

		for(int i=0; i<pop*m; i++)
		{
			for(int j=0; j<brute; j++)      		//inter-string crossover sa koef 0.5
			{
				rset[j]=ga[i][j];
			}
			generate_IVT(rset);
			popularity[i]=F()/100;
		}
		
	/*	for(int i=0; i<pop*m; i++)
		{
			for(int j=0; j<brute; j++)      		
			{
				cout<<ga[i][j]<<"| ";
			}
			cout<<popularity[i]<<endl;
		}
		cout<<endl;*/

		generisi_B(B);
		generisi_A(A,popularity);

   		for (int i = 0; i < pop*m-1; i++)      
		{	     
       		for (int j = 0; j < pop*m-i-1; j++) 
       		{
           		if (A[j] > A[j+1])
           		{
           			int pom;
           			pom=A[j];
           			A[j]=A[j+1];
           			A[j+1]=pom;
           			pom=B[j];
           			B[j]=B[j+1];
           			B[j+1]=pom;
				}
     		}       
		}	

	//	cout<<endl;
		filter2(ga,B,A,C);	
	
	/*	for(int i=0; i<pop; i++)
		{
			cout<<A[i]<<" ";
		}
		cout<<endl;*/
	
		for(int i=0; i<pop; i++)
		{
			for(int j=0; j<brute; j++)
			{
				gapom[i][j]=ga[(int)A[i]][j];
			}
		}
		
		for(int i=0; i<pop; i++)	
		{
			for(int j=0; j<brute; j++)
			{
				ga[i][j]=gapom[i][j];	
			}
		}
		
		/*for(int i=0; i<pop; i++)
		{
			for(int j=0; j<brute; j++)      		
			{
				cout<<ga[i][j]<<"| ";
			}
			cout<<popularity[(int)A[i]]<<endl;
		}
		cout<<endl;*/
	}		
	for(int i=0; i<brute; i++)
	{
		cout<<ga[0][i]<<"| ";
	}
}



