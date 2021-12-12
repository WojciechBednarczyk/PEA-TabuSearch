#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
#include<algorithm>
using namespace std;

int ilosc_miast;
int** macierz;
int** lista_tabu;

void aktualizuj_liste_tabu()
{
    for (int i = 0; i < ilosc_miast; i++)
    {
        for (int j = 0; j < ilosc_miast; j++)
        {
            if (lista_tabu[i][j] != 0)
            {
                lista_tabu[i][j]--;
            }
        }
    }
}

bool czy_zakazane(int miasto1,int miasto2)
{
    if (lista_tabu[miasto1][miasto2] != 0 || lista_tabu[miasto2][miasto1] != 0)
        return true;
    else
        return false;
}


int main()
{

    string nazwa_pliku;
    string sciezka_do_pliku = "instancje\\";
    fstream plik;
    string wyraz;
    int wartosc_optymalna;

    cout << "Podaj nazwe pliku razem z rozszerzeniem\n";
    cin >> nazwa_pliku;
    sciezka_do_pliku += nazwa_pliku;
    plik.open(sciezka_do_pliku);
    //Sprawdzanie czy plik o podanej nazwie istnieje
    while (!plik)
    {
        cout << "Plik o takiej nazwie nie istnieje!\n\n";
        cout << "Podaj nazwe pliku razem z rozszerzeniem\n";
        sciezka_do_pliku = "instancje\\";
        cin >> nazwa_pliku;
        sciezka_do_pliku += nazwa_pliku;
        plik.open(sciezka_do_pliku);
    }
    //pominiecie pierwszego wiersza
    getline(plik, wyraz);
    //wczytanie ilosci miast
    getline(plik, wyraz);

    
    ilosc_miast = stoi(wyraz);


    vector<int> miasta;

    //utworzenie macierzy i wpisanie do niej danych
    macierz = new int* [ilosc_miast];
    lista_tabu = new int* [ilosc_miast];
    for (int i = 0; i < ilosc_miast; i++)
    {
        miasta.push_back(i);
        macierz[i] = new int[ilosc_miast];
        lista_tabu[i] = new int[ilosc_miast];
    }
    int kolumna = 0;
    int rzad = 0;
    for (int i = 0; i < ilosc_miast * ilosc_miast; i++)
    {
        if (kolumna == ilosc_miast)
        {
            kolumna = 0;
            rzad++;
            //cout << endl;
        }
        plik >> wyraz;
        macierz[rzad][kolumna] = stoi(wyraz);
        lista_tabu[rzad][kolumna] = 0;
        //cout << macierz[rzad][kolumna] << " ";
        kolumna++;
    }
    plik >> wyraz;
    wartosc_optymalna = stoi(wyraz);

    plik >> wyraz;
    int miasto_startowe = 0;
    int aktualne_miasto = 0;
    int najmniejszy_koszt=INT_MAX;
    int najtansze_miasto;
    int najlepsza_wartosc;
    vector<int> odwiedzone_miasta;
    vector<int> aktualne_rozwiazanie;
    int aktualny_koszt=0;
    float prd;
    odwiedzone_miasta.push_back(miasto_startowe);

    //wyznaczenie startowego rozwiazania metoda zachlanna

    while (odwiedzone_miasta.size()!=ilosc_miast)
    {
        najmniejszy_koszt = INT_MAX;
        for (int j = 0; j < ilosc_miast; j++)
        {
            if (macierz[aktualne_miasto][j] < najmniejszy_koszt && macierz[aktualne_miasto][j]!=-1)
            {
                
                if (find(odwiedzone_miasta.begin(), odwiedzone_miasta.end(), j) != odwiedzone_miasta.end())
                {
                    continue;
                }
                else
                {
                    najmniejszy_koszt = macierz[aktualne_miasto][j];
                    najtansze_miasto = j;
                }
            }
        
        }
        aktualny_koszt += najmniejszy_koszt;
        odwiedzone_miasta.push_back(najtansze_miasto);
        aktualne_miasto = najtansze_miasto;
    }
    aktualny_koszt += macierz[aktualne_miasto][0];
    aktualne_rozwiazanie = odwiedzone_miasta;
    prd = 100 * (aktualny_koszt - wartosc_optymalna) / (float)wartosc_optymalna;
    najlepsza_wartosc = aktualny_koszt;
    cout << "0 " << najlepsza_wartosc << " ";
    cout << fixed << setprecision(2) << prd << "%" << endl;

    //ilosc iteracji algorytmu
    int const iteracje = 15360;
    int wartosc_najlepszej_zamiany=INT_MAX;
    int wartosc_zamiany;
    int miasto_do_zamiany1, miasto_do_zamiany2;
    int najlepszy_koszt_po_zmianie;
    vector<int> rozwiazanie_pom;

    int kadencja = ilosc_miast;   

    //algorytm
    for (int i = 1; i <= iteracje; i++)
    {
        wartosc_najlepszej_zamiany = INT_MAX;
        //szukanie najlepszej zmiany w sasiedztwie
        for (int j = 0; j < aktualne_rozwiazanie.size()-1; j++)
        {
            for (int k = j+1; k < aktualne_rozwiazanie.size(); k++)
            {
                rozwiazanie_pom = aktualne_rozwiazanie;
                wartosc_zamiany = 0;
                    iter_swap(rozwiazanie_pom.begin() + j, rozwiazanie_pom.begin() + k);
                    //obliczanie wartosci zmiany
                    for (int l = 0; l < rozwiazanie_pom.size(); l++)
                    {
                        if (l==rozwiazanie_pom.size()-1)
                        {
                            wartosc_zamiany += macierz[rozwiazanie_pom[l]][rozwiazanie_pom[0]];
                        }
                        else
                        {
                            wartosc_zamiany += macierz[rozwiazanie_pom[l]][rozwiazanie_pom[l + 1]];
                        }
                    }

                    //Sprawdzamy czy ruch zakazany poprawia najlepsze rozwiazanie
                    if (czy_zakazane(j, k) == true && wartosc_zamiany < najlepsza_wartosc)
                    {
                        najlepszy_koszt_po_zmianie = wartosc_zamiany;
                        miasto_do_zamiany1 = j;
                        miasto_do_zamiany2 = k;
                    }
                    else if (wartosc_zamiany - aktualny_koszt< wartosc_najlepszej_zamiany && czy_zakazane(j,k) == false)
                    {
                        wartosc_najlepszej_zamiany = wartosc_zamiany - aktualny_koszt;
                        miasto_do_zamiany1 = j;
                        miasto_do_zamiany2 = k;
                        najlepszy_koszt_po_zmianie = wartosc_zamiany;
                    }
                

            }
        }
        //aktualizacja i wpisywanie do listy tabu
        aktualizuj_liste_tabu();
        lista_tabu[miasto_do_zamiany1][miasto_do_zamiany2] = kadencja;
        lista_tabu[miasto_do_zamiany2][miasto_do_zamiany1] = kadencja;

        iter_swap(aktualne_rozwiazanie.begin() + miasto_do_zamiany1, aktualne_rozwiazanie.begin() + miasto_do_zamiany2);
        aktualny_koszt = najlepszy_koszt_po_zmianie;
        if (aktualny_koszt < najlepsza_wartosc)
        {
            najlepsza_wartosc = aktualny_koszt;
            prd = 100 * (aktualny_koszt - wartosc_optymalna) / (float)wartosc_optymalna;
            cout << i <<" " << najlepsza_wartosc << " ";
            cout << fixed << setprecision(2) << prd << "%" << endl;
        }

        
        //for (int i : aktualne_rozwiazanie)
        //    cout << i << ' ';
        //cout << endl;
    }

}

