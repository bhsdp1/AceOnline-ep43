// Code2.h: interface for the CCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODE2_H__D9EC0D71_2A9E_4B2B_A01B_313F17EFD728__INCLUDED_)
#define AFX_CODE2_H__D9EC0D71_2A9E_4B2B_A01B_313F17EFD728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCode2  
{
public:
	CString Encode(CString Input);
	int Make_Dec(CString bin);
	CString Make_Bin(int a);
	CString Make_One_char(CString digit, int len);
	CString Decode(CString code);
	int sq(int i);
	CString Make_One_Code(int a, int b);
	CString Bin_To_Dec(CString fis,CString sed, int len);
	CString OutBin(int asc);
	CCode2();
	virtual ~CCode2(); 

};

#endif // !defined(AFX_CODE_H__D9EC0D71_2A9E_4B2B_A01B_313F17EFD728__INCLUDED_)
