#ifndef MAYANCACHE_H_INCLUDED
#define MAYANCACHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "..\MayaBoids\Definition.h"

// generic swap macro managing "endianess"
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

////#define info.particleSysName	"BoidPtShape"
//#define CACHEOPTION int 
//#define ALLCHANNEL 16					// not used 
//#define POSITION 6						// id,count,birthTime,position,lifespanPP,finalLifespanPP
//#define POSITIONVELOCITY 7				// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity
//#define POSITIONVELOCITYACCELERATION 8	// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity,acceleration
//// open to futures implementations 

// channel type 
#define CHANNELTYPE int
#define _ID 0
#define _COUNT 1
#define _BIRTHTIME 2
#define _POSITION 3
#define _LIFESPANPP 4
#define _FINALLIFESPANPP 5
#define _VELOCITY 6
#define _ACCELERATION 7
#define _WORLDPOSITION 8
#define _WORLDVELOCITY 9
#define _WORLDVELOCITYINOBJECTSPACE 10
#define _MASS 11
#define _AGE 12
#define _RGBPP 13
#define _OPACITYPP 14
#define _RADIUSPP 15

// channel name 
#define CHANNELNAME char*
#define IDCHANNEL "id"
#define COUNTCHANNEL "count"
#define BIRTHTIMECHANNEL "birthTime"
#define POSITIONCHANNEL "position"
#define LIFESPANPPCHANNEL "lifespanPP"
#define FINALLIFESPANPPCHANNEL "finalLifespanPP"
#define VELOCITYCHANNEL "velocity"
#define ACCELERATION "acceleration"
#define WORLDPOSITION "worldPosition"
#define WORLDVELOCITY "worldVelocity"
#define WORLDVELOCITYINOBJECTSPACE "worldVelocityInObjectSpace"
#define MASS "mass"
#define AGE "age"
#define RGBPP "rgbPP"
#define OPACITYPP "opacityPP"
#define RADIUSPP "radiusPP"

// channel array format
#define CHANNELFORMAT char*
#define DOUBLEARRAY "DoubleArray"
#define FLOATVECTORYARRAY "FloatVectorArray"
#define FVCACHANNEL "FVCA"
#define DBLACHANNEL "DBLA"

#define FOR4 "FOR4"
#define ETIM "ETIM"
#define STIM "STIM"
#define CACHEVERSION "CACHVRSN"
#define MYCH "MYCH"
#define TIME "TIME"
#define CHNM "CHNM"
#define SIZE "SIZE"


// variable type
#define FVCA 0
#define DBLA 1

// cache format type
//#define CACHEFORMAT int
//#define ONEFILE 0
//#define ONEFILEPERFRAME 1

// default value
#define MAYATICK 6000
#define BUFFERLENGTH 65536
#define FOUR 0x04000000

typedef struct channelName
{
	char** names;
}ChannelName;

typedef  struct Info
{
	 char *particleSysName;
	 char *mcFileName;
	 char *xmlFileName;
	 CACHEFORMAT cacheFormat;
	 CACHEOPTION option;
	 unsigned int fps;
	 double duration;
	 int start;				// [tick]
	 int end;				// [tick]
	 unsigned int mayaFPS;	// [seconds]
	 FILE * mayaMCFile;
	 FILE * mayaXMLFile;
     char mcChannelBuffer[BUFFERLENGTH];
	 char xmlChannelBuffer[BUFFERLENGTH];
}Info;


//typedef struct Channel
//{
//	char *name;
//	char *attribute;
//	int type;
//	int numberOfElements;
//	void *elements;			// DBLA elements: double array; FVCA elements: float triples' array  
//}Channel;

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

ChannelName cName;
ChannelName aName;
Info info;

static const char *channelsName[]={"_id","_count","_birthTime","_position","_lifespannPP","_finalLifespanPP"
							,"_velocity","_acceleration","_worldPosition","_worldVelocity","_worldVelocityInObjectSpace"
							,"_mass","_age","_rgbPP","_opacityPP","_radiusPP"};

static const char *attributesName[]={"id","count","birthTime","position","lifespannPP","finalLifespanPP"
							,"velocity","acceleration","worldPosition","worldVelocity","worldVelocityInObjectSpace"
							,"mass","age","rgbPP","opacityPP","radiusPP"};

static int delta;

/* 
	particleSysName [MAYA particle system name]
	fileName [file name, without extension] 
	cacheType [ONEFILE/ONEFILEPERFRAME] 
	option [POSITION/POSITIONVELOCITY/ALLCHANNEL] 
	fps [frame per second,example 25/30]
	start [frame] 
	end [frame]
*/
void init(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat, CACHEOPTION option, unsigned int fps, double start, double end);

void writeMayaNCacheHeader();

// frame [actual frame], channels [array of channels]
void writeMayaNCacheBlock(int frame, Channel *channels);

//channels [array of channels]
void writeMayaNCacheChannel(Channel * channel);

void closeMayaNCacheFile();

void deleteFile();

#endif // MAYANCACHE_H_INCLUDED