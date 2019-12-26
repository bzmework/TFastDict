
/*
	模块：哈希表(HashTable)帮助
	作者：denglf
	日期：2019-04-24
	说明：本代码参考自微软.NET Frameworks源码
*/

#ifndef __HASHHELPERS_H_
#define __HASHHELPERS_H_

#include <math.h>
#include <windows.h>
#include <tchar.h>

//#define tolowerW(n) towlower((n))
//#define toupperW(n) towupper((n))
//#define strcmpW(s1,s2) wcscmp((s1),(s2))
//#define strcmpiW(s1,s2) _wcsicmp((s1),(s2))

//类型定义
#if defined(_MSC_VER) && (_MSC_VER < 1600)
	typedef signed char        int8_t;
	typedef short              int16_t;
	typedef int                int32_t;
	typedef long long          int64_t;
	typedef unsigned char      uint8_t;
	typedef unsigned short     uint16_t;
	typedef unsigned int       uint32_t;
	typedef unsigned long long uint64_t;
	typedef unsigned __int64   uint64_t;
#else
	#include <stdint.h>
#endif

//素数表
// Table of prime numbers to use as hash table sizes. 
// 用作哈希表大小的素数表。

// A typical resize algorithm would pick the smallest prime number in this array
// that is larger than twice the previous capacity. 
// 一个典型的调整大小算法会选择这个数组中最小的素数这比之前容量的两倍还要大。

// Suppose our Hashtable currently has capacity x and enough elements are added 
// such that a resize needs to occur. Resizing first computes 2x then finds the 
// first prime in the table greater than 2x, i.e. if primes are ordered 
// p_1, p_2, ..., p_i, ..., it finds p_n such that p_n-1 < 2x < p_n. 
// 假设我们的Hashtable当前具有容量x，并且添加了足够的元素因此需要进行大小调整。
// 调整大小首先计算2x，然后找到表中大于2x的第一个素数，即如果素数是有序的
// p_1、p_2……,p_i,…，它发现p_n使得p_n-1 < 2x < p_n。

// Doubling is important for preserving the asymptotic complexity of the 
// hashtable operations such as add.  Having a prime guarantees that double 
// hashing does not lead to infinite loops.  IE, your hash function will be 
// h1(key) + i*h2(key), 0 <= i < size.  h2 and the size must be relatively prime.
//对偶对于保持函数的渐近复杂度很重要
//哈希表操作(如add.)的质数保证了该值的两倍哈希不会导致无限循环。
//也就是说，你的哈希函数是：h1(key) + i*h2(key)， 0 <= i < size。h2的大小必须是质数。

//说明：为什么HashTable要保持素数大小增长？
//因为经过研究，采用素数，能在[0, buckets-1]区间取得均匀分布的bucket
//注：bucket指HashTable上的储槽, 故为了形象化，在字典设计中不再称HashTable而改称buckets
static int32_t primes[] = 
{
	3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
	1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 
	21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
	187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
	1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
};

//HashTable最大值: Int32.MaxValue = 0x7FEFFFFD(2147483647)
//21亿,已经足够大了,如果想要更大,则为int64_t了
static int32_t MaxHashTable = 0x7FEFFFFD;

//缺省HashTable容量: 百万数据级
//素数设置参考表:
//-------------------------------------------------------------------------------------------------------------
//项目				十亿级数据		千万级数据		百万级数据		十万级数据		万级数据(微软VB6字典默认值)
//					1亿-9亿			1000万-9999万	100万-999万		10万-99万		0-5万
//BUCKET_COUNT		100000007		10000019		1000003			100043			509
//HASH_MAX_RANGE	999999937		99999989		9999991			999983			1201
//-------------------------------------------------------------------------------------------------------------
//微软VB6字典默认素数设置为万级数据，因此一旦字典要容纳百万级数据后,字典的存取速度就会非常慢,
//因为哈希表(HashTable)碰撞太频繁,每个bucket(储槽)上的数据太多,所以要增大哈希表的容量，降低碰撞率。
static int32_t DefaultHashTable = 3;// 0xF4243;//0xF4243 = 1000003

//缺省的HashTable素数值
static int32_t DefaultHashPrime = 101;

class HashHelpers
{

public:

