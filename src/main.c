#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>

static struct termios org = {0};
static struct winsize w   = {0};

#define settmodraw() do{            \
  tcgetattr(STDIN_FILENO, &org);    \
	struct termios __r__ = org;       \
	__r__.c_lflag &= ~(ECHO | ICANON);\
	tcsetattr(STDIN_FILENO, TCSAFLUSH,\
	&__r__);                          \
} while(0);                         \

#define settmodorg() do{ \
  tcsetattr(STDIN_FILENO,\
	TCSAFLUSH, &org);      \
} while(0);              \

#define clear() do{       \
	write(STDOUT_FILENO,    \
	"\033[2J\033[H",        \
	sizeof "\033[2J\033[H");\
} while(0);               \

#define getwinsize() do{\
  ioctl(STDOUT_FILENO,  \
	TIOCGWINSZ, &w);      \
} while(0);             \

#define __T__len ((sizeof(int)*2+8)*3)
#define writewinsize() do{              \
  char __T__[__T__len] = {0};           \
  ioctl(STDOUT_FILENO,                  \
	TIOCGWINSZ, &w);                      \
	snprintf(__T__,  __T__len,            \
	"r:%d\nc:%d\nx:%d\ny:%d\n",           \
	w.ws_row, w.ws_col, w.ws_xpixel,      \
	w.ws_ypixel);                         \
	write(STDOUT_FILENO,__T__, __T__len); \
} while(0);                             \

#define width w.ws_col
#define height (w.ws_row - 3)

int main(void)
{
	signed int i;
	unsigned short int j;

	settmodraw();

	getwinsize();
	char frame[width*height];
	memset(frame, ' ', width*height);

  i=0;
	for ( ; i<height; ++i)
	{
		if ( i==0 )
		{
			memset(&frame[i*width], 'T', width);
			continue;
		}
		else if ( i==(height-1) )
		{
			memset(&frame[i*width], 'B', width);
			continue;
		}
		else
		{
			for ( j=0 ; j<width ; ++j )
			{
			  if ( j==0 )
          frame[i*width+j] = 'L';
				else if ( j==(width-1) )
					frame[i*width+j] = 'R'; 
			}
		}
	}

	write(STDOUT_FILENO, frame, width*height);

	settmodorg();
	return 0;
}
