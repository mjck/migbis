//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

/*
*
* Mathematical and Computational Sciences Division
* National Institute of Technology,
* Gaithersburg, MD USA
*
*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain.  NIST assumes no responsibility whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*
*/



#ifndef STOPWATCH_H
#define STOPWATCH_H

// for clock() and CLOCKS_PER_SEC
#include <time.h>


namespace TNT
{

inline static double seconds(void)
{
    const double secs_per_tick = 1.0 / CLOCKS_PER_SEC;
    return ( (double) clock() ) * secs_per_tick;
}

/**
	Performance Stopwatch  used for benchmarking codes.
	Measure seconds elapsed.
*/
class Stopwatch {
    private:
        int running_;
        double start_time_;
        double total_;

    public:
        Stopwatch();
        inline void start();
        inline double stop();
		inline double read();
		inline void resume();
		inline int running();
};

/**
	Create an instance of a Stopwatch, with its own internal
		counter.
*/
Stopwatch::Stopwatch() : running_(0), start_time_(0.0), total_(0.0) {}

/**
	Start timing from 0.00.  
*/
void Stopwatch::start() 
{
	running_ = 1;
	total_ = 0.0;
	start_time_ = seconds();
}

/**
	
	Stop timing and return elapsed time (in seconds).
*/
double Stopwatch::stop()  
{
	if (running_) 
	{
         total_ += (seconds() - start_time_); 
         running_ = 0;
    }
    return total_; 
}

/**
	Resume timing, if currently stopped.  Operation
	has no effect if Stopwatch is already running_.
*/
inline void Stopwatch::resume()
{
	if (!running_)
	{
		start_time_ = seconds();
		running_ = 1;
	}
}
		

/**
	Read current time (in seconds).
*/
inline double Stopwatch::read()   
{
	if (running_)
	{
		stop();
		resume();
	}
	return total_;
}


}
#endif
    

            

