#include<stdlib.h>
#include"Alloctor.h"

int main()
{
	char* data[128];
	for(size_t i = 0; i < 128; i++)
	{
		data[i] =  new char[1 + i];
	}

	for(size_t i = 0 ; i < 128; i++)
	{
		delete[] data[i];
	}


	return 0;
}