	//判断一个数是否为素数
	//素数又称质数。指整数在一个大于1的自然数中，除了1和此整数自身外，没法被其他自然数整除的数。
	//换句话说，只有两个正因数(1和自己)的自然数即为素数。
	//比1大但不是素数的数称为合数。
	//1和0既非素数也非合数。
    static __forceinline bool IsPrime(int32_t candidate)
    {
        if ((candidate & 1) != 0)
        {
			//如果是合数，必定是两个数的乘积，这两个数肯定一个大于根号n，一个小于根号n
            int32_t limit = (int32_t)sqrt((float)candidate);

			//如果能整除就不是素数,偶数不再素数范围
            for (int32_t divisor = 3; divisor <= limit; divisor += 2)
            {
                if ((candidate % divisor) == 0)
                    return false;
            }
            return true;
        }

		//2是素数
        return (candidate == 2);
    }

	//获得素数
    static __forceinline int GetPrime(int min)
    {
		//获得数组元素的数量
		int32_t count = (sizeof(primes) / sizeof(primes[0]));

        //未设置(0)，则从primes表中取出一个 >= min的最小素数
        for (int32_t i = 0; i < count; i++)
        {
            int prime = primes[i];
            if (prime >= min) return prime;
        }

        //超出primes的最大值，则运算一个
        //outside of our predefined table. compute the hard way. 
        for (int32_t i = (min | 1); i < MaxHashTable; i += 2)
        {
            if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
                return i;
        }
        return min;
    }

	//计算素数
	static __forceinline int CalcPrime(int min)
	{
		for (int32_t i = (min | 1); i < MaxHashTable; i += 2)
		{
			if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
				return i;
		}
		return min;
	}

	//获取最小的素数
    static __forceinline int32_t GetMinPrime()
    {
        return primes[0];
    }

	// 根据HashTable原先的大小(oldSize)，返回要扩容的HashTable的大小。
    static __forceinline int32_t ExpandPrime(int32_t oldSize)
    {
		
		//扩充成原来的2倍
        int32_t newSize = 2 * oldSize;

		//
        // Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
        // Note that this check works even when _items.Length overflowed thanks to the (uint32_t) cast
		// 允许哈希表在容量溢出之前增长到最大可能的大小(~2G元素)。
		// 注意，这个检查即使在_items时也可以工作。由于(uint32_t)强制转换，长度溢出
        if (newSize > MaxHashTable && MaxHashTable > oldSize)
        {
            return MaxHashTable;//注意: MaxHashTable = int32_t.MaxValue = 2147483647 正好是一个素数
        }

		//返回扩容后HashTable大小(一个素数)
        return GetPrime(newSize);
	}

	//获得hash值
	static __forceinline int32_t GetHashCode(VARIANT* key)
	{
		switch (V_VT(key))
		{
		
		//OLE Automation string
		case VT_BSTR | VT_BYREF:
		case VT_BSTR:
		{
			/* Only for VT_BSTR or VT_BSTR|VT_BYREF types */
			WCHAR* strKey = NULL;
			if (V_VT(key) == VT_BSTR)
				strKey = V_BSTR(key);
			else if (V_BSTRREF(key))
				strKey = *V_BSTRREF(key);

			return get_str_hash(strKey);
		}

		//unsigned char
		case VT_UI1 | VT_BYREF:
		case VT_UI1:
			return get_num_hash(V_VT(key) & VT_BYREF ? *V_UI1REF(key) : V_UI1(key));

		//2 byte signed int
		case VT_I2 | VT_BYREF:
		case VT_I2:
			return  get_num_hash(V_VT(key) & VT_BYREF ? *V_I2REF(key) : V_I2(key));

		//4 byte signed int
		case VT_I4 | VT_BYREF:
		case VT_I4:
		{
			FLOAT numI4 = static_cast<FLOAT>(V_VT(key) & VT_BYREF ? *V_I4REF(key) : V_I4(key));
			return get_num_hash(numI4);
		}
		
		//Object: IUnknown * and IDispatch *
		case VT_UNKNOWN | VT_BYREF:
		case VT_DISPATCH | VT_BYREF:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		{
			IUnknown *src = (V_VT(key) & VT_BYREF) ? *V_UNKNOWNREF(key) : V_UNKNOWN(key);
			IUnknown *unk = NULL;

			if (!src) 
			{
				return 0;
			}

			IUnknown* pT = static_cast<IUnknown*>(src);
			pT->QueryInterface(IID_IUnknown, (void**)&unk);
			if (!unk) 
			{
				return ~0u; //~0u = -1
			}

			int32_t hashVal = get_ptr_hash(unk);
			unk->Release();
			return hashVal;
		}

		//date
		case VT_DATE | VT_BYREF:
		case VT_DATE:
		{
			FLOAT numD = static_cast<FLOAT>(V_VT(key) & VT_BYREF ? *V_DATEREF(key) : V_DATE(key));
			return get_flt_hash(numD);
		}
		
		//4 byte real
		case VT_R4|VT_BYREF:
		case VT_R4:
		{
			FLOAT numR4 = static_cast<FLOAT>(V_VT(key) & VT_BYREF ? *V_R4REF(key) : V_R4(key));
			return get_flt_hash(numR4);
		}
		
		//8 byte real
		case VT_R8 | VT_BYREF:
		case VT_R8:
		{
			FLOAT numR8 = static_cast<FLOAT>(V_VT(key) & VT_BYREF ? *V_R8REF(key) : V_R8(key));
			return get_flt_hash(numR8);
		}
		
		//other
		case VT_INT://signed machine int
		case VT_UINT://unsigned machine int
		case VT_I1://signed char
		case VT_I8://signed 64-bit int
		case VT_UI2://unsigned short
		case VT_UI4://unsigned long
			return ~0u; //~0u = -1

		default:
			return 0;
		}

		return 0;
	}

