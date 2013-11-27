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

#include "stdafx.h"
#include "LDAPQuery.h"
#include "AssocObject.h"

#include <atlcoll.h>

CLDAPQuery::CLDAPQuery() : m_maxConnectionId(0L)
{
}

bool CertRoutine(PLDAP /*Connection*/, PCCERT_CONTEXT /*pServerCert*/)
{
	return true; //just skip all certificate errors
}

HRESULT STDMETHODCALLTYPE CLDAPQuery::execQuery(
	/* [in] */ BSTR username,
	/* [in] */ BSTR password,
	/* [in] */ BSTR host,
	/* [in] */ BSTR basedn,
	/* [in] */ BSTR scope,
	/* [in] */ BSTR filter,
	/* [in] */ BSTR attributes,
	/* [in] */ VARIANT_BOOL usessl,
	/* [retval][out] */ SAFEARRAY **result)
{
	LONG connectId = -1L;
	connect(username, password, host, usessl, &connectId);
	if(connectId > -1L)
	{
		query(connectId, basedn, scope, filter, attributes, result);
		close(connectId);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CLDAPQuery::getLastError(LONG* result)
{
	*result = m_errorCode;
	return S_OK;
}

STDMETHODIMP CLDAPQuery::connect(
	/* [in] */ BSTR username,
	/* [in] */ BSTR password,
	/* [in] */ BSTR host,
	/* [in] */ VARIANT_BOOL usessl,
	/* [retval][out] */ LONG *connect_id)
{
	m_errorCode = 0L;
	const ULONG no_limit = LDAP_NO_LIMIT;

	PLDAP ld = NULL;
	bool useSSL = (usessl==VARIANT_TRUE)? true : false;
	bool canFindFromRoot = true;
	CAttributesSchema *attrsSchema = NULL;
	ULONG ulPort = useSSL?LDAP_SSL_PORT:LDAP_PORT;
	
	PTCHAR port = NULL;
	CString defaultNamingContext;
	CString csHost = host;
	CString hostname = _tcstok_s(csHost.GetBuffer(), _T(":"), &port);
	if(port && _tcslen(port) > 0)
		ulPort = _tcstol(port, NULL, 10);
	csHost.ReleaseBuffer();

	try
	{
		if (useSSL)
		{
			if((ld = ldap_sslinit(hostname.GetBuffer(), ulPort, 1))==NULL)
			{
				m_errorCode = LdapGetLastError();
				return S_FALSE;
			}

			m_errorCode = ldap_set_option(ld, LDAP_OPT_SERVER_CERTIFICATE, &CertRoutine);
			if (m_errorCode != LDAP_SUCCESS)
				throw _T("error LDAP_OPT_SERVER_CERTIFICATE");
		}
		else
		{
			if((ld = ldap_init(hostname.GetBuffer(), ulPort))==NULL)
			{
				m_errorCode = LdapGetLastError();
				return S_FALSE;
			}
		}

		const ULONG version = LDAP_VERSION3;
		m_errorCode = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, (void*)&version);
		if (m_errorCode != LDAP_SUCCESS)
			throw _T("error LDAP_OPT_PROTOCOL_VERSION");

		m_errorCode = ldap_set_option(ld, LDAP_OPT_SIZELIMIT, (void*)&no_limit);
		if (m_errorCode != LDAP_SUCCESS)
			throw _T("error LDAP_OPT_SIZELIMIT");
		ld->ld_sizelimit = no_limit;

		m_errorCode = ldap_set_option(ld, LDAP_OPT_TIMELIMIT, (void*)&no_limit);
		if (m_errorCode != LDAP_SUCCESS)
			throw _T("error LDAP_OPT_TIMELIMIT");
		ld->ld_timelimit = no_limit;

		m_errorCode = ldap_connect(ld, 0);
		if (m_errorCode != LDAP_SUCCESS )
			throw _T("error ldap_connect");

		m_errorCode = ldap_bind_s(ld, CString(username).GetBuffer(), CString(password).GetBuffer(), LDAP_AUTH_SIMPLE);
		if (m_errorCode != LDAP_SUCCESS)
			throw _T("error LDAP_AUTH_SIMPLE");

		/* Get the RootDSE and BaseDN attribute (add checks on use this code).*/
		CSimpleArray<PTCHAR> a;
		a.Add(_T("defaultNamingContext"));
		a.Add(_T("subschemaSubentry"));
		a.Add(NULL);
		PLDAPMessage pBaseMsg = NULL;
		if(ldap_search_s(ld, _T(""), LDAP_SCOPE_BASE, _T("(objectClass=*)"), a.GetData(), 0, &pBaseMsg) == LDAP_SUCCESS)
		{
			PLDAPMessage const entry = ldap_first_entry(ld, pBaseMsg);
			if(entry)
			{
				PTCHAR * const pschema = ldap_get_values(ld, entry, _T("subschemaSubentry"));
				if(pschema)
				{
					attrsSchema = new CAttributesSchema(ld, *pschema);
					ldap_value_free(pschema);
				}
				//try to find one item in subtree from root, if found it is Global Catalog search
				PLDAPSearch const pPages = ldap_search_init_page(ld, NULL, LDAP_SCOPE_SUBTREE, _T("(objectClass=*)"), NULL, 0, NULL, NULL, no_limit, 1, NULL);
				if(pPages)
				{
					PLDAPMessage pMsg = NULL;
					canFindFromRoot = (ldap_get_next_page_s(ld, pPages, NULL, 1, NULL, &pMsg) == LDAP_SUCCESS);
					if(pMsg)
						ldap_msgfree(pMsg);
					ldap_search_abandon_page(ld, pPages);
				}
				PTCHAR * const pDefaultNamingContext = ldap_get_values(ld, entry, _T("defaultNamingContext"));
				if(pDefaultNamingContext)
				{
					defaultNamingContext = *pDefaultNamingContext;
					ldap_value_free(pDefaultNamingContext);
				}
			}
			ldap_msgfree(pBaseMsg);
		}

		ÑConnectInfo * const cinfo = new ÑConnectInfo(ld, attrsSchema, defaultNamingContext, canFindFromRoot);
		*connect_id = ++m_maxConnectionId;
		m_connections.Add(*connect_id, cinfo);
	}
	catch (PTCHAR /*e*/)
	{
		if (ld)
			ldap_unbind_s(ld);
		*connect_id = -1;
		return S_FALSE;
	}
	
	return S_OK;
}

STDMETHODIMP CLDAPQuery::close(
	/* [in] */ LONG connect_id)
{
	m_errorCode = 0L;
	return m_connections.Remove(connect_id) ? S_OK : S_FALSE;
}

STDMETHODIMP CLDAPQuery::query(
	/* [in] */ LONG connect_id,
	/* [in] */ BSTR basedn,
	/* [in] */ BSTR scope,
	/* [in] */ BSTR filter,
	/* [in] */ BSTR attributes,
	/* [retval][out] */ SAFEARRAY **result)
{
	LONG results_id = -1L;
	pageQuery(connect_id, basedn, scope, filter, attributes, LDAP_NO_LIMIT, &results_id);
	if(results_id > -1L)
	{
		nextResults(results_id, LDAP_NO_LIMIT, result);
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::pageQuery(
	/* [in] */ LONG connect_id,
	/* [in] */ BSTR basedn,
	/* [in] */ BSTR scope,
	/* [in] */ BSTR filter,
	/* [in] */ BSTR attributes,
	/* [in] */ ULONG total_results,
	/* [retval][out] */ LONG* results_id)
{
	m_errorCode = 0L;
	const int id = m_connections.FindKey(connect_id);
	if(id > -1)
	{
		ÑConnectInfo * const cinfo = m_connections.GetValueAt(id);

		const PLDAP ld = cinfo->ld();

		const ULONG no_limit = LDAP_NO_LIMIT;
		ULONG ulScope = LDAP_SCOPE_BASE;
		CString csScope = scope;
		csScope.MakeLower();
		if(csScope == _T("subtree"))
			ulScope = LDAP_SCOPE_SUBTREE;
		else if(csScope == _T("onelevel"))
			ulScope = LDAP_SCOPE_ONELEVEL;
		else if(csScope == _T("base"))
			ulScope = LDAP_SCOPE_BASE;

		CString csBaseDN = basedn;

		if(csBaseDN.IsEmpty() && (ulScope == LDAP_SCOPE_ONELEVEL || (!cinfo->canFindFromRoot() && ulScope == LDAP_SCOPE_SUBTREE)))
		{
			csBaseDN = cinfo->defaultNamingContext();
		}

		CSimpleArray<PTCHAR> attributesArr;
		PTCHAR nextAttributes = NULL;
		CString csArrs = attributes;
		PTCHAR attribute = _tcstok_s(csArrs.GetBuffer(), _T(","), &nextAttributes);
		while(attribute != NULL)
		{
			attributesArr.Add(attribute);
			attribute = _tcstok_s(NULL, _T(","), &nextAttributes);
		}
		csArrs.ReleaseBuffer();
		attributesArr.Add(NULL); // NULL-terminated array

		PLDAPSearch pPages = ldap_search_init_page(ld, csBaseDN.GetBuffer(), ulScope, CString(filter).GetBuffer(), attributesArr.GetData(), 0, NULL, NULL, no_limit, total_results, NULL);
		m_errorCode = LdapGetLastError();
		if(pPages)
		{
			*results_id = cinfo->addResult(pPages);
		}
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::nextResults(
	/* [in] */ LONG results_id,
	/* [in] */ ULONG num_results,
	/* [retval][out] */ SAFEARRAY** result)
{
	CComSafeArray<VARIANT> dnList(0UL);

	ÑConnectInfo *cinfo = NULL;
	ÑResultsInfo *presinfo = NULL;
	m_errorCode = 0L;
	for(int i = 0; i < m_connections.GetSize(); i++)
	{
		ÑConnectInfo * const currentConnectInfo = m_connections.GetValueAt(i);
		if(currentConnectInfo)
		{
			ÑResultsInfo * const resInfo = currentConnectInfo->findResult(results_id);
			if(resInfo)
			{
				cinfo = currentConnectInfo;
				presinfo = resInfo;
				break;
			}
		}
	}
	if(cinfo && presinfo && presinfo->pages())
	{
		const PLDAP ld = cinfo->ld();
		const PLDAPSearch pPages = presinfo->pages();
		PLDAPMessage pMsg = NULL;
		const BOOL allResults = (num_results == 0UL);
		while((allResults || num_results > 0UL) &&
			(m_errorCode = ldap_get_next_page_s(ld, pPages, NULL, (num_results < 1000UL && num_results > 0UL ? num_results : 1000UL), NULL, &pMsg)) == LDAP_SUCCESS) //not more the 1k results per query
		{
			if(pMsg)
			{
				for(PLDAPMessage entry = ldap_first_entry(ld, pMsg); entry != NULL && (allResults || num_results-- > 0UL); entry = ldap_next_entry(ld, entry))
				{
					CComObject<CAssocObject> *obj;
					CComObject<CAssocObject>::CreateInstance(&obj);

					const PTCHAR dn = ldap_get_dn(ld, entry);
					obj->AddValue(_T("dn"), CComVariant(dn));

					PTCHAR attr;
					BerElement* berElem = NULL;
					for(attr = ldap_first_attribute(ld, entry, &berElem); attr != NULL; attr = ldap_next_attribute(ld, entry, berElem))
					{
						bool single = false;
						const AttrSchema* const schema = cinfo->attrSchemaLookup(attr);
						if(schema)
						{
							single = schema->single;
							if(schema->type == OctetString) //we need to work different only with binary data
							{
								PBERVAL *vals;
								if((vals = ldap_get_values_len(ld, entry, attr)) != NULL)
								{
									if(!single)
									{
										CComSafeArray<VARIANT> attrValues(ldap_count_values_len(vals));
										for(LONG i = 0L; vals[i] != NULL; i++)
										{
											attrValues[i] = CAttributesSchema::getAttributeVariant(vals[i], schema->type);
										}
										obj->AddValue(attr, CComVariant(attrValues));
									}
									else
									{
										obj->AddValue(attr, CAttributesSchema::getAttributeVariant(vals[0], schema->type));
									}
									ldap_value_free_len(vals);
								}
								ldap_memfree(attr);
								continue;
							}
						}
						PTCHAR *vals;
						if((vals = ldap_get_values(ld, entry, attr)) != NULL && ldap_count_values(vals) > 0UL)
						{
							if(!single)
							{
								CComSafeArray<VARIANT> attrValues(ldap_count_values(vals));
								for(LONG i = 0L; vals[i] != NULL; i++)
								{
									attrValues[i] = CComVariant(vals[i]);
								}
								obj->AddValue(attr, CComVariant(attrValues));
							}
							else
							{
								obj->AddValue(attr, CComVariant(vals[0]));
							}
							ldap_value_free(vals);
						}
						else
						{
							BOOL last = FALSE;
							LONG start = 0L;
							CComSafeArray<VARIANT> attrValues(0UL);
							do
							{
								CString attrWithRange;
								if(last)
									attrWithRange.Format(_T("%s;range=%ld-*"), attr, start);
								else
									attrWithRange.Format(_T("%s;range=%ld-%ld"), attr, start, start+999L);
								CSimpleArray<PTCHAR> a;
								a.Add(attrWithRange.GetBuffer());
								a.Add(NULL);
								PLDAPMessage pItemMsg = NULL;
								if(ldap_search_s(ld, dn, LDAP_SCOPE_BASE, _T("(objectClass=*)"), a.GetData(), 0, &pItemMsg) == LDAP_SUCCESS)
								{
									PLDAPMessage const itemEntry = ldap_first_entry(ld, pItemMsg);
									if(itemEntry)
									{
										if((vals = ldap_get_values(ld, itemEntry, attrWithRange.GetBuffer())) != NULL)
										{
											if(ldap_count_values(vals) == 0)
											{
												if(last) start = -1L; //leave loop if already last
												last = TRUE;
											}
											else
											{
												for(LONG i = 0L; vals[i] != NULL; i++)
												{
													attrValues.Add(CComVariant(vals[i]));
												}
												if(last) start = -1L; //leave loop
												else start += 1000L; //next results
											}
											ldap_value_free(vals);
										}
										else
										{
											if(last) start = -1L; //leave loop if already last
											last = TRUE;
										}
									}
									else
									{
										start = -1L; //leave loop
									}
									ldap_msgfree(pItemMsg);
								}
								else
								{
									start = -1L; //leave loop
								}
							} while (start >= 0L);
							obj->AddValue(attr, CComVariant(attrValues));
						}
						ldap_memfree(attr);
					}
					dnList.Add(CComVariant(obj));
					ldap_memfree(dn);
				}
				ldap_msgfree(pMsg);
			}
		}
		if(m_errorCode != LDAP_SUCCESS && m_errorCode != LDAP_SIZELIMIT_EXCEEDED)
		{
			cinfo->removeResult(results_id);
		}
		if(m_errorCode == LDAP_NO_RESULTS_RETURNED || m_errorCode == LDAP_MORE_RESULTS_TO_RETURN || m_errorCode == LDAP_SIZELIMIT_EXCEEDED)
			m_errorCode = LDAP_SUCCESS;
	}
	*result = dnList.Detach();
	return S_OK;
}

STDMETHODIMP CLDAPQuery::dn2ufn(
	/* [in] */ BSTR dn,
	/* [retval][out] */ BSTR *ufn)
{
	const PTCHAR pUfn = ldap_dn2ufn(CComBSTR(dn));
	m_errorCode = LdapGetLastError();
	if(pUfn)
	{
		*ufn = CComBSTR(pUfn).Detach();
		ldap_memfree(pUfn);
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::ufn2dn(
	/* [in] */ BSTR ufn,
	/* [retval][out] */ BSTR *dn)
{
	PTCHAR pDn = NULL;
	m_errorCode = ldap_ufn2dn(CComBSTR(ufn), &pDn);
	if(pDn)
	{
		*dn = CComBSTR(pDn).Detach();
		ldap_memfree(pDn);
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::err2string(
	/* [in] */ ULONG ufn,
	/* [retval][out] */ BSTR *string)
{
	const PTCHAR pStr = ldap_err2string(ufn);
	m_errorCode = LdapGetLastError();
	if(pStr)
	{
		*string = CComBSTR(pStr).Detach();
		ldap_memfree(pStr);
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::explodeDn(
	/* [in] */ BSTR str,
	/* [in] */ ULONG notypes,
	/* [retval][out] */ SAFEARRAY **result)
{
	PTCHAR *vals = ldap_explode_dn(CComBSTR(str), notypes);
	m_errorCode = LdapGetLastError();
	if(vals)
	{
		CComSafeArray<VARIANT> dnValues;
		for(LONG i = 0L; vals[i]!=NULL; i++)
		{
			dnValues.Add(CComVariant(vals[i]));
		}
		ldap_value_free(vals);
		*result = dnValues.Detach();
	}
	return S_OK;
}

STDMETHODIMP CLDAPQuery::rename(
	/* [in] */ LONG connect_id,
	/* [in] */ BSTR dn,
	/* [in] */ BSTR newRDN,
	/* [in] */ BSTR newParent,
	/* [in] */ BOOL deleteOldRdn)
{
	m_errorCode = 0L;
	const int id = m_connections.FindKey(connect_id);
	if(id > -1)
	{
		const ÑConnectInfo* const cinfo = m_connections.GetValueAt(id);

		ldap_rename(cinfo->ld(), CComBSTR(dn), CComBSTR(newRDN), CComBSTR(newParent), deleteOldRdn ? 1 : 0, NULL, NULL, NULL);
		m_errorCode = LdapGetLastError();
	}
	return S_OK;
}
