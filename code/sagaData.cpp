#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <map>
#include <fstream>
#include <ctime> 

using namespace std;
#define match 3
#define mismatch 1
#define gap 0
#define N 4//tamaño de la poblacion
#define prob_cruza 0.9//probabilidad de Cruzamiento
#define prob_muta 0.2//probabilidad de mutacion

 
unsigned t0, t1;

double random_0_1(){//numeros aleatorios entre -1 y 1
	return (rand() / (double) RAND_MAX);
}

string mutar_uno(string b, int posi){
	string cad =b;
	
	if(cad[posi]=='0')
		cad[posi]='1';
	else 
		cad[posi]='0';
	
	//cout<<endl<<"****:  "<<posi<<" : "<<cad<<endl;
	return cad;
}


int ruleta(vector<double>&v, double aleatorio){
	
	double ini,fin;
	int posi=0;
	ini=0.0;
	fin =v[0];
	int i=0;
	while(aleatorio <= 1.0){
		if(ini< aleatorio && aleatorio <= fin){
			posi =i;
			//cout<<"\n*********ENTRO: "<<posi<<endl;
			//cout<<ini<<" < "<<aleatorio<<" && "<<aleatorio<<" <= "<<fin<<endl;
			break;
		}
		else{
			ini =fin;
			fin +=v[i+1];
			i++;
		}
	}
	return posi;

}

string quitar_space_cad(const string &cadena){
	string cad =cadena;
	int i=0;
	while(i < cad.size() ){
		if(cad[i]=='-')
			cad.erase(cad.begin() + i);
		else i++;
	}
	//cout<<"\nERROR\n";
	return cad;
}

void space_random(string &cad){
	int tam =cad.size();
	int num =rand() % tam;
	//cout<<"\nRandon: "<<num<<endl;
	string tmp="";
	for (int i = 0; i < cad.size(); ++i){
		if(i==num)
			tmp += "-";
		tmp += cad[i];

	}
	cad =tmp;

}
void llenar_space_cad(string &cad, int tam){
	while(cad.size() < tam){
			space_random(cad);
		}
}
void llenar_space(vector<string> &v, int tam ){
	for (int i = 0; i < v.size(); ++i){
		llenar_space_cad(v[i],tam);
	}
	/*
	//matriz vectorizado
	for (int i = 0; i < v.size(); ++i){
		cout<<endl<<v[i].size()<<" : "<<v[i]<<endl;
	}
	*/
}

void mostrar_matriz(std::vector<string> v){
	//matriz vectorizado
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i].size()<<" : "<<v[i]<<endl;
	}
}

int peso(char c1, char c2){
	if(c1 == c2 && c1 == '-')
		return 0;
	else if(c1==c2)
		return match;
	else{
		if(c1=='-')
			return gap;
		if(c2=='-')
			return gap;
		else return mismatch;
	}

}

int aptitud(vector<string> &v){
	int tmp=0;
	for (int i = 0; i < v.size(); ++i){
		for (int j = i+1; j <v.size() ; ++j){
			for (int k = 0; k <v[0].size() ; ++k){
				tmp += peso(v[i][k], v[j][k]);
			}
			
		}
	}
	return tmp;
}

string copiar(string cad, int n_ini, int n_fin ){
	string tmp="";
	bool entrar =false;
	for (int i = n_ini; i <= n_fin; ++i){
		/*if(cad[i]!='-')
			entrar=true;
		if(entrar)
			tmp += cad[i];
		*/
		tmp += cad[i];
	}
	return tmp;
}
void regular_tamanios(string &cad1,string &cad2, int tam){
	int aleatorio=0;
	//para la cadena 1
	while(cad1.size() > tam ){
		aleatorio =rand() % cad1.size();
		if(cad1[aleatorio]=='-')
			cad1.erase(cad1.begin() + aleatorio);
	}
	// cout<<"\n**********: "<<cad1<<endl;
	//para la cadena 2
	while(cad2.size() > tam ){
		aleatorio =rand() % cad2.size();
		if(cad2[aleatorio]=='-')
			cad2.erase(cad2.begin() + aleatorio);
	}
	llenar_space_cad(cad1,tam);
	llenar_space_cad(cad2,tam);
}


