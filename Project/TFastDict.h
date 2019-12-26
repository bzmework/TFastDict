

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for TFastDict.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TFastDict_h__
#define __TFastDict_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFastDict_FWD_DEFINED__
#define __IFastDict_FWD_DEFINED__
typedef interface IFastDict IFastDict;

#endif 	/* __IFastDict_FWD_DEFINED__ */


#ifndef __FastDict_FWD_DEFINED__
#define __FastDict_FWD_DEFINED__

#ifdef __cplusplus
typedef class FastDict FastDict;
#else
typedef struct FastDict FastDict;
#endif /* __cplusplus */

#endif 	/* __FastDict_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IFastDict_INTERFACE_DEFINED__
#define __IFastDict_INTERFACE_DEFINED__

/* interface IFastDict */
/* [object][unique][oleautomation][dual][hidden][helpstring][uuid] */ 


EXTERN_C const IID IID_IFastDict;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3491C1D-CDD4-498a-955E-85AD8D957CF3")
    IFastDict : public IDispatch
    {
    public:
        virtual /* [helpstring][propputref][id] */ HRESULT STDMETHODCALLTYPE putref_Item( 
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *pRetItem) = 0;
        
        virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Item( 
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *pRetItem) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *Key,
            /* [retval][out] */ VARIANT *pRetItem) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AboutMe( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *Item,
            /* [retval][out] */ VARIANT_BOOL *pOk) = 0;
        
        virtual /* [helpstring][propget][id] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Exists( 
            /* [in] */ VARIANT *Key,
            /* [retval][out] */ VARIANT_BOOL *pExists) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Keys( 
            /* [retval][out] */ VARIANT *pKeysArray) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Items( 
            /* [retval][out] */ VARIANT *pItemsArray) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Query( 
            /* [in] */ VARIANT *pItem,
            /* [retval][out] */ VARIANT *pItemsArray) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Remove( 
            /* [in] */ VARIANT *Key) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAll( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFastDictVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFastDict * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFastDict * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFastDict * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFastDict * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFastDict * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFastDict * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFastDict * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][propputref][id] */ HRESULT ( STDMETHODCALLTYPE *putref_Item )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *pRetItem);
        
        /* [helpstring][propput][id] */ HRESULT ( STDMETHODCALLTYPE *put_Item )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *pRetItem);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key,
            /* [retval][out] */ VARIANT *pRetItem);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AboutMe )( 
            IFastDict * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key,
            /* [in] */ VARIANT *Item,
            /* [retval][out] */ VARIANT_BOOL *pOk);
        
        /* [helpstring][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IFastDict * This,
            /* [retval][out] */ long *pCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Exists )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key,
            /* [retval][out] */ VARIANT_BOOL *pExists);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Keys )( 
            IFastDict * This,
            /* [retval][out] */ VARIANT *pKeysArray);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Items )( 
            IFastDict * This,
            /* [retval][out] */ VARIANT *pItemsArray);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Query )( 
            IFastDict * This,
            /* [in] */ VARIANT *pItem,
            /* [retval][out] */ VARIANT *pItemsArray);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Remove )( 
            IFastDict * This,
            /* [in] */ VARIANT *Key);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAll )( 
            IFastDict * This);
        
        END_INTERFACE
    } IFastDictVtbl;

    interface IFastDict
    {
        CONST_VTBL struct IFastDictVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFastDict_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFastDict_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFastDict_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFastDict_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFastDict_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFastDict_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFastDict_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IFastDict_putref_Item(This,Key,pRetItem)	\
    ( (This)->lpVtbl -> putref_Item(This,Key,pRetItem) ) 

#define IFastDict_put_Item(This,Key,pRetItem)	\
    ( (This)->lpVtbl -> put_Item(This,Key,pRetItem) ) 

#define IFastDict_get_Item(This,Key,pRetItem)	\
    ( (This)->lpVtbl -> get_Item(This,Key,pRetItem) ) 

#define IFastDict_AboutMe(This)	\
    ( (This)->lpVtbl -> AboutMe(This) ) 

#define IFastDict_Add(This,Key,Item,pOk)	\
    ( (This)->lpVtbl -> Add(This,Key,Item,pOk) ) 

#define IFastDict_get_Count(This,pCount)	\
    ( (This)->lpVtbl -> get_Count(This,pCount) ) 

#define IFastDict_Exists(This,Key,pExists)	\
    ( (This)->lpVtbl -> Exists(This,Key,pExists) ) 

#define IFastDict_Keys(This,pKeysArray)	\
    ( (This)->lpVtbl -> Keys(This,pKeysArray) ) 

#define IFastDict_Items(This,pItemsArray)	\
    ( (This)->lpVtbl -> Items(This,pItemsArray) ) 

#define IFastDict_Query(This,pItem,pItemsArray)	\
    ( (This)->lpVtbl -> Query(This,pItem,pItemsArray) ) 

#define IFastDict_Remove(This,Key)	\
    ( (This)->lpVtbl -> Remove(This,Key) ) 

#define IFastDict_RemoveAll(This)	\
    ( (This)->lpVtbl -> RemoveAll(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFastDict_INTERFACE_DEFINED__ */



#ifndef __TFastDict_LIBRARY_DEFINED__
#define __TFastDict_LIBRARY_DEFINED__

/* library TFastDict */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TFastDict;

EXTERN_C const CLSID CLSID_FastDict;

#ifdef __cplusplus

class DECLSPEC_UUID("A60EB914-B33D-42fc-A830-2D56A4342227")
FastDict;
#endif
#endif /* __TFastDict_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

unsigned long             __RPC_USER  VARIANT_UserSize64(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal64(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal64(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree64(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


