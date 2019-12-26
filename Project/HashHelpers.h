
/*
	ģ�飺��ϣ��(HashTable)����
	���ߣ�denglf
	���ڣ�2019-04-24
	˵����������ο���΢��.NET FrameworksԴ��
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

//���Ͷ���
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

//������
// Table of prime numbers to use as hash table sizes. 
// ������ϣ���С��������

// A typical resize algorithm would pick the smallest prime number in this array
// that is larger than twice the previous capacity. 
// һ�����͵ĵ�����С�㷨��ѡ�������������С���������֮ǰ������������Ҫ��

// Suppose our Hashtable currently has capacity x and enough elements are added 
// such that a resize needs to occur. Resizing first computes 2x then finds the 
// first prime in the table greater than 2x, i.e. if primes are ordered 
// p_1, p_2, ..., p_i, ..., it finds p_n such that p_n-1 < 2x < p_n. 
// �������ǵ�Hashtable��ǰ��������x������������㹻��Ԫ�������Ҫ���д�С������
// ������С���ȼ���2x��Ȼ���ҵ����д���2x�ĵ�һ������������������������
// p_1��p_2����,p_i,����������p_nʹ��p_n-1 < 2x < p_n��

// Doubling is important for preserving the asymptotic complexity of the 
// hashtable operations such as add.  Having a prime guarantees that double 
// hashing does not lead to infinite loops.  IE, your hash function will be 
// h1(key) + i*h2(key), 0 <= i < size.  h2 and the size must be relatively prime.
//��ż���ڱ��ֺ����Ľ������ӶȺ���Ҫ
//��ϣ�����(��add.)��������֤�˸�ֵ��������ϣ���ᵼ������ѭ����
//Ҳ����˵����Ĺ�ϣ�����ǣ�h1(key) + i*h2(key)�� 0 <= i < size��h2�Ĵ�С������������

//˵����ΪʲôHashTableҪ����������С������
//��Ϊ�����о�����������������[0, buckets-1]����ȡ�þ��ȷֲ���bucket
//ע��bucketָHashTable�ϵĴ���, ��Ϊ�����󻯣����ֵ�����в��ٳ�HashTable���ĳ�buckets
static int32_t primes[] = 
{
	3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
	1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591, 17519, 
	21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
	187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
	1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
};

//HashTable���ֵ: Int32.MaxValue = 0x7FEFFFFD(2147483647)
//21��,�Ѿ��㹻����,�����Ҫ����,��Ϊint64_t��
static int32_t MaxHashTable = 0x7FEFFFFD;

//ȱʡHashTable����: �������ݼ�
//�������òο���:
//-------------------------------------------------------------------------------------------------------------
//��Ŀ				ʮ�ڼ�����		ǧ������		��������		ʮ������		������(΢��VB6�ֵ�Ĭ��ֵ)
//					1��-9��			1000��-9999��	100��-999��		10��-99��		0-5��
//BUCKET_COUNT		100000007		10000019		1000003			100043			509
//HASH_MAX_RANGE	999999937		99999989		9999991			999983			1201
//-------------------------------------------------------------------------------------------------------------
//΢��VB6�ֵ�Ĭ����������Ϊ�����ݣ����һ���ֵ�Ҫ���ɰ������ݺ�,�ֵ�Ĵ�ȡ�ٶȾͻ�ǳ���,
//��Ϊ��ϣ��(HashTable)��ײ̫Ƶ��,ÿ��bucket(����)�ϵ�����̫��,����Ҫ�����ϣ���������������ײ�ʡ�
static int32_t DefaultHashTable = 3;// 0xF4243;//0xF4243 = 1000003

//ȱʡ��HashTable����ֵ
static int32_t DefaultHashPrime = 101;

class HashHelpers
{

public:

	//�ж�һ�����Ƿ�Ϊ����
	//�����ֳ�������ָ������һ������1����Ȼ���У�����1�ʹ����������⣬û����������Ȼ������������
	//���仰˵��ֻ������������(1���Լ�)����Ȼ����Ϊ������
	//��1�󵫲�������������Ϊ������
	//1��0�ȷ�����Ҳ�Ǻ�����
    static __forceinline bool IsPrime(int32_t candidate)
    {
        if ((candidate & 1) != 0)
        {
			//����Ǻ������ض����������ĳ˻������������϶�һ�����ڸ���n��һ��С�ڸ���n
            int32_t limit = (int32_t)sqrt((float)candidate);

			//����������Ͳ�������,ż������������Χ
            for (int32_t divisor = 3; divisor <= limit; divisor += 2)
            {
                if ((candidate % divisor) == 0)
                    return false;
            }
            return true;
        }

		//2������
        return (candidate == 2);
    }

	//�������
    static __forceinline int GetPrime(int min)
    {
		//�������Ԫ�ص�����
		int32_t count = (sizeof(primes) / sizeof(primes[0]));

        //δ����(0)�����primes����ȡ��һ�� >= min����С����
        for (int32_t i = 0; i < count; i++)
        {
            int prime = primes[i];
            if (prime >= min) return prime;
        }

        //����primes�����ֵ��������һ��
        //outside of our predefined table. compute the hard way. 
        for (int32_t i = (min | 1); i < MaxHashTable; i += 2)
        {
            if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
                return i;
        }
        return min;
    }

	//��������
	static __forceinline int CalcPrime(int min)
	{
		for (int32_t i = (min | 1); i < MaxHashTable; i += 2)
		{
			if (IsPrime(i) && ((i - 1) % DefaultHashPrime != 0))
				return i;
		}
		return min;
	}

	//��ȡ��С������
    static __forceinline int32_t GetMinPrime()
    {
        return primes[0];
    }

	// ����HashTableԭ�ȵĴ�С(oldSize)������Ҫ���ݵ�HashTable�Ĵ�С��
    static __forceinline int32_t ExpandPrime(int32_t oldSize)
    {
		
		//�����ԭ����2��
        int32_t newSize = 2 * oldSize;

		//
        // Allow the hashtables to grow to maximum possible size (~2G elements) before encoutering capacity overflow.
        // Note that this check works even when _items.Length overflowed thanks to the (uint32_t) cast
		// �����ϣ�����������֮ǰ�����������ܵĴ�С(~2GԪ��)��
		// ע�⣬�����鼴ʹ��_itemsʱҲ���Թ���������(uint32_t)ǿ��ת�����������
        if (newSize > MaxHashTable && MaxHashTable > oldSize)
        {
            return MaxHashTable;//ע��: MaxHashTable = int32_t.MaxValue = 2147483647 ������һ������
        }

		//�������ݺ�HashTable��С(һ������)
        return GetPrime(newSize);
	}

	//���hashֵ
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

	//����ַ�����Hash
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

		return hash % MaxHashTable;//ȡģ�ǽ�hash�޶���MaxHashTable��Χ
	}

	//�����������Hash
	static __forceinline int32_t get_num_hash(float num)
	{
		return (*((int32_t*)&num)) % MaxHashTable;
	}

	//��õ�ַ����Hash
	static __forceinline int32_t get_ptr_hash(void *ptr)
	{
		return PtrToUlong(ptr) % MaxHashTable;
	}

	//��ø�������Hash
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

	//�ж������Ԫ���Ƿ�û�б߽�
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

	//������ת�����ַ���
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
