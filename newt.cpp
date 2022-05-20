#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MP 100
int karr[MP];

double f(double a, double b, double c, int k) {
  return a * k * k + b * k + c;
}

int main() {
  int minK, maxK;
  double j = 0;
  
  for(int i = 0; i < MP; i++) {
    j += (rand() / (double) RAND_MAX) * 100;
    int rj = round(j);

    if (i == 0)
      minK = rj;
    else
      if (i == MP - 1)
	maxK = rj;
    
    karr[i] = rj;
    //    printf("%d, %d\n", karr[i], i);
  }

  // exit(0);
  
  // Approximate the first MP points....
  
  double a1, b1, c1;
  
  for(int n = 3; n < MP; n++) {

    a1 = b1 = c1 = 0;

    double epsilon = 2;
  
    double MSE = 1E10;
  
    double H[3][3];
    double Hinv[3][3];
    double gradient[3];
    double direction[3];
    
    while(MSE > epsilon) {

      // Compute Hessian

      H[0][0] = 0;
      for(int i = 0; i < n; i++) {
	H[0][0] += 2 * (pow(karr[i], 4));      
      }
      H[0][1] = 0;
      for(int i = 0; i < n; i++) {
	H[0][1] += 2 * (pow(karr[i], 3));      
      }
      H[0][2] = 0;
      for(int i = 0; i < n; i++) {
	H[0][2] += 2 * (pow(karr[i], 2));      
      }
      H[1][0] = 0;
      for(int i = 0; i < n; i++) {
	H[1][0] += 2 * (pow(karr[i], 3));      
      }
      H[1][1] = 0;
      for(int i = 0; i < n; i++) {
	H[1][1] += 2 * (pow(karr[i], 2));      
      }
      H[1][2] = 0;
      for(int i = 0; i < n; i++) {
	H[1][2] += 2 * (pow(karr[i], 1));      
      }
      H[2][0] = 0;
      for(int i = 0; i < n; i++) {
	H[2][0] += 2 * (pow(karr[i], 2));      
      }
      H[2][1] = 0;
      for(int i = 0; i < n; i++) {
	H[2][1] += 2 * (pow(karr[i], 1));      
      }
      H[2][2] = 0;
      for(int i = 0; i < n; i++) {
	H[2][2] += 2 * n;      
      }
    
      // Invert Hessian

      double det = H[0][0] * (H[1][1] * H[2][2] - H[2][1] * H[1][2]) -
	H[0][1] * (H[1][0] * H[2][2] - H[1][2] * H[2][0]) +
	H[0][2] * (H[1][0] * H[2][1] - H[1][1] * H[2][0]);

      double invdet = 1 / det;

      Hinv[0][0] = (H[1][1] * H[2][2] - H[2][1] * H[1][2]) * invdet;
      Hinv[0][1] = (H[0][2] * H[2][1] - H[0][1] * H[2][2]) * invdet;
      Hinv[0][2] = (H[0][1] * H[1][2] - H[0][2] * H[1][1]) * invdet;
      Hinv[1][0] = (H[1][2] * H[2][0] - H[1][0] * H[2][2]) * invdet;
      Hinv[1][1] = (H[0][0] * H[2][2] - H[0][2] * H[2][0]) * invdet;
      Hinv[1][2] = (H[1][0] * H[0][2] - H[0][0] * H[1][2]) * invdet;
      Hinv[2][0] = (H[1][0] * H[2][1] - H[2][0] * H[1][1]) * invdet;
      Hinv[2][1] = (H[2][0] * H[0][1] - H[0][0] * H[2][1]) * invdet;
      Hinv[2][2] = (H[0][0] * H[1][1] - H[1][0] * H[0][1]) * invdet;

      for(int i = 0; i < 3; i++)
	gradient[i] = 0;
    
      for(int i = 0; i < n; i++) {
	gradient[0] += 2 * ( f(a1, b1, c1, karr[i]) - i) * pow(karr[i], 2);
	gradient[1] += 2 * ( f(a1, b1, c1, karr[i]) - i) * karr[i];
	gradient[2] += 2 * ( f(a1, b1, c1, karr[i]) - i);
      }
    
      for(int i = 0; i < 3; i++) {
	direction[i] = 0;
	for(int j = 0; j < 3; j++) {
	  direction[i] += Hinv[i][j] * gradient[j];
	}
      }

      a1 -= direction[0];
      b1 -= direction[1];
      c1 -= direction[2];
    
      double nMSE = 0;
      for(int i = 0; i < n; i++) {
	nMSE += ( f(a1, b1, c1, karr[i]) - i ) * ( f(a1, b1, c1, karr[i]) - i ) / n;
      }
      printf("ITERATION %d: nMSE = %lf\n", n, nMSE);

      if (MSE > nMSE)
	MSE = nMSE;
    }

    // print n points
    for(int i = 0; i < n; i++) {
      printf("x = %d, y = %d, f = %lf \n", karr[i], i, f(a1, b1, c1, karr[i]));
    }
    //    printf("a1 = %lf, b1 = %lf, c1 = %lf \n", a1, b1, c1);
  }

  // Search random keys
  for (int i = 0; i < MP; i++) {
    int k;
    if ( i % 2 )
      k = round(rand() / (double) RAND_MAX * (maxK - minK)) + minK;
    else
      k = karr[i];
    
    int apos = round(f(a1, b1, c1, k));
    printf("k = %d, apos = %d, karr[apos] = %d\n", k, apos, karr[apos]);

    if (apos < 0) apos = 0;
    if (apos >= MP) apos = MP - 1;
    
    int j = 0;
    int end = 0;
    while ((karr[apos + j] != k) && (!end)) {
      j++;
      if (apos + j >= MP) end = 1;
      if (karr[apos + j] > k) end = 1;
    }
    if (!end)
      printf("Found key in karr, k = %d, pos = %d, offset = %d\n",  k, apos + j, j);
    else {
      int j = 0;
      int end = 0;
      while ((karr[apos - j] != k) && (!end)) {
	j++;
	if (apos - j <= -1) end = 1;
	if (karr[apos - j] < k) end = 1;
      }
      if (!end)
	printf("Found key in karr, k = %d, pos = %d, offset = %d\n",  k, apos - j, - j);
      else
	printf("Not found key in karr, k = %d\n",  k);
    }
  }
  
  return 0;
}

