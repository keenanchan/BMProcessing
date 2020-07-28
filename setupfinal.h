//FUNCTION HEADERS HERE
//   FUNCTIONS FOR READING AND WRITING
double **readpicture(ifstream &infile, int &width, int &height);
char readchar(ifstream& infile, int &num);
int readint(ifstream& infile, int numbytes, int &num);
int char2int(char thechar);
int char2int(char *thechar, int numbytes);
void readrgb(ifstream& infile, double *rgb, int &num);
void writepicture(ofstream& outfile, double **f, int width, int height);
char int2char(unsigned long theint);
void int2char(char *thechar, int theint);
void writechar(ofstream &outfile, char thechar);
void writeint(ofstream &outfile, int theint, int numbytes);
void writergb(ofstream& outfile, double r, double g, double b);
//   FUNCTIONS FOR CREATING AND REMOVING MEMORY
double **matrix(int width, int height);
void free_matrix(double **f, int width, int height);
int **imatrix(int width, int height);
void free_matrix(int **f, int width, int height);
//   ADDITIONAL FUNCTIONS FOR READING AND WRITING
void writecurveinpicture(ofstream& outfile, double **f, double **phi, int width, 
                         int height);
void readcolorpicture(double** &fr, double** &fg, double** &fb, ifstream &infile, 
                      int &width, int &height);
void writecolorpicture(ofstream& outfile, double **fr, double **fg, double **fb, 
                       int width, int height);
//   YOUR FUNCTIONS HERE
