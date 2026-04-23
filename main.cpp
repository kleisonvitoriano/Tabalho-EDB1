#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct Ponto {
    double x; 
    double y; 
};

double func_linear(double x) { return x; }
double func_quadratica(double x) { return x * x; }
double func_nlogn(double x) { return x * log2(x); }
double func_logaritmica(double x) { return log2(x); }

double calcularConstanteAjuste(const vector<Ponto>& dados, double (*func)(double)) {
    double soma_yf = 0;
    double soma_f2 = 0;
    for (const auto& p : dados) {
        double f_val = func(p.x);
        soma_yf += p.y * f_val;
        soma_f2 += f_val * f_val;
    }
    return soma_yf / soma_f2; 
}

void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) { arr[k] = L[i]; i++; } 
        else { arr[k] = R[j]; j++; }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
}

void mergeSortWrapper(vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

int buscaSequencial(const vector<int>& arr, int target) {
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int buscaBinaria(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

vector<int> gerarVetorAleatorio(int n) {
    vector<int> arr(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, n * 10);
    for (int i = 0; i < n; i++) arr[i] = dis(gen);
    return arr;
}

vector<Ponto> testarOrdenacao(const string& nome, void (*algoritmo)(vector<int>&), vector<double> tamanhos) {
    vector<Ponto> resultados;
    int repeticoes = 5;
    for (double n : tamanhos) {
        double tempo_total = 0;
        for (int r = 0; r < repeticoes; r++) {
            vector<int> arr = gerarVetorAleatorio((int)n);
            auto inicio = high_resolution_clock::now();
            algoritmo(arr);
            auto fim = high_resolution_clock::now();
            duration<double, std::milli> tempo = fim - inicio;
            tempo_total += tempo.count();
        }
        double tempo_medio = tempo_total / repeticoes;
        cout << nome << " | N = " << n << " | Tempo: " << tempo_medio << " ms\n";
        resultados.push_back({n, tempo_medio});
    }
    return resultados;
}

vector<Ponto> testarBusca(const string& nome, int (*algoritmo)(const vector<int>&, int), vector<double> tamanhos) {
    vector<Ponto> resultados;
    int num_buscas = 10000;
    random_device rd;
    mt19937 gen(rd());

    for (double n : tamanhos) {
        vector<int> arr = gerarVetorAleatorio((int)n);
        sort(arr.begin(), arr.end());
        uniform_int_distribution<> dis(1, n * 10);

        auto inicio = high_resolution_clock::now();
        for (int i = 0; i < num_buscas; i++) {
            algoritmo(arr, dis(gen));
        }
        auto fim = high_resolution_clock::now();
        
        duration<double, std::milli> tempo = fim - inicio;
        double tempo_medio = (tempo.count() / num_buscas) * 1000.0; 
        cout << nome << " | N = " << n << " | Tempo: " << tempo_medio << " us\n";
        resultados.push_back({n, tempo_medio});
    }
    return resultados;
}

string gerarJSArray_X(const vector<Ponto>& dados) {
    string res = "[";
    for (size_t i = 0; i < dados.size(); i++) {
        res += to_string((int)dados[i].x) + (i < dados.size() - 1 ? "," : "");
    }
    return res + "]";
}

string gerarJSArray_Y(const vector<Ponto>& dados) {
    string res = "[";
    for (size_t i = 0; i < dados.size(); i++) {
        res += to_string(dados[i].y) + (i < dados.size() - 1 ? "," : "");
    }
    return res + "]";
}

string gerarJSArray_Y_Teorico(const vector<Ponto>& dados, double c, double (*func)(double)) {
    string res = "[";
    for (size_t i = 0; i < dados.size(); i++) {
        res += to_string(c * func(dados[i].x)) + (i < dados.size() - 1 ? "," : "");
    }
    return res + "]";
}

void gerarHTMLGraficos(
    const vector<Ponto>& res_ins, double c_ins,
    const vector<Ponto>& res_mer, double c_mer,
    const vector<Ponto>& res_seq, double c_seq,
    const vector<Ponto>& res_bin, double c_bin
) {
    ofstream html("graficos.html");

    html << "<!DOCTYPE html><html><head><title>Resultados de Complexidade</title>\n"
         << "<script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>\n"
         << "<style>body{font-family: Arial; padding: 20px;} .chart-container{width: 800px; margin: 40px auto;}</style>\n"
         << "</head><body>\n<h1 style='text-align:center;'>Análise Empírica de Complexidade</h1>\n";

    auto gerarBlocoGrafico = [&](string id, string titulo, string lblY, const vector<Ponto>& dados, double c, double (*func)(double), string nomeTeorico) {
        html << "<div class='chart-container'><canvas id='" << id << "'></canvas></div>\n"
             << "<script>\n"
             << "new Chart(document.getElementById('" << id << "'), { type: 'line', data: {\n"
             << "  labels: " << gerarJSArray_X(dados) << ",\n"
             << "  datasets: [\n"
             << "    { label: 'Tempo Empírico', data: " << gerarJSArray_Y(dados) << ", borderColor: 'red', fill: false, tension: 0.1 },\n"
             << "    { label: 'Teórico Ajustado " << nomeTeorico << "', data: " << gerarJSArray_Y_Teorico(dados, c, func) << ", borderColor: 'blue', borderDash: [5, 5], fill: false }\n"
             << "  ]\n"
             << "}, options: { plugins: { title: { display: true, text: '" << titulo << "', font: {size: 18} } }, scales: { y: { title: { display: true, text: '" << lblY << "' } }, x: { title: { display: true, text: 'Tamanho da Entrada (N)' } } } } });\n"
             << "</script>\n";
    };

    gerarBlocoGrafico("chart1", "Insertion Sort", "Tempo (ms)", res_ins, c_ins, func_quadratica, "(O(n²))");
    gerarBlocoGrafico("chart2", "Merge Sort", "Tempo (ms)", res_mer, c_mer, func_nlogn, "(O(n log n))");
    gerarBlocoGrafico("chart3", "Busca Sequencial", "Tempo (us)", res_seq, c_seq, func_linear, "(O(n))");
    gerarBlocoGrafico("chart4", "Busca Binária", "Tempo (us)", res_bin, c_bin, func_logaritmica, "(O(log n))");

    html << "</body></html>\n";
    html.close();
}

int main() {
    cout << "=== INICIANDO ANALISE EMPIRICA EM C++ ===\n\n";

    vector<double> tamanhos_ord = {1000, 5000, 10000, 20000, 40000};
    vector<double> tamanhos_busca = {10000, 50000, 100000, 500000, 1000000, 5000000};

    cout << "Testando Insertion Sort...\n";
    vector<Ponto> res_ins = testarOrdenacao("InsertionSort", insertionSort, tamanhos_ord);
    double c_ins = calcularConstanteAjuste(res_ins, func_quadratica);

    cout << "\nTestando Merge Sort...\n";
    vector<Ponto> res_mer = testarOrdenacao("MergeSort", mergeSortWrapper, tamanhos_ord);
    double c_mer = calcularConstanteAjuste(res_mer, func_nlogn);

    cout << "\nTestando Busca Sequencial...\n";
    vector<Ponto> res_seq = testarBusca("BuscaSequencial", buscaSequencial, tamanhos_busca);
    double c_seq = calcularConstanteAjuste(res_seq, func_linear);

    cout << "\nTestando Busca Binaria...\n";
    vector<Ponto> res_bin = testarBusca("BuscaBinaria", buscaBinaria, tamanhos_busca);
    double c_bin = calcularConstanteAjuste(res_bin, func_logaritmica);

    cout << "\nA gerar ficheiro de graficos...\n";
    gerarHTMLGraficos(res_ins, c_ins, res_mer, c_mer, res_seq, c_seq, res_bin, c_bin);

    cout << "=== ANALISE CONCLUIDA ===\n";
    cout << "Abre o ficheiro 'graficos.html' gerado na mesma pasta no teu navegador de Internet (Chrome, Firefox, etc.) para veres os graficos interativos!\n";

    return 0;
}