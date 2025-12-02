#include "RedeLogistica.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <cstdlib> 
#include <algorithm>

RedeLogistica::RedeLogistica() {
    carregarDadosCapitais();
    N = cidades.size();
    matrizAdjacencia.resize(N, vector<bool>(N, false));
    matrizDistanciasFisicas.resize(N, vector<double>(N, 0.0));
    matrizBackup.resize(N, vector<bool>(N, false));
    calcularDistanciasFisicas();
    //Eficiência tem peso alto para forçar a criação de atalhos
    pesoCustoConstrucao = 1.0;
    pesoEficienciaViagem = 500.0; 
}

void RedeLogistica::carregarDadosCapitais() {
    cidades = {
        {"AC", -9.97, -67.81}, {"AL", -9.64, -35.73}, {"AM", -3.11, -60.01},
        {"AP", 0.02, -51.06},  {"BA", -12.96, -38.50}, {"CE", -3.75, -38.53},
        {"DF", -15.79, -47.88}, {"ES", -20.29, -40.30}, {"GO", -16.68, -49.24},
        {"MA", -2.55, -44.25}, {"MT", -15.60, -56.09}, {"MS", -20.46, -54.62},
        {"MG", -19.91, -43.93}, {"PA", -1.46, -48.47}, {"PB", -7.12, -34.86},
        {"PR", -25.44, -49.27}, {"PE", -8.06, -34.88}, {"PI", -5.06, -42.80},
        {"RJ", -22.90, -43.22}, {"RN", -5.81, -35.20}, {"RO", -8.76, -63.89},
        {"RS", -30.05, -51.18}, {"RR", 2.82, -60.67},  {"SC", -27.59, -48.57},
        {"SE", -10.93, -37.07}, {"SP", -23.56, -46.64}, {"TO", -10.24, -48.32}
    };
}

void RedeLogistica::calcularDistanciasFisicas() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            double dx = cidades[i].lat - cidades[j].lat;
            double dy = cidades[i].lon - cidades[j].lon;
            matrizDistanciasFisicas[i][j] = sqrt(dx*dx + dy*dy);
        }
    }
}

void RedeLogistica::salvarEstado() { matrizBackup = matrizAdjacencia; }
void RedeLogistica::restaurarEstado() { matrizAdjacencia = matrizBackup; }

void RedeLogistica::perturbarSolucao(int forca) {
    for (int k = 0; k < forca; k++) {
        int i = rand() % N; int j = rand() % N; if (i == j) continue;
        matrizAdjacencia[i][j] = !matrizAdjacencia[i][j];
        matrizAdjacencia[j][i] = !matrizAdjacencia[j][i];
    }
}

// Função Objetivo //
bool RedeLogistica::verificarConectividade() {
    vector<bool> visitado(N, false);
    vector<int> fila; fila.push_back(0); visitado[0] = true;
    int visitadosCount = 1; int head = 0;
    while(head < (int)fila.size()){
        int u = fila[head++];
        for(int v=0; v<N; v++){
            if(matrizAdjacencia[u][v] && !visitado[v]){
                visitado[v] = true; visitadosCount++; fila.push_back(v);
            }
        }
    }
    return visitadosCount == N;
}

double RedeLogistica::calcularMediaViagem() {
    vector<vector<double>> dist = matrizDistanciasFisicas;
    for(int i=0; i<N; i++) for(int j=0; j<N; j++) if(i!=j && !matrizAdjacencia[i][j]) dist[i][j] = 1e9; 
    for (int k = 0; k < N; k++) for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) 
        if (dist[i][k] + dist[k][j] < dist[i][j]) dist[i][j] = dist[i][k] + dist[k][j];
    double soma = 0; int pares = 0;
    for(int i=0; i<N; i++) for(int j=i+1; j<N; j++) if(dist[i][j] < 1e8) { soma += dist[i][j]; pares++; }
    return (pares > 0) ? soma / pares : 1e9;
}

double RedeLogistica::calcularCustoTotalConstrucao() {
    double custo = 0;
    for(int i=0; i<N; i++) for(int j=i+1; j<N; j++) if(matrizAdjacencia[i][j]) custo += matrizDistanciasFisicas[i][j];
    return custo;
}

double RedeLogistica::calcularFuncaoAvaliacao() {
    if (!verificarConectividade()) return 1e9; 
    return (calcularCustoTotalConstrucao() * pesoCustoConstrucao) + (calcularMediaViagem() * pesoEficienciaViagem);
}


// Construções //
void RedeLogistica::gerarSolucaoInicialMST() {
    for(auto &row : matrizAdjacencia) fill(row.begin(), row.end(), false);
    vector<double> minPeso(N, numeric_limits<double>::max());
    vector<int> pai(N, -1); vector<bool> visitado(N, false); minPeso[0] = 0; 
    for (int i = 0; i < N - 1; i++) {
        int u = -1;
        for (int v = 0; v < N; v++) if (!visitado[v] && (u == -1 || minPeso[v] < minPeso[u])) u = v;
        visitado[u] = true;
        for (int v = 0; v < N; v++) if (!visitado[v] && matrizDistanciasFisicas[u][v] < minPeso[v]) {
                pai[v] = u; minPeso[v] = matrizDistanciasFisicas[u][v];
            }
    }
    for (int i = 1; i < N; i++) if (pai[i] != -1) { matrizAdjacencia[i][pai[i]] = true; matrizAdjacencia[pai[i]][i] = true; }
}

