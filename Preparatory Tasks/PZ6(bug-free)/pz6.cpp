#include <iostream>
#include <utility>
#include <vector>
#include <string>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa{
private:
public:
    Mapa(){};
    virtual ~Mapa() {};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0; //brise sve el iz mape
    virtual void obrisi(const TipKljuca &kljuc) = 0; //brise jedan el, znaci osloboditi tu memoriju
    virtual TipVrijednosti &operator[](TipKljuca k) = 0;
    virtual TipVrijednosti operator[](TipKljuca k) const = 0; //NIJE INDEKS NEGO TIPKLJUCA KLJUC
};

template <typename TipKljuca, typename TipVrijednosti> class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{
private:
    std::pair<TipKljuca, TipVrijednosti> **niz;
    int kapacitet;
    int brel;
public:
    NizMapa(){
        niz=new std::pair<TipKljuca, TipVrijednosti> *[10000];
        kapacitet=10000;
        brel=0;
    }

    ~NizMapa(){
        for (int i=0;i<brel;i++) delete niz[i];
        delete [] niz;
    }

    /*NizMapa(const NizMapa &n){ //kopirajuci
        kapacitet=n.kapacitet;
        brel=n.brel;
        if (kapacitet==0) niz=nullptr;
        else
        {
            niz=new std::pair<TipKljuca, TipVrijednosti> *[kapacitet];
            for (int i=0;i<brel;i++) niz[i] = new std::pair<TipKljuca, TipVrijednosti> (*n.niz[i]);
        }
    }

    NizMapa &operator=(const NizMapa &n){ //operator dodjele (DESTR+KONSTR)
        if (this==&n) return *this;
        for (int i=0;i<brel;i++) delete niz[i];
        delete [] niz;
        kapacitet=n.kapacitet;
        brel=n.brel;
        if (kapacitet==0) niz=nullptr;
        else
        {
            niz=new std::pair<TipKljuca, TipVrijednosti> *[kapacitet];
            for (int i=0;i<brel;i++) niz[i] = new std::pair<TipKljuca, TipVrijednosti> (*n.niz[i]);
        }
        return *this;
    }*/

    int brojElemenata() const {return brel;}

    void obrisi(){
        for (int i=0;i<brel;i++) delete niz[i];
        brel=0;
    }

    void obrisi(const TipKljuca &k){
        for (int i=0;i<brel;i++)
        {
            if (niz[i]->first==k)
            {
                for (int j=i;j<brel-1;j++)
                {
                    niz[j]=niz[j+1];
                }
                delete niz[brel];
                brel--;
            }
            else throw std::logic_error("Proslijedjeni kljuc nije pronadjen u mapi!");
        }
    }

    TipVrijednosti &operator[](TipKljuca k){
         /*if(brel==kapacitet){
        auto temp=new std::pair<TipKljuca,TipVrijednosti>*[kapacitet*2]();
        for(int i=0;i<brel;i++)
        {temp[i]=new std::pair<TipKljuca,TipVrijednosti>(*niz[i]); delete niz[i];}
        delete[] niz; kapacitet+=10000;
        niz=temp; temp=nullptr;
    }*/
        for (int i=0;i<brel;i++)
        {
            if (niz[i]->first==k) return niz[i]->second;
        }
        niz[brel]=new std::pair<TipKljuca, TipVrijednosti> (k, TipVrijednosti{});
        brel++;
        return niz[brel-1]->second;
    }

    TipVrijednosti operator[](TipKljuca k) const{
        for (int i=0;i<brel;i++)
        {
            if (niz[i]->first==k) return niz[i]->second;
        }
        return TipVrijednosti{}; //ako ne nadje vrati default
    }

};

int main(){
NizMapa<std::string,std::string> m;
m["Sarajevo"] = "BiH";
m["Zagreb"] = "Hrvatska";
{
        // Konstruktor kopije
	NizMapa<std::string,std::string> m2(m);
        // Operator dodjele
	NizMapa<std::string,std::string> m3;
	m3["Kiseljak"] = "BiH";
	cout << (m3=m).brojElemenata() << " "; // 2
        // Destruktivna samododjela
	cout << (m=m).brojElemenata() << " "; // 2

        // Plitka kopija
	m.obrisi("Sarajevo");
	cout << m2.brojElemenata() << " "; // 2
        cout << m2["Sarajevo"] << " "; // BiH
        cout << m3.brojElemenata() << " "; // 2
        cout << m3["Sarajevo"] << " "; // BiH
}
cout << m.brojElemenata(); // 1
cout << " '" << m["Sarajevo"] << "' "; // ''
m.obrisi();
cout << m.brojElemenata(); // 0
cout << " '" << m["Zagreb"] << "'"; // ''
    return 0;
}


