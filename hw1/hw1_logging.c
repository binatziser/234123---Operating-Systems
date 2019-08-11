#include <linux/sched.h>
#include <linux/kernel.h>

asmlinkage void syscall_fill_arr (int res, int num){
	if (current->log_info == 0) {
		return;
	}
	if (current->size_arr ==0){
		return;
	}
	
	if( current->size_arr > current->val_num ){
		
		current->arr[current->val_num].syscall_num= num;
		current->arr[current->val_num].syscall_res= res;
		current->arr[current->val_num].time= jiffies;
		
		(current->val_num)++;
	
	}else{
		//case2 - we already filled the array ones array size=val num
		//shift left
		int i=0;
		for(; i < current->size_arr-1 ; i++){
			current->arr[i].syscall_num= current-> arr[i+1].syscall_num;
			current->arr[i].syscall_res= current-> arr[i+1].syscall_res;
			current->arr[i].time= current-> arr[i+1].time;
		}
		
		//fills the last place in the array
		current->arr[current->size_arr-1].syscall_num= num;
		current->arr[current->size_arr-1].syscall_res= res;
		current->arr[current->size_arr-1].time= jiffies;	
	}
	

}


