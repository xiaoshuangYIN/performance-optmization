#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#define SIZE var // size in Bytes of the whole array(a + b)
#define NUM_ELEMENTS (SIZE/(2*sizeof(float)))// num of ele of each array 
#define NUM_TRAVERSALS (10*BYTES_PER_GB/(3*sizeof(float)*NUM_ELEMENTS))
#define BYTES_PER_KB 1024
#define BYTES_PER_MB (1024*1024LL)
#define BYTES_PER_GB (1024*1024*1024LL)
#define SZ 45
size_t var = BYTES_PER_KB;
float* a;
float* b;
size_t num_ele;

double bw_array[SZ];
double size_array[SZ];
size_t bw_index = 0;
size_t sa_index = 0;

double calc_time(struct timeval start, struct timeval end) {
  double start_sec = (double)start.tv_sec*1000000.0 + (double)start.tv_usec;
  double end_sec = (double)end.tv_sec*1000000.0 + (double)end.tv_usec;

  if (end_sec < start_sec) {
    return 0;
  } else {
    return end_sec - start_sec;
  }
}

void init_array() {
  num_ele = NUM_ELEMENTS;
  while (num_ele% 32 != 0){
    num_ele += 1;
  }
  a = malloc(1 * num_ele * sizeof(float));
  memset(a, 1, 1 *  num_ele * sizeof(float));
  b = malloc(1 *  num_ele* sizeof(float));
  memset(b, 1, 1 *  num_ele * sizeof(float));
}


double calc_bw(double bytes, double secs) {
  double mb = bytes / ((double) BYTES_PER_MB);
  return mb / secs;
}

void memory_loop(float* restrict a, float* restrict b) {
  size_t i;
  for (i=0; i <  num_ele / sizeof(float); i+=16) {
    b[i] += a[i];
    b[i+1] += a[i+1];
    b[i+2] += a[i+2];
    b[i+3] += a[i+3];
    b[i+4] += a[i+4];
    b[i+5] += a[i+5];
    b[i+6] += a[i+6];
    b[i+7] += a[i+7];
    b[i+8] += a[i+8];
    b[i+9] += a[i+9];
    b[i+10] += a[i+10];
    b[i+11] += a[i+11];
    b[i+12] += a[i+12];
    b[i+13] += a[i+13];
    b[i+14] += a[i+14];
    b[i+15] += a[i+15];
  }
}

void timer() {
  size_t i, j;
  struct timeval start_time, end_time;
  
  gettimeofday(&start_time, NULL);
  for (j=0; j<NUM_TRAVERSALS; j++) {
    memory_loop(a, b);
  }
  gettimeofday(&end_time, NULL);
  
  double elapsed_us = calc_time(start_time, end_time);
  double elapsed_s = elapsed_us / 1000000.0;
  double bwUsed = (double) (NUM_TRAVERSALS * 3 *num_ele * sizeof(float));
  double bw = calc_bw(bwUsed, elapsed_s);
  bw_array[bw_index] = bw;
  bw_index++;
  printf("memory_loop bandwidth: %.2f MB/s\n", bw);
}

int main() {
  size_t i;
  //4KB 8Kb .....28KB
  for (i=1; i<8; i++) {
    SIZE = i*4*BYTES_PER_KB;
    size_array[sa_index] = SIZE/BYTES_PER_KB;
    sa_index++;
    printf("SIZE: %d KB\n", SIZE/BYTES_PER_KB);
    printf("NUM_ELEMENTS: %d\n", NUM_ELEMENTS);
    printf("%luKB \n", SIZE/BYTES_PER_KB);
    init_array();
    timer();
    free(a);
    free(b);
    printf("===========\n");
  }
  // 32KB...272KB 
  for (i = 2; i <18; i++){
    SIZE = i*16*BYTES_PER_KB;
    size_array[sa_index] = SIZE/BYTES_PER_KB;
    sa_index++;
    printf("SIZE: %d KB\n", SIZE/BYTES_PER_KB);
    printf("NUM_ELEMENTS: %d\n", NUM_ELEMENTS);
    printf("%luKB \n", SIZE/BYTES_PER_KB);
    init_array();
    timer();
    free(a);
    free(b);
    printf("===========\n");
  }
  //
  for (i=18; i<300; i*=2) {
    SIZE = i*16*BYTES_PER_KB;
    size_array[sa_index] = SIZE/BYTES_PER_KB;
    sa_index++;
    printf("SIZE: %d KB\n", SIZE/BYTES_PER_KB);
    printf("NUM_ELEMENTS: %d\n", NUM_ELEMENTS);
    printf("%luKB \n", SIZE/BYTES_PER_KB);
    init_array();
    timer();
    free(a);
    free(b);
    printf("===========\n");
  }
  
 
  for (i=1; i<1000; i*=2) {
    SIZE = (i+1)*BYTES_PER_MB;
    size_array[sa_index] = SIZE/BYTES_PER_KB;
    sa_index++;
    printf("SIZE: %d MB\n", SIZE/BYTES_PER_MB);
    printf("NUM_ELEMENTS: %d\n", NUM_ELEMENTS);
    printf("%lluMB \n", SIZE/BYTES_PER_KB);
    init_array();
    timer();
    free(a);
    free(b);
    printf("===========\n");
  }
  
  for (i=1; i<6; i++) {
    SIZE = i*BYTES_PER_GB;
    size_array[sa_index] = SIZE/BYTES_PER_KB;
    sa_index++;
    printf("SIZE: %d GB\n", SIZE/BYTES_PER_GB);
    printf("NUM_ELEMENTS: %d\n", NUM_ELEMENTS);
    printf("%lluMB \n", SIZE/BYTES_PER_KB);
    init_array();
    timer();
    free(a);
    free(b);
    printf("===========\n");
  }
  
  for (int k = 0 ; k < SZ; k++){
    printf("%f,",bw_array[k]);
  }
  printf("\n");
  
  for (int k = 0 ; k < SZ; k++){
    printf("%f,",size_array[k]);
  }
  printf("\n");
 
  return 0;
}
