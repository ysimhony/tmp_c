
#include "first_phase_parsing.h"
#include "second_phase_parsing.h"



int main() {

	first_phase_parsing();
	print_label_map(&data_code_labels);
	print_code_arr();
	second_phase_parsing();
	print_label_map(&data_code_labels);
	print_code_arr();

	write_arr_to_file("yacov", FALSE);

	check_result(FALSE);
	return 0;
}
