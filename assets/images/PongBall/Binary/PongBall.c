
//{{BLOCK(PongBall)

//======================================================================
//
//	PongBall, 24x24@2, 
//	+ 5 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 3x3 
//	Total size: 80 + 20 = 100
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const unsigned int PongBallTiles[20] __attribute__((aligned(4)))=
{
	0x00000000,0x00000000,0x00000000,0x00000000,0xA0000000,0xAA00A800,0xABA0AE80,0xAAA8AAA8,
	0xABEA2AA8,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAA8,0xAAAEAAAA,0xAAAAAAAE,0xAAA8AAAA,
	0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,
};

const unsigned short PongBallMap[10] __attribute__((aligned(4)))=
{
	0x0001,0x0002,0x2001,0x0003,0x0004,0x2003,0x1001,0x3002,
	0x3001,0x0000,
};

//}}BLOCK(PongBall)
