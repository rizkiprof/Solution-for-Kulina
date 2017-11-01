#include<bits/stdc++.h>
using namespace std;

class Costumer {
	public:
		string name;		// Nama costumer
		double Latitude;	// Latitude
		double Longitude;	// Longitude
		double Order;   	// Order

        string GetName() {return name;}
        double GetLat() {return Latitude;}
        double GetLon() {return Longitude;}
        double GetOrder() {return Order;}

	void input(string s, double lon, double lat, int o){
		name = s;
		Latitude = lat;
		Longitude = lon;
		Order = o;
	}
};

class Dapur{
	public :
		string name;		// Nama costumer
		double Latitude;	// Latitude
		double Longitude;	// Longitude
		int Min_cap;   		// Minimal order
		int Max_cap;   		// Maxorder
		int Tolerance;

	string GetName() {return name;}
	double GetLat() {return Latitude;}
	double GetLon() {return Longitude;}
	double GetMin() {return Min_cap;}
	double GetMax() {return Max_cap;}

	void input(string s, double lon, double lat, int mi, int ma, int tol){
		name = s;
		Latitude = lat;
		Longitude = lon;
		Min_cap = mi;
		Max_cap = ma;
		Tolerance = tol;
	}
};

struct pairDisDa{
	double dist;
	int IDdapur;
};

struct pairPrioCostumerDapur{
	int Costumer,Dapur;
	double Prioritas;
};

	Costumer 				costumer[160];		 //Array Costumer
    Dapur 					dapur[10];			 //Array Dapur
	double 					JarakCosDa [160][10]; //Jarak Costumer ke semua dapur.
	pairDisDa 				sortedJarakCosDa [160][10];	//Jarak CosDa yang terurut dari terdekat, Array[costumer][dapur].Distance, Dapur
	pairPrioCostumerDapur 	BandingPrio[160];      //Membandingkan prioritas
	int 					cnt [10] = {0};			 //Banyak costumer yang terdekat dgn Dapur[i]
	bool 			DapurAvailable [10]={1,1,1,1,1,1,1,1,1,1};	//Kalo Masih bisa nilainya 1
	int 			used[10]={0};			//..berapa order yang di plot ke dapur[i]
    int             plotCostumer[160]={0};
    vector<int>     plotDapur[10];
	double			jarakCusCus[159][159];
	bool 			belumTerplot[159];
	double 			biayaAPI = 0;

bool cmp(pairPrioCostumerDapur i, pairPrioCostumerDapur j){
    return i.Prioritas < j.Prioritas;
}

//Input Costumer & Dapur
void Input(){
	string s; double la, lo; int ord, mi, ma, to;
    for( int i = 1; i <= 158; i++ ){
		cin >> s >> lo >> la >> ord;
        costumer[i].input(s, lo , la, ord);
//        cout<< costumer[i].GetName() <<" "<< costumer[i].GetLat() << " " << costumer[i].GetLon() << " " << costumer[i].GetOrder() << endl;
    }
    dapur[1].input ("Dapur Miji", 106.8146526, -6.1507346, 50, 100, 105);
    dapur[2].input ("Dapur Nusantara", 106.8347722, -6.2794885,	30, 40, 45 );
    dapur[3].input ("Familia Catering",	106.7938203, -6.1928965, 50, 60, 65);
    dapur[4].input ("Pondok Rawon",	106.8268222,	-6.2240943,	50,	70,	75);
    dapur[5].input ("Rose Catering", 106.7959925,	-6.1574728,	70, 80,	85);
    dapur[6].input ("Tiga Kitchen Catering", 106.8472257,	-6.1841235,	30,	50, 55);
    dapur[7].input ("Ummu Uwais", 106.9142139,	-6.2569113,	20,	50,	55);
}

