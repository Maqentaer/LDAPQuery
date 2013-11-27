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

#include "DataTypes.h"
#include "AttributesSchema.h"

CAttributesSchema::CAttributesSchema(LDAP *ld, PTCHAR schema)
{
	CSimpleArray<PTCHAR> a;
	a.Add(_T("attributeTypes"));
	a.Add(NULL);
	LDAPMessage *pMsg = NULL;
	if(ldap_search_s(ld, schema, LDAP_SCOPE_BASE, _T("(objectClass=*)"), a.GetData(), 0, &pMsg) == LDAP_SUCCESS)
	{
		LDAPMessage* const entry = ldap_first_entry(ld, pMsg);
		if(entry)
		{
			PTCHAR attr;
			BerElement* berElem = NULL;
			for(attr = ldap_first_attribute(ld, entry, &berElem); attr != NULL; attr=ldap_next_attribute(ld, entry, berElem))
			{
				PTCHAR *vals;
				if((vals = ldap_get_values(ld, entry, attr)) != NULL)
				{
					for(ULONG i=0UL; vals[i]!=NULL; i++)
					{
						PTCHAR start = cs_schema_name(vals[i], _T("SYNTAX")), end;
						if(start != NULL)
						{
							start += 6;
							end = cs_schema_value(start); //get value
							const size_t len = end?end-start:wcslen(start); //length of value
							CString type(start, static_cast<int>(len)); //copy value
							AttrSchema attr;
							attr.type = getAttributeType(type);
							attr.single = (cs_schema_name(start, _T("SINGLE-VALUE")) != NULL);

							start = cs_schema_name(vals[i], _T("NAME"));
							if(start != NULL)
							{
								start += 4;
								while(*start == _T(' ')) start++; //skip spaces
								PTCHAR close = (*start == _T('('))?_tcschr(++start, _T(')')):NULL; //it is collection of names. find the end of it
								do
								{
									end = cs_schema_value(start); //get value
									const size_t len = end?end-start:close?close-start:_tcslen(start); //length of value
									if(!len) break; //not found
									CString name(start, static_cast<int>(len)); //copy value
									_attrSchemaMap.SetAt(name, attr);
									start += len; //move start pointer to end of value
									if(*start == _T('\'') || *start == _T('"')) start++; //skip quote
									while(*start == _T(' ')) start++; //skip spaces
								}
								while(start < close); //if it was collection - repeat to the end of collection
							}
						}
					}
					ldap_value_free(vals);
				}
				ldap_memfree(attr);
			}
		}
		ldap_msgfree(pMsg);
	}
}

const AttrSchema *CAttributesSchema::Lookup(const CString &attr)
{
	AttrSchema *result = NULL;
	CAtlMap<CString, AttrSchema>::CPair *pair = _attrSchemaMap.Lookup(attr);
	if(pair)
	{
		result = &pair->m_value;
	}
	return result;
}

/*static*/ CComVariant CAttributesSchema::getAttributeVariant(PBERVAL attr, const DataType type)
{
	CComVariant result;
	if(type == OctetString) //we need to convert only binary data
	{
		CComSafeArray<BYTE, VT_UI1> resArray(attr->bv_len);
		for(ULONG i=0; i < attr->bv_len; i++)
		{
			const BYTE val = (attr->bv_val[i]&0xFF);
			resArray.Add(sizeof(BYTE), &val, false);
		}
		result = resArray;
	}
	return result;
}

/*static*/ DataType CAttributesSchema::getAttributeType(const CString &type)
{
	DataType result;
	if(type == _T("1.3.6.1.4.1.1466.115.121.1.40")) //we need to convert only binary data
		result = OctetString;
	else
		result = DirectoryString;
	return result;
}

/*static*/ bool CAttributesSchema::isWordChar(const TCHAR ch)
{
	return ((ch >= _T('a') && ch <= _T('z')) || (ch >= _T('A') && ch <= _T('Z')) || (ch >= _T('0') && ch <= _T('9')));
}

/*static*/ PTCHAR CAttributesSchema::cs_schema_name(PTCHAR const str, PTCHAR const name)
{
	PTCHAR cp = str;
	PTCHAR s1, s2;

	if (!*name)
		return str;

	while (*cp)
	{
		if(*cp == _T('\'') || *cp == _T('"')) //skip values
		{
			cp = _tcschr(cp+1, *cp);
			if(!cp) break;
			cp++;
			continue;
		}

		s1 = cp;
		s2 = name;

		while ( *s1 && *s2 && !(*s1-*s2) ) //compare with the name
			s1++, s2++;

		if (!*s2 && (!*s1 || !isWordChar(*s1)) && (str == cp || !isWordChar(*(cp-1)))) //checking if found and it's isolated
			return(cp);

		cp++;
	}
	return(NULL);
}

/*static*/ PTCHAR CAttributesSchema::cs_schema_value(PTCHAR &start)
{
	while(*start == _T(' ')) start++;
	return (*start == _T('\'') || *start == _T('"')) ? _tcschr(++start, *(start-1)) : _tcspbrk(start, _T(" {}()"));
}
