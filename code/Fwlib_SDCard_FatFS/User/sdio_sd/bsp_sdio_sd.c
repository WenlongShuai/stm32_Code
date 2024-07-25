/**
  ******************************************************************************
  * @file    stm32_eval_sdio_sd.c
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file provides a set of functions needed to manage the SDIO SD 
  *          Card memory mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this memory). 
  *          
  *            
  *  @verbatim
  *
  *          ===================================================================
  *                                   How to use this driver
  *          ===================================================================
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (SDIO and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in SD_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          SD_LowLevel_Init() function.
  *            
  *          A - SD Card Initialization and configuration
  *          ============================================    
  *            - To initialize the SD Card, use the SD_Init() function.  It 
  *              Initializes the SD Card and put it into StandBy State (Ready 
  *              for data transfer). This function provide the following operations:
  *           
  *              1 - Apply the SD Card initialization process at 400KHz and check
  *                  the SD Card type (Standard Capacity or High Capacity). You 
  *                  can change or adapt this frequency by adjusting the 
  *                  "SDIO_INIT_CLK_DIV" define inside the stm32xx_eval.h file. 
  *                  The SD Card frequency (SDIO_CK) is computed as follows:
  *                    
  *                     +---------------------------------------------+    
  *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
  *                     +---------------------------------------------+  
  *                          
  *                  In initialization mode and according to the SD Card standard, 
  *                  make sure that the SDIO_CK frequency don't exceed 400KHz.        
  *         
  *              2 - Get the SD CID and CSD data. All these information are
  *                  managed by the SDCardInfo structure. This structure provide
  *                  also ready computed SD Card capacity and Block size.   
  *
  *              3 - Configure the SD Card Data transfer frequency. By Default,
  *                  the card transfer frequency is set to 24MHz. You can change
  *                  or adapt this frequency by adjusting the "SDIO_TRANSFER_CLK_DIV" 
  *                  define inside the stm32xx_eval.h file.
  *                  The SD Card frequency (SDIO_CK) is computed as follows:
  *                    
  *                     +---------------------------------------------+    
  *                     | SDIO_CK = SDIOCLK / (SDIO_INIT_CLK_DIV + 2) |
  *                     +---------------------------------------------+     
  *                                    
  *                  In transfer mode and according to the SD Card standard, 
  *                  make sure that the SDIO_CK frequency don't exceed 25MHz
  *                  and 50MHz in High-speed mode switch.
  *                  To be able to use a frequency higher than 24MHz, you should
  *                  use the SDIO peripheral in bypass mode. Refer to the 
  *                  corresponding reference manual for more details.
  *                    
  *              4 -  Select the corresponding SD Card according to the address
  *                   read with the step 2.
  *                                   
  *              5 -  Configure the SD Card in wide bus mode: 4-bits data.                
  *
  *          B - SD Card Read operation
  *          ========================== 
  *           - You can read SD card by using two function: SD_ReadBlock() and
  *             SD_ReadMultiBlocks() functions. These functions support only
  *             512-byte block length.
  *           - The SD_ReadBlock() function read only one block (512-byte). This
  *             function can transfer the data using DMA controller or using 
  *             polling mode. To select between DMA or polling mode refer to 
  *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm32_eval_sdio_sd.h
  *             file and uncomment the corresponding line. By default the SD DMA
  *             mode is selected          
  *           - The SD_ReadMultiBlocks() function read only mutli blocks (multiple 
  *             of 512-byte). 
  *           - Any read operation should be followed by two functions to check
  *             if the DMA Controller and SD Card status.
  *              - SD_ReadWaitOperation(): this function insure that the DMA
  *                controller has finished all data transfer.
  *              - SD_GetStatus(): to check that the SD Card has finished the 
  *                data transfer and it is ready for data.
  *                  
  *           - The DMA transfer is finished by the SDIO Data End interrupt. User
  *             has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler().
  *             Don't forget to enable the SDIO_IRQn interrupt using the NVIC controller.      
  *                
  *          C - SD Card Write operation
  *          =========================== 
  *           - You can write SD card by using two function: SD_WriteBlock() and
  *             SD_WriteMultiBlocks() functions. These functions support only
  *             512-byte block length.   
  *           - The SD_WriteBlock() function write only one block (512-byte). This
  *             function can transfer the data using DMA controller or using 
  *             polling mode. To select between DMA or polling mode refer to 
  *             "SD_DMA_MODE" or "SD_POLLING_MODE" inside the stm32_eval_sdio_sd.h
  *             file and uncomment the corresponding line. By default the SD DMA
  *             mode is selected          
  *           - The SD_WriteMultiBlocks() function write only mutli blocks (multiple 
  *             of 512-byte). 
  *           - Any write operation should be followed by two functions to check
  *             if the DMA Controller and SD Card status.
  *              - SD_ReadWaitOperation(): this function insure that the DMA
  *                controller has finished all data transfer.
  *              - SD_GetStatus(): to check that the SD Card has finished the 
  *                data transfer and it is ready for data.         
  *                
  *           - The DMA transfer is finished by the SDIO Data End interrupt. User
  *             has to call the SD_ProcessIRQ() function inside the SDIO_IRQHandler().
  *             Don't forget to enable the SDIO_IRQn interrupt using the NVIC controller.      
               
  *             
  *          D - SD card status
  *          ================== 
  *           - At any time, you can check the SD Card status and get the SD card
  *             state by using the SD_GetStatus() function. This function checks
  *             first if the SD card is still connected and then get the internal
  *             SD Card transfer state.     
  *           - You can also get the SD card SD Status register by using the 
  *             SD_SendSDStatus() function.       
  *               
  *          E - Programming Model
  *          ===================== 
  *             Status = SD_Init(); // Initialization Step as described in section A
  *               
  *             // SDIO Interrupt ENABLE
  *             NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  *             NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  *             NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  *             NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  *             NVIC_Init(&NVIC_InitStructure);
  *             
  *             // Write operation as described in Section C
  *             Status = SD_WriteBlock(buffer, address, 512);
  *             Status = SD_WaitWriteOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK); 
  *             
  *             Status = SD_WriteMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
  *             Status = SD_WaitWriteOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);     
  *             
  *             // Read operation as described in Section B
  *             Status = SD_ReadBlock(buffer, address, 512);
  *             Status = SD_WaitReadOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);
  *             
  *             Status = SD_ReadMultiBlocks(buffer, address, 512, NUMBEROFBLOCKS);
  *             Status = SD_WaitReadOperation();
  *             while(SD_GetStatus() != SD_TRANSFER_OK);            
  *               
  *                                     
  *          STM32 SDIO Pin assignment
  *          =========================    
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+---------------+-------------+
  *          |  STM32 SDIO Pins            |     SD        |    Pin      |
  *          +-----------------------------+---------------+-------------+
  *          |      SDIO D2                |   D2          |    1        |
  *          |      SDIO D3                |   D3          |    2        |
  *          |      SDIO CMD               |   CMD         |    3        |
  *          |                             |   VCC         |    4 (3.3 V)|
  *          |      SDIO CLK               |   CLK         |    5        |
  *          |                             |   GND         |    6 (0 V)  |
  *          |      SDIO D0                |   D0          |    7        |
  *          |      SDIO D1                |   D1          |    8        |  
  *          +-----------------------------+---------------+-------------+  
  *              
  *  @endverbatim                
  *             
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "bsp_sdio_sd.h"


//SDIO Static flags, TimeOut, FIFO Address  
#define NULL 0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)

//Mask for errors Card Status R1 (OCR Register) 
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)


// Masks for R6 Response 
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SD_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SD_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SD_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SD_ALLZERO                      ((uint32_t)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SD_CARD_LOCKED                  ((uint32_t)0x02000000)

#define SD_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SD_0TO7BITS                     ((uint32_t)0x000000FF)
#define SD_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SD_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SD_24TO31BITS                   ((uint32_t)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SD_HALFFIFO                     ((uint32_t)0x00000008)
#define SD_HALFFIFOBYTES                ((uint32_t)0x00000020)
 
// Command Class Supported 
#define SD_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SD_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SD_CCCC_ERASE                   ((uint32_t)0x00000020)


// Following commands are SD Card Specific commands.
// SDIO_APP_CMD should be sent before sending these commands. 
#define SDIO_SEND_IF_COND               ((uint32_t)0x00000008)
  

// SDIO_SD Private Variables
static uint32_t CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;
static uint8_t SDSTATUS_Tab[16];
__IO uint32_t StopCondition = 0;
__IO SD_Error TransferError = SD_OK;
__IO uint32_t TransferEnd = 0;
SD_CardInfo SDCardInfo;

SDIO_InitTypeDef SDIO_InitStructure;
SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
SDIO_DataInitTypeDef SDIO_DataInitStructure;   
 


// SDIO_SD Private Function Prototypes
static SD_Error CmdError(void);
static SD_Error CmdResp1Error(uint8_t cmd);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp3Error(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca);
static SD_Error SDEnWideBus(FunctionalState NewState);
static SD_Error IsCardProgramming(uint8_t *pstatus);
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr);
uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes);
  

// SDIO_SD Private Functions

/**
  * @brief  SDIO�����GPIO����
  * @param  ��
  * @retval ��
  */
