/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "stm32f4xx_hal.h"
//#include "stm32_hal_legacy.h"
#include "stm32f411e_discovery_accelerometer.h"
#include "string.h"
//#include "lsm303dlhc.h"
#include "stm32f411e_discovery_accelerometer.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define MAX_PERIOD 8

enum {
  M_UART_POLL = 0,
  M_UART_IT,
  M_UART_DMA,
  M_UART_MODES
};

typedef struct{
  uint32_t timestamp;
  int16_t val;
  uint8_t type;
  uint8_t subtype;
  float sensitivity;
}msg_t;

QueueHandle_t txQueue;
QueueHandle_t rxQueue;

SemaphoreHandle_t txSemaphoreDone;
SemaphoreHandle_t buttonSemaphore;
SemaphoreHandle_t i2cSemaphoreDone;
SemaphoreHandle_t i2cMutex;

volatile int uart_mode = 0;
volatile int acc_period = 0;
volatile int gyro_period = 0;
volatile int temp_period = 0;
volatile int button_period = 0;
int enviar = 0;

UART_HandleTypeDef UartHandle;
DMA_HandleTypeDef hdma_usart2_tx;

void uart_dma_init()
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  /* Peripheral DMA init*/
  hdma_usart2_tx.Instance = DMA1_Stream6;
  hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart2_tx.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart2_tx.Init.Mode = DMA_NORMAL;
  hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_usart2_tx);

  __HAL_LINKDMA(&UartHandle,hdmatx,hdma_usart2_tx);

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */
    HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE END USART2_MspInit 1 */
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

/****************************
 * 
 * UART TX task and auxiliary functions and callbacks
 * 
 ****************************/

void uart_config()
{
  //HAL_UART_MspInit(&UartHandle);
  //UART Configuration
  uart_dma_init();
  UartHandle.Instance          = USART2;

  UartHandle.Init.BaudRate     = 115200;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  //GPIO configured in MSP Callback


  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  { 
	    _Error_Handler(__FILE__, __LINE__);
  }

}

void uart_tx(void* param)
{
  //Ping pong buffer
  static char buf[2][256];
  static uint8_t buf_index = 0;
  msg_t msg;
  uint8_t len;

  uart_config();
  xSemaphoreGive(txSemaphoreDone);

  /*char hola[20]= "hola mundo";
  HAL_UART_Transmit(&UartHandle, (uint8_t *)hola, strlen(hola), 10);*/

  while(1) {
    // Get element from the queue and send it!
    xQueueReceive(txQueue, &msg, portMAX_DELAY);

    if (msg.sensitivity) {
      sprintf(buf[buf_index], "%c%c,%u,%d\r\n", msg.type, msg.subtype, msg.timestamp, msg.val);
    } else {
      sprintf(buf[buf_index], "%c%c,%u,%d\r\n", msg.type, msg.subtype, msg.timestamp, msg.val);
    }
    len = strlen(buf[buf_index]);

    switch(uart_mode) {
      case M_UART_POLL:

    	if(HAL_UART_Transmit(&UartHandle, buf[buf_index], len, 500)!= HAL_OK) {
        //if(HAL_UART_Transmit(&UartHandle, msg.ptr, msg.len, 500)!= HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
          break;
        }
        break;
      case M_UART_IT:

        if (xSemaphoreTake(txSemaphoreDone, portMAX_DELAY) != pdPASS) {
            _Error_Handler(__FILE__, __LINE__);
          break;
        }
        if(HAL_UART_Transmit_IT(&UartHandle, buf[buf_index], len)!= HAL_OK) {
        //if(HAL_UART_Transmit_IT(&UartHandle, msg.ptr, msg.len)!= HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
          break;
        }
        break;
      case M_UART_DMA:

        if (xSemaphoreTake(txSemaphoreDone, portMAX_DELAY) != pdPASS) {
            _Error_Handler(__FILE__, __LINE__);
          break;
        }
        if(HAL_UART_Transmit_DMA(&UartHandle, buf[buf_index], len)!= HAL_OK) {
        //if(HAL_UART_Transmit_DMA(&UartHandle, msg.ptr, msg.len)!= HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
          break;
        }
        break;
    }
    //buf_index = !buf_index;
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: transfer complete*/
	enviar = 1;
  //Generate Token
	xSemaphoreGiveFromISR(txSemaphoreDone, NULL);

}

void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/****************************
 * 
 * UART RX task and auxiliary functions and callbacks
 * 
 ****************************/


/****************************
 * 
 * BUTTON task and auxiliary functions and callbacks
 * 
 ****************************/
void configButton()
{
  GPIO_InitTypeDef gpio;
  __HAL_RCC_GPIOA_CLK_ENABLE();

  gpio.Mode = GPIO_MODE_IT_RISING;
  gpio.Pin = GPIO_PIN_0;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_MEDIUM;

  HAL_GPIO_Init(GPIOA, &gpio);
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  HAL_NVIC_SetPriority(EXTI0_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY, 0); // min priority
}

void button(void* param)
{
  //static int period[MAX_PERIOD] = {1000/portTICK_RATE_MS, 800/portTICK_RATE_MS, 700/portTICK_RATE_MS, 600/portTICK_RATE_MS, 500/portTICK_RATE_MS, 400/portTICK_RATE_MS, 300/portTICK_RATE_MS, 200/portTICK_RATE_MS};

  msg_t button_msg;
  configButton();
  button_msg.type = 'B';
  button_msg.subtype = '0';
  button_msg.val = 1;
  button_msg.sensitivity = 0;


  while (1) {
    if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) != pdPASS) {
    	_Error_Handler(__FILE__, __LINE__);
      break;
    }

    button_msg.timestamp = xTaskGetTickCount();

    //store the count of the push of the button

    if (button_msg.val > 5)
    {
      //reset of push
      button_msg.val = 1;

      //change the mode POL = 0, IT = 1 and DMA = 2
      uart_mode++;

      //return to uart_mode = POL
      if(uart_mode >= 3)  {uart_mode = 0;}

    }

    //Store the button in the queue
    xQueueSendToBack(txQueue, &button_msg, 0);

    button_msg.val++;

    vTaskDelay(200/portTICK_RATE_MS);
    HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_12);
    // Clear EXTI interrupt, just in case there was another pending interrupt.
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  // Disable interrupts to prevent button bounces from giving another semaphore
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);
  //Generate Token  
  xSemaphoreGiveFromISR(buttonSemaphore, NULL);

}

