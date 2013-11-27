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

enum DataType {
	IA5String,
	DirectoryString,
	PrintableString,
	OctetString,
	PostalAddress,
	CountryString,
	NumericString,
	Integer,
	GeneralizedTime,
	TelephoneNumber,
	Boolean,
	Binary,
	DistinguishedName,
	BitsStrings
};
/*
	1.3.6.1.4.1.1466.115.121.1.3 - Attribute Type Description 
	1.3.6.1.4.1.1466.115.121.1.5 - Binary syntax 
	1.3.6.1.4.1.1466.115.121.1.6 - Bit string syntax 
	1.3.6.1.4.1.1466.115.121.1.7 - Boolean syntax 
	1.3.6.1.4.1.1466.115.121.1.8 - Certificate syntax 
	1.3.6.1.4.1.1466.115.121.1.9 - Certificate List syntax 
	1.3.6.1.4.1.1466.115.121.1.10 - Certificate Pair syntax 
	1.3.6.1.4.1.1466.115.121.1.11 - Country String syntax 
	1.3.6.1.4.1.1466.115.121.1.12 - Distinguished Name syntax 
	1.3.6.1.4.1.1466.115.121.1.14 - Delivery Method 
	1.3.6.1.4.1.1466.115.121.1.15 - Directory String syntax 
	1.3.6.1.4.1.1466.115.121.1.16 - DIT Content Rule syntax 
	1.3.6.1.4.1.1466.115.121.1.17 - DIT Structure Rule Description syntax 
	1.3.6.1.4.1.1466.115.121.1.21 - Enhanced Guide 
	1.3.6.1.4.1.1466.115.121.1.22 - Facsimile Telephone Number syntax 
	1.3.6.1.4.1.1466.115.121.1.23 - Fax image syntax 
	1.3.6.1.4.1.1466.115.121.1.24 - Generalized Time syntax 
	1.3.6.1.4.1.1466.115.121.1.26 - IA5 String syntax 
	1.3.6.1.4.1.1466.115.121.1.27 - Integer syntax 
	1.3.6.1.4.1.1466.115.121.1.28 - JPeg Image syntax 
	1.3.6.1.4.1.1466.115.121.1.30 - Matching Rule Description syntax 
	1.3.6.1.4.1.1466.115.121.1.31 - Matching Rule Use Description syntax 
	1.3.6.1.4.1.1466.115.121.1.33 - MHS OR Address syntax 
	1.3.6.1.4.1.1466.115.121.1.34 - Name and Optional UID syntax 
	1.3.6.1.4.1.1466.115.121.1.35 - Name Form syntax 
	1.3.6.1.4.1.1466.115.121.1.36 - Numeric String syntax 
	1.3.6.1.4.1.1466.115.121.1.37 - Object Class Description syntax 
	1.3.6.1.4.1.1466.115.121.1.38 - OID syntax 
	1.3.6.1.4.1.1466.115.121.1.39 - Other Mailbox syntax 
	1.3.6.1.4.1.1466.115.121.1.40 - Octet String 
	1.3.6.1.4.1.1466.115.121.1.41 - Postal Address syntax 
	1.3.6.1.4.1.1466.115.121.1.43 - Presentation Address syntax 
	1.3.6.1.4.1.1466.115.121.1.44 - Printable string syntax 
	1.3.6.1.4.1.1466.115.121.1.49 - Supported Algorithm 
	1.3.6.1.4.1.1466.115.121.1.50 - Telephone number syntax 
	1.3.6.1.4.1.1466.115.121.1.51 - Teletex Terminal Identifier 
	1.3.6.1.4.1.1466.115.121.1.52 - Telex Number 
	1.3.6.1.4.1.1466.115.121.1.53 - UTCTime syntax 
	1.3.6.1.4.1.1466.115.121.1.54 - LDAP Syntax Description syntax
*/
