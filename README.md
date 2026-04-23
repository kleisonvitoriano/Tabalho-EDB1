# Análise Empírica de Complexidade de Algoritmos

**Disciplina:** Estruturas de Dados Básicos I
<br>
**Alunos:** Kleison Vitoriano da Silva e João Vitor da Silva Cunha
<br>
**Instituição:** Instituto Metrópole Digital (IMD/UFRN)

---

## 1. Sobre o Projeto

Este projeto consiste numa aplicação desenvolvida em C++ para avaliar empiricamente o tempo de execução de algoritmos clássicos de ordenação e busca. O objetivo principal é confrontar o comportamento real dos algoritmos com as suas previsões teóricas de complexidade (Notação Big-O).

## 2. Algoritmos Analisados

O sistema implementa e analisa os seguintes algoritmos:

### Ordenação
- **Insertion Sort**: Complexidade teórica `O(n²)`. Ideal para conjuntos pequenos de dados.
- **Merge Sort**: Complexidade teórica `O(n log n)`. Algoritmo de divisão e conquista altamente eficiente para grandes volumes de dados.

### Busca
- **Busca Sequencial**: Complexidade teórica `O(n)`. Realiza uma varredura linear no vetor.
- **Busca Binária**: Complexidade teórica `O(log n)`. Requer que o vetor esteja previamente ordenado.

## 3. Funcionalidades

- **Medição Automatizada**: Utiliza a biblioteca `<chrono>` para capturar tempos de execução precisos.
- **Tratamento Estatístico**: Realiza múltiplas repetições (ex: 5 para ordenação, 10.000 para busca) e calcula a média para mitigar interferências do sistema operativo.
- **Ajuste Matemático (Curve Fitting)**: Implementa o Método dos Mínimos Quadrados em C++ para calcular a constante de proporcionalidade ideal entre os dados empíricos e as funções teóricas.
- **Visualização Dinâmica**: Gera automaticamente um ficheiro `graficos.html` com gráficos interativos utilizando a biblioteca Chart.js.

## 4. Tecnologias Utilizadas

- **Linguagem Principal**: C++ (C++11 ou superior).
- **Geração de Gráficos**: HTML5 e JavaScript (Chart.js via CDN).
- **Documentação**: Markdown e Google Docs (exportado para PDF).

## 5. Como Compilar e Executar

### Pré-requisitos
- Compilador C++ (GCC/G++ ou MinGW).
- Navegador de Internet (para visualizar os gráficos).

### Instruções
1. **Compilação**:
   ```bash
   g++ -O3 main.cpp -o trabalho
