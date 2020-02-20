/******************************************************************************

                  ��Ȩ���� (C), 2018-2025, �Ϻ�������Ϣ��ȫ�����ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : convert.c
  �� �� ��   : ����
  ��    ��   : ���ķ�
  ��������   : 2018��12��6��
  ����޸�   :
  ��������   : ����ת��
  �����б�   :
              Escape_frame_data
              Reduction_frame_data
  �޸���ʷ   :
  1.��    ��   : 2018��12��6��
    ��    ��   : ���ķ�
    �޸�����   : �����ļ�

******************************************************************************/
#include "convert.h"

/*
 * ����ת����򣺲���0x7D��Ϊת���ַ�����0x7E��0x7F�Լ�0x7D����ת��
 * 0x7Eת��Ϊ0x7D��0x5E��
 * 0x7Fת��Ϊ0x7D��0x5F��
 * 0x7Dת��Ϊ0x7D��0x5D
 */


/*****************************************************************************
 �� �� ��: Escape_frame_data
 ��������: ����ת��
 �������: uint8_t* input    : ��������bufferָ��
           uint32_t ilen     : �������ݳ���
           uint8_t* output   : �������bufferָ��
           uint32_t* olen    : ������ݳ���
 �������: uint8_t
 �� �� ֵ: 1��ת��ʧ�� 0��ת��ɹ�
 ���ú���: 
 ��������: 
 
 �޸���ʷ:
  1.��    ��: 2018��12��6��
    ��    ��: ���ķ�
    �޸�����: �����ɺ���

*****************************************************************************/
uint8_t Escape_frame_data(uint8_t* input, uint32_t ilen, uint8_t* output, uint32_t* olen)
{
    int i=0,index=0;
    int count=0;
	 uint8_t replacedata[3][2]={{0x7D,0x5E},{0x7D,0x5F},{0x7D,0x5D}};

    if(input == NULL || ilen <= 0)
    {
        return 1;
    }
    
    for(i=0; i<ilen; i++)
    {
        if(input[i] == 0x7E)
        {
            memcpy(output+index,replacedata[0],2);
            index+=2;
            count++;
        }
        else if(input[i] == 0x7F)
        {
            memcpy(output+index,replacedata[1],2);
            index+=2;
            count++;
        }
        else if(input[i] == 0x7D)
        {
            memcpy(output+index,replacedata[2],2);
            index+=2;
            count++;
        }
        else
        {
            output[index]=input[i];
            index++;
        }
    }
    *olen = index;

    return 0;
}


/*****************************************************************************
 �� �� ��: Reduction_frame_data
 ��������: ���ݷ�ת��
 �������: uint8_t* input    : ��������bufferָ��
           uint32_t ilen     : �������ݳ���
           uint8_t* output   : �������bufferָ��
           uint32_t* olen    : ������ݳ���
 �������: uint8_t
 �� �� ֵ: 1����ת��ʧ�� 0����ת��ɹ�
 ���ú���: 
 ��������: 
 
 �޸���ʷ:
  1.��    ��: 2018��12��6��
    ��    ��: ���ķ�
    �޸�����: �����ɺ���

*****************************************************************************/
uint8_t Reduction_frame_data(uint8_t* input, uint32_t ilen, uint8_t* output, uint32_t* olen)
{
    int i=0, index=0;
    int count = 0;
    uint8_t* pdata=NULL;
 
    if(input == NULL || (ilen-2) <= 0)
    {
        return 1;
    }
 
    pdata = input;
    for(i=0; i < ilen; i++)
    {
        if(pdata[i] == 0x7D)
        {
       	   count++;
 
            if(pdata[i+1] == 0x5E)
            {
           	 output[index]=0x7E;
            }
            else if(pdata[i+1] == 0x5F)
            {
           	 output[index]=0x7F;
            }
            else if(pdata[i+1] == 0x5D)
            {
           	 output[index]=0x7D;
            }
            i++;
            index++;
        }
        else
        {
       	 output[index]=pdata[i];
         index++;
        }
    }
 
     *olen = index;
     return 0;
}
