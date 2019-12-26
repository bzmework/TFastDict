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
	// ����������Դ
	//
	void FinalRelease()
	{
		m_dic->Destory();
		delete m_dic;
	}

// IFastDict
public:

	//����
	STDMETHOD(AboutMe)();

	//����/����ָ��������Ŀ
	STDMETHOD(putref_Item)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* pItem);
    STDMETHOD(put_Item)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* pItem);
    STDMETHOD(get_Item)(/*[in]*/ VARIANT* Key, /*[out, retval]*/ VARIANT* pRetItem);

    //���ֵ������һ���¼�����
	STDMETHOD(Add)(/*[in]*/ VARIANT* Key, /*[in]*/ VARIANT* Item, /*[out, retval]*/ VARIANT_BOOL* pOk);

    //�����ֵ��е���Ŀ����
	STDMETHOD(get_Count)(/*[out, retval]*/ long* pCount);

	//�ж�ָ���ļ��Ƿ����ֵ���
	STDMETHOD(Exists)(/*[in]*/ VARIANT* Key, /*[out, retval]*/ VARIANT_BOOL* pExists);

	//����һ������,���а����ֵ��е����м�
    STDMETHOD(Keys)(/*[out, retval]*/ VARIANT* pKeysArray);

	//����һ������,���а����ֵ��е�������Ŀ
	STDMETHOD(Items)(/*[out, retval]*/ VARIANT* pItemsArray);

	//����һ������,���а������ֵ��еĲ��ҵ�����Ŀ
	STDMETHOD(Query)(/*[in]*/ VARIANT* pItem, /*[out, retval]*/ VARIANT* pItemsArray);

	//���ֵ���ɾ��ָ���ļ�
	STDMETHOD(Remove)(/*[in]*/ VARIANT* Key);

	//ɾ���ֵ��е�������Ŀ
	STDMETHOD(RemoveAll)();

private:
	
	FastDict *m_dic;
};

#endif //__FASTDICTINT_H_
