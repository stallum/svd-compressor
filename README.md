🖼️ Compressor de Imagem SVD

Este projeto é um sistema de compressão de imagens em tons de cinza e coloridas (RGB) utilizando a técnica matemática de Decomposição em Valores Singulares (SVD). Desenvolvido em C, o projeto foca em manipulação eficiente de memória através de ponteiros e integração com bibliotecas de álgebra linear.

A ideia central é reduzir a redundância de dados representando a matriz da imagem A como o produto de três matrizes:
A=UΣVT

A compressão ocorre ao mantermos apenas os k valores singulares mais significativos de Σ, descartando informações menos relevantes e reduzindo o peso do arquivo final.
📂 Estrutura do Projeto

De acordo com a organização atual do diretório:
Diretório/Arquivo	Descrição
src/	Arquivos fonte .c (Lógica principal, IO de imagem e matrizes).
include/	Cabeçalhos .h (Definições de structs e protótipos).
bin/	Executáveis gerados (ex: compressor).
build/	Objetos compilados .o (Arquivos temporários).
Example/	Imagens de teste (como o luna.jpg).
data/ / docs/	Documentação adicional e logs de dados.
Makefile	Script de automação da compilação.
🛠️ Requisitos e Instalação

O projeto depende da GSL (GNU Scientific Library) para os cálculos de SVD e da stb_image (já inclusa em include/) para manipulação de arquivos de imagem.
🐧 No Linux (Ubuntu/Debian/Fedora)

    Instalar dependências:
    Bash
    
    # Ubuntu/Debian
    sudo apt update && sudo apt install build-essential libgsl-dev
    
    # Fedora
    
    sudo dnf install gcc make gsl-devel
    
    Compilar o projeto:

    make
    Executar:


    ./bin/compressor
🪟 No Windows

A maneira mais estável de rodar este projeto no Windows é via WSL2 (Windows Subsystem for Linux) ou MSYS2.

Via MSYS2 (UCRT64):

    Abra o terminal do MSYS2 UCRT64.

    Instale o compilador e a GSL:
    Bash

    pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-gsl

    No diretório do projeto, compile usando:
    Bash

    mingw32-make

🚀 Como Funciona a Compilação

O Makefile automatizado gerencia as dependências. Os principais comandos são:

    make: Compila o programa principal e gera o binário em bin/.

    make testar: Compila o arquivo testar.c para verificações rápidas de funções de matriz.

    make clean: Remove as pastas build/ e bin/ para uma compilação limpa.

🤝 Colaboração e Regras do Projeto

Este projeto foi desenvolvido seguindo padrões acadêmicos de Ciência da Computação:

    Gerenciamento de Memória: Toda matriz alocada deve ser liberada via free_matrix ou free_svd_image para evitar memory leaks.

    Modularização: A lógica de álgebra linear está isolada em matrix.c, enquanto a compressão reside em svd.c.

    Portabilidade: O uso de gsl-config no Makefile garante que os caminhos das bibliotecas sejam encontrados automaticamente no Linux.

    Nota: O projeto utiliza a biblioteca stb_image.h para suporte a formatos .jpg, .png e .bmp. Certifique-se de que as imagens de teste estejam na pasta Example/.