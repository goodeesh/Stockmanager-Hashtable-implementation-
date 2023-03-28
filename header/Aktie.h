#ifndef AKTIE_H
#define AKTIE_H

#include <iostream>

using namespace std;

class Aktie
{
public:
    Aktie()
    {
    }
    Aktie(string name, string wkn, string kuerzel) //this is the base stock, constructor with them then
    {
        this->name = name;
        this->wkn = wkn;
        this->kuerzel = kuerzel;
        /*     this->hashName = hash(name);
            this->hashKuerzel = hash(kuerzel);
         */
    }
    string get_name();
    string get_wkn();
    string get_kuerzel();
    string get_Date();
    string get_Open();
    string get_High();
    string get_Low();
    string get_Close();
    string get_Adj_Close();
    string get_Volume();

    void set_aktuell(string Date, string Open, string High, string Low, string Close, string Adj_Close, string Volume);
    void print_aktuell();

private:
    string name;
    string wkn;
    string kuerzel;
    int hashName;
    int hashKuerzel;
    string aktuell;
    string Date, Open, High, Low, Close, Adj_Close, Volume;
};

string Aktie::get_name()
{
    return this->name;
}
string Aktie::get_kuerzel()
{
    return this->kuerzel;
}
string Aktie::get_wkn()
{
    return this->wkn;
}
string Aktie::get_Date()
{
    return this->Date;
}

string Aktie::get_Open()
{
    return this->Open;
}

string Aktie::get_High()
{
    return this->High;
}

string Aktie::get_Low()
{
    return this->Low;
}

string Aktie::get_Close()
{
    return this->Close;
}

string Aktie::get_Adj_Close()
{
    return this->Adj_Close;
}

string Aktie::get_Volume()
{
    return this->Volume;
}

void Aktie::set_aktuell(string Date, string Open, string High, string Low, string Close, string Adj_Close, string Volume)
{
    this->Date = Date;
    this->Open = Open;
    this->High = High;
    this->Low = Low;
    this->Close = Close;
    this->Adj_Close = Adj_Close;
    this->Volume = Volume;
}
void Aktie::print_aktuell()
{
    cout << "On the " << this->Date << ", " << this->name << " opened with " << this->Open << " and closed with " << this->Close << endl;
    cout << "The ajusted after closed position ended with " << this->Adj_Close << " and there was a total of " << this->Volume << " volume.";
    cout << endl;
}

#endif // AKTIE_H
