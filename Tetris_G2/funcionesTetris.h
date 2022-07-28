#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>

static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 720;
SDL_Renderer* rend;

SDL_Rect rectangle;

typedef struct
{
	char nombre[10];
	int puntaje;
}Usuario;

int consultarUsername(char* nombre)
{
	printf("\n\nIntroducir username:\n");
	scanf("%[^\n]", nombre);
	getchar();
	return 1;
}

typedef struct
{
	int r;
	int g;
	int b;
	int a;
}coorColor;

typedef struct
{
	int pos1[4][4];
	int pos2[4][4];
	int pos3[4][4];
	int pos4[4][4];
	coorColor color;
}tetraminos;

tetraminos blocks[7] =
{
	{
		{
			{0,1,0,0}, // L Pos1 block
			{0,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // L Pos2 block
			{0,1,1,1},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // L Pos3 block
			{0,1,1,0},
			{0,0,1,0},
			{0,0,1,0}
		},
		{
			{0,0,0,0}, // L Pos4 block
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
	{255,100,0,255} // color naranjo
	},
	{
		{
			{0,0,0,0}, // [] Pos 1 block
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // [] Pos 2 block
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // [] Pos 3 block
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // [] Pos 4 block
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
	{255,255,0,255} // color amarillo
	},
	{
		{
			{0,0,0,0}, // I Pos 1 block
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0}, // I Pos 1 block
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0}, // I Pos 3 block
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0}, // I Pos 4 block
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
	{0,255,255,255} // color celeste
	},
	{
		{
			{0,0,0,0}, // L2 Pos 1 block
			{0,1,0,0},
			{0,1,1,1},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // L2 Pos 2 block
			{0,1,1,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0}, // L2 Pos 3 block
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0}, // L2 Pos 4 block
			{0,0,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
	{0,0,255,255} // color azul
	},
	{
		{
			{0,1,0,0}, // S Pos 1 block
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // S Pos 2 block
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0}, // S Pos 3 block
			{0,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // S Pos 4 block
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
	{0,255,0,255} // color verde
	},
	{
		{
			{0,0,1,0}, // Z Pos 1 block
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{1,1,0,0}, // Z Pos 2 block
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,1,0}, // Z Pos 3 block
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{1,1,0,0}, // Z Pos 4 block
			{0,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
	{255,0,0,255} // color rojo
	},
	{
		{
			{0,1,0,0}, // perpendicular pos 1 block
			{1,1,1,0},
			{0,0,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0}, // perpendicular pos 2 block
			{0,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0}, // perpendicular pos 3 block
			{1,1,1,0},
			{0,1,0,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0}, // perpendicular pos 4 block
			{1,1,0,0},
			{0,1,0,0},
			{0,0,0,0}
		},
	{100,0,255,255} // color morado
	}
};


tetraminos actual;

int asignarPuntaje(int filaCompleta, int puntaje)
{
	puntaje += filaCompleta * filaCompleta * 10;
	return puntaje;
}

int next[4][4];
SDL_Rect impPreview[4][4];


/*
void imprimirPiezas(SDL_Rect** ptr_matrizImp, int aux[4][4], coorColor tetraColor)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (aux[i][j] == 1)
			{
				//dest.x =
				ptr_matrizImp[i][j].x += j * 45 - 45;
				ptr_matrizImp[i][j].y += i * 45 - 45;
				SDL_SetRenderDrawColor(rend, tetraColor.r, tetraColor.g, tetraColor.b, tetraColor.a);
				SDL_RenderFillRect(rend, &ptr_matrizImp[i][j]);
			}
		}
	}
}
*/

int contRot = 0;
void rotation(tetraminos auxNext, int aux[4][4])
{
	if (contRot == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				aux[i][j] = auxNext.pos2[i][j];
				printf("%d ", aux[i][j]);
			}
			printf("\n");
		}
	}
	else if (contRot == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				aux[i][j] = auxNext.pos3[i][j];
				printf("%d ", aux[i][j]);
			}
			printf("\n");
		}
	}
	else if (contRot == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				aux[i][j] = auxNext.pos4[i][j];
				printf("%d ", aux[i][j]);
			}
			printf("\n");
		}
	}
	else if (contRot == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				aux[i][j] = auxNext.pos1[i][j];
				printf("%d ", aux[i][j]);
			}
			printf("\n");
		}
	}
}
/*
void mostrarScore(SDL_Surface* textSurf, TTF_Font* font, SDL_Texture* textTexture, SDL_Rect textRect, unsigned char score, SDL_Color colorText) {
	textSurf = TTF_RenderText_Solid(font, &score, colorText);
	textTexture = SDL_CreateTextureFromSurface(rend, textSurf);
	SDL_QueryTexture(textTexture, NULL, NULL, textRect.w, textRect.h);
	textRect.x = 985;
	textRect.y = 240;
	SDL_FreeSurface(textSurf);
}
*/
void movement(SDL_Event ev, SDL_Rect* dest, tetraminos auxNext, int aux[4][4])
{

	switch (ev.key.keysym.scancode)
	{
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
		contRot++;
		rotation(auxNext, aux);
		if (contRot == 4) contRot = 0;
		break;
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		dest->x -= 45;
		break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		dest->y += 45;
		break;
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		dest->x += 45;
		break;
	default:
		break;
	}
}

