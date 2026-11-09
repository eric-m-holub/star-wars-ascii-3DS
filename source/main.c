#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <3ds.h>
#include <stdlib.h>

/*
	This app wouldn't be possible without the following:
	Animation credit: Simon Jansen (https://www.asciimation.co.nz/)
	Code credit: JLChnToZ (https://gist.github.com/JLChnToZ/5f4b1a3e81373b3b20330c6a89067f1b)
*/

int printNextLines(FILE* file)
{
	char line[256];
	int ret = -1;
	int lineCount = 0;

	while (fgets(line, sizeof(line), file))
	{
		//return how times this frame should be shown (15 FPS)
		if (lineCount == 0) {
			ret = atoi(line);
		}

		//print the frame
		else {
			printf("%s", line);
		}

		//you've reached the end of the frame. exit
		if (lineCount >= 13) {
			break;
		}

		lineCount++;
	}

	return ret;
}

void sleep_milliseconds(long milliseconds) {
    svcSleepThread(milliseconds * 1000000); // Convert milliseconds to nanoseconds
}


void clearConsole() {
    printf("\033[H\033[J");
}


int main()
{
	gfxInitDefault();
	gfxSetWide(true);
	consoleInit(GFX_TOP, NULL);
	romfsInit();

	FILE* f = fopen("romfs:/starwars.txt", "r");


	while (aptMainLoop()) {
			// Scan input
			hidScanInput();

			// Get the state of the keys
			u32 kDown = hidKeysDown();

			// Check if the START button was just released. If so, exit the loop
			if (kDown & KEY_START) {
					break; // Exit the loop if the START button is pressed
			}

			//print out next frame of Star Wars, and then wait for the specified amount of time
			int waitFrames = printNextLines(f);

			//if movie is over... exit the loop
			if (waitFrames == -1) {
				break;
			}

			//intended FPS = 15
			long waitTime = ((long)waitFrames / 15.0) * 1000;
			sleep_milliseconds(waitTime);
			//clear console output for next frame
			clearConsole();

			// Swap buffers
			gfxFlushBuffers();
			gfxSwapBuffers();
	}

	romfsExit();
	gfxExit();

	fclose(f);

	return 0;
}
