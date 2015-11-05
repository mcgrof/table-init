#include <stdbool.h>

int kasan_early_init(void);
void setup_arch_kasan(void);
bool is_kasan_setup(void);
