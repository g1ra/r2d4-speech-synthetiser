#include <stdio.h>
#include <string.h>
#define byte uint8_t

struct r2sound {
  byte f0;
  byte f1;
  byte fI;
  byte d0;
  byte d1;
  byte dI;
};

// i: 5||f0: 6700 -f1: 9300 -freqInc: 1.08 -d0: 21 -d1: 5 -durInc: 1.77
// i: 6||f0: 3000 -f1: 7400 -freqInc: 1.22 -d0: 9 -d1: 5 -durInc: 1.83
// (5-8000)-41 (6-5200)-43
// senti: 5 -sentencelength: 7 -randChoice: 5 -soundnew: 5 -soundEndDelay: 19 -randChoice: 3 -pattern: -patternRep: 2 p:4 p:1*k:5 -playRep: 1 -wordEndDelay: 235
// senti: 6 -sentencelength: 7 -randChoice: 5 -soundnew: 6 -soundEndDelay: 14 -randChoice: 1 -pattern: -patternRep: 2 p:4 p:3*k:5 -playRep: 10 -wordEndDelay: 249

struct r2sentence {
	r2sound sounds[10];
  byte sq[90];
  byte sentencelength;
};

r2sentence S_gps = {{{67,93,108,21,5,177},{30,74,122,9,5,183}},{0,19,1,4,1,145,1,14,10,4,3,149},2};

r2sentence r2_SentenceGen(byte lenmin, byte lenmax, byte nfo); 
