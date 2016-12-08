// PaintBMP.h
// http://www.geocities.com/CMyWinApp/

// Handle to a DIB
DECLARE_HANDLE(HDIB);

// DIB constants
#define PALVERSION 0x300

// DIB Macros
#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')
#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect) ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect) ((lpRect)->bottom - (lpRect)->top)

// Function prototypes
DWORD GetDIBWidth(HDIB);
DWORD GetDIBHeight(HDIB);
WORD GetDIBNumColors(HDIB);
HDIB ReadDIBFile(CFile &);
BOOL CreateDIBPalette(HDIB, CPalette *);
BOOL PaintDIB(HDC, LPRECT, HDIB, LPRECT, CPalette *);
WORD DIBNumColors(LPSTR);

DWORD GetDIBWidth(HDIB hDIB)
{
    LPSTR lpDIB;
    LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
    LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
    DWORD cxDIB;
    
    lpDIB = (LPSTR)::GlobalLock((HGLOBAL) hDIB);

    // point to the header (whether Win 3.0 and old)
    lpbmi = (LPBITMAPINFOHEADER)lpDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpDIB;
    
    // return the DIB width if it is a Win 3.0 DIB
    if (IS_WIN30_DIB(lpDIB)) {
        cxDIB = (int)lpbmi->biWidth;
    } else {  // it is an other-style DIB, so return its width
        cxDIB = (int)lpbmc->bcWidth;
    }

    ::GlobalUnlock((HGLOBAL) hDIB);

    return cxDIB;
}

DWORD GetDIBHeight(HDIB hDIB)
{
    LPSTR lpDIB;
    LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
    LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
    DWORD cyDIB;
    
    lpDIB = (LPSTR)::GlobalLock((HGLOBAL) hDIB);

    // point to the header (whether old or Win 3.0
    lpbmi = (LPBITMAPINFOHEADER)lpDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpDIB;
    
    // return the DIB height if it is a Win 3.0 DIB
    if (IS_WIN30_DIB(lpDIB)) {
        cyDIB = (int)lpbmi->biHeight;
    } else { // it is an other-style DIB, so return its height
        cyDIB = (int)lpbmc->bcHeight;
    }

    ::GlobalUnlock((HGLOBAL) hDIB);

    return cyDIB;
}

WORD GetDIBNumColors(HDIB hDIB)
{
    LPSTR lpDIB;
    WORD wNumColors;

    lpDIB = (LPSTR)::GlobalLock((HGLOBAL) hDIB);
    wNumColors = DIBNumColors(lpDIB);
    ::GlobalUnlock((HGLOBAL) hDIB);

    return wNumColors;
}

HDIB ReadDIBFile(CFile &file)
{
    BITMAPFILEHEADER bmfHeader;
    DWORD dwBitsSize;
    HDIB hDIB;
    LPSTR pDIB;
    
    // Get length of DIB in bytes for use when reading
    dwBitsSize = file.GetLength();
    
    // Go read the DIB file header and check if it's valid.
    if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader)) {
        return NULL;
    }
    
    if (bmfHeader.bfType != DIB_HEADER_MARKER) {
        return NULL;
    }
   
    // Allocate memory for DIB
    hDIB = (HDIB)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);

    if (hDIB == 0) {
        return NULL;
    }

    pDIB = (LPSTR)::GlobalLock((HGLOBAL) hDIB);
    
    // Go read the bits.
    if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != dwBitsSize - sizeof(BITMAPFILEHEADER)) {
        ::GlobalUnlock((HGLOBAL) hDIB);
        ::GlobalFree((HGLOBAL) hDIB);
        return NULL;
    }

    ::GlobalUnlock((HGLOBAL) hDIB);

    return hDIB;
}

BOOL CreateDIBPalette(HDIB hDIB, CPalette *pPal)
{
    LPLOGPALETTE lpPal;      // pointer to a logical palette
    HANDLE hLogPal;          // handle to a logical palette
    HPALETTE hPal = NULL;    // handle to a palette
    int i;                   // loop index
    WORD wNumColors;         // number of colors in color table
    LPSTR lpbi;              // pointer to packed-DIB
    LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
    LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
    BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
    BOOL bResult = FALSE;
    
    // if handle to DIB is invalid, return FALSE
    if (hDIB == NULL) {
        return FALSE;
    }
    
    lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
    
    // get pointer to BITMAPINFO (Win 3.0)
    lpbmi = (LPBITMAPINFO)lpbi;
    
    // get pointer to BITMAPCOREINFO (old 1.x)
    lpbmc = (LPBITMAPCOREINFO)lpbi;
    
    // get the number of colors in the DIB
    wNumColors = ::DIBNumColors(lpbi);
    
    if (wNumColors != 0) {
        // allocate memory block for logical palette
        hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * wNumColors);
        
        // if not enough memory, clean up and return NULL
        if (hLogPal == 0) {
            ::GlobalUnlock((HGLOBAL) hDIB);
            return FALSE;
        }
        
        lpPal = (LPLOGPALETTE)::GlobalLock((HGLOBAL) hLogPal);
        
        // set version and number of palette entries
        lpPal->palVersion = PALVERSION;
        lpPal->palNumEntries = (WORD)wNumColors;
        
        // is this a Win 3.0 DIB?
        bWinStyleDIB = IS_WIN30_DIB(lpbi);
        for (i = 0; i < (int)wNumColors; i++) {
            if (bWinStyleDIB) {
                lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
                lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
                lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
                lpPal->palPalEntry[i].peFlags = 0;
            } else {
                lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
                lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
                lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
                lpPal->palPalEntry[i].peFlags = 0;
            }
        }
        
        // create the palette and get handle to it
        bResult = pPal->CreatePalette(lpPal);
        ::GlobalUnlock((HGLOBAL) hLogPal);
        ::GlobalFree((HGLOBAL) hLogPal);
    }
    
    ::GlobalUnlock((HGLOBAL) hDIB);
    
    return bResult;
}

BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect, CPalette* pPal)
{
    LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
    LPSTR    lpDIBBits;           // Pointer to DIB bits
    BOOL     bSuccess=FALSE;      // Success/fail flag
    HPALETTE hPal=NULL;           // Our DIB's palette
    HPALETTE hOldPal=NULL;        // Previous palette
    WORD wPaletteSize;            // Palette Size
    LPBITMAPINFOHEADER lpbmi;     // pointer to a Win 3.0-style DIB
    LPBITMAPCOREHEADER lpbmc;     // pointer to an other-style DIB
    DWORD cyDIB;                  // DIB Height
    LPSTR lpDIB;
    
    // Check for valid DIB handle
    if (hDIB == NULL) {
        return FALSE;
    }
    
    // Lock down the DIB, and get a pointer to
    // the beginning of the bit buffer
    lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

    // calculate the size required by the palette
    if (IS_WIN30_DIB(lpDIBHdr)) {
        wPaletteSize = (WORD)(::DIBNumColors(lpDIBHdr) * sizeof(RGBQUAD));
    } else {
        wPaletteSize = (WORD)(::DIBNumColors(lpDIBHdr) * sizeof(RGBTRIPLE));
    }

    lpDIBBits = lpDIBHdr + *(LPDWORD)lpDIBHdr + wPaletteSize;
    
    // Get the DIB's palette, then select it into DC
    if (pPal != NULL) {
        hPal = (HPALETTE) pPal->m_hObject;
        
        // Select as background since we have
        // already realized in forground if needed
        hOldPal = ::SelectPalette(hDC, hPal, TRUE);
    }
    
    // Make sure to use the stretching mode best for color pictures
    ::SetStretchBltMode(hDC, COLORONCOLOR);
    
    lpDIB = (LPSTR)::GlobalLock((HGLOBAL) hDIB);

    // point to the header (whether old or Win 3.0
    lpbmi = (LPBITMAPINFOHEADER)lpDIB;
    lpbmc = (LPBITMAPCOREHEADER)lpDIB;
    
    // return the DIB height if it is a Win 3.0 DIB
    if (IS_WIN30_DIB(lpDIB)) {
        cyDIB = (int)lpbmi->biHeight;
    } else { // it is an other-style DIB, so return its height
        cyDIB = (DWORD)lpbmc->bcHeight;
    }

    ::GlobalUnlock((HGLOBAL) hDIB);

    // Determine whether to call StretchDIBits() or SetDIBitsToDevice()
    if ((RECTWIDTH(lpDCRect) == RECTWIDTH(lpDIBRect)) && (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect))) {
        bSuccess = ::SetDIBitsToDevice(hDC, // hDC
        lpDCRect->left,                     // DestX
        lpDCRect->top,                      // DestY
        RECTWIDTH(lpDCRect),                // nDestWidth
        RECTHEIGHT(lpDCRect),               // nDestHeight
        lpDIBRect->left,                    // SrcX
        (int)cyDIB - lpDIBRect->top - RECTHEIGHT(lpDIBRect), // SrcY
        0,                                  // nStartScan
        (WORD)cyDIB,                        // nNumScans
        lpDIBBits,                          // lpBits
        (LPBITMAPINFO)lpDIBHdr,             // lpBitsInfo
        DIB_RGB_COLORS);                    // wUsage
    } else {
        bSuccess = ::StretchDIBits(hDC, // hDC
        lpDCRect->left,                 // DestX
        lpDCRect->top,                  // DestY
        RECTWIDTH(lpDCRect),            // nDestWidth
        RECTHEIGHT(lpDCRect),           // nDestHeight
        lpDIBRect->left,                // SrcX
        (int)cyDIB - lpDIBRect->top - RECTHEIGHT(lpDIBRect), // SrcY
        RECTWIDTH(lpDIBRect),           // wSrcWidth
        RECTHEIGHT(lpDIBRect),          // wSrcHeight
        lpDIBBits,                      // lpBits
        (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
        DIB_RGB_COLORS,                 // wUsage
        SRCCOPY);                       // dwROP
    }
    
    ::GlobalUnlock((HGLOBAL) hDIB);
    
    // Reselect old palette
    if (hOldPal != NULL) {
        ::SelectPalette(hDC, hOldPal, TRUE);
    }
    
    return bSuccess;
}

WORD DIBNumColors(LPSTR lpbi)
{
    WORD wBitCount;  // DIB bit count
    
    // If this is a Windows-style DIB, the number of colors in the
    // color table can be less than the number of bits per pixel
    // allows for (i.e. lpbi->biClrUsed can be set to some value).
    // If this is the case, return the appropriate value.
    if (IS_WIN30_DIB(lpbi)) {
        DWORD dwClrUsed;
        
        dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;

        if (dwClrUsed != 0) {
            return (WORD)dwClrUsed;
        }
    }
    
    //  Calculate the number of colors in the color table based on
    //  the number of bits per pixel for the DIB.
    if (IS_WIN30_DIB(lpbi)) {
        wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
    } else {
        wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
    }
    
    // return number of colors based on bits per pixel
    switch (wBitCount) {
    case 1:
        return 2;
        
    case 4:
        return 16;
        
    case 8:
        return 256;
        
    default:
        return 0;
    }
}