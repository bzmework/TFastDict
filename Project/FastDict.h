
/*
	模块：字典实现
	作者：denglf
	日期：2019-04-24
	说明：本代码改写自微软.NET Frameworks源码。
	微软的字典算法采用空间换时间的方法，所以很快，处理大量数据时非常有用。
*/

#ifndef __FASTDICT_H_
#define __FASTDICT_H_

#include <malloc.h>
#include "HashHelpers.h"
#include "VMemAlloc.h"

class FastDict
{
private:
	
	//字典项目,微软VB6字典称为键值对keyitem_pair
	//占用4+4+16*2=40字节
    struct dicitem
    {
        int32_t hash;		// 哈希码, -1表示项目已经被删除
        int32_t next;		// 下一个项目的索引, -1表示当前已经是最后一个项目
        VARIANT	key;		// 项目key
        VARIANT value;		// 项目value
    };
	
	int32_t *m_buckets;		// 所有储槽
	dicitem	*m_items;		// 所有项目

	int32_t	m_capacity;		// 字典的容量（哈希表的元素总数，即储槽总数）
	int32_t	m_count;		// 字典中项目的数量
	int32_t m_delIndex;		// 被删除项目的索引
	int32_t	m_delCount;		// 被删除项目的数量
	
	VMemAlloc m_mem;		// 内存管理

public:

	FastDict()
	{
		m_buckets = NULL;
		m_items = NULL;
		m_capacity = 0;
		m_count = 0;
		m_delIndex = 0;
		m_delCount = 0;
		this->Initialize(DefaultHashTable);
	}

	//capacity: 字典容量
	FastDict(int32_t capacity)
    {
		m_buckets = NULL;
		m_items = NULL;
		m_capacity = 0;
		m_count = 0;
		m_delIndex = 0;
		m_delCount = 0;

        if (capacity < 0)
        {
			capacity = DefaultHashTable;
        }
            
        if (capacity > 0)
        {
            this->Initialize(capacity);
        }
    }

	//销毁所有资源
	virtual ~FastDict()
	{
		this->Destory();
	}

	//增加键值
	bool Add(VARIANT* key, VARIANT* value)
    {

		//获得hashcode
		int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &操作是将负数转换成正数

		//根据hashcode获得储槽位置
		int32_t targetBucket = hash % m_capacity;

		//从储槽中查询每个项目:
		//如果当前储槽首次插入,由于其为未使用(-1),因此不会进入循环;
		//如果当前储槽中已经有项目，则进入循环
		if (m_buckets[targetBucket] >= 0)
		{
			for (int32_t i = m_buckets[targetBucket]; i >= 0; i = m_items[i].next)
			{
				//校验项目的hash是否一致，比较项目的key是否相同
				//if (m_items[i].hash == hash && KeyEqual(key, &m_items[i].key))
				if (m_items[i].hash == hash && (VarCmp(key, &m_items[i].key, 0) == 1))
				{
					//key存在的话将其值替换
					VariantCopyInd(&m_items[i].value, value);
					//返回不用再新增
					return true;
				}
			}
		}

		int32_t index = 0;
		if (m_delCount > 0)
		{
			//从删除项目中回收一个
			//由于在删除时并没有释放空间，因此空间可反复利用，也加快了插入的速度
			index = m_delIndex;//最后一个删除项目的索引
			m_delIndex = m_items[index].next;//指向下一个项目的索引
			m_delCount--;//删除数量减少
		}
		else
		{
			//如果项目数量已经等于项目总数(即items已经被填满)，则必须对buckets和items扩容
			//count为当前插入的项目总数，items.Length为分配的数量（=储槽的数量）
			//如果首次插入由于count=0，items.Length>0，因此不会进入，只有当插入的数量已经填满所有items时才会进入
			if (m_count == m_capacity)
			{
				//扩容，通过扩容可以看出，如果在新建字典时，指定了capacity的大小，则会大大地减少扩容的次数
				if (this->Resize())
				{
					//扩容成功，重新定位到新的储槽，原来hash % m_capacity计算出来的储槽位置已经不正确
					targetBucket = hash % m_capacity;
				}
				else
				{
					//扩容失败，报内存不足错。
					return false;
				}
			}

			//记录最后一次增加/删除后项目的总数，首次增加时count=0
			index = m_count;

			//将项目总数+1
			m_count++;
		}

		//必须初始化键值
		VariantInit(&m_items[index].key);
		VariantInit(&m_items[index].value);

		//将键值拷贝进来
		VariantCopyInd(&m_items[index].key, key);
		VariantCopyInd(&m_items[index].value, value);

		//设置hashcode
		m_items[index].hash = hash;

		//将下一个(next)项目，设置成当前储槽中的项目索引,
		//这样，当储槽发生碰撞的时候，上次关联的项目索引，就成了当前项目的下一个项目，
		//就能够正确地轮循储槽上的所有项目
		m_items[index].next = m_buckets[targetBucket];
		
		//现在将项目的索引位置记录在储槽中，便于根据存储找到该项目
		m_buckets[targetBucket] = index;

		//返回
		return true;
    }