/*
void limits(SDL_Rect* dest)
{
	// limit derecho
	if (dest->x + dest->w > 865)
	{
		dest->x = 865 - dest->w;
	}
	//limit izquierdo
	if (dest->x < 415)
	{
		dest->x = 415;
	}
	//limit abajo
	if (dest->y + dest->h > 720)
	{
		dest->y = 720 - dest->h;
	}
	//limit arriba
	if (dest->y < -100)
	{
		dest->y = -100;
	}
}
*/
/*
void cambiarPuntaje(char* puntaje)
{
	if (puntaje + 1 == ':')
	{

	}
}
*/

bool filaCompleta(int gridNum[20][10], int fila)
{
	for (int j = 0; j < 10; j++)
	{
		if (gridNum[fila][j] == 0)
		{
			return 0;
		}
	}
	return 1;
}

void vaciarFila(int gridNum[20][10], int fila)
{
	for (int j = 0; j < 10; j++)
	{
		gridNum[fila][j] = 0;

	}
}

void moverFilas(int gridNum[20][10], int fila, int limpiar)
{
	for (int j = 0; j < 10; j++)
	{
		gridNum[fila + limpiar][j] = gridNum[fila][j];
		gridNum[fila][j] = 0;
	}
}
/*
void reset(SDL_Rect* dest, tetraminos actual, coorColor* tetraColor, int aux[4][4], tetraminos* auxNext, coorColor* auxNextColor, int* next[4][4])
{
	dest->x = 595;
	dest->y = -45;
	printf("\n\n\n\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			aux[i][j] = next[i][j];
			printf("%d ", aux[i][j]);
		}
		printf("\n");
	}
	*auxNextColor = *tetraColor;

}

*/
SDL_Rect** matrizImp;
void crearMatrizImprimir()
{
	matrizImp = malloc(4 * sizeof(SDL_Rect*)); // Reserva memoria para filas
	for (int i = 0; i < 4; i++)
	{
		matrizImp[i] = malloc(4 * sizeof(SDL_Rect)); // Reserva memoria para columnas
	}
}

void liberarMatrizImprimir(SDL_Rect** matrizImp)
{
	for (int i = 0; i < 4; i++)
	{
		free(matrizImp[i]);
	}
	free(matrizImp);
}

SDL_Rect** matrizPantalla;
void crearMatrizPantalla()
{
	matrizPantalla = malloc(20 * sizeof(SDL_Rect*));
	for (int i = 0; i < 20; i++)
	{
		matrizPantalla[i] = malloc(10 * sizeof(SDL_Rect));

	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			matrizPantalla[i][j].x = 415 + j * 45;
			matrizPantalla[i][j].y = -180 + i * 45;
			matrizPantalla[i][j].w = 45;
			matrizPantalla[i][j].h = 45;
		}
	}
}
void liberarMatrizPantalla(SDL_Rect** matrizPantalla)
{
	for (int i = 0; i < 20; i++)
	{
		free(matrizPantalla[i]);
	}
	free(matrizPantalla);
}

SDL_Rect** matrizGrid;
void crearMatrizGrid()
{
	matrizGrid = malloc(20 * sizeof(SDL_Rect*));

	for (int i = 0; i < 20; i++)
	{
		matrizGrid[i] = malloc(12 * sizeof(SDL_Rect));
	}

	//limite inferiores
	for (int i = 0; i < 11; i++)
	{
		matrizGrid[15][i].x = 370 + i * 45;
		matrizGrid[15][i].y = 675; //720-45
		matrizGrid[15][i].w = 45;
		matrizGrid[15][i].h = 45;
	}

	//limite izquierdo
	for (int i = 0; i < 20; i++)
	{
		matrizGrid[i][0].x = 370; //415-45
		matrizGrid[i][0].y = -180 + i * 45;
		matrizGrid[i][0].w = 45;
		matrizGrid[i][0].h = 45;
	}

	//limite derecho
	for (int i = 0; i < 20; i++)
	{
		matrizGrid[i][11].x = 865;
		matrizGrid[i][11].y = -180 + i * 45;
		matrizGrid[i][11].w = 45;
		matrizGrid[i][11].h = 45;
	}
}

void liberarMatrizGrid(SDL_Rect** matrizGrid)
{
	for (int i = 0; i < 16; i++)
	{
		free(matrizGrid[i]);
	}
	free(matrizGrid);
}

SDL_Rect** matrizInterseccion;
void crearMatrizInterseccion()
{
	matrizInterseccion = malloc(20 * sizeof(SDL_Rect*));
	for (int i = 0; i < 20; i++)
	{
		matrizInterseccion[i] = malloc(10 * sizeof(SDL_Rect));

	}
}

void liberarMatrizInterseccion(SDL_Rect** matrizInterseccion)
{
	for (int i = 0; i < 20; i++)
	{
		free(matrizInterseccion[i]);
	}
	free(matrizInterseccion);
}