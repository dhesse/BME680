#include "bme680.h"

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
void user_delay_ms(uint32_t period);

class BME680 {
private:
    struct bme680_dev gas_sensor;
    uint16_t meas_period;
    struct bme680_field_data data;
public:
    BME680();
    const struct bme680_field_data& get_data();
};