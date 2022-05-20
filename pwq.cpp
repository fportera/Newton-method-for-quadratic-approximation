#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NK 100
#define MP 10
int arr[NK * 10];
int karr[NK];

double f(double a, double b, double c, int i) {
  return a * karr[i] * karr[i] + b * karr[i] + c;
}

int main() {
  int j = 0;
  
  for(int i = 0; i < NK; i++) {
    j += round((rand() / (double) RAND_MAX) * 10);
    arr[j] = i;
    karr[i] = j;
    //    printf("%d\n", karr[i]);
  }

  // Approximate the first MP points....

  double a1, b1, c1;

  a1 = b1 = c1 = 0;

  double epsilon = 2;
  
  double MSE = 1E10;
  
  double lr = 0.00000001;

  while(MSE > epsilon) {
    
    for(int i = 0; i < MP; i++) {
      a1 -= lr * 2 * ( f(a1, b1, c1, i) - i) * (karr[i] * karr[i]);
      b1 -= lr * 2 * ( f(a1, b1, c1, i) - i) * (karr[i]);
      c1 -= lr * 2 * ( f(a1, b1, c1, i) - i);
    }
    
    double nMSE = 0;
    for(int i = 0; i < MP; i++) {
      nMSE += ( f(a1, b1, c1, i) - i ) * ( f(a1, b1, c1, i) - i );
    }
    printf("nMSE = %lf\n", nMSE);

    if (MSE > nMSE)
      MSE = nMSE;
  }

  // print MP points
  for(int i = 0; i < MP; i++) {
    printf("x = %d, y = %d, f = %lf \n", karr[i], i, f(a1, b1, c1, i));
  }
  printf("a1 = %lf, b1 = %lf, c1 = %lf \n", a1, b1, c1);
  
  return 0;
}

