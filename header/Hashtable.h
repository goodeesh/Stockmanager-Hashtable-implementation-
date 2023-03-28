#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <list>
#include <iostream>
#include <string>
#include "Aktie.h"
#include <algorithm>
#include <sstream>

using namespace std;
class Hashtable
{
private:
    static const int hashGroups = 2609; //two times number of elements and * 1.3 and prime number
    list<pair<unsigned long long, Aktie>> table[hashGroups]; // list 1 index 0, list 2 index 1...
public:
    bool is_Empty() const;
    int hashFunction(unsigned long long key);
    void insertItem(Aktie value);
    void removeItem();
    void searchTable(string str);
    unsigned long long createKey(string str);
    void printTable();
    void saveTable(string fileName);
    void loadTable(string fileName);
    void updateStock(string fileName, string nameKuerzel);
    Aktie *returnAktie(string str);
};

Aktie *Hashtable::returnAktie(string str)
{
    unsigned long long key = createKey(str);
    int hashValue = hashFunction(key);
    auto &cell = table[hashValue];//we go to the index that this stock is
    auto bItr = begin(cell);
    bool keyExist = false;
    for (; bItr != end(cell); bItr++)//then we iterate the list to find the right key
    {
        if (bItr->first == key)
        {
            keyExist = true;
            return &(bItr->second); //and we send the address
            break;
        }
    }
    if (!keyExist)
    {
        cout << "Not found" << endl;
    }
    return nullptr;
}

void Hashtable::updateStock(string fileName, string nameKuerzel) //used when import data
{
    Aktie *aktie = returnAktie(nameKuerzel);
    ifstream inFile(fileName);
    if (inFile.is_open())
    {
        string line;
        string Date, Open, High, Low, Close, Adj_Close, Volume;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            getline(ss, Date, ',');
            getline(ss, Open, ',');
            getline(ss, High, ',');
            getline(ss, Low, ',');
            getline(ss, Close, ',');
            getline(ss, Adj_Close, ',');
            getline(ss, Volume, ',');
            aktie->set_aktuell(Date, Open, High, Low, Close, Adj_Close, Volume);
        } //we take the last line, because it's the recent date and we set those values to the stock object
        inFile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
    }
}

void Hashtable::searchTable(string str)
{
    unsigned long long key = createKey(str);
    int hashValue = hashFunction(key);
    if (hashValue < 0)
        hashValue = hashValue * (-1);
    auto &cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExist = false;
    for (; bItr != end(cell); bItr++) //we look for the right key
    {
        if (bItr->first == key)
        {
            keyExist = true;
            bItr->second.print_aktuell(); //when we find then print last status
            break;
        }
    }
    if (!keyExist)
    {
        cout << "Item not found, wrong input?" << endl;
    }
}
void Hashtable::loadTable(string fileName) //reload all stocks in their last status
{
    ifstream inFile(fileName);
    if (inFile.is_open())
    {
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string name, wkn, kuerzel, Date, Open, High, Low, Close, Adj_Close, Volume;
            getline(ss, name, ',');
            getline(ss, wkn, ',');
            getline(ss, kuerzel, ',');
            getline(ss, Date, ',');
            getline(ss, Open, ',');
            getline(ss, High, ',');
            getline(ss, Low, ',');
            getline(ss, Close, ',');
            getline(ss, Adj_Close, ',');
            getline(ss, Volume, ',');
            Aktie aktie(name, wkn, kuerzel); //create item
            aktie.set_aktuell(Date, Open, High, Low, Close, Adj_Close, Volume); // add status of the stock
            insertItem(aktie); //insert the item in the table
        }
        inFile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
    }
}

void Hashtable::saveTable(string fileName) //save current status of the table
{
    ofstream outFile(fileName);
    if (outFile.is_open())
    {
        for (int i{}; i < hashGroups; i++)
        {
            if (table[i].size() == 0)
                continue;

            auto bItr = table[i].begin();
            for (; bItr != table[i].end(); bItr++)
            {
                outFile << bItr->second.get_name() << "," << bItr->second.get_wkn() << "," << bItr->second.get_kuerzel() << "," << bItr->second.get_Date() << "," << bItr->second.get_Open() << "," << bItr->second.get_High() << "," << bItr->second.get_Low() << "," << bItr->second.get_Close() << "," << bItr->second.get_Adj_Close() << "," << bItr->second.get_Volume() << endl;
                ;
            }
        }
        outFile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
    }
}

