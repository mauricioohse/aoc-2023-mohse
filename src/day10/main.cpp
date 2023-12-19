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
#include <unistd.h>

const char *resetColor = "\033[0m";
const char *redColor = "\033[1;31m";
const char *greenColor = "\033[1;32m";
const char *yellowColor = "\033[1;33m";
using namespace std;

#define INPUT_TXT "input.txt"
#define cout if(false) cout
struct Pos
{
    size_t x;
    size_t y;
    bool operator != (const Pos& other) const {
        return (x != other.x) || (y != other.y);
    }
    bool operator == (const Pos& other) const {
        return (x == other.x) && (y == other.y);
    }
};

enum DIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    WRONG
};

char ranDir[5] = {'^', 'v', '<', '>', 'X'};

struct Dir
{
    DIR dir1;
    DIR dir2;
};

char map[200][200] = {0};
Pos max_map = {0};
Pos start_pos = {0};
DIR start_DIR;

// part 2
char path[200][200] = {0}; // has char where the path was followed
char path_changes[200][200] = {0}; // has char where direction changed
char last_path_change = 0;
bool hit[200][200] = {0};  // has 1 everywhere it is on the right of the path being circled in clockwise direction


void SearchNearStart()
{
    // cout << "DIDNT FIND START!!"<< endl;
    start_DIR = WRONG;

    if (strncmp(INPUT_TXT,"test.txt", sizeof(INPUT_TXT))==0)
        start_DIR = RIGHT;

    if (strncmp(INPUT_TXT,"input.txt", sizeof(INPUT_TXT))==0)
        start_DIR = DOWN;

}

void InitGlobal()
{
    FILE *fp = fopen(INPUT_TXT, "r");
    char line[200] = {0};
    size_t n_lines = 0;

    cout << "Reading map:" << endl;
    while (fgets(line,200,fp))
    {
        bool line_ended = false;
        size_t i = 0;
        while(!line_ended)
        {
            char c = line[i];
            if (c == 'S')
            {
                start_pos.x = n_lines;
                start_pos.y = i;
            }
            if (c == '\0' || c == '\n') break;
            map[n_lines][i] = c;
            i++;
            cout << c;
        }
        cout << endl;
        max_map.y = i;
        n_lines++;
    }
    max_map.x = n_lines;

    SearchNearStart();
}

Dir GetPosDir(Pos pos)
{
    char c = map[pos.x][pos.y];
    Dir rtn;
    switch (c)
    {
    case 'L':
        rtn.dir1 = UP;
        rtn.dir2 = RIGHT;
        break;
    case '-':
        rtn.dir1 = LEFT;
        rtn.dir2 = RIGHT;
        break;
    case '7':
        rtn.dir1 = LEFT;
        rtn.dir2 = DOWN;
        break;
    case '|':
        rtn.dir1 = UP;
        rtn.dir2 = DOWN;
        break;
    case 'J':
        rtn.dir1 = UP;
        rtn.dir2 = LEFT;
        break;
    case 'F':
        rtn.dir1 = RIGHT;
        rtn.dir2 = DOWN;
        break;
    case 'S':
        // do special search near S
        rtn.dir1 = start_DIR;
        rtn.dir2 = start_DIR;
        break;
    default:
        rtn.dir1 = WRONG;
        rtn.dir2 = WRONG;
        cout << "WRONG DIR TRANSLATION!!!!" << endl;
        break;
    }
    return rtn;
}

Pos GoToDir(Pos currPos, DIR dir)
{
    // note: coord reference starts at upper left.
    Pos newPos = currPos;
    switch (dir)
    {
        case RIGHT:
            newPos.y++;
            break;
        case UP:
            newPos.x--;
            break;
        case LEFT:
            newPos.y--;
            break;
        case DOWN:
            newPos.x++;
            break;
    }
    return newPos;
}

DIR InvertDIR ( DIR dir)
{
    switch (dir)
    {
    case UP:
        return DOWN;
        break;
    case DOWN:
        return UP;
        break;
    case RIGHT:
        return LEFT;
        break;
    case LEFT:
        return RIGHT;
        break;
    }

    cout << "ERROR ON INVERT DIR" << endl;
    return WRONG;
}

