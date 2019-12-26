
/*
	ģ�飺�ֵ�ʵ��
	���ߣ�denglf
	���ڣ�2019-04-24
	˵�����������д��΢��.NET FrameworksԴ�롣
	΢����ֵ��㷨���ÿռ任ʱ��ķ��������Ժܿ죬�����������ʱ�ǳ����á�
*/

#ifndef __FASTDICT_H_
#define __FASTDICT_H_

#include <malloc.h>
#include "HashHelpers.h"
#include "VMemAlloc.h"

class FastDict
{
private:
	
	//�ֵ���Ŀ,΢��VB6�ֵ��Ϊ��ֵ��keyitem_pair
	//ռ��4+4+16*2=40�ֽ�
    struct dicitem
    {
        int32_t hash;		// ��ϣ��, -1��ʾ��Ŀ�Ѿ���ɾ��
        int32_t next;		// ��һ����Ŀ������, -1��ʾ��ǰ�Ѿ������һ����Ŀ
        VARIANT	key;		// ��Ŀkey
        VARIANT value;		// ��Ŀvalue
    };
	
	int32_t *m_buckets;		// ���д���
	dicitem	*m_items;		// ������Ŀ

	int32_t	m_capacity;		// �ֵ����������ϣ���Ԫ��������������������
	int32_t	m_count;		// �ֵ�����Ŀ������
	int32_t m_delIndex;		// ��ɾ����Ŀ������
	int32_t	m_delCount;		// ��ɾ����Ŀ������
	
	VMemAlloc m_mem;		// �ڴ����

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

	//capacity: �ֵ�����
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

	//����������Դ
	virtual ~FastDict()
	{
		this->Destory();
	}

	//���Ӽ�ֵ
	bool Add(VARIANT* key, VARIANT* value)
    {

		//���hashcode
		int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &�����ǽ�����ת��������

		//����hashcode��ô���λ��
		int32_t targetBucket = hash % m_capacity;

		//�Ӵ����в�ѯÿ����Ŀ:
		//�����ǰ�����״β���,������Ϊδʹ��(-1),��˲������ѭ��;
		//�����ǰ�������Ѿ�����Ŀ�������ѭ��
		if (m_buckets[targetBucket] >= 0)
		{
			for (int32_t i = m_buckets[targetBucket]; i >= 0; i = m_items[i].next)
			{
				//У����Ŀ��hash�Ƿ�һ�£��Ƚ���Ŀ��key�Ƿ���ͬ
				//if (m_items[i].hash == hash && KeyEqual(key, &m_items[i].key))
				if (m_items[i].hash == hash && (VarCmp(key, &m_items[i].key, 0) == 1))
				{
					//key���ڵĻ�����ֵ�滻
					VariantCopyInd(&m_items[i].value, value);
					//���ز���������
					return true;
				}
			}
		}

		int32_t index = 0;
		if (m_delCount > 0)
		{
			//��ɾ����Ŀ�л���һ��
			//������ɾ��ʱ��û���ͷſռ䣬��˿ռ�ɷ������ã�Ҳ�ӿ��˲�����ٶ�
			index = m_delIndex;//���һ��ɾ����Ŀ������
			m_delIndex = m_items[index].next;//ָ����һ����Ŀ������
			m_delCount--;//ɾ����������
		}
		else
		{
			//�����Ŀ�����Ѿ�������Ŀ����(��items�Ѿ�������)��������buckets��items����
			//countΪ��ǰ�������Ŀ������items.LengthΪ�����������=���۵�������
			//����״β�������count=0��items.Length>0����˲�����룬ֻ�е�����������Ѿ���������itemsʱ�Ż����
			if (m_count == m_capacity)
			{
				//���ݣ�ͨ�����ݿ��Կ�����������½��ֵ�ʱ��ָ����capacity�Ĵ�С�������ؼ������ݵĴ���
				if (this->Resize())
				{
					//���ݳɹ������¶�λ���µĴ��ۣ�ԭ��hash % m_capacity��������Ĵ���λ���Ѿ�����ȷ
					targetBucket = hash % m_capacity;
				}
				else
				{
					//����ʧ�ܣ����ڴ治���
					return false;
				}
			}

			//��¼���һ������/ɾ������Ŀ���������״�����ʱcount=0
			index = m_count;

			//����Ŀ����+1
			m_count++;
		}

		//�����ʼ����ֵ
		VariantInit(&m_items[index].key);
		VariantInit(&m_items[index].value);

		//����ֵ��������
		VariantCopyInd(&m_items[index].key, key);
		VariantCopyInd(&m_items[index].value, value);

		//����hashcode
		m_items[index].hash = hash;

		//����һ��(next)��Ŀ�����óɵ�ǰ�����е���Ŀ����,
		//�����������۷�����ײ��ʱ���ϴι�������Ŀ�������ͳ��˵�ǰ��Ŀ����һ����Ŀ��
		//���ܹ���ȷ����ѭ�����ϵ�������Ŀ
		m_items[index].next = m_buckets[targetBucket];
		
		//���ڽ���Ŀ������λ�ü�¼�ڴ����У����ڸ��ݴ洢�ҵ�����Ŀ
		m_buckets[targetBucket] = index;

		//����
		return true;
    }

