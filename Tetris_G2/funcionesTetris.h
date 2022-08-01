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

bool flagDestLeft = 0;
bool flagDestRight = 0;
bool flagDestDown = 0;
bool flagUp = 0;
SDL_Renderer* rend;

SDL_Rect rectangle;

typedef struct
{
	char nombre[3];
	int puntaje;
}Usuario;

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


tetraminos preview;

int asignarPuntaje(int filaCompleta, int puntaje)
{
	puntaje += filaCompleta * filaCompleta * 10;
	return puntaje;
}

int next[4][4];
SDL_Rect impPreview[4][4];


int contRot = 0;
void rotation(tetraminos actual, int matrizConversion[4][4])
{
	if (contRot == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrizConversion[i][j] = actual.pos2[i][j];
			}
		}
	}
	else if (contRot == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrizConversion[i][j] = actual.pos3[i][j];
			}
		}
	}
	else if (contRot == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrizConversion[i][j] = actual.pos4[i][j];
			}
		}
	}
	else if (contRot == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				matrizConversion[i][j] = actual.pos1[i][j];
			}
		}
	}
}

void movement(SDL_Event ev, SDL_Rect* mov, tetraminos actual, int matrizConversion[4][4])
{

	switch (ev.key.keysym.scancode)
	{
	case SDL_SCANCODE_W:
	case SDL_SCANCODE_UP:
		contRot++;
		if (flagUp == 0)rotation(actual, matrizConversion);
		if (contRot == 4) contRot = 0;
		break;
	case SDL_SCANCODE_A:
	case SDL_SCANCODE_LEFT:
		if (flagDestLeft == 0) mov->x -= 45;
		break;
	case SDL_SCANCODE_S:
	case SDL_SCANCODE_DOWN:
		if (flagDestDown == 0) mov->y += 45;
		break;
	case SDL_SCANCODE_D:
	case SDL_SCANCODE_RIGHT:
		if (flagDestRight == 0) mov->x += 45;
		break;
	default:
		break;
	}
}

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