#include <assert.h>
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

int main(void)
{
	signed int i;
	ssize_t n;

	settmodraw();

	getwinsize();
	char frame[w.ws_col*(w.ws_row - 3)];
	memset(frame, ' ', w.ws_col*(w.ws_row - 3));

	memset(frame, 'T', w.ws_col);
	memset(&frame[(w.ws_row - 3 - 1)*w.ws_col], 'B', w.ws_col);

	for ( i=0 ; i<(w.ws_row - 3) ; ++i )
	{
		frame[i*w.ws_col] =  'L';
		frame[i*w.ws_col+w.ws_col-1] =  'R';
	}

	n=write(STDOUT_FILENO, frame, w.ws_col*(w.ws_row - 3));
	assert( n>0 );

	settmodorg();
	return 0;
}
