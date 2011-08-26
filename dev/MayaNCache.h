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

//#define info.particleSysName	"BoidPtShape"
#define CACHEOPTION int 
#define ALLCHANNEL 0		// not used 
#define POSITION 6			// id,count,birthTime,position,lifespanPP,finalLifespanPP
#define POSITIONVELOCITY 7	// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity
// open to futures implementations 

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

// variable type
#define FVCA 0
#define DBLA 1

// cache format type
#define CACHETYPE int
#define ONEFILE 0
#define ONEFILEPERFRAME 1

// default value
#define MAYATICK 6000
#define BUFFERLENGTH 65536
#define FOUR 0x04000000

typedef  struct Info
{
	 char *particleSysName;
	 char *mcFileName;
	 char *xmlFileName;
	 CACHETYPE cacheType;
	 CACHEOPTION option;
	 unsigned int fps;
	 unsigned int duration;
	 int start;				// [tick]
	 int end;				// [tick]
	 unsigned int mayaFPS;	// [seconds]
	 FILE * mayaMCFile;
	 FILE * mayaXMLFile;
     char mcChannelBuffer[BUFFERLENGTH];
	 char xmlChannelBuffer[BUFFERLENGTH];
}Info;


typedef struct Channel
{
	char *name;
	char *attribute;
	int type;
	int numberOfElements;
	void *elements;			// DBLA elements: double array; FVCA elements: float triples' array  
}Channel;

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

Info info;

// particleSysName [MAYA particle system name], fileName [file name, without extension], cacheType [ONEFILE/ONEFILEPERFRAME], option [POSITION/POSITIONVELOCITY/ALLCHANNEL], fps [frame per second,example 25/30], start [frame], end [frame]
void init(char *particleSysName,char *fileName, CACHETYPE cacheType, CACHEOPTION option, unsigned int fps, int start, int end);

void writeMayaNCacheHeader();

// frame [actual frame], channels [array of channels]
void writeMayaNCacheBlock(int frame, Channel *channels);

//channels [array of channels]
void writeMayaNCacheChannel(Channel * channel);

void closeMayaNCacheFile();

#endif // MAYANCACHE_H_INCLUDED