/**
 * WinPR: Windows Portable Runtime
 * Credential Security Support Provider (CredSSP)
 *
 * Copyright 2010-2014 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *		 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/sspi.h>

#include "credssp.h"

#include "../sspi.h"

char* CREDSSP_PACKAGE_NAME = "CredSSP";

SECURITY_STATUS SEC_ENTRY credssp_InitializeSecurityContextW(PCredHandle phCredential, PCtxtHandle phContext,
		SEC_WCHAR* pszTargetName, ULONG fContextReq, ULONG Reserved1, ULONG TargetDataRep,
		PSecBufferDesc pInput, ULONG Reserved2, PCtxtHandle phNewContext,
		PSecBufferDesc pOutput, PULONG pfContextAttr, PTimeStamp ptsExpiry)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_InitializeSecurityContextA(PCredHandle phCredential, PCtxtHandle phContext,
		SEC_CHAR* pszTargetName, ULONG fContextReq, ULONG Reserved1, ULONG TargetDataRep,
		PSecBufferDesc pInput, ULONG Reserved2, PCtxtHandle phNewContext,
		PSecBufferDesc pOutput, PULONG pfContextAttr, PTimeStamp ptsExpiry)
{
	CREDSSP_CONTEXT* context;
	CREDENTIALS* credentials;

	context = sspi_SecureHandleGetLowerPointer(phContext);

	if (!context)
	{
		context = credssp_ContextNew();

		credentials = (CREDENTIALS*) sspi_SecureHandleGetLowerPointer(phCredential);

		sspi_SecureHandleSetLowerPointer(phNewContext, context);
		sspi_SecureHandleSetUpperPointer(phNewContext, (void*) CREDSSP_PACKAGE_NAME);
	}

	return SEC_E_OK;

}

CREDSSP_CONTEXT* credssp_ContextNew()
{
	CREDSSP_CONTEXT* context;

	context = (CREDSSP_CONTEXT*) calloc(1, sizeof(CREDSSP_CONTEXT));

	if (context != NULL)
	{

	}

	return context;
}

void credssp_ContextFree(CREDSSP_CONTEXT* context)
{
	if (!context)
		return;

	free(context);
}

SECURITY_STATUS SEC_ENTRY credssp_QueryContextAttributes(PCtxtHandle phContext, ULONG ulAttribute, void* pBuffer)
{
	if (!phContext)
		return SEC_E_INVALID_HANDLE;

	if (!pBuffer)
		return SEC_E_INSUFFICIENT_MEMORY;

	return SEC_E_UNSUPPORTED_FUNCTION;
}

SECURITY_STATUS SEC_ENTRY credssp_AcquireCredentialsHandleW(SEC_WCHAR* pszPrincipal, SEC_WCHAR* pszPackage,
		ULONG fCredentialUse, void* pvLogonID, void* pAuthData, SEC_GET_KEY_FN pGetKeyFn,
		void* pvGetKeyArgument, PCredHandle phCredential, PTimeStamp ptsExpiry)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_AcquireCredentialsHandleA(SEC_CHAR* pszPrincipal, SEC_CHAR* pszPackage,
		ULONG fCredentialUse, void* pvLogonID, void* pAuthData, SEC_GET_KEY_FN pGetKeyFn,
		void* pvGetKeyArgument, PCredHandle phCredential, PTimeStamp ptsExpiry)
{
	CREDENTIALS* credentials;
	SEC_WINNT_AUTH_IDENTITY* identity;

	if (fCredentialUse == SECPKG_CRED_OUTBOUND)
	{
		credentials = sspi_CredentialsNew();
		identity = (SEC_WINNT_AUTH_IDENTITY*) pAuthData;

		CopyMemory(&(credentials->identity), identity, sizeof(SEC_WINNT_AUTH_IDENTITY));

		sspi_SecureHandleSetLowerPointer(phCredential, (void*) credentials);
		sspi_SecureHandleSetUpperPointer(phCredential, (void*) CREDSSP_PACKAGE_NAME);

		return SEC_E_OK;
	}

	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_QueryCredentialsAttributesW(PCredHandle phCredential, ULONG ulAttribute, void* pBuffer)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_QueryCredentialsAttributesA(PCredHandle phCredential, ULONG ulAttribute, void* pBuffer)
{
	if (ulAttribute == SECPKG_CRED_ATTR_NAMES)
	{
		CREDENTIALS* credentials;

		credentials = (CREDENTIALS*) sspi_SecureHandleGetLowerPointer(phCredential);

		return SEC_E_OK;
	}

	return SEC_E_UNSUPPORTED_FUNCTION;
}

SECURITY_STATUS SEC_ENTRY credssp_FreeCredentialsHandle(PCredHandle phCredential)
{
	CREDENTIALS* credentials;

	if (!phCredential)
		return SEC_E_INVALID_HANDLE;

	credentials = (CREDENTIALS*) sspi_SecureHandleGetLowerPointer(phCredential);

	if (!credentials)
		return SEC_E_INVALID_HANDLE;

	sspi_CredentialsFree(credentials);

	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_EncryptMessage(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage, ULONG MessageSeqNo)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_DecryptMessage(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, ULONG* pfQOP)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_MakeSignature(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage, ULONG MessageSeqNo)
{
	return SEC_E_OK;
}

SECURITY_STATUS SEC_ENTRY credssp_VerifySignature(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, ULONG* pfQOP)
{
	return SEC_E_OK;
}

const SecurityFunctionTableA CREDSSP_SecurityFunctionTableA =
{
	1, /* dwVersion */
	NULL, /* EnumerateSecurityPackages */
	credssp_QueryCredentialsAttributesA, /* QueryCredentialsAttributes */
	credssp_AcquireCredentialsHandleA, /* AcquireCredentialsHandle */
	credssp_FreeCredentialsHandle, /* FreeCredentialsHandle */
	NULL, /* Reserved2 */
	credssp_InitializeSecurityContextA, /* InitializeSecurityContext */
	NULL, /* AcceptSecurityContext */
	NULL, /* CompleteAuthToken */
	NULL, /* DeleteSecurityContext */
	NULL, /* ApplyControlToken */
	credssp_QueryContextAttributes, /* QueryContextAttributes */
	NULL, /* ImpersonateSecurityContext */
	NULL, /* RevertSecurityContext */
	credssp_MakeSignature, /* MakeSignature */
	credssp_VerifySignature, /* VerifySignature */
	NULL, /* FreeContextBuffer */
	NULL, /* QuerySecurityPackageInfo */
	NULL, /* Reserved3 */
	NULL, /* Reserved4 */
	NULL, /* ExportSecurityContext */
	NULL, /* ImportSecurityContext */
	NULL, /* AddCredentials */
	NULL, /* Reserved8 */
	NULL, /* QuerySecurityContextToken */
	credssp_EncryptMessage, /* EncryptMessage */
	credssp_DecryptMessage, /* DecryptMessage */
	NULL, /* SetContextAttributes */
};

