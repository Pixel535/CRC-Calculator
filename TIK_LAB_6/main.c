#include <stdio.h>
#include <stdlib.h>

char rozszerzenie[] = ".txt";
char input_file_name[50];
char output_file_name[50];
int isCheck = 0;

void OpenFile()
{
    FILE *f_in;
    unsigned char buffer[1];
    int readCount = 0;
    int size = 0;
    char key_num[50];
    int readBytesLength = 1;
    char key[50];
    char j[1];
    gets(j);
    printf("Enter the file name [without extension]: ");
    gets(input_file_name);
    strcat(input_file_name, rozszerzenie);
    if((f_in = fopen(input_file_name, "rb")) == NULL)
    {
        printf("\n\n");
        printf("ERROR - NIEKTORE PLIKI SA NIEDOSTEPNE!!!\n\n\n");
        system("pause");
        system("cls");
    }
    else
    {
        f_in = fopen(input_file_name,"rb");
        while(readCount = fread(buffer, sizeof(unsigned char), readBytesLength, f_in))
        {
            size++;
        }
        char TablicaZnakow[size];
        fclose(f_in);
        printf("Enter the key: ");
        gets(key_num);
        int idx = 0;
        int indeks = 0;
        while(key_num[idx] != '1')
        {
            idx++;
        }
        while(key_num[idx] != NULL)
        {
            key[indeks] = key_num[idx];
            idx++;
            indeks++;
        }
        GenerateCRC(TablicaZnakow, size, key);
        fclose(f_in);
    }
}

void GenerateCRC(char TablicaZnakow[], int size, char key[])
{
    FILE *f_in;
    unsigned char buffer[1];
    int readCount = 0;
    int readBytesLength = 1;
    int key_length = 0;
    f_in = fopen(input_file_name,"rb");
    int i = 0;

    printf("\nInput file in ASCII: \n");
    while(readCount = fread(buffer, sizeof(unsigned char), readBytesLength, f_in))
    {
        TablicaZnakow[i] = buffer[0];
        printf("%c", TablicaZnakow[i]);
        i++;
    }
    printf("\n");
    fclose(f_in);
    int binary_tab_size = size*8;
    char BinaryTable[binary_tab_size];
    int index = 0;
    printf("\nInput file in Binary: \n");
    for(int j = 0; j < size; j++)
    {
        for (int i = 7; i >= 0; --i)
        {
            char temp = ((TablicaZnakow[j] & (1 << i)) ? '1' : '0' );
            BinaryTable[index] = temp;
            putchar((TablicaZnakow[j] & (1 << i)) ? '1' : '0' );
            index++;
        }
    }
    printf("\n\n");

    while(key[key_length] == '1' || key[key_length] == '0')
    {
        key_length++;
    }
    int remainder_size = key_length-1;
    char CRC[key_length];
    Divide(BinaryTable, CRC, key_length, binary_tab_size, remainder_size, key);
    printf("Remainder = ");
    int idx = 0;
    for(int i = 0; i < remainder_size; i++)
    {
        printf("%c", CRC[i]);
    }
    if(remainder_size == 0)
    {
        printf("CRC is 0, because key length is 1 or key has only zeros!!!");
    }
    if(isCheck == 0)
    {
        int wholeSize = binary_tab_size + remainder_size;
        char CRCArr[wholeSize];
        for(int i = 0; i < binary_tab_size; i++)
        {
            CRCArr[i] = BinaryTable[i];
        }
        for(int i = binary_tab_size; i < wholeSize; i++)
        {
            CRCArr[i] = CRC[idx];
            idx++;
        }
        printf("\n\n");
        printf("Data: \n");
        for(int i = 0; i < wholeSize; i++)
        {
            printf("%c", CRCArr[i]);
        }
        printf("\n\n\n");
        WriteToFile(wholeSize, CRCArr);
    }
    else if(isCheck == 1)
    {
        printf("\n\n");
        int isCorrect = 0;
        if(remainder_size == 0)
        {
            printf("CRC is 0, because key length is 1 or key has only zeros!!!\n\n");
        }
        else
        {
            for(int i = 0; i < remainder_size; i++)
            {
                if(CRC[i] == '0')
                {
                    isCorrect++;
                }
            }
            if(isCorrect == remainder_size)
            {
                printf("CRC is correct\n\n");
            }
            else if(isCorrect != remainder_size)
            {
                printf("File is corrupted\n\n");
            }
        }
    }
    system("pause");
    system("cls");
}

void WriteToFile(int wholeSize, char CRCArr[])
{
    FILE *f_out;
    printf("Enter the output file name [without extension]: ");
    gets(output_file_name);
    strcat(output_file_name, rozszerzenie);
    f_out = fopen(output_file_name,"wb");
    unsigned char buffer[1];
    unsigned char bitCounter = 0;
    unsigned char current = 0;
    for(int i = 0; i < wholeSize; i++)
    {
        if(CRCArr[i] == 49)
        {
            current = current << 1|1;
            bitCounter++;
        }
        else if(CRCArr[i] == 48)
        {
            current = current << 1;
            bitCounter++;
        }
        if (bitCounter == 8)
        {
            buffer[0] = current;
            fwrite(buffer,1,1,f_out);
            current = 0;
            bitCounter = 0;
        }
    }
    if(bitCounter != 0)
    {
        while(bitCounter != 8)
        {
            current = current << 1;
            bitCounter++;
        }
        buffer[0] = current;
        fwrite(buffer,1,1,f_out);
    }
    fclose(f_out);
    printf("\nCRC added to the file [%s]\n\n", output_file_name);
}

void Divide(char BinaryTable[], char CRC[], int key_length, int binary_tab_size, int remainder_size, char key[])
{
    char zero_arr[key_length];
    int index = 0;
    int temp = key_length;
    for(int i = 0; i < key_length; i++)
    {
        CRC[i] = BinaryTable[index];
        zero_arr[i] = '0';
        index++;
    }
    for(int i = 0; i < binary_tab_size; i++)
    {
        if(CRC[0] == '0')
        {
            XOR(CRC, zero_arr, key_length);
        }
        else if(CRC[0] == '1')
        {
            XOR(CRC, key, key_length);
        }
        if(temp < binary_tab_size)
        {
            CRC[remainder_size] = BinaryTable[temp];
            temp++;
        }
        else
        {
            CRC[remainder_size] = '0';
        }
    }
}

void XOR(char bit_1[], char bit_2[], int size)
{
    for(int i = 1; i<size; i++)
    {
        if(bit_1[i] == bit_2[i])
        {
            bit_1[i-1] = '0';
        }
        else if(bit_1[i] != bit_2[i])
        {
            bit_1[i-1] = '1';
        }
    }
}

void CalculateCRC()
{
    isCheck = 0;
    OpenFile();
}

void CheckCRC()
{
    isCheck = 1;
    OpenFile();
}

int main()
{
    int komenda;
	while(1)
    {
        printf("CRC\n");
        printf("----------------------------------------------\n");
        printf("[0] Wyjscie z programu\n");
        printf("[1] Calculate CRC\n");
        printf("[2] Check CRC\n");
        printf("----------------------------------------------\n\n");
		printf("\nWybrana komenda: ");
		scanf("%d", &komenda);
		printf("\n");
		switch (komenda)
		{
			case 0: return 0; break;
			case 1: CalculateCRC(); break;
			case 2: CheckCRC(); break;
		}
    }
    return 0;
}
