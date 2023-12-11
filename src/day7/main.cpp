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
#define HAND_SIZE 5
#define MAX_HANDS 1000

enum strengths_values
{
    none,
    _pair,
    two_pair,
    triple,
    full_house,
    four_of_a_kind,
    five_of_a_kind
};

struct Hands
{
uint16_t bid;
uint8_t cards[5]; // 0-12 means cards 2,3,..,T, J,Q,K,A
uint8_t strength;
};

Hands hands[MAX_HANDS] = {0};
uint16_t hands_read;

// part 1 global
unsigned long long p1_total = 0; 

void GetHand(uint8_t* destHand, char* hand)
{   // expects 5 char hand, returns the destHand transformed hand 
    for (int i = 0; i< HAND_SIZE; i++)
    {
        // // part 1 from 23456789TJQKA -> 0 1 2 3 4 5 6 7 8 9 10 11 12 
        // destHand[i] = hand[i] - '0' - 2; // part 1
        
        // if (hand[i] == 'T') destHand[i] = 8;
        // if (hand[i] == 'J') destHand[i] = 9; 
        // if (hand[i] == 'Q') destHand[i] = 10;
        // if (hand[i] == 'K') destHand[i] = 11;
        // if (hand[i] == 'A') destHand[i] = 12;

        // part 2 23456789TJQKA -> 1 2 3 4 5 6 7 8 9 0 10 11 12
        destHand[i] = hand[i] - '0' - 1; 

        if (hand[i] == 'T') destHand[i] = 9;
        if (hand[i] == 'J') destHand[i] = 0;    
        if (hand[i] == 'Q') destHand[i] = 10;
        if (hand[i] == 'K') destHand[i] = 11;
        if (hand[i] == 'A') destHand[i] = 12;

    }

}

uint16_t GetStrength(uint8_t * hand)
{
    // first discover the number of duplicates
    int duplicates[13] = {0};
    for (int i = 0; i< HAND_SIZE; i++)
    {
        duplicates[hand[i]]++;
    }



    // calc strength
    int max_dup = 0;
    int triple_flag=0;
    int pair_count = 0;
    // for (int i = 0; i < 13; i++) // part 1
    for (int i = 1; i < 13; i++)    // part 2 - dont count jokers
    {
        if (duplicates[i] > max_dup)
            max_dup = duplicates[i];

        if(duplicates[i] == 2)
            pair_count++;
    }

    // issue: double pair + joker becomes full house with pair_count ==2 -> ok
    //
    // part 2 : treat jokers (card value 0) as the highest duplicate card
    if(duplicates[0] != 0)
    {   // there are jokers - assign it the highest dupl possible
        if( pair_count!=0 && max_dup == 2) pair_count--;
        max_dup = max_dup + duplicates[0];
    }

    if(max_dup==5) return five_of_a_kind;
    if(max_dup==4) return four_of_a_kind;
    if(max_dup==3 && pair_count) return full_house;
    if(max_dup==3) return triple;
    if(max_dup==2 && pair_count==2) return two_pair;
    if(max_dup==2) return _pair;
    if(max_dup==1) return none;
    
    cout << "INVALID HAND READ" << endl;
    return 0;
}

void GetHands()
{
    FILE* fp = fopen(INPUT_TXT,"r");
    #define LINE_SIZE 1024
    char line[LINE_SIZE];
    int handIdx=0;

    while(fgets(line,LINE_SIZE,fp))
    {   // first token is hand, second token is bid
        char *c_currHand= strtok(line," ");
        GetHand(hands[handIdx].cards, c_currHand);
        hands[handIdx].strength = GetStrength(hands[handIdx].cards);
        hands[handIdx].bid = atoi(strtok(nullptr," "));
        handIdx++;

    }

    hands_read = handIdx;
}

void PrintElements(uint8_t * arr, uint16_t size)
{
    for (int i = 0; i < size; i++)
    {
        cout << (int)arr[i] << " ";
    }
}

// returns true if first hand is stronger than second, 0 otherwise (0 for draw)
bool CompareStrength(Hands first, Hands second)
{
    if (first.strength > second.strength)
        return true;
    if (second.strength > first.strength)
        return false;
    
    // here: same strength
    for (int i = 0; i<HAND_SIZE; i++)
    {
        if (first.cards[i] > second.cards[i])
            return true;
        if (second.cards[i] > first.cards[i])
            return false;
    }

    // hands are exactly equal
    return false;
}

// swaps hands at index fIdx and sIdx
void SwapHands(int fIdx, int sIdx)
{
    Hands buffer = hands[sIdx];
    hands[sIdx] = hands[fIdx];
    hands[fIdx] = buffer;
}

void SortHands()
{
    // printing
    for (int i=0; i < hands_read; i++)
    {
        cout << "h: ";
        PrintElements(hands[i].cards,5); 
          cout << " b: "<< hands[i].bid << " s: " << (int)hands[i].strength << endl;
    }

    // sorting
    for (int i = 0; i< hands_read; i++)
        for (int j  = i; j > 0 && CompareStrength(hands[j-1], hands[j]); j--)
            SwapHands(j-1,j);

    // printing after sorted
    cout << endl << "sorted hands: " << endl;
    for (int i = 0; i < hands_read; i++)
    {
        cout << "h: ";
        PrintElements(hands[i].cards, 5);
        cout << " b: " << hands[i].bid << " s: " << (int)hands[i].strength
             << " rank: " << i + 1 << " value: " << hands[i].bid * (i + 1) << endl;
        p1_total += hands[i].bid * (i + 1);
    }
}

int main()
{

    GetHands();

    SortHands();


    // cout << "Part 1 - total: " << p1_total << endl;  // part 1
    cout << "Part 2 - total: " << p1_total << endl;     // part 2

    return 0;
}