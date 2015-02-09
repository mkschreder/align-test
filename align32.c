#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define REPEAT_COUNT 50000
#define data_t uint32_t
#define DATA_SIZE (32768 * (sizeof(uint64_t) / sizeof(data_t)) / 2)	
#define INDEX_SIZE 100000

uint32_t indexes[INDEX_SIZE];
data_t data1[DATA_SIZE];
data_t data2[DATA_SIZE];
data_t data3[DATA_SIZE];
data_t data4[DATA_SIZE];

uint32_t accumulate_non_aligned(uint8_t *data1, uint8_t *data2, uint8_t *data3, uint8_t *data4, uint32_t *indexes, uint32_t size) {
	int i, j;
	uint32_t result = 0;
	for (j=0; j<REPEAT_COUNT; j++) {
		for (i=0; i<size; i++) {
			result += *((volatile data_t *)(data1+indexes[i]));
			result += *((volatile data_t *)(data2+indexes[i]));
			result += *((volatile data_t *)(data3+indexes[i]));
			result += *((volatile data_t *)(data4+indexes[i]));
		}
	}
	return result;
}

data_t accumulate_aligned(data_t *data1, data_t *data2, data_t *data3, data_t *data4, uint32_t *indexes, uint32_t size) {
	int i, j;
	data_t result = 0;
	for (j=0; j<REPEAT_COUNT; j++) {
		for (i=0; i<size; i++) {
			result += *((volatile data_t *)(data1+indexes[i]));
			result += *((volatile data_t *)(data2+indexes[i]));
			result += *((volatile data_t *)(data3+indexes[i]));
			result += *((volatile data_t *)(data4+indexes[i]));
		}
	}
	return result;
}

int main(int argc, char **argv) {
	int32_t i;
	volatile data_t result = 0;
	clock_t start, end;
	srand(1);
	for (i=0;i<INDEX_SIZE;i++) {
		indexes[i] = rand() % DATA_SIZE;
	}
	for (i=0;i<DATA_SIZE;i++) {
		data1[i] = rand();
		data2[i] = rand();
		data3[i] = rand();
		data4[i] = rand();
	}
	
	start = clock();
	result += accumulate_aligned(data1, data2, data3, data4, indexes, INDEX_SIZE);
	end = clock();
	printf("ALIGNED (%d): result = %d, time = %d\n", (int)sizeof(data_t), (int)result, (int)((end-start)/(CLOCKS_PER_SEC/1000)));
	
	for(i = 0; i < INDEX_SIZE; i++){
		if (i & 0x1) {
			indexes[i] = (rand() % (DATA_SIZE/8))*8*4 + 30;
		} else {
			indexes[i] = (rand() % (DATA_SIZE/8))*8*4 + 30;
		}
	}
	for (i=0;i<DATA_SIZE;i++) {
		data1[i] = rand();
		data2[i] = rand();
		data3[i] = rand();
		data4[i] = rand();
	}
	
	start = clock();
	result += accumulate_non_aligned((uint8_t *)data1, (uint8_t *)data2, (uint8_t *)data3, (uint8_t *)data4, indexes, INDEX_SIZE);
	end = clock();
	
	printf("NOT ALIGNED (%d): result = %d, time = %d\n", (int)sizeof(data_t), (int)result, (int)((end-start)/(CLOCKS_PER_SEC/1000)));
}

