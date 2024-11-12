// Define base addresses and offsets
#define GPIOC_BASE 0x40011000
#define RCC_BASE 0x40021000
#define TIM2_BASE 0x40000000

#define GPIOC_CRH (*(volatile unsigned long *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR (*(volatile unsigned long *)(GPIOC_BASE + 0x0C))

#define RCC_APB1ENR (*(volatile unsigned long *)(RCC_BASE + 0x1C))
#define RCC_APB2ENR (*(volatile unsigned long *)(RCC_BASE + 0x18))

#define TIM2_CR1 (*(volatile unsigned long *)(TIM2_BASE + 0x00))
#define TIM2_SR (*(volatile unsigned long *)(TIM2_BASE + 0x10))
#define TIM2_PSC (*(volatile unsigned long *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile unsigned long *)(TIM2_BASE + 0x2C))

int main() {
    RCC_APB2ENR |= (1 << 4);
    RCC_APB1ENR |= (1 << 0);

    GPIOC_CRH &= ~(0xF<<20);
    GPIOC_CRH |= (1<<20);

    TIM2_PSC = 799;
    TIM2_ARR = 1000-1;

    // TIM2_CR1 &= (0x01);
    TIM2_CR1 |= (1<<0);

    while(1) {
        if(TIM2_SR & (1 << 0))
        {
            TIM2_SR &= ~(1 << 0);
            GPIOC_ODR ^=(1<<13);
        }
    }
    return 0;
}

int main()
{
    RCC_APB2ENR |= (1 << 4); // Enable GPIOC clock
    RCC_APB1ENR |= (1 << 0); // Enable TIM2 clock

    // configuration of GPIOC for pc13 pin.
    GPIOC_CRH &= ~(0xF << 20); // GPIOC_CRH register 21th & 20th are MODE13 bits for PC13.
    GPIOC_CRH |= (1 << 20);    // Set MODE to 01 for PC13 pin that causes LED to be blinked.
    TIM2_PSC = 7999;      // Prescaler value (0 - 7999 = 8000). Divided HSI(8MHz) by 8000 to get TIM2 clock 1000 Hz.
    TIM2_ARR = 499;       // (0-999) Timer counter count from 0 to 1000 in 1000Hz TIM2 clock. Then count again 0 -999.
                          // So for every 1 second Timer clock counter to 0.
    TIM2_CR1 |= (1 << 0); // Enable the timer counter by setting TIM2_CR1 register's 0th bit to 1.

    while (1)
    {
        if (TIM2_SR & 1)
        { // TIM2_SR 0th bit(called update interrupt flag bit) is automatically set by 1 when timer counter reach TIM2_ARR value
          // i.e 999.
            TIM2_SR &= ~(1 << 0); // Reset TIM2_SR 0th by 0.
            // Toggle the LED
            GPIOC_ODR ^= (1 << 13); // when GPIOC_ODR register's 13th bit 1 then LED turns on by the use of pc13 pin.
        }
    }
    return 0;
}