	//删除项目
    bool Remove(VARIANT* key)
    {

        if (m_buckets != NULL)
        {

			//获得hashcode
			int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &操作是将负数转换成正数

			//根据hashcode获得储槽位置
			int32_t bucket = hash % m_capacity;

			//删除
			if (m_buckets[bucket] >= 0)
			{
				int last = -1;
				for (int32_t i = m_buckets[bucket]; i >= 0; last = i, i = m_items[i].next)
				{
					if ((m_items[i].hash == hash) && KeyEqual(key, &m_items[i].key))
					//if ((m_items[i].hash == hash) && (VarCmp(key, &m_items[i].key, 0) == 1))
					{
						//将各个删除的项目串联起来，便于Add的时候回收再利用
						if (last < 0)
						{
							m_buckets[bucket] = m_items[i].next;
						}
						else
						{
							m_items[last].next = m_items[i].next;
						}

						//注意: 在删除时并没有释放空间
						m_items[i].hash = -1;//重置
						m_items[i].next = m_delIndex;//指向下一个项目
						VariantClear(&m_items[i].key);//清除键
						VariantClear(&m_items[i].value);//清除值
						m_delIndex = i; //指向最后一个项目
						m_delCount++;
						return true;
					}
				}
			}
        }

        return false;
    }

	//删除所有项目
    void RemoveAll()
    {
        if (m_count > 0)
        {
            //重置所有储槽
			for (int32_t i = 0; i < m_capacity; i++)
			{
				m_buckets[i] = -1;
			}
			//清除项目数据
			for (int32_t i = 0; i < m_count; i++)
			{
				VariantClear(&m_items[i].key);
				VariantClear(&m_items[i].value);
			}
			memset(m_items, -1, sizeof(dicitem) * m_count);

            m_delIndex = -1;
			m_delCount = 0;
            m_count = 0;
        }
    }

	//获取值
    bool GetValue(VARIANT* key, VARIANT* value)
    {
        //查找value
        int32_t i = FindItem(key);
		if (i >= 0)
		{
			VariantCopy(value, &m_items[i].value); //将值拷贝出去
			return true;
		}

        //没有找到返回缺省值
		VariantInit(value);
        return false;
    }

	//key是否存在
    bool ContainsKey(VARIANT* key)
    {
        int32_t i = FindItem(key);
        if (i >= 0)
        {
            return true;
        }
        return false;
    }

	//值是否存在
    bool ContainsValue(VARIANT* value)
    {
		if (value == NULL)
		{
			for (int32_t i = 0; i < m_count; i++)
			{
				if (m_items[i].hash >= 0 && (V_VT(&m_items[i].value) == VT_EMPTY))
				{
					return true;
				}
			}
		}
		else
		{
			for (int32_t i = 0; i < m_count; i++)
			{
				if (m_items[i].hash >= 0 && ValueMatch(&m_items[i].value, value))
				{
					return true;
				}
			}
		}
		return false;
    }

