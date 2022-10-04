/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void SYSTEM_CLOCKCONFIG(void);
/* USER CODE BEGIN PFP */
#define SYSTICK_BASEADDR   0xE000E010


typedef struct
{
	volatile uint32_t CSR;  //SysTick Control and Status Register
	volatile uint32_t RVR;  //SysTick Reload Value Register
	volatile uint32_t CVR; //SysTick Current Value Register
	volatile uint32_t CALIB;    //SysTick Calibration Value Register

}SysTick_RegDef_t;


SysTick_RegDef_t *pSysTick= ((SysTick_RegDef_t*)SYSTICK_BASEADDR);

#define SYSTICK ((SysTick_RegDef_t*)SYSTICK_BASEADDR)

void sysTickDelay_US(uint32_t delay)
{
	//Sets the reload value
	pSysTick->RVR = 16000000/(1000000);

	//Clears the SysTick current value register
	pSysTick->CVR= 0;

	//Enable SysTick in the control register and select internal system clock source
	pSysTick->CSR= (1 << 0) | (1 << 2);

	for(uint32_t i=delay; i > 0; i--)
	{
		while(!(pSysTick->CSR & (1 << 16)));
	}

	//Disable SysTick counter in the Control Register
	pSysTick->CSR=0;
}


void sysTickDelay_MS(uint32_t ms)
{
    // Wait until ms reach zero
    while (ms--)
    {
        // Delay 1ms
        sysTickDelay_US(1000);
    }
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SYSTEM_CLOCKCONFIG();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* SER CODE END WHILE */
RCC->AHB1ENR |= (1<<0);

/*---------------------------------*PA5*------------------------------------------------------*/
GPIOA->MODER |= (1<<10);
GPIOA->OTYPER &= ~(1<<5);  // bit 5=0 --> Output push pull
GPIOA->OSPEEDR |= (1<<11);  // Pin PA5 (bits 11:10) as Fast Speed (1:0)
GPIOA->PUPDR &= ~((1<<10) | (1<<11));  // Pin PA5 (bits 11:10) are 0:0 --> no pull up or pulldown

/*---------------------------------*PC13*-----------------------------------------------------*/
RCC->AHB1ENR |= (1<<2);
GPIOC->MODER &= ~(3<<26);
GPIOC->OTYPER &= ~(1<<13);  // bit 5=0 --> Output push pull
GPIOC->OSPEEDR |= (1<<27);  // Pin PA5 (bits 11:10) as Fast Speed (1:0)
GPIOC->PUPDR |= (1<<27);  // Pin PA5 (bits 11:10) are 0:0 --> no pull up or pulldown


if((GPIOC->IDR & (1<<13)) != 1)
{
GPIOA->ODR |= 1<<5; // Set the Pin PA5
sysTickDelay_MS(1000);
GPIOA->ODR &= ~(1<<5); // Reset the Pin PA5
sysTickDelay_MS(1000);

}

    /* USER CODE BEGIN 3 */
  
  
  /* USER CODE END 3 */
}
}

static void SYSTEM_CLOCKCONFIG(void)
{
#ifndef HSE_VALUE 
#define HSE_VALUE (8000000)U
#endif

RCC->APB1ENR |= (1 << 28);
RCC->CR |= 1<<16;  

while (!(RCC->CR & (1<<17)));


RCC->CFGR &= ~(1<<4);
	

RCC->CFGR |= (4<<10);
	

RCC->CFGR &= ~(1<<13);
#define PLL_M 	4
#define PLL_N 	100
#define PLL_P 	2  

RCC->PLLCFGR = (PLL_M <<0) | (PLL_N << 6) | (PLL_P <<16) | (1<<22);
RCC->CR |= (1<<24);
while (!(RCC->CR & (1<<25)));
RCC->CFGR |= (2<<0);
while (!(RCC->CFGR & (2<<2)));
RCC->APB2ENR |= (1<<14);
}


/**************************************************************************************
 *                     INTERRUPT STM32                                                * 
 *                                                                                    *
 *                                                                                    *
 * ***********************************************************************************/
void INIT_INTERRUPT(void)
{

RCC->APB2ENR |= (1<<14);  // Enable SYSCNFG
RCC->APB2ENR |= (1<<0);  // Enable AFIO CLOCK


};
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
