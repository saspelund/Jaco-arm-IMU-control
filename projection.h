#include <map>
#include <vector>
#include <cmath>
#include <iostream>
#include "include/gnuplot-iostream.h"
#include <stdlib.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <chrono>

//these values get to be played with as the graphs resizes according to its limits
#define X_GRAPH_MIN 0.08
#define X_GRAPH_MAX 0.75
#define Y_GRAPH_MIN 0.08
#define Y_GRAPH_MAX 0.635

#define X_ROBOT_MIN 0.08
#define X_ROBOT_MAX 0.60
#define Y_ROBOT_MIN 0.20
#define Y_ROBOT_MAX 0.62


void graph ( Gnuplot &gp, double Xcurrent, double Ycurrent, double & Xgoal, double & Ygoal, double Xdelta, double Ydelta)

{
	
	if(pow(double ((Xcurrent-Xgoal)*(Xcurrent-Xgoal) + (Ycurrent-Ygoal)*(Ycurrent-Ygoal)), (0.5))<=0.025 )
    {
		do{
			Xgoal = X_ROBOT_MIN+ ( rand() % (int)( (X_ROBOT_MAX - X_ROBOT_MIN - 0.02)*100.0 ) )/100.0;
			Ygoal = Y_ROBOT_MIN+ ( rand() % (int)( (Y_ROBOT_MAX - Y_ROBOT_MIN - 0.02)*100.0 ) )/100.0;
		}while( (Xgoal*Xgoal + Ygoal*Ygoal) > 0.75); //make sure the goal isn't out of reach
		
		
		printf("Xgoal,Ygoal: %.4f,%.4f\n",Xgoal,Ygoal);
	}
	
	std::vector<std::pair<double, double> > goalPoint;
	goalPoint.push_back(std::make_pair(Xgoal, Ygoal));
	
	std::vector<std::pair<double, double> > currentPoint;
	currentPoint.push_back(std::make_pair(Xcurrent, Ycurrent));
	
	std::vector<boost::tuple<double, double, double, double> > directionVector;
	directionVector.push_back(boost::make_tuple(Xcurrent, Ycurrent, Xdelta, Ydelta));
	
	gp << "set xrange [" << X_GRAPH_MIN << ":" << X_GRAPH_MAX << "]\nset yrange [" << Y_GRAPH_MIN << ":" << Y_GRAPH_MAX << "]\n";
	gp << "set size ratio -1" << std::endl;
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	
	gp << "plot" << gp.file1d(goalPoint) << " with points pointtype 7 pointsize 14 notitle," << gp.file1d(currentPoint) 
	<< " with points" << " pointtype 7 pointsize 7 notitle, '-' with vectors ls 4 lw 5 notitle" << std::endl;
	
	gp.send1d(directionVector);
	
	return;
}


void Data_Write( std::string filename, float begin, float T, double x, double y, double X, double Y, double A, double B) 

{

  std::ofstream myfile ( filename , std::ios::app);
  if (myfile.is_open())
  {
    myfile << begin << "\t" << T << "\t" << x << "\t" << y << "\t" << x-A << "\t" << y-B << "\t" << X << "\t" << Y << "\t"<< "\n";
    myfile.close();
  }
  else std::cout << "Unable to open file";
  return ;
  
}


void Time_Stamp(std::string filename)

{

	using std::chrono::system_clock;
	std::chrono::duration<int,std::ratio<60*60*24> > one_day (1);
	system_clock::time_point today = system_clock::now();
	std::time_t tt;
	tt = system_clock::to_time_t ( today );

	std::ofstream myfile ( filename , std::ios::app);
  		
	if (myfile.is_open())
  		
  		{
  		
    		myfile << "Time Stamp (EST): " << ctime(&tt);
    		myfile <<"RunTime\tPT Time\tx pos\ty pos\tVx\tVy\tGoal X\tGoal Y" << "\n";
    		myfile.close();
    		
  		}
  
  	else std::cout << "Unable to open file";
  
}

