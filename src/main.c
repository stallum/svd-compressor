#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"
#include "image.h"

//Carrega a imagem e e olha a extensão, dependendo da extensão chama a função correspondente.
//Se o arquivo não for compatível avisa no console
Image loadImage(const char *filename){
    char *ext = strrchr(filename, '.');
    if(!ext){
        g_print("Formato desconhecido\n");
        exit(1);
    }
    if(strcmp(ext, ".ppm") == 0) return loadPPM(filename);
    if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return loadJPEG(filename);
    if(strcmp(ext, ".png") == 0) return loadPNG(filename);

    g_print("Formato nao suportado\n");
    exit(1);
}


//Pega o nome da imagem tirando a extensão, para colocar no nome da imagem comprimida
void getBaseName(const char *input, char *base){
    strcpy(base, input);
    char *dot = strrchr(base, '.');
    if(dot) *dot = '\0';
}

//Encerra o programa ao apertar o botão
void sair(GtkWidget *botao, gpointer app) {
    (void)botao; 
    g_application_quit(G_APPLICATION(app));
}

void arquivo_selecionado(GtkFileDialog *dialog, GAsyncResult *result, gpointer data) {

    GtkWindow *parent_window = GTK_WINDOW(data);
    GFile *file = gtk_file_dialog_open_finish(dialog, result, NULL); //Volta para a janela principal e pega o arquivo escolhido

    if (file != NULL) {
        char *input_path = g_file_get_path(file);
        const char *downloads = g_get_user_special_dir(G_USER_DIRECTORY_DOWNLOAD);
        
        char base[512];
        char ext_str[32] = "";
        char output_huf[512];
        char output_comprimida[512];  //Se o usuario escolher uma imagem, pega o caminho em forma de string

        getBaseName(input_path, base);
        char *ext = strrchr(input_path, '.');
        if (ext) strcpy(ext_str, ext);  //Chama a função que separa o nome da extensão

        snprintf(output_huf, sizeof(output_huf), "%s.huf", base);
        
        snprintf(output_comprimida, sizeof(output_comprimida),
         "%s/%s_comprimida%s",
         downloads,
         g_path_get_basename(base),
         ext_str);

        g_print("\n========================================\n");
        g_print("Iniciando processo para: %s\n", input_path);
        g_print("========================================\n");

        g_print("[1/2] Gerando arquivo compactado (.huf)...\n");  //prints do terminal
        
        Image img = loadImage(input_path);
        int freq[256] = {0};
        countRGBFrequency(&img, freq);
        
        Node* root = buildHuffmanTree(freq);
        HuffCode codes[256] = {0};
        buildCodes(root, 0, 0, codes);  //Conta a frequencia das cores e constroi a arvore de huff

        int format = 3; 
        if (ext) {
            if(strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) format = 1;
            else if(strcmp(ext, ".png") == 0) format = 2;
        } //escolhe qual formato será salvo no arquivo

        compressRGB(&img, output_huf, codes, freq, format);
        freeImage(&img); //Salva o arquivo .huf e limpa a RAM
        
        g_print("      -> Salvo como: %s\n", output_huf);

        g_print("[2/2] Lendo o .huf e recriando a imagem original...\n");
        
        decompressRGB(output_huf, output_comprimida);

        if (remove(output_huf) == 0) {
            g_print("Arquivo temporario %s removido.\n", output_huf);
        } else {
            g_print("Erro ao remover o arquivo temporario %s.\n", output_huf);
}
        
        g_print("      -> Imagem salva como: %s\n", output_comprimida);
        g_print("========================================\n");
        g_print("SUCESSO! Processo concluido.\n\n");  //Lê o arquivo .huf e salva a imagem comprimida

        GtkAlertDialog *alert = gtk_alert_dialog_new("Imagem comprimida!");
        gtk_alert_dialog_show(alert, parent_window);
        g_object_unref(alert); //Chama a mensagem de aviso

        g_free(input_path);
        g_object_unref(file);
    }
    g_object_unref(dialog); //Limpa a memória
}

void escolher_arquivo_comprimir(GtkWidget *botao, gpointer window) {
    (void)botao;
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Escolha a imagem para comprimir");

    gtk_file_dialog_open(dialog, GTK_WINDOW(window), NULL, (GAsyncReadyCallback) arquivo_selecionado, window);
} //Manda o usuario para os arquivos do computador e ao final chama a função arquivo_selecionado

static void activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    GtkWidget *window;
    GtkWidget *menu;
    GtkWidget *titulo;  //Cria as váriaveis para cada parte da interface

    window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300); 
    gtk_window_set_title(GTK_WINDOW(window), "Compressor");  //Cria a janela vinculada ao app, define o tamanho e o título

    menu = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_halign(menu, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(menu, GTK_ALIGN_CENTER); //Cria uma "caixa" onde ficarão os botões

    titulo = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(titulo), "<span size='30000'><b>COMPRESSOR</b></span>"); //Cria o label do título e define seu tamanho

    GtkWidget *btn1 = gtk_button_new_with_label("Comprimir imagem");
    GtkWidget *btn2 = gtk_button_new_with_label("Sair"); //Cria os botões e colaca o nome neles

    g_signal_connect(btn1, "clicked", G_CALLBACK(escolher_arquivo_comprimir), window);
    g_signal_connect(btn2, "clicked", G_CALLBACK(sair), app); //chama as funções relacionadas a cada botão

    gtk_box_append(GTK_BOX(menu), titulo);
    gtk_box_append(GTK_BOX(menu), btn1);
    gtk_box_append(GTK_BOX(menu), btn2);

    gtk_window_set_child(GTK_WINDOW(window), menu);
    gtk_window_present(GTK_WINDOW(window));  //Coloca o título e os botões na caixa(menu), coloca ela na janela principal e exibe a interface
}

int main(int argc, char **argv) {

    g_setenv("GSK_RENDERER", "cairo", TRUE);  //muda o renderizador automaticamente(para n ter q trocar quando for rodar o programa)

    GtkApplication *app = gtk_application_new("compressor.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;  //Cria a aplicação GTK, conecta a função activate, roda o programa em loop até clicar em "sair" ou no X e limpa a memória.
}