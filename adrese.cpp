#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
using namespace std;

ifstream fin("adrese.in");
ofstream fout("adrese.out");

struct personDistinct {
	string name;
	int nr;
	set<string> emails;
};
int N;
vector<int> adj[1000];
vector<int> duplicates;
vector <personDistinct> allPersons;
vector <personDistinct> distinctPersons;

// create a grapgh edge
void addEdge(vector<int> adj[], int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}
// create a distinct person, by parcing the duplicates vector
void createPerson() {
    personDistinct newPerson;
    vector<int>::iterator first = duplicates.begin();
    string min_name = allPersons[*first].name;

    vector<int>::iterator i;
    for (i = duplicates.begin(); i != duplicates.end(); ++i) {
        // find the minimum lexicografic name from the duplicates
        int res = min_name.compare(allPersons[*i].name);
        if (res > 0) {
                min_name = allPersons[*i].name;
        }
        // insert all the emails of all the duplicates in the newPerson
        newPerson.emails.insert(allPersons[*i].emails.begin(),
                                allPersons[*i].emails.end());
    }
    newPerson.name = min_name;
    newPerson.nr = newPerson.emails.size();
    // add person to vector of ditinct people
    distinctPersons.push_back(newPerson);
    duplicates.clear();  // get ready for the next distinct person
}
void DFS(int v, bool visited[]) {
    visited[v] = true;
    duplicates.push_back(v);  // add person to the list of duplicates

    vector<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])  // visit the nodes
            DFS(*i, visited);
}
void connectedComponents() {
    bool *visited = new bool[N];
    for (int i = 0; i < N; i++)
        visited[i] = false;

    for (int i = 0; i < N; i++) {
        if (visited[i] == false) {
            DFS(i, visited);
            createPerson();  // create a distinct person from its duplicates
        }
    }
    delete[] visited;
}
// compare people by nr emails and name for sort
bool compareDistinctPeople(const personDistinct &a, const personDistinct &b) {
    if (a.nr < b.nr)
        return true;
    else if (a.nr > b.nr)
        return false;
    else
        return a.name < b.name;
}
void show() {
    fout << distinctPersons.size() << endl;
    for (unsigned int i = 0; i < distinctPersons.size(); i++) {
        fout << distinctPersons[i].name << " " << distinctPersons[i].nr<< endl;
        for (auto it = distinctPersons[i].emails.begin();
				it != distinctPersons[i].emails.end(); it++)
            fout << *it << endl;
    }
}
int main(int argc, char const *argv[]) {
	string name, email;
    unordered_map <string, int> email_index;
	fin >> N;

	for (int i = 0; i < N; i++) {
		personDistinct newPerson;
		fin >> newPerson.name  >> newPerson.nr;
		for (int j = 0; j < newPerson.nr; j++) {
			fin >> email;
            newPerson.emails.insert(email);
			if (email_index.find(email) == email_index.end()) {
                email_index.insert({email, i});  // email was not found
            } else {  // we a have a duplicate person - > add edge
                addEdge(adj, email_index[email], i);
            }
		}
        allPersons.push_back(newPerson);  // add person to allPersons vector
	}

    // apply the connected components algorithm with dfs
    connectedComponents();
    //  sort distinct pople by nr emails and name
    sort(distinctPersons.begin(), distinctPersons.end(), compareDistinctPeople);
    // show results
	show();
	return 0;
}