static void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_SDIO_CLK_FUNC(GPIO_SDIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_D0_PIN;
	GPIO_Init(GPIO_SDIO_D0_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_D1_PIN;
	GPIO_Init(GPIO_SDIO_D1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_D2_PIN;
	GPIO_Init(GPIO_SDIO_D2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_D3_PIN;
	GPIO_Init(GPIO_SDIO_D3_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_CK_PIN;
	GPIO_Init(GPIO_SDIO_CK_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_SDIO_CMD_PIN;
	GPIO_Init(GPIO_SDIO_CMD_PORT, &GPIO_InitStruct);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

/**
  * @brief  SDIO�����Ƕ���ж��������ƣ������ж����ȼ�
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  //����SDIO���ж����ȼ���
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  SDIO �� Tx ����DMA2����
  * @param  BufferSRC: ָ��Դ��������ָ��
  * @param  BufferSize: buffer��С
  * @retval ��
  */
static void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{

  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  /*!< DMA2 Channel4 disable */
  DMA_Cmd(DMA2_Channel4, DISABLE);

  /*!< DMA2 Channel4 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  /*!< DMA2 Channel4 enable */
  DMA_Cmd(DMA2_Channel4, ENABLE);  
}

/**
  * @brief  SDIO �� Rx ����DMA2����
  * @param  BufferDST: ָ��Ŀ�껺������ָ��
  * @param  BufferSize: buffer��С
  * @retval ��
  */
static void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  /*!< DMA2 Channel4 disable */
  DMA_Cmd(DMA2_Channel4, DISABLE);

  /*!< DMA2 Channel4 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  /*!< DMA2 Channel4 enable */
  DMA_Cmd(DMA2_Channel4, ENABLE); 
}


/**
  * @brief  ����DMA���������״̬
  * @param  ��
  * @retval SDIO �� DMA ͨ��״̬
  */
uint32_t SD_DMAEndOfTransferStatus(void)
{
  return (uint32_t)DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

/**
  * @brief  SD��GPIO��ʼ�����ж����ȼ����Լ���λSDIO���мĴ���
  * @param  ��
  * @retval ��
  */
void SD_Init(void)
{
	GPIO_Config();  //��ʼ��SDIO�����GPIO
  NVIC_Configuration();  //�ж����ȼ�
  SDIO_DeInit(); //��SDIO��������мĴ�����λ
	SD_PowerOFF();
}

/**
  * @brief  ��SD�����ڴ���״̬(׼����������)��
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_EnterCardIdentificationMode(void)
{
  SD_Error errorstatus = SD_OK;

  errorstatus = SD_PowerON();  //�ж� SD�� ��״̬(����ѹ��û����Ӧ(MMC��))

  if (errorstatus != SD_OK)
  {
    //CMD��Ӧ��ʱ(�ȴ�CMDSENT��־)
    return(errorstatus);
  }

  errorstatus = SD_InitializeCards();  //SD���Ŀ�״̬��������SD������׼��״̬��ʶ��״̬�Լ�����״̬

  if (errorstatus != SD_OK)
  {
    //CMD��Ӧ��ʱ(�ȴ�CMDSENT��־)
    return(errorstatus);
  }

  //����SDIO����
  /*!< SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_TRANSFER_CLK_DIV) */
  /*!< on STM32F2xx devices, SDIOCLK is fixed to 72MHz */
	//SD���������״̬����ʶ��ģʽ�Ѿ���ɣ����ڴ������ݴ���ģʽ
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);
  
  if (errorstatus == SD_OK)
  {
    /*----------------- Read CSD/CID MSD registers ------------------*/
    errorstatus = SD_GetCardInfo(&SDCardInfo);  //�����SD_InitializeCards������ȡ�Ŀ���Ϣ�����SDCardInfo�ṹ����
	}

  if (errorstatus == SD_OK)
  {
    /*----------------- Select Card --------------------------------*/
    errorstatus = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
  }

  if (errorstatus == SD_OK)
  {
    errorstatus = SD_EnableWideBusOperation(SDIO_BusWide_4b); //�������߿��Ϊ4bit
  }  

  return(errorstatus);
}

/**
  * @brief  ��ȡ��ǰsd�����ݴ���״̬
  * @param  ��
  * @retval SDTransferState: ���ݴ���״̬
  *   This value can be: 
  *        - SD_TRANSFER_OK: û�����ݴ������ڽ���
  *        - SD_TRANSFER_BUSY: ���ݴ������ڽ���
  */
SDTransferState SD_GetStatus(void)
{
  SDCardState cardstate =  SD_CARD_TRANSFER;

  cardstate = SD_GetState();
  
  if (cardstate == SD_CARD_TRANSFER)
  {
    return(SD_TRANSFER_OK);
  }
  else if(cardstate == SD_CARD_ERROR)
  {
    return (SD_TRANSFER_ERROR);
  }
  else
  {
    return(SD_TRANSFER_BUSY);
  }
}

/**
  * @brief  ���ص�ǰ����״̬
  * @param  ��
  * @retval SDCardState: SD�������SD����ǰ״̬
  */
SDCardState SD_GetState(void)
{
  uint32_t resp1 = 0;
	if (SD_SendStatus(&resp1) != SD_OK)
	{
		return SD_CARD_ERROR;
	}
	else
	{
		return (SDCardState)((resp1 >> 9) & 0x0F);
	}
}


/**
  * @brief  ��ѯ���Ĺ�����ѹ������ʱ��
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_PowerON(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t response = 0, count = 0, validvoltage = 0;
  uint32_t SDType = SD_STD_CAPACITY;

  /*!< Power ON Sequence -----------------------------------------------------*/
  /*!< Configure the SDIO peripheral */
  /*!< SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_INIT_CLK_DIV) */
  /*!< on STM32F2xx devices, SDIOCLK is fixed to 72MHz */
  /*!< SDIO_CK for initialization should not exceed 400 KHz */ 
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;  //178
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;  //�ر���·����
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);

  /*!< Set Power State to ON */
  SDIO_SetPowerState(SDIO_PowerState_ON);  //����Դ

  /*!< Enable SDIO Clock */
  SDIO_ClockCmd(ENABLE);   //ʹ��SDIOʱ��

  /*!< CMD0: GO_IDLE_STATE ---------------------------------------------------*/
  /*!< No CMD response required */
	//CMD0����λ���еĿ���idle״̬
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;  //����Ĳ������޲���
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_GO_IDLE_STATE;  //������� CMD0
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;  //��Ӧ����(����Ӧ)
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;    //SDIO ���ȴ�
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //����CPSM״̬��
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdError();  //���CMD0�����Ƿ��ͳɹ�

  if (errorstatus != SD_OK)
  {
    /*!< CMD Response TimeOut (wait for CMDSENT flag) */
    return(errorstatus);
  }

  /*!< CMD8: SEND_IF_COND ----------------------------------------------------*/
  /*!< ����CMD8��֤SD���ӿڹ���״̬ */
  /*!< ����: - [31:12]: ���� (Ӧ������Ϊ0)
               - [11:8]: ������ѹ (VHS) 0x1 (��Χ: 2.7-3.6 V)
               - [7:0]: ���ģʽ (�Ƽ� 0xAA) */
  /*!< ������Ӧ����: R7 */
	//����SD���ӿ���������������֧�ֵĵ�ѹ��Ϣ����ѯ�ʿ��Ƿ�֧��
  SDIO_CmdInitStructure.SDIO_Argument = SD_CHECK_PATTERN; //�в���������Ϊ��[31:12]����λ��[11:8]VHS��[7:0]���ģʽ
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_IF_COND; //������ţ�CMD8
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short; //��Ӧ����(����Ӧ)
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;  //SDIO ���ȴ�����Ϊ�����Ը�������Ƿ�ɹ������ж�
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp7Error();

	/* ͨ��CMD8�����ж�SD�İ汾�ţ������V2.0���ȷ�CMD8�����ض�����ACMD41��
	�������V2.0���ȷ�CMD55�����ض�����ACMD41 */
  if (errorstatus == SD_OK)
  {
    CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /*!< SD Card 2.0 */
    SDType = SD_HIGH_CAPACITY;  //���SD��Ϊ2.0�汾������Ǵ�����SD��
  }
  else
  {
    /*!< CMD55 */
		//CMD55��ָ���¸�����Ϊ�ض�Ӧ��������Ǳ�׼����
    SDIO_CmdInitStructure.SDIO_Argument = 0x00; //�������(�޲���)
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;  //������ţ�CMD55
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ����(����Ӧ)
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;  //SDIO ���ȴ�
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SD_CMD_APP_CMD);  //�ж���Ӧ����Ϊ R1 ������
  }
  /*!< CMD55 */
  SDIO_CmdInitStructure.SDIO_Argument = 0x00;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

	//���errorstatusΪCommand_TimeOut��˵����MMC��
	//���errorstatusΪSD_OK��˵����SD��2.0��SD��1.x
  if (errorstatus == SD_OK)
  {
		//ACMD41������Ҫ�󿨷�������֧����Ϣ(HCS)��OCR�Ĵ���������
    while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
    {
      //����CMD55 APP_CMD, RCAΪ0
			//ָ���¸�����Ϊ�ض�Ӧ��������Ǳ�׼����
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType; //���������[32]����λ��[30]HCS(OCR[30]),[29:24]����λ,[23:0]VDD��ѹ(OCR[23:0])
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND; 
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp3Error();  //�ж���Ӧ����Ϊ R1 ������
      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      response = SDIO_GetResponse(SDIO_RESP1);  //SDIO��Ӧ�Ĵ�������ΪACMD41�Ƕ���Ӧ���ͣ����Կ���״̬�Ǵ���SDIO_RESP1�Ĵ����С������������״̬�����յ���Ӧ�Ĳ�����Ϣ��
      validvoltage = (((response >> 31) == 1) ? 1 : 0);
      count++;
    }
    if (count >= SD_MAX_VOLT_TRIAL)
    {
      errorstatus = SD_INVALID_VOLTRANGE;
      return(errorstatus);
    }

    if (response &= SD_HIGH_CAPACITY)
    {
      CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }

  }/*!< else MMC Card */

  return(errorstatus);
}

/**
  * @brief  �ر�SDIO����ź�
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_PowerOFF(void)
{
  SD_Error errorstatus = SD_OK;

	//����Դ״̬����Ϊ�ر�
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  return(errorstatus);
}

/**
  * @brief  ���п����ſ��������״̬��
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_InitializeCards(void)
{
  SD_Error errorstatus = SD_OK;
  uint16_t rca = 0x01;

  if (SDIO_GetPowerState() == SDIO_PowerState_OFF)
  {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    /*!< Send CMD2 ALL_SEND_CID */
		//CMD2��֪ͨ���еĿ�ͨ��CMD�߷���CIDֵ
    SDIO_CmdInitStructure.SDIO_Argument = 0x0;  //����������޲�����
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID;  //������ţ�CMD2
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;  //��Ӧ���ͣ�����Ӧ��
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No; //SDIO ���ȴ�
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp2Error();  //�ж���Ӧ����Ϊ R2 ������

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
		//SDIO��Ӧ�Ĵ�������ΪCMD2�ǳ���Ӧ���ͣ�����CID�Ǵ���SDIO_RESP1/2/3/4�Ĵ����С�
    CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);  
    CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }
	//����CardType��ֵ��SD������ʶ��״̬
  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||  (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||  (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
      ||  (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
		//CMD3��֪ͨ���п������µ�RCA
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;  //�������ͣ��޲�����
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR; //������ţ�CMD3
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No; //SDIO ���ȴ�
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca); //�ж���Ӧ����ΪR6�����ݣ���������RCA��ֵ��rca

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    RCA = rca;

    //����CMD9 SD_CMD_SEND_CSD��������Ϊ��Ƭ��RCA
		//CMD9:ͨ��RCAѡ������Ȼ����ͨ��CMD�߷���CSD����
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)(rca << 16); //���������[31:16]RCA,[15:0]���λ
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_CSD;  //������ţ�CMD9
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;  //��Ӧ���ͣ�����Ӧ��
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;  //SDIO ���ȴ�
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp2Error();  //�ж���Ӧ����ΪR6������

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
		//SDIO��Ӧ�Ĵ�������ΪCMD9�ǳ���Ӧ���ͣ�����CSD�Ǵ���SDIO_RESP1/2/3/4�Ĵ����С�
    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }

  errorstatus = SD_OK; //���еĿ�������ʼ��

  return(errorstatus);
}

