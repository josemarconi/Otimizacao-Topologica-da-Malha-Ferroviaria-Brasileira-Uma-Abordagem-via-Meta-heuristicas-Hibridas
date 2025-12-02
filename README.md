# Otimização Topológica da Malha Ferroviária Brasileira: Uma Abordagem via Meta-heurísticas Híbridas

Este repositório contém a implementação computacional de algoritmos de otimização combinatória aplicados ao *Network Design Problem* (Problema de Projeto de Redes). O objetivo é projetar uma topologia de rede ferroviária conectando as 27 capitais brasileiras, minimizando custos de construção e maximizando a eficiência logística.

Trabalho desenvolvido como requisito da disciplina de **Computação Evolucionária / Pesquisa Operacional** do curso de Engenharia de Computação do **CEFET-MG**.

## 📋 Sobre o Projeto

O projeto utiliza a linguagem C++ para resolver o problema bicritério (Custo vs. Tempo de Viagem) utilizando as seguintes abordagens:

1.  **MST (Minimum Spanning Tree):** Solução inicial construtiva (Baseline).
2.  **Hill Climbing (HC):** Busca local estocástica (Bit-flip).
3.  **ILS (Iterated Local Search):** Meta-heurística baseada em perturbação e busca local.
4.  **GRASP:** Construção gulosa aleatorizada seguida de busca local.
5.  **VNS (Variable Neighborhood Search):** Mudança sistemática de vizinhança.

Os resultados são exportados para arquivos de texto e visualizados graficamente via script Python.

## 🚀 Estrutura do Repositório

O projeto está organizado em módulos conforme exigido:

* **`src/` (ou raiz):**
    * `main.cpp`: Arquivo principal que gerencia a execução de todos os algoritmos.
    * `RedeLogistica.cpp`: Implementação dos métodos da classe (lógica do grafo).
    * `RedeLogistica.hpp`: Arquivo de cabeçalho com definições das estruturas e classe.
* **`Makefile`**: Script de automação de compilação para ambiente Linux.
* **`plot_grafos.ipynb`**: Notebook Python para gerar as visualizações gráficas (PNG) a partir das saídas do C++.
* **Arquivos de Saída Gerados**: `grafo_mst.txt`, `grafo_hc.txt`, `grafo_ils.txt`, etc.

## ⚙️ Pré-requisitos

Para executar este projeto, você precisará de um ambiente **Linux** com os seguintes pacotes instalados:

* **G++**: Compilador C++ (suporte a C++11 ou superior).
* **Make**: Ferramenta de automação de compilação.
* **Python 3** (Opcional, apenas para gerar os gráficos):
    * Bibliotecas: `networkx`, `matplotlib`.

## 🛠️ Compilação e Execução (Linux)

O projeto inclui um `Makefile` configurado. Siga os passos abaixo no terminal:

### 1. Limpar compilações anteriores
Para garantir uma compilação limpa, execute:
```bash
make clean
```
### 2. Compilar o projeto
Utilize o comando `make` para compilar os módulos e gerar o binário executável `./Upcv`:

```bash
make
```

### 3. Executar o projeto
Depois de gerado o executável, basta jogar o seguinte comando para executar `./tsp`:

```bash
./Upcv
```

## 📝 Notas sobre Entrada e Saída

* **Entrada de Dados:** Para este problema específico, os dados de entrada (coordenadas geográficas das 27 capitais conforme dados do IBGE) estão estruturados internamente na classe `RedeLogistica` (método `carregarDadosCapitais`) para garantir a integridade e precisão dos dados georreferenciados, dispensando a leitura de arquivo externo nesta versão.
* **Arquivos de Saída:** Além do relatório textual no terminal (stdout), o programa gera automaticamente arquivos de texto com as topologias resultantes para plotagem:
    * `grafo_mst.txt`
    * `grafo_hc.txt`
    * `grafo_ils.txt`
    * `grafo_grasp.txt`
    * `grafo_vns.txt`

## 📊 Visualização dos Resultados

Após a execução do código C++, você pode visualizar as malhas ferroviárias geradas:

1.  Certifique-se de ter o Python e as bibliotecas instaladas:
    ```bash
    pip install networkx matplotlib
    ```

2.  Execute o script/notebook `plot_grafos.ipynb` (ou o script `.py`) para ler os arquivos `.txt` gerados e salvar as imagens `.png` dos mapas.

## ✍️ Autor

**José Marconi de Almeida Júnior**
Engenharia de Computação – CEFET-MG
Divinópolis, 2025.