/****************************
 * 
 * ACCELEROMETER task and auxiliary functions and callbacks
 * 
 ****************************/
void accelero_config() {
  BSP_ACCELERO_Init();
}

void accelerometer(void* param) {
  msg_t msg;
  static int period[MAX_PERIOD] = {1000/portTICK_RATE_MS, 500/portTICK_RATE_MS, 250/portTICK_RATE_MS, 100/portTICK_RATE_MS, 40/portTICK_RATE_MS, 20/portTICK_RATE_MS, 10/portTICK_RATE_MS, 5/portTICK_RATE_MS};
  int16_t xyz[3];

  TickType_t prevWakeTime = xTaskGetTickCount();

  msg.type = 'A';
  msg.sensitivity = 1;
  accelero_config();
  while (1) {
    //xSemaphoreTake(i2cMutex, portMAX_DELAY);
    msg.timestamp = xTaskGetTickCount();
    BSP_ACCELERO_GetXYZ(xyz);
    //xSemaphoreGive(i2cMutex);
    msg.subtype = 'X';
    msg.val = xyz[0];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Y';
    msg.val = xyz[1];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Z';
    msg.val = xyz[2];
    xQueueSendToBack(txQueue, &msg, 0);

    vTaskDelayUntil(&prevWakeTime, period[acc_period]);
  }
}

/****************************
 * 
 * GYROSCOPE task and auxiliary functions and callbacks
 * 
 ****************************/
void gyroscope_config() {
  //BSP_ACCELERO_Init();
}

void gyroscope(void* param) {
  msg_t msg;
  static int period[MAX_PERIOD] = {1000/portTICK_RATE_MS, 500/portTICK_RATE_MS, 250/portTICK_RATE_MS, 100/portTICK_RATE_MS, 40/portTICK_RATE_MS, 20/portTICK_RATE_MS, 10/portTICK_RATE_MS, 5/portTICK_RATE_MS};
  int16_t xyz[3];

  TickType_t prevWakeTime = xTaskGetTickCount();

  msg.type = 'G';

  while (1) {
    //xSemaphoreTake(i2cMutex, portMAX_DELAY); SPI not I2C
    msg.timestamp = xTaskGetTickCount();
    //Read XYZ. Check l3gd20.c, which is the device used in stm32f411_discovery_giroscope.c
    //xSemaphoreGive(i2cMutex);
    msg.subtype = 'X';
    msg.val = xyz[0];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Y';
    msg.val = xyz[1];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Z';
    msg.val = xyz[2];
    xQueueSendToBack(txQueue, &msg, 0);

    vTaskDelayUntil(&prevWakeTime, period[gyro_period]);
  }
}

