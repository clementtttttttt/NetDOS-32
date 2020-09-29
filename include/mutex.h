#include<stdbool.h>
int mutex_lock(volatile bool* lock){
    if(!__sync_bool_compare_and_swap(lock,false,true)){
            return -1;
    }
    else return 0;
}
void mutex_unlock(volatile bool* lock){
    *lock=false;
}
