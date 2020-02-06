
typedef struct image_ {
	union  {
		double* image_values_d;
		int* image_values_i;
	};
	unsigned int lengthX;
	unsigned int lengthY;
} imageMatrix;
