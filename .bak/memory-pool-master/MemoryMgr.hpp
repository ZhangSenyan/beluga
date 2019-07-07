#ifndef _MEMORYMGR_HPP
#define _MEMORYMGR_HPP
#include <stdlib.h>
#include <assert.h>

define MAX_MEMORY_SIZE 1024


class MemoryAlloc;
class MemoryBlock{
public:
	MemoryAlloc* _pAlloc;
	MemoryAlloc* _pNext;
	int _nId;
	int _nRef;
	bool _bPool;
};

class MemoryAlloc{

public:
	MemoryAlloc(){
		_pBuf = nullptr;
		_pHeader = nullptr;
		_nSize = 0;
		_nBlockSize = 0;
    }

    ~MemoryAlloc(){
    	if(_pBuf)
    		free(_pBuf);
    }

    void* allocMemory(size_t nSize)
    {
    	if(!_pBuf)
    	{
    		initMemory();
    	}

    	MemoryBlock* pReturn = nullptr;
    	if(nullptr == _pHeader)
    	{
    		pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
    		pReturn->_bPool = false;
    		pReturn->_nId = -1;
    		pReturn->_nRef = 1;
    		pReturn->_pAlloc = nullptr;
    		pReturn->_pNext = nullptr;
    	}else{
    		pReturn = _pHeader;
    		_pHeader = _pHeader->_pNext;
    		assert(0 == pReturn->_nRef);
    		pReturn->_nRef = 1;
    	}

    	return ((char*)pReturn + sizeof(MemoryBlock));
    }

  	void freeMemory(void* pMem)
  	{
  		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
  		assert(1 == pBlock->_nRef);
  		if(--pBlock->_nRef != 0 )
  		{
  			return;
  		}
  		if(pBlock->_bPool)
  		{
  			pBlock->_pNext = _pHeader;
  			_pHeader =  pBlock;
  		}else{
  			free(pBlock);
  		}
  	}  

  	void initMemory()
  	{
  		assert(nullptr == _pBuf);
  		if(_pBuf)
  			return;
  		size_t realSzie = _nSize + sizeof(MemoryBlock);
  		size_t bufSize = realSzie * _nBlockSize;
  		_pBuf = (char*)malloc(bufSize);

  		_pHeader = (MemoryBlock*)_pBuf;
  		_pHeader->_bPool = true;
		_pHeader->_nId = 0;
		_pHeader->_nRef = 0;
		_pHeader->_pAlloc = this;
		_pHeader->_pNext = nullptr;
  	
		MemoryBlock* pTemp = _pHeader;
		for(size_t n =1; n<_nBlockSize; n++)
		{
			MemoryBlock* pTemp2 = (MemoryBlock*)(_pBuf + (n*realSzie));
			pTemp2->_bPool = true;
			pTemp2->_nId = n;
			pTemp2->_nRef = 0;
			pTemp2->_pAlloc = this;
			pTemp2->_pNext = nullptr;
			pTemp->_pNext = pTemp2;
			pTemp = pTemp2;
		}
  	}

protected:
	char* _pBuf;
	MemoryBlock* _pHeader;
	size_t _nSize;
	size_t _nBlockSize;
};

template<size_t nSize, size_t nBlockSize>
class MemoryAlloctor:public MemoryAlloc
{
public:
	MemoryAlloctor()
	{
		const size_t n = sizeof(void*);
		_nSize = (nSize/n)*n + (nSize % n ? n: 0);
		_nBlockSize = nBlockSize;

	}

};


class MemoryMgr{
private:
	MemoryMgr()
	{
		init_szAlloc(0,64, &_mem64);
		init_szAlloc(65,128, &_mem128);
		init_szAlloc(129,256, &_mem256);
		init_szAlloc(257,512, &_mem512);
	}

public:
	static MemoryMgr& Instance()
	{
		static MemoryMgr mgr;
		return mgr;
	}

	void* allocMem(size_t nSize)
	{
		if(nSize <= MAX_MEMORY_SIZE)
		{
			return _szAlloc[nSize]->allocMemory(nSize);
		}else{
			MemoryBlock* pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
	    	pReturn->_bPool = false;
    		pReturn->_nId = -1;
    		pReturn->_nRef = 1;
    		pReturn->_pAlloc = nullptr;
    		pReturn->_pNext = nullptr;		
    		return ((char*)pReturn + sizeof(MemoryBlock));
		}
	}

	void freeMem(void* pMem)
	{
  		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
  		assert(1 == pBlock->_nRef);
  		if(pBlock->_bPool)
  		{
  			pBlock->_pAlloc->freeMemory(pMem);
  		}else{
  			if(--pBlock->_nRef == 0)
  			free(pBlock);
  		}

	}

	void addRef(void* pMem)
	{
		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
		++pBlock->_nRef;
	}

private:
	void init_szAlloc(int nBegin, int nEnd, MemoryAlloc* pMem)
	{
		for(int n = nBegin; n<= nEnd; n++)
		{
			_szAlloc[n] = pMem;
		}
	}
private:
	MemoryAlloctor<64,100000> _mem64;
	MemoryAlloctor<128,100000> _mem128;
	MemoryAlloctor<256,100000> _mem256;
	MemoryAlloctor<512,100000> _mem512;

	MemoryAlloc* _szAlloc[MAX_MEMORY_SIZE + 1];

};


#endif