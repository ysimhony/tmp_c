
#include "first_phase_parsing.h"
#include "second_phase_parsing.h"



int main() {
   

   first_phase_parsing();
   print_label_map(&data_code_labels);
   print_code_arr();
   second_phase_parsing();
   print_label_map(&data_code_labels);
   print_code_arr();

   return 0;
}
