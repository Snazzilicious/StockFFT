/*This program can either do a Discrete fourier transform or an inverse
discrete transform. It reads in data from files and then writes to
the screen. This program also makes use of the complex data type of std
C++ for carrying out the transforms.

Author: Ian Holloway
4/10/2015
C++
*/


#define USE_MATH_DEFINES


#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>


using namespace std;


int runDFT() {
	
	//this vector contains complex variable data types
	//for storing frequency output from DFT or input
	//for IFT
	vector<complex<double>> freq;
	vector<double> data;
	vector<complex<double>> inv;
	
	int N;
	bool done=false;
	bool inverse=false;
	
	ifstream read;
	
	
	ofstream write;
	
	
	string input;
	double temp;
	complex<double> grab;
	
	complex<double> exponent;
	complex<double> sum (0.0,0.0);
	
	//Asks user for DFT or IFT
	cout << "1 for Forward \n2 for Inverse" << endl;
	getline(cin, input, '\n');
	//opens the appropiate input file based on input
	if(input == "2"){
		inverse = true;
		read.open("output.txt");
	}
	else{
		read.open("input.txt");
	}
	
	//the first piece is the DFT
	if(!inverse){
		
		//this loop gets input data from the file
		//and stores in a vector
		while (!read.eof()){
			//read >> temp;
			getline(read, input, '\n');
			
				stringstream(input) >> temp;
				//cout << temp << endl;
				data.push_back(temp);
				//data.shrink_to_fit();
			
			
		}
		//gets number of data points
		N = data.size();
		read.close();
	
	
	
		//here is the actual DFT. Each frequency is 
		//multiplied by each data point and the values
		//are summed up
		for (int n=0; n<N; n++) {
			sum = complex<double> (0.0,0.0);
			
		
			for (int m=0; m<N; m++){
		
				exponent = complex<double>(0.0, -(2*M_PI*m*n)/N);
				sum += data[m]*exp(exponent);
		
		
			}
	
			freq.push_back(sum);
			//freq.shrink_to_fit();
		}
		//writes output to screen and a file
		write.open("output.txt");
		cout << "n\t" << "w\t" << "g(w)" << endl;
		for (int i=0; i<freq.size(); i++){
			write << freq[i] << '\n';
			cout << i << "\t" << 2*i << "PI\t" << real(freq[i]) << "\t" << imag(freq[i]) << "i" << endl;
		
		}
	
	
	
	}
	//this else contains the inverse transform
	else{/*
		for (int l=0; l<8; l++){
			getline(read, input, '\n');
			stringstream(input) >> grab;
			cout << grab << '\t' << read.eof() << endl;
		}*/
	
		//this loop reads in data from the file of
		//frequency data to a vector.
		while (!done){
			//read >> temp;
			getline(read, input, '\n');
			if (!read.eof()){
				stringstream(input) >> grab;
				//cout << temp << endl;
				freq.push_back(grab);
				//freq.shrink_to_fit();
			
			}
			else {
				done = true;
			}
		}
		//gets number of data points
		N=freq.size();
		read.close();
		
		//runs the inverse transform
		for (int n=0; n<N; n++) {
			sum = complex<double> (0.0,0.0);
			
		
			for (int m=0; m<N; m++){
		
				exponent = complex<double>(0.0, (2*M_PI*m*n)/N);
				sum += freq[m]*exp(exponent);
		
		
			}
			sum = sum*(1.0/N);
			inv.push_back(sum);

		}
		
		
		//writes to the screen
		for (int j=0; j<inv.size(); j++){
			
			cout << j << "\t" << 0.125*j << "s\t" << real(inv[j]) << "\t" << imag(inv[j]) << "i" << endl;
			
		}
	
	}
	
	
	write.close();
	return 0;
}