	/*static __forceinline int32_t GetHashCode(VARIANT* key)
	{
		BSTR str;
		CStr(key, &str);
		int len = SysStringByteLen(str);
		int32_t hash = SuperFastHash(CB2A(str), len);
		return hash % MaxHashTable;
	}*/

private:

	//获得字符类型Hash
	static __forceinline int32_t get_str_hash(const WCHAR *str)
	{
		int32_t hash = 0;

		if (str) 
		{
			
			while (*str) 
			{
				WCHAR ch;

				ch = *str;

				hash += (hash << 6) + ch;
				str++;
			}
		}

		return hash % MaxHashTable;//取模是将hash限定在MaxHashTable范围
	}

	//获得数字类型Hash
	static __forceinline int32_t get_num_hash(float num)
	{
		return (*((int32_t*)&num)) % MaxHashTable;
	}

	//获得地址类型Hash
	static __forceinline int32_t get_ptr_hash(void *ptr)
	{
		return PtrToUlong(ptr) % MaxHashTable;
	}

	//获得浮点类型Hash
	static __forceinline int32_t get_flt_hash(float flt)
	{
		if (isinf(flt)) 
		{
			return 0;
		}
		else if (!isnan(flt)) 
		{
			return get_num_hash(flt);
		}

		/* NaN case */
		return  ~0u;
	}

	//判断数组的元素是否没有边界
	static __forceinline int isinf (double d) 
	{
		int expon = 0;
		double val = frexp(d, &expon);
		if (expon == 1025) {
			if (val == 0.5) {
				return 1;
			} else if (val == -0.5) {
				return -1;
			} else {
				return 0;
			}
		} 
		else {
			return 0;
		}
	}

	static __forceinline int isnan(double d) 
	{
		int expon = 0;
		double val = frexp(d, &expon);
		if (expon == 1025) {
			if (val == 0.5) {
				return 0;
			} else if (val == -0.5) {
				return 0;
			} else {
				return 1;
			}
		} 
		else {
			return 0;
		}
	}

	//将变体转换成字符串
	static __forceinline bool CStr(VARIANT *v, BSTR *ret)
	{
		VARIANT dst;
		HRESULT hres;

		V_VT(&dst) = VT_EMPTY;
		hres = VariantChangeType(&dst, v, VARIANT_LOCALBOOL, VT_BSTR);
		if (FAILED(hres))
			return false;

		*ret = V_BSTR(&dst);
		return true;
	}

	//Convert char * to BSTR
	static __forceinline BSTR CA2B(const char* pSrc)
	{
		if (!pSrc) return NULL;

		DWORD cwch;

		BSTR wsOut = NULL;

		if (cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc, -1, NULL, 0))//get size minus NULL terminator
		{
			cwch--;
			wsOut = ::SysAllocStringLen(NULL, cwch);

			if (wsOut)
			{
				if (!::MultiByteToWideChar(CP_ACP, 0, pSrc, -1, wsOut, cwch))
				{
					if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
						return wsOut;
					::SysFreeString(wsOut);//must clean up
					wsOut = NULL;
				}
			}

		};

		return wsOut;
	};

	//Convert BSTR to char *
	static __forceinline char* CB2A(BSTR pSrc)
	{
		if (!pSrc) return NULL;

		DWORD cb, cwch = ::SysStringLen(pSrc);//convert even embeded NULL

		char *szOut = NULL;

		if (cb = ::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, NULL, 0, 0, 0))
		{
			szOut = new char[cb];
			if (szOut)
			{
				szOut[cb - 1] = '\0';

				if (!::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, szOut, cb, 0, 0))
				{
					delete[] szOut;//clean up if failed;
					szOut = NULL;
				}
			}
		}

		return szOut;
	};

};

#endif //__ATL_HASHHELPERS_H_
