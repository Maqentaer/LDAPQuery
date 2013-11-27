/*
Copyright (c) 2013, Roman Glebsky (Maqentaer)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

#pragma once
#include "resource.h"       // main symbols

#include "LDAPQueryLib_i.h"
#include "AutoDeleteSimpleMap.h"
#include "DataTypes.h"
#include "ConnectInfo.h"

#include <atlstr.h>
#include <atlcoll.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#ifndef MAXULONG
#define MAXULONG ((ULONG)~((ULONG)0))
#endif

// CLDAPQuery
class ATL_NO_VTABLE CLDAPQuery :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLDAPQuery, &CLSID_LDAPQuery>,
	public IDispatchImpl<ILDAPQuery, &IID_ILDAPQuery, &LIBID_LDAPQueryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLDAPQuery();

DECLARE_REGISTRY_RESOURCEID(IDR_LDAPQUERY)

BEGIN_COM_MAP(CLDAPQuery)
	COM_INTERFACE_ENTRY(ILDAPQuery)
END_COM_MAP()

DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
//ILDAPQuery
	HRESULT STDMETHODCALLTYPE execQuery( 
		/* [in] */ BSTR username,
		/* [in] */ BSTR password,
		/* [in] */ BSTR host,
		/* [in] */ BSTR basedn,
		/* [in] */ BSTR scope,
		/* [in] */ BSTR filter,
		/* [in] */ BSTR attributes,
		/* [in] */ VARIANT_BOOL usessl,
		/* [retval][out] */ SAFEARRAY **result);

	HRESULT STDMETHODCALLTYPE getLastError( 
		/* [retval][out] */ LONG *result);

public:
//ILDAPQuery2
	STDMETHOD(connect)(
		/* [in] */ BSTR username,
		/* [in] */ BSTR password,
		/* [in] */ BSTR host,
		/* [in] */ VARIANT_BOOL usessl,
		/* [retval][out] */ LONG *connect_id);

	STDMETHOD(close)(
		/* [in] */ LONG connect_id);

	STDMETHOD(query)(
		/* [in] */ LONG connect_id,
		/* [in] */ BSTR basedn,
		/* [in] */ BSTR scope,
		/* [in] */ BSTR filter,
		/* [in] */ BSTR attributes,
		/* [retval][out] */ SAFEARRAY **result);

	STDMETHOD(pageQuery)(
		/* [in] */ LONG connect_id,
		/* [in] */ BSTR basedn,
		/* [in] */ BSTR scope,
		/* [in] */ BSTR filter,
		/* [in] */ BSTR attributes,
		/* [in] */ ULONG total_results,
		/* [retval][out] */ LONG *results_id);

	STDMETHOD(nextResults)(
		/* [in] */ LONG results_id,
		/* [in] */ ULONG num_results,
		/* [retval][out] */ SAFEARRAY **result);

	STDMETHOD(dn2ufn)(
		/* [in] */ BSTR dn,
		/* [retval][out] */ BSTR *ufn);

	STDMETHOD(ufn2dn)(
		/* [in] */ BSTR ufn,
		/* [retval][out] */ BSTR *dn);

	STDMETHOD(err2string)(
		/* [in] */ ULONG ufn,
		/* [retval][out] */ BSTR *string);

	STDMETHOD(explodeDn)(
		/* [in] */ BSTR str,
		/* [in] */ ULONG notypes,
		/* [retval][out] */ SAFEARRAY **result);

	STDMETHOD(rename)(
		/* [in] */ LONG connect_id,
		/* [in] */ BSTR dn,
		/* [in] */ BSTR newRDN,
		/* [in] */ BSTR newParent,
		/* [in] */ BOOL deleteOldRdn);

private:
	LONG m_errorCode;
	LONG m_maxConnectionId;
	CAutoDeleteSimpleMap<LONG, ÑConnectInfo*> m_connections;
};

OBJECT_ENTRY_AUTO(__uuidof(LDAPQuery), CLDAPQuery)