	//ɾ����Ŀ
    bool Remove(VARIANT* key)
    {

        if (m_buckets != NULL)
        {

			//���hashcode
			int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &�����ǽ�����ת��������

			//����hashcode��ô���λ��
			int32_t bucket = hash % m_capacity;

			//ɾ��
			if (m_buckets[bucket] >= 0)
			{
				int last = -1;
				for (int32_t i = m_buckets[bucket]; i >= 0; last = i, i = m_items[i].next)
				{
					if ((m_items[i].hash == hash) && KeyEqual(key, &m_items[i].key))
					//if ((m_items[i].hash == hash) && (VarCmp(key, &m_items[i].key, 0) == 1))
					{
						//������ɾ������Ŀ��������������Add��ʱ�����������
						if (last < 0)
						{
							m_buckets[bucket] = m_items[i].next;
						}
						else
						{
							m_items[last].next = m_items[i].next;
						}

						//ע��: ��ɾ��ʱ��û���ͷſռ�
						m_items[i].hash = -1;//����
						m_items[i].next = m_delIndex;//ָ����һ����Ŀ
						VariantClear(&m_items[i].key);//�����
						VariantClear(&m_items[i].value);//���ֵ
						m_delIndex = i; //ָ�����һ����Ŀ
						m_delCount++;
						return true;
					}
				}
			}
        }

        return false;
    }

	//ɾ��������Ŀ
    void RemoveAll()
    {
        if (m_count > 0)
        {
            //�������д���
			for (int32_t i = 0; i < m_capacity; i++)
			{
				m_buckets[i] = -1;
			}
			//�����Ŀ����
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

	//��ȡֵ
    bool GetValue(VARIANT* key, VARIANT* value)
    {
        //����value
        int32_t i = FindItem(key);
		if (i >= 0)
		{
			VariantCopy(value, &m_items[i].value); //��ֵ������ȥ
			return true;
		}

        //û���ҵ�����ȱʡֵ
		VariantInit(value);
        return false;
    }

	//key�Ƿ����
    bool ContainsKey(VARIANT* key)
    {
        int32_t i = FindItem(key);
        if (i >= 0)
        {
            return true;
        }
        return false;
    }

	//ֵ�Ƿ����
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

	//��������Keys
	HRESULT Keys(VARIANT* keys)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//��������
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}

		//У������
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//ȡ����������������
		for (int32_t i = 0; i < m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				VariantCopy(&v[i], &m_items[i].key);
			}
		}
		SafeArrayUnaccessData(sa);
		
		//����
		V_VT(keys) = VT_ARRAY|VT_VARIANT;
		V_ARRAY(keys) = sa;

