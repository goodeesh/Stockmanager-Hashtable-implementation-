#include <iostream>
#include <fstream>
#include <string>
#include "header/Aktie.h"
#include "header/Hashtable.h"
#include <filesystem>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;
namespace fs = std::filesystem;

int import2(Hashtable *HT);
void quit(Hashtable *HT, bool *boolean_p);
void add(Hashtable *HT);
void search(Hashtable *HT);
void save(Hashtable *HT);
void load(Hashtable *HT);
void plot(Hashtable *HT);
void input(Hashtable *HT, bool *boolean_p);

int main()
{
    // create table at the beggining
    Hashtable *HT = new Hashtable;
    HT->loadTable("hashtable.txt"); // reloads the last state of the table, i don't need the item replaced, need to work on that.a
    bool boolean = true;
    bool *boolean_p = &boolean;
    while (boolean == true)
    {
        input(HT, boolean_p); // lets call a function depending on the input of the user
    }
    return 0;
}

int import2(Hashtable *HT)
{
    // Prompt the user to enter the path to the source file
    string input;
    cout << "Enter a Name or Abbreviation: " << endl;
    cin >> input;
    Aktie *aux = HT->returnAktie(input);
    if (aux == nullptr)
        return 1;
    string name_aktie = aux->get_name();
    string kuerzel_aktie = aux->get_kuerzel();
    cout << "Enter the path to the source file: ";
    cin.ignore(); // ignore the newline character
    string sourceFilePath;
    getline(cin, sourceFilePath); // this way to enter input allows for empty spaces " " like this one in the string
    // Open the source file
    ifstream sourceFile(sourceFilePath, ios::binary);
    if (!sourceFile)
    {
        cerr << "Error: Could not open source file" << endl;
        return 1;
    }

    // We save the file one time with the name
    string destFilePath = R"(csv\)";
    destFilePath.append(name_aktie);
    // and one time with the kuerzel
    string destFilePath2 = R"(csv\)";
    destFilePath2.append(kuerzel_aktie);
    HT->updateStock(sourceFilePath, name_aktie); //we update the last state of the stock, with the newest data
    HT->updateStock(sourceFilePath, kuerzel_aktie);
    // Copy the source file to the destination file using the copy_file function
    try
    {
        if (std::filesystem::remove(destFilePath))
            std::cout << "file " << destFilePath << " deleted.\n";
        else
            std::cout << "file " << destFilePath << " not found.\n";

        if (std::filesystem::remove(destFilePath2))
            std::cout << "file " << destFilePath2 << " deleted.\n";
        else
            std::cout << "file " << destFilePath2 << " not found.\n";
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        std::cout << "filesystem error: " << err.what() << '\n';
    }

    fs::copy_file(sourceFilePath, destFilePath);
    fs::copy_file(sourceFilePath, destFilePath2);

    // Close the source file
    sourceFile.close();

    cout << "File copied successfully" << endl;
    return 0;
}

void quit(Hashtable *HT, bool *boolean_p)
{
    HT->saveTable("hashtable.txt");
    delete HT;
    *boolean_p = false;
}
void add(Hashtable *HT)
{
    string input_name;
    string input_wkn;
    string input_abb;
    cout << "Enter a name: ";
    cin.ignore(); // ignore the newline character
    getline(cin, input_name);
    cout << "Enter the wkn: ";
    cin >> input_wkn;
    cout << "Enter the abbreviation: ";
    cin >> input_abb;
    Aktie neue(input_name, input_wkn, input_abb);
    HT->insertItem(neue);
}
void search(Hashtable *HT)
{
    string input;
    cout << "Enter a name or and abbreviation to look for: " << endl;
    cin >> input;
    HT->searchTable(input);
}
void save(Hashtable *HT)
{
    string input;
    cout << "Enter the name of the backup: ";
    cin >> input;
    HT->saveTable(input);
}
void load(Hashtable *HT)
{
    string input;
    cout << "Enter the path for the file to load: ";
    cin >> input;
    HT->loadTable(input);
}

