// Code.cpp: implementation of the CCode2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Code2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCode2::CCode2()
{

}

CCode2::~CCode2()
{ 

}

CString CCode2::Encode(CString Input)
{
	int len;
	len = Input.GetLength();
	char temp;

	CString word,first,second,result;
	
	for(int i = 0; i < len; i++)
	{
		temp = Input[i];	//ascii값으로 ..
		word = OutBin((int)temp);	//8bit의 2진수
								//앞 뒤 4자리 씩 자른다.
		first = word.Mid(0,4);
		second = word.Mid(4,4);
			
		result += Bin_To_Dec(first,second,len);//	
	}
	
	return result;
}

CString CCode2::OutBin(int asc) //ascii값을 8bit 2진수로 바꿈.
{
	int n = asc;
	char bit[8];
	CString str;
	int s = 8;
	int i = -1;

	/*2진수로 바꾸어 앞에서 뒤로 순서를 바꾼다 .. 예로 .. 10001101 --> 10110001로 */
	do {
		itoa(asc%2,&bit[++i],2);
		asc /= 2;
	} while (asc >= 1);

	while (i < s-1)
		bit[++i] = '0';
	bit[i+1] = '\0';
	str = bit;
	return str;
}

CString CCode2::Bin_To_Dec(CString fis, CString sed, int len) //2진수를 10진수로 
//그리고 길이를 더한다.
{
	CString first,second,result;

	int fi = 0,se = 0,i,j;
	int x ;
	first = fis; 	second = sed;
	char temp1,temp2;
	
	for (  i = 0, j = 0 , x = 3 ; i < 4 ,j < 4 ; i++,j++, x--) 
	{
		temp1 = first.GetAt(i);
		temp2 = second.GetAt(i);
		/*if(temp1 == '1')
			fi += sq(j);
		if(temp2 == '1')
			se += sq(j);
			*/
		if(temp1 == '1')
			fi += sq(x);
		if(temp2 == '1')
			se += sq(x);

	}
	se += fi;
	/*webcall...*/
//	fi += len;
//	se += len;
	
	return Make_One_Code(fi,se); 
}

CString CCode2::Make_One_Code(int a, int b) 
{
	char fi[10],se[10],temp,str[10];
	_itoa(a,fi,10);
	_itoa(b,se,10);
	if(a < 10 )
	{
		temp = fi[0];
		fi[0] = '0';
		fi[1] = temp;
		fi[2] = '\0';
	}	
	if(b < 10)
	{
		
		temp = se[0];
		se[0] = '0';
		se[1] = temp;
		se[2] = '\0';
	}

	strcpy(str,fi);
	strcat(str,se);
	return str;
}


///*Decode **********************/
CString CCode2::Decode(CString code)
{
	int len ;
	CString four_ch, result;
	len = code.GetLength();
	len = len/4;  // 글자 수 
	for(int i = 0,j = 0 ; j < len ; j++ )
	{
		four_ch = code.Left(4);
		code.Delete(0,4);
		result += Make_One_char(four_ch,len);
		i += 4;
		
	}
	return result;
}


CString CCode2::Make_One_char(CString digit,int len)
{

	CString result, fi,se;
	int fi_i, se_i;
	fi = digit.Mid(0,2);
	se = digit.Mid(2,2);

	//TRACE("\n%s\n", digit);
	fi_i = atoi(fi); 
	se_i = atoi(se);
		
	//webcall..
//	fi_i -= len; 
//	se_i -= len; 
	if(se_i > fi_i)
		se_i -= fi_i;
	else
		return _T("");

	
	fi = Make_Bin(fi_i); 
	se = Make_Bin(se_i);


	result = fi+se;
	
//	AfxMessageBox(result);
	result.MakeReverse();
	int a = Make_Dec(result);
	char b[1];
	b[0] = (char)a;
	b[1] = '\0';
	result = b;
	return result;
}

CString CCode2::Make_Bin(int a)
{
	char bit[4];
	CString str;
	int s = 4;
	int i = -1;

	/*2진수로 바꾸어 앞에서 뒤로 순서를 바꾼다 .. 예로 .. 10001101 --> 10110001로 */
	do {
		itoa(a%2,&bit[++i],2);
		a /= 2;
	} while (a >= 1);

	while (i < s-1)
		bit[++i] = '0';
	bit[i+1] = '\0';
	str = bit;
	str.MakeReverse();
	return str;

}


int CCode2::Make_Dec(CString bin)
{
	//bin.MakeReverse();

	char bit[9];
	int result = 0;
	strcpy(bit,bin);
	
	for(int i = 7 ,  j = 0; i > 0 ;i--,j++)
	{
		if(bit[i] == '1')
			result += sq(j);
	}
	return result;
}

int CCode2::sq(int i) //sqare 2의 승수 구하기
{
	int n = 1 ;
	if( i == 0 )
		return 1;
	else {
		for( int x = 0; x < i ; x++)
			n *= 2;
		return n;
	}
}
