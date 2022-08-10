/*=========================================================================== 
    (c) Copyright 1999, Emmanuel KARTMANN, all rights reserved                 
  =========================================================================== 
    File           : FWindowsRegistry.cpp
    $Header: $
    Author         : Emmanuel KARTMANN
    Creation       : Tuesday 9/21/99 2:49:45 PM
    Remake         : 
  ------------------------------- Description ------------------------------- 

           Implementation of the CWindowsRegistry class.

  ------------------------------ Modifications ------------------------------ 
    $Log: $  
  =========================================================================== 
*/
#include "stdafx.h"
#include "WindowsRegistry.h"

// Maximum size for the write buffer (recommended in the WIN32 API: bigger
// data shouldn't be stored in the registry).
const size_t CWindowsRegistry::MaximumWriteSize = 2048;


// Array of strings and their associated base handles
CWindowsRegistry::BaseNameEntry CWindowsRegistry::m_pBaseNameMap[] = {
	{ "Unknown", NULL },
	{ "HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE },
	{ "HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT },
	{ "HKEY_USERS", HKEY_USERS },
	{ "HKEY_CURRENT_USER", HKEY_CURRENT_USER },
	{ "HKEY_PERFORMANCE_DATA", HKEY_PERFORMANCE_DATA },
	{ "HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG },
	{ "HKEY_DYN_DATA", HKEY_DYN_DATA },
	{ NULL, NULL }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindowsRegistry::CWindowsRegistry()
	: m_pKey(NULL),
	  m_pBaseKey(NULL),
	  m_pSubKeyName(NULL),
      m_nSecurityAccessMask(0),
	  m_nErrorCode(ERROR_SUCCESS)
{

}

CWindowsRegistry::CWindowsRegistry(LPCTSTR pSubKeyName, HKEY pBaseKey, REGSAM nSecurityAccessMask)
	: m_pKey(NULL),
	  m_pBaseKey(NULL),
	  m_pSubKeyName(NULL),
      m_nSecurityAccessMask(0),
	  m_nErrorCode(ERROR_SUCCESS)
{
	m_nErrorCode=OpenKey(pSubKeyName, pBaseKey, nSecurityAccessMask);
}

CWindowsRegistry::~CWindowsRegistry()
{
	CloseKey();
}

LONG CWindowsRegistry::OpenKey(LPCTSTR pSubKeyName, HKEY pBaseKey, REGSAM nSecurityAccessMask)
{
	m_nErrorCode=ERROR_SUCCESS;

	if (IsOpen() && pBaseKey != (HKEY)NULL) {
		CloseKey();
	}

	if (*pSubKeyName == '\\') {
		// It's an absolute path: get base name
		while (*pSubKeyName == '\\') {
			pSubKeyName++; // skip the backslash(es)
		}
		for (int i=0; m_pBaseNameMap[i].String!=NULL; i++) {
			if (!strncmp(m_pBaseNameMap[i].String, pSubKeyName, strlen(m_pBaseNameMap[i].String)))
			{
				if (m_pKey!=(HKEY)NULL) {
					CloseKey();
				}
				pBaseKey = m_pBaseNameMap[i].Handle;
				pSubKeyName += strlen(m_pBaseNameMap[i].String)+1;
				break;
			}
		}
	}

	DWORD ulOptions=0;
	m_pBaseKey=pBaseKey;
	m_pSubKeyName=pSubKeyName;
	m_nSecurityAccessMask=nSecurityAccessMask;
	m_nErrorCode=RegOpenKeyEx(m_pBaseKey,            // handle to base key
							  m_pSubKeyName,         // name of subkey to open
							  ulOptions,             // reserved
							  m_nSecurityAccessMask, // security access mask
							  &m_pKey                // handle of opened key
							  );

	// Force WIN32 error (it is not set by RegOpenKeyEx).
	if (m_nErrorCode!=ERROR_SUCCESS) {
		SetLastError(m_nErrorCode);
	}

	return(m_nErrorCode);
}

LONG CWindowsRegistry::CreateKey(LPCTSTR pSubKeyName, HKEY pBaseKey, REGSAM nSecurityAccessMask)
{
	m_nErrorCode=ERROR_SUCCESS;
	HKEY pNewKeyHandle;

    // Check parameter
    if ( (pSubKeyName==NULL) || (pSubKeyName!=NULL && strlen(pSubKeyName)==0)) {
        return(ERROR_INVALID_PARAMETER);
    }

	if (IsOpen() && pBaseKey != (HKEY)NULL) {
		CloseKey();
	} else {
		m_pBaseKey = m_pKey; // reuse opened handle as the base
	}

	if (*pSubKeyName == '\\') {
		// It's an absolute path: get base name
		while (*pSubKeyName == '\\') {
			pSubKeyName++; // skip the backslash(es)
		}
		for (int i=0; m_pBaseNameMap[i].String!=NULL; i++) {
			if (!strncmp(m_pBaseNameMap[i].String, pSubKeyName, strlen(m_pBaseNameMap[i].String)))
			{
				if (pBaseKey!=(HKEY)NULL) {
					CloseKey();
				}
				pBaseKey = m_pBaseNameMap[i].Handle;
				m_pBaseKey=pBaseKey;
				pSubKeyName += strlen(m_pBaseNameMap[i].String)+1;
				break;
			}
		}
	}

	m_pSubKeyName=pSubKeyName;

	DWORD nOptions=REG_OPTION_NON_VOLATILE;
	DWORD nReserved=0;
	DWORD nDisposition=0;
	LPTSTR pClass="";
	m_pSubKeyName=pSubKeyName;
	m_nSecurityAccessMask=nSecurityAccessMask;
	m_nErrorCode=RegCreateKeyEx(m_pBaseKey,            // handle to base key
							    m_pSubKeyName,         // name of subkey to open
								nReserved,             // reserved (must be zero)
								pClass,                // class of object to create
							    nOptions,              // option flag
							    m_nSecurityAccessMask, // security access mask
								NULL,                  // key security structure (NULL: use default)
							    &pNewKeyHandle,        // handle of opened key
								&nDisposition          // disposition value buffer (created/open)
							    );
	if (m_nErrorCode == ERROR_SUCCESS) {
		// Immediately close the new key (avoid memory leaks)
		RegCloseKey(pNewKeyHandle);
	} else {
		// Force WIN32 error (it is not set by RegCreateKeyEx).
		SetLastError(m_nErrorCode);
	}
	return(m_nErrorCode);
}

LONG CWindowsRegistry::RecursivelyDeleteAllSubKeys( HKEY key_handle, LPCTSTR key_name )
{
   HKEY child_key_handle = NULL;

   LONG return_value = 0;

   LPTSTR temporary_key_name = NULL;

   return_value = RegOpenKeyEx( key_handle, key_name, NULL, KEY_ALL_ACCESS, &child_key_handle );

   if ( return_value != ERROR_SUCCESS )
   {
      return( return_value );
   }

   try
   {
      temporary_key_name = new TCHAR[ MAX_PATH ];
   }
   catch( ... )
   {
      temporary_key_name = NULL;
   }

   if ( temporary_key_name == NULL )
   {
      return( ERROR_NOT_ENOUGH_MEMORY );
   }

   return_value = RegEnumKey( child_key_handle, 0, temporary_key_name, MAX_PATH );

   while( return_value == ERROR_SUCCESS )
   {
      RecursivelyDeleteAllSubKeys( child_key_handle, temporary_key_name );

      return_value = RegEnumKey( child_key_handle, 0, temporary_key_name, MAX_PATH );
   }

   delete [] temporary_key_name;

   temporary_key_name = NULL;

   RegCloseKey( child_key_handle );

   return_value = RegDeleteKey( key_handle, key_name );

   return( return_value );
}

LONG CWindowsRegistry::DeleteKey(LPCTSTR pSubKeyName, HKEY pKey, BOOL bRecursive)
{
	m_nErrorCode=ERROR_SUCCESS;

    // Check parameter
    if ( (pSubKeyName==NULL) || (pSubKeyName!=NULL && strlen(pSubKeyName)==0)) {
        return(ERROR_INVALID_PARAMETER);
    }

	if (IsOpen() && pKey != (HKEY)NULL) {
		CloseKey();
	}

	if (*pSubKeyName == '\\') {
		// It's an absolute path: get base name
		while (*pSubKeyName == '\\') {
			pSubKeyName++; // skip the backslash(es)
		}
		for (int i=0; m_pBaseNameMap[i].String!=NULL; i++) {
			if (!strncmp(m_pBaseNameMap[i].String, pSubKeyName, strlen(m_pBaseNameMap[i].String)))
			{
				if (pKey!=(HKEY)NULL) {
					CloseKey();
				}
				pKey = m_pBaseNameMap[i].Handle;
				pSubKeyName += strlen(m_pBaseNameMap[i].String)+1;
				break;
			}
		}
	}
	if (pKey != (HKEY)NULL) {
		m_pKey=pKey;
	} // else: reuse same handle
	m_pSubKeyName=pSubKeyName;

    if (bRecursive) {
	    m_nErrorCode=RecursivelyDeleteAllSubKeys(m_pKey,            // handle to base key
											     m_pSubKeyName          // name of subkey to open
											     );
    } else {
        m_nErrorCode = RegDeleteKey(m_pKey, m_pSubKeyName);
    }

	// Force WIN32 error (it is not set by default).
	if (m_nErrorCode!=ERROR_SUCCESS) {
		SetLastError(m_nErrorCode);
	}
	return(m_nErrorCode);
}

LONG CWindowsRegistry::CloseKey()
{
	LONG nResult=ERROR_SUCCESS;
	if (IsOpen()) {
		nResult=RegCloseKey(m_pKey);
		m_pKey=NULL;
	}
	return(nResult);
}

BOOL CWindowsRegistry::IsOpen()
{
	if (m_pKey != (HKEY)NULL) {
		return(TRUE);
	} else {
		return(FALSE);
	}
}


LONG CWindowsRegistry::QueryValue(LPCTSTR pKeyName, char **KeyValue, LPDWORD pValueType)
{ 
	LONG nResult=ERROR_SUCCESS;
	DWORD nType=0;
	DWORD nValueBufferSize=MaximumWriteSize;
	LPBYTE pValueBuffer = new BYTE[nValueBufferSize+1];
	//LPBYTE pValueBuffer = (LPBYTE)malloc( (nValueBufferSize+1)*sizeof(BYTE) );
	DWORD pSupportedTypes[] = {
		REG_EXPAND_SZ,
		REG_SZ,
	};
	if (!KeyValue) {
		return(ERROR_INVALID_PARAMETER);
	}
	// Reset value
	if (*KeyValue!=NULL) {
		*KeyValue=NULL;
	}

	// Fetch data in registry and check result type
	nResult=RawQueryValue(pKeyName, pValueBuffer, &nValueBufferSize, &nType, pSupportedTypes, sizeof(pSupportedTypes) / sizeof(DWORD));

	if (nResult==ERROR_SUCCESS) {
		if (nValueBufferSize > 0) {
			// Set the Key Value: note that the '\0' is counted in the buffer size, so we must substract one
			pValueBuffer[nValueBufferSize-1]='\0';
			*KeyValue = new char[nValueBufferSize+1];
			if (*KeyValue) {
				// We can use 'strcpy' since we put a '\0' at the end of the value buffer
				strcpy(*KeyValue, (const char*)pValueBuffer);
			} else {
				nResult = ERROR_NOT_ENOUGH_MEMORY;
			}
			// Set the Key Type
			if (pValueType != NULL) {
				*pValueType=nType;
			}
		}
	}

	if (pValueBuffer) {
		delete [] pValueBuffer;
		//free(pValueBuffer);
	}
	return(nResult);
}

LONG CWindowsRegistry::QueryValue(LPCTSTR pKeyName, DWORD &KeyValue, LPDWORD pValueType) 
{
	LONG nResult=ERROR_SUCCESS;
	DWORD nType=0;
	DWORD nValueBufferSize=MaximumWriteSize;
	LPBYTE pValueBuffer = new BYTE[nValueBufferSize+1];
	DWORD pSupportedTypes[] = {
		REG_DWORD,
		REG_DWORD_LITTLE_ENDIAN,
		REG_DWORD_BIG_ENDIAN,
	};

	// Reset value
	KeyValue=0;

	// Fetch data in registry and check result type
	nResult=RawQueryValue(pKeyName, pValueBuffer, &nValueBufferSize, &nType, pSupportedTypes, sizeof(pSupportedTypes) / sizeof(DWORD));

	if (nResult==ERROR_SUCCESS) {
		// Set the Key Value
		int *pIntValue = (int *)pValueBuffer;
		KeyValue = *pIntValue;
		// Set the Key Type
		if (pValueType != NULL) {
			*pValueType=nType;
		}
	}

	if (pValueBuffer) {
		delete [] pValueBuffer;
	}
	return(nResult);
}

LONG CWindowsRegistry::QueryValue(LPCTSTR pKeyName, char **KeyValue, int &KeyArraySize, LPDWORD pValueType)
{
	return(ERROR_CALL_NOT_IMPLEMENTED);
}

LONG CWindowsRegistry::RawQueryValue(LPCTSTR pKeyName, LPBYTE pValueBuffer, DWORD *pValueBufferSize, DWORD *pType, DWORD pSupportedTypes[], size_t nNumberOfTypes)
{
	LONG nResult=ERROR_SUCCESS;
	HKEY tempKeyHandle = NULL;
	LPDWORD pReserved=NULL;
	char pSubKeyName[FWR_MAXIMUM_KEY_SIZE];
	char pSubPathName[FWR_MAXIMUM_KEY_SIZE];
	BOOL bCloseOnExit = FALSE;

    // Check parameter
    if ( pKeyName==NULL ) {
        return(ERROR_INVALID_PARAMETER);
    }

	if (!IsOpen()) {
		return(ERROR_INVALID_HANDLE);
	}

	const char *separatorPosition = strrchr(pKeyName, '\\');
	if (separatorPosition) {
		// We were given a path name with slashes: must open new handle
		DWORD ulOptions=0;
		bCloseOnExit = TRUE;
		// Fetch name of the last key in the path
		strncpy(pSubKeyName, separatorPosition+1, FWR_MAXIMUM_KEY_SIZE);
		// Fetch name of the sub-path, except the last key
		strncpy(pSubPathName, pKeyName, separatorPosition - pKeyName);
		pSubPathName[separatorPosition - pKeyName]='\0';
		nResult=RegOpenKeyEx(m_pKey,           // handle to base key
							 pSubPathName,     // name of subkey to open
							 ulOptions,        // reserved
							 KEY_QUERY_VALUE,    // security access mask
							 &tempKeyHandle    // handle of opened key
							 );
	} else {
		// We were given a direct value name: reuse currently open handle
		tempKeyHandle = m_pKey;
		strncpy(pSubKeyName, pKeyName, FWR_MAXIMUM_KEY_SIZE);
	}

	// Read the value
	if (nResult == ERROR_SUCCESS) {
		nResult = RegQueryValueEx(tempKeyHandle,
								  pSubKeyName,
								  pReserved,
								  pType,
								  pValueBuffer,
								  pValueBufferSize);

	} else {
		// Force WIN32 error (it is not set by default).
		SetLastError(nResult);
	}

	if (nResult == ERROR_SUCCESS) {
		// Check key type
		if (pSupportedTypes != NULL && pType != NULL) {
			nResult=ERROR_INVALID_DATA;
			for (unsigned int i = 0; i < nNumberOfTypes; i++) {
				if (pSupportedTypes[i]==*pType) {
					nResult=ERROR_SUCCESS;
					break;
				}
			}
		}
	}

	if (bCloseOnExit) {
		RegCloseKey(tempKeyHandle);
	}

	return(nResult);

}

LONG CWindowsRegistry::WriteValue(LPCTSTR pKeyName, LPCTSTR pKeyValue, DWORD nValueType)
{
	LONG nResult=ERROR_SUCCESS;
	DWORD nValueBufferSize=0;
	BYTE pValueBuffer[MaximumWriteSize+1];
	size_t nEffectiveSize=0;
	
	// If string is NULL... do not write anything (Q: should we write an empty string ?)
	if (pKeyValue == NULL) {
		return(ERROR_SUCCESS); // Not an error
	}

	nEffectiveSize=strlen(pKeyValue)+1; // Always write the '\0'

	// Set buffer for registry I/O
	if (nEffectiveSize >= MaximumWriteSize) {
		// Try not to overflow the write buffer
		nEffectiveSize=MaximumWriteSize;
	}
	memcpy((void *)pValueBuffer, (void *)pKeyValue, nEffectiveSize);
	
	// Set data in registry with low-level function
	nResult=RawWriteValue(pKeyName, pValueBuffer, nEffectiveSize, nValueType);

	return(nResult);
}

LONG CWindowsRegistry::WriteValue(LPCTSTR pKeyName, int KeyValue, DWORD nValueType)
{
	LONG nResult=ERROR_SUCCESS;
	DWORD nValueBufferSize=0;
	BYTE pValueBuffer[MaximumWriteSize+1];
	size_t nEffectiveSize=0;
	
	nEffectiveSize=sizeof(KeyValue);

	// Set buffer for registry I/O
	if (nEffectiveSize >= MaximumWriteSize) {
		// Try not to overflow the write buffer
		nEffectiveSize=MaximumWriteSize;
	}
	memcpy((void *)pValueBuffer, (void *)&KeyValue, nEffectiveSize);

	// Set data in registry with low-level function
	nResult=RawWriteValue(pKeyName, pValueBuffer, nEffectiveSize, nValueType);

	return(nResult);
}

LONG CWindowsRegistry::RawWriteValue(LPCTSTR pKeyName, LPBYTE pValueBuffer, DWORD nValueBufferSize, DWORD nType)
{
	LONG nResult=ERROR_SUCCESS;
	HKEY tempKeyHandle = NULL;
	DWORD nReserved=0;
	char pSubKeyName[FWR_MAXIMUM_KEY_SIZE];
	char pSubPathName[FWR_MAXIMUM_KEY_SIZE];
	BOOL bCloseOnExit = FALSE;

    // Check parameter
    if ( pKeyName==NULL ) {
        return(ERROR_INVALID_PARAMETER);
    }

	if (!IsOpen()) {
		return(ERROR_INVALID_HANDLE);
	}
	const char *separatorPosition = strrchr(pKeyName, '\\');
	if (separatorPosition) {
		// We were given a path name with slashes: must open new handle
		DWORD ulOptions=0;
		bCloseOnExit = TRUE;
		// Fetch name of the last key in the path
		strncpy(pSubKeyName, separatorPosition+1, FWR_MAXIMUM_KEY_SIZE);
		// Fetch name of the sub-path, except the last key
		strncpy(pSubPathName, pKeyName, separatorPosition - pKeyName);
		pSubPathName[separatorPosition - pKeyName]='\0';
		nResult=RegOpenKeyEx(m_pKey,           // handle to base key
							 pSubPathName,     // name of subkey to open
							 ulOptions,        // reserved
							 KEY_SET_VALUE,    // security access mask
							 &tempKeyHandle    // handle of opened key
							 );
	} else {
		// We were given a direct value name: reuse currently open handle
		tempKeyHandle = m_pKey;
		strncpy(pSubKeyName, pKeyName, FWR_MAXIMUM_KEY_SIZE);
	}

	if (nResult == ERROR_SUCCESS) {
		nResult = RegSetValueEx(tempKeyHandle,
								pSubKeyName,
								nReserved,
								nType,
								pValueBuffer,
								nValueBufferSize);
	} else {
		// Force WIN32 error (it is not set by default).
		SetLastError(nResult);
	}

	if (bCloseOnExit) {
		RegCloseKey(tempKeyHandle);
	}

	return(nResult);
}

const char *CWindowsRegistry::GetKeyName(void) const
{
	return(m_pSubKeyName);
}

BOOL CWindowsRegistry::EnumerateSubKeys(const DWORD subkeyIndex, char * pSubKeyName)
{
	LONG nResult=ERROR_SUCCESS;

	DWORD size_of_subkey_name_string = FWR_MAXIMUM_KEY_SIZE;

	::ZeroMemory(pSubKeyName,FWR_MAXIMUM_KEY_SIZE);

	nResult = RegEnumKey(m_pKey, 
                         subkeyIndex, 
                         pSubKeyName, 
                         size_of_subkey_name_string
						);
	if ( nResult == ERROR_SUCCESS )
	{
		// There are still subkeys
		return( TRUE );
	}
	else
	{
		// No more subkeys
		return( FALSE );
	}
}

BOOL CWindowsRegistry::EnumerateValues( const DWORD subValueIndex, char *pSubValueName)
{
	LONG nResult=ERROR_SUCCESS;

	DWORD size_of_subkey_name_string = FWR_MAXIMUM_KEY_SIZE;

	::ZeroMemory(pSubValueName,FWR_MAXIMUM_KEY_SIZE);

	nResult = RegEnumValue(m_pKey, 
						   subValueIndex, 
                           pSubValueName, 
                           &size_of_subkey_name_string,
						   NULL,   // Reserved
						   NULL,   // value type: not required
						   NULL,   // value data: not required
						   NULL    // value data size: not required
						   );
	if ( nResult == ERROR_SUCCESS )
	{
		// There are still subkeys
		return( TRUE );
	}
	else
	{
		// No more subkeys
		return( FALSE );
	}
}

LONG CWindowsRegistry::CopyKeys(LPCTSTR srcKey, LPCTSTR dstKey)
{
	LONG nResult=ERROR_SUCCESS;

  // Open the source
	CWindowsRegistry srcRegistry;
	nResult = srcRegistry.OpenKey(srcKey, NULL, KEY_READ);
  if (nResult != ERROR_SUCCESS) {
	  // Error while opening source
		return(nResult);
	}
	// Open or create the destination
	CWindowsRegistry dstRegistry;
	nResult = dstRegistry.OpenKey(dstKey, NULL, KEY_WRITE);
	if (nResult != ERROR_SUCCESS) {
		nResult = dstRegistry.CreateKey(dstKey, NULL, KEY_WRITE);
		if (nResult != ERROR_SUCCESS) {
			// Can't open & can't create: error
			return(nResult);
		}
	}

	// Loop on all subkeys
	char srcSubKey[FWR_MAXIMUM_KEY_SIZE];
	char fullDstKey[FWR_MAXIMUM_KEY_SIZE];
	char fullSrcKey[FWR_MAXIMUM_KEY_SIZE];
  for (int i=0; srcRegistry.EnumerateSubKeys(i, srcSubKey)==TRUE; i++) {
	  strcpy(fullDstKey, dstKey);
		strcat(fullDstKey, "\\");
		strcat(fullDstKey, srcSubKey);
	  strcpy(fullSrcKey, srcKey);
		strcat(fullSrcKey, "\\");
		strcat(fullSrcKey, srcSubKey);
		// Recursive call for subkeys
		nResult=CopyKeys(fullSrcKey, fullDstKey);
		if (nResult != ERROR_SUCCESS) {
			return(nResult);
		}
  }
	// Copy values
  for (int j=0; srcRegistry.EnumerateValues(j, srcSubKey)==TRUE; j++) {
		// Copy value
    char *pStringValue;
    DWORD IntegerValue;

		nResult = srcRegistry.OpenKey(srcKey,NULL,KEY_READ);
    if (nResult == ERROR_SUCCESS) {
			nResult = dstRegistry.OpenKey(dstKey,NULL,KEY_WRITE);
			if (nResult == ERROR_SUCCESS) {
				// Reads the value (if it's a string)
				nResult = srcRegistry.QueryValue(srcSubKey,&pStringValue);
				if (nResult == ERROR_SUCCESS) {
					nResult = dstRegistry.WriteValue(srcSubKey, pStringValue);
					if (pStringValue) {
						delete [] pStringValue;
					}
				} else {
					// Reads the value (if it's an integer)
					nResult = srcRegistry.QueryValue(srcSubKey,IntegerValue);
					if (nResult == ERROR_SUCCESS) {
						nResult = dstRegistry.WriteValue(srcSubKey, IntegerValue);
					} else {
						return(nResult);
					}
				}
			}
		}
	}

	return(nResult);
}
