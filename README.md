# Huffman Image Compressor

Huffman Image Compressor é uma aplicação em C com interface gráfica construída usando GTK que realiza compressão lossless de imagens utilizando o algoritmo Huffman Coding.

O programa suporta imagens nos formatos:

* JPEG
* PNG
* PPM

A compressão média observada no projeto é de aproximadamente 50% de redução de tamanho, sem perda de qualidade, pois o algoritmo Huffman é lossless.

## Como executar o projeto

### 1. Instalar dependências

Em sistemas Linux (Ubuntu/Debian):

```bash
sudo apt update
sudo apt install build-essential libgtk-4-dev libjpeg-dev libpng-dev
```

Em sistemas Fedora (Workstation):

```bash
sudo dnf update
sudo dnf install gcc make gtk4-devel libjpeg-turbo-devel libpng-devel

Bibliotecas utilizadas:

* GTK (GTK4)
* libjpeg
* libpng

### 2. Compilar o projeto

O projeto utiliza Makefile para automação de compilação.

Execute:

```bash
make
```

Isso irá gerar o executável na pasta:

`bin/compressor`

### 3. Executar o programa

```bash
./bin/compressor
```

Ao executar, uma interface gráfica será aberta.


## Interface Gráfica

A interface foi desenvolvida utilizando GTK.

Ela contém dois botões principais:

### Comprimir imagem

Abre um seletor de arquivos permitindo escolher uma imagem suportada.

O programa então:

1. lê a imagem
2. gera um arquivo comprimido .huf
3. reconstrói a imagem comprimida
4. salva a imagem final na pasta Downloads

### Sair

Fecha a aplicação.


## Estrutura do projeto

```
Huffman Image Compressor
│
├── include
│   ├── bit_io.h
│   ├── huffman.h
│   └── image.h
│
├── src
│   ├── bit_io.c
│   ├── huffman.c
│   ├── image.c
│   └── main.c
│
├── bin
│   └── compressor
│
├── Makefile
└── README.md
```

## Fluxo de compressão

Quando uma imagem é selecionada, o processo ocorre da seguinte forma:

```
Imagem original
      │
      ▼
Leitura da imagem (PNG/JPEG/PPM)
      │
      ▼
Contagem de frequência dos bytes
      │
      ▼
Construção da árvore de Huffman
      │
      ▼
Codificação binária dos dados
      │
      ▼
Arquivo temporário .huf
      │
      ▼
Descompressão automática
      │
      ▼
Imagem comprimida salva
```

O arquivo .huf é apenas um formato intermediário e é removido após a reconstrução da imagem.

## Algoritmo de Huffman

O algoritmo Huffman Coding é um método de compressão sem perda baseado na frequência dos símbolos.

Funcionamento simplificado:

1. Conta quantas vezes cada valor aparece nos dados
2. Constrói uma árvore binária baseada nessas frequências
3. Atribui códigos binários menores para símbolos mais frequentes
4. Substitui os dados originais pelos códigos

Exemplo:

| Valor | Frequência | Código |
|-------|------------|--------|
| A     | 50         | 0      |
| B     | 25         | 10     |
| C     | 25         | 11     |

Como valores frequentes usam menos bits, o tamanho total dos dados diminui.

## Formato do arquivo .huf

Durante a compressão, é gerado um arquivo intermediário:

`imagem.huf`

Ele contém:

* header
* informações da imagem
* tabela de frequência
* dados comprimidos em bits

Esse arquivo é utilizado para reconstruir a imagem final e é removido automaticamente após o processo.

## Resultados de compressão

Em testes realizados no projeto:

| Imagem | Tamanho original | Tamanho comprimido | Redução |
|--------|------------------|--------------------|---------|
| PNG    | 5.2 MB           | 2.5 MB             | ~50%    |
| JPEG   | 2.9 MB           | 1.5 MB             | ~50%    |

A compressão é lossless, ou seja:

* ✔ quase nenhuma perda de qualidade
* ✔ pixels quase idênticos ao original