void RedeLogistica::gerarSolucaoGRASP(float alpha) {
    for(auto &row : matrizAdjacencia) fill(row.begin(), row.end(), false);
    vector<bool> visitado(N, false); visitado[0] = true; int visitadosCount = 1;
    while (visitadosCount < N) {
        vector<ArestaCandidata> candidatos;
        for (int u = 0; u < N; u++) if (visitado[u]) for (int v = 0; v < N; v++) 
            if (!visitado[v]) candidatos.push_back({u, v, matrizDistanciasFisicas[u][v]});
        sort(candidatos.begin(), candidatos.end());
        int limit = max(1, (int)(candidatos.size() * alpha));
        int escolhidoIdx = rand() % limit;
        ArestaCandidata escolhida = candidatos[escolhidoIdx];
        matrizAdjacencia[escolhida.u][escolhida.v] = true; matrizAdjacencia[escolhida.v][escolhida.u] = true;
        visitado[escolhida.v] = true; visitadosCount++;
    }
}


// Busca Local (Hill Climbing)

void RedeLogistica::resolverHillClimbing(int iteracoesMaximas) {
    double custoAtual = calcularFuncaoAvaliacao();
    for (int it = 0; it < iteracoesMaximas; it++) {
        int i = rand() % N; int j = rand() % N; if (i == j) continue;
        matrizAdjacencia[i][j] = !matrizAdjacencia[i][j]; matrizAdjacencia[j][i] = !matrizAdjacencia[j][i];
        double novoCusto = calcularFuncaoAvaliacao();
        if (novoCusto < custoAtual) custoAtual = novoCusto; 
        else { matrizAdjacencia[i][j] = !matrizAdjacencia[i][j]; matrizAdjacencia[j][i] = !matrizAdjacencia[j][i]; }
    }
}


// Meta-heurísticas //
void RedeLogistica::resolverILS(int ciclosExternos, int iterInternas, int forca) {
    gerarSolucaoInicialMST();
    double melhorCusto = calcularFuncaoAvaliacao();
    salvarEstado();

    for (int iter = 0; iter < ciclosExternos; iter++) {
        resolverHillClimbing(iterInternas); // Busca Local
        
        double custo = calcularFuncaoAvaliacao();
        if (custo < melhorCusto) {
            melhorCusto = custo;
            salvarEstado();
        } else {
            restaurarEstado();
        }
        perturbarSolucao(forca); // Perturbação
    }
    restaurarEstado();
}

void RedeLogistica::resolverGRASP_Metaheuristica(int ciclosExternos, int iterInternas, float alpha) {
    double melhorCustoGlobal = 1e18;
    for(int iter = 0; iter < ciclosExternos; iter++) {
        gerarSolucaoGRASP(alpha); // Construção Aleatorizada
        
        resolverHillClimbing(iterInternas); // Busca Local
        
        double custoAtual = calcularFuncaoAvaliacao();
        if(custoAtual < melhorCustoGlobal) {
            melhorCustoGlobal = custoAtual;
            salvarEstado();
        }
    }
    restaurarEstado();
}

void RedeLogistica::resolverVNS(int ciclosExternos, int iterInternas) {
    gerarSolucaoInicialMST();
    double melhorCusto = calcularFuncaoAvaliacao();
    salvarEstado();
    int k_max = 3;
    
    for (int iter = 0; iter < ciclosExternos; iter++) {
        int k = 1;
        while (k <= k_max) {
            restaurarEstado(); 
            perturbarSolucao(k); // Agitação (Shake)
            
            resolverHillClimbing(iterInternas); // Busca Local
            
            double custoCandidato = calcularFuncaoAvaliacao();
            if (custoCandidato < melhorCusto) {
                melhorCusto = custoCandidato;
                salvarEstado();
                k = 1; // Voltar para vizinhança mais fina
            } else {
                k++; // Tentar vizinhança mais distante
            }
        }
    }
    restaurarEstado();
}

// Função para salvar os resultados em um arquivo CSV para gerar os grafos // 
void RedeLogistica::exportarParaArquivo(string nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo: " << nomeArquivo << "\n";
        return;
    }
    arquivo << "Origem,Destino\n";

    for(int i = 0; i < N; i++) {
        for(int j = i + 1; j < N; j++) {
            if (matrizAdjacencia[i][j]) {
                arquivo << cidades[i].sigla << "," << cidades[j].sigla << "\n";
            }
        }
    }
    
    arquivo.close();
    cout << "--> Arquivo gerado: " << nomeArquivo << "\n";
}

void RedeLogistica::imprimirSolucao() {
    cout << "Solucao Final: " << fixed << setprecision(2) << calcularFuncaoAvaliacao() << "\n";
}