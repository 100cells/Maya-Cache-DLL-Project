#define DLL

#include "MayaNCache.h"
#include "XmlWriter.h"
#include <string.h>

 GENERICSWAP(double,unsigned long long)		// swap for double variables: swapdouble(unsigned long long d)
 GENERICSWAP(float,unsigned int)			// swap for float variables: swapfloat(unsigned int d)
 GENERICSWAP(int,unsigned int)				// swap for int variables:  swapint(unsigned int d)

  //utility functions
 void makeName(CHANNELTYPE type, char **channelName);
 BOOL fileExists(const char * filename);
 void writeMayaNCacheHeader();
 void writeMayaNCacheChannel(CHANNELTYPE cType);
 void writeMayaNCacheBlock();

 void enableChannel(CHANNELTYPE channelActive, ENABLEDISABLED ea)
 {
	 mayaChannels[channelActive].enabled=ea;
	 mayaChannels[channelActive].elementsD=NULL;
	 mayaChannels[channelActive].elementsF=NULL;
	 if (ea==ENABLED)
	 {
		mayaChannels[channelActive].numberOfElements=info.numberOfElements;
		// assigning name
		makeName(channelActive, &mayaChannels[channelActive].name);
		switch(channelActive) 
		{
			case IDCHANNEL:
			case COUNTCHANNEL:
			case BIRTHTIMECHANNEL:
			case MASSCHANNEL:
			case AGECHANNEL:
			case OPACITYPPCHANNEL:
			case RADIUSPPCHANNEL:
			case FINALLIFESPANPPCHANNEL:
				mayaChannels[channelActive].type = DBLA;
				mayaChannels[channelActive].numberOfComponents = 1;
				break;
			case RGBPPCHANNEL:
			case POSITIONCHANNEL:
			case VELOCITYCHANNEL:
			case ACCELERATIONCHANNEL:
			case WORLDPOSITIONCHANNEL:
			case WORLDVELOCITYCHANNEL:
			case WORLDVELOCITYINOBJECTSPACECHANNEL:
				mayaChannels[channelActive].numberOfComponents = 3;
				mayaChannels[channelActive].type=FVCA;
				break;
			case LIFESPANPPCHANNEL:
				mayaChannels[channelActive].numberOfComponents = 3;
				mayaChannels[channelActive].type = DBLA;
				break;
		};
		info.numberOfChannels++;
	 } 
	 else
	 {

		info.numberOfChannels--;
		mayaChannels[channelActive].numberOfElements=0;
		mayaChannels[channelActive].numberOfComponents = 0;
		mayaChannels[channelActive].type=FVCA;
		free(mayaChannels[channelActive].elementsD);
		free(mayaChannels[channelActive].elementsF);
		free(mayaChannels[channelActive].name);
		mayaChannels[channelActive].elementsD=NULL;
		mayaChannels[channelActive].elementsF=NULL;
	 }
}

 void assignChannelValues(CHANNELTYPE channelActive, void *sourceValues)
 {
	if (mayaChannels[channelActive].type == FVCA)
	{
		mayaChannels[channelActive].elementsF=(float *)sourceValues;
		mayaChannels[channelActive].elementsD=NULL;
	}
	else
	{
		mayaChannels[channelActive].elementsD=(double*)sourceValues;
		mayaChannels[channelActive].elementsF=NULL;
	}
 }
 
 void init(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat,int numberOfElements, unsigned int fps, double start, double end,char *extra[], int nE)
 {
	char *mcName, *xmlName;
	int i=0;
	int padding=1,temp=0;
	temp=(int)(end-start)*fps;

	info.extras=(char**)malloc(nE*sizeof(char*));
	info.nExtras=nE;
	for (i=0;i<nE;i++)
	{	
		info.extras[i]=(char*)malloc(sizeof(char)*strlen(extra[i]));
		strcpy(info.extras[i],extra[i]);
	}

	info.numberOfChannels=0;
	info.particleSysName=particleSysName;
	info.numberOfElements=numberOfElements;

	info.startFrame=(int)floor(start*fps);			//computing starting frame
	info.currentFrame=info.startFrame;
	info.frameIncrement=1;

	for(i=0;i<CACHENUMBEROFCHANNELS;i++)
		mayaChannels[i].enabled=DISABLED;

	while (temp/10>1)
	{
		padding++;
		temp=temp/10;
	}

	// adding extension ".mc"
	mcName = (char*)malloc(sizeof(char)*(strlen(fileName)+padding+4));
	strcpy(mcName, fileName);
	strcat(mcName, ".mc");
	info.mcFileName=mcName;
	
	mayaCacheFileName=(char*)malloc(sizeof(char)*(strlen(fileName)));
	strcpy(mayaCacheFileName, fileName);
	
	printf("Enabling %s in write mode\n",info.mcFileName);
	
	// adding extension ".xml"
	xmlName = (char*)malloc(sizeof(char)*(strlen(fileName)+5));
	strcpy(xmlName, fileName);
	strcat(xmlName, ".xml");
	info.xmlFileName=xmlName;
		
	printf("Enabling %s in write mode \n",info.xmlFileName);

	info.cacheFormat=cacheFormat;
	info.fps=fps;												// [frame/sec]
	info.duration=(int)(fps*(end-start));						// [frame]
	info.mayaTPF=MAYATICK/fps;									// [mayaticks/frame]
	info.start=(int)floor(start*fps)*info.mayaTPF;				// [mayaticks]
	info.end=info.duration*info.mayaTPF+info.start;				// [mayaticks]			
		
	if (info.cacheFormat==ONEFILE)
	{
		info.mayaMCFile=fopen(info.mcFileName, "wb");
		writeMayaNCacheHeader();
	}	
	isXmlFinalized=FALSE;
 }

