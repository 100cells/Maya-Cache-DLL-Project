#include "MayaNCache.h"
#include "XmlWriter.h"

 GENERICSWAP(double,unsigned long long)		// swap for double variables
 GENERICSWAP(float,unsigned int)			// swap for float variables
 GENERICSWAP(int,unsigned int)				// swap for int variables

  //utility functions
 void makeName(CHANNELTYPE type);
 BOOL fileExists(const char * filename);

 void init(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat, CACHEOPTION option, unsigned int fps, double start, double end)
 {
	char *mcName, *xmlName;
	int i=0;
	// adding extension ".mc"
	mcName = (char*)malloc(sizeof(char)*(strlen(fileName)+4));
	strcpy(mcName, fileName);
	strcat(mcName, ".mc");
	info.mcFileName=mcName;
	info.mayaMCFile=fopen(mcName, "wb");
	
	if(info.mayaMCFile==NULL)
		printf("Impossibile aprire il file %s in scrittura\n",info.mcFileName);
	else
		printf("Il file %s e' aperto in  scrittura\n",info.mcFileName);
	setvbuf(info.mayaMCFile, info.mcChannelBuffer, _IOFBF, BUFFERLENGTH);

	// adding extension ".xml"
	xmlName = (char*)malloc(sizeof(char)*(strlen(fileName)+5));
	strcpy(xmlName, fileName);
	strcat(xmlName, ".xml");
	info.xmlFileName=xmlName;
	info.mayaXMLFile=fopen(xmlName, "wb");
	
	if(info.mayaXMLFile!=NULL)
		printf("Il file %s e' aperto in  scrittura\n",info.xmlFileName);
	else
		printf("Impossibile aprire il file %s in scrittura\n",info.xmlFileName);
	setvbuf(info.mayaXMLFile, info.xmlChannelBuffer, _IOFBF, BUFFERLENGTH);

	if(cacheFormat!=ONEFILE)
		printf("Option is not yet supported. Next revision will support the multi file option\nCache will be saved in a single file\n");
	info.cacheFormat=ONEFILE;

	info.option=option;
	info.fps=fps;									// [frame/sec]
	info.start=(int)floor((start*MAYATICK)/fps);	// [mayaticks]
	info.end=(int)floor(MAYATICK*end);				// [mayaticks]
	info.duration=end-start;						// [sec]
	info.mayaFPS=MAYATICK/fps;						// [mayaticksfps]
	info.particleSysName=particleSysName;

	// computing the gap between starting simulation and starting animation
	// the 2 values couldn't be the same
	delta=(int)(start*info.fps);			

	cName.names=(char **)malloc(option*sizeof(char *));
	aName.names=(char **)malloc(option*sizeof(char *));
	for(i=0;i<option;i++)
		makeName(i);

	isXmlFinalized=FALSE;
 }


void closeMayaNCacheFile()
{
	printf("closing file mc\n");
	
    if(info.mayaMCFile!=NULL)
	{
		fflush(info.mayaMCFile);
		printf("File '%s' was successfully closed and saved\n", info.mcFileName);
        fclose(info.mayaMCFile);
	}
	else
		printf("Maya NCache file '%s' not saved!\n", info.mcFileName);

	closeXmlFile();
}


void writeMayaNCacheHeader() 
{
	Header header;
	int startTime,endTime;
	startTime=0;
	endTime=1;
	
	if(info.mayaMCFile==NULL)
		printf("Impossibile aprire il file %s in scrittura\n",info.mcFileName);
	else
	{
		memcpy(header.format, FOR4, sizeof(header.format));
		header.length = swapint(40);
		memcpy(header.cacheVersion, CACHEVERSION, sizeof(header.cacheVersion));
		header.separator1 = swapint(4);
		memcpy(header.version, "0.1", sizeof(header.version));
		memcpy(header.stim, STIM, sizeof(header.stim));
		header.stimFirstPart = swapint(4);
		header.stimSecondPart = swapint(startTime);
		memcpy(header.etim, ETIM, sizeof(header.etim));
		header.etimFirstPart = swapint(4);
		header.etimSecondPart = swapint(endTime);
		fwrite(&header, sizeof(header), 1, info.mayaMCFile);
	}
}

