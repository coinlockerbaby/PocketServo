/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"

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
/* USER CODE BEGIN Variables */
static TaskHandle_t xHandleTaskLCD_ECAT = NULL;
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskUartShell = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

SemaphoreHandle_t xTxSemaphore;  // 用于发送完成的信号量
SemaphoreHandle_t xUartMutex;    // 用于保护串口资源的互斥锁
SemaphoreHandle_t xFocMutex;    // 用于保护FOC参数资源的互斥锁

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void AppTaskLCD_ECAT(void *argument);
static void AppTaskUserIF(void *argument);
static void AppTaskUartShell(void *argument);
static void AppTaskStart(void *argument);
static void AppTaskCreate(void);

extern void ecat_Init(void);
extern void ecat_main(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
	xUartMutex = xSemaphoreCreateMutex();
	xFocMutex = xSemaphoreCreateMutex();
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	xTxSemaphore = xSemaphoreCreateBinary();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	xTaskCreate(  AppTaskStart,         /* 任务函数  */
								"AppTaskStart",       /* 任务名    */
								512,                 	/* 任务栈大小，单位word，也就是4字节 */
								NULL,                 /* 任务参数  */
								osPriorityNormal6,    /* 任务优先级*/
								&xHandleTaskStart );  /* 任务句柄  */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_Device */
  MX_USB_Device_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/*
*********************************************************************************************************
*	函 数 名: AppTaskLCD_ECAT
*	功能说明: SPI3任务(LCD屏幕或ECAT)
*	形    参: 无
*	返 回 值: 无
* 优 先 级: osPriorityNormal3
*********************************************************************************************************
*/
static void AppTaskLCD_ECAT(void *argument)
{
	int32_t	spi_ctrl_last = SPI3_Ctrl; /* 设置最新状态为CONTROL_LCD */
	/* 初始化任务 */
	
	while(1)
	{
		/* 需要周期性处理的程序 */
		/* 如果状态发生变换 */
		if (SPI3_Ctrl != spi_ctrl_last)
		{
			spi_ctrl_last = SPI3_Ctrl;
			switch (spi_ctrl_last)
			{
			  case CONTROL_LCD:
					/* LCD初始化 */
					bsp_spi3_Lcd_Init();
					LCD_Init();
					break;
				case CONTROL_ECAT:
					/* ECAT初始化 */
					LCD_Fill(0, 0, 240, 320, BLACK);
					LCD_Disable(); /* LCD显示[EtherCAT] */
					bsp_spi3_ECAT_Init();
					ecat_Init();
					break;
				default:
					break;
			}
		}

		/* 根据SPI的控制外设执行对应功能 */
		switch (spi_ctrl_last)
		{
			case CONTROL_LCD:
				LCD_Demo();
				break;
			case CONTROL_ECAT:
				ecat_main();
				break;
			default:
				break;
		}
		vTaskDelay(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUserIF
*	功能说明: 按键消息处理
*	形    参: 无
*	返 回 值: 无
* 优 先 级: osPriorityNormal4
*********************************************************************************************************
*/
static void AppTaskUserIF(void *argument)
{
	uint8_t ucKeyCode;
	/* 初始化任务 */
	
	while(1)
	{
		ucKeyCode = bsp_GetKey();
		switch (ucKeyCode)
		{
			case KEY_DOWN_K1:		/* K1键按下 */
				shellPrint(&shell, "KEY_DOWN_K1\r\n");
				// 获取互斥锁，保护 FOC参数 资源
				xSemaphoreTake(xFocMutex, portMAX_DELAY);
				switch (UsrConfig.control_mode)
				{
					case CONTROL_MODE_TORQUE_RAMP:
						Foc.target_i_q -= 0.1f;
						break;
					case CONTROL_MODE_VELOCITY_RAMP:
						Foc.target_velocity -= 5.0f;
						break;
					case CONTROL_MODE_POSITION_RAMP:
						Foc.target_position -= 5.0f;
						break;
					case CONTROL_MODE_POSITION_PROFILE:
						Foc.target_position -= 5.0f;
						break;
					default:
						break;
				}
				// 释放互斥锁
				xSemaphoreGive(xFocMutex);
				break;
				
			case KEY_DOWN_K2:		/* K2键按下 */
				shellPrint(&shell, "KEY_DOWN_K3\r\n");
				// 获取互斥锁，保护 FOC参数 资源
				xSemaphoreTake(xFocMutex, portMAX_DELAY);
				switch (UsrConfig.control_mode)
				{
					case CONTROL_MODE_TORQUE_RAMP:
						Foc.target_i_q += 0.1f;
						break;
					case CONTROL_MODE_VELOCITY_RAMP:
						Foc.target_velocity += 5.0f;
						break;
					case CONTROL_MODE_POSITION_RAMP:
						Foc.target_position += 5.0f;
						break;
					case CONTROL_MODE_POSITION_PROFILE:
						Foc.target_position += 5.0f;
						break;
					default:
						break;
				}
				// 释放互斥锁
				xSemaphoreGive(xFocMutex);
				break;

			case KEY_DOWN_K3:		/* K3键按下 */
				/* 急停 */
				shellPrint(&shell, "KEY_DOWN_K3\r\n");
				MCT_set_state(M_ERR); /* 设置为错误状态 */
				break;
				
			case KEY_DOWN_K4: 	/* K4键按下 */
				shellPrint(&shell, "KEY_DOWN_K4\r\n");
				// 获取互斥锁，保护 FOC参数 资源
				xSemaphoreTake(xFocMutex, portMAX_DELAY);
				if (UsrConfig.control_mode < 4)
				{
					UsrConfig.control_mode++;
				}
				else
				{
					UsrConfig.control_mode = 1;
				}
				// 释放互斥锁
				xSemaphoreGive(xFocMutex);
				break;
				
			case SYS_DOWN_K1K2:
				shellPrint(&shell, "SYS_DOWN_K1K2\r\n");
				SPI3_Ctrl = !SPI3_Ctrl; /* 切换SPI3控制对象 */
				break;
			default:
				break;
		}
		/* 需要周期性处理的程序 */
		vTaskDelay(20);
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskUartShell
*	功能说明: 串口shell任务，用于接收串口数据，并执行相关命令。
*	形    参: 无
*	返 回 值: 无
* 优 先 级: osPriorityNormal5
*********************************************************************************************************
*/
static void AppTaskUartShell(void *argument)
{
	/* 初始化任务 */
	
	while(1)
	{
		/* 需要周期性处理的程序 */
		shellTask(&shell);
		vTaskDelay(1);
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务，这里用作BSP驱动包处理。
*	形    参: 无
*	返 回 值: 无
* 优 先 级: osPriorityNormal6  
*********************************************************************************************************
*/
static void AppTaskStart(void *argument)
{
	uint8_t  ucCount = 0;
	/* 初始化外设 */
	
	/* 初始化组件 */
	bsp_InitKey();		 				/* 按键初始化 */
	bsp_spi3_manager_init();  			/* SPI3控制管理初始化 */
	bsp_spi3_Lcd_Init();
	LCD_Init();							/* LCD屏幕初始化 */
	User_Shell_Init(); 					/* letter-shell初始化 */
	MyFoc_init();						/* FOC初始化 */
	
	
	/* 创建任务 */
	AppTaskCreate();
	
	while(1)
	{
		/* 需要周期性处理的程序 */
		
		
		/* 1ms 处理 */
		bsp_RunPer1ms();
		/* 10ms 处理 */
		if(++ucCount == 10)
		{
			ucCount = 0;
			bsp_RunPer10ms();
		}
		vTaskDelay(1);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
	BaseType_t status;

	status = xTaskCreate(  AppTaskLCD_ECAT,  			 		 /* 任务函数  */
												"AppTaskLCD_ECAT", 			 		 /* 任务名    */
												512,                 		 		 /* 任务栈大小，单位word，也就是4字节 */
												NULL,                 	 		 /* 任务参数  */
												osPriorityNormal3,    	 		 /* 任务优先级*/
												&xHandleTaskLCD_ECAT );   	 /* 任务句柄  */
	if ( pdPASS == status)
		shellPrint(&shell, "[AppTaskCreate]:AppTaskLCD_ECAT task create success\r\n");
	else
		shellPrint(&shell, "[AppTaskCreate]:AppTaskLCD_ECAT task create failed\r\n");

	status = xTaskCreate(  AppTaskUserIF,  			 	 /* 任务函数  */
												"AppTaskUserIF", 			 	 /* 任务名    */
												256,                 		 /* 任务栈大小，单位word，也就是4字节 */
												NULL,                 	 /* 任务参数  */
												osPriorityNormal4,    	 /* 任务优先级*/
												&xHandleTaskUserIF );    /* 任务句柄  */
	if ( pdPASS == status)
		shellPrint(&shell, "[AppTaskCreate]:AppTaskUserIF task create success\r\n");
	else
		shellPrint(&shell, "[AppTaskCreate]:AppTaskUserIF task create failed\r\n");
	
	status = xTaskCreate(  AppTaskUartShell,  			 /* 任务函数  */
												"AppTaskUartShell", 			 /* 任务名    */
												512,                 			 /* 任务栈大小，单位word，也就是4字节 */
												NULL,                 		 /* 任务参数  */
												osPriorityNormal5,    		 /* 任务优先级*/
												&xHandleTaskUartShell );   /* 任务句柄  */
	if ( pdPASS == status)
		shellPrint(&shell, "[AppTaskCreate]:AppTaskUartShell task create success\r\n");
	else
		shellPrint(&shell, "[AppTaskCreate]:AppTaskUartShell task create failed\r\n");
	
	shellPrint(&shell, "*************************************************************\r\n");
}

/*
*********************************************************************************************************
*	函 数 名: HAL_ADCEx_InjectedConvCpltCallback
*	功能说明: ADC注入转换完成回调函数
*	形    参：hadc：ADC句柄
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Calc ADC offset */
	static int adc_sum_a = 0;
	static int adc_sum_b = 0;
	static int adc_sum_c = 0;
	static uint8_t isCalcAdcOffsetOvered = FALSE;
	const int measCnt = 64;
	static int measCntCopy = measCnt;
	if (hadc->Instance == ADC1 && !isCalcAdcOffsetOvered)
	{
		adc_sum_a += hadc1.Instance->JDR1;
		adc_sum_b += hadc1.Instance->JDR2;
		adc_sum_c += hadc1.Instance->JDR3;

		if (--measCntCopy <= 0)
		{
			phase_a_adc_offset = adc_sum_a / measCnt;
			phase_b_adc_offset = adc_sum_b / measCnt;
			phase_c_adc_offset = adc_sum_c / measCnt;

			isCalcAdcOffsetOvered = TRUE;
		}
	}
	if (hadc->Instance == ADC1 && isCalcAdcOffsetOvered)
	{
		
    MCT_high_frequency_task();
		
		vofaFrame.fdata[0] = Foc.i_a;
		vofaFrame.fdata[1] = Foc.i_b;
		vofaFrame.fdata[2] = Foc.i_c;
		vofaFrame.fdata[3] = Foc.i_d_filt;
		vofaFrame.fdata[4] = Foc.i_q_filt;
		vofaFrame.fdata[5] = Foc.target_position;
		vofaFrame.fdata[6] = Foc.position;
		vofaFrame.fdata[7] = Foc.target_velocity;
		vofaFrame.fdata[8] = Foc.velocity_filt;
		vofa_print();
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PrintfLogo(void)
{
	
	/* 检测CPU ID */
	uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;
	
	CPU_Sn0 = HAL_GetUIDw0();
	CPU_Sn1 = HAL_GetUIDw1();
	CPU_Sn2 = HAL_GetUIDw2();
	
	shellPrint(&shell, "*************************************************************\r\n");
	shellPrint(&shell, "    __________  ______   ______________ \r\n");
	shellPrint(&shell, "   / ____/ __ \\/ ____/  / ____/_  __/ / \r\n");
	shellPrint(&shell, "  / /_  / / / / /      / /     / / / /  \r\n");
	shellPrint(&shell, " / __/ / /_/ / /___   / /___  / / / /___\r\n");
	shellPrint(&shell, "/_/    \\____/\\____/   \\____/ /_/ /_____/\r\n");
	shellPrint(&shell, "                                        \r\n");
	shellPrint(&shell, "Build:       "__DATE__" "__TIME__"\r\n");
	shellPrint(&shell, "Version:     "SOFTWARE_VERSION"\r\n");
	shellPrint(&shell, "Copyright:   (C) 2025 astronG\r\n");
	shellPrint(&shell, "-------------------------------------------------------------\r\n");
	shellPrint(&shell, "CPU: STM32G474RET6, SYSCLK: %dMHz, RAM: 128KB, ROM: 512KB\r\n", SystemCoreClock / 1000000);
	shellPrint(&shell, "UID: %08X %08X %08X\r\n", CPU_Sn2, CPU_Sn1, CPU_Sn0);
	shellPrint(&shell, "STM32G4xx_HAL_Driver:STM32Cube_FW_G4 V1.6.1\r\n");
	shellPrint(&shell, "Current version of FOC Algorithm: %s\r\n", MYFOC_SOFTWARE_VERSION);
  shellPrint(&shell, "*************************************************************\r\n");
}

/* USER CODE END Application */