void plot(Hashtable *HT)
{
    // Create Vector to save close and date
    vector<double> closeV;
    vector<string> dateV;

    // Get file name from user
    cout << "Enter the name or kuerzel of the file to display: ";
    string input;
    cin >> input;
    string sourceFilePath = R"(csv\)";
    sourceFilePath.append(input);

    // Open the source file in binary mode
    ifstream sourceFile(sourceFilePath, ios::binary);
    if (!sourceFile)
    {
        cerr << "Error: Could not open file" << endl;
        return;
    }

    // Read the contents of the source file and write them in the vector
    int aux = 0;
    string line;
    while (getline(sourceFile, line))
    {
        if (aux == 0)
        {
            aux++;
            continue;
        }

        stringstream ss(line);
        string date, open, high, low, close;
        getline(ss, date, ','); //
        getline(ss, open, ',');
        getline(ss, high, ',');
        getline(ss, low, ',');
        getline(ss, close, ','); //

        closeV.push_back(stod(close));
        if (closeV.size() > 30)
        {
            closeV.erase(closeV.begin());
            dateV.erase(dateV.begin());
        }
        dateV.push_back(date);
    }

    // Close file
    sourceFile.close();
    cout << endl;

    // Minimale und maximale Werte bestimmen
    double min_value = *min_element(closeV.begin(), closeV.end());
    double max_value = *max_element(closeV.begin(), closeV.end());

    // Skala berechnen
    double scale = max_value - min_value;

    // Breite und Höhe der ASCII-Grafik bestimmen
    int width = 30 + 2;                       // 2 zusätzliche Zeichen für Achsenbeschriftung
    int height = static_cast<int>(scale) + 2; // 2 zusätzliche Zeichen für Achsenbeschriftung

    // ASCII-Grafik erstellen
    vector<string> graph(height, string(width, ' ')); // Matrix mit Leerzeichen initialisieren

    // Achsenbeschriftung
    graph[0][0] = '+';
    graph[height - 1][0] = '+';
    graph[0][width - 1] = '+';
    graph[height - 1][width - 1] = '+';

    for (int i = 1; i < height - 1; i++)
    {
        graph[i][0] = '|';
        graph[i][width - 1] = '|';
    }

    for (int i = 1; i < width - 1; i++)
    {
        graph[0][i] = '-';
        graph[height - 1][i] = '-';
    }

    // Aktienkursdaten in ASCII-Grafik umwandeln
    for (int i = 0; i < dateV.size(); i++)
    {
        int row = static_cast<int>((max_value - closeV[dateV.size() - i - 1]) + 1); // Zeile berechnen
        int col = i + 1;                                                            // Spalte berechnen
        graph[row][col] = '*';                                                      // Sternchen als Datenpunkt setzen
    }

    // ASCII-Grafik ausgeben
    for (int i = 0; i < height; i++)
    {
        cout << graph[i] << endl;
    }
    cout << dateV[0] << " - " << dateV[29] << endl;
}

void input(Hashtable *HT, bool *boolean_p)
{
    while (*boolean_p == true)
    {
        string input;
        cout << "Enter an input: " << endl;
        cout << "Enter (a) to add a new Stock." << endl;
        cout << "Enter (p) to print the current stocks." << endl;
        cout << "Enter (s) to search for a stock." << endl;
        cout << "Enter (d) to delete a stock." << endl;
        cout << "Enter (i) to import data for a stock." << endl;
        cout << "Enter (f) to save the Hashtable Data in a file" << endl;
        cout << "Enter (l) to load the Hashtable Data from a file" << endl;
        cout << "Enter (g) to show the graphic of the last 30 days" << endl;
        cout << "Enter (q) to exit the application." << endl;
        cin >> input;
        if (input.length() > 1)
        {
            cout << "That is not a valid input" << endl;
            continue;
        }
        char input_char = input[0];
        if (input_char == 'a')
            add(HT);
        else if (input_char == 'p')
            HT->printTable();
        else if (input_char == 'q')
            quit(HT, boolean_p);
        else if (input_char == 's')
            search(HT);
        else if (input_char == 'f')
            save(HT);
        else if (input_char == 'l')
            load(HT);
        else if (input_char == 'g')
            plot(HT);
        else if (input_char == 'i')
            import2(HT);
        else if (input_char == 'd')
        {   
            HT->removeItem();
        }
        else
        {
            cout << "That is not a valid input" << endl;
            continue;
        }
    }
}
