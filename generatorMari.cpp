#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void save(string folder, int id, const vector<int>& v) {
    string path = "teste/" + folder + "/test_" + to_string(id) + ".txt";
    ofstream fout(path);
    
    // Optimizare: scriem numerele folosind un buffer
    for (size_t i = 0; i < v.size(); i++) {
        fout << v[i] << (i == v.size() - 1 ? "" : " ");
    }
    fout.close();
}

int main() {
    // Dezactivam sincronizarea pentru viteza
    ios::sync_with_stdio(0);
    cin.tie(0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distLarge(0, 1000000000); // 0 - 1 miliard
    uniform_int_distribution<int> distSmall(0, 10);

    // --- CONFIGURARE ---
    int nrTeste = 5;  

    int N = 50000;  
    
    vector<string> tipuri = {"random", "sorted", "reversed", "almost_sorted", "plateau"};
    for (const string& t : tipuri) fs::create_directories("teste/" + t);

    cout << "Generare N = " << N << " (" << nrTeste << " teste per tip)..." << endl;

    for (int i = 1; i <= nrTeste; i++) {
        vector<int> v(N);

        // 1. RANDOM
        for (int j = 0; j < N; j++) v[j] = distLarge(gen);
        save("random", i, v);

        // 2. SORTED
        for (int j = 0; j < N; j++) v[j] = j;
        save("sorted", i, v);

        // 3. REVERSED
        for (int j = 0; j < N; j++) v[j] = N - j;
        save("reversed", i, v);

        // 4. ALMOST SORTED
        for (int j = 0; j < N; j++) v[j] = j;
        for (int j = 0; j < N / 100; j++) swap(v[distLarge(gen) % N], v[distLarge(gen) % N]);
        save("almost_sorted", i, v);

        // 5. PLATEAU
        for (int j = 0; j < N; j++) v[j] = distSmall(gen);
        save("plateau", i, v);

        cout << "Finalizat setul " << i << "/" << nrTeste << endl;
    }

    cout << "Gata! Toate fisierele mari au fost create in /teste/." << endl;
    return 0;
}