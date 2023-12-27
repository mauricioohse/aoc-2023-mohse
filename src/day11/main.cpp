#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <fstream>

// #define cout if(false) cout 
#define INPUT_TEXT "input.txt"

const char *resetColor = "\033[0m";
const char *redColor = "\033[1;31m";
const char *greenColor = "\033[1;32m";

using namespace std;

struct Pos
{
    long long int i;
    long long int j;
};

char original_map[200][200] = {0};
int max_i, max_j;
bool void_rows[200] = {0};
bool void_cols[200] = {0};
Pos original_stars[500]={0};
int n_original_stars = 0;

void read_input()
{
    FILE *fp = fopen(INPUT_TEXT, "r");

    int j = 0, i = 0;
    char buf[200] = {0};
    while (fgets(buf,200,fp))
    {
        bool is_void_row = true;
        j = 0;
        while (buf[j] != '\n' && buf[j] != '\0')
        {
            original_map[i][j] = buf[j];
            if (buf[j]=='#')
            {
                is_void_row = false;
                original_stars[n_original_stars].i = i;
                original_stars[n_original_stars].j = j;
                n_original_stars++;
            }
            cout << buf[j];
            j++;
        }
        void_rows[i] = is_void_row;
        i++;

        cout << endl;
    }
    max_i = i;
    max_j = j;

    // run through map and get void collumns
    for (int j = 0; j < max_j; j++)
    {
        bool is_void_col = true;
        for (int i = 0; i < max_i; i++)
        {
            if(original_map[i][j]=='#')
                is_void_col = false;
        }
        void_cols[j] = is_void_col;
    }

    cout << "Printing original_map with void lines and rows colored:" << endl;
    for (int i = 0; i < max_i; i++)
    {
        for (int j = 0; j < max_j; j++)
        {
            char c = original_map[i][j];
            if(void_cols[j]) cout << greenColor << c << resetColor;
            else if(void_rows[i]) cout << redColor << c << resetColor;
            else cout << c;
        }
        cout << endl;
    }
}

char exp_map[280][280] = {0}; // expanded map
char exp_row_map[280][280] = {0}; // first step of expanding, row expanded
int delta_i=0, delta_j=0;
Pos stars[500]={0};


long long int Calc2Ddist(long long int i, long long int j, long long int x, long long int y)
{
    return (abs(i - x) + abs(j - y));
}

void P1_expand_space()
{
    int p1_void_rows[200];
    copy(begin(void_rows), end(void_rows), begin(p1_void_rows));
    int p1_void_cols[200];
    copy(begin(void_cols), end(void_cols), begin(p1_void_cols));

    cout << "expanding rows" << endl;
    for (int i = 0; i < max_i; i++)
    {
        for (int j = 0; j < max_j; j++)
        {
            exp_row_map[i + delta_i][j] = original_map[i][j];
            cout << exp_row_map[i + delta_i][j];
        }
        cout << resetColor;
        if (p1_void_rows[i])
        {
            p1_void_rows[i] = 0;
            i--;
            delta_i+=1;
            cout << endl << redColor;
        }
        else
            cout << endl;
    }

    cout << "verifing expanded rows" << endl;
    for (int i = 0; i < max_i; i++)
    {
        for (int j = 0; j < max_j; j++)
        {
            cout << exp_row_map[i][j];
        }
        cout << endl;
    }

    cout << "expanding columns" << endl;
    for (int j = 0; j < max_j ; j++)
    {
        for (int i = 0; i < max_i + delta_i; i++)
        {
            exp_map[i][j + delta_j] = exp_row_map[i][j];
            cout << exp_map[i][j + delta_j];
        }
        cout << resetColor;
        if (p1_void_cols[j])
        {
            p1_void_cols[j] = 0;
            j--;
            delta_j+=1;
            cout << endl <<greenColor;
        }
        else
            cout << endl;
    }

    cout << "print whole expanded map and get stars positions" << endl;
    int n_stars = 0;
    
    for (int i = 0; i < max_i + delta_i; i++)
    {
        for (int j = 0; j < max_j + delta_j; j++)
        {
            char c = exp_map[i][j];
            if ( c == '#') {
                stars[n_stars].i = i;
                stars[n_stars].j = j;
                c = n_stars + '1';
                n_stars++;
            }
            cout << c;
        }
        cout << endl;
    }

    // calculate the stars distance for every star
    long long int sum = 0;
    int n_sums = 0;
    for (int s = 0; s < n_stars; s++)
        for (int ss = s + 1; ss < n_stars; ss++)
        {
            Pos first_star = stars[s];
            Pos second_star = stars[ss];
            sum += Calc2Ddist(first_star.i, first_star.j, second_star.i, second_star.j);
            // cout << "star " << s + 1 << " and " << ss + 1 << 
            // " distance: " << Calc2Ddist(first_star.i, first_star.j, second_star.i, second_star.j) << endl;
            n_sums++;
        }

#define cout cout
    cout << "Sum of stars distances: " << sum << " with " << n_stars << " stars and " << n_sums << " combinations of stars" << endl;
}

void P2_expand_space_really_hard()
{ // now, the expansion is of one million times. now we wont actually map the whole space onto memory, only get the points

    const int MILLION = 999999; // 9; // 1000000;
    Pos new_stars[500];
    copy(begin(original_stars), end(original_stars), begin(new_stars));

    // increase rows
    int row_delta = 0;
    for (int i = 0; i < max_i; i++)
    {
        if (void_rows[i])
            for (int s = 0; s < n_original_stars; s++)
            {
                if (original_stars[s].i > i)
                {
                    new_stars[s].i += MILLION;
                }
            }
    }

    // increase cols
    for (int j = 0; j < max_j; j++)
    {
        if (void_cols[j])
            for (int s = 0; s < n_original_stars; s++)
            {
                if (original_stars[s].j > j)
                {
                    new_stars[s].j += MILLION;
                }
            }
    }

    // calculate the stars distance for every star
    long long int sum = 0;
    int n_sums = 0;
    for (int s = 0; s < n_original_stars; s++)
        for (int ss = s + 1; ss < n_original_stars; ss++)
        {
            Pos first_star = new_stars[s];
            Pos second_star = new_stars[ss];
            sum += Calc2Ddist(first_star.i, first_star.j, second_star.i, second_star.j);
            // cout << "star " << s + 1 << " and " << ss + 1 <<
            // " distance: " << Calc2Ddist(first_star.i, first_star.j, second_star.i, second_star.j) << endl;
            n_sums++;
        }

#define cout cout
    cout << "Sum of stars distances: " << sum << " with " << n_original_stars << " stars and " << n_sums << " combinations of stars";
}

int main()
{
    read_input();

    P1_expand_space();

    P2_expand_space_really_hard();

    return 0;
}