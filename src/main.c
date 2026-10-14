#include <stdio.h>

#include <SDL3/SDL.h>


// defines

#define SCREEN_W 			1920
#define SCREEN_H 			1080
#define WIN_TITLE 		"Jokeri_Sieni.exe"


// asset paths

// PNGs
#define PNG_JOKRUSIENI		"res/image/jokrusieni.png"

// WAVs
#define WAV_JOKRUSIENI		"res/audio/jokrusieni.wav"

// structs
typedef struct {
	int x;
	int y;

} IVec2;

typedef struct {
	IVec2 upLCorner;
	IVec2 downRCorner;

} Rect2D;


// Main Function
int main() {	
	// Init SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) == false) {
		printf("SDL_Init: Failed to init SDL\n");
		printf("SDL_Init: %s\n", SDL_GetError());
		
		return -1;

	}
	
	// Create window and renderer
	SDL_Window* window 			= NULL;
	SDL_Renderer* renderer 	= NULL;

	if (!SDL_CreateWindowAndRenderer(WIN_TITLE, SCREEN_W, SCREEN_H, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {	
		printf("SDL_CreateWindowAndRenderer: Error creating window and/or renderer\n");
		printf("SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
		
		SDL_Quit();
		return -1;

	}
	
	// Open Default Playback Device as a audio stream
	SDL_AudioStream* def_Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);	
	if(def_Stream == NULL) {
		printf("SDL_OpenAudioDeviceStream: Error opening default playback device as stream\n");
		printf("SDL_OpenAudioDeviceStream: %s\n", SDL_GetError());
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}
	// unpause def_Stream
	if (SDL_ResumeAudioStreamDevice(def_Stream) == false) {
		printf("SDL_ResumeAudioStreamDevice: Error resuming audio stream device\n");
		printf("SDL_ResumeAudioStreamDevice: %s\n", SDL_GetError());
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	// Load PNGs
	SDL_Surface* png_Jokrusieni	= SDL_LoadPNG(PNG_JOKRUSIENI);
	
	if (png_Jokrusieni == NULL) {
		printf("SDL_LoadPNG: Error loading PNG image\n");
		printf("SDL_LoadPNG: %s\n", SDL_GetError());

		SDL_DestroySurface(png_Jokrusieni);

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}

	// Create Textures
	SDL_Texture* tex_Jokrusieni	= SDL_CreateTextureFromSurface(renderer, png_Jokrusieni);

	if(tex_Jokrusieni == NULL) {
		printf("SDL_CreateTextureFromSurface: Error creating texture from surface\n");
		printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Jokrusieni);
		SDL_DestroySurface(png_Jokrusieni);
	
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	// Create the Audio Specs
	// Audio Spec structs
	SDL_AudioSpec wav_Jokrusieni	= {.format = SDL_AUDIO_S16LE, .channels = 1, .freq = 44100};

	// Audio buffers
	Uint8* wav_Jokrusieni_Buf			= NULL;	

	// Audio lenghts
	Uint32 wav_Jokrusieni_Len 				= 0;
	
	// Load WAV data.
	if (SDL_LoadWAV(WAV_JOKRUSIENI, &wav_Jokrusieni, &wav_Jokrusieni_Buf, &wav_Jokrusieni_Len) == false) {
		printf("SDL_LoadWAV: Error loading WAV data\n");
		printf("SDL_LoadWAV: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Jokrusieni);
		SDL_DestroySurface(png_Jokrusieni);

		SDL_free(wav_Jokrusieni_Buf);
	
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	// Main loop
	bool running = true;
	while(running) {
		// Input
		SDL_Event e;
		SDL_PollEvent(&e);
	
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
		}
	
		// Update
		if (SDL_GetAudioStreamQueued(def_Stream) < (int)wav_Jokrusieni_Len) {
			if (SDL_PutAudioStreamData(def_Stream, wav_Jokrusieni_Buf, (int)wav_Jokrusieni_Len) == false) {
				printf("SDL_PutAudioStreamData: Error putting data to default plauback stream\n");
				printf("SDL_PutAudioStreamData: %s\n", SDL_GetError());
		
					SDL_DestroyTexture(tex_Jokrusieni);
					SDL_DestroySurface(png_Jokrusieni);

					SDL_free(wav_Jokrusieni_Buf);
	
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);

					SDL_Quit();
					return -1;
			}
	
		}
	
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, tex_Jokrusieni, NULL, NULL);		
		
		SDL_RenderPresent(renderer);
		
	}

	// Deinit SDL
	SDL_DestroyTexture(tex_Jokrusieni);
	SDL_DestroySurface(png_Jokrusieni);

	SDL_free(wav_Jokrusieni_Buf);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;

}
