/*=========================================================================== 
    (c) Copyright 1999, Emmanuel KARTMANN, all rights reserved                 
  =========================================================================== 
    File           : WindowsRegistry.h
    $Header: $
    Author         : Emmanuel KARTMANN
    Creation       : Tuesday 9/21/99 2:44:27 PM
    Remake         : 
  ------------------------------- Description ------------------------------- 

           Provides an easier access to the Windows Registry

  ------------------------------ Modifications ------------------------------ 
    $Log: $  
  =========================================================================== 
*/


#if !defined(AFX_CWindowsRegistry_H__597585E1_4D46_11D2_BFFA_00E02924E982__INCLUDED_)
#define AFX_CWindowsRegistry_H__597585E1_4D46_11D2_BFFA_00E02924E982__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/* Enforce strict type checking for Windows Types */
#ifndef STRICT
#define STRICT 1
#endif /* STRICT */

#ifdef _AFXDLL
#include <afxwin.h>
#else
#include <windows.h>
#endif /* _AFXDLL */
#include <winreg.h>

/* -----------------------------------------------------------------
CLASS

    CWindowsRegistry

    Provides an easier access to the Windows Registry.

DESCRIPTION

    This class provides an easy access to the Windows Registry
    (much easier than the WIN32 API).

USAGE

    To access the Windows Registry via this class, follow these steps:
    <UL>
    <LI>Create an instance of the class
    <LI>Open the registry with a given key name (and optionally key base)
        with function OpenKey
    <LI>Get a key value with function QueryValue
    <LI>Set a key value with function WriteValue
    <LI>Create a new key with function CreateKey
    <LI>Loop on all subkeys with function EnumerateSubKeys
    <LI>Loop on all values with function EnumerateValues
    <LI>Close the registry with function CloseKey
    </UL>

    There's a class constructor which automatically opens the registry
    (the key must exists).

    The class destructor automatically closes the registry.

    When you delete a key, this class can <B>recursively delete all sub-keys</B>
    (on Windows NT, the WIN32 function ::RegDeleteKey does NOT recursively
    deletes the keys) if you specify the proper parameter (see function
    DeleteKey()).

    You can keep a single instance of this class for a given 
    application and use only function QueryValue to access the registry
    (if you don't need to switch to another registry base key).

EXAMPLE

    <PRE>

    // At the application initialization: create object (without opening registry)
    CWindowsRegistry MyRegistry;

    // Open the registry via the object instance
    if (!MyRegistry.IsOpen()) {
        // Build key name
        CString KeyName = "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0";

        // Open Registry (note: the base key "LOCAL_MACHINE" is part of the KeyName)
        LONG nResult=MyRegistry.OpenKey(KeyName);
        if (nResult != ERROR_SUCCESS) {
            // Here we could CREATE the registry entries for the application
            cerr << "Cannot open registry!\n";
        }
    }


    [...]


    // At some point in the application, read values (several times, if needed)
    char *TemporaryValue;
    CString RealValue;
    BOOL bTemporaryValue,bRealValue;
    int nTemporaryValue,nRealValue;
    
    // Reads the value in "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0\\MyStringKey"
    if (MyRegistry.QueryValue("MyStringKey",&TemporaryValue) == ERROR_SUCCESS) {
        RealValue=TemporaryValue;
        // For a string value, the pointer has been allocated: do not forget to free it
        if (TemporaryValue) {
            // <FONT COLOR=red><U>CAUTION:</U></FONT> use <B>delete []</B>, not free or LocalFree
            delete [] TemporaryValue;
        }
    }

    // Reads the value in "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0\\MyBooleanKey"
    if (MyRegistry.QueryValue("MyBooleanKey",bTemporaryValue) == ERROR_SUCCESS) {
        bRealValue=bTemporaryValue;
    }

    // Reads the value in "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0\\MyIntegerKey"
    if (MyRegistry.QueryValue("MyIntegerKey",nTemporaryValue) == ERROR_SUCCESS) {
        nRealValue=nTemporaryValue;
    }


    [...]


    // At some point in the application, write values (several times, if needed)
    LPCTSTR RealValue = "MyString";
    BOOL bRealValue = TRUE;
    int nRealValue = 357;

    if (MyRegistry.WriteValue("MyStringKey", RealValue) != ERROR_SUCCESS) {
        // Process Write Error...
    }

    if (MyRegistry.WriteValue("MyBooleanKey", bRealValue) != ERROR_SUCCESS) {
        // Process Write Error...
    }

    if (MyRegistry.WriteValue("MyIntegerKey", nRealValue) != ERROR_SUCCESS) {
        // Process Write Error...
    }


    [...]

    // At some point in the application (usually startup), copy keys from LOCAL_MACHINE to CURRENT_USER
    LONG nResult = ERROR_SUCCESS;
    nResult = CWindowsRegistry::CopyKeys("\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0", 
                                         "\\HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\MSMQ\\2.0");

    if (nResult != ERROR_SUCCESS) {
        // Process Copy error
    }

    [...]


    // At some point in the application, create a new SUBKEY
    // Create the key "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0\\SubKey1"
    MyRegistry.CreateKey("SubKey1");
    // Note that the currently open key is still "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\1.0"


    [...]


    // At some point in the application, create a complete new key
    MyRegistry.CloseKey();
    MyRegistry.CreateKey("\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\Dummy\\1.0");
    // We must open this key if we want to use it
    MyRegistry.OpenKey("\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\Dummy\\1.0");

    [...]

    // At some point in the application, loop on all subkeys
    char subKey[FWR_MAXIMUM_KEY_SIZE];
    for (int i=0; MyRegistry.EnumerateSubKeys(i, subKey)==TRUE; i++) {
        // Here you should see "SubKey1"
        cout << "Key #" << i << ": " << subKey << eol;
    }

    [...]

    // At the end of the application: close registry
    // Note that the object's destructor automatically closes the registry
    MyRegistry.CloseKey();

    </PRE>

TO DO

    This class may be improved to:
    <UL>
     <LI> connect to a remote computer to access its registry
     <LI> handle MFC types (or be embedded in another C++ class with MFC support;
          especially usefull for CString and CStringArray)
     <LI> handle STL types ('string' and 'list<string>')
     <LI> handle ATL types (BSTR...)
    </UL>

ADMINISTRATIVE

  Author     Emmanuel KARTMANN

  Date       Tuesday 9/21/99 2:47:45 PM

SEE ALSO

    RegOpenKeyEx RegCloseKey RegQueryValueEx RegCreateKeyEx

----------------------------------------------------------------- */
class CWindowsRegistry
{
public:
/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> create an instance of the class (empty)
//
// <U>Parameters:</U> none
//
// <U>Return value :</U> none
//
// <U>Description  :</U> Before using this instance, the application
//                       must call function OpenKey().
//
    CWindowsRegistry();

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> create an instance of the class and opens
//                 the registry.
//
// <U>Parameters:</U> 
//
//       [in] pSubKeyName
//                name of the subkey, e.g. 
//                "SYSTEM\\CurrentControlSet\\Services\\"
//       [in] pBaseKey
//                Handle to a currently open key or any of the 
//                predefined reserved handle values (see function
//                OpenKey() for a list).
//       [in] SecurityAccessMask
//                Security access mask for the key to open. (see
//                function OpenKey() for a list).
//
// <U>Return value :</U> none
//
// <U>Description  :</U> calls function OpenKey() with the given parameters
//
    CWindowsRegistry(LPCTSTR pSubKeyName, HKEY pBaseKey = NULL, REGSAM SecurityAccessMask = KEY_QUERY_VALUE);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> delete an instance of the class
//
// <U>Parameters:</U> none
//
// <U>Return value :</U> none
//
// <U>Description  :</U> This function simply close the registry 
//                       (see function CloseKey())
//
    virtual ~CWindowsRegistry();


/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> open the specified key in the Windows Registry
//
// <U>Parameters:</U> 
//
//       [in] pSubKeyName
//                name of the subkey, e.g. 
//                "SYSTEM\\CurrentControlSet\\Services\\"
//       [in] pBaseKey
//                Handle to a currently open key or any of the 
//                following predefined reserved handle values: 
//                <UL>
//                    <LI>NULL<B><EM>[the default]</EM></B>
//                    <LI>HKEY_CLASSES_ROOT
//                    <LI>HKEY_CURRENT_CONFIG
//                    <LI>HKEY_CURRENT_USER
//                    <LI>HKEY_LOCAL_MACHINE
//                    <LI>HKEY_USERS
//                </UL>
//       [in] SecurityAccessMask
//                Security access mask for the key to open.
//                This parameter can be a combination of the 
//                following values: 
//                <UL>
//                    <LI>KEY_ALL_ACCESS: Combination of KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, KEY_CREATE_SUB_KEY, KEY_CREATE_LINK, and KEY_SET_VALUE access.
//                    <LI>KEY_CREATE_LINK: Permission to create a symbolic link.
//                    <LI>KEY_CREATE_SUB_KEY: Permission to create subkeys.
//                    <LI>KEY_ENUMERATE_SUB_KEYS: Permission to enumerate subkeys.
//                    <LI>KEY_EXECUTE: Permission for read access.
//                    <LI>KEY_NOTIFY: Permission for change notification.
//                    <LI>KEY_QUERY_VALUE: Permission to query subkey data. <B><EM>[the default]</EM></B>
//                    <LI>KEY_READ: Combination of KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and KEY_NOTIFY access.
//                    <LI>KEY_SET_VALUE: Permission to set subkey data.
//                    <LI>KEY_WRITE: Combination of KEY_SET_VALUE and KEY_CREATE_SUB_KEY access.
//                </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> this function open the registry and keeps a handle
//                       in the member variable m_pKey for further use with
//                       the other member functions: QueryValue, WriteValue...
//
//                       Unlike the CreateKey function, the OpenKey function 
//                       does not create the specified key if the key does not
//                       exist in the registry (it issues an error).
//
//                       If the <B>pBaseKey</B> is NULL, this function reuse the
//                       handle previously open (it opens the subkey 
//                       <B>pSubKeyName</B>).
//
    LONG OpenKey(LPCTSTR pSubKeyName, HKEY pBaseKey = NULL, REGSAM SecurityAccessMask = KEY_QUERY_VALUE);


/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> check if the registry is open
//
// <U>Parameters:</U> none
//
// <U>Return value :</U> BOOL 
//
// <U>Description  :</U> if the registry is not open, many function
//                       are not available: QueryValue, WriteValue...
//
    BOOL IsOpen();

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> close the registry (if opened with function
//                 OpenKey()).
//
// <U>Parameters:</U> none
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> -
//
    LONG CloseKey();


/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> create a key in the registry
//
// <U>Parameters:</U> 
//
//       [in] pSubKeyName
//                name of the subkey, e.g. 
//                "SYSTEM\\CurrentControlSet\\Services\\MyService\\Parameters"
//       [in] pBaseKey 
//                Handle to a currently open key or any of the 
//                following predefined reserved handle values: 
//                <UL>
//                    <LI>NULL<B><EM>[the default]</EM></B>
//                    <LI>HKEY_CLASSES_ROOT
//                    <LI>HKEY_CURRENT_CONFIG
//                    <LI>HKEY_CURRENT_USER
//                    <LI>HKEY_LOCAL_MACHINE
//                    <LI>HKEY_USERS
//                </UL>
//       [in] nSecurityAccessMask 
//                Security access mask for the key to open.
//                This parameter can be a combination of the 
//                following values: 
//                <UL>
//                    <LI>KEY_ALL_ACCESS: Combination of KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, KEY_CREATE_SUB_KEY, KEY_CREATE_LINK, and KEY_SET_VALUE access.  <B><EM>[the default]</EM></B>
//                    <LI>KEY_CREATE_LINK: Permission to create a symbolic link.
//                    <LI>KEY_CREATE_SUB_KEY: Permission to create subkeys.
//                    <LI>KEY_ENUMERATE_SUB_KEYS: Permission to enumerate subkeys.
//                    <LI>KEY_EXECUTE: Permission for read access.
//                    <LI>KEY_NOTIFY: Permission for change notification.
//                    <LI>KEY_QUERY_VALUE: Permission to query subkey data.
//                    <LI>KEY_READ: Combination of KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and KEY_NOTIFY access.
//                    <LI>KEY_SET_VALUE: Permission to set subkey data.
//                    <LI>KEY_WRITE: Combination of KEY_SET_VALUE and KEY_CREATE_SUB_KEY access.
//                </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> 
//                       If the <B>pBaseKey</B> is NULL, this function reuse the
//                       handle previously open (it creates the subkey 
//                       <B>pSubKeyName</B>).
//
//
    LONG CreateKey(LPCTSTR pSubKeyName, HKEY pBaseKey = NULL, REGSAM nSecurityAccessMask = KEY_ALL_ACCESS);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> delete a key from the registry
//
// <U>Parameters:</U> 
//
//       [in] pSubKeyName
//                name of the subkey, e.g. 
//                "SYSTEM\\CurrentControlSet\\Services\\MyService\\Parameters"
//       [in] pBaseKey 
//                Handle to a currently open key or any of the 
//                following predefined reserved handle values: 
//                <UL>
//                    <LI>NULL<B><EM>[the default]</EM></B>
//                    <LI>HKEY_CLASSES_ROOT
//                    <LI>HKEY_CURRENT_CONFIG
//                    <LI>HKEY_CURRENT_USER
//                    <LI>HKEY_LOCAL_MACHINE
//                    <LI>HKEY_USERS
//                </UL>
//       [in] bRecursive 
//                Boolean Flag: TRUE if the deletion must be recursive.
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U>
//
//                       If the <B>pBaseKey</B> is NULL, this function reuse the
//                       handle previously open (it opens the subkey 
//                       <B>pSubKeyName</B>).
//
//
    LONG DeleteKey(LPCTSTR pSubKeyName, HKEY pBaseKey = NULL, BOOL bRecursive=TRUE);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> get a value in the registry (a string)
//
// <U>Parameters:</U> 
//
//       [in] pValueName
//                name of the value
//       [out] readValue
//                value read (a string)
//       [out] pValueType 
//                effective type of value, i.e.:
//             <UL>
//                 <LI>REG_SZ: A null-terminated string.
//                 <LI>REG_EXPAND_SZ: A null-terminated string 
//                     that contains unexpanded references to 
//                     environment variables (for example, "%PATH%").
//             </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> If the registry is not open (via function
//                       OpenKey()), this function returns 
//                       "ERROR_INVALID_HANDLE".
//
//                       If the key type is not compatible with a
//                       string, this function returns "ERROR_INVALID_DATA"
//
//                       This function allocates memory for its result
//                       (readValue). The calling application is responsible
//                       for freeing this memory (delete [] (*readValue)).
//
    LONG QueryValue(LPCTSTR pValueName, char **readValue, LPDWORD pValueType = NULL);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> get a value in the registry (an integer)
//
// <U>Parameters:</U> 
//
//       [in] pValueName
//                name of the value
//       [out] readValue
//                value read (a 32-bit integer)
//       [out] pValueType 
//                effective type of value, i.e.:
//             <UL>
//                 <LI>REG_DWORD: A 32-bit number.
//                 <LI>REG_DWORD_LITTLE_ENDIAN: A 32-bit number 
//                         in little-endian format. This is 
//                         equivalent to REG_DWORD.
//                 <LI>REG_DWORD_BIG_ENDIAN: A 32-bit number in 
//                         big-endian format. 
//             </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> If the registry is not open (via function
//                       OpenKey()), this function returns 
//                       "ERROR_INVALID_HANDLE".
//
//                       If the key type is not compatible with an
//                       integer, this function returns "ERROR_INVALID_DATA"
//
    LONG QueryValue(LPCTSTR pValueName, DWORD &readValue, LPDWORD pValueType = NULL);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> get a value in the registry (an array of strings)
//
// <U>Parameters:</U> 
//
//       [in] pValueName
//                name of the value
//       [out] readValue
//                value read (an array of char *)
//       [out] KeyArraySize
//                size of the returned array
//       [out] pValueType 
//                effective type of value, i.e.:
//             <UL>
//                 <LI>REG_MULTI_SZ
//             </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> <FONT COLOR=RED><BLINK>This function is not implemented yet.</BLINK></FONT>
//
//                       If the registry is not open (via function
//                       OpenKey()), this function returns 
//                       "ERROR_INVALID_HANDLE".
//
//                       If the key type is not compatible with an
//                       array of string objects, this function 
//                       returns "ERROR_INVALID_DATA"
//
//                       This function allocates memory for its result
//                       (readValue). The calling application is responsible
//                       for freeing this memory (loop on KeyArraySize and
//                       delete [] (readValue[x])).
//
    LONG QueryValue(LPCTSTR pValueName, char **readValue, int &KeyArraySize, LPDWORD pValueType = NULL);


/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> set a value in the registry (a string)
//
// <U>Parameters:</U> 
//
//       [in] pValueName
//                name of the value
//       [in] pValueToWrite
//                value yo write (a string)
//       [in] ValueType 
//                effective type of value, i.e.:
//             <UL>
//                 <LI>REG_SZ: A null-terminated string.
//                 <LI>REG_EXPAND_SZ: A null-terminated string 
//                     that contains unexpanded references to 
//                     environment variables (for example, "%PATH%").
//             </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> If the registry is not open (via function
//                       OpenKey()), this function returns 
//                       "ERROR_INVALID_HANDLE".
//
//                       If the string pointer is NULL, nothing
//                       is written in the registry (the key is not
//                       created) and the function succeeds.
//
//                       If the value name is a relative path (if it contains
//                       backslashes), this function will write the value in
//                       the appropriate sub-keys (unlike the WIN32 function
//                       ::RegSetValueEx which doesn't support sub-key names).
//
//                       If the value name is an empty string (""), the default
//                       value of the opened KEY will be set.
//
    LONG WriteValue(LPCTSTR pValueName, LPCTSTR pValueToWrite, DWORD nValueType = REG_SZ);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> set a value in the registry (an integer)
//
// <U>Parameters:</U> 
//
//       [in] pValueName
//                name of the value
//       [in] valueToWrite
//                value to write (an 'int')
//       [in] ValueType 
//                effective type of value, i.e.:
//             <UL>
//                 <LI>REG_DWORD: A 32-bit number.
//                 <LI>REG_DWORD_LITTLE_ENDIAN: A 32-bit number 
//                         in little-endian format. This is 
//                         equivalent to REG_DWORD.
//                 <LI>REG_DWORD_BIG_ENDIAN: A 32-bit number in 
//                         big-endian format. 
//             </UL>
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> If the registry is not open (via function
//                       OpenKey()), this function returns 
//                       "ERROR_INVALID_HANDLE".
//
//                       If the key name is a relative path (if it contains
//                       backslashes), this function will write the value in
//                       the appropriate sub-keys (unlike the WIN32 function
//                       ::RegSetValueEx which doesn't support sub-key names).
//
    LONG WriteValue(LPCTSTR pValueName, int valueToWrite, DWORD nValueType = REG_DWORD);


/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> list all keys located under current key
//
// <U>Parameters:</U> 
//
//       [in] subkeyIndex
//                the index of the subkey to retrieve (this value 
//                should be zero for the first call to this function 
//                and then incremented for subsequent calls).
//       [in/out] pSubKeyName
//                pointer to a buffer that receives the name of the subkey
//
// <U>Return value :</U> BOOL = FALSE if no more keys are found, TRUE otherwise
//
// <U>Description  :</U> The calling application should have allocated
//                       (at least) FWR_MAXIMUM_KEY_SIZE bytes in the
//                       buffer referenced by the pSubKeyName pointer.
//
//                       The values located under the currently open
//                       key are NOT returned by this function (it
//                       only enumerates the KEYS, not the VALUES). To
//                       enumerate the values, use EnumerateValues instead.
//
    BOOL EnumerateSubKeys( const DWORD subkeyIndex, char *pSubKeyName);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> list all values located under current key
//
// <U>Parameters:</U> 
//
//       [in] subValueIndex
//                the index of the subvalue to retrieve (this value 
//                should be zero for the first call to this function 
//                and then incremented for subsequent calls).
//       [in/out] pSubValueName
//                pointer to a buffer that receives the name of the value
//
// <U>Return value :</U> BOOL  = FALSE if no more values are found, TRUE otherwise
//
// <U>Description  :</U> The calling application should have allocated
//                       (at least) FWR_MAXIMUM_KEY_SIZE bytes in the
//                       buffer referenced by the pSubValueName pointer.
//
//                       The keys located under the currently open
//                       key are NOT returned by this function (it
//                       only enumerates the VALUES, not the KEYS). To
//                       enumerate the keys, use EnumerateSubKeys instead.
//
    BOOL EnumerateValues( const DWORD subValueIndex, char *pSubValueName);

/////////////////////////////////////////////////////////////////////
//
// <U>Purpose:</U> Copy a registry hierarchy from one key to another
//
// <U>Parameters:</U> 
//
//       [in] srcKey
//               the source key from which is loaded the hierarchy
//       [in] destKey
//               the destination key where to copy the hierarchy
//
// <U>Return value :</U> LONG = ERROR_SUCCESS in case of success, 
//                              otherwise an error occured.
//
// <U>Description  :</U> This method is static.
//
//                       The keys are copied recursively (i.e. 
//                       subkeys AND values are copied).
//
//                       If the destination key doesn't exists,
//                       it is created by this function.
//
    static LONG CopyKeys( LPCTSTR srcKey, LPCTSTR destKey );

/////////////////////////////////////////////////////////////////////
//
//  Maximum size of the buffer during a WRITE access to the registry
//  (function WriteValue()).
//
//  I created this variable because the WIN32 API recommend that the
//  data stored in the Registry shouldn't exceed 2048 bytes
//  (bigger data should be stored in files instead).
//
    static const size_t MaximumWriteSize;

/////////////////////////////////////////////////////////////////////
//
//  Maximum size of the key name.
//
#define FWR_MAXIMUM_KEY_SIZE 2048

    const char *GetKeyName(void) const;

protected:
    static LONG RecursivelyDeleteAllSubKeys( HKEY key_handle, LPCTSTR key_name );
    typedef struct _BaseNameEntry {
        const char *String;
        HKEY Handle;
    } BaseNameEntry;
    static BaseNameEntry m_pBaseNameMap[];
    LONG m_nErrorCode;
    REGSAM m_nSecurityAccessMask;
    LPCTSTR m_pSubKeyName;
    HKEY m_pBaseKey;
    HKEY m_pKey;
    LONG RawQueryValue(LPCTSTR pValueName, LPBYTE pValueBuffer, DWORD *pValueBufferSize, DWORD *pType, DWORD pSupportedTypes[], size_t nNumberOfTypes);
    LONG RawWriteValue(LPCTSTR pValueName, LPBYTE pValueBuffer, DWORD nValueBufferSize, DWORD nType);

};

#endif // !defined(AFX_CWindowsRegistry_H__597585E1_4D46_11D2_BFFA_00E02924E982__INCLUDED_)
