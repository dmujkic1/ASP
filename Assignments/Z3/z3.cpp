#include <deque>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Zad1
void radixSort(vector<int> &v) {
  std::vector<std::deque<int>> redovi(10);
  int zadnjaCifra = 1;
  for (;;)
  {
    for (int broj : v) redovi[(broj / zadnjaCifra) % 10].push_back(broj);
      zadnjaCifra *= 10;
      int i = 0;
      for (int j = 0; j < redovi.size(); j++)
      {
        while (!redovi[j].empty())
        {
            v[i++] = redovi[j].at(0);
            redovi[j].pop_front();
        }
        if (j == 0 && i == v.size()) return;
      }
    
  }
}

//Zad2
int lijevoDijete(int i) { return 2*i+1; }
int desnoDijete(int i) { return 2*i+2; }
bool jeLiList(int vel,int i) { return (i<vel&&i>=vel/2); }
void popraviDole(vector<int> &v,int i, int vel){
    while(!jeLiList(vel,i))
    {
        int veci=lijevoDijete(i);
        int desno=desnoDijete(i);
        if(desno<vel&&v.at(desno)>v.at(veci)) veci=desno;
        if(v.at(i)>v.at(veci)) return;
        std::swap(v.at(i),v.at(veci));
        i=veci;
    }
}
void stvoriGomilu(vector<int> &v){
    int vel=v.size();
    for(int i=(vel-1)/2;i>=0;i--) popraviDole(v,i,vel);
}
void gomilaSort(vector<int> &v){
    stvoriGomilu(v);
    int vel=v.size();
    for(int i=vel-1;i>=1;i--)
    {
        std::swap(v.at(0),v.at(i));
        vel--;
        popraviDole(v,0,vel);
    }
}

int izbaciPrvi(vector<int> &v,int &vel){
    if(vel==0) throw std::logic_error("PRAZNO JE");
    vel--;
    swap(v.at(0),v.at(vel));
    if(vel!=0) popraviDole(v,0,vel);
    return v.at(vel);
}
int parenT(int i) {return (i-1)/2;}

void popraviGore(vector<int> &v,int i){
    while(i!=0&& v.at(i)>v.at(parenT(i)))
    {
        std::swap(v.at(i),v.at(parenT(i)));
        i=parenT(i);
    }
}

void umetniUGomilu(vector<int> &v, int umetni, int &vel){
    v.push_back(umetni);
    vel++;
    popraviGore(v,vel-1);
}

int main() {
  std::vector<int> v;
  std::cout<<"Uneseni niz glasi: ";
  v.push_back(3);
  std::cout<<v.back()<<" ";
  v.push_back(300);
  std::cout<<v.back()<<" ";
  v.push_back(13);
  std::cout<<v.back()<<" ";
  v.push_back(99);
  std::cout<<v.back()<<" \n";
  radixSort(v);
  std::cout<<"Sortirani niz glasi: ";
  for (int i=0; i < v.size(); i++) std::cout << v[i] << " ";
  return 0;
}