//convert degree to radian
double Radians(double x){
  	const double PIx = 3.141592653589793;
    return x * PIx / 180;
}
//Convert jarak LatLong Ke Kilometer
double HitungDistance(double lon1, double lat1, double lon2, double lat2){
   	const double RADIUS = 6378.16;			//radius bumi

	double dlon = Radians(lon2 - lon1);
    double dlat = Radians(lat2 - lat1);

    double a = (sin(dlat / 2) * sin(dlat / 2)) + cos(Radians(lat1)) * cos(Radians(lat2)) * (sin(dlon / 2) * sin(dlon / 2));
    double angle = 2 * atan2(sqrt(a), sqrt(1 - a));
    return angle * RADIUS;
}
//Hitung JarakCostumerDapur
void HitungJarakCosDa(){
	for (int i = 1; i <= 158; i++){
		for (int j = 1; j <= 7; j++){
			JarakCosDa [i][j] = HitungDistance (costumer[i].GetLon(), costumer[i].GetLat(), dapur[j].GetLon(), dapur[j].GetLat());
		}
	}
	biayaAPI += 158*7;
}

void SortJarakCosDa(){
	for (int i = 1; i <= 158; i++) {
		vector<pair< double, int> > V;
		for (int j = 1; j <= 7; j++) {
			V.push_back ( make_pair( JarakCosDa [i][j] , j) );
		}
		sort(V.begin() , V.end());
		for (int j = 0; j < 7; j++) {
			sortedJarakCosDa[i][j+1].dist = V[j].first;
			sortedJarakCosDa[i][j+1].IDdapur = V[j].second;
		}
		cnt [sortedJarakCosDa[i][1].IDdapur]++;
		sortedJarakCosDa[i][8].dist = 50; 
		sortedJarakCosDa[i][8].IDdapur = 8;
	}
}

pair<int,int> findDapuruntukCostumer(int C){
    int i = 1, countt = 0, D1,D2;
    while(i <= 7){
        int D = sortedJarakCosDa[C][i].IDdapur;     // Dapur
        if(countt == 0){
            if( used[D] + costumer[C].GetOrder() <= dapur[D].GetMax() ){
                D1 = i;
                countt++;
            }
        }
        else if(countt == 1){
            if(used[D] + costumer[C].GetOrder() <= dapur[D].GetMax() ){
                D2 = i;
                countt++;
                break;
            }
        }
        i++;
    }
	if(i == 8) D2 = 8;
    return make_pair(D1,D2);
}

void PerbandinganPrioritas(){
	for(int i = 0 ; i <= 158; i++){
		BandingPrio[i].Prioritas  = 2;
		BandingPrio[i].Costumer = -1;
		BandingPrio[i].Dapur = -1;
	}
	for( int C = 1 ; C <= 158; C++){
        if( belumTerplot[C] ){
			pair<int,int> P = findDapuruntukCostumer(C);			//Dapat dapur terdekat ke1 dan ke2
			BandingPrio[C].Prioritas  = sortedJarakCosDa [C][P.first].dist / sortedJarakCosDa [C][P.second].dist ;
			BandingPrio[C].Costumer = C;
			BandingPrio[C].Dapur = sortedJarakCosDa[C][P.first].IDdapur;
		}
	}
	sort(BandingPrio+1, BandingPrio+158+1, cmp);
}

void TampilkanHasilPlot();

void PlottingCostumerToDapur (){
	int Sisa_costumer = 158;
	int iteras = 1;
	while(Sisa_costumer > 0){
//		cout<< "ITERASI #" <<iteras<<endl; iteras++;
		PerbandinganPrioritas();
//		TampilkanHasilBandingPrio2();
		for( int i = 1 ; i <= 158; i++){
			int C = BandingPrio[i].Costumer;
			if( belumTerplot[C] ){
				int D = BandingPrio[i].Dapur;
				if( used[D] + costumer[C].GetOrder() <= dapur[D].GetMax() ){
					used[D] += costumer[C].GetOrder();
					plotCostumer[C] = D;
					plotDapur[D].push_back(C);
					Sisa_costumer--;
					belumTerplot[C] = false;
				}
			}
		}
	}
}

void TampilkanHasilPlot(){
	cout<<"Dapur"<< setw(24) <<"MinOrder"<< setw(13)<<"Max Order "<< setw(13)<<"Order Masuk"<< setw(12)<<"Customer"<<endl;

    for(int i = 1; i<=7; i++){
        cout<< setw(21)<<dapur[i].name;
		cout<< setw(8)<< dapur[i].GetMin() << setw(9) <<dapur[i].GetMax() << setw(12) << used[i]<<setw(12)<<plotDapur[i].size()<<" \t\t: ";
		biayaAPI += pow(plotDapur[i].size(),2);
        for(int j = 0; j < plotDapur[i].size() ;j++)
            cout<<plotDapur[i][j]<<" ";
		cout<<endl;
    }
}

