#include "PinDeff.h"
volatile uint32_t last_interrupt_time_button1 = 0;
volatile uint32_t last_interrupt_time_button2 = 0;
volatile bool b1Process = false;
volatile bool b2Process = false;

void gpio_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == b1 && (current_time - last_interrupt_time_button1 > DEBOUNCE_TIME_MS))
    {
        b1Process = true;
        last_interrupt_time_button1 = current_time;
    }
    else if (gpio == b2 && (current_time - last_interrupt_time_button2 > DEBOUNCE_TIME_MS))
    {
        b2Process = true;
        last_interrupt_time_button2 = current_time;
    }
}

void setup()
{
    stdio_init_all();
    watchdog_enable(20000, 1);
    mutex_init(&StopMem);

    // LED Pin Control Setup
    gpio_init(Chan0LED);
    gpio_set_dir(Chan0LED, GPIO_OUT);

    gpio_init(Chan1LED);
    gpio_set_dir(Chan1LED, GPIO_OUT);

    gpio_init(Chan2LED);
    gpio_set_dir(Chan2LED, GPIO_OUT);

    gpio_init(Chan3LED);
    gpio_set_dir(Chan3LED, GPIO_OUT);
    
    gpio_init(ioutControl);
    gpio_set_dir(ioutControl, GPIO_OUT);

    gpio_init(CurrentConA0);
    gpio_set_dir(CurrentConA0, GPIO_OUT);

    gpio_init(CurrentConA1);
    gpio_set_dir(CurrentConA1, GPIO_OUT);
    multicore_launch_core1(core1_ent);

    gpio_init(b1);
    gpio_set_dir(b1, GPIO_IN);

    gpio_init(b2);
    gpio_set_dir(b2, GPIO_IN);
    gpio_set_irq_enabled_with_callback(b1, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(b2, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

}