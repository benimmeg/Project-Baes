#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// global variabeles

double reduced_m = 0.03;
double time_step = 0.01;
double time_begin = 0.;
double time_end = 500.;

// vector class

class Vec {

	double _x;
	double _y;
	
public:

	Vec() {_x=0 ; _y = 0; }
	Vec(double x, double y) {_x = x; _y = y; }
	
	double x() const { return _x; }
	double y() const { return _y; }
	
	double norm() const {return sqrt(_x*_x + _y*_y);}
	
	double norm2() const {return _x*_x+ _y*_y;}
	
	double norm3() const {return norm2()*norm();}
	
	Vec& operator+=(Vec v) {
		_x += v._x;
		_y += v._y;
		return *this;
	}
	
	Vec& operator-=(Vec v) {
		_x -= v._x;
		_y -= v._y;
		return *this;
	}
	
	Vec& operator*=(double s) {
		_x *= s;
		_y *= s;
		return *this;
	}
	
	Vec& operator/=(double s) {
		_x /= s;
		_y /= s;
		return *this;
	}
	
};

// additional vector operations

Vec operator+(Vec a, Vec b) {return a += b;}

Vec operator-(Vec a, Vec b) {return a -= b;}

// additional scalair vector operations

Vec operator*(Vec a, double s) {return a *= s;}

Vec operator*(double s, Vec a) {return a *= s;}

Vec operator/(Vec a, double s) {return a /= s;}

// vector acceleration

Vec a (Vec r) {return -1. * reduced_m * r/(r.norm3());}

// energy

double E (Vec v, Vec r) {return 0.5 * v.norm2() - reduced_m/r.norm();}

// interpolation

Vec interpolation_mean(Vec a, Vec b) {return 0.5*(a+b);}

Vec interpolation_taylor(Vec r_12, Vec rn ,Vec vn) 
	{return r_12 + 0.5*time_step*vn + 0.25*time_step*time_step*a(rn) + 0.125 * time_step*time_step*a(r_12);}

// starting conditions

const Vec r_begin( 1. , 0.2);
const Vec v_begin( 0.05 , 0.06);
const double energy_begin = E(v_begin,r_begin);

// leap frog integrator
Vec leapfrog_r(Vec r, Vec v)
{
	return r +  time_step*v;}
	
Vec leapfrog_v(Vec v, Vec a)
{
	return v +  time_step*a;}


int main()
{	
	cout << "Writing to file 'oef6_solution.txt'..." << endl;
	ofstream outfile_solution("oef6_solution.txt");
    outfile_solution << setprecision(12);
    
    outfile_solution << "reduced_m" << ' ' << "time_step" << '\n';
    
    outfile_solution << reduced_m << ' ' << time_step << '\n';
    
    outfile_solution << "time" << ' ' << "rn.x" << ' ' << "rn.y" << ' ' << "vn.x" << ' ' 
					<< "vn.y" << ' ' << "E_n" << ' ' << "En_error" << '\n';
					
	outfile_solution << 0.0 << ' ' << r_begin.x() << ' ' << r_begin.y() << ' ' << v_begin.x() << ' ' << v_begin.y() << ' ' <<	
					energy_begin << ' ' << 0.0 << '\n';
    
	Vec r_12 = r_begin + 0.5 * time_step * v_begin + time_step*time_step * a(r_begin)/8;
	Vec vn = v_begin;
	for (double t = time_begin; t <= time_end; t += time_step)
	{
		Vec vn_next = leapfrog_v(vn, a(r_12));
		Vec r_12_next = leapfrog_r(r_12, vn_next);
		Vec rn = interpolation_mean(r_12,r_12_next);
		//Vec rn = interpolation_taylor(r_12,r_12_next);
		double energy_n = E(vn,rn);
		//Vec v_12 = interpolation(vn,vn_next);
		//double energy_12 = E(v_12,r_12);
		double energy_n_error = abs(energy_begin-energy_n)/abs(energy_begin);
		//cout << energy_n_error << endl;
		vn = vn_next;
		r_12 = r_12_next;
		
		outfile_solution << t+time_step << ' ' << rn.x() << ' ' << rn.y() << ' ' << vn.x() << ' ' << vn.y() << ' ' <<	
					energy_n << ' ' << energy_n_error << '\n';
		}
		
	outfile_solution.close();
	cout << "Done writing." << endl;
	
	return 0;
}

