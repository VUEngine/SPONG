//======================================================================
//
//	PongBallParticle, 8x40@2, 
//	+ 5 tiles not compressed
//	+ regular map (flat), not compressed, 1x5 
//	Total size: 80 + 12 = 92
//
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

const uint32 PongBallParticleTiles[20] __attribute__((aligned(4)))=
{
	0x00C00000,0x3FF003C0,0x03C00FFC,0x00000300,
	0x00000000,0x03F00300,0x00C00FC0,0x00000000,0x00000000,0x03C00000,0x000003C0,0x00000000,
	0x00000000,0x00C00000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint16 PongBallParticleMap[6] __attribute__((aligned(4)))=
{
	0x0000,0x0001,0x0002,0x0003,0x0004,0x0000,
};
