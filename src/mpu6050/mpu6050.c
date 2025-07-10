
#include "hardware/i2c.h"
#include <math.h>
#include "pico/stdlib.h"
#include "mpu6050.h"

#define MPU6050_ADDR 0x68 
#define SDA_PIN 0         // Pino GPIO configurado para a linha de dados (SDA).
#define SCL_PIN 1         // Pino GPIO configurado para a linha de clock (SCL).

/**
 * @brief Inicializa o barramento I2C e o sensor MPU6050.
 */
void mpu6050_init(void) {
    // Inicializa o barramento i2c0 com velocidade de 400kHz.
    i2c_init(i2c0, 400 * 1000);

    // Define os pinos GPIO para as funções I2C.
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Ativa os resistores de pull-up internos.
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Acorda o MPU6050, pois ele inicia em modo de baixo consumo.
    // Escreve 0x00 no registrador de gerenciamento de energia (0x6B).
    uint8_t reset[2] = {0x6B, 0x00};
    i2c_write_blocking(i2c0, MPU6050_ADDR, reset, 2, false);
}

/**
 * @brief Lê os valores brutos de aceleração dos eixos X, Y e Z.
 * @param[out] ax Ponteiro para armazenar o valor bruto do eixo X.
 * @param[out] ay Ponteiro para armazenar o valor bruto do eixo Y.
 * @param[out] az Ponteiro para armazenar o valor bruto do eixo Z.
 */
void mpu6050_read_raw(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buffer[6];

    // Define o registrador inicial a partir do qual a leitura será feita (0x3B = ACCEL_XOUT_H).
    i2c_write_blocking(i2c0, MPU6050_ADDR, (uint8_t[]){0x3B}, 1, true); // true para manter o barramento ativo

    // Lê 6 bytes de dados sequencialmente (2 para cada eixo: X, Y, Z).
    i2c_read_blocking(i2c0, MPU6050_ADDR, buffer, 6, false);

}