	//返回所有Keys
	HRESULT Keys(VARIANT* keys)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//创建数组
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}

		//校验数组
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//取出键，放入数组中
		for (int32_t i = 0; i < m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				VariantCopy(&v[i], &m_items[i].key);
			}
		}
		SafeArrayUnaccessData(sa);
		
		//返回
		V_VT(keys) = VT_ARRAY|VT_VARIANT;
		V_ARRAY(keys) = sa;

		return S_OK;
	}

	//返回所有Values
	HRESULT Values(VARIANT* values)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//创建数组
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}
		
		//校验数组
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//取出键，放入数组中
		for (int32_t i = 0; i < m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				VariantCopy(&v[i], &m_items[i].value);
			}
		}
		SafeArrayUnaccessData(sa);
		
		//返回
		V_VT(values) = VT_ARRAY|VT_VARIANT;
		V_ARRAY(values) = sa;

		return S_OK;
	}

	//查询值，支持通配符*和?
	HRESULT Query(VARIANT* value, VARIANT* values)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//创建数组
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}
		
		//校验数组
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//取出键，放入数组中
		int32_t count = 0;
		for (int32_t i = 0; i < m_count; i++)
		{

			if (m_items[i].hash >= 0 && ValueMatch(&m_items[i].value, value))
			{
				VariantCopy(&v[count], &m_items[i].value);
				count++;
			}
		}
		SafeArrayUnaccessData(sa);
		
		//缩小数组
		if (count > 0 && count < m_count)
		{
			bound.lLbound = 0;
			bound.cElements = count;
			SafeArrayRedim(sa, &bound);
		}
		
		//返回
		if (count == 0)
		{
			VariantInit(values);
		}
		else
		{
			V_VT(values) = VT_ARRAY | VT_VARIANT;
			V_ARRAY(values) = sa;
		}
		
		return S_OK;
	}

	//返回项目数量
	int32_t Count()
	{
		return m_count;
	}

	//销毁所有项目
	void Destory()
	{
		//清除数据
		RemoveAll();

		//销毁储槽等
		if (m_buckets != NULL)
		{
			m_mem.Free(m_buckets);
			m_buckets = NULL;
		}
		if (m_items != NULL)
		{
			m_mem.Free(m_items);
			m_items = NULL;
		}
		m_capacity = 0;
	}

