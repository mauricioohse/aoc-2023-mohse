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

enum {
    RIGHT,
    LEFT
};

struct Pos
{
    char name[4];
    int posValue[3];
};

struct Map
{
    Pos pos;
    Pos left;
    Pos right;
};

// global
char dir[300] = {0};
int last_dir_idx = 0;
int last_map_idx = {0};

Map wholeMap[26][26][26] = {0};
int wholeMapIndexes[700][3] = {0};
int last_map_index = 0;
int first_A_i;
int first_A_j;
int first_A_k;

// part 2 globals
int A_indexes[500][3];
int n_As = 0;
int steps_ghost_took[500]={0};

void SetPos(Pos* pos, char* str)
{
    memcpy(pos->name, str, 3); // assumes 3 length str and that pos.name is filled with \0

    for (int i = 0; i < 3; i++)
    {
        pos->posValue[i] = str[i] - 'A';
    }
}

void TransferMapToWholeMap(Map map)
{
    int i= map.pos.posValue[0];
    int j = map.pos.posValue[1];
    int k = map.pos.posValue[2];
    memcpy(&wholeMap[i][j][k], &map,sizeof(map));
    // wholeMap[i][j][k] = map;

    for (int i = 0; i<3; i++)
        wholeMapIndexes[last_map_index][i] = map.pos.posValue[i];

    last_map_index++;

    printf("wholeMap[%d][%d][%d] pos.name %c %c %c L:%c %c %c R:%c %c %c \n",
            i,j,k, wholeMap[i][j][k].pos.name[0],wholeMap[i][j][k].pos.name[1],wholeMap[i][j][k].pos.name[2],
            wholeMap[i][j][k].left.name[0],wholeMap[i][j][k].left.name[1],wholeMap[i][j][k].left.name[2],
            wholeMap[i][j][k].right.name[0],wholeMap[i][j][k].right.name[1],wholeMap[i][j][k].right.name[2]);

    if ( wholeMap[i][j][k].pos.name[0]=='A'&& wholeMap[i][j][k].pos.name[1]=='A'&& wholeMap[i][j][k].pos.name[2]=='A' )
    {
        first_A_i = i;
        first_A_j = j;
        first_A_k = k;
    }

    // part 2 - saves every pos that ends with A
    if (wholeMap[i][j][k].pos.name[2] == 'A')
    {
        A_indexes[n_As][0] = i;
        A_indexes[n_As][1] = j;
        A_indexes[n_As][2] = k;
        n_As++;
    }
}

void InitGlobal()
{
    FILE* fp = fopen(INPUT_TXT, "r");
    fgets(dir,300,fp); // first line is directions

    for ( int i = 0; dir[i]!='\0'; i++)
        last_dir_idx = i;
    
    char line[50]; 
    fgets(line,50,fp); // ignore blank line
    int i = 0;
    while(fgets(line,50,fp))
    {   // reads every line
        Map map = {0};
        SetPos(&map.pos, strtok(line, " "));
        strtok(nullptr, " "); // thrashes '='
        SetPos(&map.left, strtok(nullptr, " ")+1);
        SetPos(&map.right, strtok(nullptr, " "));

        TransferMapToWholeMap(map);
        i++;
    }

    cout << "Map:" << endl;
    for (int i = 0; i<last_map_index;i++)
    {
        Map map = wholeMap[wholeMapIndexes[i][0]][wholeMapIndexes[i][1]][wholeMapIndexes[i][2]];
        cout << map.pos.name << " | >" << map.left.name << " <> " << map.right.name << "<" << endl;
    }

}

