#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>


int sys_enable_log(pid_t pid, int sizes){
	unsigned int res;
	if (pid < 0) {
		res = -ESRCH;
		return (int) res;
	}
	if (sizes < 0) {
		res = -EINVAL;
		return (int) res;
		//return -1;
	}
	task_t *task = find_task_by_pid(pid);
	
	if (!task){
		res = -ESRCH;
		return res;			 //not pid like this - ESRCH
	}
	
	if(task->log_info == 1){
		res = -EINVAL;
		return (int) res;		 //enable already 1 - EINVAL
	}
	if(sizes == 0 ){
		task->val_num = 0;
		task->log_info = 1;
		task->size_arr = 0;
		res=0;
		return (int) res;
	}
	
	syscall_log_t* arr;
	arr = (syscall_log_t*) kmalloc(sizes * (sizeof(*arr)), GFP_KERNEL);
	if(!arr){
		res = -ENOMEM;
		return (int) res; 		//MEM - ENOMEM
	}
	
	task->arr = arr;
	
	int i=0;
	for (i=0; i<sizes; i++){
		task->arr[i].time = -1; //init the array with negative time
	}
	
	task->val_num = 0;
	task->log_info = 1;
	task->size_arr = sizes; 
	
	res = 0;
	return (int) res;
}

int sys_disable_log (pid_t pid){
	unsigned int res;
	if (pid < 0) {
		res = -ESRCH;
		return (int) res;
	}
	task_t *task = find_task_by_pid(pid);
	if (!task){
		res = -ESRCH;
		return (int) res; //not pid like this - ESRCH
	}
	if(task->log_info == 0){
		res = -EINVAL;
		return (int) res;  //enable already 0 - EINVAL
	}
	
	kfree(task->arr);
	
	task->arr = NULL;
	task->val_num = 0;
	task->log_info = 0;
	task->size_arr = -1; 
	
	res = 0;
	return (int) res;
}

int sys_get_log(pid_t pid, int size, struct syscall_log_info* user_mem){
	unsigned int res;
	//printk("\nres1\n");
	//printk("\size: %d\n", size);	
	if (pid < 0) {
		res = -ESRCH;
		return (int) res;
	}
	if(size == 0 ){
		//printk("res2\n");
		res = 0;
		return (int) res;
	}
	if (size < 0) {
				//	printk("@@res1\n");
		res = -EINVAL;
		return (int) res;
	}

	task_t *task = find_task_by_pid(pid);
	
			//printk("res3\n");
	if (!task){
		res = -ESRCH;
		return (int) res; //not pid like this - ESRCH
	}
	
	if((!(task->arr) && (task->size_arr != 0)) || (task->log_info == 0)){
		//	printk("@@res2\n");
		res = -EINVAL;
		return (int) res; 		//EINVAL
	}
	if (size > task->size_arr){
			//printk("@@res3\n");
		res = -EINVAL;
		return (int) res;  		//EINVAL
	}
	
	if(!user_mem){
			//printk("@@res4\n");
		res = -EINVAL;
		return (int) res; 		//MEM - ENOMEM
	}
				//printk("res4\n");
				
	//if the user is asking for more vals than we have in the array
	
	if (size > task->val_num ){
			//printk("@@res5\n");
		res = -EINVAL;
		return (int) res;
	}
	
	//if works - copy 
	if (copy_to_user (user_mem, task->arr, sizeof(syscall_log_t)*size)){
		res = -ENOMEM; 	//the 3rd parameters should be the size- how to caculate that
		return (int) res; 		//MEM - ENOMEM
	}
				//printk("res5\n");
				

	
	//shift left the ones we didnt copy
	if(task->val_num != size){
		int i=0;
		for ( ; i<task->size_arr-size ; i++){
			task->arr[i].syscall_num= task->arr[size+i].syscall_num;
			task->arr[i].syscall_res= task->arr[size+i].syscall_res;
			task->arr[i].time= task->arr[size+i].time;
		}
	}
				//printk("@@res6\n");
	//update the num of vals in the array
	task->val_num = (task->val_num)-size;

	
	res = 0;
	return (int) res;
}