/**
  * @brief  ���ع����ض���Ƭ����Ϣ
  * @param  cardinfo: ָ���������SD����SD_CardInfo�ṹ��ָ����Ϣ
  * @retval SD_Error: SD��������
  */
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;    
  }


  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;
  
  return(errorstatus);
}

/**
  * @brief  ���card֧�֣����������Ŀ����ÿ����߲���
  * @param  WideMode: ָ��SD��������ģʽ
  *   �ò�������������ֵ֮һ:
  *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
  *     @arg SDIO_BusWide_4b: 4-bit data transfer
  *     @arg SDIO_BusWide_1b: 1-bit data transfer
  * @retval SD_Error: SD��������
  */
SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus)
{
  SD_Error errorstatus = SD_OK;
  uint8_t tmp = 0;

  errorstatus = SD_SendSDStatus((uint32_t *)SDSTATUS_Tab);

  if (errorstatus  != SD_OK)
  {
    return(errorstatus);
  }

  /*!< Byte 0 */
  tmp = (uint8_t)((SDSTATUS_Tab[0] & 0xC0) >> 6);
  cardstatus->DAT_BUS_WIDTH = tmp;

  /*!< Byte 0 */
  tmp = (uint8_t)((SDSTATUS_Tab[0] & 0x20) >> 5);
  cardstatus->SECURED_MODE = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((SDSTATUS_Tab[2] & 0xFF));
  cardstatus->SD_CARD_TYPE = tmp << 8;

  /*!< Byte 3 */
  tmp = (uint8_t)((SDSTATUS_Tab[3] & 0xFF));
  cardstatus->SD_CARD_TYPE |= tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)(SDSTATUS_Tab[4] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;

  /*!< Byte 5 */
  tmp = (uint8_t)(SDSTATUS_Tab[5] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;

  /*!< Byte 6 */
  tmp = (uint8_t)(SDSTATUS_Tab[6] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;

  /*!< Byte 7 */
  tmp = (uint8_t)(SDSTATUS_Tab[7] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((SDSTATUS_Tab[8] & 0xFF));
  cardstatus->SPEED_CLASS = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((SDSTATUS_Tab[9] & 0xFF));
  cardstatus->PERFORMANCE_MOVE = tmp;

  /*!< Byte 10 */
  tmp = (uint8_t)((SDSTATUS_Tab[10] & 0xF0) >> 4);
  cardstatus->AU_SIZE = tmp;

  /*!< Byte 11 */
  tmp = (uint8_t)(SDSTATUS_Tab[11] & 0xFF);
  cardstatus->ERASE_SIZE = tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)(SDSTATUS_Tab[12] & 0xFF);
  cardstatus->ERASE_SIZE |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((SDSTATUS_Tab[13] & 0xFC) >> 2);
  cardstatus->ERASE_TIMEOUT = tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((SDSTATUS_Tab[13] & 0x3));
  cardstatus->ERASE_OFFSET = tmp;
 
  return(errorstatus);
}

/**
  * @brief  ���card֧�֣����������Ŀ����ÿ����߲���
  * @param  WideMode: ָ��SD��������ģʽ
  *   �ò�������������ֵ֮һ:
  *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
  *     @arg SDIO_BusWide_4b: 4-bit data transfer
  *     @arg SDIO_BusWide_1b: 1-bit data transfer
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_EnableWideBusOperation(uint32_t WideMode)
{
  SD_Error errorstatus = SD_OK;

  //MMC����֧�ִ˹���
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SD_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
  else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    if (SDIO_BusWide_8b == WideMode)
    {
      errorstatus = SD_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
    else if (SDIO_BusWide_4b == WideMode)
    {
      errorstatus = SDEnWideBus(ENABLE);

      if (SD_OK == errorstatus)
      {
        //����SDIO����
				//��SDIO�������߿������Ϊ4bit
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
    else
    {
      errorstatus = SDEnWideBus(DISABLE);

      if (SD_OK == errorstatus)
      {
        //����SDIO����
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
  }

  return(errorstatus);
}

/**
  * @brief  ѡ���ȡ��ѡ����Ӧ�Ŀ�
  * @param  addr: Ҫѡ��Ŀ��ĵ�ַ
  * @retval SD_Error: SD��������
  */
SD_Error SD_SelectDeselect(uint32_t addr)
{
  SD_Error errorstatus = SD_OK;

  /*!< Send CMD7 SDIO_SEL_DESEL_CARD */
	//CMD7:ѡ��/ȡ��RCA��ַ��
  SDIO_CmdInitStructure.SDIO_Argument =  addr;  //���������[31:16]RCA��ַ,[15:0]���λ
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEL_DESEL_CARD; //������ţ�CMD7
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No; //SDIO ���ȴ�
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SEL_DESEL_CARD);  //�ж���Ӧ����λR1������

  return(errorstatus);
}

/**
  * @brief  ����ӿ�Ƭ��ָ���ĵ�ַ��ȡһ�����ݿ顣���ݴ���ɲ���DMA��ʽ����ѯ��ʽ���й���
  * @note   �˲���Ӧ��ѭ�������������DMA��������SD����״̬
  *          - SD_ReadWaitOperation(): �ù���ȷ��DMA������������������ݴ���
  *          - SD_GetStatus(): ���SD���Ƿ���������ݴ��䣬�Ƿ���׼��������            
  * @param  readbuff: ָ��������յ������ݵĻ�������ָ��
  * @param  ReadAddr: Ҫ��ȡ���ݵĵ�ַ
  * @param  BlockSize: SD�����ݿ��С�����СӦ����512
  * @retval SD_Error: SD��������
  */
SD_Error SD_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
  SD_Error errorstatus = SD_OK;
#if defined (SD_POLLING_MODE) 
  uint32_t count = 0, *tempbuff = (uint32_t *)readbuff;
#endif

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 0;
  
  SDIO->DCTRL = 0x0;

  
  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  /*!< Send CMD17 READ_SINGLE_BLOCK */
	//CMD17��ȡ512�ֽڵĿ�
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)ReadAddr;  //���������[31:0]��ȡ���ݵĵ�ַ
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

