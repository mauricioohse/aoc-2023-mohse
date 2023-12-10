// do not read this code, it sucks REALLY bad. I was stuck with this problem and wanted to push through it fast.
// I swear I dont code like this :p

#include <iostream>
#include <cstring>
#include <cctype>
#include <math.h>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <climits>

using namespace std;

#define FIRST_LINE_LENGTH 250
#define LINE_SIZE 100

enum {
    TO_VALUE,
    FROM_VALUE,
    RANGE
};

enum MAP_TYPES{
    SEED_TO_SOIL,
    SOIL_TO_FERT,
    FERT_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMP,
    TEMP_TO_HUMID,
    HUMID_TO_LOC
};

enum {
    FROM_SEEDS,
    RANGE_SEEDS
};

enum SMALLEST_MAPS_ENUM {
    SMALLEST_TO_VALUE,
    SMALLEST_FROM_VALUE,
    SMALLEST_RANGE,
    SMALLEST_IDX,
    SMALLEST_ENUM_MAX_VALUE
};

#define N_MAPS 7
#define N_TRANSFORMS 50
#define N_COL 3
#define N_SEEDS 20
#define cout if(false) cout
// each map has:
// from data: fromValue, toValue, range
// i can make a function
// if (seed > fromValue && seed < fromValue + range)
//      seed = seed + (toValue - fromValue);
// so each map can be described in 3 collumns
// map[3], 0: fromValue, 1:toValue, 2:range
// but then i would need to compare the seed to each map[][], which would be really inneficient...
// Can I do a more efficient and smart way to transform these numbers?
// ha! i probably can. The catch is that I only need the lowest number possible. So maybe I can work backwards
// from the last map, see what values account for the lowest numbers.
// I will implement that in part 2 if needed. but the idea would be to see the smallest range on the last map,
// and go back through and see if any of the seeds contain a value that can map to there.

void printSeedsArray( long long int* arr)
{
    int i = 0;
    while( i !=N_SEEDS )
        {
            cout << arr[i] << " ";
            i++;
        }
        cout << endl;
}

long long int seeds[N_SEEDS] = {LLONG_MAX};
long long int seeds_mapped[N_MAPS + 1][N_SEEDS] = {LLONG_MAX};
long long int seeds_range[N_SEEDS / 2][2];
int last_seed_idx = 0;
long long int maps[N_MAPS][N_TRANSFORMS][N_COL] = {LLONG_MAX};
long long int smallest_maps[N_MAPS][SMALLEST_ENUM_MAX_VALUE]; // See enum
FILE* f;
long long int part1_minValue = LLONG_MAX;


// part two - bucket solution. I will pick N_PICK seeds from every range, and than transform them
// after finding the best seed, I will search near that seed.
#define N_PICK 10 //1000000
long long int seeds_picked[N_SEEDS*N_PICK]; // seeds "bucket" picked to be evaluated
long long int seeds_picked_mapped[N_MAPS+1][N_SEEDS*N_PICK];
long long int part2_min = LLONG_MAX;
int part2_seed_idx = 0;

void InitGlobal()
{
    f = fopen("input.txt", "r");
    fill_n(seeds, sizeof(seeds), -1);

    for (int i = 0; i < N_MAPS; i++) {
        for (int j = 0; j < N_COL; j++) {
            for (int k = 0; k < N_TRANSFORMS; k++) {
                maps[i][k][j] = -1;
            }
        }
    }

    std::fill(&seeds_mapped[0][0], &seeds_mapped[0][0] + (N_MAPS + 1) * N_SEEDS, -1);
    fill(&seeds_range[0][0], &seeds_range[0][0] + (N_MAPS/2)*2,-1);
    fill(&smallest_maps[0][0],&smallest_maps[0][0] + N_MAPS*SMALLEST_ENUM_MAX_VALUE,LLONG_MAX);
    fill(&seeds_picked_mapped[0][0],&seeds_picked_mapped[0][0] + (N_MAPS+1)*N_SEEDS*N_PICK,-1);
    fill(&seeds_picked[0] ,&seeds_picked[0]  + N_PICK * N_SEEDS, -1);
    int test = sizeof(maps);
}

