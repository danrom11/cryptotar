#include <iostream>
#include <string>
#include "vigenereCryptoModule.hpp"



class crypto : public cryptoModule{

public:
	void* cryptoData(unsigned char* data, size_t size) override 
	{
		for(size_t i = 0; i < size; i++)
		{
			data[i]= (data[i] + key[i % sizeKey]) % 256;
		}

		return data;
	
	}

	void* uncryptoData(unsigned char* data, size_t size) override
	{		
		for (size_t i = 0; i < size; i++)
		{
			data[i] = (data[i] - key[i % sizeKey] + 256) % 256;
		}

		return data;

	}
};

cryptoModule* makechild(){
	return new crypto();
}

