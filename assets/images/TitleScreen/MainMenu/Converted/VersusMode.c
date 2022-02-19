//---------------------------------------------------------------------------------------------------------
//
//  VersusMode
//  • 88×32 pixels
//  • 44 tiles, unreduced, not compressed
//  • 11×4 map, not compressed
//  Size: 708 + 88 = 796
//
//---------------------------------------------------------------------------------------------------------

const uint32 VersusModeTiles[177] __attribute__((aligned(4))) =
{
    0x00000000,0xC3F00000,0xC3F0C3F0,0xF3F0F3F0,0x3FF0F3F0,0xFFF30000,0x03F3C3F3,0x03F03FF0,
    0xFFF0C3F0,0xFFF30000,0xC3F0C3F3,0xF3F0FFF0,0xC3F3C3F3,0xFFF30000,0x03F3C3F3,0xC000FFF3,
    0xFFF3C3F3,0xC3F30000,0xC3F0C3F3,0xC3F3C3F3,0xFFF3C3F3,0xFFF30000,0x03F3C3F3,0xC003FFF3,
    0xFFF3C3F3,0xF0030000,0xF000F003,0xF003F003,0xF003F003,0xCF030000,0xCFFFCFCF,0xCF33CFFF,
    0xCF03CF03,0xCFFF0000,0xCF0FCF0F,0xCF0FCF0F,0xCFFFCF0F,0xC3FF0000,0xCF0FCF0F,0xCF0FCF0F,
    0xC3FFCF0F,0x0FFF0000,0x000F0F0F,0x000F00FF,0x0FFF0F0F,0x03F00000,0xFFF0CFF0,0x33F0FFF0,
    0x03F003F0,0xFFCF0000,0x0FCF0FCF,0x0FCFFFCF,0xFFCF0FCF,0x0FCF0000,0x0FC00FCF,0x0FC0FFC0,
    0x0FCF0FCF,0xFFCF0000,0x0FCF0FCF,0xCFCFFFCF,0x0FCF0FCF,0xFFCF0000,0x0FCF0FCF,0x0003FFCF,
    0xFFCF0FCF,0xFFCF0000,0x0FC00FCF,0x0FCFFFCF,0x0FCF0FCF,0xCFCF0000,0xCFCFCFCF,0xCFC0CFCF,
    0xCFC0CFC0,0xCFFF0000,0xC00FCF0F,0xC00FC0FF,0xCFFFCF0F,0xC00F0000,0xC00FC00F,0xC00FC00F,
    0xCFFFCF0F,0xCFFF0000,0xC00FCF0F,0xC00FC0FF,0xCFFFCF0F,0x0FFF0000,0x0F0F0F0F,0x03CF0FFF,
    0x0F0F0F0F,0x03F00000,0xFFF0CFF0,0x33F0FFF0,0x03F003F0,0xFFCF0000,0x0FCF0FCF,0x0FCF0FCF,
    0xFFCF0FCF,0xFFCF0000,0x0FCF0FCF,0x0FCF0FCF,0xFFCF0FCF,0xFFC30000,0x0FCF0FCF,0x0FCF0FCF,
    0xFFC30FCF,0xC00F0000,0xC00FC00F,0xC00FC00F,0xC00FC00F,0xCF0F0000,0xCF0FCF0F,0xC3CFC3CF,
    0xC0FFC3CF,0xCFFF0000,0xC00FCF0F,0xC00FC0FF,0xCFFFCF0F,0xCFFF0000,0xCF0FCF0F,0x03CFCFFF,
    0xCF0FCF0F,0xCFFF0000,0xC00FCF0F,0xCF00CFFF,0xCFFFCF0F,0xCF0F0000,0xCF0FCF0F,0x0F0FCF0F,
    0xCFFFCF0F,0x0FFF0000,0x000F0F0F,0x0F000FFF,0x0FFF0F0F,0xC3F00000,0xC3F0C3F0,0xF3F0F3F0,
    0x3FF0F3F0,0xFFF30000,0x03F3C3F3,0x03F03FF0,0xFFF0C3F0,0xFFF30000,0xC3F0C3F3,0xF3F0FFF0,
    0xC3F3C3F3,0xFFF30000,0x03F3C3F3,0xC000FFF3,0xFFF3C3F3,0xC3F30000,0xC3F0C3F3,0xC3F3C3F3,
    0xFFF3C3F3,0xFFF30000,0x03F3C3F3,0xC003FFF3,0xFFF3C3F3,0xF0030000,0xF000F003,0xF003F003,
    0xF003F003,0xCF030000,0xCFFFCFCF,0xCF33CFFF,0xCF03CF03,0xCFFF0000,0xCF0FCF0F,0xCF0FCF0F,
    0xCFFFCF0F,0xC3FF0000,0xCF0FCF0F,0xCF0FCF0F,0xC3FFCF0F,0x0FFF0000,0x000F0F0F,0x000F00FF,
    0x0FFF0F0F,
};

const uint16 VersusModeMap[44] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,
    0x0028,0x0029,0x002A,0x002B,
};
