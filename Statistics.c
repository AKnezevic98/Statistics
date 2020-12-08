#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "pbPlots.h"
#include "supportLib.h"

/* 
This program does statistical analysis on data you input, either manually, through .txt file or through .bin file

It can do Single variable statistics, multiple results statistics and linear regression.

The program is highly optimized for both speed and memory, especially when reading .bin files.
*/

//datastructure for results statistics and linear regression
typedef struct datastructure{ 
	double data1;
	double data2;
}dataset;

//functions for finding mimimum and maximum of an array
double min(double a[], unsigned long int length){
	double minimum = a[0];
	unsigned long int i=0;
	for(i=0; i<length; i++){
		if(a[i]<minimum){
			minimum = a[i];
		}
	}
	
	return minimum;
}

double max(double a[], unsigned long int length){
	double maximum = a[0];
	unsigned long int i=0;
	for(i=0; i<length; i++){
		if(a[i]>maximum){
			maximum = a[i];
		}
	}
	
	return maximum;
}


unsigned char main(){
	//Initial user input, defines what the user wants the program to do
	unsigned char num = 1;
	START:
	printf("What do you need?\n1-Single variable stat.\n2-Results stat.\n3-Linear regression\n0-exit\n");
	scanf("%hhu", &num);
	fflush(stdin);
	
	switch(num){
		
		//exit
		case 0:{
			return 0;
		}
		
		//Single variable statistics
		case 1:{
			//initial definitions of important values
			unsigned long int i=0, k=0;
			double x_avg=0, m=0, M=0, R=0, x_med=0, del_x=0;
			double sum = 0;
			double* data = (double*)malloc(0);
			
			//sorting algorithm for quicksort
			signed char sortiranje(double* a, double* b)
			{
				if(*a<*b) return -1;
				else if(*a>*b) return 1;
				else return 0;
			}
			
			//Data reading definition
			printf("Do you want to read data from a file?\n1-Yes .txt file\n2-Yes .bin file\n0-No\n");
			scanf("%hhu", &num);
			fflush(stdin);
			
			switch(num){
				//Manual input
				case 0:{
					i==1;
					
					printf("Insert sample vector: (you end by inserting non-number)\n");
					data = (double*)realloc(data, sizeof(double));
					printf("x[%lu]=", k+1);
					while(scanf(" %lf", &data[k])==1)
					{
						fflush(stdin);
						k++; i=0;
						data = (double*)realloc(data, sizeof(double)*(k+1));
						printf("x[%lu]=", k+1);
					}
					fflush(stdin);
					data = (double*)realloc(data, sizeof(double)*k);
	
					if(i==1){
						printf("Input ERROR!\n");
						exit(10);
					}
					break;
				}
				
				//.txt file
				case 1:{
					char filename[64];
					char test;
					printf("Please enter file name: (excluding extension)\nFile must be organized in a column of floating point numbers!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(110);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "txt"); 
					FILE* f = fopen(filename, "r");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(111);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(112);
					}
					
					//get number of lines in file
					k = 0;
					do{
						test= getc(f);
						if(test == '\n'){
							k++;
							num = 1;
						}else if(test!=EOF){
							num = 0;
						}
					}while(test!=EOF);
					fseek(f, 0, SEEK_SET);
					if(num == 0){
						k++;
					}
					
					//read data
					data = (double*)realloc(data, k*sizeof(double));
					for(i=0; i<k-1; i++){
						fscanf(f, "%lf\n", &data[i]);
					}
					if(num == 0){
						fscanf(f, "%lf", &data[k-1]);
					} else{
						fscanf(f, "%lf\n", &data[k-1]);
					}
					fclose(f);
					break;
				}
				
				//.bin file
				case 2:{
					char filename[64];
					printf("Please enter file name: (excluding extension)\nFile must be organized in long doubles!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(120);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "bin");
					FILE* f = fopen(filename, "rb");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(121);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(122);
					}
	
					//read data
					k = i/sizeof(double);
					data = (double*)realloc(data, k*sizeof(double));
					fread(data, sizeof(double), k, f);
					fclose(f);
					break;
				}
				
				
				default:{
					printf("ERROR: Invalid input!");
					exit(13);
				}
			}
			
			qsort((void*)data, k, sizeof(double), (int (*)(const void*, const void*))sortiranje);

			printf("Median is: median(x)=");
			if(k%2==0) x_med = (data[k/2-1]+data[k/2])/2;
			else x_med = data[k/2];
			printf("%le\n", x_med);
			
			sum = 0;
			printf("Sample mean is: avg(x)=");
			for(i=0; i<k; i++)
			{
				sum += data[i];
			}
			x_avg = sum/k;
			printf("%le\n", x_avg);
			
			sum = 0;
			del_x = fabs(data[0]-x_avg);
			printf("Standard deviation is: m=");
			for(i=0; i<k; i++)
			{
				sum += (data[i]-x_avg)*(data[i]-x_avg);
				if(del_x*del_x < (data[i]-x_avg)*(data[i]-x_avg)) del_x = fabs(data[i]-x_avg);
			}
			m = sqrt(sum/(k-1));
			printf("%e\n", m);
			
			printf("Greatest deviation from mean is: delta_x=%le\n", del_x);
			
			printf("Variance is: M=");
			M = m/sqrt(k);
			printf("%le\n", M);
			
			printf("Result: x = %le +- %le\n", x_avg, M);
			
			printf("Coefficient of variation is: R=");
			R = M/x_avg;
			printf("%lf%%\n", R*100);
			
			free(data);
			break;
		}
		
		//Statistics of results
		case 2:{
			long int i=0, k=0;
			double x_avg=0, M_avg=0, R=0;
			double sum=0;
			dataset* data = (dataset*)malloc(0);
			
			printf("Do you want to read data from a file?\n1-Yes .txt file\n2-Yes .bin file\n0-No\n");
			scanf("%hhu", &num);
			fflush(stdin);
			
			switch(num){
				//Manual input
				case 0:{
					i==1;
					
					printf("Insert sample vector: (you end by inserting non-number)\nFormat: x, Mx\n");
					data = (dataset*)realloc(data, sizeof(dataset));
					printf("x[%lu], M[%lu] = ", k+1, k+1);
					while(scanf("%lf, %lf", &data[k].data1, &data[k].data2)==2)
					{
						fflush(stdin);
						k++; i=0;
						data = (dataset*)realloc(data, sizeof(dataset)*(k+1));
						printf("x[%lu], M[%lu] = ", k+1, k+1);
					}
					fflush(stdin);
					data = (dataset*)realloc(data, sizeof(dataset)*k);
	
					if(i==1){
						printf("Input ERROR!\n");
						exit(20);
					}
					break;
				}
				
				//.txt file
				case 1:{
					char filename[64];
					char test;
					printf("Please enter file name: (excluding extension)\nFile must be organized in a column of x, Mx!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(210);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "txt"); 
					FILE* f = fopen(filename, "r");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(211);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(212);
					}
					
					//get number of lines in file
					k = 0;
					do{
						test= getc(f);
						if(test == '\n'){
							k++;
							num = 1;
						}else if(test!=EOF){
							num = 0;
						}
					}while(test!=EOF);
					fseek(f, 0, SEEK_SET);
					if(num == 0){
						k++;
					}
					
					//read data
					data = (dataset*)realloc(data, k*sizeof(dataset));
					for(i=0; i<k-1; i++){
						fscanf(f, "%lf, %lf\n", &data[i].data1, &data[i].data2);
					}
					if(num == 0){
						fscanf(f, "%lf, %lf", &data[k-1].data1, &data[k-1].data2);
					} else{
						fscanf(f, "%lf, %lf\n", &data[k-1].data1, &data[k-1].data2);
					}
					fclose(f);
					break;
				}
				
				//.bin file
				case 2:{
					char filename[64];
					printf("Please enter file name: (excluding extension)\nFile must be organized in 2 long doubles!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(220);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "bin");
					FILE* f = fopen(filename, "rb");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(221);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(222);
					}
	
					//read data
					k = i/sizeof(dataset);
					data = (dataset*)realloc(data, k*sizeof(dataset));
					fread(data, sizeof(dataset), k, f);
					fclose(f);
					break;
				}
				
				
				default:{
					printf("ERROR: Invalid input!");
					exit(23);
				}
			}
			
			printf("General variance is: avg(M)="); sum = 0;
			for(i=0; i<k; i++) sum += 1/(data[i].data2*data[i].data2);
			M_avg = sqrt(1/sum);
			printf("%le\n", M_avg);
			
			printf("General mean is: avg(m)="); sum = 0;
			for(i=0; i<k; i++) sum += data[i].data1/(data[i].data2*data[i].data2);
			x_avg = sum*M_avg*M_avg;
			printf("%le\n", x_avg);
			
			printf("General result: x = %le +- %le\n", x_avg, M_avg);
			
			R = M_avg/x_avg;
			printf("General coefficient of variation is: %lf%%\n", R*100);
			
			free(data);
			break;
		}
		
		//Linear regression
		case 3:{
			unsigned char num2 = 0;
			long int i=0, k=0, del=0, difi=0;
			double sumx=0, sumy=0, sumxy=0, sumx2=0, sumy2=0;
			double a, b, Ma, Mb, R;
			double sum=0, diff=0;
			dataset* data = (dataset*)malloc(0);
			dataset* removed = (dataset*)malloc(0);
			double* r = (double*)malloc(0);
			
			printf("Do you want to read data from a file?\n1-Yes .txt file\n2-Yes .bin file\n0-No\n");
			scanf("%hhu", &num);
			fflush(stdin);
			
			switch(num){
				//Manual input
				case 0:{
					i==1;
					
					printf("Insert sample vector: (you end by inserting non-number)\nFormat: x, y\n");
					data = (dataset*)realloc(data, sizeof(dataset));
					printf("x[%lu], y[%lu]=", k+1, k+1);
					while(scanf("%lf, %lf", &data[k].data1, &data[k].data2)==2)
					{
						fflush(stdin);
						k++; i=0;
						data = (dataset*)realloc(data, sizeof(dataset)*(k+1));
						printf("x[%lu], y[%lu]=", k+1, k+1);
					}
					fflush(stdin);
					data = (dataset*)realloc(data, sizeof(dataset)*k);
	
					if(i==1){
						printf("Input ERROR!\n");
						exit(30);
					}
					break;
				}
				
				//.txt file
				case 1:{
					char filename[64];
					char test;
					printf("Please enter file name: (excluding extension)\nFile must be organized in a column of x, y!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(310);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "txt"); 
					FILE* f = fopen(filename, "r");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(311);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(312);
					}
					
					//get number of lines in file
					k = 0;
					do{
						test= getc(f);
						if(test == '\n'){
							k++;
							num = 1;
						}else if(test!=EOF){
							num = 0;
						}
					}while(test!=EOF);
					fseek(f, 0, SEEK_SET);
					if(num == 0){
						k++;
					}
					
					//read data
					data = (dataset*)realloc(data, k*sizeof(dataset));
					for(i=0; i<k-1; i++){
						fscanf(f, "%lf, %lf\n", &data[i].data1, &data[i].data2);
					}
					if(num == 0){
						fscanf(f, "%lf, %lf", &data[k-1].data1, &data[k-1].data2);
					} else{
						fscanf(f, "%lf, %lf\n", &data[k-1].data1, &data[k-1].data2);
					}
					fclose(f);
					break;
				}
				
				//.bin file
				case 2:{
					char filename[64];
					printf("Please enter file name: (excluding extension)\nFile must be organized in 2 long doubles!\n");
					fgets(filename, 60, stdin);
					fflush(stdin);
	
					if(strlen(filename)>60){
						printf("ERROR: Filename too long! Please keep file name shorter than 60 characters!\n");
						exit(320);
					}
					
					//removing newline from filename
					for(i=0; i<60; i++){
						if(filename[i]=='\n' || filename[i]=='\r'){
							filename[i] = '.';
							filename[i+1] = '\0';
							break;
						}
					}
	
					strcat(filename, "bin");
					FILE* f = fopen(filename, "rb");
	
					if(!f){
						printf("ERROR: Could not open %s file!", filename);
						exit(321);
					}
	
					fseek(f, 0, SEEK_END);
					i = ftell(f); fseek(f, 0, SEEK_SET);
	
					if(i==0){
						printf("ERROR: File %s is empty!", filename);
						exit(322);
					}
	
					//read data
					k = i/sizeof(dataset);
					data = (dataset*)realloc(data, k*sizeof(dataset));
					fread(data, sizeof(dataset), k, f);
					fclose(f);
					break;
				}
				
				
				default:{
					printf("ERROR: Invalid input!");
					exit(33);
				}
			}
			
			printf("What is proposed linear relationship?\n0 ... x - y\n1 ... ln x - y\n2 ... x - ln y\n3 ... ln x - ln y\n");
			scanf("%hhu", &num);
			fflush(stdin);
			
			//automatic linearization
			switch(num){
				// x - y
				case 0:{
					break;
				}
				
				// lnx - y
				case 1:{
					for(i=0; i<k; i++){
						data[i].data1 = log(data[i].data1);
					}
					break;
				}
				
				// x - lny
				case 2:{
					for(i=0; i<k; i++){
						data[i].data2 = log(data[i].data2);
					}
					break;
				}
				
				// lnx - lny
				case 3:{
					for(i=0; i<k; i++){
						data[i].data1 = log(data[i].data1);
						data[i].data2 = log(data[i].data2);
					}
					break;
				}
				
				
				default:{
					printf("ERROR: Invalid input!");
					exit(34);
				}
			}
			
			CALC:
			r = (double*)realloc(NULL, k*sizeof(double));
			sumx=0; sumy=0; sumxy=0; sumx2=0; sumy2=0;
			for(i=0; i<k; i++){
				sumx += data[i].data1;
				sumy += data[i].data2;
				sumxy += data[i].data1*data[i].data2;
				sumx2 += data[i].data1*data[i].data1;
				sumy2 += data[i].data2*data[i].data2;
			}
			a = (k*sumxy-sumx*sumy)/(k*sumx2-sumx*sumx);
			b = (sumy-a*sumx)/k;
			Ma = sqrt((k*sumy2-sumy*sumy-a*a*(k*sumx2-sumx*sumx))/((k-2)*(k*sumx2-sumx*sumx)));
			Mb = Ma*sqrt(sumx2/k);
			
			sum = 0;
			for(i=0; i<k; i++){
				r[i] = data[i].data2 - a*data[i].data1 - b;
				sum += r[i]*r[i];
			}
			R = sqrt(sum);
			
			sum = 0;
			for(i=0; i<k; i++){
				sum += data[i].data2*data[i].data2;
			}
			R /= sqrt(sum);
			
			if(del==0){
				printf("Do you want to skip automatic removal of bad points?\n1-Yes\n0-No\n");
				scanf("%hhu", &num2);
				fflush(stdin);
				if(num2!=1 && num2!=0){
					printf("ERROR: Invalid input!\n");
					exit(333);
				}
			}
			
			//Whether line is good enough aproximation (ie. is there a bad point)
			if(R>=(1-sqrt(0.9)) && num2==0){
				del++;
				removed = (dataset*)realloc(removed, sizeof(dataset)*del);
				diff = r[0]; difi = 0;
				for(i=1; i<k; i++){
					if(diff*diff<r[i]*r[i]){
						diff=r[i]; difi=i;
					}
				}
				removed[del-1].data1 = data[difi].data1;
				removed[del-1].data2 = data[difi].data2;
				
				//removal
				long long int temp=0;
				for(i=0; i<k; i++)
				{
					if(i!=difi){
						data[temp].data1 = data[i].data1;
						data[temp].data2 = data[i].data2;
						temp++;
					}
				}
				data[k-1].data1 = 0; data[k-1].data2 = 0;
				k--;
				data = (dataset*)realloc(data, sizeof(dataset)*k);
				goto CALC;
			}
			free(r);
			
			printf("Slope: a = %le +- %le\n", a, Ma);
			printf("Y-intercept: b = %le +- %le\n", b, Mb);
			printf("Slope-intercept form: y = %le x + %le\n", a, b);
			printf("Variance from line: r=%lf%%\n", R*100);
			
			printf("Points removed from calculations:");
			if(del==0) printf(" none\n");
			else{
				printf("\n");
				for(i=0; i<del; i++){
					printf("[%lf, %lf]\n", removed[i].data1, removed[i].data2);
				}
			}
			
			//Regrouping of data and removed data
			data = (dataset*)realloc(data, sizeof(dataset)*(k+del));
			for(i=k; i<k+del; i++){
				data[i].data1 = removed[i-k].data1;
				data[i].data2 = removed[i-k].data2;
			}
			k+=del;
			free(removed);
			
			
			//Plotting part
			double xmin, xmax, ymin, ymax, cmin, cmax, ymin2, ymax2, cmin2, cmax2;
			
			double xs[k];
			double ys[k];
			for(i=0; i<k; i++){
				xs[i] = data[i].data1;
				ys[i] = data[i].data2;
			}
			free(data);
			
			xmin = min(xs, k); xmax = max(xs, k);
			ymin = min(ys, k); ymax = max(ys, k);
			cmin = xmin - 0.1*(xmax-xmin); //canvas x min
			cmax = xmax + 0.1*(xmax-xmin); //canvas x max
			
			ymin2 = a*cmin + b;
			ymax2 = a*cmax + b;
			if(ymin<ymin2){
				cmin2 = ymin;
			}else{
				cmin2 = ymin2;
			}
			if(ymax>ymax2){
				cmax2 = ymax;
			}else{
				cmax2 = ymax2;
			}
			
			double xs2[] = {cmin, cmax};
			double ys2[] = {ymin2, ymax2};
			
		
			ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
			series->xs = xs;
			series->xsLength = sizeof(xs)/sizeof(double);
			series->ys = ys;
			series->ysLength = sizeof(ys)/sizeof(double);
			series->linearInterpolation = false;
			series->pointType = L"dots";
			series->pointTypeLength = wcslen(series->pointType);
			series->color = CreateRGBColor(1, 0, 0);
			
			ScatterPlotSeries *series2 = GetDefaultScatterPlotSeriesSettings();
			series2->xs = xs2;
			series2->xsLength = sizeof(xs2)/sizeof(double);
			series2->ys = ys2;
			series2->ysLength = sizeof(ys2)/sizeof(double);
			series2->linearInterpolation = true;
			series2->lineThickness = 1.0;
			series2->color = CreateRGBColor(0, 0, 1);
			
		
			ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
			settings->width = 1920;
			settings->height = 1080;
			settings->autoBoundaries = false;
			settings->xMin = cmin;
			settings->xMax = cmax;
			settings->yMin = cmin2;
			settings->yMax = cmax2;
			settings->autoPadding = true;
			
			//getting axis names
			switch(num){
				// x - y
				case 0:{
					settings->xLabel = L"Y";
					settings->xLabelLength = wcslen(settings->xLabel);
					settings->yLabel = L"X";
					settings->yLabelLength = wcslen(settings->yLabel);
					break;
				}
				
				// lnx - y
				case 1:{
					settings->xLabel = L"Y";
					settings->xLabelLength = wcslen(settings->xLabel);
					settings->yLabel = L"lnX";
					settings->yLabelLength = wcslen(settings->yLabel);
					break;
				}
				
				// x - lny
				case 2:{
					settings->xLabel = L"lnY";
					settings->xLabelLength = wcslen(settings->xLabel);
					settings->yLabel = L"X";
					settings->yLabelLength = wcslen(settings->yLabel);
					break;
				}
				
				// lnx - lny
				case 3:{
					settings->xLabel = L"lnY";
					settings->xLabelLength = wcslen(settings->xLabel);
					settings->yLabel = L"lnX";
					settings->yLabelLength = wcslen(settings->yLabel);
					break;
				}
			}
			settings->title = L"Linear regression y(x)";
			settings->titleLength = wcslen(settings->title);
			ScatterPlotSeries *s [] = {series, series2};
			settings->scatterPlotSeries = s;
			settings->scatterPlotSeriesLength = 2;
			free(series); free(series2);
		
			RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
			DrawScatterPlotFromSettings(canvasReference, settings);
			free(settings);
		
			size_t length;
			double *pngdata = ConvertToPNG(&length, canvasReference->image);
			WriteToFile(pngdata, length, "regression_plot.png");
			free(pngdata);
			
			DeleteImage(canvasReference->image);
			free(canvasReference);
			
			break;
		}
		

		default: {
			printf("ERROR: Invalid input!\n");
			exit(4);
		}
	}
	
	//whether to redo
	printf("Do you want to redo?\n1-Yes\n0-No\n");
	fflush(stdin);
	scanf("%hhu", &num);
	fflush(stdin);
	if(num==1) goto START;
	
	return 0;
}