pair<string, string > cruzamiento_punto(string &cad1, string &cad2, int random){
	pair<string, string> par;
	string tmp1_ini =copiar(cad1,0, random);//mitad izquierdo
	string tmp1_fin =copiar(cad1,random,cad1.size());//mitad derecho
	// cout<<"\t 1ini: "<<tmp1_ini<<endl;
	// cout<<"\t 1fin: "<<tmp1_fin<<endl;
	vector<char> v;
	for (int i = 0; i < random; ++i){
		if(cad1[i]!='-')
			v.push_back(cad1[i]);
	}
	// cout<<"\n*********";
	// for (int i = 0; i < v.size(); ++i){
	// 	cout<<v[i];
	// }
	// cout<<endl;
	int i=0;
	int posi=0;
	int j=0;
	while( j < v.size()){
		if(cad2[i]==v[j]){
			posi =i;
			j++;
		}
		i++;
	}

	// cout<<"\nPOSI: "<<posi<<endl;
	string tmp2_ini =copiar(cad2,0, posi);//mitad izquierdo
	string tmp2_fin =copiar(cad2,posi+1,cad1.size());//mitad derecho
	// cout<<"\t 2ini: "<<tmp2_ini<<endl;
	// cout<<"\t 2fin: "<<tmp2_fin<<endl;
	string res_cad1 =tmp1_ini + tmp2_fin;
	string res_cad2 =tmp2_ini + tmp1_fin;
	
	regular_tamanios(res_cad1,res_cad2, cad1.size());	
	par =make_pair(res_cad1,res_cad2);
	return par;
}

void ordenar_todo(vector<vector<string> >&n_data){
	//ordenamos todo
	
	vector<vector<string> > n_data_temp;//=  n_data;
	double suma=0.0;
	for (int i = 0; i < n_data.size(); ++i){
		suma +=aptitud(n_data[i]);
	}

	multimap<double, int > mymap;
	for (int i = 0; i < n_data.size(); ++i){
		mymap.insert(pair<double,int>(1.0*aptitud(n_data[i])/suma, i));
	}

	
	std::map<double, int>::iterator it;
	int i=0;
	for (it=mymap.begin(); it!=mymap.end(); ++it){
    	// std::cout << it->first << " => " << it->second << '\n';
    	if(mymap.size()/2 >i)
    		n_data_temp.push_back(n_data[it->second]);
    	i++;
	}
	n_data.clear();
	n_data = n_data_temp;	

}


