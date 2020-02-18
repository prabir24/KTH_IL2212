
typedef struct image_ {
	
	double* image_values_d;	
	unsigned int lengthX;
	unsigned int lengthY;
} imageMatrix;

typedef struct ppmFile_ {
	unsigned int lengthX;
	unsigned int lengthY;
	unsigned int maxValue;
	unsigned int* pixelValues;
} ppmFile;