bool terKirim[160];				//customer done
vector <int> RuteDriver[100];
int DriverBox[100];
double Driverjarak[100];
int NoDriver = 0;

void HitungJarakCusCus(int k){
	for(int j = 0; j < plotDapur[k].size() - 1; j++){
		for( int i = j+1; i < plotDapur[k].size(); i++){
			int A = plotDapur[k][j];
			int B = plotDapur[k][i];
			double Jarak = HitungDistance ( costumer[A].GetLon(), costumer[A].GetLat(), costumer[B].GetLon(), costumer[B].GetLat());

			jarakCusCus [A][B] = Jarak;
			jarakCusCus [B][A] = Jarak;
		}
	}
}

int cari (int D, int now){
	int jarak = 1000, C;
	int terdekat = -1;
	for(int i = 0; i < plotDapur[D].size(); i++){
		C = plotDapur[D][i];
		if( !terKirim[C] && DriverBox[NoDriver] + costumer[C].GetOrder() <= 40){
			if( jarak > jarakCusCus[now][C]  ){
				jarak = jarakCusCus[now][C];
				terdekat = C;
			}
		}
	}
	return terdekat;
}

void masukin(int now, double jarak){
	terKirim[now] = 1;
	Driverjarak[NoDriver] += jarak;
	DriverBox[NoDriver] += costumer[now].GetOrder();
	RuteDriver[NoDriver].push_back(now);
}

void AssignDriver(){
	memset(jarakCusCus, 100000, sizeof(jarakCusCus) );
	memset(terKirim, 0 ,sizeof(terKirim));
	double totaljarak = 0;
	int waktu = 0;
	
	for(int i = 1; i <= 7; i++){
		HitungJarakCusCus(i);
		double jarakkk = 0;
		int totalcostumer = 0;
		while(totalcostumer < plotDapur[i].size()){
			//cari yg terdekat
			double JrkTerdkt = 100;
			int terdekat = -1;
			for(int j = 0; j < plotDapur[i].size(); j++){
				if( !terKirim[ plotDapur[i][j] ] ){
					if( JrkTerdkt > JarakCosDa[j][i] ){
						JrkTerdkt = JarakCosDa[j][i];
						terdekat = plotDapur[i][j];
					}
				}
			}
			//Driver start
			int now = terdekat;
			NoDriver++;
			masukin( now, JrkTerdkt);
			totalcostumer++;
			cout<<NoDriver<< "\t: ";
			cout<< now<<" ";
			
			totaljarak+= JrkTerdkt;
			jarakkk += JrkTerdkt;
			
			//cari yang selanjutnya
			while(DriverBox[NoDriver] < 40 && totalcostumer < plotDapur[i].size()){
				int next = cari(i, now);
				cout<< next<<" ";
				masukin ( next, jarakCusCus[now][next]);	
				
				totaljarak += jarakCusCus[now][next];
				jarakkk += jarakCusCus[now][next];
				
				now = next;
				totalcostumer++;
			}
			cout<<setw(40)<<"\t\t jarak tempuh : "<< jarakkk;
			waktu += ceil(jarakkk/20);
			cout<<"\t waktu : "<< ceil(jarakkk / 20) << " jam";
			cout<< endl;
		}
	}
	cout<< endl<<endl<<"TOTAL JARAK: "<<totaljarak<<" Km"<<endl;
	cout<< "TOTAL WAKTU: "<<waktu<<" jam"<<endl;
	cout<< "Penggunaan GoogleAPI: "<<biayaAPI <<endl;
	
}

int main(){
	memset(belumTerplot,true,sizeof(belumTerplot));
	
	Input();
	HitungJarakCosDa();
	SortJarakCosDa();

    PerbandinganPrioritas();
	PlottingCostumerToDapur();	
	
	TampilkanHasilPlot();
	
	cout<< "\n Driver\n";
	AssignDriver(); 
	
}
