//==================================================================================================
//  File:       unicode_utils.cpp
//
//  Copyright (c) 2018 Bob Steagall and KEWB Computing, All Rights Reserved
//==================================================================================================
//
#include "utf_utils.h"
#include <cstdio>
#include <experimental/simd>
namespace stdx = std::experimental;

#if defined KEWB_PLATFORM_LINUX
    #include <emmintrin.h>
    #include <immintrin.h>
    #include <xmmintrin.h>
#elif defined KEWB_PLATFORM_WINDOWS
    #include <intrin.h>
#endif

namespace uu {
//- Static member data init.
//
UtfUtils::LookupTables const    UtfUtils::smTables =
{
    //- Initialize the maFirstUnitTable member array.  This array implements a lookup table that
    //  maps the first code unit of a sequence to: 1. a pre-masked value to start the computation
    //  of the resulting code point; and, 2. the next state in the DFA for this code unit.
    //
    {
        { 0x00, BGN },   //- 0x00
        { 0x01, BGN },   //- 0x01
        { 0x02, BGN },   //- 0x02
        { 0x03, BGN },   //- 0x03
        { 0x04, BGN },   //- 0x04
        { 0x05, BGN },   //- 0x05
        { 0x06, BGN },   //- 0x06
        { 0x07, BGN },   //- 0x07
        { 0x08, BGN },   //- 0x08
        { 0x09, BGN },   //- 0x09
        { 0x0A, BGN },   //- 0x0A
        { 0x0B, BGN },   //- 0x0B
        { 0x0C, BGN },   //- 0x0C
        { 0x0D, BGN },   //- 0x0D
        { 0x0E, BGN },   //- 0x0E
        { 0x0F, BGN },   //- 0x0F

        { 0x10, BGN },   //- 0x10
        { 0x11, BGN },   //- 0x11
        { 0x12, BGN },   //- 0x12
        { 0x13, BGN },   //- 0x13
        { 0x14, BGN },   //- 0x14
        { 0x15, BGN },   //- 0x15
        { 0x16, BGN },   //- 0x16
        { 0x17, BGN },   //- 0x17
        { 0x18, BGN },   //- 0x18
        { 0x19, BGN },   //- 0x19
        { 0x1A, BGN },   //- 0x1A
        { 0x1B, BGN },   //- 0x1B
        { 0x1C, BGN },   //- 0x1C
        { 0x1D, BGN },   //- 0x1D
        { 0x1E, BGN },   //- 0x1E
        { 0x1F, BGN },   //- 0x1F

        { 0x20, BGN },   //- 0x20
        { 0x21, BGN },   //- 0x21
        { 0x22, BGN },   //- 0x22
        { 0x23, BGN },   //- 0x23
        { 0x24, BGN },   //- 0x24
        { 0x25, BGN },   //- 0x25
        { 0x26, BGN },   //- 0x26
        { 0x27, BGN },   //- 0x27
        { 0x28, BGN },   //- 0x28
        { 0x29, BGN },   //- 0x29
        { 0x2A, BGN },   //- 0x2A
        { 0x2B, BGN },   //- 0x2B
        { 0x2C, BGN },   //- 0x2C
        { 0x2D, BGN },   //- 0x2D
        { 0x2E, BGN },   //- 0x2E
        { 0x2F, BGN },   //- 0x2F

        { 0x30, BGN },   //- 0x30
        { 0x31, BGN },   //- 0x31
        { 0x32, BGN },   //- 0x32
        { 0x33, BGN },   //- 0x33
        { 0x34, BGN },   //- 0x34
        { 0x35, BGN },   //- 0x35
        { 0x36, BGN },   //- 0x36
        { 0x37, BGN },   //- 0x37
        { 0x38, BGN },   //- 0x38
        { 0x39, BGN },   //- 0x39
        { 0x3A, BGN },   //- 0x3A
        { 0x3B, BGN },   //- 0x3B
        { 0x3C, BGN },   //- 0x3C
        { 0x3D, BGN },   //- 0x3D
        { 0x3E, BGN },   //- 0x3E
        { 0x3F, BGN },   //- 0x3F

        { 0x40, BGN },   //- 0x40
        { 0x41, BGN },   //- 0x41
        { 0x42, BGN },   //- 0x42
        { 0x43, BGN },   //- 0x43
        { 0x44, BGN },   //- 0x44
        { 0x45, BGN },   //- 0x45
        { 0x46, BGN },   //- 0x46
        { 0x47, BGN },   //- 0x47
        { 0x48, BGN },   //- 0x48
        { 0x49, BGN },   //- 0x49
        { 0x4A, BGN },   //- 0x4A
        { 0x4B, BGN },   //- 0x4B
        { 0x4C, BGN },   //- 0x4C
        { 0x4D, BGN },   //- 0x4D
        { 0x4E, BGN },   //- 0x4E
        { 0x4F, BGN },   //- 0x4F

        { 0x50, BGN },   //- 0x50
        { 0x51, BGN },   //- 0x51
        { 0x52, BGN },   //- 0x52
        { 0x53, BGN },   //- 0x53
        { 0x54, BGN },   //- 0x54
        { 0x55, BGN },   //- 0x55
        { 0x56, BGN },   //- 0x56
        { 0x57, BGN },   //- 0x57
        { 0x58, BGN },   //- 0x58
        { 0x59, BGN },   //- 0x59
        { 0x5A, BGN },   //- 0x5A
        { 0x5B, BGN },   //- 0x5B
        { 0x5C, BGN },   //- 0x5C
        { 0x5D, BGN },   //- 0x5D
        { 0x5E, BGN },   //- 0x5E
        { 0x5F, BGN },   //- 0x5F

        { 0x60, BGN },   //- 0x60
        { 0x61, BGN },   //- 0x61
        { 0x62, BGN },   //- 0x62
        { 0x63, BGN },   //- 0x63
        { 0x64, BGN },   //- 0x64
        { 0x65, BGN },   //- 0x65
        { 0x66, BGN },   //- 0x66
        { 0x67, BGN },   //- 0x67
        { 0x68, BGN },   //- 0x68
        { 0x69, BGN },   //- 0x69
        { 0x6A, BGN },   //- 0x6A
        { 0x6B, BGN },   //- 0x6B
        { 0x6C, BGN },   //- 0x6C
        { 0x6D, BGN },   //- 0x6D
        { 0x6E, BGN },   //- 0x6E
        { 0x6F, BGN },   //- 0x6F

        { 0x70, BGN },   //- 0x70
        { 0x71, BGN },   //- 0x71
        { 0x72, BGN },   //- 0x72
        { 0x73, BGN },   //- 0x73
        { 0x74, BGN },   //- 0x74
        { 0x75, BGN },   //- 0x75
        { 0x76, BGN },   //- 0x76
        { 0x77, BGN },   //- 0x77
        { 0x78, BGN },   //- 0x78
        { 0x79, BGN },   //- 0x79
        { 0x7A, BGN },   //- 0x7A
        { 0x7B, BGN },   //- 0x7B
        { 0x7C, BGN },   //- 0x7C
        { 0x7D, BGN },   //- 0x7D
        { 0x7E, BGN },   //- 0x7E
        { 0x7F, BGN },   //- 0x7F

        { 0x00, ERR },   //- 0x80
        { 0x01, ERR },   //- 0x81
        { 0x02, ERR },   //- 0x82
        { 0x03, ERR },   //- 0x83
        { 0x04, ERR },   //- 0x84
        { 0x05, ERR },   //- 0x85
        { 0x06, ERR },   //- 0x86
        { 0x07, ERR },   //- 0x87
        { 0x08, ERR },   //- 0x88
        { 0x09, ERR },   //- 0x89
        { 0x0A, ERR },   //- 0x8A
        { 0x0B, ERR },   //- 0x8B
        { 0x0C, ERR },   //- 0x8C
        { 0x0D, ERR },   //- 0x8D
        { 0x0E, ERR },   //- 0x8E
        { 0x0F, ERR },   //- 0x8F

        { 0x10, ERR },   //- 0x90
        { 0x11, ERR },   //- 0x91
        { 0x12, ERR },   //- 0x92
        { 0x13, ERR },   //- 0x93
        { 0x14, ERR },   //- 0x94
        { 0x15, ERR },   //- 0x95
        { 0x16, ERR },   //- 0x96
        { 0x17, ERR },   //- 0x97
        { 0x18, ERR },   //- 0x98
        { 0x19, ERR },   //- 0x99
        { 0x1A, ERR },   //- 0x9A
        { 0x1B, ERR },   //- 0x9B
        { 0x1C, ERR },   //- 0x9C
        { 0x1D, ERR },   //- 0x9D
        { 0x1E, ERR },   //- 0x9E
        { 0x1F, ERR },   //- 0x9F

        { 0x20, ERR },   //- 0xA0
        { 0x21, ERR },   //- 0xA1
        { 0x22, ERR },   //- 0xA2
        { 0x23, ERR },   //- 0xA3
        { 0x24, ERR },   //- 0xA4
        { 0x25, ERR },   //- 0xA5
        { 0x26, ERR },   //- 0xA6
        { 0x27, ERR },   //- 0xA7
        { 0x28, ERR },   //- 0xA8
        { 0x29, ERR },   //- 0xA9
        { 0x2A, ERR },   //- 0xAA
        { 0x2B, ERR },   //- 0xAB
        { 0x2C, ERR },   //- 0xAC
        { 0x2D, ERR },   //- 0xAD
        { 0x2E, ERR },   //- 0xAE
        { 0x2F, ERR },   //- 0xAF

        { 0x30, ERR },   //- 0xB0
        { 0x31, ERR },   //- 0xB1
        { 0x32, ERR },   //- 0xB2
        { 0x33, ERR },   //- 0xB3
        { 0x34, ERR },   //- 0xB4
        { 0x35, ERR },   //- 0xB5
        { 0x36, ERR },   //- 0xB6
        { 0x37, ERR },   //- 0xB7
        { 0x38, ERR },   //- 0xB8
        { 0x39, ERR },   //- 0xB9
        { 0x3A, ERR },   //- 0xBA
        { 0x3B, ERR },   //- 0xBB
        { 0x3C, ERR },   //- 0xBC
        { 0x3D, ERR },   //- 0xBD
        { 0x3E, ERR },   //- 0xBE
        { 0x3F, ERR },   //- 0xBF

        { 0xC0, ERR },   //- 0xC0
        { 0xC1, ERR },   //- 0xC1
        { 0x02, CS1 },   //- 0xC2
        { 0x03, CS1 },   //- 0xC3
        { 0x04, CS1 },   //- 0xC4
        { 0x05, CS1 },   //- 0xC5
        { 0x06, CS1 },   //- 0xC6
        { 0x07, CS1 },   //- 0xC7
        { 0x08, CS1 },   //- 0xC8
        { 0x09, CS1 },   //- 0xC9
        { 0x0A, CS1 },   //- 0xCA
        { 0x0B, CS1 },   //- 0xCB
        { 0x0C, CS1 },   //- 0xCC
        { 0x0D, CS1 },   //- 0xCD
        { 0x0E, CS1 },   //- 0xCE
        { 0x0F, CS1 },   //- 0xCF

        { 0x10, CS1 },   //- 0xD0
        { 0x11, CS1 },   //- 0xD1
        { 0x12, CS1 },   //- 0xD2
        { 0x13, CS1 },   //- 0xD3
        { 0x14, CS1 },   //- 0xD4
        { 0x15, CS1 },   //- 0xD5
        { 0x16, CS1 },   //- 0xD6
        { 0x17, CS1 },   //- 0xD7
        { 0x18, CS1 },   //- 0xD8
        { 0x19, CS1 },   //- 0xD9
        { 0x1A, CS1 },   //- 0xDA
        { 0x1B, CS1 },   //- 0xDB
        { 0x1C, CS1 },   //- 0xDC
        { 0x1D, CS1 },   //- 0xDD
        { 0x1E, CS1 },   //- 0xDE
        { 0x1F, CS1 },   //- 0xDF

        { 0x00, P3A },   //- 0xE0
        { 0x01, CS2 },   //- 0xE1
        { 0x02, CS2 },   //- 0xE2
        { 0x03, CS2 },   //- 0xE3
        { 0x04, CS2 },   //- 0xE4
        { 0x05, CS2 },   //- 0xE5
        { 0x06, CS2 },   //- 0xE6
        { 0x07, CS2 },   //- 0xE7
        { 0x08, CS2 },   //- 0xE8
        { 0x09, CS2 },   //- 0xE9
        { 0x0A, CS2 },   //- 0xEA
        { 0x0B, CS2 },   //- 0xEB
        { 0x0C, CS2 },   //- 0xEC
        { 0x0D, P3B },   //- 0xED
        { 0x0E, CS2 },   //- 0xEE
        { 0x0F, CS2 },   //- 0xEF

        { 0x00, P4A },   //- 0xF0
        { 0x01, CS3 },   //- 0xF1
        { 0x02, CS3 },   //- 0xF2
        { 0x03, CS3 },   //- 0xF3
        { 0x04, P4B },   //- 0xF4
        { 0xF5, ERR },   //- 0xF5
        { 0xF6, ERR },   //- 0xF6
        { 0xF7, ERR },   //- 0xF7
        { 0xF8, ERR },   //- 0xF8
        { 0xF9, ERR },   //- 0xF9
        { 0xFA, ERR },   //- 0xFA
        { 0xFB, ERR },   //- 0xFB
        { 0xFC, ERR },   //- 0xFC
        { 0xFD, ERR },   //- 0xFD
        { 0xFE, ERR },   //- 0xFE
        { 0xFF, ERR },   //- 0xFF
    },

    //- Initialize the maOctetCategory member array.  This array implements a lookup table
    //  that maps an input octet to a corresponding octet category.
    //
    //   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    //============================================================================================
    {
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 00..0F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 10..1F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 20..2F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 30..3F
                                                                                        //
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 40..4F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 50..5F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 60..6F
        ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, ASC, //- 70..7F
                                                                                        //
        CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, CR1, //- 80..8F
        CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, CR2, //- 90..9F
        CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, //- A0..AF
        CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, CR3, //- B0..BF
                                                                                        //
        ILL, ILL, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, //- C0..CF
        L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, L2A, //- D0..DF
        L3A, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3B, L3C, L3B, L3B, //- E0..EF
        L4A, L4B, L4B, L4B, L4C, ILL, ILL, ILL, ILL, ILL, ILL, ILL, ILL, ILL, ILL, ILL, //- F0..FF
    },

    //- Initialize the maTransitions member array.  This array implements a lookup table that,
    //  given the current DFA state and an input code unit, indicates the next DFA state. 
	//
    //  ILL  ASC  CR1  CR2  CR3  L2A  L3A  L3B  L3C  L4A  L4B  L4C  CLASS/STATE
    //=========================================================================
    {
        err, END, err, err, err, CS1, P3A, CS2, P3B, P4A, CS3, P4B,	  //- BGN|END
        err, err, err, err, err, err, err, err, err, err, err, err,   //- ERR
																	  //
        err, err, END, END, END, err, err, err, err, err, err, err,   //- CS1
        err, err, CS1, CS1, CS1, err, err, err, err, err, err, err,   //- CS2
        err, err, CS2, CS2, CS2, err, err, err, err, err, err, err,   //- CS3
																	  //
        err, err, err, err, CS1, err, err, err, err, err, err, err,   //- P3A
        err, err, CS1, CS1, err, err, err, err, err, err, err, err,   //- P3B
																	  //
        err, err, err, CS2, CS2, err, err, err, err, err, err, err,   //- P4A
        err, err, CS2, err, err, err, err, err, err, err, err, err,   //- P4B
    },

    //- Initialize the maFirstOctetMask member array.  This array implements a lookup table that
    //  maps a character class to a mask that is applied to the first code unit in a sequence.
    //
    {
        0xFF,   //- ILL - C0..C1, F5..FF    Illegal code unit
                //
        0x7F,   //- ASC - 00..7F            ASCII byte range
                //
        0x3F,   //- CR1 - 80..8F            Continuation range 1
        0x3F,   //- CR2 - 90..9F            Continuation range 2
        0x3F,   //- CR3 - A0..BF            Continuation range 3
                //
        0x1F,   //- L2A - C2..DF            Leading byte range 2A / 2-byte sequence
                //
        0x0F,   //- L3A - E0                Leading byte range 3A / 3-byte sequence
        0x0F,   //- L3B - E1..EC, EE..EF    Leading byte range 3B / 3-byte sequence
        0x0F,   //- L3C - ED                Leading byte range 3C / 3-byte sequence
                //
        0x07,   //- L4A - F0                Leading byte range 4A / 4-byte sequence
        0x07,   //- L4B - F1..F3            Leading byte range 4B / 4-byte sequence
        0x07,   //- L4C - F4                Leading byte range 4C / 4-byte sequence
    },
};

//- These are the human-readable names assigned to the code unit categories.
//
char const*     UtfUtils::smClassNames[12] =
{
    "ILL", "ASC", "CR1", "CR2", "CR3", "L2A", "L3A", "L3B", "L3C", "L4A", "L4B", "L4C",
};

//- These are the human-readable names assigned to the various states comprising the DFA.
//
char const*     UtfUtils::smStateNames[9] =
{
    "BGN", "ERR", "CS1", "CS2", "CS3", "P3A", "P3B", "P4A", "P4B",
};

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It performs conversion by traversing
///     the DFA without any optimizations using the `AdvanceWithBigTable` member function to
///     read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::BasicBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
        {
            *pDst++ = cdpt;
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by checking for ASCII code units and
///     converting them directly to code points.  It uses the `AdvanceWithBigTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::FastBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using SSE intrinsics.  It uses the `AdvanceWithBigTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SseBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < (pSrcEnd - sizeof(__m128i)))
    {
        if (*pSrc < 0x80)
        {
            ConvertAsciiWithSse(pSrc, pDst);
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

using char8v = stdx::native_simd<UtfUtils::char8_t>;
template <typename T, int N>
using deduced_simd = stdx::simd<T, stdx::simd_abi::deduce_t<T, N>>;

template <typename T, typename U>
std::enable_if_t<
    sizeof(T) == sizeof(U) && std::is_trivially_copyable_v<U> && std::is_trivial_v<T>, T>
bit_cast(const U& x)
{
  T r;
  std::memcpy(static_cast<void*>(&r), static_cast<const void*>(&x), sizeof(T));
  return r;
}

static inline int count_leading_zeros(std::uint32_t x) { return __builtin_clz(x); }
template <int N> static inline int expect(int x) { return __builtin_expect(x, N); }

struct invalid_utf8 {};

template <int VecNBytes, bool ParallelShifts, typename T>
std::ptrdiff_t UtfUtils::VirConvert(char8_t const* pSrc, char8_t const* const pSrcEnd,
                                    T* pDst)
{
  constexpr bool vec1byte = 1 <= VecNBytes; // vectorize 1-Byte decoding
  constexpr bool vec2byte = 2 <= VecNBytes; // vectorize 2-Byte decoding
  constexpr bool vec3byte = 3 <= VecNBytes; // vectorize 3-Byte decoding
  constexpr bool vec4byte = 4 <= VecNBytes; // vectorize 4-Byte decoding
  T* pDstOrig = pDst;

  auto&& scal1 = [&]() { *pDst++ = *pSrc++; };

  auto&& scal2 = [&]() {
    if (expect<false>((pSrc[1] & 0b1100'0000) != 0b1000'0000)) {
      throw invalid_utf8();
    }
    const auto cdpt = ((pSrc[0] & 0x1f) << 6) | (pSrc[1] & 0x3f);
    *pDst++ = cdpt;
    pSrc += 2;
    if (expect<false>(cdpt < 0x80)) {
      throw invalid_utf8();
    }
  };

  auto&& scal3 = [&]() {
    if (expect<false>((pSrc[1] & 0b1100'0000) != 0b1000'0000 ||
                      (pSrc[2] & 0b1100'0000) != 0b1000'0000)) {
      throw invalid_utf8();
    }
    const auto cdpt =
        ((pSrc[0] & 0x0f) << 12) | ((pSrc[1] & 0x3f) << 6) | (pSrc[2] & 0x3f);
    *pDst++ = cdpt;
    pSrc += 3;
    if (expect<false>(cdpt < 0x800)) {
      throw invalid_utf8();
    }
  };

  auto&& scal4 = [&]() {
    if (expect<false>((pSrc[1] & 0b1100'0000) != 0b1000'0000 ||
                      (pSrc[2] & 0b1100'0000) != 0b1000'0000 ||
                      (pSrc[3] & 0b1100'0000) != 0b1000'0000)) {
      throw invalid_utf8();
    }
    if constexpr (std::is_same_v<T, char32_t>) {
      const auto cdpt = ((pSrc[0] & 0x07) << 18) | ((pSrc[1] & 0x3f) << 12) |
                        ((pSrc[2] & 0x3f) << 6) | (pSrc[3] & 0x3f);
      *pDst++ = cdpt;
      if (expect<false>(cdpt < 0x10000)) {
        throw invalid_utf8();
      }
    } else {
      static_assert(std::is_same_v<T, char16_t>);
      const char16_t lo = ((pSrc[2] & 0x0f) << 6) | (pSrc[3] & 0x3f);
      const char16_t hi =
          ((pSrc[0] & 0x07) << 8) | ((pSrc[1] & 0x3f) << 2) | ((pSrc[2] & 0x3f) >> 4);
      pDst[0] = hi + 0xd7c0;
      pDst[1] = lo + 0xdc00;
      pDst += 2;
      if (expect<false>(hi < 0x40)) {
        throw invalid_utf8();
      }
    }
    pSrc += 4;
  };

  while (pSrcEnd - char8v::size() >
         pSrc) {  // while there are enough input bytes for vectorization
    const char8v chunk(pSrc, stdx::element_aligned);

    auto&& vec1 = [&]() {
      do {
        const char8v chunk(pSrc, stdx::element_aligned);
        chunk.copy_to(pDst, stdx::element_aligned);
        if (none_of(chunk > 0x7f)) {
          pSrc += chunk.size();
          pDst += chunk.size();
        } else {
          const int n_valid = find_first_set(chunk > 0x7f);
          pSrc += n_valid;
          pDst += n_valid;
          break;
        }
      } while (pSrcEnd - char8v::size() > pSrc);
    };

    auto&& vec2 = [&]() {
      const auto invalid_2byte_seq =
          chunk < char8v([](auto i) { return i % 2 == 0 ? 0b1100'0010 : 0b1000'0000; }) ||
          chunk > char8v([](auto i) { return i % 2 == 0 ? 0b1101'1111 : 0b1011'1111; });
      const int n_valid = none_of(invalid_2byte_seq)
                              ? char8v::size() / 2
                              : find_first_set(invalid_2byte_seq) / 2;
      pSrc += n_valid * 2;
      if (expect<false>(n_valid == 0)) {
        throw invalid_utf8();
      }
      if constexpr (ParallelShifts) {
        using char16v = deduced_simd<std::uint16_t, char8v::size() / 2>;
        auto chunk16 = bit_cast<char16v>(chunk);
        chunk16 = ((chunk16 & 0x3f00) >> 8) | ((chunk16 & 0x001f) << 6);
        for (int i = 0; i < n_valid; ++i) {
          *pDst++ = chunk16[i];
        }
      } else {
        const auto lo6bits =
            chunk & 0x3f;  // in principle {0x1f, 0x3f, 0x1f, 0x3f, ...}, but we already
                           // checked that bit 0x20 is 0 for even indexes
        for (int i = 0; i < n_valid; ++i) {
          *pDst++ = (lo6bits[2 * i] << 6) | lo6bits[2 * i + 1];
        }
      }
    };

    auto&& vec3 = [&]() {
      constexpr char8v mask(
          [](auto i) { return i % 3 == 0 ? 0b1111'0000 : 0b1100'0000; });
      constexpr char8v expected_bits(
          [](auto i) { return i % 3 == 0 ? 0b1110'0000 : 0b1000'0000; });
      const auto invalid_3byte_seq = (chunk & mask) != expected_bits;
      const int n_valid = none_of(invalid_3byte_seq)
                              ? char8v::size() / 3
                              : find_first_set(invalid_3byte_seq) / 3;
      if (expect<false>(n_valid == 0)) {
        throw invalid_utf8();
      }
      pSrc += 3 * n_valid;
      constexpr char8v code_mask(
          [](auto i) { return i % 3 == 0 ? 0b0000'1111 : 0b0011'1111; });
      const auto chunk3f = chunk & code_mask;
      for (int i = 0; i < n_valid; ++i) {
        const auto cdpt =
            (chunk3f[i * 3] << 12) | (chunk3f[i * 3 + 1] << 6) | chunk3f[i * 3 + 2];
        *pDst++ = cdpt;
        if (expect<false>(cdpt < 0x800)) {
          throw invalid_utf8();
        }
      }
    };

    auto&& vec4 = [&]() {
      constexpr char8v mask(
          [](auto i) { return i % 4 == 0 ? 0b1111'1000 : 0b1100'0000; });
      constexpr char8v expected_bits(
          [](auto i) { return i % 4 == 0 ? 0b1111'0000 : 0b1000'0000; });
      const auto invalid_4byte_seq = (chunk & mask) != expected_bits;
      const int n_valid = none_of(invalid_4byte_seq)
                              ? char8v::size() / 4
                              : find_first_set(invalid_4byte_seq) / 4;
      if (expect<false>(n_valid == 0)) {
        throw invalid_utf8();
      }
      constexpr char8v code_mask(
          [](auto i) { return i % 4 == 0 ? 0b0000'0111 : 0b0011'1111; });
      const char8v codebits = chunk & code_mask;
      pSrc += n_valid * 4;
      if (sizeof(T) == 4) {
        for (int i = 0; i < n_valid; ++i) {
          const auto cdpt = (codebits[4 * i + 0] << 18) | (codebits[4 * i + 1] << 12) |
                            (codebits[4 * i + 2] << 6) | codebits[4 * i + 3];
          *pDst++ = cdpt;
          if (expect<false>(cdpt < 0x10000)) {
            throw invalid_utf8();
          }
        }
      } else {
        for (int i = 0; i < n_valid; ++i) {
          /*
        const auto cdpt = (codebits[4 * i + 0] << 18) | (codebits[4 * i + 1] << 12) |
                          (codebits[4 * i + 2] << 6) | codebits[4 * i + 3];
        *pDst++ = 0xd7c0 + (cdpt >> 10);
        *pDst++ = 0xdc00 + (cdpt & 0x3ff);
        if (expect<false>(cdpt < 0x10000)) {
          throw invalid_utf8();
        }
        */
          pDst[0] = ((codebits[4 * i + 0] << 8) | (codebits[4 * i + 1] << 2) |
                     (codebits[4 * i + 2] >> 4)) +
                    0xd7c0;
          const auto hi = ((codebits[4 * i + 2] & 0x0f) << 6) | codebits[4 * i + 3];
          pDst[1] = hi + 0xdc00;
          pDst += 2;
          if (expect<false>(hi < 0x40)) {
            throw invalid_utf8();
          }
        }
      }
    };

    switch (expect<0>(count_leading_zeros(~(pSrc[0] << 24)))) {
    case 0:
      if (!vec1byte || expect<false>(pSrc[1] > 0x7f)) {
        scal1();
      } else {
        vec1();
      }
      break;
    case 1:
      throw invalid_utf8();
    case 2:
      if (vec2byte && expect<true>((~pSrc[2] & 0b1100'0000) == 0)) {
        vec2();
      } else {
        scal2();
      }
      break;
    case 3:
      if (vec3byte && expect<true>((~pSrc[3] & 0b1110'0000) == 0)) {
        vec3();
      } else {
        scal3();
      }
      break;
    case 4:
      if (vec4byte && expect<false>((~pSrc[4] & 0b1111'0000) == 0)) {
        vec4();
      } else {
        scal4();
      }
      break;
    default:
      throw invalid_utf8();
    }
  }
  while (pSrcEnd > pSrc) {
    switch (expect<0>(count_leading_zeros(~(pSrc[0] << 24)))) {
    case 0:
      scal1();
      break;
    case 1:
      throw invalid_utf8();
    case 2:
      scal2();
      break;
    case 3:
      scal3();
      break;
    case 4:
      scal4();
      break;
    default:
      throw invalid_utf8();
    }
  }
  return pDst - pDstOrig;
}

template std::ptrdiff_t UtfUtils::VirConvert<0, false, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<1, false, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<2, false, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<2,  true, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<3, false, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<3,  true, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<4, false, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<4,  true, char16_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<0, false, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<1, false, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<2, false, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<2,  true, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<3, false, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<3,  true, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<4, false, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::VirConvert<4,  true, char32_t>(char8_t const* pSrc,
                                                                 char8_t const* const pSrcEnd,
                                                                 char32_t* pDst) noexcept;


template <class T>
KEWB_FORCE_INLINE bool ConvertNonAsciiWithSimd(UtfUtils::char8_t const*& pSrc,
                                               T*& pDst) noexcept;

template <UtfUtils::SimdImplVariant ImplVariant, UtfUtils::TableFun TableFun, typename T>
std::ptrdiff_t UtfUtils::SimdConvert(char8_t const* pSrc, char8_t const* const pSrcEnd,
                                     T* pDst) noexcept
{
    T*        pDstOrig = pDst;
    char32_t  cdpt;

    auto&& advanceWithTable =
        TableFun == BigTable ? AdvanceWithBigTable : AdvanceWithSmallTable;

    while (pSrcEnd - char8v::size() > pSrc)
    {
        if constexpr (ImplVariant & UseNonAsciiKnowledge) {
          const NextChar next =
              pSrc[0] < 0x80 ? ConvertAsciiWithSimd(pSrc, pDst) : NextChar::NotAscii;
          if (next == NextChar::NotAscii) {
            do {
              if (ImplVariant & VecAll && ConvertNonAsciiWithSimd(pSrc, pDst)) ;
              else if (advanceWithTable(pSrc, pSrcEnd, cdpt) != ERR) {
                if constexpr (std::is_same_v<T, char16_t>) {
                  GetCodeUnits(cdpt, pDst);
                } else {
                  *pDst++ = cdpt;
                }
              } else {
                return -1;
              }
            } while (pSrc < pSrcEnd && pSrc[0] > 0x7f);
            }
        }
        else if (*pSrc < 0x80)
        {
            ConvertAsciiWithSimd(pSrc, pDst);
        }
        else if (ImplVariant & VecAll && ConvertNonAsciiWithSimd(pSrc, pDst)) ;
        else
        {
            if (advanceWithTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                if constexpr(std::is_same_v<T, char16_t>)
                {
                    GetCodeUnits(cdpt, pDst);
                }
                else
                {
                    *pDst++ = cdpt;
                }
            }
            else
            {
                return -1;
            }
        }
    }

    while (pSrc < pSrcEnd)
    {
        if (pSrc[0] < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else if (advanceWithTable(pSrc, pSrcEnd, cdpt) != ERR)
        {
            if constexpr(std::is_same_v<T, char16_t>)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                *pDst++ = cdpt;
            }
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewb, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewb, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewb, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewb, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;

template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledge, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledge, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledge, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledge, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;

template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewbButVecAll, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewbButVecAll, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewbButVecAll, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::LikeKewbButVecAll, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;

template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledgeVecAll, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledgeVecAll, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char32_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledgeVecAll, UtfUtils::BigTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;
template std::ptrdiff_t UtfUtils::SimdConvert<UtfUtils::UseNonAsciiKnowledgeVecAll, UtfUtils::SmallTable>(
    char8_t const* pSrc, char8_t const* const pSrcEnd, char16_t* pDst) noexcept;

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16/32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16/32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using std::simd.  It uses the `AdvanceWithBigTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-16/32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
template <typename T>
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SimdBigTableConvert(char8_t const* pSrc, char8_t const* const pSrcEnd, T* pDst) noexcept
{
    return SimdConvert<LikeKewb, BigTable>(pSrc, pSrcEnd, pDst);
}
template std::ptrdiff_t UtfUtils::SimdBigTableConvert(char8_t const*,
                                                      char8_t const* const,
                                                      char16_t*) noexcept;
template std::ptrdiff_t UtfUtils::SimdBigTableConvert(char8_t const*,
                                                      char8_t const* const,
                                                      char32_t*) noexcept;

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code units.  It performs conversion by traversing
///     the DFA without any optimizations using the `AdvanceWithBigTable` member function to
///     read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::BasicBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
        {
            GetCodeUnits(cdpt, pDst);
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code unis.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by checking for ASCII code units and
///     converting them directly to code points.  It uses the `AdvanceWithBigTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::FastBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code units.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using SSE intrinsics.  It uses the `AdvanceWithBigTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SseBigTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < (pSrcEnd - sizeof(__m128i)))
    {
        if (*pSrc < 0x80)
        {
            ConvertAsciiWithSse(pSrc, pDst);
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithBigTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It performs conversion by traversing
///     the DFA without any optimizations using the `AdvanceWithSmallTable` member function to
///     read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::BasicSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
        {
            *pDst++ = cdpt;
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by checking for ASCII code units and
///     converting them directly to code points.  It uses the `AdvanceWithSmallTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::FastSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using SSE intrinsics.  It uses the `AdvanceWithSmallTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SseSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < (pSrcEnd - sizeof(__m128i)))
    {
        if (*pSrc < 0x80)
        {
            ConvertAsciiWithSse(pSrc, pDst);
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                *pDst++ = cdpt;
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16/32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16/32 code points.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using std::simd.  It uses the `AdvanceWithSmallTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-16/32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
template <typename T>
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SimdSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, T* pDst) noexcept
{
    return SimdConvert<LikeKewb, SmallTable>(pSrc, pSrcEnd, pDst);
}
template std::ptrdiff_t UtfUtils::SimdSmallTableConvert(char8_t const*,
                                                        char8_t const* const,
                                                        char16_t*) noexcept;
template std::ptrdiff_t UtfUtils::SimdSmallTableConvert(char8_t const*,
                                                        char8_t const* const,
                                                        char32_t*) noexcept;

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code units.  It performs conversion by traversing
///     the DFA without any optimizations using the `AdvanceWithSmallTable` member function to
///     read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::BasicSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
        {
            GetCodeUnits(cdpt, pDst);
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code unis.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by checking for ASCII code units and
///     converting them directly to code points.  It uses the `AdvanceWithSmallTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::FastSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code units.  It uses the DFA to perform non-ascii
///     code-unit sequence conversions, but optimizes by converting contiguous sequences of
///     ASCII code units using SSE intrinsics.  It uses the `AdvanceWithSmallTable` member
///     function to read and convert input.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
KEWB_ALIGN_FN std::ptrdiff_t
UtfUtils::SseSmallTableConvert(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < (pSrcEnd - sizeof(__m128i)))
    {
        if (*pSrc < 0x80)
        {
            ConvertAsciiWithSse(pSrc, pDst);
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    while (pSrc < pSrcEnd)
    {
        if (*pSrc < 0x80)
        {
            *pDst++ = *pSrc++;
        }
        else
        {
            if (AdvanceWithSmallTable(pSrc, pSrcEnd, cdpt) != ERR)
            {
                GetCodeUnits(cdpt, pDst);
            }
            else
            {
                return -1;
            }
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Trace converts a sequence of UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-32 code points.  It uses only the DFA to perform
///     conversion.  It prints current and next state transition information as it proceeds.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code point output range.
///
/// \returns
///     If successful, the number of UTF-32 code points written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
std::ptrdiff_t
UtfUtils::ConvertWithTrace(char8_t const* pSrc, char8_t const* pSrcEnd, char32_t* pDst) noexcept
{
    char32_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithTrace(pSrc, pSrcEnd, cdpt) != ERR)
        {
            *pDst++ = cdpt;
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Trace converts a sequence of UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function reads an input sequence of UTF-8 code units and converts
///     it to an output sequence of UTF-16 code units.  It uses only the DFA to perform
///     conversion.  It prints current and next state transition information as it proceeds.
///
/// \param pSrc
///     A non-null pointer defining the beginning of the code unit input range.
/// \param pSrcEnd
///     A non-null past-the-end pointer defining the end of the code unit input range.
/// \param pDst
///     A non-null pointer defining the beginning of the code unit output range.
///
/// \returns
///     If successful, the number of UTF-16 code units written; otherwise -1 is returned to
///     indicate an error was encountered.
//--------------------------------------------------------------------------------------------------
//
std::ptrdiff_t
UtfUtils::ConvertWithTrace(char8_t const* pSrc, char8_t const* pSrcEnd, char16_t* pDst) noexcept
{
    char16_t*   pDstOrig = pDst;
    char32_t    cdpt;

    while (pSrc < pSrcEnd)
    {
        if (AdvanceWithTrace(pSrc, pSrcEnd, cdpt) != ERR)
        {
            GetCodeUnits(cdpt, pDst);
        }
        else
        {
            return -1;
        }
    }

    return pDst - pDstOrig;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of ASCII UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function uses SSE intrinsics to convert a register of ASCII code
///     units to four registers of equivalent UTF-32 code units.
///
/// \param pSrc
///     A reference to a non-null pointer defining the start of the code unit input range.
/// \param pDst
///     A reference to a non-null pointer defining the start of the code point output range.
//--------------------------------------------------------------------------------------------------
//
KEWB_FORCE_INLINE void
UtfUtils::ConvertAsciiWithSse(char8_t const*& pSrc, char32_t*& pDst) noexcept
{
    __m128i     chunk, half, qrtr, zero;
    int32_t     mask, incr;

    zero  = _mm_set1_epi8(0);                           //- Zero out the interleave register
    chunk = _mm_loadu_si128((__m128i const*) pSrc);     //- Load a register with 8-bit bytes
    mask  = _mm_movemask_epi8(chunk);                   //- Determine which octets have high bit set

    half = _mm_unpacklo_epi8(chunk, zero);              //- Unpack bytes 0-7 into 16-bit words
    qrtr = _mm_unpacklo_epi16(half, zero);              //- Unpack words 0-3 into 32-bit dwords
    _mm_storeu_si128((__m128i*) pDst, qrtr);            //- Write to memory
    qrtr = _mm_unpackhi_epi16(half, zero);              //- Unpack words 4-7 into 32-bit dwords
    _mm_storeu_si128((__m128i*) (pDst + 4), qrtr);      //- Write to memory

    half = _mm_unpackhi_epi8(chunk, zero);              //- Unpack bytes 8-15 into 16-bit words
    qrtr = _mm_unpacklo_epi16(half, zero);              //- Unpack words 8-11 into 32-bit dwords
    _mm_storeu_si128((__m128i*) (pDst + 8), qrtr);      //- Write to memory
    qrtr = _mm_unpackhi_epi16(half, zero);              //- Unpack words 12-15 into 32-bit dwords
    _mm_storeu_si128((__m128i*) (pDst + 12), qrtr);     //- Write to memory

    //- If no bits were set in the mask, then all 16 code units were ASCII, and therefore
    //  both pointers are advanced by 16.
    //
    if (mask == 0)
    {
        pSrc += 16;
        pDst += 16;
    }

    //- Otherwise, the number of trailing (low-order) zero bits in the mask indicates the number
    //  of ASCII code units starting from the lowest byte address.
    else
    {
        incr  = GetTrailingZeros(mask);
        pSrc += incr;
        pDst += incr;
    }
}

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of ASCII UTF-8 code units to a sequence of UTF-16 code units.
///
/// \details
///     This static member function uses SSE intrinsics to convert a register of ASCII code
///     units to two registers of equivalent UTF-16 code units.
///
/// \param pSrc
///     A reference to a non-null pointer defining the start of the code unit input range.
/// \param pDst
///     A reference to a non-null pointer defining the start of the code unit output range.
//--------------------------------------------------------------------------------------------------
//
KEWB_FORCE_INLINE void
UtfUtils::ConvertAsciiWithSse(char8_t const*& pSrc, char16_t*& pDst) noexcept
{
    __m128i     chunk, half;
    int32_t     mask, incr;

    chunk = _mm_loadu_si128((__m128i const*) pSrc);     //- Load the register with 8-bit bytes
    mask  = _mm_movemask_epi8(chunk);                   //- Determine which octets have high bit set

    half = _mm_unpacklo_epi8(chunk, _mm_set1_epi8(0));  //- Unpack lower half into 16-bit words
    _mm_storeu_si128((__m128i*) pDst, half);            //- Write to memory

    half = _mm_unpackhi_epi8(chunk, _mm_set1_epi8(0));  //- Unpack upper half into 16-bit words
    _mm_storeu_si128((__m128i*) (pDst + 8), half);      //- Write to memory

    //- If no bits were set in the mask, then all 16 code units were ASCII, and therefore
    //  both pointers are advanced by 16.
    //
    if (mask == 0)
    {
        pSrc += 16;
        pDst += 16;
    }

    //- Otherwise, the number of trailing (low-order) zero bits in the mask indicates the number
    //  of ASCII code units starting from the lowest byte address.
    else
    {
        incr  = GetTrailingZeros(mask);
        pSrc += incr;
        pDst += incr;
    }
}

//--------------------------------------------------------------------------------------------------
/// \brief  Returns the number of trailing 0-bits in an integer, starting with the least
///         significant bit.
///
/// \details
///     This static member function uses compiler intrinsics to determine the number of trailing
///     (i.e., low-order) zero bits in an int32_t parameter.  For example, an input value of 8
///     (0000 1000) would return a value of 3; an input value of 64 (0100 0000) would return a
///     value of 6.
///
/// \param x
///     An `int32_t` value whose number of trailing bits is to be determined.

/// \returns
///     the number of trailing zero bits, as an `int32_t`.
//--------------------------------------------------------------------------------------------------
//
#if defined KEWB_PLATFORM_LINUX  &&  (defined KEWB_COMPILER_CLANG  ||  defined KEWB_COMPILER_GCC)

    KEWB_FORCE_INLINE int32_t
    UtfUtils::GetTrailingZeros(int32_t x) noexcept
    {
        return  __builtin_ctz((unsigned int) x);
    }

#elif defined KEWB_PLATFORM_WINDOWS  &&  defined KEWB_COMPILER_MSVC

    KEWB_FORCE_INLINE int32_t
    UtfUtils::GetTrailingZeros(int32_t x) noexcept
    {
        unsigned long   indx;
        _BitScanForward(&indx, (unsigned long) x);
        return (int32_t) indx;
    }

#endif

//--------------------------------------------------------------------------------------------------
/// \brief  Converts a sequence of ASCII UTF-8 code units to a sequence of UTF-32 code points.
///
/// \details
///     This static member function uses SSE intrinsics to convert a register of ASCII code
///     units to four registers of equivalent UTF-32 code units.
///
/// \param pSrc
///     A reference to a non-null pointer defining the start of the code unit input range.
/// \param pDst
///     A reference to a non-null pointer defining the start of the code point output range.
//--------------------------------------------------------------------------------------------------
//
template <class T>
KEWB_FORCE_INLINE UtfUtils::NextChar
UtfUtils::ConvertAsciiWithSimd(char8_t const*& pSrc, T*& pDst) noexcept
{
  if (char8v::size() > 1 && pSrc[1] > 0x7f) {
    // quick exit if vectorization is overkill
    *pDst++ = *pSrc++;
    return NextChar::NotAscii;
  }
  const char8v chunk(pSrc, stdx::element_aligned);
  chunk.copy_to(pDst, stdx::element_aligned);
  if (none_of(chunk > 0x7f)) {
    pSrc += chunk.size();
    pDst += chunk.size();
    return NextChar::MayBeAscii;
  } else {
    const int n_valid = find_first_set(chunk > 0x7f);
    pSrc += n_valid;
    pDst += n_valid;
    return NextChar::NotAscii;
  }
}

template <class T>
KEWB_FORCE_INLINE bool ConvertNonAsciiWithSimd(UtfUtils::char8_t const*& pSrc,
                                               T*& pDst) noexcept
{
  const char8v chunk(pSrc, stdx::element_aligned);
  if (*pSrc < 0b1100'0000)
      return false;
  if (pSrc[0] < 0b1110'0000)
  {
      constexpr char8v mask([](auto i) { return i % 2 == 0 ? 0b1110'0000 : 0b1100'0000; });
      constexpr char8v expected_bits([](auto i) { return i % 2 == 0 ? 0b1100'0000 : 0b1000'0000; });
      using char16v = deduced_simd<std::uint16_t, char8v::size() / 2>;
      auto chunk16 = bit_cast<char16v>(chunk);
      chunk16 = ((chunk16 & 0x3f00) >> 8) | ((chunk16 & 0x001f) << 6);
      const auto invalid_2byte_seq = (chunk & mask) != expected_bits;
      if (__builtin_expect(none_of(invalid_2byte_seq), false)) {
          chunk16.copy_to(pDst, stdx::element_aligned);
          pSrc += chunk16.size() * 2;
          pDst += chunk16.size();
      } else {
          const int n_valid = find_first_set(invalid_2byte_seq) / 2;
          if (__builtin_expect(n_valid == 0, false))
          {
              return false;
          }
          pSrc += n_valid * 2;
          for (int i = 0; i < n_valid; ++i) {
              *pDst++ = chunk16[i];
          }
      }
  }
  else if (pSrc[0] < 0b1111'0000)
  {
      constexpr char8v mask([](auto i) { return i % 3 == 0 ? 0b1111'0000 : 0b1100'0000; });
      constexpr char8v expected_bits([](auto i) { return i % 3 == 0 ? 0b1110'0000 : 0b1000'0000; });
      const auto invalid_3byte_seq = (chunk & mask) != expected_bits;
      const int n_valid = none_of(invalid_3byte_seq)
                              ? char8v::size() / 3
                              : find_first_set(invalid_3byte_seq) / 3;
      if (__builtin_expect(n_valid == 0, false))
      {
          return false;
      }
      pSrc += 3 * n_valid;
      const auto chunk3f = chunk & 0x3f;
      for (int i = 0; i < n_valid; ++i) {
        *pDst++ = ((chunk3f[i * 3] << 12) & 0xffff) + (chunk3f[i * 3 + 1] << 6) +
                  chunk3f[i * 3 + 2];
      }
  }
  else if (pSrc[0] < 0b1111'1000)
  {
      constexpr char8v mask([](auto i) { return i % 4 == 0 ? 0b1111'1000 : 0b1100'0000; });
      constexpr char8v expected_bits([](auto i) { return i % 4 == 0 ? 0b1111'0000 : 0b1000'0000; });
      using char32v = deduced_simd<std::uint32_t, char8v::size() / 4>;
      auto chunk32 = bit_cast<char32v>(chunk);
      if (sizeof(T) == 4)
      {
          chunk32 = ((chunk32 & 0x3f000000) >> 24) + ((chunk32 & 0x003f0000) >> 10) +
                    ((chunk32 & 0x00003f00) << 4) + ((chunk32 & 0x00000007) << 18);
          chunk32.copy_to(pDst, stdx::element_aligned);
      }
      else
      {
          chunk32 = ((((chunk32 & 0x3f000000) >> 8) + ((chunk32 & 0x000f0000) << 6)) |
                     (((chunk32 & 0x00300000) >> 20) +
                      ((chunk32 & 0x00003f00) >> 6) + ((chunk32 & 0x00000007) << 8))) +
                    0xdc00'd7c0;
          using char16v = deduced_simd<std::uint16_t, char32v::size() * 2>;
          bit_cast<char16v>(chunk32).copy_to(pDst, stdx::element_aligned);
      }
      const auto invalid_4byte_seq = (chunk & mask) != expected_bits;
      if (__builtin_expect(none_of(invalid_4byte_seq), false)) {
          pSrc += chunk32.size() * 4;
          pDst += chunk32.size() * (4 / sizeof(T));
      } else {
          const int n_valid = find_first_set(invalid_4byte_seq) / 4;
          pSrc += n_valid * 4;
          pDst += n_valid * (4 / sizeof(T));
      }
  }
  else
      return false;
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief  Prints state information for tracing versions of converters.
///
/// \param curr
///     The current DFA state.
/// \param type
///     The character class of the lookahead input octet.
/// \param unit
///     The lookahead input octet.
/// \param next
///     The next DFA state, based on the current state and lookahead character class.
//--------------------------------------------------------------------------------------------------
//
void
UtfUtils::PrintStateData(State curr, CharClass type, uint32_t unit, State next)
{
    uint32_t    currState = ((uint32_t) curr) / 12;
    uint32_t    nextState = ((uint32_t) next) / 12;
    uint32_t    unitValue = unit & 0xFF;

    printf("[%s, %s (0x%02X)] ==>> %s\n", smStateNames[currState],
            smClassNames[type], unitValue, smStateNames[nextState]);
}

}   //- Namespace uu