void writeMayaNCacheBlock(int frame,  Channel *channels)
{
	// single data block writing, 
	// each Block start with "FOR4"
	// each block contains:
	// MYCH (Group)	
	// TIME		// Time (int)

	unsigned int time, dimB, timeVar, nChannels=0, dataLength=0, dataSize=0;
	int padding,i;
	timeVar = swapint(4);
	frame+=delta;
	time = swapint(frame*MAYATICK/info.fps);
	fwrite(FOR4, 4*sizeof(char), 1, info.mayaMCFile);


	// computing the block length
	for(i=0;i<info.option;i++)
	{
		int size=0;
		dataSize = 0;
		padding=4-strlen(channels[i].name)%4;
		dataLength += (unsigned int)strlen(channels[i].name) + padding;
		if(channels[i].type==FVCA)
			dataSize = 3 * 4 * channels[i].numberOfElements;
		else
			dataSize = 8 * channels[i].numberOfElements;

		dataLength += dataSize;
	}

	dimB = 16 + 28 * info.option + dataLength;
	dimB = swapint(dimB);

	fwrite(&dimB, sizeof(unsigned int), 1, info.mayaMCFile);
	fwrite(MYCH, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(TIME, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&timeVar, sizeof(int), 1, info.mayaMCFile);
	fwrite(&time, sizeof(int), 1, info.mayaMCFile);

	for(i=0;i<info.option;i++)
		writeMayaNCacheChannel(&channels[i]);

	fflush(info.mayaMCFile);
	if(isXmlFinalized==FALSE)
	{
		printXml(channels,"none","maya 2011 x64","me");
		isXmlFinalized=TRUE;
	}
}

void writeMayaNCacheChannel(Channel * channel)
{
	unsigned int  dim, swappedDim, padding, size, nElements, i, channelArrayLength, dataTypeSize, arrayElements, temp, four;
	long long int channelLenght=0, tempLong;
	float * pFloat;
	double * pDouble;
	char type[4];
	char *paddingString;
	
	dim = (unsigned int)strlen(channel->name);
	padding = 4 - dim%4;
	swappedDim = swapint(dim+1);
	four = swapint(4);
	size = swapint(channel->numberOfElements);
	paddingString = (char*)calloc(padding, sizeof(char));
	
	//pDouble = (double *)channel->elements;
	if(channel->type==FVCA)
	{
		pFloat = (float *)channel->elements;
		channelArrayLength = 3 * 4 * channel->numberOfElements;
		strncpy(type, FVCACHANNEL, 4);
		dataTypeSize = 4;
		arrayElements = 3 * channel->numberOfElements;
	}
	else
	{
		pDouble = (double *)channel->elements;
		channelArrayLength = 8 * channel->numberOfElements;
		strncpy(type, DBLACHANNEL, 4);
		dataTypeSize = 8;
		arrayElements = channel->numberOfElements;
	}

	channelArrayLength = swapint(channelArrayLength);
	nElements = swapint(channel->numberOfElements);

	fwrite(CHNM, 4*sizeof(char), 1, info.mayaMCFile);
	fwrite(&swappedDim, sizeof(int), 1, info.mayaMCFile);
	fwrite(channel->name, dim, 1, info.mayaMCFile);
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
		if(channel->type==FVCA)
		{
			//temp=swapfloat((float)pDouble[i]);	
			temp=swapfloat(pFloat[i]);
			fwrite(&temp, sizeof(int), 1, info.mayaMCFile);
		}
		else
		{
			/*double d;
			d=pDouble[i];*/
			tempLong = swapdouble(pDouble[i]);
			fwrite(&tempLong, sizeof(long long int), 1, info.mayaMCFile);
		}
	}
}

void makeName(CHANNELTYPE type)
{
	int dim;
	switch(type)
	{
		case _ID:
			dim=4;
			break;
		case _COUNT:
			dim=7;
			break;
		case _BIRTHTIME:
			dim=11;
			break;
		case _POSITION:
			dim=10;
			break;
		case _LIFESPANPP:
			dim=12;
			break;
		case _FINALLIFESPANPP:
			dim=17;
			break;
		case _VELOCITY:
			dim=10;
			break;
		default:
			printf("Option not allowed yet.\n Exit from program.\n Press a key to exit");
			getchar();
			break;
	}
	cName.names[type]=(char*)calloc(sizeof(info.particleSysName)+dim, sizeof(char));
	aName.names[type]=(char*)calloc(sizeof(info.particleSysName)+dim-1, sizeof(char));
	strcpy(cName.names[type], info.particleSysName);
	strcat(cName.names[type], channelsName[type]);
	strcpy(aName.names[type], attributesName[type]);
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
	// chiudo gli stream dei file nel caso fossero aperti	
	closeMayaNCacheFile();

	// cancello i file
	if(fileExists(info.mcFileName))
		remove(info.mcFileName);

	if(fileExists(info.xmlFileName))
		remove(info.xmlFileName);
}