#include <stdio.h>
#include <stdlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <math.h>


// Compile using :
// gcc main.c -o main $(pkg-config --cflags --libs cairo); ./main ; evince essai.pdf&


typedef struct {float x; float y;} point;
typedef struct {float r; float g; float b;} rgb;


// Makes the curve with all the points of a list
// Very slow because of the stroke at every iteration
// TODO : find a faster way to make the color fading towards red, that wouldn't have to stroke every time

void trace_liste(cairo_t *cr, point pts[], int size)
{
  int i;
  rgb color = {0., 0., 1.};
  cairo_move_to(cr, pts->x, pts->y);
  for (i=1; i<size; i++)
    {
      cairo_line_to(cr, pts[i].x, pts[i].y);
      cairo_stroke(cr);
      cairo_move_to(cr, pts[i].x, pts[i].y);
      cairo_set_source_rgb(cr, i*(1./size), 0, 1.-(i*(1./size)));
      if (!(i%1000))
	printf("%dème itération\n", i);
    }
}






// Makes the first step of the construction

void step1(point pts[], point origine, float length)
{
  float l = length/4;
  pts[0].x = origine.x + l;
  pts[0].y = origine.y + 3*l;
  pts[1].x = origine.x + l;
  pts[1].y = origine.y + l;
  pts[2].x = origine.x + 3*l;
  pts[2].y = origine.y + l;
  pts[3].x = origine.x + 3*l;
  pts[3].y = origine.y + 3*l;
}



// Functions to get the corners of a set of points

point get_dl_corner(point pts[], int size)
{
  point result=pts[0];
  int i;
  for (i=1; i<size; i++)
    result = result.x <= pts[i].x && result.y >= pts[i].y ? pts[i] : result;
  return result;
}

point get_dr_corner(point pts[], int size)
{
  point result=pts[0];
  int i;
  for (i=1; i<size; i++)
    result = result.x >= pts[i].x && result.y >= pts[i].y ? pts[i] : result;
  return result;
}

point get_ul_corner(point pts[], int size)
{
  point result=pts[0];
  int i;
  for (i=1; i<size; i++)
    result = result.x <= pts[i].x && result.y <= pts[i].y ? pts[i] : result;
  return result;
}

point get_ur_corner(point pts[], int size)
{
  point result=pts[0];
  int i;
  for (i=1; i<size; i++)
    result = result.x >= pts[i].x && result.y <= pts[i].y ? pts[i] : result;
  return result;
}



// Functions to make points of a set rotate

void diag_sym_right(point pts[], int size)
{
  point ul = get_ul_corner(pts, size);
  int i;
  point tmp;
  for (i=0; i<size; i++)
    {
      tmp = pts[i];
      pts[i].x = tmp.y - ul.y + ul.x;
      pts[i].y = tmp.x - ul.x + ul.y;
    }
}

void diag_sym_left(point pts[], int size)
{
  point dl = get_dl_corner(pts, size);
  int i;
  point tmp;
  for (i=0; i<size; i++)
    {
      tmp = pts[i];
      pts[i].x = dl.x + dl.y - tmp.y;
      pts[i].y = dl.x + dl.y - tmp.x;
    }
}


int power(int x, int n)
{
  int i, result = 1;
  for (i=0; i<n; i++)
    result *= x;
  return result;
}


void print_point_list(point pts[], int size)
{
  int i;
  for (i=0; i<size; i++)
    printf("%d. x: %f\ty: %f\n", i, pts[i].x, pts[i].y);
}



// Main function

void hilbert_aux(point pts[], int n, float size, point origine);

void hilbert(cairo_t* cr, int n, float size, point origine)
{
  point* pts = NULL;
  pts = (point*) malloc(power(4,n+1)*sizeof(point));
  if (pts != NULL)
    {
      hilbert_aux(pts, n, size, origine);
      trace_liste(cr, pts, power(4,n+1));
    }
  else
    {
      printf("Pas possible de créer une liste de %d éléments\n", power(4,n+1));
    }
  //print_point_list(pts, power(4,n+1)); // for debug
}

void hilbert_aux(point pts[], int n, float size, point origine)
{
  if (n==0)
    step1(pts, origine ,size);
  else
    {
      point o1 = {origine.x + size/2, origine.y},
	    o2 = {origine.x, origine.y + size/2},
	    o3 = {origine.x + size/2, origine.y + size/2};
      hilbert_aux(pts, n-1, size/2, o2);
      hilbert_aux(&pts[power(4,n)], n-1, size/2, origine);
      hilbert_aux(&pts[2*power(4, n)], n-1, size/2, o1);
      hilbert_aux(&pts[3*power(4, n)], n-1, size/2, o3);
      diag_sym_left(pts, power(4, n));
      diag_sym_right(&pts[3*power(4, n)], power(4, n));
    }
}




int main(int argc, char* args[])
{
  // Setting up cairo stuff
  cairo_surface_t *surface;
  cairo_t *cr;

  surface = cairo_pdf_surface_create("essai.pdf", 1500, 1500);
  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0, 0, 1.0);
  cairo_set_line_width(cr, 3);


  // Hilbert ftw
  point origine = {0,0};
  hilbert(cr, 5, 1500, origine);
  
  // Closes files properly and all
  cairo_surface_destroy(surface);
  cairo_destroy(cr);

  printf("It all went fine\n");
  return 0;
}