void closeMayaNCacheFile()
{
    if(info.mayaMCFile!=NULL)
	{
		fflush(info.mayaMCFile);
        fclose(info.mayaMCFile);
		if (info.cacheFormat == ONEFILE)
			printf("File '%s' was successfully closed and saved\n", info.mcFileName);
	}
	else
		printf("Unable to save NCache %s file\n", info.mcFileName);
}

// header 
void writeMayaNCacheHeader() 
{
	Header header;
	int startTime,endTime;
	startTime=0;
	endTime=1;

	if(info.mayaMCFile==NULL)
		printf("Unable to open %s file in write mode\n",info.mcFileName);
	else
	{

		info.mayaMCFile=fopen(info.mcFileName, "wb");
		setvbuf(info.mayaMCFile, info.mcChannelBuffer, _IOFBF, BUFFERLENGTH);
		memcpy(header.format, FOR4, sizeof(header.format));
		header.length = swapint(40);
		memcpy(header.cacheVersion, CACHEVERSION, sizeof(header.cacheVersion));
		header.separator1 = swapint(4);
		memcpy(header.version, "0.1", sizeof(header.version));
		memcpy(header.stim, STIM, sizeof(header.stim)); 
		memcpy(header.etim, ETIM, sizeof(header.etim));
		header.stimFirstPart = swapint(4);
		header.etimFirstPart = swapint(4);
		if (info.cacheFormat==ONEFILE)
		{
			header.stimSecondPart = swapint(startTime);	
			header.etimSecondPart = swapint(endTime);
		}
		else
		{
			header.stimSecondPart = swapint(info.currentFrame*info.mayaTPF);
			header.etimSecondPart = header.stimSecondPart;
		}
		fwrite(&header, sizeof(header), 1, info.mayaMCFile);
	}
}

