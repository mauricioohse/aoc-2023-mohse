#include <iostream>
#include <cstring>

using namespace std;

char* findNextNewline(char* str) {
    return strchr(str, '\n');
}

const int padding = 1;

enum {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

// global vars related to fullText inputs
int lineLength;
FILE* f;
long fileSize;
char *fullText;     // holder for part 1
char *fullText_end;
char *fullText2;    // for part 2
char *fullText2_end;
int maxDisplacement;
int idx =0;

void ReadAOCinputAsFullArray(const char textInput[100])
{
    f = fopen("input.txt","r");
    char line[300];

    fseek(f, 0, SEEK_END);
    fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate memory for fullText
    fullText = new char[fileSize + 1]; // +1 for null terminator
    fullText2 = new char[fileSize + 1];

    while (fgets(line, sizeof(line), f))
    {
        strncpy(fullText + idx, line, strlen(line));
        idx += strlen(line);
    }
    fullText[idx] = '\0';
    fullText_end = fullText + idx;
    // now fullText is the whole written text

    // discover fullText row length
    char *newlinepos = findNextNewline(fullText);
    lineLength = newlinepos - fullText +1;
    maxDisplacement = idx;


    cout << fullText <<endl;
    cout <<" fullText strlen " << strlen(fullText) << " lineLength "<<lineLength <<endl;

    int textSize = strlen(fullText) + 1;

    // for part 2, I make a copy of the fullText since the popNextNumber method is destructive.
    strncpy(fullText2, fullText, fileSize + 1);
    fullText2_end = fullText2 + textSize;

    cout << "is fullText 2 equal to fullText? " << (strcmp(fullText2, fullText) == 0) << endl;
}

bool isSymbol(char c)
{
    return (!isdigit(c) && c != '\0' && c != '.' && c != '\n');
}

bool isStar(char c)
{
    return (c == '*');
}


bool isAnySymbolAdjacent(char *ptr)
{
    bool b = false;
    const int dir[] = {-1,0,1};

    for (int vertical = 0; vertical < 3; vertical++)
        for (int horizontal = 0; horizontal < 3; horizontal++)
        {
            int displacement = lineLength * dir[vertical] + dir[horizontal];
            if (ptr + displacement > fullText && ptr + displacement < fullText + maxDisplacement)
                b += isSymbol(*(ptr + displacement ));
        }

    return b;
}

int PopNextNumber(char *ptr, char * beg, char * end)
{                               // beg and end are the limits of ptr, to ensure I dont read from garbage memmory
    int sum = 0;
    int idx = 0;

    // first, go back if there are digits back:
    while(isdigit(ptr[idx]) && ptr + idx > beg ) // second condition sanity check for not checking before first position
        idx--;

    idx++; // one back to get to the first digit

    while (isdigit(ptr[idx]) && ptr + idx < end)
    {
        sum = sum*10 + ptr[idx] - '0';
        ptr[idx] = '.'; // w for was number
        idx++;
    }

    return sum;
}

// a gear is a symbol that has exactly 2 adjacent numbers. If it is a gear, also returns the multiplication of this 2 numbers.
bool isGear(char * ptr, int &gearValue)
{
    int mult = 1;
    bool b = false;
    const int dir[] = {-1, 0, 1};
    int num_of_adjacent_digits = 0;

    if (isStar(*ptr)) // can only be a gear if the ptr points to a star
        for (int vertical = 0; vertical < 3; vertical++)            // parses vertically in the 2d-array
            for (int horizontal = 0; horizontal < 3; horizontal++)  // parser horizontally
            {
                int displacement = lineLength * dir[vertical] + dir[horizontal]; // ptr + displacement is abstractically the same as ptr[x][y]
                char *p_currChar = (ptr + displacement);
                if (p_currChar > fullText2 && p_currChar < fullText2 + maxDisplacement) // check valid displacement

                    if (isdigit(*p_currChar))
                    {
                        int value = PopNextNumber(p_currChar, fullText2, fullText2_end);
                        mult *= value;
                        num_of_adjacent_digits++;
                    }
            }

    if (num_of_adjacent_digits==2)
        b = true;
        gearValue = mult;
    return b;
}

int totalSum = 0;
int totalGearValue = 0;

int main() {

    // put the text into fullText holders and set related global variables.
    ReadAOCinputAsFullArray("input.txt");

    // imagine fullText[y][x]
    for (int y = 0; y < fileSize / (lineLength); y++)
    {
        for (int x = 0; x < lineLength ; x++)
        {
            char * ptr = fullText + x + y*(lineLength); // current part 1 char pointer

            // part 1
            if (isAnySymbolAdjacent(ptr)) //isSymbol(*ptr))
            {
                if(isdigit(*ptr))
                {
                    int value = PopNextNumber(ptr, fullText, fullText_end);
                    totalSum += value;
                }
                cout << "S"; // S denotes there is an adjacent symbol
            }
            else
            {
                cout << ".";
            }

            // part 2
            int multiplier = 0;
            char * ptr2 = fullText2 + x + y*(lineLength);
            if(isGear(ptr2,multiplier))
                totalGearValue += multiplier;

        } // inside line parsing
        cout << endl;
    } // outside line parsing

    cout << endl <<"Part 1 - totalSum : " << totalSum << endl;
    cout << endl <<"Part 2 - totalGearValue : " << totalGearValue << endl;
    return 0;
}