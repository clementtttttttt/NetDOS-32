#include<mutex.h>
int mutex_lock(volatile bool* lock){
    for(;!__sync_bool_compare_and_swap(lock,false,true);){
        asm("pause");
    }
    return 0;
}
void mutex_unlock(volatile bool* lock){
    *lock=false;
}
