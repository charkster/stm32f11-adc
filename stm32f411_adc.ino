// Using PA1 as the ADC input

#define __MAX_DAC_12bit 4096
#define __VREF          3.3
#define SerialUSB Serial // this is an alias for the usb serial port

volatile char serial_command;

void setup() 
{
  SerialUSB.begin(115200);
  while(!SerialUSB);
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;                      // enable ADC1 clock
  GPIOA->MODER |= GPIO_MODER_MODE1_1 | GPIO_MODER_MODE1_0; // analog mode PA1
  ADC1->SQR3   |= ADC_SQR3_SQ1_0;                          // channel 1, PA1
  ADC1->CR2    |= ADC_CR2_ADON;                            // enable ADC
}
 
void loop() 
{
  if (SerialUSB.available())
  {
    serial_command = SerialUSB.read();
    if (serial_command == 'c')
    {
       ADC1->CR2 |= ADC_CR2_SWSTART;                        // adc software trigger
       while (!(ADC1->SR & ADC_SR_EOC));                    // wait for end-of-conversion status bit
       float voltage = ADC1->DR * __VREF / __MAX_DAC_12bit; // read and scale result
       SerialUSB.print("Voltage is:");
       SerialUSB.println(voltage);                          // Print the voltage.
    }
    else
    {
      SerialUSB.print("--> input c to request ADC conversion <--\n");
    }
    delay(1); // Delay 1ms
  }
}