		return S_OK;
	}

	//��������Values
	HRESULT Values(VARIANT* values)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//��������
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}
		
		//У������
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//ȡ����������������
		for (int32_t i = 0; i < m_count; i++)
		{
			if (m_items[i].hash >= 0)
			{
				VariantCopy(&v[i], &m_items[i].value);
			}
		}
		SafeArrayUnaccessData(sa);
		
		//����
		V_VT(values) = VT_ARRAY|VT_VARIANT;
		V_ARRAY(values) = sa;

		return S_OK;
	}

	//��ѯֵ��֧��ͨ���*��?
	HRESULT Query(VARIANT* value, VARIANT* values)
	{
		SAFEARRAYBOUND bound;
		SAFEARRAY *sa;
		VARIANT *v;
		HRESULT hr;

		//��������
		bound.lLbound = 0;
		bound.cElements = m_count;
		sa = SafeArrayCreate(VT_VARIANT, 1, &bound);
		if (!sa)
		{
			return E_OUTOFMEMORY;
		}
		
		//У������
		hr = SafeArrayAccessData(sa, (void**)&v);
		if (FAILED(hr)) 
		{
			SafeArrayDestroy(sa);
			return hr;
		}
		
		//ȡ����������������
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
		
		//��С����
		if (count > 0 && count < m_count)
		{
			bound.lLbound = 0;
			bound.cElements = count;
			SafeArrayRedim(sa, &bound);
		}
		
		//����
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

	//������Ŀ����
	int32_t Count()
	{
		return m_count;
	}

	//����������Ŀ
	void Destory()
	{
		//�������
		RemoveAll();

		//���ٴ��۵�
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

	//��ʼ��
	__forceinline bool Initialize(int32_t capacity)
    {
   
		//�����ֵ������
        //ʵ����������hashtable�Ĵ�С�����Ĵ�С����������ײ��Ƶ��
        //Խ������Ƶ�ʾ�Խ�ͣ�����Ӧ��ռ���ڴ�ͻ�����
        //���������ľ����ǣ�����capacity��prime���в���һ������capacity����С����
		int32_t size = HashHelpers::GetPrime(capacity);

		//�ȷ�����
		//�������ɼ�ֵ����ĿsΪhashtable�Ĵ�С������ʼ��������Ŀ
		//dicitem* newItems = new dicitem[size];
		//dicitem* newItems = (dicitem*)malloc(sizeof(dicitem) * size);
		dicitem* newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * size);
		if (newItems == NULL)
		{
			return false; //�ڴ����ʧ��
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

        //��������s(��hashtable)������ʼ�����д���Ϊδʹ��(-1)״̬
		//int32_t* newBuckets = new int32_t[size]; //new��ʵ����malloc
		//int32_t* newBuckets = (int32_t*)malloc(sizeof(int32_t) * size);
		int32_t* newBuckets = (int32_t*)m_mem.Alloc(sizeof(int32_t) * size);
		if (newBuckets == NULL)
		{
			//�ͷ�newItemsռ�õ��ڴ��ֹ�ڴ�й©
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return false; //�ڴ����ʧ��
		}
		else
		{
			for (int32_t i = 0; i < size; i++)
			{
				newBuckets[i] = -1;
			}
		}
        
		//ָ��buckets��items
		this->m_buckets = newBuckets;
		this->m_items = newItems;

		//��ʼ��������Ϣ
		this->m_delIndex = -1;		
		this->m_delCount = 0;	
		this->m_count = 0;

		//���뱣���ֵ䳤��
		this->m_capacity = size;

		//����
		return true;
    }

	//����
	//�����٣�������2*m_count����С������
	__forceinline bool Resize()
	{
		//��������
		int32_t newSize = GetCapacity();

		//���������������
		//�����µ���Ŀs(����=newBuckets.Length)����ʼ��������Ŀ��
		//΢����ֵ��㷨�����Ȼ�������Ŀ죬�������˼�����ÿռ任ʱ�䣬����
		//ǧ������: ��50000000 * 40��/ 1048576 ��1907 MB�����ڴ治�����������£�100%�����ڴ��ʧ�ܡ�
		//dicitem *newItems = new dicitem[newSize];
		//dicitem *newItems = (dicitem*)malloc(sizeof(dicitem) * newSize);
		dicitem *newItems = (dicitem*)m_mem.Alloc(sizeof(dicitem) * newSize);
		if (newItems == NULL)
		{
			return false; //�ڴ����ʧ��
		}
		else
		{
			//��ʼ������Ŀ
			for (int32_t i = 0; i < newSize; i++)
			{
				newItems[i].hash = -1;
				newItems[i].next = -1;
				VariantInit(&newItems[i].key);
				VariantInit(&newItems[i].value);
			}
		}

		//�����µĴ���s������ʼ��Ϊδʹ��
		int32_t *newBuckets = (int32_t*)m_mem.Alloc(sizeof(int32_t) * newSize);
		if (newBuckets == NULL)
		{
			//�ͷ�newItemsռ�õ��ڴ��ֹ�ڴ�й©
			if (newItems != NULL)
			{
				m_mem.Free(newItems);
			}
			return false; //�ڴ����ʧ��
		}
		else
		{
			//���³�ʼ��
			for (int32_t i = 0; i < newSize; i++)
			{
				newBuckets[i] = -1;
			}
		}

		//���ڽ����оɵ���Ŀ�����䵽�½���Ŀ��
		memcpy(newItems, m_items, sizeof(dicitem) * m_count);

		//Ȼ�����½���Ŀ(entry)�봢��(���й���)��ΪʲôҪ��ô����
		//��Ϊhashcode % newSize����hashcode % newBuckets.Length���Ժ��Ѿ�����ԭ���Ĵ����ˣ�
		//���Ա������½��й�����
		for (int32_t i = 0; i < m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32_t bucket = newItems[i].hash % newSize;//���¼���bucket

				//������һ����Ŀ
				//�����۷�����ײ������ͬkeyͨ��hash�����õ���ͬһ���ۣ���ʱ��,
				//����������Ѿ���������Ŀ����������ʱ�ͽ��ѹ�����Ŀ��Ϊ������Ŀ����һ����Ŀ��
				//��ȷ��Ӧ���ǳ�Ϊ��ǰ������Ŀ����һ����Ŀ����ʹ˳�򱻴����˵��Ⲣ����Ҫ��
				//��Ϊ����m_items���������ʼ�ն���ö�ٳ�˳�����ӵ�ÿһ����Ŀ��
				if (newBuckets[bucket] >= 0) //�ѷ�������(��������ײ)
				{
					newItems[i].next = newBuckets[bucket];//��Ϊ��ǰ��Ŀ����һ����Ŀ
				}
				else //����ײ
				{
					newItems[i].next = -1; //�Ѿ������һ����Ŀ
				}

				//����ǰ��Ŀ�������µ�bucket���۹���������ͨ��bucket�ܿ����ҵ��Լ�
				newBuckets[bucket] = i;
			}
		}

		//�ͷ�ԭ��ռ�õ��ڴ�,��ֹ�ڴ�й©
		m_mem.Free(m_buckets);
		m_mem.Free(m_items);
		
		//ָ���µ�buckets��items
		m_buckets = newBuckets;
		m_items = newItems;

		//���뱣���ֵ䳤��
		m_capacity = newSize;

		//����
		return true;
	}

	/*
	//�������δ����ȱ����һ��������ڴ����̫����п���ʧ�ܣ����磺
	//ǧ������: ��50000000 * 40��/ 1048576 ��1907 MB��
	//���ڴ治�����������£�malloc 90%��ʧ��
	__forceinline void Resize()
    {
		//����������
		int32_t newSize = HashHelpers::ExpandPrime(m_count);

		//�����µĴ���s������ʼ��Ϊδʹ��
		int32_t *newBuckets = (int32_t*)realloc(m_buckets, sizeof(int32_t) * newSize);//���� 
		for (int32_t i = m_count; i < newSize; i++)
		{
			newBuckets[i] = -1;
		}

		//�����µ���Ŀs(����=newBuckets.Length)������ʼ��������Ŀ��
		//���ڽṹdicitem�ĳߴ�=40�ֽڣ�����������ǧ���Ժ��ڴ���䲻��
		//�����.netƽ̨��ȴ��new�ɹ���Ϊʲô���ѵ�.netʹ�����ڴ�أ�
		//������ǵ��ڴ����أ�΢���������Ʋ�����õ�
		dicitem *newItems = (dicitem*)realloc(m_items, sizeof(dicitem) * newSize);//���� 
		for (int32_t i = m_count; i < newSize; i++)
		{
			newItems[i].hash = -1;
			newItems[i].next = -1;
			VariantInit(&newItems[i].key);
			VariantInit(&newItems[i].value);
		}
		
		//���½���Ŀ(entry)�봢��(���й���)��ΪʲôҪ��ô����
		//��Ϊhashcode % newSize����hashcode % newBuckets.Length���Ժ��Ѿ�����ԭ���Ĵ�����
		//���Ա������½��й���
		for (int32_t i = 0; i < m_count; i++)
		{
			if (newItems[i].hash >= 0)
			{
				int32_t bucket = newItems[i].hash % newSize;//ȡ�ô���
				newBuckets[bucket] = i;//�޸��´��۵�״̬Ϊ��ʹ��(>=0)(��ʵ���ó�1�ɶ��Ը���)
				newItems[i].next = -1;//����һ��(next)��Ŀ�����ó�δʹ��״̬(-1)����Ϊ���ݺ�ÿ�����ۺ���Ŀ��������δ����ǰ��������Ŀ��������ó�δʹ��״̬(-1)
			}
		}

		//ָ���µ�buckets��items
		m_buckets = newBuckets;
		m_items = newItems;

		//���뱣���ֵ䳤��
		m_len = newSize;
    }
	*/

	//������Ŀ, �ҵ�������Ŀ������,���򷵻�-1
    __forceinline int32_t FindItem(VARIANT* key)
    {

		//���hashcode
		int32_t hash = HashHelpers::GetHashCode(key) & 0x7FFFFFFF;//0x7FFFFFFF=2147483647 &�����ǽ�����ת��������

		//����hashcode��ô���λ��
		int32_t bucket = hash % m_capacity;

        //ȡ�ô��۵�λ��i��������۴���(i >= 0)����ʼ����Ŀ��������Ŀ
		for (int32_t i = m_buckets[bucket]; i >= 0; i = m_items[i].next)
		{
			//����ֻ���hash, ���ַ�Ҫ���бȽ�
			//if ((m_items[i].hash == hash) && KeyEqual(key, &m_items[i].key)) 
			if ((m_items[i].hash == hash) && (VarCmp(key, &m_items[i].key, 0) == 1))
			{
				return i;
			}
		}
        
        return -1;//û���ҵ�
    }

	//���Ƿ����
	__forceinline bool KeyEqual(VARIANT* key1, VARIANT* key2)
	{
		//key�Ƿ��ַ���
		bool bStrKey1 = (V_VT(key1) == VT_BSTR) || (V_VT(key1) == (VT_BSTR|VT_BYREF));
		bool bStrKey2 = (V_VT(key2) == VT_BSTR) || (V_VT(key2) == (VT_BSTR|VT_BYREF));

		//�Ƚϼ��Ƿ����
		if (bStrKey1 && bStrKey2) 
		{
			// ת����WCHAR Only for VT_BSTR or VT_BSTR|VT_BYREF types
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
		
		//�����Ͳ�һ��
		if ((bStrKey1 && !bStrKey2) || (!bStrKey1 && bStrKey2))
		{
			return false;
		}

		//�����������1�����������ȽϺ�������
		return true;
	}
	
	//ֵ�Ƿ�ƥ��
	__forceinline bool ValueMatch(VARIANT* value1, VARIANT* value2)
	{
		BSTR str1, str2;
		CStr(value1, &str1);
		CStr(value2, &str2);

		return strmatchW(str1, str2);
	}

    //���ַ����Ƚ�
	//Compare S1 and S2, returning less than, equal to or
	//greater than zero if S1 is lexicographically less than,
	//equal to or greater than S2.  
	//�Ƚ�S1��S2�����S1���ֵ���С�ڡ����ڻ����S2���򷵻�С�ڡ����ڻ�����㡣
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

	//���ַ���ƥ��
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

	//������ת�����ַ���
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

	//���һ����Ч����������
	__forceinline int32_t GetCapacity()
	{
		//ȡ�������ֽ���
		int32_t newCapacity = HashHelpers::ExpandPrime(m_capacity);//��������
		int32_t blocks = sizeof(int32_t) + sizeof(dicitem); //ÿ�����ۺ���Ŀռ�õ��ֽ���
		DWORDLONG newSize = blocks * (DWORDLONG)newCapacity;//�ֽ���

		//ȡ���ڴ���ÿռ�
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&ms);
		DWORDLONG ullAvailPhys = ms.ullAvailPhys; //�ֽ���
		DWORDLONG ullAvailVirtual = ms.ullAvailVirtual; //�ֽ���

		//�Ѿ������˿��õ������ڴ棬����û�г������õ������ڴ棬�����������ڴ��50%�����ڴ�
		if (newSize >= ullAvailVirtual && newSize < ullAvailPhys)
		{
			//���������������
			int32_t capacity = (int32_t)((ullAvailVirtual * 0.5)/ blocks) ;
			//���¼�����������(���һ��>=m_capacity ����С����)
			capacity = HashHelpers::CalcPrime(capacity);
			if (capacity > m_capacity)
			{
				newCapacity = capacity;
			}
		}

		//����
		return newCapacity;
	}
};

#endif //__FASTDICT_H_