#if defined (SD_POLLING_MODE)  //��ѯģʽ����ͨģʽ�� 
  /*!< In case of single block transfer, no need of stop transfer at all.*/
  /*!< Polling mode */
  while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
      {
        *(tempbuff + count) = SDIO_ReadData();
      }
      tempbuff += 8;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }
  while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
  {
    *tempbuff = SDIO_ReadData();
    tempbuff++;
  }
  
  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

#elif defined (SD_DMA_MODE)  //DMAģʽ
    SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
    SDIO_DMACmd(ENABLE);
    SD_LowLevel_DMA_RxConfig((uint32_t *)readbuff, BlockSize);
#endif

  return(errorstatus);
}

/**
  * @brief  ����ӿ�Ƭ��ָ���ĵ�ַ��ȡ�顣���ݴ���ɲ���DMA��ʽ����ѯ��ʽ���й���
  * @note   �˲���Ӧ��ѭ�������������DMA��������SD����״̬
  *          - SD_ReadWaitOperation(): �ù���ȷ��DMA������������������ݴ���
  *          - SD_GetStatus(): ���SD���Ƿ���������ݴ��䣬�Ƿ���׼��������  
  * @param  readbuff: ָ��������յ������ݵĻ�������ָ��
  * @param  ReadAddr: Ҫ��ȡ���ݵĵ�ַ
  * @param  BlockSize: SD�����ݿ��С�����СӦ����512
  * @param  NumberOfBlocks: Ҫ��ȡ�Ŀ���
  * @retval SD_Error: SD��������
  */
SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 1;
	
  SDIO->DCTRL = 0x0;

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /*!< Set Block Size for Card */
	//CMD16�����ÿ�����ĳ��ȣ�SDHC�������ĳ��ȹ̶�Ϊ512�ֽ�
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) BlockSize;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

  if (SD_OK != errorstatus)
  {
    return(errorstatus);
  }
   
	//�Զ�ȡ���ݽṹ���������
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;  //�������ݵ��ֽڴ�СΪ32*512
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;  //�������ݿ�Ĵ�С512�ֽ�
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO; //����������
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;  //�鴫��
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;  //ʹ��DPSM״̬��
  SDIO_DataConfig(&SDIO_DataInitStructure);

  /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
	//CMD18:������SD����ȡ���ݿ飬ֱ����CMD12�жϣ�����SDHC����һ�ζ�ȡ512�ֽڵĿ�
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)ReadAddr;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_MULT_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_READ_MULT_BLOCK);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
	
  SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
  SDIO_DMACmd(ENABLE);
	SD_LowLevel_DMA_RxConfig((uint32_t *)readbuff, (NumberOfBlocks * BlockSize));
  
  return(errorstatus);
}

/**
  * @brief  �ú����ȴ�ֱ��SDIO DMA���ݴ�����ɡ�
						�ú���Ӧ����SDIO_ReadMultiBlocks()����֮����ã�
						��ȷ���ɿ����͵��������ݶ��Ѿ�ͨ��DMA���������䡣        
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_WaitReadOperation(void)
{
  SD_Error errorstatus = SD_OK;

  while ((SD_DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK));

  if (TransferError != SD_OK)
  {
    return(TransferError);
  }

  return(errorstatus);
}

/**
  * @brief  ����ӿ�Ƭ��ָ����ַ��ʼд��һ���顣���ݴ���ɲ���DMA��ʽ����ѯ��ʽ���й���
  * @note   �˲���Ӧ��ѭ�������������DMA��������SD����״̬
  *          - SD_ReadWaitOperation(): �ù���ȷ��DMA������������������ݴ���
  *          - SD_GetStatus(): ���SD���Ƿ���������ݴ��䣬�Ƿ���׼��������    
  * @param  writebuff: ָ�����Ҫ��������ݵĻ�������ָ��
  * @param  WriteAddr: Ҫ��ȡ���ݵĵ�ַ  
  * @param  BlockSize: SD�����ݿ��С�����СӦ����512
  * @retval SD_Error: SD��������
  */
