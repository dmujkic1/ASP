#include <fstream>
#include <iostream> //autotestovi za PZ6 jer su izmiksani
#include <vector>
using namespace std;

/*dvostruka petlja, vanjska sluzi samo kao brojac za unutrasnju koja ide od j=0 do j<brel-1-i
poredim elemente na [j] i [j+1] te ako je prvi veci, zamijenim pozicije
NEmodifikovani je uvijek O(n^2)*/
template<typename Tip>
void bubble_sort(Tip* niz, int vel){
    for (int i=0;i<vel-1;i++)
    {
        for (int j=0;j<vel-1-i;j++)
        {
            if (niz[j]>niz[j+1]) std::swap(niz[j],niz[j+1]);
        }
    }
}

/*vanjska petlja ide od 0 do brel-1, unutrasnja od i+1 do brel-1
uzimamo prvi element kao min, i trazimo da li postoji neki koji je manji
ako postoji manji, to je novi min i zamijenimo mjesta izabranog i min onda */
template<typename Tip>
void selection_sort(Tip* niz, int vel){
    for (int i=0;i<vel-1;i++)
    {
        Tip min=niz[i];
        int indeks=i;
        for (int j=i+1;j<vel;j++)
        {
            if (niz[j]<min)
            {
                indeks=j;
                min=niz[j];
            }
        }
        std::swap(niz[i], niz[indeks]);
    }
}

template<typename Tip>
int particija(Tip* niz, int prvi, int zadnji){
    Tip pivot=niz[prvi];
    int p=prvi+1;
    while (p<=zadnji && niz[p]<pivot) p++;
    for (int i=p+1; i<=zadnji; i++)
    {
        if (niz[i]<pivot)
        {
            std::swap(niz[i],niz[p]);
            p++;
        }
    }
    std::swap(niz[prvi], niz[p-1]);
    return p-1;
}

template<typename Tip>
void quick_sort(Tip* niz, int vel){
    static int prvi=0;
    static int zadnji=vel-1;
    if (prvi<zadnji)
    {
        int j=particija(niz, prvi, zadnji);
        int first=prvi;
        int last=zadnji;
        zadnji=j-1;
        quick_sort(niz, vel);
        prvi=j+1;
        zadnji=last;
        quick_sort(niz, vel);
        prvi=first;
    }
    
}

template<typename Tip>
void merge(Tip* niz, int l, int p, int q, int u){
    int i=0;
    int j=q-l;
    int k=l;
    int brojel=u-l+1;
    Tip *B = new Tip[brojel]; //velicina din all niza je jednaka broju elemenata proslijedjenog
    for (int m=0; m<=u-l; m++) B[m]=niz[l+m]; //prekopirao sve elemente u B
    while (i <= p-l && j <= u-l)
    {
        if (B[i]<B[j])
        {
            niz[k]=B[i];
            i++;
        }
        else
        {
            niz[k]=B[j];
            j++;
        }
        k++;      
    }
    while (i <= p-l)
    {
        niz[k]=B[i];
        k++;
        i++;
    }
    while (j <= u-l)
    {
        niz[k]=B[j];
        k++;
        j++;
    }
    delete[] B;
}


template<typename Tip>
void merge_sort(Tip* niz, int vel){
    static int l=0;
    static int u=vel-1;
    int p, q;
    if (u>l)
    {
        p=(l+u-1)/2;
        q=p+1;
        int gornji=u;
        int donji=l;
        u=p;
        merge_sort(niz, vel); //prva polovica poslana
        u=gornji;
        l=q;
        merge_sort(niz, vel); //druga polovica poslana
        l=donji;
        merge(niz, l, p, q, u); //mergan originalni
    }
}

template<typename Tip>
void ispisi(Tip *niz, int vel){
    for (int i=0;i<vel-1;i++)
    {
        std::cout<<niz[i]<<", ";
    }
    std::cout<<niz[vel-1];
}

/*Ova funkcija iz datoteke čiji je puni put dat u parametru filename učitava niz cijelih brojeva u
pokazivač niz (dat preko reference kako biste mogli u funkciji izvršiti dinamičku alokaciju), a u
referencu vel upisuje broj učitanih članova niza. Format datoteke treba biti: niz brojeva razdvojenih
razmakom ili praznim redom.
Napravite funkciju koja:
● pomoću funkcije ucitaj() učitava brojeve u niz;
● omogućuje korisniku da izabere koji algoritam sortiranja će se koristiti;
● poziva odgovarajuću funkciju za sortiranje;
● ispisuje vrijeme koliko se funkcija izvršavala (u milisekundama);
● provjerava da li je niz zaista sortiran (prolazi kroz niz i provjerava da li je svaki član veći ili
jednak prethodnom),
● zapisuje sortirani niz u neku novu izlaznu datoteku.*/
void ucitaj(string filename, int*& niz, int &vel){
    std::ifstream izlaz(filename);
    int broj;
    std::vector<int> v;
    while (izlaz>>broj)
    {
        v.push_back(broj);
    }
    niz = new int[v.size()];
    for (int i=0;i<v.size();i++) niz[i]=v.at(i);
    vel=v.size();
}

/*Radi lakšeg testiranja programa napravite i sljedeću funkciju
koja u datoteku čiji je put dat u parametru filename upisuje vel slučajno generisanih brojeva*/
void generisi(string filename, int vel){
    std::ofstream ulaz;
    while (vel>0)
    {
        ulaz<<int(std::rand());
        vel--;
    }
}
void funkcija(){}



int main(){
    int niz[10]{3,1,7,3,9,1,17,4,3,5};
    quick_sort(niz, 10);
    ispisi(niz, 10);
}