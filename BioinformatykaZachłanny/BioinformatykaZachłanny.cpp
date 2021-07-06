#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string finalResult{};

class Node1 {
public:
    string olig;
    vector<long unsigned int> nextones;
    long unsigned int index;
    int visited = 0;
    int matches;
};

void Hamilton(vector<Node1>& list, vector<int>& listVisited, int probe, int length, Node1& current, string& result) {
    listVisited.push_back(current.index);
    current.visited++;
    for (long unsigned int i = 0; i < current.nextones.size(); i++)
    {
        if (list[current.nextones[i]].visited < list[current.nextones[i]].matches)
        {
            Hamilton(list, listVisited, probe, length, list[current.nextones[i]], result);
        }
    }
    if (listVisited[0] == current.index) {
        for (int j = probe-1; j >= 0; j--) {
            result.insert(result.begin(), list[current.index].olig[j]);
        }
    }
    else {
        result.insert(result.begin(), list[current.index].olig[probe - 1]);
    }
}

void nextOnes(vector<Node1>& list, Node1& parent, int probe) {
    for (long unsigned int i = 0; i < list.size(); i++)
    {
        if ((i != parent.index) && (list[i].olig.substr(0, probe - 1) == parent.olig.substr(1, probe - 1))) {
            parent.nextones.push_back(i);
        }
    }
}

int randomizeMatch(int lower, int upper)
{
    return (lower + (rand() % (upper - lower + 1)));
}

int intensityToMatch(int intensity)
{
    switch (intensity)
    {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    case 2:
        return 1;
        break;
    case 3:
        //return randomizeMatch(1,2);
        return 1;
        break;
    case 4:
        return 2;
        break;
    case 5:
        //return randomizeMatch(2,3);
        return 2;
        break;
    case 6:
        //return randomizeMatch(3,4);
        return 3;
        break;
    case 7:
        //return randomizeMatch(3, 6);
        return 3;
        break;
    case 8:
        //return randomizeMatch(4, 8);
        return 4;
        break;
    case 9:
        //return randomizeMatch(6, 9);
        return 6;
        break;
    default:
        return 0;
    }
}

int main()
{
    srand(time(NULL));
    ifstream inputFile;
    inputFile.open("input.xml");
    string linia;
    getline(inputFile, linia);
    int indeks1;
    indeks1 = linia.find("length=\"");
    int indeks2;
    indeks2 = linia.find("\" start=\"");
    int indeks3;
    indeks3 = linia.find("\">");
    int length = stoi(linia.substr((indeks1+8),indeks2- (indeks1 + 8)));
    string start = linia.substr((indeks2 + 9), indeks3 - (indeks2 + 9));
    cout << "Length: " << length << endl;
    cout << "Start: " << start << endl;

    for (int i = 0; i < 15; i++)
    {
        getline(inputFile, linia);
    }
    
    int probe = start.size();

    int indexToStart = 0;

    vector<Node1> nodes{};

    vector<int> listVisited{};

    getline(inputFile, linia);
    int i = 0;
    int intensity;
    string oligonucleotide;
    int oligonucleotidesNumber = 0;

    while (linia.find("</probe>") == string::npos)
    {
        //dla łatwego inputu:
        /*
        indeks2 = linia.find("l>");
        indeks3 = linia.find("</cell>");
        oligonucleotide = linia.substr((indeks2 + 2), indeks3 - (indeks2 + 2));
        intensity = 1;
        */
        //dla zwyklego inputu:
        
        indeks1 = linia.find("ity=\"");
        indeks2 = linia.find("\">");
        intensity = stoi(linia.substr((indeks1 + 5), 1));
        oligonucleotide = linia.substr((indeks2 + 2), probe);
        
        Node1 x{};
        x.index = i;
        x.olig = oligonucleotide;
        x.matches = intensityToMatch(intensity);
        oligonucleotidesNumber += intensityToMatch(intensity);
        nodes.push_back(x);
        if (start == oligonucleotide) {
            indexToStart = i;
        }
        getline(inputFile, linia);
        i++;
    } 
    for (int i = 0; i < nodes.size(); i++) {
        nextOnes(nodes, nodes[i], probe);
    }
    
    cout << "Zaczynamy tym: " << nodes[indexToStart].olig << endl;
    string partialResult{};
    Hamilton(nodes, listVisited, probe, length, nodes[indexToStart], partialResult);
    finalResult += partialResult;
    partialResult.clear();
    bool found;
    while ((listVisited.size() < oligonucleotidesNumber) && (finalResult.size() < length))
    {
        found = false;
        int index = 0;
        for (int j = probe - 2; j >= 0; j--)
        {
            for (int i = 0; i < nodes.size(); i++)
            {
                if (nodes[i].visited < nodes[i].matches)
                {
                    if (j == 0)
                    {
                        index = i;
                        found = true;
                        finalResult += nodes[i].olig.substr(j, probe - 1 - j);
                        //cout << "Dodaje: " << probe - 1 - j << " nukleotydow." << endl;
                    }
                    else
                    {
                        if (finalResult.substr(finalResult.size() - j) == nodes[i].olig.substr(0, j))
                        {
                            //cout << "Polaczylem " << finalResult.substr(finalResult.size() - j) << " z " << nodes[i].olig << " dla j = " << j << endl;
                            index = i;
                            found = true;
                            finalResult += nodes[i].olig.substr(j, probe - 1 - j);
                            //cout << "i wtedy dodalem: " << nodes[i].olig.substr(j, probe - 1 - j) << endl;
                            //cout << "Dodaje: " << probe - 1 - j << " nukleotydow." << endl;
                        }
                        else
                        {
                            //cout << "Odrzucilem " << finalResult.substr(finalResult.size() - j) << " z " << nodes[i].olig << " dla j = " << j << endl;
                        }
                    }
                }
                if (found) break;
            }
            if (found) break;
        }
        if (found)
        {
            Hamilton(nodes, listVisited, probe, length, nodes[index], partialResult);
            finalResult += partialResult;
            partialResult.clear();
        }
        else
        {
            break;
        }
    }
    int roznica = length - finalResult.size();
    cout << finalResult << endl;
    cout << "Brakuje " << roznica << " nukleotydow." << endl;
}
