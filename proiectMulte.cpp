#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// --- ALGORITMI DE SORTARE ---


int partition(vector<int>& v, int low, int high) {
    int pivot = v[high]; 
    int i = (low - 1);   

    for (int j = low; j <= high - 1; j++) {
        if (v[j] <= pivot) {
            i++;
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[high]);
    return (i + 1);
}


void qSort(vector<int>& v, int low, int high) {
    if (low < high) {
        int pi = partition(v, low, high);
        qSort(v, low, pi - 1);
        qSort(v, pi + 1, high);
    }
}


void quickSort(vector<int>& v) { 
    if (!v.empty()) {
        qSort(v, 0, v.size() - 1); 
    }
}


void introSort(vector<int>& v) {
    sort(v.begin(), v.end());
}

void bubbleSort(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (v[j] > v[j + 1]) swap(v[j], v[j + 1]);
}

void selectionSort(vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (v[j] < v[min_idx]) min_idx = j;
        swap(v[i], v[min_idx]);
    }
}

void insertionSort(vector<int>& v) {
    int n = v.size();
    for (int i = 1; i < n; i++) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = key;
    }
}

void merge(vector<int>& v, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = v[l + i];
    for (int j = 0; j < n2; j++) R[j] = v[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) v[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];
}

void mSort(vector<int>& v, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mSort(v, l, m); mSort(v, m + 1, r);
        merge(v, l, m, r);
    }
}

void mergeSort(vector<int>& v) { if(!v.empty()) mSort(v, 0, v.size() - 1); }

// --- MOTORUL DE TESTARE ---
double measure(void (*algo)(vector<int>&), const vector<int>& data) {
    vector<int> copy = data;
    auto s = high_resolution_clock::now();
    algo(copy);
    auto e = high_resolution_clock::now();
    return duration_cast<nanoseconds>(e - s).count() / 1000.0; 
}

void runBenchmark(string tip, int nrTeste) {
    double tQ = 0, tM = 0, tI = 0, tS = 0, tB = 0, tStd = 0;
    cout << "\n>>> ANALIZA: " << tip << " (Media pe " << nrTeste << " teste)" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int i = 1; i <= nrTeste; i++) {
        string path = "teste/" + tip + "/test_" + to_string(i) + ".txt";
        ifstream fin(path);
        vector<int> v; int val;
        while (fin >> val) v.push_back(val);
        fin.close();

        if (v.empty()) continue;

        tQ   += measure(quickSort, v);
        tStd += measure(introSort, v);
        tM   += measure(mergeSort, v);
        tI   += measure(insertionSort, v);
        tS   += measure(selectionSort, v);
        tB   += measure(bubbleSort, v);
    }

    cout << fixed << setprecision(4);
    cout << left << setw(15) << "IntroSort(std):" << tStd / nrTeste << " us" << endl;
    cout << left << setw(15) << "QuickSort:"     << tQ / nrTeste << " us" << endl;
    cout << left << setw(15) << "MergeSort:"     << tM / nrTeste << " us" << endl;
    cout << left << setw(15) << "Insertion:"     << tI / nrTeste << " us" << endl;
    cout << left << setw(15) << "Selection:"     << tS / nrTeste << " us" << endl;
    cout << left << setw(15) << "Bubble:"        << tB / nrTeste << " us" << endl;
    cout << "--------------------------------------------------------" << endl;
}

int main() {
    int nTeste = 5;
    vector<string> tipuri = {"random", "sorted", "reversed", "almost_sorted", "plateau"};

    for (const string& t : tipuri) {
        runBenchmark(t, nTeste);
    }

    return 0;
}