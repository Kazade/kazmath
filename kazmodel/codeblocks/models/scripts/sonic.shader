models/players/sonic/shadow
{
	{
		map models/players/sonic/shadow.tga
		rgbGen lightingDiffuse
	}
	{
		map models/players/sonic/shadow-face.tga
		blendfunc blend
		rgbGen lightingDiffuse
		alphaGen wave triangle -4 6 6 0.3
	}
	{
		map models/players/sonic/shadow.tga
		rgbGen lightingDiffuse
		alphaFunc GE128
	}
}

models/players/sonic/power
{
	cull disable
	{
                map models/players/sonic/power.tga
                tcMod scroll 0.5 0
		blendfunc add
		rgbGen identity
	}
}

models/players/sonic/power_b
{
	cull disable
	{
                map models/players/sonic/power_b.tga
                tcMod scroll 0.5 0
		blendfunc add
		rgbGen identity
	}
}

models/players/sonic/power_r
{
	cull disable
	{
                map models/players/sonic/power_r.tga
                tcMod scroll 0.5 0
		blendfunc add
		rgbGen identity
	}
}
