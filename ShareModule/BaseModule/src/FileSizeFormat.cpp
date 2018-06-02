#include "StdAfx.h"
#include "FileSizeFormat.h"

CFileSizeFormat::CFileSizeFormat(void)
{
}

CFileSizeFormat::~CFileSizeFormat(void)
{
}

/**
 * Converts a positive number to a string while inserting separators.
 *
 * @param dwNumber A positive number to add thousands separator for
 *
 * @return The number with thousand separators as a CString
 */
CString CFileSizeFormat::s_InsertSeparator (DWORD dwNumber)
{
	CString str;

	str.Format(_T("%u"), dwNumber);

	for (int i = str.GetLength()-3; i > 0; i -= 3)
	{
		str.Insert(i, _T(","));
	}

	return (str);
}


//-- Global Functions --

/**
 * Converts a filesize to a human readable format.
 *
 * This involves the use of K and M as multipliers.  Hence,
 * strings are formatted as 1024 -> 1KB, 1023 -> 1,023 B,
 * and so on.
 *
 * @param dwFileSize File size to be formatted.
 *
 * @return The formatted filesize as a CString
 */
CString CFileSizeFormat::s_Format (DWORD dwFileSize)
{
  static const DWORD dwKB = 1024;          // Kilobyte
  static const DWORD dwMB = 1024 * dwKB;   // Megabyte
  static const DWORD dwGB = 1024 * dwMB;   // Gigabyte

  DWORD dwNumber, dwRemainder;
  CString strNumber;

  if (dwFileSize < dwKB)
  {
	  strNumber = s_InsertSeparator(dwFileSize) + _T(" B");
  } 
  else
  {
    if (dwFileSize < dwMB)
    {
      dwNumber = dwFileSize / dwKB;
      dwRemainder = (dwFileSize * 100 / dwKB) % 100;
	  strNumber.Format(_T("%s.%02d KB"), s_InsertSeparator(dwNumber), dwRemainder);
    }
    else
    {
      if (dwFileSize < dwGB)
      {
        dwNumber = dwFileSize / dwMB;
        dwRemainder = (dwFileSize * 100 / dwMB) % 100;
        strNumber.Format(_T("%s.%02d MB"), s_InsertSeparator(dwNumber), dwRemainder);
      }
      else
      {
        if (dwFileSize >= dwGB)
        {
          dwNumber = dwFileSize / dwGB;
          dwRemainder = (dwFileSize * 100 / dwGB) % 100;
          strNumber.Format(_T("%s.%02d GB"), s_InsertSeparator(dwNumber), dwRemainder);
        }
      }
    }
  }

  // Display decimal points only if needed
  // another alternative to this approach is to check before calling str.Format, and 
  // have separate cases depending on whether dwRemainder == 0 or not.
  strNumber.Replace(_T(".00"), _T(""));

  return strNumber;
}


//-----------------------------------------------
//					the end
//-----------------------------------------------