SD_Error SD_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
  SD_Error errorstatus = SD_OK;

#if defined (SD_POLLING_MODE)
  uint32_t bytestransferred = 0, count = 0, restwords = 0;
  uint32_t *tempbuff = (uint32_t *)writebuff;
#endif

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 0;
  
  SDIO->DCTRL = 0x0;


  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    WriteAddr /= 512;
  }
  
  /*!< Send CMD24 WRITE_SINGLE_BLOCK */
	//CMD24:д��512�ֽڴ�С�Ŀ�
  SDIO_CmdInitStructure.SDIO_Argument = WriteAddr;  //���������[31:0]д�����ݵ�ַ
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  /*!< In case of single data block transfer no need of stop command at all */
#if defined (SD_POLLING_MODE)  //��ѯģʽ
  while (!(SDIO->STA & (SDIO_FLAG_DBCKEND | SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)
    {
      if ((512 - bytestransferred) < 32)
      {
        restwords = ((512 - bytestransferred) % 4 == 0) ? ((512 - bytestransferred) / 4) : (( 512 -  bytestransferred) / 4 + 1);
        for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
        {
          SDIO_WriteData(*tempbuff);
        }
      }
      else
      {
        for (count = 0; count < 8; count++)
        {
          SDIO_WriteData(*(tempbuff + count));
        }
        tempbuff += 8;
        bytestransferred += 32;
      }
    }
  }
  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
    errorstatus = SD_TX_UNDERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }
#elif defined (SD_DMA_MODE)  //DMAģʽ
  SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
  SD_LowLevel_DMA_TxConfig((uint32_t *)writebuff, BlockSize);
  SDIO_DMACmd(ENABLE);
#endif

  return(errorstatus);
}

/**
  * @brief  ����ӿ�Ƭ��ָ����ַ��ʼд�顣���ݴ���ֻ����DMAģʽ���� 
  * @note   �˲���Ӧ��ѭ�������������DMA��������SD����״̬
  *          - SD_ReadWaitOperation(): �ù���ȷ��DMA������������������ݴ���
  *          - SD_GetStatus(): ���SD���Ƿ���������ݴ��䣬�Ƿ���׼��������    
  * @param  WriteAddr: Ҫ��ȡ���ݵĵ�ַ
  * @param  writebuff: ָ�����Ҫ��������ݵĻ�������ָ��
  * @param  BlockSize: SD�����ݿ��С�����СӦ����512
  * @param  NumberOfBlocks: Ҫд��Ŀ���
  * @retval SD_Error: SD��������
  */
SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  __IO uint32_t count = 0;

  TransferError = SD_OK;
  TransferEnd = 0;
  StopCondition = 1;
  
  SDIO->DCTRL = 0x0;

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /*!< To improve performance */
	//CMD55
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) (RCA << 16);
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);


  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
  /*!< To improve performance */
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)NumberOfBlocks;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCK_COUNT;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCK_COUNT);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }


  /*!< Send CMD25 WRITE_MULT_BLOCK with argument data address */
	//CMD25:������SD��д�����ݿ飬ֱ����CMD12�жϣ�ÿ���鳤��Ϊ512�ֽ�
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)WriteAddr;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_MULT_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_WRITE_MULT_BLOCK);

  if (SD_OK != errorstatus)
  {
    return(errorstatus);
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
  SDIO_DMACmd(ENABLE);
  SD_LowLevel_DMA_TxConfig((uint32_t *)writebuff, (NumberOfBlocks * BlockSize));
  return(errorstatus);
}

