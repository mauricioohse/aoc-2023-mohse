#include <iostream>
#include <cstring>
#include <stdint.h>

using namespace std;

const char colors[][10]= {
    "red",
    "green",
    "blue"
};

const int limits[]={
    12,
    13,
    14
};

int TotalPowerSum = 0;
int maxBalls[3] = {0};

// Verify if token is one of the colors. If it is, check for part 1 and part 2 requirements.
bool DealWithColorTokens(char *token, int lastNumberRead)
{
    bool rtn = true;

    // search match in color table
    for (int i = 0; i < 3; i++)
    {
        if(!strncmp(token, colors[i], strlen(colors[i])))
            {
                // part 1: check if ball is inside limit
                if(lastNumberRead > limits[i])
                {
                    // cout << " invalid game: reason lastNumberRead " << lastNumberRead << " limits[i] " << limits[i] << endl;
                    rtn = false; // invalid value
                }

                // part 2: fills maxBalls with each maximum value found so far.
                if( lastNumberRead > maxBalls [i])
                {
                    // cout <<  colors[i] << lastNumberRead << "  ";
                    maxBalls[i] = lastNumberRead;
                }
            }
    }

    return rtn; 

}

bool TokenIsInteger(const char* string)
{
    int idx = 0;
    while(string[idx])
    {
        if (!isdigit(string[idx]))
            return false;
        
        idx++;
    }

    return true;
}

int main()
{

    FILE* f = fopen("realInput.txt","r");
    char line[300];
    int gameCnt = 1;
    int possibleGameSum = 0;

    while (fgets(line, sizeof(line), f))
    {   // note: each line ends with '\n' except the last one, probably
        bool validGame = true;
        cout << line;
        int idx = 0;

        int lastNumberRead = 0;

        char *token = strtok(line, " "); // note that strtok fills line with '\0' where token belonged. It is destructive for line.
        while(token)
        {

            if (TokenIsInteger(token))
            {   // verify if everything is digit
                sscanf(token, "%d", &lastNumberRead);
                // cout << " lastNumberRead : " << lastNumberRead;
            }

            if (!DealWithColorTokens(token, lastNumberRead))
            {
                validGame = false;
            }

            // for part 1 only, this is a small optimization:
            // if(!validGame)
            //     break;

            token = strtok(NULL, " "); // gets next token from line
        } // all tokens parsed and dealt with.
        
        // part 1 metric
        if (validGame)
        {
            possibleGameSum += gameCnt;
            cout << endl <<">>>> game cnt "<< gameCnt << " possibleGameSum " << possibleGameSum<< endl;
        }

        // part 2 metric 
        int linePowerSum = maxBalls[0] * maxBalls[1] * maxBalls[2];
        TotalPowerSum += linePowerSum;
        printf(">>>> TotalPowerSum %d+= %d ( %d * %d* %d ) \n",
               TotalPowerSum, linePowerSum, maxBalls[0], maxBalls[1], maxBalls[2]);

        // end of line. increase game count and reset line-dependents variables
        memset(maxBalls, 0, sizeof(maxBalls));
        memset(line, 0, sizeof(line));
        gameCnt++;
    } // all lines parsed

    cout << "\n Part 1 - sum of possible games indexes: " << possibleGameSum;  
    cout << "\n Part 2 - sum of maxBalls: " << TotalPowerSum;  
    return 0;
}