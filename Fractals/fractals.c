//HW #10 - Jack Roberts
//December 8, 2024
//This homework creates fractal visualizations via recursive functions. 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gfx.h"

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 );
void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3);
void shrink_squares (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4); 
void drawSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
void spiralFractal(int initX, int initY, float a, float thetaMax, float thetaStep);
void drawSquareSpiral(int x, int y, int size);
void circleFractal(int x, int y, int r);
void snowFractal(int x1, int y1, int x2, int y2, float dist);
void treeFractal(int x1, int y1, int x2, int y2, float dist, float theta1, float theta2);
void fernFractal (int x, int y, float length, float theta); 
void spiralFractal2(int x, int y, float radius, float theta);


int main()
{
  char c;
  int width = 700, height = 700, mrgn = 20;

  gfx_open(width, height, "Fractals!");

  while(1) {
		c = gfx_wait();
		gfx_clear();

		if (c == '1') { //Calls the function to draw sierpinski triangles
    		sierpinski(mrgn, mrgn, width-mrgn, mrgn, width/2, height-mrgn);
		}
		else if (c == '2') { //Calls the function to draw shrinking squares
			int initSize = width/4;
			shrink_squares(initSize, initSize, width-initSize, initSize, width-initSize, height-initSize, initSize, height-initSize);
  		}
		else if (c =='3') { //Calls the function to draw a spiral of squares
			spiralFractal(width/2, height/2, 1.3, 7*M_PI, 0.75);
				}
		else if (c == '4') { //Calls the circles fractal function
			circleFractal(width/2, height/2, height/3);
		}
		else if (c == '5') { //Calls the snowflake fractals function
			float dist = height/2-5*mrgn;
			for (int i = 0; i < 5; i++) { //For loop iterates 5 separate times for 5 separate mini-fractals
        		float angle =(M_PI/2)+ i * (2 * M_PI / 5); //Angle is increased by 2pi/5 every iteration
        		int x2 = width/2+ dist * cos(angle); //New x2 and y2 values are calculated every time based on the updated angle
        		int y2 = height/2 + dist * sin(angle);
        		snowFractal(width/2, height/2, x2, y2, dist / 3);
      	}
		}
		else if (c == '6') { //Calls the function to create the tree fractal
			treeFractal(width/2, height-mrgn, width/2, height-mrgn-150, 150, 2.0944, 1.0472); //2.0944 corresponds to 120 degrees, and 1.0472 is 60 degrees.
		}
		else if (c == '7') { //Calls the fern fractal function
			fernFractal(width/2, height-mrgn, 400, 0);
		}
		else if (c == '8') { //Calls the spiral of spiral fractal function
			spiralFractal2(width/2, height/2, width, 0);
		}
    	else if (c == 'q') break; //Exits the program if q is pushed
  	}
	
	return 0;
}

void sierpinski( int x1, int y1, int x2, int y2, int x3, int y3 ) { //Function was given to us
  // Base case. 
  if( abs(x2-x1) < 5) return;

  // Draw a triangle
  drawTriangle( x1, y1, x2, y2, x3, y3 ); //Draws a triangel on the screen

  // Recursive calls: creates three triangles for every initial triangle
  sierpinski( x1, y1, (x1+x2)/2, (y1+y2)/2, (x1+x3)/2, (y1+y3)/2 );
  sierpinski( (x1+x2)/2, (y1+y2)/2, x2, y2, (x2+x3)/2, (y2+y3)/2 );
  sierpinski( (x1+x3)/2, (y1+y3)/2, (x2+x3)/2, (y2+y3)/2, x3, y3 );
}

void drawTriangle( int x1, int y1, int x2, int y2, int x3, int y3) { //Uses gfx_line() to draw a triangle
  gfx_line(x1,y1,x2,y2);
  gfx_line(x2,y2,x3,y3);
  gfx_line(x3,y3,x1,y1);
}

void drawSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) { //Uses gfx_line() to draw a square
	gfx_line(x1, y1, x2, y2);
	gfx_line(x2, y2, x3, y3);
	gfx_line(x3, y3, x4, y4);
	gfx_line(x4, y4, x1, y1);

}

void shrink_squares(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    if (abs(x2-x1) < 3) return;
    drawSquare(x1, y1, x2, y2, x3, y3, x4, y4);	//Draws a square with all the updated coordinates

	 int dist2 = (x2/2-x1/2)/2; //Distance from the center of the larger square to the edge of the smaller one

	 //Makes 4 calls to the function to create 4 separate squares for every initial square created
    shrink_squares(x1-dist2, y1-dist2, x1+dist2, y1-dist2, x1+dist2, y1+dist2, x1-dist2, y1+dist2);
    shrink_squares(x2-dist2, y2-dist2, x2+dist2, y2-dist2, x2+dist2, y2+dist2, x2-dist2, y2+dist2);
    shrink_squares(x3-dist2, y3-dist2, x3+dist2, y3-dist2, x3+dist2, y3+dist2, x3-dist2, y3+dist2);
    shrink_squares(x4-dist2, y4-dist2, x4+dist2, y4-dist2, x4+dist2, y4+dist2, x4-dist2, y4+dist2);
}

void spiralFractal(int initX, int initY, float a, float thetaMax, float thetaStep) {

	float theta = 0;
	float size = 10;
	float r = 0;

	while (theta < thetaMax) { //Uses a while loop to expand the size of the spiral iteratively
		r = r + a*theta;

		//Calculates the updated x and values for the spiral using polar coordinates. This method was found online. 
		int x = initX + r*cos(-theta); 
		int y = initY + r*sin(-theta);

		drawSquareSpiral(x, y, size); //Draws a square at each calculated point

		size = size * 1.15; //Increases the size of the square by 115% through each iteration

		theta += thetaStep; //Increases the angle theta through each iteration, allowing for a spiral shape
	}
}

