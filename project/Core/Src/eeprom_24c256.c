/*
 * eeprom_24c256.c
 *
 *  Created on: Jan 9, 2025
 *      Author: clong
 */

#include "eeprom_24c256.h"

uint8_t at24c_Check(void)
{
	uint32_t addr;
    uint8_t testValue;  // Test value to write
    uint8_t readValue;

    for (addr = 0; addr <= EE_SIZE; addr+=1000)
    {
//        lcd_Clear(BLACK); // Clear the screen for each iteration

        testValue = addr % 256; // Set test value (mod 256 for 8-bit range)

        // Show debug info
        char debug_str[50];

        printf("\n\n== ADDRESS %d ==", addr);
//		lcd_ShowStr(10, 50, debug_str, WHITE, BLACK, 16, 1);

        // Write the test value
        at24c_WriteOneByte(addr, testValue);
        printf("\n%d Write: %d", addr, testValue);
//        lcd_ShowStr(10, 70, debug_str, WHITE, BLACK, 16, 1);

        // Read back the value
        readValue = at24c_ReadOneByte(addr);
        printf("\n%d Read: %d", addr, readValue);
//        lcd_ShowStr(10, 90, debug_str, WHITE, BLACK, 16, 1);

        // Verify the value
        if (readValue != testValue)
        {
            return 1; // Error
        }
//        HAL_Delay(1000);
    }

    addr=1;
    char characterToWrite = 'A';
    at24c_WriteOneByte(addr, (uint8_t)characterToWrite);

    char characterRead;
    characterRead = (char)at24c_ReadOneByte(addr);
    printf("\n\nRead character: %c", characterRead);
    printf("\nSize of character: %d bytes",sizeof(char));

    // Write a string (including its null terminator).
    addr=2;
    char myString[] = "Hello EEPROM!";
    uint16_t length = strlen(myString) + 1;  // +1 for the '\0'
    at24c_Write(addr, (uint8_t*)myString, length);

    // Read the string back
    char readBuffer[20]; // big enough to hold our string
    at24c_Read(addr, (uint8_t*)readBuffer, length);
    printf("\n\nString read from EEPROM: \"%s\"", readBuffer);
    printf("\nSize of String: %d bytes", length);



    addr = 10001;
    uint32_t big_int_write = 1234567890;
    at24c_Write(addr, (uint8_t *)&big_int_write, 4);  // Write the 4 bytes

    uint32_t big_int_read = 0;
    at24c_Read(addr, (uint8_t *)&big_int_read, 4);    // Read the 4 bytes

    // Print the 32-bit integer that was just read
    printf("\n\nRead big int: %u", big_int_read);
    printf("\nSize of big int: %d bytes",sizeof(uint32_t));

    uint16_t floatAddr = 20001; // e.g. some EEPROM address
    float float_write = 3.14159;
    float float_read  = 0.0;

    // Write the float:
    at24c_Write(floatAddr, (uint8_t*)&float_write, sizeof(float));

    // Read the float:
    at24c_Read(floatAddr, (uint8_t*)&float_read, sizeof(float));

    // Print
    printf("\n\nFloat read from EEPROM: %f", float_read);
    printf("\nSize of float: %d bytes",sizeof(float));


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