/****************************
 * 
 * COMPASS task and auxiliary functions and callbacks
 * 
 ****************************/
void compass_config() {
  BSP_ACCELERO_Init();
}

void compass(void* param) {
  msg_t msg;
  static int period[MAX_PERIOD] = {5000/portTICK_RATE_MS, 2500/portTICK_RATE_MS, 1000/portTICK_RATE_MS, 500/portTICK_RATE_MS, 200/portTICK_RATE_MS, 100/portTICK_RATE_MS, 50/portTICK_RATE_MS, 20/portTICK_RATE_MS};
  int16_t xyz[3];

  TickType_t prevWakeTime = xTaskGetTickCount();

  msg.type = 'C';

  while (1) {
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    msg.timestamp = xTaskGetTickCount();
    //BSP_ACCELERO_GetXYZ(xyz);
    xSemaphoreGive(i2cMutex);
    msg.subtype = 'X';
    msg.val = xyz[0];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Y';
    msg.val = xyz[1];
    xQueueSendToBack(txQueue, &msg, 0);
    msg.subtype = 'Z';
    msg.val = xyz[2];
    xQueueSendToBack(txQueue, &msg, 0);

    //vTaskDelayUntil(&prevWakeTime, period[compass_period]);
  }
}

/****************************
 * 
 * TEMPERATURE task and auxiliary functions and callbacks
 * 
 ****************************/
void temperature_config() {
  //BSP_ACCELERO_Init();
}

void temperature(void* param) {
  msg_t msg;
  static int period[MAX_PERIOD] = {600000/portTICK_RATE_MS, 300000/portTICK_RATE_MS, 120000/portTICK_RATE_MS, 60000/portTICK_RATE_MS, 30000/portTICK_RATE_MS, 10000/portTICK_RATE_MS, 5000/portTICK_RATE_MS, 1000/portTICK_RATE_MS};
  int16_t temp;

  TickType_t prevWakeTime = xTaskGetTickCount();

  msg.type = 'T';
  msg.subtype = '0';

  while (1) {
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    msg.timestamp = xTaskGetTickCount();
    //BSP_ACCELERO_GetXYZ(xyz);
    xSemaphoreGive(i2cMutex);
    msg.val = temp;
    xQueueSendToBack(txQueue, &msg, 0);

    vTaskDelayUntil(&prevWakeTime, period[temp_period]);
  }
}



void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK |
                   RCC_CLOCKTYPE_PCLK1 |
                   RCC_CLOCKTYPE_PCLK2 |
                   RCC_CLOCKTYPE_SYSCLK);
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

void config_leds() {
  __HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitTypeDef gpio;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_LOW;

  HAL_GPIO_Init(GPIOD, &gpio);
}

int main(void)
{

  //GPIO_InitStruct.GPIO_Mode = GPIO_MODE_OUT;
    SystemClock_Config();
    HAL_Init();

    config_leds();
    buttonSemaphore = xSemaphoreCreateBinary();
    txSemaphoreDone = xSemaphoreCreateBinary();
    rxQueue = xQueueCreate( 10, sizeof( msg_t ) );
    txQueue = xQueueCreate( 10, sizeof( msg_t ) );


    //i2c_mutex = 

    if (xTaskCreate(button, "button", 1024, NULL, 2, NULL) != pdPASS) {
      _Error_Handler(__FILE__, __LINE__);
    }

    if (xTaskCreate(uart_tx, "uart", 1024, NULL, 2, NULL) != pdPASS) {
          _Error_Handler(__FILE__, __LINE__);
        }

    /*if (xTaskCreate(accelerometer, "accelerometer", 1024, NULL, 5, NULL) != pdPASS) {
      _Error_Handler(__FILE__, __LINE__);
    }*/


  vTaskStartScheduler();

  return 0;
}

/*void HAL_GPIO_EXTI_Callback(uint16_t pin) {
  // Disable interrupts to prevent button bounces from giving another semaphore
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);
  //Generate Token
}*/