const SecurityFunctionTableW CREDSSP_SecurityFunctionTableW =
{
	1, /* dwVersion */
	NULL, /* EnumerateSecurityPackages */
	credssp_QueryCredentialsAttributesW, /* QueryCredentialsAttributes */
	credssp_AcquireCredentialsHandleW, /* AcquireCredentialsHandle */
	credssp_FreeCredentialsHandle, /* FreeCredentialsHandle */
	NULL, /* Reserved2 */
	credssp_InitializeSecurityContextW, /* InitializeSecurityContext */
	NULL, /* AcceptSecurityContext */
	NULL, /* CompleteAuthToken */
	NULL, /* DeleteSecurityContext */
	NULL, /* ApplyControlToken */
	credssp_QueryContextAttributes, /* QueryContextAttributes */
	NULL, /* ImpersonateSecurityContext */
	NULL, /* RevertSecurityContext */
	credssp_MakeSignature, /* MakeSignature */
	credssp_VerifySignature, /* VerifySignature */
	NULL, /* FreeContextBuffer */
	NULL, /* QuerySecurityPackageInfo */
	NULL, /* Reserved3 */
	NULL, /* Reserved4 */
	NULL, /* ExportSecurityContext */
	NULL, /* ImportSecurityContext */
	NULL, /* AddCredentials */
	NULL, /* Reserved8 */
	NULL, /* QuerySecurityContextToken */
	credssp_EncryptMessage, /* EncryptMessage */
	credssp_DecryptMessage, /* DecryptMessage */
	NULL, /* SetContextAttributes */
};

const SecPkgInfoA CREDSSP_SecPkgInfoA =
{
	0x000110733, /* fCapabilities */
	1, /* wVersion */
	0xFFFF, /* wRPCID */
	0x000090A8, /* cbMaxToken */
	"CREDSSP", /* Name */
	"Microsoft CredSSP Security Provider" /* Comment */
};

WCHAR CREDSSP_SecPkgInfoW_Name[] = { 'C','R','E','D','S','S','P','\0' };

WCHAR CREDSSP_SecPkgInfoW_Comment[] =
{
	'M','i','c','r','o','s','o','f','t',' ',
	'C','r','e','d','S','S','P',' ',
	'S','e','c','u','r','i','t','y',' ',
	'P','r','o','v','i','d','e','r','\0'
};

const SecPkgInfoW CREDSSP_SecPkgInfoW =
{
	0x000110733, /* fCapabilities */
	1, /* wVersion */
	0xFFFF, /* wRPCID */
	0x000090A8, /* cbMaxToken */
	CREDSSP_SecPkgInfoW_Name, /* Name */
	CREDSSP_SecPkgInfoW_Comment /* Comment */
};
