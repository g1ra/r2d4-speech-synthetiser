//#include "musical_notes.h"
#include <TrueRandom.h>
#include <r2d4.h>
#define byte uint8_t
//#include <string.h>

int speakerPin = 10; // speaker connected to digital pin 9 
int PRNT = 1;

void setup() 
{   
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT); // sets the speakerPin to be an output 
}    

int r2_soundPlay(byte f0b, byte f1b, byte fIb, byte d0, byte d1, byte dIb, byte rep, int simul)
{
  int f0 = (int)f0b*100;
  int f1 = (int)f1b*100;
  float dI = (float)dIb/100;
  float fI = (float)fIb/100;
  float freq = f0;
  float dur = d0;
  byte retD = 0;
  byte retF = 0;
  int wholedur = 0;
  if ( (f0 == f1) and (d0 == d1) ) return wholedur;
  while (1) {
  	if (rep > 0) rep--;
    if ((f0 == f1) or (fI == 1)) retF = 1;
    else retF = 0;
    if ((d0 == d1) or (dI == 1)) retD = 1;
    else retD = 0;
    if (simul == 0) beep(speakerPin,freq,dur);
    wholedur += dur;
    if (retF != 1) {
      if (f0 < f1) {
        if (freq <= f1) freq *= fI;
        else retF = 1;
      }
      else {
        if (freq >= f1) freq /= fI;
        else retF = 1;
      }
    }
    if (retD != 1) {
      if (d0 < d1) {
        if (dur <= d1) dur *= dI;
        else retD = 1;
      } else {
        if (dur >= d1) dur /= dI;
        else retD = 1;
      }
    }
    if (retD and retF and (rep <= 0)) break;
    fI += 0.05;
    dI += 0.05;
  }
  return wholedur;
}

void r2_soundPlayTyp(r2sound soundA, byte typ, byte playRep) {
	switch (typ) {
	    case 1:
	      r2_soundPlay(soundA.f0, soundA.f1, soundA.fI, soundA.d0, soundA.d1, soundA.dI, playRep, 0);
	      break;
	    case 2:
	      r2_soundPlay(soundA.f1, soundA.f0, soundA.fI, soundA.d0, soundA.d1, soundA.dI, playRep, 0);
	      break;
	    case 3:
	      r2_soundPlay(soundA.f0, soundA.f1, soundA.fI, soundA.d1, soundA.d0, soundA.dI, playRep, 0);
	      break;
	    case 4:
	      r2_soundPlay(soundA.f1, soundA.f0, soundA.fI, soundA.d1, soundA.d0, soundA.dI, playRep, 0);
	      break;
	}
}

