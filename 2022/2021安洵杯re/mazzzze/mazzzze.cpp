#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<windows.h>

using namespace std;
BYTE key[44];
BYTE k[44] = {14, 93, 125, 125, 93, 78, 78, 78, 93, 125, 107, 75, 93, 93, 78, 78, 89, 89, 89, 89, 107, 93, 83, 36, 123, 52, 7, 73, 1, 27, 35, 39, 126, 53, 63, 18, 27, 41, 50, 9, 22, 18, 96, 74};
char base64char[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
string base = "QCAmN2sYNGUfR3EvOUMuNWYkW3k1JR==";
int len1, x, y, z;
char *mapp="S#O######O########O##############################################O#############################O##O##########O#####################O####################O###O#####O##############O#####O###O#############O##############O#OO#########################O###############O########################################O######O###############O######O########################OO###O#O#OOO####################O#################################O#####O#######################################O#############O#############O##############O#####O###############OO###############O#######O####E#####################################O###########O##O###################################OO###########OOO################O##########O###O##############O##########O#O#O#O#####O###########";


int len(string flag)
{
	char s[22];
	for(int i = 0; i < flag.length(); i++)
		s[i] = flag[i];
	if(!*s)
		return 0;
	for(int i = 0; i < 63; i+=2)
		swap(base64char[i], base64char[i + 1]);
	return sizeof(s);
}

char *base64_encode( const unsigned char * bindata, char * base64, int binlength )
{
    int i, j;
    unsigned char current;

    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}

void encry()
{
	for(int i = 1; i < 44; i++)
		k[i] = (key[i] & 0x1F) | (key[i - 1] & 0xE0);
	k[0] = (key[0] & 0x1F) | (key[43] & 0xE0);
}

void answer(string flag)
{
	string s = "";
	for(int i = 0; i < 44; i++)
		s += flag[i % flag.length()] ^ key[i];
	cout << "Congratulations!!! \nBut more than...";
	Sleep(10000000);
	cout << "Maybe there's something wrong with the front..."; 
	return ;
}

void fail()
{
	cout << "Think again...";
}

int main()
{
	string flag;
	cout << "Welcome to this game£¡\n";
	cout << "please input your flag:";	
	cin >> flag;
	len1 = len(flag);
	if(len1 != 22)
	{
		fail();
		return 0;
	}
	for(int i = 0; i < 22; i++)
		switch(flag[i])
		{
			case 'w':
				x += 2;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
			case 's':
				x -= 1;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
			case 'd':
				y += 2;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
			case 'a':
				y -= 1;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
			case 'W':
				z += 1;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
			case 'S':
				z -= 1;
				if(*(mapp + z * 10 * 15 + x * 15 + y) != 'O' && *(mapp + z * 10 * 15 + x * 15 + y) != 'E')
				{
					fail();
					return 0;
				}
				break;
		}
	if(*(mapp + z * 10 * 15 + x * 15 + y) == 'E')
		answer(flag);
	else 
	{
		fail();
		return 0;
	}
	return 0;
}

