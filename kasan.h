#include <stdbool.h>

int kasan_early_init(void);
int kasan_init(void);
bool is_kasan_setup(void);
