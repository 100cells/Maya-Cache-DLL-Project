#ifndef MAYANCACHE_H_INCLUDED
#define MAYANCACHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// generic swap macro "endianess"
#define GENERICSWAP(type,returnType)				\
	returnType  swap##type(type d)					\
	{												\
													\
		returnType a;								\
		int i,dimension;							\
		unsigned char *dst = (unsigned char *)&a;	\
		unsigned char *src = (unsigned char *)&d;	\
		dimension=sizeof(type);						\
		for(i=0;i<dimension;i++)					\
			dst[i]=src[dimension-i-1];				\
													\
		return a;									\
	}	

//#define PTSYS	"BoidPtShape"
#define CACHETYPE int 
#define ALLCHANNEL 0		// not used 
#define POSITION 6			// id,count,birthTime,position,lifespanPP,finalLifespanPP
#define POSITIONVELOCITY 7	// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity

// channel type 
#define CHANNELTYPE int
#define _ID 0
#define _COUNT 1
#define _BIRTHTIME 2
#define _POSITION 3
#define _LIFESPANPP 4
#define _FINALLIFESPANPP 5
#define _VELOCITY 6

// channel name 
#define IDCHANNEL "id"
#define COUNTCHANNEL "count"
#define BIRTHTIMECHANNEL "birthTime"
#define POSITIONCHANNEL "position"
#define LIFESPANPPCHANNEL "lifespanPP"
#define FINALLIFESPANPPCHANNEL "finalLifespanPP"
#define VELOCITYCHANNEL "velocity"

// suffix
#define PTSYS	"nParticleShape1"

// variable type
#define FVCA 0
#define DBLA 1

// cache format type
#define CACHETYPE int
#define ONEFILE 0
#define ONEFILEPERFRAME 1

// default value
#define BUFFERLENGTH 65536
#define FOUR 0x04000000

typedef struct Channel
{
	char *name;
	char *attribute;
	int type;
	int numberOfElements;
	void *elements;			// DBLA elements: double array; FVCA elements: float triples' array  
}
Channel;

typedef struct Header
{
    char format[4];
    unsigned int length;
    char cacheVersion[8];
    unsigned int separator1;
    char version[4];
    char stim[4];
    unsigned int stimFirstPart;
    unsigned int stimSecondPart;
	char etim[4];
    unsigned int etimFirstPart;
    unsigned int etimSecondPart;
} Header;

static FILE * mayaNCacheFile;
static char channelBuffer[BUFFERLENGTH];
static char * mayaNcacheFileName;

void setMayaNCacheFileName(char * fileName);
void closeMayaNCacheFile();
void writeMayaNCacheHeader(CACHETYPE type);
void writeMayaNCacheBlock(int frame, int fps, Channel *channels, CACHETYPE typeCache);
void writeMayaNCacheChannel(Channel * channel);

#endif // MAYANCACHE_H_INCLUDED