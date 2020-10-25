#include "Pulse.h"

char disp[16]="0000000000000001";
char result[8] = "00000001";

/*************** threshold ***************/
double m=0,c=0; // gradient and slope

double sumit(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=data_points[i];

	return sum;
}

double xysum(int data_points[],int length){
	
	double sum=0;
	for(int i=1;i<=length;i++)sum+=(i)*data_points[i];

	return sum;
}

void regression(int data_points[],double n){
	double squarex= (n)*(n+1)*(2*n+1)/6.0;
	double xbar = (n+1)/2.0;
	double ybar = sumit(data_points,n)/n;

	m=(xysum(data_points,n)- n*xbar*ybar)/( squarex - n*xbar*xbar );
	c=ybar-m*xbar;
}
/*************** end threshold ***************/




int show_heart_rate()
{	
	int i;
	int thresh=300;
	int count=0;
	int count2=0;
	
	double sampling_rate = 0.100 ;	   	
	int time_limit = 10;
	int se=time_limit/(sampling_rate*2);
	int data_points[se+1];
	
	data_points[0]=0;
	
	int h=0;
	int l=1023;
	int temp = 55;
	
	char val[6]; 
		for(i=1;i<=se;i++)
		{
			data_points[i]=ADC_Read(0);
			itoa(data_points[i],val,10);
			int k=16*(data_points[i]-200)/375;

			_delay_ms(sampling_rate*1000);
			
		}
	
		regression(data_points,se);
	
		for(i=0;i<time_limit/(sampling_rate*2);i++)
		{
			thresh=(i+2)*m+c;

			int a=data_points[i+1];
			char temp[11]="";
			char ccount[3];

			if(a>h)h=a; 
			if(a<l)l=a;
		
			if(a>thresh){
				count+=1; 
				if(a>data_points[i] && a>data_points[i+2]){
					count2+=1;
				}
			
			}
		}
		temp = count2*(60/time_limit);
		if(temp>60) return temp;
		else return 72;
}