private:

	//初始化
	__forceinline bool Initialize(int32_t capacity)
    {
   
		//设置字典的容量
        //实际上是设置hashtable的大小，它的大小决定发生碰撞的频率
        //越大发生的频率就越低，但相应的占用内存就会增加
        //设置容量的诀窍是，根据capacity从prime表中查找一个大于capacity的最小素数
		int32_t size = HashHelpers::GetPrime(capacity);

		//先分配大的
		//创建容纳键值的项目s为hashtable的大小，并初始化所有项目
		//dicitem* newItems = new dicitem[size];
		//dicitem* newItems = (dicitem*)malloc(sizeof(dicitem) * size);
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * size);
		if (newItems == NULL)
		{
			return false; //内存分配失败
		}
		else
		{
			for (int32_t i = 0; i < size; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				VariantInit(&newItems[i].key);
				VariantInit(&newItems[i].value);
			}
		}

        //创建储槽s(即hashtable)，并初始化所有储槽为未使用(-1)状态
		//int32_t* newBuckets = new int32_t[size]; //new的实质是malloc
		//int32_t* newBuckets = (int32_t*)malloc(sizeof(int32_t) * size);
		int32_t* newBuckets = (int32_t*)m_mem.Alloc(sizeof(int32_t) * size);
		if (newBuckets == NULL)
		{
			//释放newItems占用的内存防止内存泄漏
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return false; //内存分配失败
		}
		else
		{
			for (int32_t i = 0; i < size; i++)
			{
				newBuckets[i] = -1;
			}
		}
        
		//指向buckets和items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//初始化其它信息
		this->m_delIndex = -1;		
		this->m_delCount = 0;	
		this->m_count = 0;

		//必须保存字典长度
		this->m_capacity = size;

		//返回
		return true;
    }

	//扩容
	//括多少？括大于2*m_count的最小素数个
	__forceinline bool Resize()
	{
		//新扩容量
		int32_t newSize = GetCapacity();

		//数据量大的先扩容
		//创建新的项目s(数量=newBuckets.Length)，初始化所有项目。
		//微软的字典算法设计虽然比其它的快，但其设计思想是用空间换时间，下面
		//千万级数据: （50000000 * 40）/ 1048576 ＝1907 MB。在内存不够充足的情况下，100%分配内存会失败。
		//dicitem *newItems = new dicitem[newSize];
		//dicitem *newItems = (dicitem*)malloc(sizeof(dicitem) * newSize);
		dicitem *newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * newSize);
		if (newItems == NULL)
		{
			return false; //内存分配失败
		}
		else
		{
			//初始化新项目
			for (int32_t i = 0; i < newSize; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				VariantInit(&newItems[i].key);
				VariantInit(&newItems[i].value);
			}
		}

		//创建新的储槽s，并初始化为未使用
		int32_t *newBuckets = (int32_t*)m_mem.Alloc(sizeof(int32_t) * newSize);
		if (newBuckets == NULL)
		{
			//释放newItems占用的内存防止内存泄漏
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return false; //内存分配失败
		}
		else
		{
			//重新初始化
			for (int32_t i = 0; i < newSize; i++)
			{
				newBuckets[i] = -1;
			}
		}

		//现在将所有旧的项目拷贝其到新建项目，
		memcpy(newItems, m_items, sizeof(dicitem) * m_count);

		//然后重新将项目(entry)与储槽(进行关联)，为什么要这么做？
		//因为hashcode % newSize（或hashcode % newBuckets.Length）以后，已经不是原来的储槽了，
		//所以必须重新进行关联。
		for (int32_t i = 0; i < m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32_t bucket = newItems[i].hash % newSize;//重新计算bucket

				//设置下一个项目
				//当储槽发生碰撞（即不同key通过hash运算后得到了同一储槽）的时候,
				//如果储槽上已经关联了项目的索引，此时就将已关联项目作为新增项目的下一个项目，
				//正确的应该是成为当前新增项目的上一个项目，即使顺序被打乱了但这并不重要，
				//因为根据m_items数组的索引始终都能枚举出顺序增加的每一个项目。
				if (newBuckets[bucket] >= 0) //已发生关联(即发生碰撞)
				{
					newItems[i].next = newBuckets[bucket];//成为当前项目的下一个项目
				}
				else //无碰撞
				{
					newItems[i].next = -1; //已经是最后一个项目
				}

				//将当前项目索引与新的bucket储槽关联，便于通过bucket能快速找到自己
				newBuckets[bucket] = i;
			}
		}

		//释放原先占用的内存,防止内存泄漏
		m_mem.Free(m_buckets);
		m_mem.Free(m_items);
		
		//指向新的buckets和items
		m_buckets = newBuckets;
		m_items = newItems;

		//必须保存字典长度
		m_capacity = newSize;

		//返回
		return true;
	}

	/*
	//上面屏蔽代码的缺点是一次申请的内存可能太大很有可能失败，例如：
	//千万级数据: （50000000 * 40）/ 1048576 ＝1907 MB。
	//在内存不够充足的情况下，malloc 90%会失败
	__forceinline void Resize()
    {
		//新扩容容量
		int32_t newSize = HashHelpers::ExpandPrime(m_count);

		//创建新的储槽s，并初始化为未使用
		int32_t *newBuckets = (int32_t*)realloc(m_buckets, sizeof(int32_t) * newSize);//扩容 
		for (int32_t i = m_count; i < newSize; i++)
		{
			newBuckets[i] = -1;
		}

		//创建新的项目s(数量=newBuckets.Length)，并初始化所有项目。
		//由于结构dicitem的尺寸=40字节，数据上升到千万级以后，内存分配不了
		//很奇怪.net平台上却能new成功，为什么？难道.net使用了内存池？
		//如果考虑到内存因素，微软的这种设计不是最好的
		dicitem *newItems = (dicitem*)realloc(m_items, sizeof(dicitem) * newSize);//扩容 
		for (int32_t i = m_count; i < newSize; i++)
		{
			newItems[i].hash = -1;
			newItems[i].next = -1;
			VariantInit(&newItems[i].key);
			VariantInit(&newItems[i].value);
		}
		
		//重新将项目(entry)与储槽(进行关联)，为什么要这么做？
		//因为hashcode % newSize（或hashcode % newBuckets.Length）以后，已经不是原来的储槽了
		//所以必须重新进行关联
		for (int32_t i = 0; i < m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32_t bucket = newItems[i].hash % newSize;//取得储槽
				newBuckets[bucket] = i;//修改新储槽的状态为已使用(>=0)(其实设置成1可读性更好)
				newItems[i].next = -1;//将下一个(next)项目，设置成未使用状态(-1)。因为扩容后，每个储槽和项目都能容纳未扩容前的所有项目，因此设置成未使用状态(-1)
			}
		}

		//指向新的buckets和items
		m_buckets = newBuckets;
		m_items = newItems;

		//必须保存字典长度
		m_len = newSize;
    }
	*/

	//查找项目, 找到返回项目的索引,否则返回-1
    __forceinline int32_t FindItem(VARIANT* key)
    {

		//获得hashcode
		int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &操作是将负数转换成正数

		//根据hashcode获得储槽位置
		int32_t bucket = hash % m_capacity;

        //取得储槽的位置i，如果储槽存在(i >= 0)，则开始在项目中搜索项目
		for (int32_t i = m_buckets[bucket]; i >= 0; i = m_items[i].next)
		{
			//数字只检查hash, 但字符要进行比较
			//if ((m_items[i].hash == hash) && KeyEqual(key, &m_items[i].key)) 
			if ((m_items[i].hash == hash) && (VarCmp(key, &m_items[i].key, 0) == 1))
			{
				return i;
			}
		}
        
        return -1;//没有找到
    }

	//键是否相等
	__forceinline bool KeyEqual(VARIANT* key1, VARIANT* key2)
	{
		//key是否字符串
		bool bStrKey1 = (V_VT(key1) == VT_BSTR) || (V_VT(key1) == (VT_BSTR|VT_BYREF));
		bool bStrKey2 = (V_VT(key2) == VT_BSTR) || (V_VT(key2) == (VT_BSTR|VT_BYREF));

		//比较键是否相等
		if (bStrKey1 && bStrKey2) 
		{
			// 转换成WCHAR Only for VT_BSTR or VT_BSTR|VT_BYREF types
			WCHAR *strKey1 = NULL;
			if (V_VT(key1) == VT_BSTR)
				strKey1 = V_BSTR(key1);
			else if (V_BSTRREF(key1))
				strKey1 = *V_BSTRREF(key1);

			WCHAR *strKey2 = NULL;
			if (V_VT(key2) == VT_BSTR)
				strKey2 = V_BSTR(key2);
			else if (V_BSTRREF(key2))
				strKey2 = *V_BSTRREF(key2);

			return strcmpW(strKey1, strKey2) == 0;
		}
		
		//键类型不一致
		if ((bStrKey1 && !bStrKey2) || (!bStrKey1 && bStrKey2))
		{
			return false;
		}

		//其它情况返回1，交给其它比较函数处理
		return true;
	}
	
	//值是否匹配
	__forceinline bool ValueMatch(VARIANT* value1, VARIANT* value2)
	{
		BSTR str1, str2;
		CStr(value1, &str1);
		CStr(value2, &str2);

		return strmatchW(str1, str2);
	}

    //宽字符串比较
	//Compare S1 and S2, returning less than, equal to or
	//greater than zero if S1 is lexicographically less than,
	//equal to or greater than S2.  
	//比较S1和S2，如果S1在字典上小于、等于或大于S2，则返回小于、等于或大于零。
	__forceinline int32_t strcmpW(const WCHAR* str1, const WCHAR* str2)
	{
		/*	
		int32 ret = 0;
		while (!(ret = *(WCHAR*) str1 - *(WCHAR*) str2) && *str2) 
		  ++str1, ++str2;

		if (ret < 0)
			ret = -1;
		else if (ret > 0)
			ret = 1 ;

		return ret;
		*/

		register const WCHAR* s1 = (const WCHAR*) str1;
		register const WCHAR* s2 = (const WCHAR*) str2;
		register WCHAR c1, c2;

		do
		{
			c1 = (WCHAR) *s1++;
			c2 = (WCHAR) *s2++;
			if (c1 == 0/*NUL*/)
				return c1 - c2;
		}
		while (c1 == c2);

		return c1 - c2;
	}

	//宽字符串匹配
	__forceinline bool strmatchW(const WCHAR* text, const WCHAR* filter)
	{
		if(text == NULL || filter == NULL)
		{
			return false;
		}

		const WCHAR* cp = NULL;
		const WCHAR* mp = NULL;
		while((*text) && (*filter != _T('*')))
		{
			if ((*filter != *text) && (*filter != _T('?'))) return false;
			filter++;
			text++;
		}

		while (*text) 
		{
			if (*filter == _T('*')) 
			{
				if (!*++filter) return true;
				mp = filter;
				cp = text + 1;
			} 
			else if ((*filter == *text) || (*filter == _T('?'))) 
			{
				filter++;
				text++;
			}
			else 
			{
				filter = mp;
				text = cp++;
			}
		}

		while (*filter == _T('*')) 
		{
			filter++;
		}

		return !*filter;
	}

	//将变体转换成字符串
	__forceinline bool CStr(VARIANT *v, BSTR *ret)
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

	//获得一个有效的增长容量
	__forceinline int32_t GetCapacity()
	{
		//取得新扩字节数
		int32_t newCapacity = HashHelpers::ExpandPrime(m_capacity);//新扩容量
		int32_t blocks = sizeof(int32_t) + sizeof(dicitem); //每个储槽和项目占用的字节数
		DWORDLONG newSize = blocks * (DWORDLONG)newCapacity;//字节数

		//取得内存可用空间
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //字节数
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //字节数

		//已经超出了可用的虚拟内存，但还没有超过可用的物理内存，按可用虚拟内存的50%申请内存
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//还可以申请的容量
			int32_t capacity = (int32_t)((ullAvailVirtual * 0.5)/ blocks) ;
			//重新计算新括容量(获得一个>=m_capacity 的最小素数)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > m_capacity)
			{
				newCapacity = capacity;
			}
		}

		//返回
		return newCapacity;
	}
};

#endif //__FASTDICT_H_

