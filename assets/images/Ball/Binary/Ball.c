
//{{BLOCK(Ball)

//======================================================================
//
//	Ball, 16x16@2, 
//	+ 2 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 2x2 
//	Total size: 32 + 8 = 40
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned int BallTiles[8] __attribute__((aligned(4)))=
{
	0x00000000,0x00000000,0x00000000,0x00000000,0xEA80A800,0xEAA8AAA0,0xEAAAEAA8,0xFFEEEAAA,
};

const unsigned short BallMap[4] __attribute__((aligned(4)))=
{
	0x0001,0x2001,0x1001,0x3001,
};

//}}BLOCK(Ball)