/**
  * @brief  �ú����ȴ�ֱ��SDIO DMA���ݴ�����ɡ�
						�ú���Ӧ����SDIO_WriteBlock()��SDIO_WriteMultiBlocks()����֮����ã�
						��ȷ�������͵��������ݶ��Ѿ�ͨ��DMA���������䡣       
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_WaitWriteOperation(void)
{
  SD_Error errorstatus = SD_OK;

  while ((SD_DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK));

  if (TransferError != SD_OK)
  {
    return(TransferError);
  }

  //������о�̬��־
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}

/**
  * @brief  ��ȡ��ǰ���ݴ���״̬
  * @param  ��
  * @retval SDTransferState: ���ݴ���״̬
  *   ���ֵ������: 
  *        - SD_TRANSFER_OK: û�����ݴ������ڽ���
  *        - SD_TRANSFER_BUSY: ���ݴ������ڽ���
  */
SDTransferState SD_GetTransferState(void)
{
  if (SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_RXACT))
  {
    return(SD_TRANSFER_BUSY);
  }
  else
  {
    return(SD_TRANSFER_OK);
  }
}

/**
  * @brief  ��ֹ���ڽ��е����ݴ���
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_StopTransfer(void)
{
  SD_Error errorstatus = SD_OK;

  /*!< Send CMD12 STOP_TRANSMISSION  */
  SDIO->ARG = 0x0;  //���������ֱ�Ӳ����Ĵ���
  SDIO->CMD = 0x44C; //������ţ�ֱ�Ӳ����Ĵ���
  errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);

  return(errorstatus);
}

/**
  * @brief  �������Ϊ������ָ�����ڴ�����
  * @param  startaddr: ��ʼ��ַ
  * @param  endaddr: ������ַ
  * @retval SD_Error: SD��������
  */
SD_Error SD_Erase(uint32_t startaddr, uint32_t endaddr)
{
  SD_Error errorstatus = SD_OK;
  uint32_t delay = 0;
  __IO uint32_t maxdelay = 0;
  uint8_t cardstate = 0;

	//���card�������Ƿ�֧��erase����
  if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0)
  {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

	//SDIO_CKƵ�� = SDIOCLK/[CLKDIV + 2]
  maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
		//�ֽڶ��룬��ΪSDIO_HIGH_CAPACITY_SD_CARD���͵�SD������Ĺ̶��Ĵ�СΪ512�ֽ�
    startaddr /= 512;
    endaddr /= 512;
  }
  
	//����sd���淶1.0 ERASE_GROUP_START (CMD32)��erase_group_end(CMD33)
  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
		//CMD32�����ò�������ʼ���ַ
    SDIO_CmdInitStructure.SDIO_Argument = startaddr; //���������[31:0]���ݵ�ַ
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_START; //������ţ�CMD32
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;  //ʹ��CPSM״̬��
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START);  //�ж���Ӧ����Ϊ R1 ������
    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }

		//CMD33�����ò����Ľ������ַ
    SDIO_CmdInitStructure.SDIO_Argument = endaddr;//���������[31:0]���ݵ�ַ
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_END;  //������ţ�CMD33
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);
    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }
  }

  /*!< Send CMD38 ERASE */
	//CMD38������Ԥ��ѡ���Ŀ�
  SDIO_CmdInitStructure.SDIO_Argument = 0;  //����������޲�����
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ERASE; //������ţ�CMD38
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_ERASE);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

	//�ȴ��������
  for (delay = 0; delay < maxdelay; delay++)
  {}
		
  //�ȴ���Ƭ������״̬
  errorstatus = IsCardProgramming(&cardstate);
	//�ȴ�SD���˳� SD_CARD_PROGRAMMING ״̬
  while ((errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return(errorstatus);
}

/**
  * @brief  ���ص�ǰ����״̬
  * @param  ָ�򻺳�����ָ�룬���н�����SD��״̬(��״̬�Ĵ���)
  * @retval SD_Error: SD��������
  */
SD_Error SD_SendStatus(uint32_t *pcardstatus)
{
  SD_Error errorstatus = SD_OK;

  SDIO->ARG = (uint32_t) RCA << 16;
  SDIO->CMD = 0x44D;
  
  errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  *pcardstatus = SDIO->RESP1;
  return(errorstatus);
}

/**
  * @brief  ���ص�ǰSD����״̬
  * @param  psdstatus: ָ�����SD��״̬�Ļ�������ָ��(SD״̬�Ĵ���)
  * @retval SD_Error: SD��������
  */
SD_Error SD_SendSDStatus(uint32_t *psdstatus)
{
  SD_Error errorstatus = SD_OK;
  uint32_t count = 0;

	//����Ӧ�Ĵ���RESP1��ȡ��Ӧֵ
  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  //������Ŀ��С�����ڿ��ĵ�ǰ���С�������ÿ��Ŀ��С
  SDIO_CmdInitStructure.SDIO_Argument = 64;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  /*!< CMD55 */
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 64;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  //����ACMD13 sd_app_status��������Ϊ����RCA
  SDIO_CmdInitStructure.SDIO_Argument = 0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_STAUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SD_CMD_SD_APP_STAUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
      {
        *(psdstatus + count) = SDIO_ReadData();
      }
      psdstatus += 8;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
  {
    *psdstatus = SDIO_ReadData();
    psdstatus++;
  }

  //������о�̬״̬��־
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}

/**
  * @brief  ���������и��ж�
  * @param  ��
  * @retval SD_Error: SD��������
  */
SD_Error SD_ProcessIRQSrc(void)
{
  if (StopCondition == 1)
  {
    SDIO->ARG = 0x0;
    SDIO->CMD = 0x44C;
    TransferError = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
  }
  else
  {
    TransferError = SD_OK;
  }
  SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
  SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE);
  TransferEnd = 1;
  return(TransferError);
}

/**
  * @brief  ���CMD0�Ĵ�������
  * @param  ��
  * @retval SD_Error: SD��������
  */
static SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t timeout;

  timeout = SDIO_CMD0TIMEOUT; /*!< 10000 */

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
  {
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  //������о�̬״̬��־
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}

