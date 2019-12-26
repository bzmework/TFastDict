// FastDict.cpp : Implementation of CSkin
#include "stdafx.h"
#include "TFastDict.h"
#include "TFastDictInt.h"

/////////////////////////////////////////////////////////////////////////////
// CFastDict

//����
STDMETHODIMP CFastDict::AboutMe()
{
	::MessageBox(0, ABOUTME, COPYRIGHT, MB_ICONWARNING | MB_OK);
	return S_OK;
}
   
//����ָ��������Ŀ(����)
STDMETHODIMP CFastDict::putref_Item(VARIANT *key, VARIANT *item)
{
    //FIXME("(%p)->(%p %p)\n", this, key, item);

    return E_NOTIMPL;
}

//����ָ��������Ŀ(ֵ)
STDMETHODIMP CFastDict::put_Item(VARIANT *key, VARIANT *item)
{
	m_dic->Add(key, item);
    return S_OK;
}


//����ָ��������Ŀ
STDMETHODIMP CFastDict::get_Item(VARIANT *key, VARIANT *item)
{
	bool bExists = m_dic->GetValue(key, item);
	return S_OK;
}

//���ֵ������һ���¼�����
STDMETHODIMP CFastDict::Add(VARIANT *key, VARIANT *item, VARIANT_BOOL* pOk)
{
	*pOk = m_dic->Add(key, item)? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

//�����ֵ��е���Ŀ����
STDMETHODIMP CFastDict::get_Count(LONG *count)
{
    *count = m_dic->Count();
    return S_OK;
}

//�ж�ָ���ļ��Ƿ����ֵ���
STDMETHODIMP CFastDict::Exists(VARIANT *key, VARIANT_BOOL *exists)
{
    if (!exists)
        return CTL_E_ILLEGALFUNCTIONCALL;

    *exists = m_dic->ContainsKey(key) != NULL ? VARIANT_TRUE : VARIANT_FALSE;

    return S_OK;
}

//����һ������,���а����ֵ��е����м�
STDMETHODIMP CFastDict::Keys(VARIANT *keys)
{
    return m_dic->Keys(keys);
}


//����һ������,���а����ֵ��е�������Ŀ
STDMETHODIMP CFastDict::Items(VARIANT *items)
{
    return m_dic->Values(items);
}

//����һ������,���а������ֵ��еĲ��ҵ�����Ŀ
STDMETHODIMP CFastDict::Query(VARIANT* pItem, VARIANT* pItemsArray)
{
	return m_dic->Query(pItem, pItemsArray);
}

//���ֵ���ɾ��ָ���ļ�
STDMETHODIMP CFastDict::Remove(VARIANT *key)
{
	m_dic->Remove(key);
    return S_OK;
}


//ɾ���ֵ��е�������Ŀ
STDMETHODIMP CFastDict::RemoveAll()
{
    m_dic->RemoveAll();
    return S_OK;
}

//ö��
//STDMETHODIMP CFastDict::__NewEnum(IUnknown **ret)
//{
//    return create_dict_enum(this, ret);
//}
