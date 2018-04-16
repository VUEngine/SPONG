
//{{BLOCK(Paddle)

//======================================================================
//
//	Paddle, 32x32@2, 
//	+ 3 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 4x4 
//	Total size: 48 + 32 = 80
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned int PaddleTiles[12] __attribute__((aligned(4)))=
{
	0x00000000,0x00000000,0x00000000,0x00000000,0xFFFFFFFC,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
	0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
};

const unsigned short PaddleMap[16] __attribute__((aligned(4)))=
{
	0x0001,0x0002,0x0002,0x2001,0x0002,0x0002,0x0002,0x0002,
	0x0002,0x0002,0x0002,0x0002,0x1001,0x0002,0x0002,0x3001,
};

//}}BLOCK(Paddle)
