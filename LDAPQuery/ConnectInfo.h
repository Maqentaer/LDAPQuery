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

#include "DataTypes.h"
#include "AttributesSchema.h"
#include "ResultsInfo.h"

#include <atlstr.h>
#include <atlcoll.h>

class ÑConnectInfo
{
public:
	ÑConnectInfo(PLDAP ld, CAttributesSchema *attrsSchema, CString defaultNamingContext, bool canFindFromRoot)
		: _ld(ld), _attrsSchema(attrsSchema), _defaultNamingContext(defaultNamingContext), _canFindFromRoot(canFindFromRoot)
	{
	}

	virtual ~ÑConnectInfo()
	{
		if(_attrsSchema)
			delete _attrsSchema;
		_results.RemoveAll();
		if(_ld)
			ldap_unbind_s(_ld);
	}

	LONG addResult(PLDAPSearch pages)
	{
		static LONG maxResultId = 0L;
		ÑResultsInfo *info = new ÑResultsInfo(_ld, pages);
		_results.Add(maxResultId, info);
		return maxResultId++;
	}

	BOOL removeResult(LONG id)
	{
		return _results.Remove(id);
	}

	ÑResultsInfo *findResult(LONG result_id)
	{
		ÑResultsInfo *result = NULL;
		const int index = _results.FindKey(result_id);
		if(index > -1)
		{
			result = _results.GetValueAt(index);
		}
		return result;
	}

	const AttrSchema *attrSchemaLookup (const CString &attr) const
	{
		return _attrsSchema->Lookup(attr);
	}

	PLDAP ld() const { return _ld; }

	CString defaultNamingContext() const { return _defaultNamingContext; }

	bool canFindFromRoot() const { return _canFindFromRoot; }

private:
	PLDAP _ld;
	CAttributesSchema *_attrsSchema;
	CAutoDeleteSimpleMap<LONG, ÑResultsInfo*> _results;
	CString _defaultNamingContext;
	bool _canFindFromRoot;
};
