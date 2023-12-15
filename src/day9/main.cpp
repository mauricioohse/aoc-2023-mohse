#include <iostream>
#include <cstring>
#include <cctype>
#include <math.h>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <climits>
#include  <time.h>
#include <cstdint>


using namespace std;

#define INPUT_TXT "input.txt"
// #define INPUT_TXT "test.txt"

long long int nums[200][100] = {0}; // contains every number read from file
int n_lines = 0;
int n_numbers_on_line[200] = {0};

void InitGlobal()
{
    FILE *fp = fopen(INPUT_TXT, "r");
    char line[200] = {0};

    // read lines
    while(fgets(line,200,fp))
    {
        int currNumberIdx = 0;
        nums[n_lines][currNumberIdx] = atoll(strtok(line," "));
        cout << nums[n_lines][currNumberIdx] << " ";
        currNumberIdx++;

        char *tok;
        while((tok = strtok(nullptr," ")) != nullptr)
        {
            nums[n_lines][currNumberIdx] = atoll(tok);
            cout << nums[n_lines][currNumberIdx] << " ";
            currNumberIdx++;
        }
        n_numbers_on_line[n_lines] = currNumberIdx;
        n_lines++;

        cout << endl;

        memset(line, 0, sizeof(line));
    }


}

long long int back_num = 0;
long long int PredictNextNumber(long long int *arr, int size)
{
    long long int diffs[200][100] = {0}; // keeps all diffs
    long long int next_diff[100] = {0}; // builds next diff
    int n_diffs = 0;
    long long int sum = 0;
    long long int backward_sum = 0;

    memcpy(diffs[0], arr, size*sizeof(long long int));

    int currDiffSize = size-1;
    bool isConst = false;

    // form diff arrays
    while(!isConst)
    {
        for (int i = 0; i < currDiffSize; i++)
        {
            diffs[n_diffs+1][i] = diffs[n_diffs][i+1] - diffs[n_diffs][i];
            cout << diffs[n_diffs+1][i] << " ";
        }
        cout << endl;
        isConst = !(diffs[n_diffs + 1][0] || diffs[n_diffs + 1][1]);

        if (currDiffSize==1) {currDiffSize--; n_diffs++;
            cout << endl
                 << "***********************************" << endl;
        } // the issue is when there is little numbers

        if (isConst) cout << " Constant value reached " << endl;

        currDiffSize--;
        n_diffs++;
    }

    // predict next number

    while(currDiffSize!=(size))
    {
        sum += diffs[n_diffs][currDiffSize];
        backward_sum = -backward_sum + diffs[n_diffs][0];
        cout << "sum: " << sum << " ad backward_sum = " << backward_sum << endl;
        currDiffSize++;
        n_diffs--;
    }

    back_num += backward_sum;
    return sum;
}

void Part1()
{
    long long int p1_sum = 0;
    for (int i = 0; i < n_lines; i++)
        p1_sum += PredictNextNumber(nums[i], n_numbers_on_line[i]);

    cout << "P1 total sum: " << p1_sum << endl;
}

int main()
{
    InitGlobal();

    Part1();

    cout << "back_num: " << back_num << endl;
}