void drawSquareSpiral(int x, int y, int size) { 
	float increase = size/6; //Increases the size of the square each time

	//Uses gfx_line() to draw four lines (creating a square)
	gfx_line(x-increase, y-increase, x+increase, y-increase);
	gfx_line(x+increase, y-increase, x+increase, y+increase);
	gfx_line(x+increase, y+increase, x-increase, y+increase);
	gfx_line(x-increase, y+increase, x-increase, y-increase);

}


void circleFractal(int x, int y, int r) {
	gfx_circle(x, y, r); 

	if (r < 1) return;

	//Calls itself 6 times using polar coordinates to draw 6 circles. Increases theta by PI_3 each time to make equally spaced circles.
	circleFractal(x+r, y, r/3);
	circleFractal(x+r*cos(M_PI/3), y + r*sin(M_PI/3), r/3);
	circleFractal(x+r*cos(2*M_PI/3), y + r*sin(2*M_PI/3), r/3);
	circleFractal(x+r*cos(3*M_PI/3), y + r*sin(3*M_PI/3), r/3);
	circleFractal(x+r*cos(4*M_PI/3), y + r*sin(4*M_PI/3), r/3);
	circleFractal(x+r*cos(5*M_PI/3), y + r*sin(5*M_PI/3), r/3);
}

void snowFractal(int x1, int y1, int x2, int y2, float dist) {

	 if (dist <= 1) return;

	 gfx_line(x1, y1, x2, y2);
	
	 //Calls the snow fractal function 5 times. The new x2 and y2 coordinates are updated within the function calls, and the distance is divided by three through each iteration. 
	 snowFractal(x2, y2, x2+dist*cos(2*M_PI/5+M_PI/2), y2+dist*sin(2*M_PI/5+M_PI/2), dist/3);
    snowFractal(x2, y2, x2+dist*cos(4*M_PI/5+M_PI/2), y2+dist*sin(4*M_PI/5+M_PI/2), dist/3);
    snowFractal(x2, y2, x2+dist*cos(6*M_PI/5+M_PI/2), y2+dist*sin(6*M_PI/5+M_PI/2), dist/3);
    snowFractal(x2, y2, x2+dist*cos(8*M_PI/5+M_PI/2), y2+dist*sin(8*M_PI/5+M_PI/2), dist/3);
    snowFractal(x2, y2, x2+dist*cos(10*M_PI/5+M_PI/2), y2+dist*sin(10*M_PI/5+M_PI/2), dist/3);
}

void treeFractal(int x1, int y1, int x2, int y2, float dist, float theta1, float theta2) {

	if (dist <= 1) return;

	gfx_line(x1, y1, x2, y2);

	//Calls itself twice. The new x2 and y2 coordinates are calculated within the function calls with a theta that increases by pi/4 through each iteration. 
	treeFractal(x2, y2, x2 - dist*cos(theta1), y2 - dist*sin(theta1), 2*dist/3, theta1+M_PI/4, theta2+M_PI/4);
	treeFractal(x2, y2, x2 - dist*cos(theta2), y2 - dist*sin(theta2), 2*dist/3, theta1-M_PI/4, theta2-M_PI/4);


}

void fernFractal (int x, int y, float length, float theta) {

	if (length <= 2) return;

	int x2 = x + length*sin(theta);
	int y2 = y - length*cos(theta);

	gfx_line(x, y, x2, y2);

	//Calls the function 8 times, creating 8 different line segments through each iteration. The initial x and y values are updated within each function call, the theta values are increased by pi/4, and the length is divided by 3 through each iteration. 
	fernFractal(x2, y2, length/3, theta + M_PI/4);
	fernFractal(x2, y2, length/3, theta - M_PI/4);
	fernFractal(x2 - (3*length/4)*sin(theta), y2 + (3*length/4)*cos(theta), length/3, theta + M_PI/4);
	fernFractal(x2 - (3*length/4)*sin(theta), y2 + (3*length/4)*cos(theta), length/3, theta - M_PI/4);
	fernFractal(x2 - (2*length/4)*sin(theta), y2 + (2*length/4)*cos(theta), length/3, theta + M_PI/4);
	fernFractal(x2 - (2*length/4)*sin(theta), y2 + (2*length/4)*cos(theta), length/3, theta - M_PI/4);
	fernFractal(x2 - (length/4)*sin(theta), y2 + (length/4)*cos(theta), length/3, theta + M_PI/4);
	fernFractal(x2 - (length/4)*sin(theta), y2 + (length/4)*cos(theta), length/3, theta - M_PI/4);

}

void spiralFractal2(int x, int y, float radius, float  angle){
	if (radius < 2) return;

	//Uses polar coordinates to calculate the x2 and y2 values. 
	int x2 = x + radius*cos(angle);
	int y2 = y + radius*sin(angle);

	gfx_point(x2, y2); //The spirals are based on points. 


	//Calls itself twice. The first call creates the smaller spiral at (x2, y2), continuing along the same direction as the larger spiral. The second call creates a smaller spiral starting from the current center of the spiral. 
	spiralFractal2(x2, y2, radius*0.25, angle);
	spiralFractal2(x, y, radius*0.95, angle + 0.5*(M_PI/4));
}





