#include "bsp_sd_test.h"


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;



/**
  * @brief  SD���������ԣ�����ָ����ַ���ڴ档ͨ����ȡ��Щ��ַ�����������бȽϣ����Ƿ�����ɹ�
  * @param  ��
  * @retval ��
  */
void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {
    /* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));  //��0x00��ַ���� NUMBER_OF_BLOCKS �� BLOCK_SIZE ���ֽڴ�С�Ŀռ�
  }

  if (Status == SD_OK)
  {
		//��ȡ���������
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    
		//�ȴ�DMA���ݴ���
    Status = SD_WaitReadOperation();
	
    //�ȴ�SD�����ݽ������
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
	
  if (Status == SD_OK)
  {
		//�����������ȷ��
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(EraseStatus == PASSED)
  {
		//�����ɹ������̵�
    LED_G(ON);
		printf("SD���������Գɹ���\n");
  }
  else
  {
		//����ʧ�ܵ������
    LED_R(ON);  
		printf("SD����������ʧ�ܣ�\n");
    printf("��ܰ��ʾ������SD����֧�ֲ������ԣ���SD����ͨ�������single��д���ԣ�����ʾSD���ܹ�����ʹ�á�\n");
  }
}

/**
  * @brief  SD�������д����
  * @param  ��
  * @retval ��
  */
void SD_SingleBlockTest(void)
{
  /*------------------- Block Read/Write --------------------------*/
  //�����Ҫ���͵�buffer�ռ�
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_OK)
  {
    //��0x00��ַд��һ���飨512byte����С������
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
    //�ȴ�DMA���ݴ���
    Status = SD_WaitWriteOperation();
		
		//�ȴ�SD�������������
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
		//��0x00��ַ��ȡһ�����С������
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
    
		//�ȴ�DMA���ݴ���
    Status = SD_WaitReadOperation();
		
		//�ж�SD�������Ƿ���ɹ�
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
		//���д������ݵ���ȷ��
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
  }
  
  if(TransferStatus1 == PASSED)
  {
    LED_G(ON);
		printf("Single block ���Գɹ���\n");
  }
  else
  {
    LED_R(ON);   
		printf("Single block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�\n");
  }
}

/**
  * @brief  SD��������д����
  * @param  ��
  * @retval ��
  */
void SD_MultiBlockTest(void)
{
  /*--------------- Multiple Block Read/Write ---------------------*/
  //��伴��д��������buffer
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    //��0x00��ַд�����飨512byte����С������
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    
		//�ȴ�DMA���ݴ���
    Status = SD_WaitWriteOperation();
		
		//�ȴ�SD��������
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    //��0x00��ַ��ȡ����飨512byte����С������
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    
		//�ȴ�DMA���ݴ���
    Status = SD_WaitReadOperation();
		
		//�ж�SD���Ƿ���յ�����
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
		//�������д�����ݵ���ȷ��
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)
  {
    LED_G(ON);
		printf("Multi block ���Գɹ���");
  }
  else
  {
    LED_R(ON);    
		printf("Multi block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�");
  }
}

/**
  * @brief  �Ƚ�����buffer������
  * @param  pBuffer1����һ��buffer���ݵĵ�ַ 
						pBuffer2: �ڶ���buffer���ݵĵ�ַ
  * @param  BufferLength: buffer�ĳ���
  * @retval PASSED: pBuffer1 �� pBuffer2 ���
  *         FAILED: pBuffer1 �� pBuffer2 �����
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  ������ݻ�����
  * @param  pBuffer: ָ��Ҫ���Ļ�����
  * @param  BufferLength: ��仺�����Ĵ�С
  * @param  Offset:�ڻ����������ĵ�һ��ֵ
  * @retval ��
  */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * @brief  ���һ���������Ƿ����е�ֵ������0
  * @param  pBuffer: ��Ҫ�ȽϵĻ�����
  * @param  BufferLength: buffer�Ĵ�С
  * @retval PASSED: ��������ֵȫ��Ϊ0
  *         FAILED: ��������ֵ������һ��������0
  */
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    //��һЩSD���У�����״̬��0xFF��������SD������0x00
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/**
  * @brief  SD���Ե�������
  * @param  ��
  * @retval ��
  */
void SDCardTest(void)
{
  /*------------------------------ SD Init ---------------------------------- */
	//SD���Ŀ�ʶ��ģʽ��ɣ�Ҳ��SDIO_CK��Ƶ������Ϊ24MHz�������߿��Ϊ4bit
  if((Status = SD_EnterCardIdentificationMode()) != SD_OK)
  {
    LED_R(ON);  //�����ʼ��ʧ���������
		printf("SDCard ��ʼ��ʧ��,Status = %d\r\n",Status);
		return ;
  }
	else
	{
		LED_G(ON);
		printf("SDCard ��ʼ���ɹ�\r\n");
	}
      
	if(Status == SD_OK)
	{
		/* ��Բ�ͬ���̵� SDCard ��Щ���ܻ����ʧ�ܣ���һֱ���ڲ�������  */
		//LED_B(ON);
		//printf("SDCard ��ʼ��������\r\n");
		//SD_EraseTest();
		
		printf("SDCard ��ʼ�����д����\r\n");
		SD_SingleBlockTest();
		
		printf("SDCard ��ʼ����д����\r\n");
		SD_MultiBlockTest();
	}
}