void p1_RunThroughMap()
{
    bool finished = false;
    int i,j,k;
    int next_i = first_A_i; // first init
    int next_j = first_A_j;
    int next_k = first_A_k;
    int dir_idx = 0;
    int next_dir = 0;
    long long int steps = 0;
    long long int p1_n_steps;
    while (!finished)
    {
        i = next_i;
        j = next_j;
        k = next_k;

        if(dir_idx >= last_dir_idx)
        {
            dir_idx = 0;
        }
        
        next_dir = (dir[dir_idx] == 'R');
        dir_idx++;

        Map currMap = wholeMap[i][j][k];

        Map nextMap = next_dir ? wholeMap[currMap.right.posValue[0]][currMap.right.posValue[1]][currMap.right.posValue[2]] 
                               : wholeMap[currMap.left.posValue[0]][currMap.left.posValue[1]][currMap.left.posValue[2]];

        steps++;

        if(nextMap.pos.name[0] == 'Z' &&nextMap.pos.name[1] == 'Z' &&nextMap.pos.name[2] == 'Z'  )
        {
            finished = true;
            p1_n_steps = steps;
        }

        next_i = nextMap.pos.posValue[0];
        next_j = nextMap.pos.posValue[1];
        next_k = nextMap.pos.posValue[2];

    }


    cout << endl << "Part 1 - number of steps: "  << p1_n_steps<< endl;
}

long long int CalcMMC(int * arr,size_t size)
{
    int factors[1000] = {0};
    int factor_idx = 0;
    long long int mmc = 1;

    // find all factors. 
    for (int i = 0; i<size; i++ )
    {
        int currDivisor = 2;
        while(currDivisor< arr[i]/2 + 1)
        {
            if ((arr[i] % currDivisor) == 0)
            {   // found divisor
                bool newDivisor = true;
                for (int j = 0; j < factor_idx; j++)
                {   // searching if it is already a divisor
                    if (currDivisor == factors[j]) newDivisor = false;
                }
                if(newDivisor)
                {
                    factors[factor_idx] = currDivisor;
                    factor_idx++;
                }
            }
            currDivisor++;
        }
    }

    // multiply all divisors to get mmc
    for (int k = 0; k < factor_idx ; k++)
    {
        mmc *= factors[k];
    }

    return mmc;
}


void p2_GhostsThroughMap()
{
    int n_finished = 0;
    int i,j,k;
    int dir_idx = 0;
    int next_dir = 0;
    long long int steps = 0;
    long long int p2_n_steps;
    int n_ghosts_cycled = 0;
    while (n_finished!=n_As)
    {
        n_finished = 0;

        if(dir_idx >= last_dir_idx)
        {
            dir_idx = 0;
        }
        
        next_dir = (dir[dir_idx] == 'R');
        dir_idx++;


        for (int a_idx = 0; a_idx < n_As; a_idx++)
        {
            i = A_indexes[a_idx][0];
            j = A_indexes[a_idx][1];
            k = A_indexes[a_idx][2];
            
            Map currMap = wholeMap[i][j][k]; 

            Map nextMap = next_dir ? wholeMap[currMap.right.posValue[0]][currMap.right.posValue[1]][currMap.right.posValue[2]]
                                   : wholeMap[currMap.left.posValue[0]][currMap.left.posValue[1]][currMap.left.posValue[2]];

            if ( nextMap.pos.name[2] == 'Z')
            {
                n_finished++;

                if (!steps_ghost_took[a_idx])
                {
                    steps_ghost_took[a_idx] = steps+1;
                    cout << "a_idx " << a_idx << " took " << steps_ghost_took[a_idx] << " steps!" << endl;
                    n_ghosts_cycled++;
                    if (n_ghosts_cycled == n_As )
                    {
                        n_finished = n_As;
                    }
                }
            }

            A_indexes[a_idx][0] = nextMap.pos.posValue[0];
            A_indexes[a_idx][1] = nextMap.pos.posValue[1];
            A_indexes[a_idx][2] = nextMap.pos.posValue[2];
        }
        steps++;

    }
    p2_n_steps = steps;

    cout << endl
         << "Part 2 - MMC of ghost cycles: " << CalcMMC(steps_ghost_took,n_As) << endl;
}


int main()
{
    InitGlobal();

    p1_RunThroughMap();

    p2_GhostsThroughMap();

    return 0;
}
