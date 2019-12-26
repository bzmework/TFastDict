// FastDict.cpp : Implementation of CSkin
#include "stdafx.h"
#include "TFastDict.h"
#include "TFastDictInt.h"

/////////////////////////////////////////////////////////////////////////////
// CFastDict

//关于
STDMETHODIMP CFastDict::AboutMe()
{
	::MessageBox(0, ABOUTME, COPYRIGHT, MB_ICONWARNING | MB_OK);
	return S_OK;
}
   
//设置指定键的项目(对象)
STDMETHODIMP CFastDict::putref_Item(VARIANT *key, VARIANT *item)
{
    //FIXME("(%p)->(%p %p)\n", this, key, item);

    return E_NOTIMPL;
}

//设置指定键的项目(值)
STDMETHODIMP CFastDict::put_Item(VARIANT *key, VARIANT *item)
{
	m_dic->Add(key, item);
    return S_OK;
}


//返回指定键的项目
STDMETHODIMP CFastDict::get_Item(VARIANT *key, VARIANT *item)
{
	bool bExists = m_dic->GetValue(key, item);
	return S_OK;
}

//向字典中添加一个新键和项
STDMETHODIMP CFastDict::Add(VARIANT *key, VARIANT *item, VARIANT_BOOL* pOk)
{
	*pOk = m_dic->Add(key, item)? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

//返回字典中的项目数量
STDMETHODIMP CFastDict::get_Count(LONG *count)
{
    *count = m_dic->Count();
    return S_OK;
}

//判断指定的键是否在字典中
STDMETHODIMP CFastDict::Exists(VARIANT *key, VARIANT_BOOL *exists)
{
    if (!exists)
        return CTL_E_ILLEGALFUNCTIONCALL;

    *exists = m_dic->ContainsKey(key) != NULL ? VARIANT_TRUE : VARIANT_FALSE;

    return S_OK;
}

//返回一个数组,其中包含字典中的所有键
STDMETHODIMP CFastDict::Keys(VARIANT *keys)
{
    return m_dic->Keys(keys);
}


//返回一个数组,其中包含字典中的所有项目
STDMETHODIMP CFastDict::Items(VARIANT *items)
{
    return m_dic->Values(items);
}

//返回一个数组,其中包含从字典中的查找到的项目
STDMETHODIMP CFastDict::Query(VARIANT* pItem, VARIANT* pItemsArray)
{
	return m_dic->Query(pItem, pItemsArray);
}

//从字典中删除指定的键
STDMETHODIMP CFastDict::Remove(VARIANT *key)
{
	m_dic->Remove(key);
    return S_OK;
}


//删除字典中的所有项目
STDMETHODIMP CFastDict::RemoveAll()
{
    m_dic->RemoveAll();
    return S_OK;
}

//枚举
//STDMETHODIMP CFastDict::__NewEnum(IUnknown **ret)
//{
//    return create_dict_enum(this, ret);
//}