r2sentence r2_SentenceGen(byte lenmin, byte lenmax, byte fmin, byte fmax, byte fdval, 
													byte dmin, byte dmax, byte buzzf, byte buzzd, byte pRep, float wend, byte nfo) {
  byte sentencelength = TrueRandom.random(lenmin, lenmax);
  byte wordEndDelay, soundEndDelay, patternRep, playRep;
  byte k, randChoice;
  r2sentence r2s;
	byte f0, f1, d0, d1, dA0, fI, flow, fdist, dI;
	byte senti = 0;
	byte sqi = 0;
	byte prevsound = 255;
	bool pRepBool = false;
	bool sRepBool = false;
	r2s.sentencelength = sentencelength; 
  //SS[i] = r2_soundGen(3000  , 10000  , 10000, 10, 1000, 6000, 5, 25, 20, 10, patternRep); //normal
  //r2sound sound0 = r2_soundGen(100  , 7100 , 10000, 10, 1000, 6000, 5, 25, 20, 10); //angry
  //r2sound sound0 = r2_soundGen(10000  , 27000 , 10000, 10, 10000, 16000, 5, 25, 20, 10); //problem   
  // filling up the sound array
  fdist = (fmax - fmin) / (255 / fdval);
	for(int i=0; i<10; i++){
		// soundgen
	  dA0 = TrueRandom.random(0, fdist);
	  f0 = TrueRandom.random(fmin, fmax-dA0);
	  f1 = f0 + dA0;
	  fI = 100 + TrueRandom.random(buzzd, buzzf);
	  d0 = TrueRandom.random(dmin, dmax);
	  d1 = TrueRandom.random(dmin, dmax);
	  dI = 100 + TrueRandom.random(buzzd, buzzf);
    //storage
	  r2s.sounds[i] = {f0, f1, fI, d0, d1, dI};	      
		// soundinfo
	  if (nfo > 0) printp("i: ", i);
		if (nfo > 0) printsoundinfo2(&r2s.sounds[i]);
	}  
	// filling up the sequence 
  while(1) {
    if (nfo > 0) printp("senti: ", senti);
    if (nfo > 0) printp("sqi: ", sqi);
    if (nfo > 0) printp(" -sentencelength: ", r2s.sentencelength);
		if ((senti >= r2s.sentencelength) or (sqi>90)) break; 
		// choose a sound
		randChoice = TrueRandom.random(0, 7);
		if (nfo > 0) printp(" -randChoice: ", randChoice);
		if ((randChoice == 0) and sRepBool) {
			r2s.sq[sqi] = prevsound; //previous sound
			sRepBool = false;
			if (nfo > 0) printp(" -soundprev: ", prevsound);
		} else {
			if (senti >= 10) {
				r2s.sq[sqi] = prevsound; // random sound
				while (r2s.sq[sqi] == prevsound) r2s.sq[sqi] = TrueRandom.random(0, 10);
			} else r2s.sq[sqi] = senti; // same sound as index
			prevsound = r2s.sq[sqi];
			if (nfo > 0) printp(" -soundnew: ", prevsound);
			sRepBool = true;
		}
		//pattern
		randChoice = TrueRandom.random(0, 7);
		if (nfo > 0) printp(" -randChoice: ", randChoice);
   	if (nfo > 0) Serial.print(" -pattern:");
		k = 3;
		if ((randChoice == 0) and pRepBool) {
			r2s.sq[sqi+k] = 0; //repeat prev pattern  
			pRepBool = false; // do not repeat again 
    	if (nfo > 0) Serial.print(0);
    	k++;
		} else {
			patternRep = TrueRandom.random(2, 5);
			if (nfo > 0) printp(" -patternRep: ", patternRep);
			pRepBool = true;
			for(k=3; k<patternRep+3; k++) {
				r2s.sq[sqi+k] = TrueRandom.random(1, 5);
    		if (nfo > 0) printp(" p:", r2s.sq[sqi+k]);
			}
		}
		//soundEndDelay
		soundEndDelay = TrueRandom.random(50, 180) / (6-patternRep);
		r2s.sq[sqi+1] = soundEndDelay;
    if (nfo > 0) printp(" -soundEndDelay: ", soundEndDelay);
		if (nfo > 0) printp(" *k:", k);
		//playrep
	  playRep = TrueRandom.random(0, pRep) * ((6 - patternRep) / 3);
    if (nfo > 0) printp(" -playRep: ", playRep);
		r2s.sq[sqi+2] = playRep;
		//wordEndDelay
		wordEndDelay = TrueRandom.random(100*wend, 200*wend);
    if (nfo > 0) printp2(" -wordEndDelay: ", wordEndDelay);
		r2s.sq[sqi+k] = wordEndDelay;
		sqi += k+1;
		senti += 1;
  }
  if (nfo > 0) Serial.println();
  if (nfo > 0) printp2(" sentencelength", r2s.sentencelength);
  return r2s;
}