void get_data(string archivo, vector<string> &v)
{
	ifstream file(archivo.c_str());
	char c;
	string cad = "";
	std::vector<string> nucleotidos = {"A", "G", "C", "T"};
	getline(file, cad);
	cad = "";
	while (!file.eof())
	{
		file >> c;
		if (c == 'N')
		{
			//cout<<"entrooo";
			cad += nucleotidos[rand() % 4];
		}
		else if (c == '-')
			break;
		else
		{
			cad += c;
			c = '-';
		}
	}
	v.push_back(cad);
	file.close();
}
int main(int argc, char const *argv[])
{
	
	vector<string> data;

	std::vector<string> data_ADN;
	data_ADN.push_back("data/AB007372.1.fasta");
	data_ADN.push_back("data/AF008136.1.fasta");
	data_ADN.push_back("data/AF115687.1.fasta");
	data_ADN.push_back("data/AF338803.1.fasta");
	data_ADN.push_back("data/AY172676.1.fasta");
	// data_ADN.push_back("data/AY570178.1.fasta");
	data_ADN.push_back("data/AY916804.1.fasta");
	// data_ADN.push_back("data/FJ978005.1.fasta");
	// data_ADN.push_back("data/JN182019.1.fasta");
	// data_ADN.push_back("data/KF723519.1.fasta");
	// data_ADN.push_back("data/KJ195895.1.fasta");
	for (int i = 0; i < data_ADN.size(); ++i){
		get_data(data_ADN[i],data);
	}
	int max = data[0].size();
	for (int i = 0; i < data.size(); ++i){
		if(max< data[i].size() ){
			max =data[i].size(); 
		}
		// cout<<data[i].size()<<endl;
	}
	// cout<< "Longitud de maxima cadena: "<< max <<endl;
	// data.push_back("AGTCATTAATTGCGGTTAG");
	// data.push_back("CCAATTGTAGTT");
	// data.push_back("AGCATTCGTAGTT");
	// data.push_back("GTTCAAGGGAGTT");
	// int tam_max =22;
	int tam_max = max + 10;
	vector<string> data_ini;
	vector<vector<string> > n_data;
	std::vector<double> v_ruleta;
	//saga de Codificación
	for (int i = 0; i <N ; ++i){
		data_ini =data;
		llenar_space(data_ini,tam_max);
		n_data.push_back(data_ini);
	}
	//SAGA- Aptitud
	double suma=0.0;
	for (int i = 0; i < n_data.size(); ++i){
		//cout<<"\n"<<i<<": \n";
		//mostrar_matriz(n_data[i]);
		//cout<<"\tSuma Pares: "<<aptitud(n_data[i])<<endl;
		suma +=aptitud(n_data[i]);
	}
	// cout<<"\nMostrar toda la poblacion \n";
	for (int i = 0; i < n_data.size(); ++i){
		// cout<<"\n"<<i<<": \n";
		// mostrar_matriz(n_data[i]);
		// cout<<"\tSuma Pares: "<<aptitud(n_data[i])<<endl;
		// cout<<"\tPorcentaje: "<<1.0*aptitud(n_data[i])/suma<<endl;
		v_ruleta.push_back(1.0*aptitud(n_data[i])/suma);
	}
	// printf("\n******************************\n");
	
	
	//SAGA - Cruzamiento
	vector<string> v_padre;
	vector<string> v_madre;
	vector<string> v_padre_hijos;
	vector<string> v_madre_hijos;
	int itera =0;  
	t0=clock();
	while(itera <= 100){
		// cout<<"\n**************************ITERACION: "<<itera<<endl;
		for (int i = 0; i < N/2; ++i)
		{
			
			double n_ramdon =random_0_1();
			int padre_posi = ruleta(v_ruleta,n_ramdon);
			// cout<<"\tPadre: "<<n_ramdon<<" -> "<<padre_posi<<endl;
			int madre_posi =padre_posi;
			//buscaremos una posicion diferente al padre
			while(madre_posi == padre_posi){
				n_ramdon =random_0_1();
				madre_posi =ruleta(v_ruleta,n_ramdon);
				if(madre_posi != padre_posi)
					break;
			}
			// cout<<"\tMadre: "<<n_ramdon<<" -> "<<madre_posi<<endl;

			//saga de cruzamiento
			v_padre =n_data[padre_posi];
			v_madre =n_data[madre_posi];
			int ramdon_i;
			double ramdon_cruz,ramdon_muta;
			pair<string,string> hijos;
			for (int i = 0; i < v_padre.size(); ++i){
				ramdon_cruz = random_0_1();
				if( prob_cruza <= ramdon_cruz){
					ramdon_i =rand() % v_padre[i].size();
					// cout<<"\nCruzamiento\n";
					hijos =cruzamiento_punto(v_padre[i],v_madre[i],ramdon_i);
					v_padre_hijos.push_back(hijos.first);
					v_madre_hijos.push_back(hijos.second);	

				}
				else{
					v_padre_hijos.push_back(v_padre[i]);
					v_madre_hijos.push_back(v_madre[i]);
				} 
			}
			// cout<<"\nHIJOS Padre sin mutar: \n";
			// mostrar_matriz(v_padre_hijos);
			
			// cout<<"\nHIJOS Madre sin mutar: \n";
			// mostrar_matriz(v_madre_hijos);
			//padre
			ramdon_muta = random_0_1();
			if( prob_muta <= ramdon_muta ){
				// cout<<"\tMuta hijo de padre\n";
				int ramdon_hijo = rand() % v_padre.size();
				int tamanio =v_padre[ramdon_hijo].size();
				v_padre_hijos[ramdon_hijo] =quitar_space_cad(v_padre_hijos[ramdon_hijo]);
				llenar_space_cad(v_padre_hijos[ramdon_hijo],tamanio );
			}
			//madre
			ramdon_muta = random_0_1();
			if( prob_muta <= ramdon_muta ){
				// cout<<"\tMuta hijo de madre\n";
				int ramdon_hijo = rand() % v_madre.size();
				int tamanio =v_madre[ramdon_hijo].size();
				v_madre_hijos[ramdon_hijo] =quitar_space_cad(v_madre_hijos[ramdon_hijo]);
				llenar_space_cad(v_madre_hijos[ramdon_hijo],tamanio );
			}
			//agregamos a los hijos
			n_data.push_back(v_padre_hijos);
			n_data.push_back(v_madre_hijos);
		/*	cout<<"\nPadre: \n";
			mostrar_matriz(n_data[padre_posi]);
			
			cout<<"\nMadre: \n";
			mostrar_matriz(n_data[madre_posi]);
			//cout<<"\n***********************************************\n";
			cout<<"\nHIJOS Padre: \n";
			mostrar_matriz(v_padre_hijos);
			
			cout<<"\nHIJOS Madre: \n";
			mostrar_matriz(v_madre_hijos);
			*/

			//limpiamos
			
			v_padre.clear();
			v_madre.clear();
			v_padre_hijos.clear();
			v_madre_hijos.clear();
		}
		ordenar_todo(n_data);
		// cout<<"\nMostrar toda la poblacion \n";
		for (int i = 0; i < n_data.size(); ++i){
			// cout<<"\n"<<i<<": \n";
			// mostrar_matriz(n_data[i]);
			// cout<<"\tSuma Pares: "<<aptitud(n_data[i])<<endl;
			// cout<<"\tPorcentaje: "<<1.0*aptitud(n_data[i])/suma<<endl;
			v_ruleta.push_back(1.0*aptitud(n_data[i])/suma);
		}
		// printf("\n******************************\n");
	
		// cout<<"\nCantidad: "<<n_data.size()<<endl;	
			itera++;

	}
	
t1 = clock();

double time = (double(t1-t0)/CLOCKS_PER_SEC);
cout << "Execution Time: " << time << endl;









	//cout<<"\n****: \t"<<quitar_space_cad("T---TA--G")<<endl;


/*
	//string cad1 ="AG-TCATTAATT-GCGG-TTAG";
	//string cad2 ="AGTCAT-TAATT-GC-GGTTAG";
	string cad1 ="C----C-A--A-TTGTAG--TT";
	string cad2 ="C--CA--AT-----T-GTAGTT";
	
	cout<<"\t"<<cad1<<"\n\t"<<cad2<<endl;
	int cad1_i = 9;
	cad1_i =rand() % cad1.size();
	pair<string,string> hijos =cruzamiento_punto(cad1,cad2,cad1_i);
	cout<<"Hijo 1  "<<hijos.first<<endl;
	cout<<"Hijo 2  "<<hijos.second<<endl;
*/
	/*
	data_ini.push_back("AAC-AC");
	data_ini.push_back("AATT-C");
	int tmp=0;
	for (int i = 0; i < data_ini[0].size(); ++i){
		tmp +=peso( data_ini[0][i],data_ini[1][i],match,mismatch,gap);
		cout<<endl<<tmp<<endl;
	}

	cout<<"\nTMP: "<<tmp<<endl;
	*/
	//space_random(data[1]);
	//llenar_space(data, tam_max);
	//data_ini.push_back("--AGTCATTAAT-TGCGGTTAG");
	//data_ini.push_back("C-CAA-TT-G-T-----AGT-T");
	//data_ini.push_back("A-G--CATT-CG-T-A-G-T-T");
	//data_ini.push_back("GT-TCA-AG-GGA---G---TT");
	//data_ini.push_back("AAC-AC");
	//data_ini.push_back("AATT-C");
	//data_ini.push_back("A--T-C");
	//cout<<"\n::::::"<<aptitud(data_ini);



	cout<<"\nGOOD\n";
	return 0;
}




