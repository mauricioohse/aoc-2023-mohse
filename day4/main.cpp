#include <iostream>
#include <cstring>
#include <cctype>
#include <math.h>

// #include "stdio.h"

using namespace std;

int part1mult = 0;
int ExtraCopies[300]={0}; // used for part 2
int cardValue[300]={0}; // used for part2
int numMatchesArr[300]={0};

int WinMain()
{
    FILE* f = fopen("input.txt","r");

    char line[200] = {0};

    int currentCardNumber = 1; // starts at 1
    while(fgets(line, 200, f))
    {
        cout << line << endl;

        int winningNumbers[200] = {0};
        int n_winningNumber = 0;
        bool winningSet= false;;

        int rolledNumbers[200] = {0};
        int n_rolledNumbers = 0;

        char *token = strtok(line," "); // is destructive2^numMatches
        token = strtok(nullptr, " "); 

        while (line)
        {
            token = strtok(nullptr," "); // will de

            if (!token) // token is null - end
                break;
            else if (isdigit(*token))
            {
                int value = atoi(token); // do logic to winningNumber and rolledNumber
                if (!winningSet)
                {
                    winningNumbers[n_winningNumber] = value;
                    n_winningNumber++;
                }
                else
                {
                    rolledNumbers[n_rolledNumbers] = value;
                    n_rolledNumbers++;
                }
            }
            else if (*token == '|')
            {
                winningSet = true;
            }

        }

        std::cout << ">>>Winning Numbers: ";
        for (int number : winningNumbers)
        {
            if(number)
            std::cout << number << " ";
        }

        std::cout << std::endl;

        // Printing rolledNumbers
        std::cout << ">>>Rolled Numbers: ";
        for (int number : rolledNumbers)
        {
            if (number)
            std::cout << number << " ";
        }
        std::cout << std::endl;

        // part 1: verify how many numbers won
        int numMatches = 0;
        for (int i = 0; i < 200; i++)
            for (int j = 0; j < 200; j++)
            {
                if (winningNumbers[i] && rolledNumbers[j] && winningNumbers[i] == rolledNumbers[j])
                    numMatches++;
            }
            if (numMatches)
            {
                cardValue[currentCardNumber] = pow(2, numMatches - 1);
                part1mult += cardValue[currentCardNumber];

                numMatchesArr[currentCardNumber] = numMatches;

                // part 2: when a card win, you increase the amount of extra value for the next numMatches cards
                // fom currentCardNumber to numMatches, add one to ExtraCopies
                for (int i = currentCardNumber +  1; i <= currentCardNumber + numMatches; i++)
                {
                    ExtraCopies[i]+=1 + ExtraCopies[currentCardNumber];
                }


            }
            cout << "numMatches = " << numMatches << " card value = " << cardValue << endl;
            currentCardNumber++; // increase current card number
            
    }
    cout << endl<<"Part 1 total: " << part1mult << endl;
    
    // calculate part 2:
    int part2total = 0;
    for (int i=0; i<212; i++)
        part2total+=ExtraCopies[i];

    cout << "Part 2 total : " << part2total +211<< endl;

    return 0;
}