void r2_SentencePlay(r2sentence *r2s) {
	byte sqi = 0;
	byte j, soundEndDelay, wordEndDelay, playRep;
	byte patternprev[6];
	byte k = 0;
	byte wordnum = 0;
	while(1) {
		if (wordnum == r2s->sentencelength) break;
		printp("wordnum: ", wordnum);
		printp(" sl: ", r2s->sentencelength);
		printp(" sqi: ", sqi);
		printp(" (", r2s->sq[sqi]);
		printp("-",((r2s->sounds[r2s->sq[sqi]].f0+r2s->sounds[r2s->sq[sqi]].f1)/2)*100, ")");
		soundEndDelay = r2s->sq[sqi+1];
		printp(" -soundEndDelay", soundEndDelay);
		playRep = r2s->sq[sqi+2];
		printp(" -playRep:", playRep);
		k = 3;
		printp(" -pattern:",r2s->sq[sqi+k]);
		Serial.print("-");
		if (r2s->sq[sqi+k] == 0) {
			j=0;
			while(patternprev[j] < 10){
				Serial.print(patternprev[j]);
				r2_soundPlayTyp(r2s->sounds[r2s->sq[sqi]], patternprev[j], playRep);
				j++;
			}
			k++;
		} else {
			while (r2s->sq[sqi+k] < 10) {
				Serial.print(r2s->sq[sqi+k]);
				r2_soundPlayTyp(r2s->sounds[r2s->sq[sqi]], r2s->sq[sqi+k], playRep);
				patternprev[k-3] = r2s->sq[sqi+k];
				k++;
			}
			patternprev[k-3] = r2s->sq[sqi+k];
			for(int z=0; z<6; z++){
			   printp(" pp:", patternprev[z]);
			}
		}
		wordEndDelay = r2s->sq[sqi+k];
		printp(" -k:", k);
    printp2(" -wordEndDelay: ", wordEndDelay);
    delay(wordEndDelay);
		sqi += k + 1;
		wordnum++;
	}
	Serial.println();
}

r2sentence r2_SentenceGenMood(byte level, byte tired, byte slow) {

}

int sentencei = 1;
r2sentence r2s;
void loop()
{
  //int patternRep = random(1, 6);
  if (sentencei < 4000) {
    printp2("Sentence: ", sentencei);
    // r2s = r2_SentenceGen(6,12, 2,255,   75,  5, 100,  20, 0.2, 1);
    // r2_SentencePlay(&r2s);
    // r2s = r2_SentenceGen(6,12, 2,255,   75,  1, 20,  30, 0.4, 1);
    // r2_SentencePlay(&r2s);

    r2s = r2_SentenceGen(6,12, 30,100,255, 5,20,  2, 40,  20, 0.8, 1);
    r2_SentencePlay(&r2s);
//    r2s = r2_SentenceGen(6,12, 200,255, 65, 2, 60, 20, 0.4, 1);
    //r2_SentencePlay(&S_gps);
    // r2sentence sent0 = r2_SentenceGen(2,6);
    // r2_SentencePlay(sent0);
    // if (TrueRandom.random(1, 10) == 1) {
    //   int ldelay = TrueRandom.random(2500, 6000);
    //   Serial.print("long delay");
    //   Serial.println(ldelay);
    //   delay(ldelay);
    // }
    sentencei++;
  }
}     
     
void beep (int speakerPin, float noteFrequency, long noteDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
          {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
          } 
} 

void printsoundinfo(r2sound *sound0) {
		printp("||f0: ", (int)sound0->f0*100);
		printp(" -f1: ",(int)sound0->f1*100);
		printp(" -freqInc: ",(float)sound0->fI/100);
		printp(" -d0: ",sound0->d0);
		printp(" -d1: ",sound0->d1);
		printp2(" -durInc: ", (float)sound0->dI/100);
}
void printsoundinfo2(r2sound *sound0) {
		printp("{",sound0->f0);
		printp(",",sound0->f1);
		printp(",",sound0->fI);
		printp(",",sound0->d0);
		printp(",",sound0->d1);
		printp2(",",sound0->dI, "}");
}
void printp(String s0, int i0, String s1) {
  if (PRNT == 1) {Serial.print(s0);  Serial.print(i0); Serial.print(s1); };
} 
void printp(String s0, int i0) {
  if (PRNT == 1) {Serial.print(s0);  Serial.print(i0);};
} 
void printp(String s0, float i0) {
  if (PRNT == 1) {Serial.print(s0);  Serial.print(i0);};
}
void printp2(String s0, int i0) {
  if (PRNT == 1) {Serial.print(s0);  Serial.println(i0);};
} 
void printp2(String s0, int i0, String s1) {
  if (PRNT == 1) {Serial.print(s0);  Serial.print(i0); Serial.println(s1); };
} 
void printp2(String s0, float i0) {
  if (PRNT == 1) {Serial.print(s0);  Serial.println(i0);};
}
