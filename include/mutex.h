#include<stdbool.h>
int mutex_lock(volatile bool* lock);
void mutex_unlock(volatile bool* lock);
