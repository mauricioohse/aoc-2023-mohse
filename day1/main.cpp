#include <iostream>
#include "input.h"
#include <string.h>

using namespace std;

// global variables
int sum_part1 = 0;
int sum_part2 = 0;

const char table[][10] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"};

void SumOutmostNumbersInEachLine(char text[][99], size_t x_max)
{
    char zeroASCII = '0';
    char firstNumber = 0;
    char lastNumber = 0;
    char firstNumberInText = 0;
    char lastNumberInText = 0;

    // text[x][y]
    for (int x = 0; x < x_max; x++)
    {
        for (int y = 0; y < 99; y++)
        {
            char c = text[x][y];

            if (c == '\0')
            {
                break;
            }
            else if (c >= '0' && c <= '9')
            {
                if (!firstNumber)
                {
                    firstNumber = c;
                }

                lastNumber = c;

                if (!firstNumberInText)
                {
                    firstNumberInText = c;
                }
                lastNumberInText = c;
            }
            else
            {   // checking for numbers written in text 
                for (int tableIndex = 0; tableIndex < 10; tableIndex++)
                {
                    if (strncmp(text[x] + y, table[tableIndex], strlen(table[tableIndex])) == 0)
                    {
                        if (!firstNumberInText)
                        {
                            firstNumberInText = tableIndex + '0';
                        }

                        lastNumberInText = tableIndex + '0';
                    }
                }
            }
        }
        int numToSum = (lastNumber - zeroASCII) + (firstNumber - zeroASCII) * 10;
        sum_part1 += numToSum;

        int numToSum2 = (lastNumberInText - zeroASCII) + (firstNumberInText - zeroASCII) * 10;
        sum_part2 += numToSum2;

        firstNumber = 0;
        lastNumber = 0;
        firstNumberInText = 0;
        lastNumberInText = 0;
    }
}

int main()
{

    SumOutmostNumbersInEachLine(text, sizeof(text) / 99);

    cout << endl<< "Sum part 1 : " << sum_part1 << endl;
    cout <<  "Sum part 2 : " << sum_part2 << endl;

    return 0;
}