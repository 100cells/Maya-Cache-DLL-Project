#ifndef MAYANCACHE_H_INCLUDED
#define MAYANCACHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Boolean.h>


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

// channel type 
#define CHANNELTYPE int
#define IDCHANNEL 0
#define COUNTCHANNEL 1
#define BIRTHTIMECHANNEL 2
#define POSITIONCHANNEL 3
#define LIFESPANPPCHANNEL 4
#define FINALLIFESPANPPCHANNEL 5
#define VELOCITYCHANNEL 6
#define ACCELERATIONCHANNEL 7
#define WORLDPOSITIONCHANNEL 8
#define WORLDVELOCITYCHANNEL 9
#define WORLDVELOCITYINOBJECTSPACECHANNEL 10
#define MASSCHANNEL 11
#define AGECHANNEL 12
#define RGBPPCHANNEL 13
#define OPACITYPPCHANNEL 14
#define RADIUSPPCHANNEL 15

// channel array format
#define CHANNELFORMAT char*
#define DOUBLEARRAY "DoubleArray"
#define DBLACHANNEL "DBLA"
#define FLOATVECTORYARRAY "FloatVectorArray"
#define FVCACHANNEL "FVCA"

// CONSTANT VALUES DEFINITION
#define FOR4 "FOR4"
#define ETIM "ETIM"
#define STIM "STIM"
#define CACHEVERSION "CACHVRSN"
#define MYCH "MYCH"
#define TIME "TIME"
#define CHNM "CHNM"
#define SIZE "SIZE"

// the following "magic numbers" correspond to the fixed byte lenght within blocks and channels descriptions
#define CHANNELFIXEDDIM 28				// 28 stands for "CHNM" + channel name data lenght + "SIZE" + 0x4h + array size (number) + data type tag (DBLA/FVCA) +array size (in bytes)
#define SINGLEFILEBLOCKFIXEDDIM 16		// 16 stands for the byte lenght of "MYCH" + "TIME" + 0x4h + time
#define MULTIFILEBLOCKFIXEDDIM 4		// 4 stands for the byte lenght of "MYCH"


// channel variable type
#define CHANNELVARIABLETYPE int
#define FVCA 0
#define DBLA 1

// default value
#define MAYATICK 6000
#define BUFFERLENGTH 65536
#define FOUR 0x04000000

#define ENABLEDISABLED BOOL
#define ENABLED TRUE
#define DISABLED FALSE

#define CACHEFORMAT int
#define ONEFILE 0
#define ONEFILEPERFRAME 1

#define CACHENUMBEROFCHANNELS 16		// number of defined cache channels

typedef struct channel
{
	int channelReference;		// channel name reference
	BOOL enabled;				// enable or disable channel
	char *name;					// channel name
	CHANNELVARIABLETYPE type;	// type of channel DBLA/FVCA
	int numberOfElements;		// number of elements
	int numberOfComponents;		// number of components per element (example position 3 components, mass 1 component)
	double *elementsD;			// DBLA elements: double array; 
	float *elementsF;			// FVCA elements: float triples' array  
}Channel;

Channel mayaChannels[CACHENUMBEROFCHANNELS];	// array of channels

// info data structure
typedef  struct Info
{
	 char *particleSysName;
	 
	 char *mcFileName;
	 FILE *mayaMCFile;
	 char mcChannelBuffer[BUFFERLENGTH];

	 char *xmlFileName;
	 FILE *mayaXMLFile;
	 char xmlChannelBuffer[BUFFERLENGTH];

	 char **extras;
	 int nExtras;

	 CACHEFORMAT cacheFormat;	// [ONEFILE/ONEFILEPERFRAME]
	 unsigned int fps;			// [frame/sec]
	 unsigned int duration;		// [frame]
	 int start;					// [tick]
	 int end;					// [tick]
	 unsigned int mayaTPF;		// [seconds]
	 int numberOfChannels;		// [int]
	 int numberOfElements;		// [int]

	 int startFrame;
	 int currentFrame;
	 int frameIncrement;

}Info;

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

static const char *names[]={"id","count","birthTime","position","lifespanPP","finalLifespanPP"
							,"velocity","acceleration","worldPosition","worldVelocity","worldVelocityInObjectSpace"
							,"mass","age","rgbPP","opacityPP","radiusPP"};

char *mayaCacheFileName;

#ifdef DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif
void DLL_EXPORT enableChannel(CHANNELTYPE channelActive, ENABLEDISABLED ea);

void DLL_EXPORT init(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat, int numberOfElements, unsigned int fps, double start, double end,char *extras[], int nExtras);

void DLL_EXPORT assignChannelValues(CHANNELTYPE channelActive, void *sourceValues);

void DLL_EXPORT mayaCache();

void DLL_EXPORT closeMayaNCacheFile();


unsigned int DLL_EXPORT getDuration();
int DLL_EXPORT getStartFrame();
int DLL_EXPORT getFrameIncrement();




#ifdef __cplusplus
}
#endif

void deleteFile();
#endif // MAYANCACHE_H_INCLUDED