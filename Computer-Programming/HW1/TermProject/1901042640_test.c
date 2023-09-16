#include "x.c"


int main ()
{

Figure *fig = start_figure(2000.0,1000.0);
Color c;
c.red=0;
c.blue=50;
c.green=0;

set_thickness_resolution(fig,20,0.5);
set_color(fig,c);




//	POLYLINE
Point2D * poly_line = (Point2D *) calloc (5, sizeof (Point2D));
poly_line[0].x = 0; 
poly_line[0].y = 0;
poly_line[1].x = 5; 
poly_line[1].y = 0; 
poly_line[2].x = 9; 
poly_line[2].y = 7; 
poly_line[3].x = 3; 
poly_line[3].y = 12;
poly_line[4].x = -10; 
poly_line[4].y = 13; 
draw_polyline (fig, poly_line, 5);
export_eps(fig, "poly_line.eps");

// POLYGON
Point2D * polygon = (Point2D *) calloc (4, sizeof (Point2D));
polygon[0].x = -10; 
polygon[0].y = 0;
polygon[0].jump = 1;
polygon[1].x = 7; 
polygon[1].y = 0; 
polygon[1].jump = 1;
polygon[2].x = 13; 
polygon[2].y = 9;
polygon[2].jump = 1;
polygon[3].x = -1; 
polygon[3].y = 14;
polygon[3].jump = 1;
polygon[4].jump = 0;
draw_polygon (fig, polygon);
export_eps(fig, "polygon.eps");

// FX (sin)

draw_fx (fig, g, -30.0, 50.0, 1.0);
export_eps (fig, "fx.eps");

// SCALE
double scale_x = 2, scale_y = 4;
scale_figure (fig, scale_x, scale_y);
export_eps (fig, "scale.eps");

// RESIZE

Point2D * start_roi, * end_roi;
start_roi-> x = 40;
start_roi-> y = 40;
end_roi-> x = 400;
end_roi-> y = 400;
resize_figure (fig, start_roi, end_roi);
export_eps (fig, "resize.eps");

// ELLIPSE
Point2D * centre, * width_height;
centre = (Point2D*) malloc (sizeof (Point2D));
width_height = (Point2D*) malloc (sizeof (Point2D));
centre-> x = 0.0;
centre-> y = 5.0;
width_height-> x = 5.0;
width_height-> y = 3.0;
draw_ellipse (ellipse, centre, width_height,fig, centre->x - width_height->x, centre->x + width_height->x,1);
export_eps (fig, "ellipse.eps");

return 0;
}