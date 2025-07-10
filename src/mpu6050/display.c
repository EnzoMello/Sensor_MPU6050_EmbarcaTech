
#include "display.h"

///< Definição da instância global da estrutura do display, usada pelas funções deste módulo.
ssd1306_t display;

/**
 * @brief Inicializa o barramento I2C1 e o controlador do display SSD1306.
 * @return 0 em caso de sucesso, 1 em caso de falha.
 */
int initializeDisplay() {

    // Inicializa a instância i2c1 e seus pinos.
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Tenta inicializar o controlador SSD1306 no barramento i2c1.
    if (!ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_ADDRESS, i2c1)) { 
        printf("Falha ao inicializar o display SSD1306\n"); 
        return 1;
    }

    return 0;
}

/**
 * @brief Exibe um texto na tela em uma posição e escala específicas.
 * @param texto A string de texto a ser exibida.
 * @param x A coordenada X (horizontal) inicial do texto.
 * @param y A coordenada Y (vertical) inicial do texto.
 * @param scale O fator de escala da fonte (1 = normal).
 */
void showText(const char *texto, uint32_t x, uint32_t y, uint32_t scale){
    ssd1306_draw_string(&display, x, y, scale, texto); 
    ssd1306_show(&display); 
}

/**
 * @brief Limpa completamente a tela do display.
 */
void clearScreen(){
    ssd1306_clear(&display); // Limpa o buffer da memória.
    ssd1306_show(&display); // Envia a tela limpa para exibição.
}

/**
 * @brief Atualiza uma linha de texto, limpando a área antes de desenhar.
 * @param text A nova string de texto a ser exibida.
 * @param x A coordenada X (horizontal) inicial do texto.
 * @param y A coordenada Y (vertical) inicial do texto.
 * @param scale O fator de escala da fonte.
 * @param clear_width A largura da área (em pixels) a ser limpa antes de desenhar o novo texto.
 */
void updateTextLine(const char* text, uint32_t x, uint32_t y, uint32_t scale, uint32_t clear_width) {
    // A altura da fonte padrão na biblioteca é 8 pixels.
    uint32_t char_height = 8;

    // Limpa uma área retangular onde o novo texto será escrito para evitar sobreposição.
    ssd1306_clear_square(&display, x, y, clear_width, char_height * scale);

    // Desenha a nova string no buffer da memória.
    ssd1306_draw_string(&display, x, y, scale, text);
}