#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_LINE_LEN 20

int
main(int argc,char *argv[])
{
	struct winsize ws;

	if (ioctl(0,TIOCGWINSZ,&ws)!=0) {
		fprintf(stderr,"TIOCGWINSZ:%s\n",strerror(errno));
		exit(-1);
	}

	const int lines = ws.ws_row;
	const int chars = ws.ws_col;

	FILE* fd = fopen("pages.txt", "r");
	int curr_char = fgetc(fd);

	int used_lines = 0;
	int chars_in_line = 0;
	int page = 0;

	while(curr_char != -1)
	{
		printf("%c", curr_char);
		curr_char = fgetc(fd);

		if(curr_char == '\n')
		{
			chars_in_line = chars;
		}

		chars_in_line++;
		if(chars_in_line >= chars)
		{
			used_lines++;
			chars_in_line = 0;

			if(used_lines == lines - 1)
			{
				page++;
				printf("[Page %d] b to go back 1 page, a to continue: ", page);
			
				char input = getchar();
				// Catches the \n
				char ignoreBuffer = getchar();

				if(input == 'b')
				{
					if(page < 2)
					{
						printf("\nOut of range!\n");
						return 0;
					}

					page -= 2;
					used_lines = 0;
					chars_in_line = 0;

					fseek(fd, (chars * lines * page), SEEK_SET);
				}
				else
				{
					used_lines = 0;
					chars_in_line = 0;
				}
			}
		}
	}

	return 0;
}
