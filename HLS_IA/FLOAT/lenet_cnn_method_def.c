#include "lenet_cnn_float.h"


void Conv1_28x28x1_5x5x20_1_0(float 			input[IMG_DEPTH][IMG_HEIGHT][IMG_WIDTH], 	                // IN
	float 		    kernel[CONV1_NBOUTPUT][IMG_DEPTH][CONV1_DIM][CONV1_DIM], 	// IN
	float 		    bias[CONV1_NBOUTPUT],						                // IN
	float 		    output[CONV1_NBOUTPUT][CONV1_HEIGHT][CONV1_WIDTH]) 		// OUT
{
	float res = 0.0f;
	for (short filter_index = 0; filter_index < CONV1_NBOUTPUT; filter_index++) { // FILTER AND IMAGE DON'T SHARE THE SAME DIMENSION!!!!
		for (short y = 0;y < IMG_WIDTH - CONV1_DIM + 1;y += CONV1_STRIDE) {
			for (short x = 0; x < IMG_HEIGHT - CONV1_DIM + 1; x += CONV1_STRIDE) {
				res = 0;
				for (short ky = 0; ky < CONV1_DIM; ky += CONV1_STRIDE) {
					for (short kx = 0; kx < CONV1_DIM; kx += CONV1_STRIDE) {
						res += input[0][y + ky][x + kx] * kernel[filter_index][0][ky][kx]; // 0 because the depth is 1 so index 0 
					}
				}
				res += bias[filter_index];
				output[filter_index][y][x] =res >0 ? res :0; //added
			}
		}
	}

}


void Pool1_24x24x20_2x2x20_2_0(float 	input[CONV1_NBOUTPUT][CONV1_HEIGHT][CONV1_WIDTH], 	    // IN
	float 	output[POOL1_NBOUTPUT][POOL1_HEIGHT][POOL1_WIDTH])	// OUT
{
	for (short z = 0; z < CONV1_NBOUTPUT; z++) {
		for (short x = 0; x < CONV1_WIDTH - POOL1_DIM + 1; x += POOL1_STRIDE) {
			for (short y = 0; y < CONV1_HEIGHT - POOL1_DIM + 1; y += POOL1_STRIDE) {
				float tab[4] = { input[z][y][x], input[z][y][x + 1],input[z][y + 1][x], input[z][y + 1][x + 1] };
				float max = tab[0];
				for (short i = 1; i < 4; i++) {  // Changed to i < 4
					max = max < tab[i] ? tab[i] : max;
				}
				output[z][y / POOL1_STRIDE][x / POOL1_STRIDE] = max;
				
			}
		}
	}
}

void Conv2_12x12x20_5x5x40_1_0(float input[POOL1_NBOUTPUT][POOL1_HEIGHT][POOL1_WIDTH], 	            // IN
	float kernel[CONV2_NBOUTPUT][POOL1_NBOUTPUT][CONV2_DIM][CONV2_DIM], 	// IN
	float bias[CONV2_NBOUTPUT], 						                    // IN
	float output[CONV2_NBOUTPUT][CONV2_HEIGHT][CONV2_WIDTH]) 		        // OUT
{
	float res = 0.0f;

	for (short filter_index = 0; filter_index < CONV2_NBOUTPUT; filter_index++) { // FILTER AND IMAGE DON'T SHARE THE SAME DIMENSION!!!!
		for (short y = 0;y < POOL1_WIDTH - CONV2_DIM + 1;y += CONV2_STRIDE) {
			for (short x = 0; x < POOL1_HEIGHT - CONV2_DIM + 1; x += CONV2_STRIDE) {
				res = 0;
				for (short z = 0; z < POOL1_NBOUTPUT; z++) {
					for (short ky = 0; ky < CONV2_DIM; ky += CONV2_STRIDE) {
						for (short kx = 0; kx < CONV2_DIM; kx += CONV2_STRIDE) {
							res += input[z][y + ky][x + kx] * kernel[filter_index][z][ky][kx]; // 0 because the depth is 1 so index 0 
						}
					}
				}

				res+= bias[filter_index];
				output[filter_index][y][x] = res>0 ? res:0; //added
			}
		}
	}
}

void Pool2_8x8x40_2x2x40_2_0(float 	input[CONV2_NBOUTPUT][CONV2_HEIGHT][CONV2_WIDTH], 	    // IN
	float 	output[POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH])		// OUT
{
	for (short z = 0; z < CONV2_NBOUTPUT; z++) {
		for (short x = 0; x < CONV2_WIDTH - POOL2_DIM + 1; x += POOL2_STRIDE) {
			for (short y = 0; y < CONV2_HEIGHT - POOL2_DIM + 1; y += POOL2_STRIDE) {
				float tab[4] = { input[z][y][x], input[z][y][x + 1],input[z][y + 1][x], input[z][y + 1][x + 1] };
				float max = tab[0];
				for (short i = 1; i < 4; i++) {
					max = max < tab[i] ? tab[i] : max;
				}
				output[z][y / POOL2_STRIDE][x / POOL2_STRIDE] = max;
			}
		}
	}
}

void Fc1_40_400(float 	input[POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH], 			        // IN
	float 	kernel[FC1_NBOUTPUT][POOL2_NBOUTPUT][POOL2_HEIGHT][POOL2_WIDTH],	// IN
	float 	bias[FC1_NBOUTPUT],			 					                        // IN
	float 	output[FC1_NBOUTPUT])						                    // OUT

{
	float sum =0;
	for (short k = 0; k < FC1_NBOUTPUT; k++) {
		sum= bias[k];
		for (short z = 0; z < POOL2_NBOUTPUT; z++)
			for (short y = 0; y < POOL2_HEIGHT; y++)
				for (short x = 0; x < POOL2_WIDTH; x++)
					sum += input[z][y][x] * kernel[k][z][y][x];
		output[k] = ( sum > 0) ? sum : 0;
	}


}

void Fc2_400_10(float 	input[FC1_NBOUTPUT], 			        // IN
	float 	kernel[FC2_NBOUTPUT][FC1_NBOUTPUT],	    // IN
	float 	bias[FC2_NBOUTPUT],			            // IN
	float 	output[FC2_NBOUTPUT])		        // OUT 
{
	float sum=0;
	for (short k = 0; k < FC2_NBOUTPUT; k++) {
		sum= bias[k];
		for (short i = 0; i < FC1_NBOUTPUT; i++){
			sum+= input[i] * kernel[k][i];
		}
		output[k] = (sum> 0) ? sum : 0;//added
		
		    

	}
}


void Softmax(float vector_in[FC2_NBOUTPUT], float vector_out[FC2_NBOUTPUT]) {

	float frac_sum = 0;
	float exp_values[FC2_NBOUTPUT];
	for (short k = 0;k < FC2_NBOUTPUT; k++) {
		exp_values[k]=exp(vector_in[k]);
		frac_sum += exp_values[k];
	}

	for (short j = 0; j < FC2_NBOUTPUT;j++) {
		vector_out[j] = exp_values[j]/ frac_sum;

	}


}