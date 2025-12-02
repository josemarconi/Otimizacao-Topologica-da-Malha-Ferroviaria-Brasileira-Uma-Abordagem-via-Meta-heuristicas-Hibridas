#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>
#include <string>
#include "RedeLogistica.hpp"

// Buscas Puras
const int ITER_HC_PURO = 10000;

// Meta-heurísticas
const int CICLOS_META = 30;
const int ITER_BUSCA_INTERNA = 500;

// Parâmetros Específicos
const int ILS_FORCA = 3;
const float GRASP_ALPHA = 0.2f;

struct Resultado
{
    string nome;
    double custo;
    double tempo;
    double gap;
};

int main()
{
    srand(time(nullptr));
    RedeLogistica rede;
    vector<Resultado> tabela;

    cout << "    EXPERIMENTO FINAL (SELECIONADOS): HC, ILS, GRASP, VNS\n";
    cout << "=========================================================\n\n";

    // 1. BASELINE: MST
    cout << "[1] Baseline MST (Kruskal/Prim)...\n";
    rede.gerarSolucaoInicialMST();
    double custoMST = rede.calcularFuncaoAvaliacao();
    tabela.push_back({"MST (Inicial)", custoMST, 0.0, 0.0});
    rede.exportarParaArquivo("grafo_mst.txt");
    // 2. BUSCA LOCAL PURA
    cout << "[2] Hill Climbing Puro...\n";
    rede.gerarSolucaoInicialMST();
    clock_t t1 = clock();
    rede.resolverHillClimbing(ITER_HC_PURO);
    clock_t t2 = clock();
    double cHC = rede.calcularFuncaoAvaliacao();
    tabela.push_back({"HC Puro", cHC, (double)(t2 - t1) / CLOCKS_PER_SEC, (custoMST - cHC) / custoMST * 100.0});
    rede.exportarParaArquivo("grafo_hc.txt");
    // 3. META-HEURÍSTICAS (Todas usando HC)
    cout << "[3] Meta-heuristicas Avancadas...\n";

    // ILS
    cout << "   - Rodando ILS...\n";
    clock_t t3 = clock();
    rede.resolverILS(CICLOS_META, ITER_BUSCA_INTERNA, ILS_FORCA);
    clock_t t4 = clock();
    double cILS = rede.calcularFuncaoAvaliacao();
    tabela.push_back({"ILS", cILS, (double)(t4 - t3) / CLOCKS_PER_SEC, (custoMST - cILS) / custoMST * 100.0});
    rede.exportarParaArquivo("grafo_ils.txt");
    // GRASP
    cout << "   - Rodando GRASP...\n";
    clock_t t5 = clock();
    rede.resolverGRASP_Metaheuristica(CICLOS_META, ITER_BUSCA_INTERNA, GRASP_ALPHA);
    clock_t t6 = clock();
    double cGRASP = rede.calcularFuncaoAvaliacao();
    tabela.push_back({"GRASP", cGRASP, (double)(t6 - t5) / CLOCKS_PER_SEC, (custoMST - cGRASP) / custoMST * 100.0});
    rede.exportarParaArquivo("grafo_grasp.txt");
    // VNS
    cout << "   - Rodando VNS...\n";
    clock_t t7 = clock();
    rede.resolverVNS(CICLOS_META, ITER_BUSCA_INTERNA);
    clock_t t8 = clock();
    double cVNS = rede.calcularFuncaoAvaliacao();
    tabela.push_back({"VNS", cVNS, (double)(t8 - t7) / CLOCKS_PER_SEC, (custoMST - cVNS) / custoMST * 100.0});
    rede.exportarParaArquivo("grafo_vns.txt");
    // --- RELATÓRIO FINAL ---
    cout << "\n\n";
    cout << "====================================================================\n";
    cout << "                      TABELA COMPARATIVA FINAL                      \n";
    cout << "====================================================================\n";
    cout << "| Metodo         | Custo Final     | Tempo (s) | Gap (Melhoria %) |\n";
    cout << "|----------------|-----------------|-----------|------------------|\n";

    for (const auto &r : tabela)
    {
        cout << "| " << left << setw(14) << r.nome
                  << " | " << right << setw(15) << fixed << setprecision(2) << r.custo
                  << " | " << setw(9) << setprecision(3) << r.tempo
                  << " | " << setw(14) << setprecision(2) << r.gap << " % |\n";
    }
    cout << "====================================================================\n";

    return 0;
}