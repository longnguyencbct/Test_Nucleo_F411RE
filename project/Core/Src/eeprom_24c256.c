/*
 * eeprom_24c256.c
 *
 *  Created on: Jan 9, 2025
 *      Author: clong
 */

#include "eeprom_24c256.h"

uint8_t at24c_Full_Check(void)
{
	uint32_t addr;
    uint8_t testValue;  // Test value to write
    uint8_t readValue;

    for (addr = 0; addr <= EE_SIZE; addr++)
    {
//        lcd_Clear(BLACK); // Clear the screen for each iteration

        testValue = addr % 256; // Set test value (mod 256 for 8-bit range)

        // Show debug info
        char debug_str[50];

        printf("== ADDRESS %d ==", addr);
//		lcd_ShowStr(10, 50, debug_str, WHITE, BLACK, 16, 1);

        // Write the test value
        at24c_WriteOneByte(addr, testValue);
        printf("%d Write: %d", addr, testValue);
//        lcd_ShowStr(10, 70, debug_str, WHITE, BLACK, 16, 1);

        // Read back the value
        readValue = at24c_ReadOneByte(addr);
        printf("%d Read: %d", addr, readValue);
//        lcd_ShowStr(10, 90, debug_str, WHITE, BLACK, 16, 1);

        // Verify the value
        if (readValue != testValue)
        {
            return 1; // Error
        }
//        HAL_Delay(100);
    }

    return 0; // Success
}

uint8_t at24c_ReadOneByte(uint16_t ReadAddr)
{
    uint8_t temp = 0;
    HAL_I2C_Mem_Read(&hi2c2, 0xA0, ReadAddr, I2C_MEMADD_SIZE_16BIT, &temp, 1, 10);
    return temp;
}

void at24c_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
	if (HAL_I2C_Mem_Write(&hi2c2, 0xA0, WriteAddr, I2C_MEMADD_SIZE_16BIT, &DataToWrite, 1, 100) != HAL_OK)
	{
        printf("Write Error");
	}
    HAL_Delay(5);
}

void at24c_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead)
{
    while (NumToRead)
    {
        *pBuffer++ = at24c_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

void at24c_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite)
{
    while (NumToWrite--)
    {
        at24c_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

void at24c_init(void)
{
    at24c_Check();
}

uint8_t at24c_Check(void)
{
    uint8_t temp;
    temp = at24c_ReadOneByte(EE_SIZE);
    if (temp == 0x55) return 0;
    else
    {
        at24c_WriteOneByte(EE_SIZE, 0x55);
        temp = at24c_ReadOneByte(EE_SIZE);
        if (temp == 0x55) return 0;
    }
    return 1;
}

