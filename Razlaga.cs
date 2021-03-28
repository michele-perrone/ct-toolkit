1. vaja: stiskanje
Datoteka IMG. Velikost slik: 512*512. Vsaka vrednost pixela je [-2048, 2047]
Datoteka RGB. Gre za polje od 0 do 255. Vsak element polja je RGB vrednost.
	i = ( ( x + 2048 ) / 4095 ) * 255
	x je vrednost v datoteki IMG
	i je indeks v RGB polju
	
/* Primer branja v C# - prvi del */
byte[,] barve = new byte [256, 3];
BinaryReader br = new BinaryReader (FileOpen("fileName", FileMode.Open));
for (int i = 0; i < 256; i++)
{
	for (int j = 0; j < 3; j++)
	{
		barve[i,j] = br.ReadByte();
	}
}

/* Primer branja v C# - drugi del */
short[,] img = new short [512, 512];
BinaryReader br = new BinaryReader (FileOpen("fileName", FileMode.Open));
for (int i = 0; i < 512; i++)
{
	for (int j = 0; j < 512; j++)
	{
		img[i,j] = br.ReadInt();
	}
}

Sliko delimo na 4 enake dele. To izvajamo na originalni sliki, ne na tisti, kjer smo izracunali barve.
Lahko nastavimo threshold: toleranca v tem, ko gledamo, ce so barve v kvadratu iste.
Uporabimo razliko od prvega piksla (najhitreje), ali povprecje.