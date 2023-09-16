#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point2D
{
double x,y;
struct Point2D *nextp;
int jump;
}Point2D;

typedef struct
{
 double red,green,blue; 
}Color;

typedef struct
{
 double width, height,
 resolution, 
 thickness,
 llx,lly; 
Color color;
 Point2D *point;
}Figure;


Figure * start_figure (double width, double height)
{
	Figure *fig = (Figure*) malloc (sizeof (Figure));
	fig->width=width;
	fig->height=height;
 	return fig;
}

void set_thickness_resolution (Figure *fig, double resolution, double thickness)
{
 fig->resolution=resolution;
 fig->thickness=thickness;
}

void set_color (Figure * fig, Color c)
{
 fig->color.red=c.red;
 fig->color.green=c.green;
 fig->color.blue=c.blue;
}

void draw_fx(Figure * fig, double f(double x), double start_x, double end_x, double step_size)
{
int i;	
Point2D * headp;
headp = (Point2D *) malloc (sizeof (Point2D));

fig->point-> x = start_x;
fig->point-> y = f (start_x);
headp = fig->point;

	for (i = start_x; i <= end_x; i += step_size)
	{
		headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
		headp = headp->nextp;
		headp-> x = i;
		headp-> y = f (i);
		headp->nextp = NULL;	
	}

}

double g (double x)
{
	return sin (x);
}

double ellipse (double x, double a, double b)
{
	return sqrt((1-((x*x)/(a*a)))*(b*b));
}

double circle (double x, double r)
{
	return sqrt (r * r - x * x);
}

void draw_ellipse (double f (double x, double a, double b), Point2D * centre, Point2D * width_height, Figure * fig, double start_x, double end_x, double step_size)
{
	int i;
	double a = width_height-> x;
	double b = width_height-> y;
	Point2D * headp;
	headp = (Point2D *) malloc (sizeof (Point2D));
	fig->point-> x = start_x;
	fig->point-> y = centre->y + f (start_x, a, b);
	headp = fig->point;

		for (i = start_x; i <= end_x; i += step_size)
		{
		headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
		headp = headp->nextp;
		headp-> x = i;
		headp-> y = centre->y + f (i, a, b);
		headp->nextp = NULL;	
		}
			for (i = end_x; i >= start_x; i -= step_size)
			{
			headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
			headp = headp->nextp;
			headp-> x = i;
			headp-> y = centre-> y - f (i, a, b);
			headp->nextp = NULL;	
			}
}

void export_eps (Figure * fig, char * file_name)
{
	Point2D *tmp;
 	FILE *ptr;
 	ptr = fopen (("%s", file_name),"w");

 	fprintf(ptr,"%%!PS-Adobe-3.0 EPSF-3.0");
	fprintf(ptr,"\n%%%%BoundingBox: %d %d %d %d",(int)fig->llx,(int)fig->llx,(int)fig->width,(int)fig->height);
	fprintf(ptr,"\n%d setlinewidth",(int)fig->thickness);
	fprintf(ptr,"\n%.1f %.1f %.1f setrgbcolor",fig->color.red,fig->color.green,fig->color.blue);
 	fprintf(ptr,"\n%.1f %.1f moveto", 20 * fig->point->x+fig->width/2.0, 20 * fig->point->y+fig->height/2.0);

 		for(tmp=fig->point->nextp; tmp!=NULL; tmp=tmp->nextp)
 		{
			fprintf(ptr,"\n%.1f %.1f lineto", 20 * tmp->x+fig->width/2.0, 20 * tmp->y+fig->height/2.0);
 		}
 
 	fprintf(ptr,"\nstroke\n");
 	fclose(ptr);	

}	

void draw_polyline(Figure * fig, Point2D * poly_line, int n)
{
	int i;
	Point2D * headp;
	headp = (Point2D *) malloc (sizeof (Point2D));
	fig->point-> x = poly_line-> x;
	fig->point-> y = poly_line-> y;
	headp = fig-> point;

		for (i = 0; i < n; i++)
		{
			headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
			headp = headp->nextp;
			headp->x = poly_line[i].x;
			headp->y = poly_line[i].y;
		}

}

void draw_polygon(Figure * fig, Point2D * poly_line)
{
	int i;
	int start_x, start_y;
	Point2D * headp;
	headp = (Point2D *) malloc (sizeof (Point2D));
	start_x = poly_line-> x;
	start_y = poly_line-> y;
	fig->point-> x = poly_line-> x;
	fig->point-> y = poly_line-> y;
	headp = fig-> point;

		while (poly_line[i].jump != 0)
		{
			headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
			headp = headp->nextp;
			headp->x = poly_line[i].x;
			headp->y = poly_line[i].y;
			i++;
		}
	headp->nextp = (Point2D *) malloc (sizeof (Point2D));	
	headp = headp->nextp;
	headp->x = start_x;	
	headp->y = start_y;
}

void scale_figure (Figure *fig, double scale_x, double scale_y)
{
	 Point2D *cursor = fig->point;
 		while(cursor != NULL)
 		{
 			cursor-> x*= scale_x;
 			cursor-> y*= scale_y;
 			cursor = cursor-> nextp;
	 	}
}

void resize_figure(Figure *fig, Point2D *start_roi, Point2D *end_roi)
{
 	fig -> llx=start_roi->x;
	fig -> lly=start_roi->y;
	fig -> width=end_roi->x;
 	fig -> height=end_roi->y;
}
