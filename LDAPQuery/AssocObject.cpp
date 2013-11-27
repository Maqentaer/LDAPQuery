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

#include "StdAfx.h"
#include "AssocObject.h"

CAssocObject::CAssocObject(void): m_id(DISPID_VALUE + 1L) //start from non zero
{
}

CAssocObject::~CAssocObject(void)
{
}

void CAssocObject::AddValue(CString name, CComVariant value)
{
	idValue.insert(std::pair<int,CComVariant>(m_id, value));
	nameId.insert(std::pair<CString,int>(name,m_id));
	++m_id;
}

STDMETHODIMP CAssocObject::GetIDsOfNames( 
	/* [in] */ REFIID /*riid*/, 
	/* [size_is][in] */ LPOLESTR *rgszNames, 
	/* [in] */ UINT /*cNames*/, 
	/* [in] */ LCID /*lcid*/, 
	/* [size_is][out] */ DISPID *rgDispId )
{
	std::map<CString,DISPID>::const_iterator it = nameId.find(CString(*rgszNames));
	if (it!=nameId.end())
	{
		*rgDispId = (*it).second;
	}
	else
	{
		*rgDispId = m_id++;
		nameId.insert(std::pair<CString,int>(*rgszNames,*rgDispId));
	}
	return S_OK;
}

STDMETHODIMP CAssocObject::Invoke(
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID /*riid*/,
	/* [in] */ LCID /*lcid*/,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO* /*pExcepInfo*/,
	/* [out] */ UINT* /*puArgErr*/)
{
	if(pDispParams && pDispParams->cArgs > 0)
	{
		CString name;
		CComVariant var0 = pDispParams->rgvarg[pDispParams->cArgs-1];
		if(var0.vt == (VT_VARIANT | VT_BYREF))
			var0 = *var0.pvarVal;
		if(var0.vt == VT_BSTR)
		{
			name = var0.bstrVal;
		}
		else if(var0.vt == (VT_BSTR | VT_BYREF))
		{
			name = *var0.pbstrVal;
		}
		if(!name.IsEmpty())
		{
			std::map<CString,DISPID>::const_iterator it = nameId.find(name);
			if (it!=nameId.end())
			{
				dispIdMember = (*it).second;
			}
			else
			{
				dispIdMember = m_id++;
				nameId.insert(std::pair<CString,int>(name,dispIdMember));
			}
		}
	}
	if(wFlags & DISPATCH_PROPERTYGET || wFlags == DISPATCH_METHOD)
	{
		CComVariant result;
		std::map<DISPID,CComVariant>::const_iterator itm = idValue.find(dispIdMember);
		if (itm!=idValue.end())
		{
			result = (*itm).second;
		}
		result.Detach(pVarResult);
	}
	else if(wFlags & DISPATCH_PROPERTYPUT || wFlags & DISPATCH_PROPERTYPUTREF)
	{
		std::map<DISPID,CComVariant>::iterator itm = idValue.find(dispIdMember);
		if (itm!=idValue.end())
		{
			(*itm).second = pDispParams->rgvarg[0];
		}
		else if(dispIdMember > DISPID_VALUE)
		{
			idValue.insert(std::pair<int,CComVariant>(dispIdMember, pDispParams->rgvarg[0]));
		}
	}
	return S_OK;
}
