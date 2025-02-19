#include "task.h"

#include <stdio.h>
#include <ucontext.h>

//Running_TT_ptr

int j =0;
int task_index =0;

status_type activate_task(task_type id)
{
    status_type result = STATUS_OK;

    for(j=0; j<TASKS_COUNT; j++)
    {
        if(TT_ptr[j]->id == id)
        {
            task_index = j;
        }
    }

    if(TT_ptr[task_index]->state == SUSPENDED)
    {
        TT_ptr[task_index]->state = READY;
        activate_order_value +=1;
        TT_ptr[task_index]->activate_order = activate_order_value;
        result = STATUS_OK;
        //Running_TT_ptr->state = READY;//////
        //swapcontext( &(Running_TT_ptr->child),main_ptr);
	if(Running_TT_ptr->now_priority < TT_ptr[task_index]->now_priority){
            Running_TT_ptr->state = READY;
	    TT_ptr[task_index]->state = RUNNING;
	    swapcontext( &(Running_TT_ptr->child),&(TT_ptr[task_index]->child));
	}
    }
    else
    {
        result = STATUS_ERROR;
    }

    return result;
}

status_type terminate_task(void)
{
    status_type result = STATUS_OK;

    if(Running_TT_ptr->state == RUNNING && Running_TT_ptr->resource_num == 0)
    {
        Running_TT_ptr->state = SUSPENDED;
        Running_TT_ptr->activate_order = 1000;
        result = STATUS_OK;
        //swapcontext( &(Running_TT_ptr->child),main_ptr);//////?

        Running_TT_ptr = TT_ptr[0]; ////////// running pointer should not point to terminated task
    }
    else
    {
        result = STATUS_ERROR;
    }

    return result;
}

/*
void access_TTptr(void){
    for(j=0;j<TASKS_COUNT;j++){
        printf("%d  %d \n",TT_ptr[j]->id,TT_ptr[j]->now_priority);
    }

}
*/

/*
void set_task_activated(task_type id){
    TT_ptr[id]->state = READY;
}
*/

