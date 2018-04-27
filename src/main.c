
#include "first_phase_parsing.h"
#include "second_phase_parsing.h"
#include "create_files.h"


int main() {

	first_phase_parsing();
#if DEBUG
	print_label_map(&data_code_labels);
	print_code_arr();
#endif

	second_phase_parsing();
#if DEBUG
	print_label_map(&data_code_labels);
	print_code_arr();
#endif

	create_files();

	check_result(FALSE);
	return 0;
}
