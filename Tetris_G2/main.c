#include "funcionesTetris.h"

int main(int argc, char* args[])
{

	// Se inicializan todos los subsistemas
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("Error al inicializar SDL: %s\n", SDL_GetError());
	}
	SDL_Window* window = SDL_CreateWindow("Tetris_G2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Delay(2000);

	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = SCREEN_WIDTH;
	rectangle.h = SCREEN_HEIGHT;

	unsigned char scoreChar[7];

	Usuario usuarioActual;
	strcpy(usuarioActual.nombre, "");
	usuarioActual.puntaje = 0;

	//tetramino en movimiento
	srand(time(NULL));

	coorColor tetraColor;

	//Llama al programa que controla la tarjeta grafica y setea flags
	uint32_t render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Renderer* rend = SDL_CreateRenderer(window, -1, render_flags);

	TTF_Init();
	// Crea un surface para cargar imagen dentro del main memory
	SDL_Surface* surface;

	TTF_Font* font = TTF_OpenFont("modern-tetris.ttf", 25);
	SDL_Color colorText = { 255,0,0,255 };

	SDL_Surface* textSurf = NULL;
	SDL_Texture* textTexture = NULL;
	SDL_Rect textRect;

	//Deja controlar la imagen para moverlo con el keyboard
	SDL_Rect dest;

	int close = 0;

	SDL_Event ev;
	int menu = 1;
	double cont = 0;
	int play = 1;
	int flagUsername = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_Music* music = Mix_LoadMUS("Tetris_Main_Menu.mp3");
	Mix_Chunk* gameOverEffect = NULL;

	while (close == 0)
	{
		SDL_RenderClear(rend);

		crearMatrizImprimir();
		crearMatrizPantalla();
		crearMatrizGrid();
		crearMatrizInterseccion();

		srand(time(NULL));
		actual = blocks[rand() % 7];
		tetraColor = actual.color;
		tetraminos auxNext = blocks[rand() % 7];
		coorColor auxNextColor = auxNext.color;

		int gridNum[20][10];
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				gridNum[i][j] = 0;
			}
		}

		int aux[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				aux[i][j] = auxNext.pos1[i][j];
			}
		}
		//auxNext = blocks[rand() % 7];
		//auxNextColor = auxNext.color;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				impPreview[i][j].x = 135;
				impPreview[i][j].y = 180;
				impPreview[i][j].w = 45;
				impPreview[i][j].h = 45;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				impPreview[i][j].x += j * 45;
				impPreview[i][j].y += i * 45;

			}
		}
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				next[i][j] = actual.pos1[i][j];
			}
		}

		int contGameOver = 0;
		int filasEliminadas = 0;

		//ajusta la altura h y el ancho w de la imagen
		dest.w = 45;
		dest.h = 45;

		dest.x = 595; //setea la posicion inicial en el eje x
		dest.y = -90; //setea la posicion inicial en el eje y

		play = 1;
		int contChar = 0;
		Mix_PlayMusic(music, -1);
		while (play == 1)
		{
			//Mix_PlayMusic(mainMenuMusic, -1);
			SDL_RenderClear(rend);

			while (SDL_PollEvent(&ev))
			{
				if (menu == 1)
				{
					surface = IMG_Load("Main_Menu_Background.jpg");
					SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
					SDL_FreeSurface(surface);
					SDL_RenderCopy(rend, texture, NULL, &rectangle);
					SDL_DestroyTexture(texture);

					if (flagUsername == 0)
					{
						SDL_StartTextInput();

						textSurf = TTF_RenderText_Solid(font, &usuarioActual.nombre, colorText);
						textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
						SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
						textRect.x = 500;
						textRect.y = 520;
						SDL_FreeSurface(textSurf);
						SDL_RenderCopy(rend, textTexture, NULL, &textRect);
						SDL_RenderPresent(rend);
						SDL_DestroyTexture(textTexture);
					}
				}

				switch (ev.type)
				{
					// Para eliminar la ventana
				case SDL_QUIT:
					play = 0;
					close = 1;
					break;
				case SDL_TEXTINPUT:
					if (contChar < 10)
					{
						contChar++;
						strcat(usuarioActual.nombre, ev.text.text);
						printf("usuario: %s", usuarioActual.nombre);
					}
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.sym == SDLK_RETURN && contChar > 0)
					{
						printf("SE APRETO ENTER\n");
						flagUsername = 1;
						if (menu != 0) {
							Mix_HaltMusic();
							music = Mix_LoadMUS("Tetris_Theme.mp3");
							Mix_PlayMusic(music, -1);
							contChar = 0;
						}
						menu = 0;
					}
					else if (flagUsername == 1) movement(ev, &dest, auxNext, aux);
					break;
				}
				break;
			}

			if (menu == 0)
			{
				// limites de movimiento
				// limits(&dest);

				SDL_RenderClear(rend);

				//imagen de fondo
				surface = IMG_Load("Background_image.jpg");
				SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
				SDL_FreeSurface(surface);
				SDL_RenderCopy(rend, texture, NULL, &rectangle);
				SDL_DestroyTexture(texture);

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						matrizImp[i][j].x = dest.x;
						matrizImp[i][j].y = dest.y;
						matrizImp[i][j].h = dest.h;
						matrizImp[i][j].w = dest.w;
					}
				}

				int flagReset = 0;
				int flagInferior = 0;
				int flagColisionInferior = 0;
				//imprimirPiezas(matrizImp, aux, tetraColor);
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (next[i][j] == 1)
						{
							SDL_SetRenderDrawColor(rend, tetraColor.r, tetraColor.g, tetraColor.b, tetraColor.a);
							SDL_RenderFillRect(rend, &impPreview[i][j]);
						}
					}
				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (aux[i][j] == 1)
						{
							matrizImp[i][j].x += j * 45 - 45;
							matrizImp[i][j].y += i * 45 - 45;
							SDL_SetRenderDrawColor(rend, auxNextColor.r, auxNextColor.g, auxNextColor.b, auxNextColor.a);
							SDL_RenderFillRect(rend, &matrizImp[i][j]);
						}
					}
				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						for (int l = 0; l < 10; l++)
						{
							// limite inferior
							if (SDL_HasIntersection(&matrizImp[i][j], &matrizGrid[15][l]) == 1)
							{
								flagInferior = 1;
								flagReset = 1;
							}
						}
					}
				}

				if (flagInferior == 1)
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							int coorx = matrizImp[i][j].y / 45;
							int coory = (matrizImp[i][j].x - 415) / 45;

							//SDL_IntersectRect(&matrizImp[i][j], &matrizPantalla[coorx + 4][coory], &matrizInterseccion[coorx + 4][coory]);
							//matrizGrid[coorx + 4][coory] = matrizInterseccion[coorx + 4][coory];
							gridNum[coorx + 4][coory] = 1;
						}
					}
				}

				if (flagReset == 1)
				{
					//reset(&dest, actual, &tetraColor, aux, &auxNext, &auxNextColor, next);
					dest.x = 595;
					dest.y = -90;
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							aux[i][j] = next[i][j];
						}
					}

					auxNextColor = tetraColor;
					auxNext = actual;

					actual = blocks[rand() % 7];
					tetraColor = actual.color;

					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							next[i][j] = actual.pos1[i][j];
						}
					}
					//auxNext = actual;
				}

				// colision inferior
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int coorx = matrizImp[i][j].y / 45;
						int coory = (matrizImp[i][j].x - 415) / 45;

						if (gridNum[coorx + 5][coory] + aux[i][j] == 2)
						{
							if (cont == 29) flagColisionInferior = 1;
							else if (ev.key.keysym.scancode == SDL_SCANCODE_S || ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
							{
								dest.y -= 45;
								flagColisionInferior = 1;
							}
						}
					}
				}

				if (flagColisionInferior == 1)
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							int coorx = matrizImp[i][j].y / 45;
							int coory = (matrizImp[i][j].x - 415) / 45;

							//SDL_IntersectRect(&matrizImp[i][j], &matrizPantalla[coorx + 4][coory], &matrizInterseccion[coorx + 4][coory]);
							//matrizGrid[coorx + 4][coory] = matrizInterseccion[coorx + 4][coory];
							gridNum[coorx + 4][coory] = 1;
						}
					}
					//reset(&dest, actual, &tetraColor, aux, &auxNext, &auxNextColor, next);
					dest.x = 595;
					dest.y = -90;
					printf("NEXT\n");
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							printf("%d ", next[i][j]);
							aux[i][j] = next[i][j];
						}
						printf("\n");
					}
					auxNextColor = tetraColor;
					auxNext = actual;

					actual = blocks[rand() % 7];
					tetraColor = actual.color;

					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							next[i][j] = actual.pos1[i][j];
						}
					}

				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int coorx = matrizImp[i][j].y / 45;
						int coory = (matrizImp[i][j].x - 415) / 45;
						if (gridNum[coorx][coory - 1] + aux[i][j] == 2)
						{
							dest.x += 0;
						}
						if (gridNum[coorx][coory + 1] + aux[i][j] == 2)
						{
							dest.x -= 0;
						}
					}
				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						// limite derecho
						if (matrizImp[i][j].x + 45 > 865)
						{
							dest.x -= 45;
						}

						// limite izquierdo
						if (matrizImp[i][j].x - 45 < 415)
						{
							dest.x = 415;
							//matrizImp[i][j].x = 415 + 45;
						}

						int coorx = matrizImp[i][j].y / 45;
						int coory = (matrizImp[i][j].x - 415) / 45;
						printf("MATIMP %d --- GRIDNUM %d\n", coorx / 45, gridNum[coorx + 1][coory]);
						if ((matrizImp[i][j].x + 45) / 45 + gridNum[coorx + 1][coory] == 2)
						{
							dest.x -= 45;
							//matrizImp[i][j].x = 415 + 45;
						}
						if ((matrizImp[i][j].x - 45) / 45 + gridNum[coorx - 1][coory] == 2)
						{
							dest.x = 45;
							//matrizImp[i][j].x = 415 + 45;
						}
					}
				}

				for (int i = 0; i < 20; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						printf("%d ", gridNum[i][j]);
						if (gridNum[i][j] == 1)
						{
							SDL_SetRenderDrawColor(rend, 225, 198, 153, 255);
							SDL_RenderFillRect(rend, &matrizPantalla[i][j]);
						}
					}
					printf("\n");
				}

				int limpiar = 0;
				for (int i = 19; i >= 0; i--)
				{
					if (filaCompleta(gridNum, i) == 1)
					{
						vaciarFila(gridNum, i);
						limpiar++;
						filasEliminadas++;
					}
					else if (limpiar > 0)
					{
						moverFilas(gridNum, i, limpiar);
					}
				}
				usuarioActual.puntaje = asignarPuntaje(limpiar, usuarioActual.puntaje);
				sprintf(scoreChar, "%d", usuarioActual.puntaje);

				//Contador de filas eliminadas
				char filasElim[100];
				sprintf(filasElim, "%d", filasEliminadas);

				textSurf = TTF_RenderText_Solid(font, &filasElim, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 208;
				textRect.y = 590;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				//imagen de grilla
				surface = IMG_Load("GRILLA_TETRIS.bmp");
				SDL_Texture* texture_grid = SDL_CreateTextureFromSurface(rend, surface);
				SDL_FreeSurface(surface);
				SDL_RenderCopy(rend, texture_grid, NULL, &rectangle);
				SDL_DestroyTexture(texture_grid);

				//grila de preview
				surface = IMG_Load("Preview.png");
				texture = SDL_CreateTextureFromSurface(rend, surface);
				SDL_FreeSurface(surface);
				SDL_RenderCopy(rend, texture, NULL, &rectangle);
				SDL_DestroyTexture(texture);

				// mostrar score
				//mostrarScore(&textSurf, &font, &textTexture, textRect, score, colorText);
				textSurf = TTF_RenderText_Solid(font, &scoreChar, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 1040;
				textRect.y = 190;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				// mostrar highscore
				char strHighscore[100];
				FILE* highscore = fopen("highscore.txt", "r");
				fgets(strHighscore, 100, highscore);
				fclose(highscore);

				textSurf = TTF_RenderText_Solid(font, &strHighscore, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 1040;
				textRect.y = 490;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				// mostrar usuario de highscore
				char strUsuarioHighscore[10];
				FILE* usuarioHighscore = fopen("usuarioHighscore.txt", "r");
				fgets(strUsuarioHighscore, 10, usuarioHighscore);
				fclose(usuarioHighscore);

				textSurf = TTF_RenderText_Solid(font, &strUsuarioHighscore, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 1020;
				textRect.y = 590;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				if (usuarioActual.puntaje >= 0 && usuarioActual.puntaje < 100) cont++;          // nivel 0
				if (usuarioActual.puntaje >= 100 && usuarioActual.puntaje < 300) cont += 1.75;  // nivel 1
				if (usuarioActual.puntaje >= 300 && usuarioActual.puntaje < 500) cont += 2.5;   // nivel 2
				if (usuarioActual.puntaje >= 500 && usuarioActual.puntaje < 700) cont += 3.25;  // nivel 3
				if (usuarioActual.puntaje >= 700 && usuarioActual.puntaje < 900) cont += 4;     // nivel 4
				if (usuarioActual.puntaje >= 900 && usuarioActual.puntaje < 1100) cont += 4.75; // nivel 5

				if (cont >= 30)
				{
					dest.y += 45;
					cont = 0;
				}

				for (int i = 0; i < 20; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						if (gridNum[i][j] == 1)
						{
							contGameOver++;
							break; // lee cada fila hasta encontrar un 1
						}
					}
				}

				SDL_RenderPresent(rend);

				printf("CONTGAMEOVER: %d\n\n\n", contGameOver);
				if (contGameOver >= 17)
				{
					char strHighscore[100];
					FILE* highscore = fopen("highscore.txt", "r");
					fgets(strHighscore, 100, highscore);
					int highscoreInt = atoi(strHighscore);
					fclose(highscore);
					if (usuarioActual.puntaje > highscoreInt)
					{
						FILE* highscore = fopen("highscore.txt", "w");
						sprintf(strHighscore, "%d", usuarioActual.puntaje);
						fprintf(highscore, strHighscore);
						fclose(highscore);
						FILE* usuarioHighscore = fopen("usuarioHighscore.txt", "w");
						fprintf(usuarioHighscore, usuarioActual.nombre);
						fclose(usuarioHighscore);
					}

					surface = IMG_Load("Game_Over.png");
					SDL_DestroyTexture(texture);
					SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
					SDL_FreeSurface(surface);
					SDL_RenderCopy(rend, texture, NULL, &rectangle);
					SDL_RenderPresent(rend);
					SDL_DestroyTexture(texture);

					Mix_HaltMusic();
					gameOverEffect = Mix_LoadWAV("Game_Over_Effect.mp3");
					Mix_PlayChannel(-1, gameOverEffect, 0);

					usuarioActual.puntaje = 0;

					break;
				}
				else contGameOver = 0;
			}
			SDL_RenderClear(rend);
		}

		SDL_StopTextInput();

		while (play == 1)
		{
			while (SDL_PollEvent(&ev))
			{
				switch (ev.type)
				{
					// Para eliminar la ventana
				case SDL_QUIT:
					close = 1;
					play = 0;
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.sym == SDLK_r)
					{
						printf("\n\nR\n\n");
						menu = 0;
						play = 0;
					}
					else if (ev.key.keysym.sym == SDLK_m)
					{
						printf("\n\nM\n\n");
						strcpy(usuarioActual.nombre, "");
						menu = 1;
						play = 0;
						flagUsername = 0;
						music = Mix_LoadMUS("Tetris_Main_Menu.mp3");
					}
				}
				break;
			}
		}
		Mix_FreeChunk(gameOverEffect);
		SDL_RenderClear(rend);
		liberarMatrizImprimir(matrizImp);
		liberarMatrizPantalla(matrizPantalla);
		liberarMatrizGrid(matrizGrid);
		liberarMatrizInterseccion(matrizInterseccion);

	}

	surface = NULL;
	window = NULL;
	Mix_FreeMusic(music);
	Mix_FreeChunk(gameOverEffect);
	gameOverEffect = NULL;
	Mix_Quit();
	TTF_CloseFont(font);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}