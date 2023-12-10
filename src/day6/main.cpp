#include <iostream>
#include <cstring>
#include <cctype>
#include <math.h>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <climits>
#include  <time.h>


using namespace std;

#define N_RACES 4
#define INPUT_TXT "input.txt"

clock_t start_time, end_time;
double cpu_time_used;

// part 1 globals
int times[N_RACES] = {0};
int distances[N_RACES] = {0};
int n_runs = 0; // dynamic N_RACES
int times_max_idx = 0;
int distance_max_idx = 0;

// part 2 -since parsing is so easy for this case, I just manually inputed the values. Might fix it later.
long long int p2_RunTime = 48989083; 
long long int p2_Distance = 390110311121360;

FILE *f;
void GlobalInit()
{

    start_time = clock();
    f = fopen(INPUT_TXT, "r");
}

void FillTables()
{
    char line[100];
    int n_line = 0;
    while (fgets(line, 100, f))
    {
        char *tok = strtok(line, " "); // first token is trashed

        int idx = 0;
        while (tok)
        {
            tok = strtok(nullptr, " ");
            if (n_line == 0) // first line - time
            {
                times[idx] = atoi(tok);
            }
            else
            {
                distances[idx] = atoi(tok);
            }
            idx++;
        }

        n_line++;
        n_runs = idx -1;
    }

    cout << "Number of runs: " << n_runs << endl;

    for (int i = 0; i < n_runs; i++)
    {
        cout << "time: " << times[i] << "  distance: " << distances[i] << endl;
    }
}

bool CalcRaceWon(long long int timePressingButton,long long int recordDistance, long long int totalRaceTime)
{
    long long int speed = timePressingButton;
    long long int runDistance = speed*(totalRaceTime-timePressingButton);

    return (runDistance > recordDistance);
}

//
long long int CalcHowManyWins(long long int totalRaceTime, long long int recordDistance)
{
    int wins = 0;

    for (long long int timePressingButton = 1; timePressingButton< totalRaceTime; timePressingButton++)
    {
        wins+=CalcRaceWon(timePressingButton,recordDistance, totalRaceTime);
    }

    return wins;
}
//
long long int Part1_simulateRuns()
{
    long long int part1_total = 1;

    for (long long int i = 0; i < n_runs; i++)
    {
        long long int howManyWins = CalcHowManyWins(times[i],distances[i]);
         part1_total*= howManyWins;
        cout << "Race "<< i<< " won " <<  howManyWins << endl;
    }

    return part1_total;
}

long long int Part2_simulateRun(){

    return CalcHowManyWins(p2_RunTime, p2_Distance);
}



int main ()
{

    GlobalInit();

    FillTables();

    long long int part1_total = Part1_simulateRuns();

    long long int part2_total = Part2_simulateRun();


    cout <<" part1_total : " << part1_total << endl;
    cout <<" part2_total : " << part2_total << endl;

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used: %f seconds\n", cpu_time_used);
    return 0;
}