// channel
void writeMayaNCacheChannel(CHANNELTYPE cType)
{
	unsigned int  dim, swappedDim, padding, size, nElements, i, channelArrayLength, dataTypeSize, arrayElements, temp, four;
	long long int channelLenght=0, tempLong;
	float * pFloat;
	double * pDouble;
	char type[4];
	char *paddingString;
	
	dim = (unsigned int)strlen(mayaChannels[cType].name);
	padding = 4 - dim%4;
	swappedDim = swapint(dim+1);
	four = swapint(4);
	size = swapint(mayaChannels[cType].numberOfElements);
	
	if(mayaChannels[cType].type==FVCA)
	{
		pFloat = mayaChannels[cType].elementsF;
		channelArrayLength = 3 * 4 * mayaChannels[cType].numberOfElements;
		strncpy(type, FVCACHANNEL, 4);
		dataTypeSize = 4;
		arrayElements = 3 * mayaChannels[cType].numberOfElements;
	}
	else
	{
		pDouble = mayaChannels[cType].elementsD;
		strncpy(type, DBLACHANNEL, 4);
		dataTypeSize = 8;

		if (cType == COUNTCHANNEL)
		{
			channelArrayLength = 8;
			arrayElements = 1;
			size = swapint(1);
		}
		else
		{
			arrayElements = mayaChannels[cType].numberOfElements;
			channelArrayLength = 8 * arrayElements;
		}
	}

	channelArrayLength = swapint(channelArrayLength);
	nElements = swapint(arrayElements);
	paddingString = (char*)calloc(padding, sizeof(char));
	fwrite(CHNM, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&swappedDim, sizeof(int), 1, info.mayaMCFile);
	fwrite(mayaChannels[cType].name, dim, 1, info.mayaMCFile);
	fwrite(paddingString, padding, 1, info.mayaMCFile);
	fwrite(SIZE, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&four, sizeof(int), 1, info.mayaMCFile);
	fwrite(&size, sizeof(int), 1, info.mayaMCFile);
	fwrite(type, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&channelArrayLength, sizeof(int), 1, info.mayaMCFile);
		
	//writing elements
	for(i=0; i<arrayElements; i++)
	{
		//swap type + write
		if(mayaChannels[cType].type==FVCA)
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
	free(paddingString);
}

// data block
void writeMayaNCacheBlock()
{
	// single data block writing, 
	// each Block start with "FOR4"
	// each block contains:
	// MYCH (Group)	
	// TIME		// Time (int)

	unsigned int time, dimB, timeVar, nChannels=0, dataLength=0, dataSize=0;
	int padding,i;
	timeVar = swapint(4);
	time = swapint(info.currentFrame*MAYATICK/info.fps);
	fwrite(FOR4, 4*sizeof(char), 1, info.mayaMCFile);

	// computing the block length
	for(i=0;i<CACHENUMBEROFCHANNELS;i++)
	{
		int size=0;
		dataSize = 0;
		if (mayaChannels[i].enabled)
		{
			padding=4-strlen(mayaChannels[i].name)%4;
			dataLength += (unsigned int)strlen(mayaChannels[i].name) + padding;
			if(mayaChannels[i].type==FVCA)
				dataSize = 3 * 4 * mayaChannels[i].numberOfElements;
			else
				if (i==COUNTCHANNEL)
					dataSize = 8;
				else
					dataSize = 8* mayaChannels[i].numberOfElements;
			
			dataLength += dataSize;
		}
	}
	
	if (info.cacheFormat==ONEFILE)
		dimB = SINGLEFILEBLOCKFIXEDDIM + CHANNELFIXEDDIM * info.numberOfChannels + dataLength;	
	else
		dimB = MULTIFILEBLOCKFIXEDDIM + CHANNELFIXEDDIM * info.numberOfChannels + dataLength;	

	dimB = swapint(dimB);
	fwrite(&dimB, sizeof(unsigned int), 1, info.mayaMCFile);
	fwrite(MYCH, 4*sizeof(char), 1, info.mayaMCFile);
	if (info.cacheFormat==ONEFILE)
	{
		fwrite(TIME, 4*sizeof(char), 1, info.mayaMCFile);
		fwrite(&timeVar, sizeof(int), 1, info.mayaMCFile);
		fwrite(&time, sizeof(int), 1, info.mayaMCFile);
	}

	for(i=0;i<CACHENUMBEROFCHANNELS;i++)
		if (mayaChannels[i].enabled)
			writeMayaNCacheChannel(i);
		
	fflush(info.mayaMCFile);
	if(isXmlFinalized==FALSE)
	{
		printXml(mayaChannels);
		closeXmlFile();
		isXmlFinalized=TRUE;
	}
}

// entry point for saving data in maya cache file format (support "one file per frame" and "one file" options)
void mayaCache()
{
	char *mcName;
	char *subfix;
	int sflenght,fps;

	if (info.cacheFormat==ONEFILE)
		writeMayaNCacheBlock();
	else
	{
	    fps=info.currentFrame;
		sflenght=1;
		while (fps>1)
		{
			sflenght++;
			fps=fps/10;
		}
		subfix = (char*)malloc(sizeof(char)*(strlen(mayaCacheFileName)+sflenght));
		itoa(info.currentFrame,subfix,10);
		
		// adding extension ".mc"
		// "Frame"---->5
		mcName = (char*)malloc(sizeof(char)*(strlen(mayaCacheFileName)+sflenght+4+5));
		strcpy(mcName, mayaCacheFileName);
		strcat(mcName, "Frame");
		strcat(mcName, subfix);
		strcat(mcName, ".mc");
		if (info.mcFileName!=NULL)
			free(info.mcFileName);
		info.mcFileName=mcName;
		info.mayaMCFile=fopen(info.mcFileName, "wb");
		
		writeMayaNCacheHeader();
			setvbuf(info.mayaMCFile, info.mcChannelBuffer, _IOFBF, BUFFERLENGTH);

		writeMayaNCacheBlock();
		if(info.mayaMCFile!=NULL)
			closeMayaNCacheFile();
		else
			printf("Maya NCache file '%s' not saved!\n", info.mcFileName);
	}

	info.currentFrame+=info.frameIncrement;
}

void makeName(CHANNELTYPE type, char **channelName)
{
	int dim;
	char *underscore={"_"};
	switch(type)
	{
		case IDCHANNEL:
			dim=4;
			break;
		case COUNTCHANNEL:
			dim=7;
			break;
		case BIRTHTIMECHANNEL:
			dim=11;
			break;
		case POSITIONCHANNEL:
			dim=10;
			break;
		case LIFESPANPPCHANNEL:
			dim=12;
			break;
		case FINALLIFESPANPPCHANNEL:
			dim=17;
			break;
		case VELOCITYCHANNEL:
			dim=10;
			break;
		case ACCELERATIONCHANNEL:
			dim=14;
			break;
		case WORLDPOSITIONCHANNEL:
			dim=15;
			break;
		case WORLDVELOCITYCHANNEL:
			dim=15;
			break;
		case WORLDVELOCITYINOBJECTSPACECHANNEL:
			dim=28;
			break;
		case MASSCHANNEL:
			dim=6;
			break;
		case AGECHANNEL:
			dim=5;
			break;
		case RGBPPCHANNEL:
			dim=7;
			break;
		case OPACITYPPCHANNEL:
			dim=11;
			break;
		case RADIUSPPCHANNEL:
			dim=10;
			break;

		default:
			printf("Option not allowed yet.\n Exit from program.\n Press a key to exit");
			getchar();
			break;
	}
	*channelName=(char*)calloc(sizeof(info.particleSysName)+dim, sizeof(char));
	strcpy(channelName[0], info.particleSysName);
	strcat(channelName[0], underscore);
	strcat(channelName[0], names[type]);
}


BOOL fileExists(const char * filename)
{
	FILE * file;
	if ( file= fopen(filename, "r"))
	{
		fclose(file);
		return TRUE;
	}
	return FALSE;
}

void deleteFile()
{
	// closing the streams
	closeMayaNCacheFile();

	// deleting mc file
	if(fileExists(info.mcFileName))
		remove(info.mcFileName);

	// deleting xml file
	if(fileExists(info.xmlFileName))
		remove(info.xmlFileName);
}

unsigned int DLL_EXPORT getDuration(){return info.duration;}
int DLL_EXPORT getStartFrame(){return info.startFrame;}
int DLL_EXPORT getFrameIncrement(){return info.frameIncrement;}