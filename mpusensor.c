
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"

// Drivers dos periféricos
#include "mpu6050.h"
#include "servo.h"
#include "display.h"

/// Limite em graus para acionar o alerta visual no display.
#define INCLINATION_ALERT_THRESHOLD 65.0f

// Variáveis globais para os dados do acelerômetro.
int16_t ax, ay, az;

/**
 * @brief Mapeia um valor de uma faixa de entrada para uma faixa de saída.
 * @param value O valor a ser mapeado.
 * @param fromLow Limite inferior da faixa de entrada.
 * @param fromHigh Limite superior da faixa de entrada.
 * @param toLow Limite inferior da faixa de saída.
 * @param toHigh Limite superior da faixa de saída.
 * @return O valor mapeado como um inteiro longo.
 */
long map_value(float value, float fromLow, float fromHigh, long toLow, long toHigh) {
    if (value < fromLow) value = fromLow;
    if (value > fromHigh) value = fromHigh;
    return (long)((value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow);
}

/**
 * @brief Centraliza todas as rotinas de inicialização do hardware.
 * @note A inicialização de cada periférico (MPU, Servo, Display) é chamada aqui.
 
 */
void setup() {
    stdio_init_all();
    sleep_ms(2000); // Delay para permitir a conexão do monitor serial.
    printf(" Iniciando sistema...\n");

    // Inicializa periféricos usando seus drivers.
    mpu6050_init();
    printf(" MPU6050 em i2c0 (pinos 4,5) inicializado.\n");

    servo_init();
    printf(" Servo Motor (pino 28) inicializado.\n");

    if (initializeDisplay() == 0) {
        printf(" Display SSD1306 em i2c1 (pinos 14,15) inicializado.\n");
    } else {
        printf(" Falha ao iniciar o display.\n");
        while(1); // Trava em caso de falha crítica.
    }

    clearScreen();
    servo_set_angle(90); // Centraliza o servo na posição inicial.
    sleep_ms(500);
    printf("Sistema pronto!\n\n");
}

/**
 * @brief Função principal do programa. Contém o loop infinito de operação.
 * @return int Código de saída (nunca deve ser atingido).
 */
int main() {
    setup();
    char buffer[30];

    while (true) {
        // LEITURA DO SENSOR 
        mpu6050_read_raw(&ax, &ay, &az);

        // PROCESSAMENTO DOS DADOS 
        float pitch_angle = mpu6050_get_inclination(ax, ay, az);
        uint servo_angle = map_value(pitch_angle, -90.0f, 90.0f, 0, 180);

        // ATUAÇÃO E FEEDBACK 
        servo_set_angle(servo_angle);
        printf("Angulo: %.2f -> Servo: %u\n", pitch_angle, servo_angle);

        // FEEDBACK VISUAL no display 
        sprintf(buffer, "Angulo: %.1f", pitch_angle);
        updateTextLine(buffer, 5, 10, 1, 128);

        // Lógica do desafio: exibe alerta se a inclinação for excessiva.
        if (fabs(pitch_angle) > INCLINATION_ALERT_THRESHOLD) {
            updateTextLine("ALERTA!\n", 25, 45, 1, 128);
        } else {
            updateTextLine("Status: OK\n", 10, 35, 1, 128);
        }

        // Atualiza a tela com as novas informações desenhadas.
        ssd1306_show(&display);

        // @note Um delay de 1 segundo pode fazer o sistema parecer pouco responsivo.
        // Um valor menor (ex: 100-200ms) pode ser mais adequado.
        sleep_ms(1000);
    }
    return 0;
}