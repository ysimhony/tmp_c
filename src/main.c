
#include "first_phase_parsing.h"
#include "second_phase_parsing.h"


int code_arr[NUM_OF_BYTES] = {0};
int IC = 0;
int L = 0;

/* Array holding the data */
int data_arr[NUM_OF_BYTES] = {0};
int DC = 0;

int main() {
   

   first_phase_parsing();
   print_label_map();
   print_code_arr();
   second_phase_parsing();
   print_label_map();
   print_code_arr();

   return 0;
}
