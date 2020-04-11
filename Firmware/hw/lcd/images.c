// File: images.c
// Auth: M. Fras, Electronics Division, MPI for Physics, Munich
// Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
// Date: 03 Apr 2020
// Rev.: 11 Apr 2020
//
// File containing images to be used with the GrImageDraw function of the TI
// TivaWare Graphics Library.
//
// This file is based on the images.c file of grlib_demo example design by TI.
//
// The data for the MPP logo have been generate from a 106 x 45 pixel bitmap of
// the MPP logo converted using the Online Image to C Array Converter:
// https://littlevgl.com/image-to-c-array
// Thanks a lot!
//



#include <stdbool.h>
#include <stdint.h>
#include "grlib/grlib.h"



const uint8_t g_pui8LogoMpp[] =
{
    IMAGE_FMT_4BPP_UNCOMP,  // 4 bits per pixel (bbp) encoding.
    106, 0,                 // Width in pixels.
    45 + 1, 0,              // Height in pixels. Add one for the empty data line.
                            // This may be a bug in the TivaWare Graphics Library.

    // Color map: 16 colors corresponding to 4 bits per pixel (bpp).
    15,
    0x6b, 0x72, 0x0b,
    0xb8, 0xbb, 0x89,
    0xde, 0xdf, 0xc8,
    0x92, 0x97, 0x4b,
    0xcc, 0xce, 0xab,
    0xa5, 0xa8, 0x68,
    0xf2, 0xf3, 0xeb,
    0x7e, 0x84, 0x28,
    0xae, 0xb1, 0x76,
    0xc2, 0xc4, 0x98,
    0xe9, 0xe9, 0xd9,
    0x88, 0x8d, 0x38,
    0x76, 0x7c, 0x19,
    0xd5, 0xd7, 0xba,
    0x9c, 0xa0, 0x59,
    0xfc, 0xfe, 0xfc,

    // First data line is unused.
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Image data.
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x24, 0x11, 0x19, 0x4a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa1, 0xee, 0x84, 0xdd, 0xd4, 0x95, 0xe5, 0x46, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x15, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x43, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x4e, 0x5a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd3, 0x9f, 0xff, 0xfd, 0x15, 0xee, 0xee, 0x89, 0xaf, 0xff, 0xa5, 0x56, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x65, 0x1f, 0xff, 0x4e, 0xe9, 0x4a, 0xff, 0xdf, 0x69, 0x83, 0x8a, 0xff, 0xa3, 0x4f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf2, 0x3a, 0xff, 0x93, 0x4d, 0x6f, 0xa4, 0xf6, 0x4f, 0xf1, 0xff, 0x48, 0x3a, 0xff, 0x95, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf5, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0xff, 0x23, 0x1f, 0xf6, 0x9f, 0xf4, 0xf6, 0x2f, 0x22, 0xf4, 0x2a, 0xe6, 0xff, 0xfd, 0xe6, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x78, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x5f, 0xf9, 0xe6, 0x29, 0xff, 0xd9, 0x5e, 0xee, 0xee, 0xe8, 0xdd, 0x6e, 0x6f, 0xff, 0xff, 0x23, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xe7, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xae, 0xff, 0x18, 0xff, 0xfd, 0x2e, 0x5d, 0x24, 0x18, 0x81, 0xd2, 0x93, 0x36, 0xff, 0xff, 0xff, 0xf2, 0xef, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd7, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0x36, 0xf9, 0x1a, 0x8a, 0xf8, 0x52, 0x13, 0xe1, 0x4d, 0xd9, 0x5b, 0xe1, 0x5f, 0xff, 0xff, 0xff, 0xff, 0x19, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xcd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xdf, 0xd5, 0xff, 0xf4, 0x3d, 0x13, 0xdf, 0xff, 0xff, 0xff, 0xf2, 0x86, 0x39, 0xef, 0xff, 0xff, 0xff, 0xfe, 0xaf, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xef, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xbe, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x25, 0xf6, 0x34, 0x2f, 0xa3, 0x2e, 0x4f, 0xff, 0xff, 0xff, 0xff, 0x95, 0xd1, 0x65, 0x3e, 0x8f, 0xff, 0xff, 0xf6, 0xef, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x5f, 0xff, 0xff, 0xff, 0xff, 0x4c, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xe6, 0xfe, 0xaf, 0xd4, 0x32, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xd9, 0xf3, 0xbe, 0x4d, 0x86, 0x1f, 0xff, 0xff, 0xff, 0x1d, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x8f, 0xff, 0xff, 0xff, 0x6e, 0xba, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfd, 0x1f, 0xd1, 0xff, 0xf3, 0x25, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xd8, 0x8a, 0xa9, 0x94, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xf3, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xf5, 0x9f, 0xff, 0xff, 0xfa, 0x75, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfe, 0x6f, 0x34, 0x14, 0x14, 0x56, 0xff, 0xff, 0xff, 0xf6, 0x84, 0xd7, 0x85, 0x29, 0xdf, 0xff, 0x68, 0xff, 0xff, 0xff, 0xf8, 0x2f, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6c, 0xf3, 0xdf, 0xff, 0xff, 0xdc, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x65, 0xf2, 0x1f, 0xfa, 0xe9, 0x2f, 0xff, 0xff, 0xff, 0xf9, 0x94, 0x1f, 0x29, 0x4d, 0xff, 0xf2, 0x13, 0xff, 0xff, 0xff, 0xfa, 0x5f, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x6c, 0x6f, 0xff, 0xf9, 0xcd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x44, 0xf5, 0xaf, 0xf1, 0xd8, 0xff, 0xff, 0xff, 0xff, 0x1a, 0x87, 0x84, 0x2d, 0xdf, 0x61, 0xe3, 0x93, 0xff, 0xff, 0xff, 0xff, 0x3f, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x2c, 0xff, 0xff, 0x1c, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x56, 0xf3, 0x24, 0xd3, 0x4d, 0xff, 0xff, 0xff, 0xf1, 0x34, 0x9a, 0x56, 0xa9, 0xf9, 0x7a, 0x15, 0x49, 0xff, 0xff, 0xff, 0xff, 0x8a, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x8e, 0xff, 0xf8, 0x7a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xef, 0x65, 0xa6, 0x28, 0x8f, 0xff, 0xff, 0xff, 0x4a, 0xe1, 0x24, 0x26, 0x16, 0x5d, 0xb5, 0x9f, 0xfd, 0xaf, 0xff, 0xff, 0xff, 0x44, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x34, 0xff, 0x5b, 0x6f, 0xff, 0xf6, 0xd1, 0x53, 0xbb, 0xbb, 0x35, 0x14, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xef, 0xd9, 0xff, 0x9d, 0x8f, 0xff, 0xff, 0xf9, 0x3a, 0x31, 0x3f, 0xf1, 0xf5, 0x6d, 0x9f, 0xf4, 0x2f, 0x4f, 0xff, 0xff, 0xff, 0xa8, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0x06, 0xfe, 0x36, 0xff, 0xd5, 0x7c, 0x38, 0x94, 0x22, 0x22, 0xd9, 0x83, 0xcc, 0x54, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf6, 0x8f, 0x92, 0x64, 0x52, 0x1f, 0xff, 0xff, 0xaa, 0x58, 0xfd, 0xdf, 0x46, 0x52, 0x84, 0xfa, 0xdf, 0x2f, 0x4f, 0xff, 0xff, 0xff, 0xf5, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0xef, 0x3e, 0xf2, 0x5c, 0x39, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x4e, 0x6f, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfa, 0x1f, 0x14, 0x4f, 0xed, 0x4f, 0xff, 0xfd, 0xef, 0x75, 0x3a, 0xf2, 0x24, 0x57, 0x8f, 0xf6, 0x95, 0x2f, 0x9f, 0xff, 0xff, 0xff, 0xfe, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x10, 0xd3, 0xe2, 0x3c, 0xe8, 0x5b, 0xcc, 0xcc, 0xc7, 0x38, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf2, 0x1f, 0x86, 0xff, 0xed, 0x4f, 0xff, 0xf4, 0x55, 0x26, 0x1f, 0x69, 0x68, 0xf5, 0x8f, 0x68, 0x91, 0x6f, 0x64, 0xff, 0xff, 0xff, 0xfe, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x37, 0x3b, 0xe0, 0xcc, 0x75, 0x92, 0x66, 0xff, 0x6a, 0x48, 0xbc, 0xed, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfa, 0x1f, 0x8a, 0xf4, 0xed, 0x9f, 0xff, 0x22, 0x91, 0x41, 0xf6, 0x16, 0x32, 0x5a, 0x5f, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xfe, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x00, 0x00, 0x0c, 0x82, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x97, 0x74, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xf6, 0x8f, 0x94, 0x8a, 0x52, 0x5f, 0xf4, 0xe4, 0xe1, 0x41, 0x49, 0x11, 0x39, 0xa9, 0x4f, 0xff, 0xff, 0xff, 0xf2, 0x2f, 0xff, 0xff, 0xfe, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x16, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x47, 0x3a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x5f, 0xd9, 0xff, 0x12, 0x3f, 0xf9, 0xa7, 0xaf, 0x8a, 0x19, 0x1e, 0x42, 0xf3, 0xff, 0xff, 0xff, 0xa6, 0xff, 0x4f, 0xff, 0xff, 0x68, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6c, 0x00, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xef, 0xa8, 0xf6, 0x99, 0xb6, 0xa9, 0x6c, 0xeb, 0x2a, 0x16, 0x49, 0x39, 0x68, 0xff, 0xff, 0xff, 0x2d, 0x4d, 0x2f, 0xff, 0xff, 0xd9, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0xcd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x75, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x56, 0xfe, 0xf8, 0x6e, 0x11, 0x13, 0x4d, 0xf1, 0xf6, 0x8d, 0xaf, 0xfd, 0x16, 0xff, 0xff, 0xff, 0xf6, 0xdf, 0xff, 0xff, 0xff, 0x12, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x0b, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x63, 0xdf, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x9d, 0xf5, 0x46, 0xf5, 0x2c, 0xd4, 0x5a, 0x12, 0xf9, 0x5d, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xdf, 0xff, 0xff, 0xff, 0xef, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0x65, 0xf2, 0x8f, 0xf9, 0x51, 0xdd, 0xe2, 0x86, 0xf4, 0x58, 0x6a, 0xff, 0xff, 0xff, 0xff, 0xf6, 0xdd, 0x4f, 0xff, 0xff, 0xf6, 0x5f, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf2, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfe, 0x6f, 0x3f, 0x9a, 0x8d, 0x55, 0x62, 0x46, 0x14, 0xf8, 0x35, 0xff, 0xff, 0xff, 0xff, 0xff, 0x64, 0x2f, 0xff, 0xff, 0xf1, 0xdf, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0x76, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xfd, 0x9f, 0xd1, 0x9f, 0x63, 0x1c, 0x9a, 0xff, 0xff, 0xf1, 0xd1, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xf3, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xf5, 0x00, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x6f, 0x12, 0x32, 0xb6, 0xff, 0xff, 0xf9, 0xa1, 0x21, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0x94, 0xff, 
    0xff, 0xff, 0xff, 0xf4, 0x70, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0x49, 0xfa, 0xef, 0x12, 0x9f, 0x45, 0xff, 0xff, 0xf2, 0xd8, 0xf2, 0x86, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0x3f, 0xff, 
    0xff, 0xff, 0x28, 0x7c, 0x00, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xaf, 0x91, 0x44, 0xdf, 0xfe, 0x6f, 0xff, 0xf6, 0x91, 0xff, 0x68, 0x96, 0xff, 0xf6, 0x12, 0xff, 0xff, 0xf8, 0xdf, 0xff, 
    0x13, 0xbc, 0x78, 0x5c, 0xb0, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x3f, 0xf8, 0x44, 0x4f, 0xf1, 0x22, 0x18, 0x8e, 0xd9, 0x6f, 0xff, 0xa8, 0x81, 0x19, 0x6f, 0xff, 0xff, 0xd5, 0xff, 0xff, 
    0x24, 0x26, 0xa3, 0x74, 0x0c, 0xff, 0x63, 0xdf, 0xff, 0xff, 0xff, 0xf3, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0xff, 0xf6, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd5, 0xff, 0x51, 0x8f, 0xfb, 0x6d, 0x2a, 0x6f, 0xf5, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x3f, 0xff, 0xff, 
    0xff, 0xae, 0xc9, 0xfb, 0x01, 0xff, 0x52, 0x8f, 0xff, 0xff, 0xff, 0xd4, 0x86, 0xff, 0xff, 0x92, 0xff, 0xff, 0xff, 0x21, 0xff, 0xfe, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0x8f, 0xf1, 0xc6, 0x15, 0x83, 0x55, 0x97, 0x5e, 0xaf, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xa3, 0x6f, 0xff, 0xff, 
    0xfa, 0xc1, 0x6f, 0x13, 0xca, 0xfd, 0x9f, 0x5f, 0xa4, 0xff, 0xff, 0x5f, 0x21, 0xff, 0x92, 0x65, 0x5d, 0x26, 0xfa, 0x82, 0xfa, 0x30, 0x3e, 0x2f, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0xff, 0xd9, 0x23, 0x61, 0x12, 0xf3, 0xa1, 0xaf, 0xff, 0xf5, 0xff, 0xff, 0xff, 0xfa, 0x36, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xfa, 0xc4, 0x7f, 0xf5, 0xff, 0x5f, 0x35, 0x8f, 0xf4, 0x9f, 0xf3, 0xf5, 0x14, 0xff, 0x61, 0xcd, 0xf4, 0xe1, 0xff, 0x5a, 0xff, 0x6f, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xe6, 0xff, 0xf1, 0x45, 0x94, 0xf6, 0x3d, 0xff, 0xff, 0xf1, 0xaf, 0xff, 0xff, 0x4e, 0x6f, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xf3, 0x11, 0x5f, 0x2b, 0x12, 0xe2, 0xb5, 0x36, 0xab, 0xb8, 0x93, 0x65, 0xbf, 0xf6, 0x88, 0x12, 0xf6, 0x7f, 0xff, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa3, 0x4f, 0xff, 0x28, 0x8b, 0x24, 0xb4, 0x91, 0x88, 0x99, 0x5f, 0xff, 0x65, 0x1f, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xd7, 0xfe, 0x9f, 0xf2, 0x48, 0x5f, 0x36, 0x2f, 0xf1, 0xa2, 0xdd, 0xf6, 0x8f, 0x65, 0x85, 0xdf, 0xf4, 0xe6, 0xfa, 0x53, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x85, 0xaf, 0xff, 0x66, 0xd2, 0x66, 0x66, 0x6a, 0x24, 0x9f, 0xf4, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0x22, 0xfb, 0x2f, 0xff, 0xff, 0xfa, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x2f, 0xf8, 0xdf, 0xff, 0x6d, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x5e, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x61, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xfc, 0x6f, 0xff, 0xff, 0xfa, 0xaf, 0xff, 0xff, 0xff, 0xff, 0x6e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x9e, 0xe9, 0x26, 0xff, 0xf6, 0xa4, 0x83, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x62, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf2, 0x98, 0x5e, 0xe5, 0x14, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
};

