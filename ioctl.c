// WARNING: Program contains a bug!!
// 
// Going back to a previous page doesn't
// work correctly after page 3.
// -------------------
//   WALLINGER Marc
//   2AHIF (2018/19)
// -------------------
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc,char *argv[])
{
	struct winsize ws;

	if (ioctl(0,TIOCGWINSZ,&ws)!=0) {
		fprintf(stderr,"TIOCGWINSZ:%s\n",strerror(errno));
		exit(-1);
	}

	// Get height/width of terminal
	const int lines = ws.ws_row;
	const int chars = ws.ws_col;

	// Open file read-only
	FILE* fd = fopen("pages.txt", "r");
	int curr_char = fgetc(fd);

	// Counters for lines/chars/page
	int used_lines = 0;
	int chars_in_line = 0;
	int page = 0;

	// Break on EOF
	while(curr_char != -1)
	{
		// Print current char
		printf("%c", curr_char);
		curr_char = fgetc(fd);

		// Count new lines as full chars in line
		if(curr_char == '\n')
		{
			chars_in_line = chars;
		}

		chars_in_line++;
		// If current line is full
		if(chars_in_line >= chars)
		{
			used_lines++;
			chars_in_line = 0;

			if(used_lines == lines - 1)
			{
				page++;
				printf("[Page %d] b + Enter to go previous page, Enter twice to continue: ", page);
			
				char input = getchar();
				// Catches the \n
				char ignoreBuffer = getchar();

				if(input == 'b')
				{
					// Check range
					if(page < 2)
					{
						printf("\nOut of range!\n");
						return 0;
					}

					// Go to previous page
					page -= 2;
					used_lines = 0;
					chars_in_line = 0;
					fseek(fd, (chars * lines * page), SEEK_SET);
				}
				else
				{
					// Go to next page
					used_lines = 0;
					chars_in_line = 0;
				}
			}
		}
	}

	return 0;
}