void GoToNextPos(Pos* currPos, DIR* lastDIR)
{
    Dir currDir = GetPosDir(*currPos);
    Pos lastPos = *currPos;
    DIR goDIR;

    if (*lastDIR == InvertDIR(currDir.dir1))
    {
        *lastDIR = currDir.dir2;
        *currPos = GoToDir(*currPos, *lastDIR);
    }
    else if (*lastDIR == InvertDIR(currDir.dir2))
    {
        *lastDIR = currDir.dir1;
        *currPos = GoToDir(*currPos, *lastDIR);
    }
    else if (*currPos == start_pos)
    {
        *lastDIR = currDir.dir1;
        *currPos = GoToDir(*currPos, *lastDIR);
    }
    else
    {
        cout << "GoToNextPos invalid lastDIR!" << endl;
        *currPos = start_pos;
    }


    path[lastPos.x][lastPos.y] = ranDir[*lastDIR]; // for part 2
    if (last_path_change != ranDir[*lastDIR])
    {
        path_changes[lastPos.x][lastPos.y] = last_path_change;
        last_path_change = ranDir[*lastDIR];
    }
}


void P1_RunThroughMap()
{
    Pos currPos = start_pos;
    bool once = true;
    DIR lastDIR = start_DIR;
    cout << "(" << currPos.x << "," << currPos.y << ") " << endl;

    int totalSteps = 0;
    while (currPos != start_pos || once)
    {
        once = false;
        
        GoToNextPos(&currPos, &lastDIR); 
        // changes both currPos and lastDir
        cout << "(" << currPos.x << "," << currPos.y << ") " << endl;
        totalSteps++;
    }

    cout << "TotalSteps: " << totalSteps <<endl;
    cout << "divided by 2: " << totalSteps/2 <<endl;
}

// fills hit map based on path - fills all tiles to the right of the path (clockwise) with ones until it hits path again
void fill_hit_map(char c, int x, int y)
{ // note: (0,0) is the top left
    int i = x;
    int j = y;
    int step = 1;
    switch (c)
    {
    case 'v':
        step = -1;
        // no break on purpose
    case '^':
        j += step;
        while (path[x][j] == 0)
        {
            hit[x][j] = 1;
            j += step;
        }
        break;
    case '<':
        step = -1;
    case '>':
            i += step;
        while (path[i][y] == 0)
        {
            hit[i][y] = 1;
            i += step;
        }
        break;
    }
}

void P2_HitClockwiseOnMap()
{
    
    char last_c = '.';
    cout << "Printing path:" << endl;
    for (int x = 0; x < max_map.x; x++)
    {
        for (int y = 0; y < max_map.y; y++)
            {
                char c = path[x][y];
                if (!c) c = '.';
                cout << c;
                fill_hit_map(c, x, y);
                if (path_changes[x][y]!=0 ) // for corners (where direction changed during path), we also do a fill hit for both before and after turning on that tile
                {
                    fill_hit_map(path_changes[x][y], x, y);
                    last_c = c;
                }
            }

        cout << endl;
    }

    int hit_count = 0;
    cout << "Hit map:" << endl;
    for (int x = 0; x < max_map.x; x++)
    {
        for (int y = 0; y < max_map.y; y++)
            {
                char c = hit[x][y];
                if (!c) {if(path[x][y]) c = path[x][y]; else c = '.'; cout << c;}
                if (c == 1) {
                    c = 'x';
                    hit_count++;
                    cout << redColor << c << resetColor;
                }
            }

        cout << endl;
    }

    cout << "Hit count:" << hit_count << endl;
    printf("Hit Count:%d\n", hit_count);
}

clock_t start_time, end_time;
double cpu_time_used;

int main()
{
    start_time = clock();
    InitGlobal();

    P1_RunThroughMap();

    P2_HitClockwiseOnMap(); // necessary to run P1

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used: %f seconds\n", cpu_time_used);
    return 0;
}