/**
  * @brief  ���R7��Ӧ�Ĵ�������
  * @param  ��
  * @retval SD_Error: SD��������
  */
static SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t status;
  uint32_t timeout = SDIO_CMD0TIMEOUT;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0))
  {
    timeout--;
    status = SDIO->STA;
  }

  if ((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT))
  {
    //��������V2.0�򿨲�֧�����õĵ�ѹ��Χ
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }

  if (status & SDIO_FLAG_CMDREND)
  {
    //����SD ����V2.0
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
    return(errorstatus);
  }
  return(errorstatus);
}

/**
  * @brief ���R1��Ӧ�Ĵ�������
  * @param  cmd: ������������
  * @retval SD_Error: SD��������
  */
static SD_Error CmdResp1Error(uint8_t cmd)
{
  while (!(SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
  }

  SDIO->ICR = SDIO_STATIC_FLAGS;

  return (SD_Error)(SDIO->RESP1 &  SD_OCR_ERRORBITS);
}

/**
  * @brief  ���R3 (OCR)��Ӧ�Ĵ�������
  * @param  ��
  * @retval SD_Error: SD��������
  */
static SD_Error CmdResp3Error(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  return(errorstatus);
}

/**
  * @brief  ���R2 (CID��CSD)��Ӧ�Ĵ�������
  * @param  ��
  * @retval SD_Error: SD��������
  */
static SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
  uint32_t status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}

/**
  * @brief  ���R6 (RCA)��Ӧ�Ĵ�������
  * @param  cmd: ������������
  * @param  prca: ָ�������ָ�룬�ñ���������SD����Ե�ַRCA
  * @retval SD_Error: SD��������
  */
static SD_Error CmdResp6Error(uint8_t cmd, uint16_t *prca)
{
  SD_Error errorstatus = SD_OK;
  uint32_t status;
  uint32_t response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  //����յ�����Ӧ�Ƿ�������������
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  /*!< We have received response, retrieve it.  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  {
    *prca = (uint16_t) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_R6_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_R6_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  return(errorstatus);
}

/**
  * @brief  ���û����SDIO������ģʽ
  * @param  NewState: SDIO������ģʽ����״̬
  *   �������������: ENABLE or DISABLE.
  * @retval SD_Error: SD��������
  */
static SD_Error SDEnWideBus(FunctionalState NewState)
{
  SD_Error errorstatus = SD_OK;

  uint32_t scr[2] = {0, 0};

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  
  if (NewState == ENABLE)  //���Ҫ���ÿ����߲���
  {
    //�������֧�ֿ����߲���
    if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			//ACMD6�������������߿�ȣ�"00"= 1bit,"10" = 4bit
      SDIO_CmdInitStructure.SDIO_Argument = 0x2; //���������[31:2]���λ��[1:0]���߿��
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;  //������ţ�ACMD6
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);  //�ж���Ӧ����Ϊ R1 ������

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }   
  else  //��������߲���������
  {
    //�������Ŀ�֧��1λģʽ����
    if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);


      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			//����ACMD6 APP_CMD����Ϊ2�Ŀ�����ģʽ
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}

/**
  * @brief  ���SD���Ƿ��ڱ��״̬
  * @param  pstatus: ָ�����SD��״̬�ı�����ָ��
  * @retval SD_Error: SD��������
  */
static SD_Error IsCardProgramming(uint8_t *pstatus)
{
  SD_Error errorstatus = SD_OK;
  __IO uint32_t respR1 = 0, status = 0;

	//CMD13��ѡ����ͨ��CMD�߷�������״̬�Ĵ���
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  status = SDIO->STA;
  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  status = (uint32_t)SDIO_GetCommandResponse();

  //����յ�����Ӧ�Ƿ�������������
  if (status != SD_CMD_SEND_STATUS)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);


  /*!< We have received response, retrieve it for analysis  */
  respR1 = SDIO_GetResponse(SDIO_RESP1);
  /*!< Find out card status */
  *pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

  if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (respR1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (respR1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (respR1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (respR1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (respR1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (respR1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (respR1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (respR1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (respR1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (respR1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (respR1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (respR1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}

/**
  * @brief  �ҵ�SD��SCR�Ĵ���ֵ
  * @param  rca: ��ѡ��Ƭ��ַ
  * @param  pscr: ָ�����SCRֵ�Ļ�������ָ��
  * @retval SD_Error: SD��������
  */
static SD_Error FindSCR(uint16_t rca, uint32_t *pscr)
{
  uint32_t index = 0;
  SD_Error errorstatus = SD_OK;
  uint32_t tempscr[2] = {0, 0};

  /*!< Set Block Size To 8 Bytes */
  /*!< Send CMD55 APP_CMD with argument as card's RCA */
	//CMD16������SDIO_HIGH_CAPACITY_SD_CARD��Ĺ̶�Ϊ512�ֽڣ�������SDIO_STD_CAPACITY_SD_CARD_V2_0����Ҫָ����ĳ���
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)8;  //ָ����Ĵ�СΪ8�ֽ�
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;   //���������ֽ���
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;  //�鳤��Ϊ8�ֽ�
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;  //ָ�����ݴ��䷽�򣬿���������
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;  //ָ�����ݴ���ģʽ�������ݴ���
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;  //ʹ��DPSM״̬��
  SDIO_DataConfig(&SDIO_DataInitStructure);


  /*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
	//ACMD51:��ȡ���üĴ���
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;  //����������޲�����
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_SEND_SCR; //������ţ�ACMD51
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //��Ӧ���ͣ�����Ӧ��
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  while (!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
    {
      *(tempscr + index) = SDIO_ReadData();
      index++;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  *(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

  *(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

  return(errorstatus);
}

/**
  * @brief  ת����2Ϊ�ݵ��ֽ��������ظ���
  * @param  NumberOfBytes: �ֽ���
  * @retval ��
  */
uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes)
{
  uint8_t count = 0;

  while (NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }
  return(count);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