int main()
{
    InitGlobal();

    // gets seeds input
    {
        char firstLine[FIRST_LINE_LENGTH] = {0};
        fgets(firstLine, FIRST_LINE_LENGTH, f);
        char * token = strtok(firstLine," "); // reads "seeds:", can be discardad

        int i = 0; // seeds index
        // parses first line, that gets seeds
        while(1)
        {
            token = strtok(nullptr, " ");
            if (!token)
                break;
            if (isdigit(*token))
                {   // token is a number.
                     long long int value = atoll(token);
                    if (value != 0)
                        {
                            seeds[i] = value;
                            i++;
                        }
                }
        }   // seeds[] value extracted
        last_seed_idx = i - 1;
    // std::copy(std::begin(seeds), std::end(seeds), std::ostream_iterator<int>(std::cout, " "));
    cout <<" seeds: ";
    printSeedsArray(seeds);
    cout<< endl;
    }

    // build maps
    {
        char line[LINE_SIZE];
        fgets(line, LINE_SIZE, f); // ignores first empty line
        fgets(line, LINE_SIZE, f); // ignores first empty line

        int currentMap = 0;
        int currentLineInMap = 0;

        while (fgets(line, LINE_SIZE, f))
        {
            if (line[0] == '\n')
            {                              // empty line found - start next map
                fgets(line, LINE_SIZE, f); // ignores map name line
                currentMap++;
                currentLineInMap = 0;
            }
            else
            {
                // add read values to map accordingly. each line has 3 values only
                char *token = strtok(line, " ");
                maps[currentMap][currentLineInMap][TO_VALUE] = atoll(token);
                token = strtok(nullptr, " ");
                maps[currentMap][currentLineInMap][FROM_VALUE] = atoll(token);
                token = strtok(nullptr, " ");
                maps[currentMap][currentLineInMap][RANGE] = atoll(token);

                currentLineInMap++;
            }
            memset(line,0,LINE_SIZE);
        }// end of line loop

        // maps are done!
        for (int i = 0; i < N_MAPS; ++i) {
            printf("Map %d \n", i);
            for (int j = 0; j < N_TRANSFORMS; ++j) {
                for (int k = 0; k < N_COL; ++k) {
                    printf("%lld ", maps[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    copy(seeds, seeds + N_SEEDS, seeds_mapped[0]);




    // transform each seed by each map - part 1 & 2
    {
        for (int i = 0; i <= last_seed_idx; i++)
        {
            cout << "Seed :" << i << " value :"  << seeds_mapped[0][i]  << endl;
            for (int m = 0; m < N_MAPS; m++)
            {
                for (int t =0; t < N_TRANSFORMS; t++)
                {
                     long long int fromValue = maps[m][t][FROM_VALUE];
                     long long int toValue = maps[m][t][TO_VALUE];
                     long long int range = maps[m][t][RANGE];
                     long long int currSeed = seeds_mapped[m][i];
                    MAP_TYPES type = static_cast<MAP_TYPES>(m);

                        if (currSeed >= fromValue && currSeed <= fromValue + range)
                        {
                            seeds_mapped[m+1][i] = currSeed + (toValue - fromValue);
                            break;
                        }
                        else
                        {
                            seeds_mapped[m+1][i] = currSeed;
                        }

                }
                cout << "m:"<<m+1<<" "<<seeds_mapped[m + 1][i] << " ";
            }
            if (seeds_mapped[7][i] < part1_minValue && seeds_mapped[7][i] >= 0)
                part1_minValue = seeds_mapped[7][i];
            cout << endl;
        }
    }

    cout << endl << "All resulting seeds: " << endl; 
    for (int i = 0 ; i < 20 ; i++)
    {   // 
        cout << " " << seeds_mapped[7][i];
        if (seeds_mapped[7][i] < part1_minValue && seeds_mapped[7][i] >= 0)
                part1_minValue = seeds_mapped[7][i];
    
    }

    cout << endl << "Resulting seeds minus minimum seed" << endl;
    for  (int i = 0; i<20;i++)
    {
        cout << seeds_mapped[7][i] - part1_minValue << " ";
    }


    // part 2 - smallest maps solution - WIP
    {
        // loop to search for smallest maps
        // start from last map, see smallest range. Then trace back from that until the first range. 
        // then see if any seeds is in that range.
        // gets lowest humidity range:
        {
            for (int i = N_TRANSFORMS-1; i >=0;i--)
            {
                if(maps[6][i][TO_VALUE] < smallest_maps[6][SMALLEST_TO_VALUE] || smallest_maps[6][SMALLEST_TO_VALUE] == -1)
                {
                    smallest_maps[6][SMALLEST_TO_VALUE] = maps[6][i][TO_VALUE];
                    smallest_maps[6][SMALLEST_FROM_VALUE] = maps[6][i][FROM_VALUE];
                    smallest_maps[6][SMALLEST_RANGE] = maps[6][i][RANGE];
                    smallest_maps[6][SMALLEST_IDX] = i;
                }
            }
        }

        cout << endl << " Lowest humidity range: <" << smallest_maps[6][SMALLEST_FROM_VALUE] 
        <<"-" <<smallest_maps[6][SMALLEST_FROM_VALUE] + smallest_maps[6][SMALLEST_RANGE] <<"> " << endl;

        cout << "Lowst humidity toValue: " << smallest_maps[6][SMALLEST_TO_VALUE]  << endl;
    }
    // above code is WIP 

    // part 2 - bucket solution
    { 
        // pick seed bucket
        cout << "seeds_picked:" << endl;
        int idx_picked = 0;
        for (int i = 0; i < N_SEEDS; i += 2)
        {
            long long int start_seed = seeds[i];
            long long int seed_range = seeds[i + 1];
            long long int picked_range_avg_distance = seed_range/N_PICK;
            for (int j = 0; j< N_PICK; j++)
            {
                seeds_picked[idx_picked]= start_seed + j*picked_range_avg_distance;
                cout << seeds_picked[idx_picked] << " ";
                idx_picked++;
            }
        }
        cout << endl;
    
    
        // bucket of seeds constructed. Now, map each one.
        copy(seeds_picked, seeds_picked + N_SEEDS*N_PICK, seeds_picked_mapped[0]);
    
        // now fill seeds_picked_mapped by looping into map

        //for (int s = 0; s < N_SEEDS * N_PICK ; s++) // each picked_seed
        for (int s = N_PICK; s < N_PICK *2; s++)
        {
            cout << s <<" seed: " <<  seeds_picked_mapped[0][s] << endl;
            for (int m = 0; m < N_MAPS; m++)
            {
                
                for (int t = 0; t < N_TRANSFORMS; t++)
                {

                    long long int currSeed = seeds_picked_mapped[m][s];
                    long long int fromValue = maps[m][t][FROM_VALUE];
                    long long int toValue = maps[m][t][TO_VALUE];
                    long long int range = maps[m][t][RANGE];

                    if (currSeed >= fromValue && currSeed <= fromValue + range)
                    {
                        seeds_picked_mapped[m + 1][s] = currSeed + (-fromValue + toValue);
                        break;
                    }
                    else
                    { // pass value if not mapped
                        seeds_picked_mapped[m + 1][s] = currSeed;
                    }
                }
                cout << " " << seeds_picked_mapped[m + 1][s];

            } 
            
            // part 2 min calc
            if (seeds_picked_mapped[7][s] < part2_min && seeds_picked_mapped[7][s] > 0)
            {
                part2_min = seeds_picked_mapped[7][s];
                part2_seed_idx = s;
            }
            cout << endl;
        } // now I know that seed 8 was the one that gave the lowest value
    }

    // doing very specific shit for part 2 - only seeind second seed range <1736484943 , range: 907429186 >
    {
        long long int part2_specific = LLONG_MAX;
        long long int seed_min = 1886895233ll; //seeds[2];
        long long int max_seed = 1980497843ll; // seed_min + seeds[3];

        printf("\n looking for specific seeds, on range < %lld, %lld>", seed_min, max_seed);
        
        for (long long int s = seed_min; s <= max_seed; s++)
        {
            long long int seed_buffer = s; 
            for(int m = 0 ; m < N_MAPS ; m++)
            {
                for (int t = 0 ; t < N_TRANSFORMS ; t++)
                {
                    long long int currSeed = seed_buffer;
                    long long int fromValue = maps[m][t][FROM_VALUE];
                    long long int toValue = maps[m][t][TO_VALUE];
                    long long int range = maps[m][t][RANGE];

                    if (currSeed >= fromValue && currSeed <= fromValue + range)
                    {
                        seed_buffer = currSeed + (-fromValue + toValue);
                        break;
                    }
                }
            }
            if ( seed_buffer < part2_min)
            {
                part2_min = seed_buffer;
                printf("\n part2 min : %lld starting seed %lld ", seed_buffer, s);
            }

            if ( s %1000000 == 0)
                {
                printf("\n seed %lld analysed! Result: %ld" , s, seed_buffer);

                }
        }
    }


    

    cout << endl <<  "Part 1 - min value: " << part1_minValue << endl;
    cout << endl <<  "Part 2 - min value: " << part2_min <<  "  idx:" << part2_seed_idx <<endl;
    printf("\n part 2 min value %lld idx %d", part2_min, part2_seed_idx);
    return 0;
}