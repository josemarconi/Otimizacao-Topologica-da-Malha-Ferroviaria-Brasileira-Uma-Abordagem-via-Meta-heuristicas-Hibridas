#ifndef REDELOGISTICA_HPP
#define REDELOGISTICA_HPP

#include <vector>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

struct Cidade {
    string sigla;
    double lat;
    double lon;
};

struct ArestaCandidata {
    int u, v;
    double peso;
    bool operator<(const ArestaCandidata& other) const { //Para calcular o peso das arestas//
        return peso < other.peso;
    }
};

class RedeLogistica {
private:
    int N;
    vector<Cidade> cidades;
    vector<vector<bool>> matrizAdjacencia;
    vector<vector<double>> matrizDistanciasFisicas;
    vector<vector<bool>> matrizBackup;

    double pesoCustoConstrucao;
    double pesoEficienciaViagem;

    void carregarDadosCapitais();
    void calcularDistanciasFisicas();
    bool verificarConectividade();
    double calcularCustoTotalConstrucao();
    double calcularMediaViagem();

    void salvarEstado();
    void restaurarEstado();
    void perturbarSolucao(int forca);

public:
    RedeLogistica();

    // Contruções
    void gerarSolucaoInicialMST();
    void gerarSolucaoGRASP(float alpha);

    // Função de Avaliação
    double calcularFuncaoAvaliacao();

    // Busca Local Principal
    void resolverHillClimbing(int iteracoesMaximas);

    // Meta-heurísticas (Todas usam Hill Climbing internamente)
    void resolverILS(int ciclosExternos, int iterInternas, int forca);
    void resolverGRASP_Metaheuristica(int ciclosExternos, int iterInternas, float alpha);
    void resolverVNS(int ciclosExternos, int iterInternas);
    void exportarParaArquivo(string nomeArquivo);
    void imprimirSolucao();
};

#endif