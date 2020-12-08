/******************************************************************************
 * Autor: Rodrigo Mendes Leme                     Numero USP: 3151151         *
 * Curso: computacao                              Data: 9/12/2000             *
 * Professor: Sadao Isotani                                                   *
 * Exercicio programa: grupo 2 (otimizacao)                                   *
 * Compilador usado: GCC-Linux                                                *
 * Descricao: desenha na tela uma reta interpolada, obtida pelo metodo de mi- *
 *            nimos quadrados via pontos gerados aleatoriamente.              *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xwc.h"

#define MAX 1000             // Numero maximo de pontos que podem ser gerados
#define TAM_X_REAL 30        // Intervalo do eixo x mapeado pelo programa
#define TAM_Y_REAL 30        // Intervalo do eixo y mapeado pelo programa
#define TAM_X_TELA 600       // Tamanho da janela na direcao horizontal
#define TAM_Y_TELA 600       // Tamanho da janela na direcao vertical

typedef struct               // Define um ponto do eixo cartesiano
        {
          double x,
                 y;
        } PONTO;

WINDOW *janela;              // Janela na qual o programa trabalhara

// Funcao: ajuste_x
// Entrada: a coordenada real x a ser convertida para coordenadas da janela.
// Descricao: converte coordenadas reais x em coordenadas da janela.

int ajuste_x(double x)
{
  return (int) rint((x + TAM_X_REAL / 2) * TAM_X_TELA / TAM_X_REAL);
}

// Funcao: ajuste_y
// Entrada: a coordenada real y a ser convertida para coordenadas da janela.
// Descricao: converte coordenadas reais y em coordenadas da janela.

int ajuste_y(double y)
{
  if (y <= TAM_Y_REAL / 2)
    return (int) abs(rint((y - TAM_Y_REAL / 2) * TAM_Y_TELA / TAM_Y_REAL));
  else
    return (int) rint((-y + TAM_Y_REAL / 2) * TAM_Y_TELA / TAM_Y_REAL);
}

// Funcao: desenha_segmento
// Entrada: os pontos extremos do segmento e sua cor.
// Descricao: desenha um segmento de reta na janela.

void desenha_segmento(double x1, double y1, double x2, double y2, int cor)
{
  WLine(janela,ajuste_x(x1),ajuste_y(y1),ajuste_x(x2),ajuste_y(y2),cor);
}

// Funcao: desenha_reta
// Entrada: os coeficientes angular e linear de uma reta e sua cor.
// Descricao: desenha uma reta na janela.

void desenha_reta(double a, double b, int cor)
{
  desenha_segmento(0,b,TAM_X_REAL / 2,a * (TAM_X_REAL / 2) + b,cor);
  desenha_segmento(-TAM_X_REAL / 2,a * (-TAM_X_REAL / 2) + b,0,b,cor);
}

// Funcao: desenha_eixos
// Descricao: desenha na janela os eixos cartesianos com uma graduacao.

void desenha_eixos(void)
{
  int i;

  desenha_segmento(-TAM_X_REAL / 2,0,TAM_X_REAL / 2,0,0x00FF00);
  desenha_segmento(0,-TAM_Y_REAL / 2,0,TAM_Y_REAL / 2,0x00FF00);
  for (i = -TAM_X_REAL / 2; i <= TAM_X_REAL / 2; i++)
  {
    desenha_segmento(-0.2,i,0.2,i,0x00FF00);
    desenha_segmento(i,-0.2,i,0.2,0x00FF00);
  }
}

// Funcao: desenha_pontos
// Entrada: um conjunto de pontos e a quantidade dos mesmos.
// Descricao: desenha na janela um conjunto de pontos.

void desenha_pontos(PONTO pontos[MAX], int n)
{
  int i;

  for (i = 0; i <= n - 1; i++)
    WPlot(janela,ajuste_x(pontos[i].x),ajuste_y(pontos[i].y),0xFFFFFF);
}

// Funcao: gera_pontos
// Saida: um conjunto de pontos e a quantidade dos mesmos.
// Descricao: a partir de coeficientes angular e linear dados, gera uma reta e
//            pontos proximos a esta.

void gera_pontos(PONTO pontos[MAX], int *n)
{
  const double DELTA = 3;         // Intervalo de preenchimento dos
                                  // pontos em relacao a reta gerada
  double a,           // Coeficiente angular da reta gerada
         b,           // Coeficiente linear da reta gerada
         delta_x;     // Intervalo de preenchimento dos pontos no eixo x
  int i;

  printf("Digite o coeficiente angular da reta a ser gerada: ");
  scanf("%lf",&a); 
  printf("Digite o coeficiente linear da reta a ser gerada: ");
  scanf("%lf",&b);
  printf("Reta gerada: y = %gx + %g\n",a,b);
  do
  {
    printf("Digite o numero de pontos a serem gerados: ");
    scanf("%d",n);
    if (*n > 1000)
      printf("Erro: nao pode haver mais de %d pontos.\n",MAX);
  } while (*n > 1000);

  delta_x = (double) TAM_X_REAL / *n;
  if (abs(a) <= 1)                      // A reta esta mais "proxima" do eixo x
    for (i = 0; i <= *n - 1; i++)
    {
      pontos[i].x = -(TAM_X_REAL / 2) + i * delta_x;
      pontos[i].y = a * pontos[i].x + b;
      if (rand() < RAND_MAX / 2)    // Decide se o ponto ficara acima ou abaixo
        pontos[i].y -= (rand() / ((double) RAND_MAX + 1)) * DELTA;   // da reta
      else
        pontos[i].y += (rand() / ((double) RAND_MAX + 1)) * DELTA;
    }
  else                                  // A reta esta mais "proxima" do eixo y
    for (i = 0; i <= *n - 1; i++)
    {
      pontos[i].x = -(TAM_X_REAL / 2) + i * delta_x;
      pontos[i].y = a * pontos[i].x + b;
      if (rand() < RAND_MAX / 2)    // Decide se o ponto ficara a esquerda ou a
        pontos[i].x -= (rand() / ((double) RAND_MAX + 1)) * DELTA;   // direita
      else                                                           // da reta
        pontos[i].x += (rand() / ((double) RAND_MAX + 1)) * DELTA;
    }
}

// Funcao: quad_min
// Entrada: um conjunto de pontos e a quantidade dos mesmos.
// Saida: os coeficientes angular (a) e linear (b) de uma reta.
// Descricao: calcula os coeficientes angular e linear de uma reta, atraves do
//            metodo de minimos quadrados.

void quad_min(double *a, double *b, PONTO pontos[MAX], int n)
{
  double soma_x_med = 0,
         soma_y_med = 0,
         soma_xy    = 0,
         soma_xx    = 0,
         x_med,               // Media das coordenadas x dos pontos
         y_med;               // Media das coordenadas y dos pontos
  int i;

  for (i = 0; i <= n - 1; i++)
  {
    soma_x_med += pontos[i].x;
    soma_y_med += pontos[i].y;
    soma_xy    += pontos[i].x * pontos[i].y;
    soma_xx    += pontos[i].x * pontos[i].x;
  }
  x_med = soma_x_med / n;
  y_med = soma_y_med / n;
  *a    = (soma_xy - n * x_med * y_med) / (soma_xx - n * x_med * x_med);
  *b    = y_med - *a * x_med;
}

// Funcao: main
// Descricao: inicializa o modo grafico, cria uma janela e chama as rotinas de
//            criacao de pontos e obtencao e desenho da reta interpolada.

void main(void)
{
  PONTO pontos[MAX];     // Conjunto de pontos gerados
  double a,              // Coeficiente angular da reta interpolada
         b;              // Coeficiente linear da reta interpolada
  int n;                 // Numero de pontos gerados

  janela = InitGraph(TAM_X_TELA,TAM_Y_TELA,"EP de FAP 137 - Otimização");
  WShow(janela);
  desenha_eixos();
  gera_pontos(pontos,&n);
  desenha_pontos(pontos,n);
  quad_min(&a,&b,pontos,n);
  desenha_reta(a,b,0xFFFFFF);
  printf("Mensagem: pressione uma tecla para terminar.");
  getchar();
  getchar();
  CloseGraph();
}