unsigned long long Hashtable::createKey(string str)
{ // this uses the algorith from hashCode from java
    unsigned long long hash = 0;
    int n = str.length();
    for (int i = 0; i < n; i++)
    {
        char c = str[i];
        hash = (hash * 31 + c) & 0xFFFFFFFFFFFF; // limit the hash to 48 bits
    }
    return hash;
}

bool Hashtable::is_Empty() const
{
    int sum{};
    for (int i{}; i < hashGroups; i++)
    {
        sum += table[i].size();
    }
    if (!sum)
    {
        return true;
    }
    return false;
}
int Hashtable::hashFunction(unsigned long long key)
{
    return key % hashGroups;
}

void Hashtable::insertItem(Aktie value)
{
    unsigned long long key = createKey(value.get_name());
    int hashValue = (hashFunction(key));
    if (hashValue < 0)
        hashValue = hashValue * (-1);
    auto &cell = table[hashValue]; // we go to index of the key
    auto bItr = begin(cell); //we start at the beggining of the list
    bool keyExist = false; 
    for (; bItr != end(cell); bItr++)
    {
        if (bItr->first == key) //if there is the key, we update it
        {
            keyExist = true;
            bItr->second = value;
            break;
        }
    }
    if (!keyExist)
    {
        cell.emplace_back(key, value); //if not at the end of the list
    }
    //and the same for the kuerzel
    unsigned long long key2 = createKey(value.get_kuerzel());
    if (key2 == key) return;
    int hashValue2 = hashFunction(key2);
    auto &cell2 = table[hashValue2];
    auto bItr2 = begin(cell2);
    bool keyExist2 = false;
    for (; bItr2 != end(cell2); bItr2++)
    {
        if (bItr2->first == key2)
        {
            keyExist2 = true;
            bItr2->second = value;
            // cout << "Item already exists. Value replaced." << endl;
            break;
        }
    }
    if (!keyExist)
    {
        cell2.emplace_back(key2, value);
    }
}

void Hashtable::printTable() //this is just to see the state of the table
{
    for (int i{}; i < hashGroups; i++)
    {
        if (table[i].size() == 0)
            continue;

        auto bItr = table[i].begin();
        for (; bItr != table[i].end(); bItr++)
        {
            cout << "Key :" << bItr->first << " Value: " << bItr->second.get_name() << " "
                 << bItr->second.get_wkn() << " " << bItr->second.get_kuerzel() << endl;
        }
    }
    return;
}
void Hashtable::removeItem() 
{
    string str;
    cout << "Enter the name or abbreviation of the stock to remove: ";
    cin.ignore(); // ignore the newline character
    getline(cin, str); // this way to enter input allows for empty spaces " " like this one in the string
    Aktie *aktie = returnAktie(str);
    if (aktie == nullptr)
        return;
    int key = createKey(aktie->get_name());
    int key2 = createKey(aktie->get_kuerzel());
    int hashValue = hashFunction(key);
    auto &cell = table[hashValue];
    auto bItr = begin(cell);
    bool keyExist = false;
    for (; bItr != end(cell); bItr++)
    {
        if (bItr->first == key)
        {
            keyExist = true;
            cell.erase(bItr); // we delete the node with that key
            cout << "Item removed." << endl;
            break;
        }
    }
    if (!keyExist)
    {
        cout << "Item not found." << endl;
    }
    int hashValue2 = hashFunction(key2);
    auto &cell2 = table[hashValue2];
    auto bItr2 = begin(cell2);
    bool keyExist2 = false;
    for (; bItr2 != end(cell2); bItr2++)
    {
        if (bItr2->first == key2)
        {
            keyExist2 = true;
            cell.erase(bItr2);
            cout << "Item removed." << endl;
            break;
        }
    }
    if (!keyExist2)
    {
        //cout << "Item not found." << endl;
    }
}

#endif // HASHTABLE_H