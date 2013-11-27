

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Nov 27 14:40:44 2013
 */
/* Compiler settings for .\LDAPQueryLib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __LDAPQueryLib_i_h__
#define __LDAPQueryLib_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILDAPQuery_FWD_DEFINED__
#define __ILDAPQuery_FWD_DEFINED__
typedef interface ILDAPQuery ILDAPQuery;
#endif 	/* __ILDAPQuery_FWD_DEFINED__ */


#ifndef __LDAPQuery_FWD_DEFINED__
#define __LDAPQuery_FWD_DEFINED__

#ifdef __cplusplus
typedef class LDAPQuery LDAPQuery;
#else
typedef struct LDAPQuery LDAPQuery;
#endif /* __cplusplus */

#endif 	/* __LDAPQuery_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ILDAPQuery_INTERFACE_DEFINED__
#define __ILDAPQuery_INTERFACE_DEFINED__

/* interface ILDAPQuery */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILDAPQuery;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0CE2E92E-70E7-4682-9E4F-13C1C0D43435")
    ILDAPQuery : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE execQuery( 
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ BSTR host,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [in] */ VARIANT_BOOL usessl,
            /* [retval][out] */ SAFEARRAY * *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getLastError( 
            /* [retval][out] */ LONG *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE connect( 
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ BSTR host,
            /* [in] */ VARIANT_BOOL usessl,
            /* [retval][out] */ LONG *connect_id) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE close( 
            /* [in] */ LONG connect_id) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE query( 
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [retval][out] */ SAFEARRAY * *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE pageQuery( 
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [in] */ ULONG total_results,
            /* [retval][out] */ LONG *results_id) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE nextResults( 
            /* [in] */ LONG results_id,
            /* [in] */ ULONG num_results,
            /* [retval][out] */ SAFEARRAY * *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE rename( 
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR dn,
            /* [in] */ BSTR newRDN,
            /* [in] */ BSTR newParent,
            /* [in] */ BOOL deleteOldRdn) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE dn2ufn( 
            /* [in] */ BSTR dn,
            /* [retval][out] */ BSTR *ufn) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ufn2dn( 
            /* [in] */ BSTR ufn,
            /* [retval][out] */ BSTR *dn) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE err2string( 
            /* [in] */ ULONG ufn,
            /* [retval][out] */ BSTR *string) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE explodeDn( 
            /* [in] */ BSTR str,
            /* [in] */ ULONG notypes,
            /* [retval][out] */ SAFEARRAY * *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILDAPQueryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILDAPQuery * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILDAPQuery * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILDAPQuery * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILDAPQuery * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILDAPQuery * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILDAPQuery * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILDAPQuery * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *execQuery )( 
            ILDAPQuery * This,
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ BSTR host,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [in] */ VARIANT_BOOL usessl,
            /* [retval][out] */ SAFEARRAY * *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getLastError )( 
            ILDAPQuery * This,
            /* [retval][out] */ LONG *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *connect )( 
            ILDAPQuery * This,
            /* [in] */ BSTR username,
            /* [in] */ BSTR password,
            /* [in] */ BSTR host,
            /* [in] */ VARIANT_BOOL usessl,
            /* [retval][out] */ LONG *connect_id);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            ILDAPQuery * This,
            /* [in] */ LONG connect_id);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *query )( 
            ILDAPQuery * This,
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [retval][out] */ SAFEARRAY * *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *pageQuery )( 
            ILDAPQuery * This,
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR basedn,
            /* [in] */ BSTR scope,
            /* [in] */ BSTR filter,
            /* [in] */ BSTR attributes,
            /* [in] */ ULONG total_results,
            /* [retval][out] */ LONG *results_id);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *nextResults )( 
            ILDAPQuery * This,
            /* [in] */ LONG results_id,
            /* [in] */ ULONG num_results,
            /* [retval][out] */ SAFEARRAY * *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *rename )( 
            ILDAPQuery * This,
            /* [in] */ LONG connect_id,
            /* [in] */ BSTR dn,
            /* [in] */ BSTR newRDN,
            /* [in] */ BSTR newParent,
            /* [in] */ BOOL deleteOldRdn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *dn2ufn )( 
            ILDAPQuery * This,
            /* [in] */ BSTR dn,
            /* [retval][out] */ BSTR *ufn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ufn2dn )( 
            ILDAPQuery * This,
            /* [in] */ BSTR ufn,
            /* [retval][out] */ BSTR *dn);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *err2string )( 
            ILDAPQuery * This,
            /* [in] */ ULONG ufn,
            /* [retval][out] */ BSTR *string);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *explodeDn )( 
            ILDAPQuery * This,
            /* [in] */ BSTR str,
            /* [in] */ ULONG notypes,
            /* [retval][out] */ SAFEARRAY * *result);
        
        END_INTERFACE
    } ILDAPQueryVtbl;

    interface ILDAPQuery
    {
        CONST_VTBL struct ILDAPQueryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILDAPQuery_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILDAPQuery_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILDAPQuery_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILDAPQuery_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILDAPQuery_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILDAPQuery_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILDAPQuery_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILDAPQuery_execQuery(This,username,password,host,basedn,scope,filter,attributes,usessl,result)	\
    ( (This)->lpVtbl -> execQuery(This,username,password,host,basedn,scope,filter,attributes,usessl,result) ) 

#define ILDAPQuery_getLastError(This,result)	\
    ( (This)->lpVtbl -> getLastError(This,result) ) 

#define ILDAPQuery_connect(This,username,password,host,usessl,connect_id)	\
    ( (This)->lpVtbl -> connect(This,username,password,host,usessl,connect_id) ) 

#define ILDAPQuery_close(This,connect_id)	\
    ( (This)->lpVtbl -> close(This,connect_id) ) 

#define ILDAPQuery_query(This,connect_id,basedn,scope,filter,attributes,result)	\
    ( (This)->lpVtbl -> query(This,connect_id,basedn,scope,filter,attributes,result) ) 

#define ILDAPQuery_pageQuery(This,connect_id,basedn,scope,filter,attributes,total_results,results_id)	\
    ( (This)->lpVtbl -> pageQuery(This,connect_id,basedn,scope,filter,attributes,total_results,results_id) ) 

#define ILDAPQuery_nextResults(This,results_id,num_results,result)	\
    ( (This)->lpVtbl -> nextResults(This,results_id,num_results,result) ) 

#define ILDAPQuery_rename(This,connect_id,dn,newRDN,newParent,deleteOldRdn)	\
    ( (This)->lpVtbl -> rename(This,connect_id,dn,newRDN,newParent,deleteOldRdn) ) 

#define ILDAPQuery_dn2ufn(This,dn,ufn)	\
    ( (This)->lpVtbl -> dn2ufn(This,dn,ufn) ) 

#define ILDAPQuery_ufn2dn(This,ufn,dn)	\
    ( (This)->lpVtbl -> ufn2dn(This,ufn,dn) ) 

#define ILDAPQuery_err2string(This,ufn,string)	\
    ( (This)->lpVtbl -> err2string(This,ufn,string) ) 

#define ILDAPQuery_explodeDn(This,str,notypes,result)	\
    ( (This)->lpVtbl -> explodeDn(This,str,notypes,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILDAPQuery_INTERFACE_DEFINED__ */



#ifndef __LDAPQueryLib_LIBRARY_DEFINED__
#define __LDAPQueryLib_LIBRARY_DEFINED__

/* library LDAPQueryLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_LDAPQueryLib;

EXTERN_C const CLSID CLSID_LDAPQuery;

#ifdef __cplusplus

class DECLSPEC_UUID("FC08F164-F94E-4060-B9A0-10FBB23B4EFF")
LDAPQuery;
#endif
#endif /* __LDAPQueryLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


