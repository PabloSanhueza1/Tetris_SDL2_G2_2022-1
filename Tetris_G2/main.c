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

	coorColor nextColor;

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
	SDL_Rect mov;

	int close = 0;

	SDL_Event ev;
	int menu = 1;
	double iteracion = 0;
	int play = 1;
	int flagUsername = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("Error: %s\n", Mix_GetError());
	}

	Mix_Music* music = Mix_LoadMUS("Tetris_Main_Menu.mp3");
	Mix_Chunk* gameOverEffect = NULL;

	while (close == 0)
	{
		SDL_RenderClear(rend);

		crearMatrizImprimir();
		crearMatrizPantalla();

		srand(time(NULL));
		preview = blocks[rand() % 7];
		nextColor = preview.color;
		tetraminos actual = blocks[rand() % 7];
		coorColor actualColor = actual.color;

		int gridNum[20][10];
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				gridNum[i][j] = 0;
			}
		}

		int bordes[21][12];
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				bordes[i][j] = 0;
			}
		}

		for (int i = 0; i < 12; i++)
		{

			bordes[20][i] = 1; // borde inferior

		}
		for (int i = 0; i < 21; i++)
		{

			bordes[i][0] = 1;  // borde izquierdo
			bordes[i][11] = 1; // borde derecho
		}

		int matrizConversion[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrizConversion[i][j] = actual.pos1[i][j];
			}
		}

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
				next[i][j] = preview.pos1[i][j];
			}
		}

		int contGameOver = 0;
		int filasEliminadas = 0;

		//ajusta la altura h y el ancho w de la imagen
		mov.w = 45 * 4;
		mov.h = 45 * 4;

		mov.x = 595; //setea la posicion inicial en el eje x
		mov.y = -90; //setea la posicion inicial en el eje y

		play = 1;
		int contChar = 0;
		Mix_PlayMusic(music, -1);

		while (play == 1)
		{
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
						textRect.x = 600;
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
					if (contChar < 3)
					{
						contChar++;
						strcat(usuarioActual.nombre, ev.text.text);
					}
					break;
				case SDL_KEYDOWN:
					if (ev.key.keysym.sym == SDLK_RETURN && contChar > 0)
					{
						flagUsername = 1;
						if (menu != 0) {
							Mix_HaltMusic();
							music = Mix_LoadMUS("Tetris_Theme.mp3");
							Mix_PlayMusic(music, -1);
							contChar = 0;
						}
						menu = 0;
					}
					else if (flagUsername == 1) movement(ev, &mov, actual, matrizConversion);
					break;
				}
				break;
			}

			if (menu == 0)
			{
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
						matrizImp[i][j].x = mov.x;
						matrizImp[i][j].y = mov.y;
						matrizImp[i][j].w = 45;
						matrizImp[i][j].h = 45;
					}
				}

				bool flagColisionInferior = 0;

				// impresion de la siguiente pieza
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (next[i][j] == 1)
						{
							SDL_SetRenderDrawColor(rend, nextColor.r, nextColor.g, nextColor.b, nextColor.a);
							SDL_RenderFillRect(rend, &impPreview[i][j]);
						}
					}
				}

				// impresion de pieza que va cayendo
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (matrizConversion[i][j] == 1)
						{
							matrizImp[i][j].x += j * 45 - 45;
							matrizImp[i][j].y += i * 45 - 45;
							SDL_SetRenderDrawColor(rend, actualColor.r, actualColor.g, actualColor.b, actualColor.a);
							SDL_RenderFillRect(rend, &matrizImp[i][j]);
						}
					}
				}

				// deteccion de colisiones
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (matrizConversion[i][j] == 1)
						{
							int coorx = matrizImp[i][j].y / 45;
							int coory = (matrizImp[i][j].x - 415) / 45;

							// colision borde inferior
							if (bordes[coorx + 5][coory + 1] + matrizConversion[i][j] == 2)
							{
								if (iteracion >= 29)
								{
									flagColisionInferior = 1;
								}
								else
								{
									flagDestDown = 1;
									if (ev.key.keysym.scancode == SDL_SCANCODE_S || ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
									{
										flagColisionInferior = 1;
									}
								}
							}

							// colision con otros tetraminos
							if (gridNum[coorx + 5][coory] + matrizConversion[i][j] == 2)
							{

								if (iteracion >= 29)
								{

									flagColisionInferior = 1;
								}
								else
								{
									flagDestDown = 1;
									if (ev.key.keysym.scancode == SDL_SCANCODE_S || ev.key.keysym.scancode == SDL_SCANCODE_DOWN)
									{

										//mov.y -= 45;
										flagColisionInferior = 1;
									}

								}
							}

							// colision por izquierda
							if (gridNum[coorx + 4][coory - 1] + matrizConversion[i][j] == 2)
							{
								flagDestLeft = 1;
							}
							else flagDestLeft = 0;

							// colision por derecha
							if (gridNum[coorx + 4][coory + 1] + matrizConversion[i][j] == 2)
							{
								flagDestRight = 1;
							}
							else flagDestRight = 0;
						}
					}
				}

				// bloquear pieza y resetear
				if (flagColisionInferior == 1)
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							if (matrizConversion[i][j] == 1)
							{
								int coorx = matrizImp[i][j].y / 45;
								int coory = (matrizImp[i][j].x - 415) / 45;

								gridNum[coorx + 4][coory] = 1;
							}
						}
					}

					flagDestDown = 0;
					mov.x = 595;
					mov.y = -90;

					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							matrizConversion[i][j] = next[i][j];
						}
					}

					actualColor = nextColor;
					actual = preview;

					preview = blocks[rand() % 7];
					nextColor = preview.color;

					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							next[i][j] = preview.pos1[i][j];
						}
					}
				}

				// limites laterales
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int coorx = matrizImp[i][j].y / 45;
						int coory = (matrizImp[i][j].x - 415) / 45;

						// limite derecho
						if (bordes[coorx + 4][coory + 1 + 1] + matrizConversion[i][j] == 2)
						{
							flagDestRight = 1;
						}

						// limite izquierdo
						if (bordes[coorx + 4][coory + 1 - 1] + matrizConversion[i][j] == 2)
						{
							flagDestLeft = 1;
						}
					}
				}

				// imprimir los tetraminos fijos
				for (int i = 0; i < 20; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						if (gridNum[i][j] == 1)
						{
							SDL_SetRenderDrawColor(rend, 225, 198, 153, 255);
							SDL_RenderFillRect(rend, &matrizPantalla[i][j]);
						}
					}
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

				// contador de filas eliminadas
				char filasElim[100];
				sprintf(filasElim, "%d", filasEliminadas);

				// mostrar filas eliminadas
				textSurf = TTF_RenderText_Solid(font, &filasElim, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 208;
				textRect.y = 590;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				// imagen de grilla
				surface = IMG_Load("GRILLA_TETRIS.bmp");
				SDL_Texture* texture_grid = SDL_CreateTextureFromSurface(rend, surface);
				SDL_FreeSurface(surface);
				SDL_RenderCopy(rend, texture_grid, NULL, &rectangle);
				SDL_DestroyTexture(texture_grid);

				// grilla de preview
				surface = IMG_Load("Preview.png");
				texture = SDL_CreateTextureFromSurface(rend, surface);
				SDL_FreeSurface(surface);
				SDL_RenderCopy(rend, texture, NULL, &rectangle);
				SDL_DestroyTexture(texture);

				// mostrar score
				textSurf = TTF_RenderText_Solid(font, &scoreChar, colorText);
				textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
				SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
				textRect.x = 1010;
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
				textRect.x = 1010;
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
				textRect.x = 1010;
				textRect.y = 590;
				SDL_FreeSurface(textSurf);
				SDL_RenderCopy(rend, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);

				if (usuarioActual.puntaje >= 0 && usuarioActual.puntaje < 100) iteracion++;               // nivel 0
				else if (usuarioActual.puntaje >= 100 && usuarioActual.puntaje < 300) iteracion += 1.75;  // nivel 1
				else if (usuarioActual.puntaje >= 300 && usuarioActual.puntaje < 500) iteracion += 2.5;   // nivel 2
				else if (usuarioActual.puntaje >= 500 && usuarioActual.puntaje < 700) iteracion += 3.25;  // nivel 3
				else if (usuarioActual.puntaje >= 700 && usuarioActual.puntaje < 900) iteracion += 4;     // nivel 4
				else if (usuarioActual.puntaje >= 900 && usuarioActual.puntaje < 1100) iteracion += 4.75; // nivel 5

				if (iteracion >= 30)
				{
					mov.y += 45;
					iteracion = 0;
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
						menu = 0;
						play = 0;
					}
					else if (ev.key.keysym.sym == SDLK_m)
					{
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