//c89
#ifndef _DEFINITION_H_INCLUDED
#define _DEFINITION_H_INCLUDED

#include "Boolean.h"

#define LOG BOOL
#define LOGENABLEDON TRUE
#define LOGENABLEDOFF FALSE

#define SAVEMETHOD int
#define MAYANCACHE 0
#define TEXTDATA 1
#define XMLDATA 2

#define CACHEFORMAT int
#define ONEFILE 0
#define ONEFILEPERFRAME 1

//#define info.particleSysName	"BoidPtShape"
#define CACHEOPTION int 
#define ALLCHANNEL 16					// not used 
#define POSITION 6						// id,count,birthTime,position,lifespanPP,finalLifespanPP
#define POSITIONVELOCITY 7				// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity
#define POSITIONVELOCITYACCELERATION 8	// id,count,birthTime,position,lifespanPP,finalLifespanPP,velocity,acceleration
// open to futures implementations 

typedef struct channel
{
	char *name;
	char *attribute;
	int type;
	int numberOfElements;
	void *elements;			// DBLA elements: double array; FVCA elements: float triples' array  
}Channel;

typedef struct infoCache
{
	char *particleSysName;
	char *fileName;
	SAVEMETHOD saveMethod;
	CACHEFORMAT cacheFormat;
	CACHEOPTION option;
	LOG loging;
	unsigned int fps;
	long int start;
	long int end;
}InfoCache;

#endif