// TFastDictInt.h : Declaration of the CFastDict

#ifndef __FASTDICTINT_H_
#define __FASTDICTINT_H_

#include "stdafx.h"
#include "resource.h"       // main symbols
#include "fastdict.h"

/////////////////////////////////////////////////////////////////////////////
// CFastDict
class ATL_NO_VTABLE CFastDict : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFastDict, &CLSID_FastDict>,
	public IDispatchImpl<IFastDict, &IID_IFastDict, &LIBID_TFastDict>
{
public:
	CFastDict()
	{
		m_dic = new FastDict();
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_FastDict)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CFastDict)
		COM_INTERFACE_ENTRY(IFastDict)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	//
	// 销毁所有资源
	//
	void FinalRelease()
	{
		m_dic->Destory();
		delete m_dic;
	}

// IFastDict
public:

	//关于
	STDMETHOD(AboutMe)();

	//返回/设置指定键的项目
	STDMETHOD(putref_Item)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* pItem);
    STDMETHOD(put_Item)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* pItem);
    STDMETHOD(get_Item)(/*[in]*/ VARIANT* Key, /*[out, retval]*/ VARIANT* pRetItem);

    //向字典中添加一个新键和项
	STDMETHOD(Add)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* Item, /*[out, retval]*/ VARIANT_BOOL* pOk);

    //返回字典中的项目数量
	STDMETHOD(get_Count)(/*[out, retval]*/ long* pCount);

	//判断指定的键是否在字典中
	STDMETHOD(Exists)(/*[in]*/ VARIANT* Key, /*[out, retval]*/ VARIANT_BOOL* pExists);

	//返回一个数组,其中包含字典中的所有键
    STDMETHOD(Keys)(/*[out, retval]*/ VARIANT* pKeysArray);

	//返回一个数组,其中包含字典中的所有项目
	STDMETHOD(Items)(/*[out, retval]*/ VARIANT* pItemsArray);

	//返回一个数组,其中包含从字典中的查找到的项目
	STDMETHOD(Query)(/*[in]*/ VARIANT* pItem, /*[out, retval]*/ VARIANT* pItemsArray);

	//从字典中删除指定的键
	STDMETHOD(Remove)(/*[in]*/ VARIANT* Key);

	//删除字典中的所有项目
	STDMETHOD(RemoveAll)();

private:
	
	FastDict *m_dic;
};

#endif //__FASTDICTINT_H_
