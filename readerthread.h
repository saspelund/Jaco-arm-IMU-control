// This is going to be the reader thread's function that just keeps looping as long as keepGoing is true
// 

// Plan of attack as I can't change the header, we have to assume eerything works fine and dandy
/*
read in two chars
	if the first char is 0 then the packet was successful
	else start back at the top
the second char signifies the logicID -> store it
the third char signifies returnedDataLength
read in returnedDataLength number of chars (12)

lock orient[logicID].mutex
cast first four bytes read in data as float (is this part necessary?) and set/copy to orient[logicID].pitch
cast last  four bytes read in data as float (is this part necessary?) and set/copy to orient[logicID].roll
unlock orient[logicID].mutex

start back at top
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef CALCCHECKSUM_ENDIANSWAP_INCLUDED
#define CALCCHECKSUM_ENDIANSWAP_INCLUDED
unsigned char calcCheckSum( unsigned char * startAddress, int numberOfElements)
{
	unsigned int checkSum = 0;
	for (int i=0; i<numberOfElements; i++)
	{
		checkSum += (unsigned int)startAddress[i]; //assuming it is being passed less than 2^24-1 elements
	}
	
	return (unsigned char)(checkSum & 0xff);
}

void endian_swap_32(unsigned int & x)
{
    x = (x>>24) |
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
         (x<<24);
}
#endif //CALCCHECKSUM_ENDIANSWAP_INCLUDED

void *readerFunction(void*)
{
	unsigned char success_failure_logicID[2] = {'\0','\0'};
	unsigned char dataLengthChar = '\0';
	unsigned char returnedData[12];
	memset(returnedData, '\0', sizeof returnedData);
	
	unsigned int logicID, dataLengthInt;
	
	
	while(keepGoing)
	{
		read( fd_, &success_failure_logicID, 2 );
		if ( success_failure_logicID[0] != '\0' )	continue;
		
		//implied else
		logicID = (unsigned int) success_failure_logicID[1];
		
		read( fd_, &dataLengthChar, 1 );
		dataLengthInt = (unsigned int) dataLengthChar;
		
		read(fd_, &returnedData, dataLengthInt ); //pitch,yaw,roll
		
		endian_swap_32( (unsigned int&) returnedData[0] ); //pitch
		endian_swap_32( (unsigned int&) returnedData[8] ); //roll
		
		pthread_mutex_lock( &orient[logicID].mutex );
		
		memcpy(&orient[logicID].pitch, &returnedData[0], sizeof( float ) );
		memcpy(&orient[logicID].roll , &returnedData[8], sizeof( float ) );
		
		pthread_mutex_unlock( &orient[logicID].mutex );
	}
}
