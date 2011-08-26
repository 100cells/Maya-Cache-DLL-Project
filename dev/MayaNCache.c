#include "MayaNCache.h"

 GENERICSWAP(double,unsigned long long)		// swap for double variables
 GENERICSWAP(float,unsigned int)			// swap for float variables
 GENERICSWAP(int,unsigned int)				// swap for int variables

 void init(char *particleSysName,char *fileName, CACHETYPE cacheType, CACHEOPTION option, unsigned int fps, int start, int end)
 {
	char *mcName, *xmlName;
	
	// adding extension ".mc"
	mcName = (char*)malloc(sizeof(char)*(strlen(fileName)+4));
	strcpy(mcName, fileName);
	strcat(mcName, ".mc");
	info.mcFileName=mcName;
	info.mayaMCFile=fopen(mcName, "wb");
	setvbuf(info.mayaMCFile, info.mcChannelBuffer, _IOFBF, BUFFERLENGTH);

	// adding extension ".xml"
	xmlName = (char*)malloc(sizeof(char)*(strlen(fileName)+5));
	strcpy(xmlName, fileName);
	strcat(xmlName, ".xml");
	info.xmlFileName=xmlName;
	info.mayaXMLFile=fopen(xmlName, "wb");
	setvbuf(info.mayaXMLFile, info.xmlChannelBuffer, _IOFBF, BUFFERLENGTH);

	if(cacheType!=ONEFILE)
		printf("Option is not yet supported. Next revision will support the multi file option\nCache will be saved in a single file\n");
		info.cacheType=ONEFILE;

	info.option=option;
	info.fps=fps;
	info.start=MAYATICK/fps*start;			
	info.end=MAYATICK/fps*end;				
	info.duration=(end-start)/fps;
	info.mayaFPS=MAYATICK/fps;
	info.particleSysName=particleSysName;
 }


void closeMayaNCacheFile()
{
    if(info.mayaMCFile!=NULL)
	{
		printf("File '%s' was successfully closed and saved\n", info.mcFileName);
        fclose(info.mayaMCFile);
	}
	else
		printf("Maya NCache file '%s' not saved!\n", info.mcFileName);
}


void writeMayaNCacheHeader() 
{
	Header header;
	int startTime,endTime;
	startTime=0;
	endTime=1;

	// TO DO!!
	// generalize the header function for "one file per frame" and "one file"
    
    memcpy(header.format, "FOR4", sizeof(header.format));
    header.length = swapint(40);
    memcpy(header.cacheVersion, "CACHVRSN", sizeof(header.cacheVersion));
    header.separator1 = swapint(4);
    memcpy(header.version, "0.1", sizeof(header.version));
    memcpy(header.stim, "STIM", sizeof(header.stim));
    header.stimFirstPart = swapint(4);
    header.stimSecondPart = swapint(startTime);
    memcpy(header.etim, "ETIM", sizeof(header.etim));
    header.etimFirstPart = swapint(4);
    header.etimSecondPart = swapint(endTime);
	fwrite(&header, sizeof(header), 1, info.mayaMCFile);
}

void writeMayaNCacheBlock(int frame,  Channel *channels)
{
	// funzione per la scrittura di un blocco dati relativo ad ogni BLOCCO "Block"
	// ogni Block inizia con "FOR4"
	// ogni blocco contiene:
	// MYCH (Group)	
	// TIME		// Time (int)

	unsigned int time, dimB, timeVar, nChannels=0, dataLength=0, dataSize=0;
	int padding,i;
	timeVar = swapint(4);
	time = swapint(frame*MAYATICK/info.fps);
	fwrite("FOR4", 4*sizeof(char), 1, info.mayaMCFile);

	// il campo seguente è la dimensione del blocco totale espresso in esadecimale (4 byte)
	for(i=0;i<info.option;i++)
	{
		int size=0;
		dataSize = 0;
		padding=4-strlen(channels[i].name)%4;
		dataLength += strlen(channels[i].name) + padding;
		if(channels[i].type==FVCA)
			dataSize = 3 * 4 * channels[i].numberOfElements;
		else
			dataSize = 8 * channels[i].numberOfElements;

		dataLength += dataSize;
	}

	dimB = 16 + 28 * info.option + dataLength;
	dimB = swapint(dimB);
	fwrite(&dimB, sizeof(unsigned int), 1, info.mayaMCFile);
	fwrite("MYCH", 4*sizeof(char), 1, info.mayaMCFile);
	fwrite("TIME", 4*sizeof(char), 1, info.mayaMCFile);
	// dimensione in byte della variabile "time" (4 byte)
	fwrite(&timeVar, sizeof(int), 1, info.mayaMCFile);
	fwrite(&time, sizeof(int), 1, info.mayaMCFile);

	for(i=0;i<info.option;i++)
		writeMayaNCacheChannel(&channels[i]);
}

void writeMayaNCacheChannel(Channel * channel)
{
	unsigned int  dim, swappedDim, padding, size, nElements, i, channelArrayLength, dataTypeSize, arrayElements, temp, four;
	long long int channelLenght=0, tempLong;
	float * pFloat;
	double * pDouble;
	char type[4];
	char *paddingString;
	
	dim = strlen(channel->name) + 1;
	padding = 4 - dim%4;
	if(padding==4)
		padding=0;
	swappedDim = swapint(dim);
	four = swapint(4);
	size = swapint(channel->numberOfElements);
	paddingString = (char*)calloc(padding, sizeof(char));
	if(channel->type==FVCA)
	{
		pFloat = (float *)channel->elements;
		channelArrayLength = 3 * 4 * channel->numberOfElements;
		strncpy(type, "FVCA", 4);
		dataTypeSize = 4;
		arrayElements = 3 * channel->numberOfElements;
	}
	else
	{
		pDouble = (double *)channel->elements;
		channelArrayLength = 8 * channel->numberOfElements;
		strncpy(type, "DBLA", 4);
		dataTypeSize = 8;
		arrayElements = channel->numberOfElements;
	}

	channelArrayLength = swapint(channelArrayLength);
	nElements = swapint(channel->numberOfElements);

	fwrite("CHNM", 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&swappedDim, sizeof(int), 1, info.mayaMCFile);
	fwrite(channel->name, dim, 1, info.mayaMCFile);
	fwrite(paddingString, padding, 1, info.mayaMCFile);
	fwrite("SIZE", 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&four, sizeof(int), 1, info.mayaMCFile);
	fwrite(&size, sizeof(int), 1, info.mayaMCFile);
	fwrite(type, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&channelArrayLength, sizeof(int), 1, info.mayaMCFile);
	
	//writing elements
	for(i=0; i<arrayElements; i++)
	{
		//swap type + write
		if(channel->type==FVCA)
		{
			temp=swapfloat(pFloat[i]);
			fwrite(&temp, sizeof(int), 1, info.mayaMCFile);
		}
		else
		{
			tempLong = swapdouble(pDouble[i]);
			fwrite(&tempLong, sizeof(long long int), 1, info.mayaMCFile);
		}
	}
}