#include "resource.h"
#include "task.h"

#include <ucontext.h>
#include <signal.h>
#include "task_set.h"
#include "config.h"
#include "typedefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>

int i =0;

int main()
{
    //TASK(T1); --> wrong
    /*
    char stack[1024*128];
    ucontext_t child,parent;

    getcontext(&child);
    child.uc_stack.ss_sp = stack;
    child.uc_stack.ss_size = sizeof(stack);
    child.uc_stack.ss_flags = 0;
    child.uc_link = &parent;
    */
    //makecontext(&child,(void (*)(void))TASK_T1,0);
    //makecontext(&child,(void (*)(void))TASK(T1),0); --> wrong : TASK(T1)
    //makecontext(&child,(void (*)(void))task_const[T1].entry,0);

    //printf("%ld\n",sizeof(auto_start_tasks_list));
    /*
    int max_start = 0;
    for(i=0;i<sizeof(auto_start_tasks_list);i++){
        if(task_const[auto_start_tasks_list[i]].static_priority > max_start){
            //max_start = auto_start_tasks_list[i];
        max_start = task_const[auto_start_tasks_list[i]].id;
        }
    }
    makecontext(&child,(void (*)(void))task_const[max_start].entry,0);

    swapcontext(&parent,&child);
    */
    //puts("parent");

    ucontext_t parent;

    my_task TT[TASKS_COUNT];

    for(i=0; i<TASKS_COUNT; i++)
    {
        TT[i].entry = task_const[i].entry;
        TT[i].id = task_const[i].id;
        TT[i].now_priority = task_const[i].static_priority;
        TT[i].before_priority = task_const[i].static_priority;
        TT[i].resource_num = 0;
        TT[i].state = SUSPENDED;
        TT[i].activate_order = 1000;

        getcontext(&(TT[i].child));
        TT[i].child.uc_stack.ss_sp = TT[i].stack;
        TT[i].child.uc_stack.ss_size = sizeof(TT[i].stack);
        TT[i].child.uc_stack.ss_flags = 0;
        //TT[i].child.uc_link = NULL;
        TT[i].child.uc_link = &parent;

        makecontext(&(TT[i].child),(void (*)(void))TT[i].entry,0);

        //printf("%d  %d \n",TT[i].id,TT[i].now_priority);
        //swapcontext(&parent,&(TT[i].child));
    }

    /*
    for(i=1;i<TASKS_COUNT;i++){
        swapcontext(&parent,&TT[i].child);
    //setcontext(&TT[i].child); --> Segmentation fault (core dumped)
    }
    */

    for(i=0; i<TASKS_COUNT; i++)
    {
        TT_ptr[i] = malloc(sizeof(my_task));
        TT_ptr[i] = &TT[i];
        //printf("%d  %d \n",TT_ptr[i]->id,TT_ptr[i]->now_priority);
    }
    //access_TTptr();

    main_ptr = &parent;


    my_resource RR[RESOURCES_COUNT];

    for(i=0; i<RESOURCES_COUNT; i++)
    {
        RR[i].res_id = resources_id[i];
        RR[i].ceiling_priority = resources_priority[i];
        RR[i].owner_id = TT[0].id;
        //RR[i].owner_id = idle_task;
        //printf("%d\n",RR[i].ceiling_priority);
    }

    for(i=0; i<RESOURCES_COUNT; i++)
    {
        RR_ptr[i] = malloc(sizeof(my_resource));
        RR_ptr[i] = &RR[i];
    }


    //// auto start
    int max_start = 0;
    activate_order_value = 1;

    for(i=0; i<sizeof(auto_start_tasks_list); i++)
    {
        //set_task_activated(auto_start_tasks_list[i]);
        TT[auto_start_tasks_list[i]].state = READY;
        TT[auto_start_tasks_list[i]].activate_order = activate_order_value;

        if(TT[auto_start_tasks_list[i]].now_priority > max_start)
        {
            //max_start = TT[auto_start_tasks_list[i]].id;
            max_start = auto_start_tasks_list[i];
        }
    }
    //swapcontext(&parent,&TT[max_start].child);

    /*
    int still_has_activated_task = 0; //boolean
    if(sizeof(auto_start_tasks_list)>0){
    still_has_activated_task = 1;
    }

    task_priority_type max_priority =0;
    task_type max_id =0;

    while(still_has_activated_task == 1){
    max_priority = -1;
    max_id = -1;
        for(i=0;i<TASKS_COUNT;i++){
            if(TT[i].state == READY && TT[i].now_priority > max_priority){
                max_priority = TT[i].now_priority;
    	max_id = TT[i].id;
            }
    }
    swapcontext(&parent,&TT[max_id].child);

    still_has_activated_task = 0;
    for(i=0;i<TASKS_COUNT;i++){
            if(TT[i].state == READY){
                still_has_activated_task = 1;
        }
        }
    }
    */

    Running_TT_ptr = &TT[max_start];

    task_priority_type max_priority =0;
    int max_index =0;

    while(1)
    {
        //getcontext(&parent);
        //max_priority = 0;
        //max_index = 0;
        max_priority = Running_TT_ptr->now_priority;
        max_index = Running_TT_ptr->id;
        for(i=0; i<TASKS_COUNT; i++)
        {
            if(TT[i].state == READY && TT[i].now_priority > max_priority)
            {
                max_priority = TT[i].now_priority;
                max_index = i;
            }
        }

        int temp_index = max_index; // fixed max_index
	int temp_activate_order = TT[max_index].activate_order;

        for(i=0; i<TASKS_COUNT; i++)
        {
	    /*
            if(i!=temp_index && TT[i].state == READY && TT[i].now_priority == TT[temp_index].now_priority && TT[i].activate_order < TT[temp_index].activate_order)
            {
                max_index = i;
            }
	    */
	    if(TT[i].state == READY && TT[i].now_priority == TT[temp_index].now_priority && TT[i].activate_order < temp_activate_order)
            {
                max_index = i;
		temp_activate_order = TT[i].activate_order;
            }

        }

        /*
        for(i=0;i<TASKS_COUNT;i++){
                if(i!=max_index && TT[i].state == RUNNING){
                    TT[i].state = READY;
            }
        }
        */
        TT[max_index].state = RUNNING;
        Running_TT_ptr = &TT[max_index];
        swapcontext(&parent,&TT[max_index].child);

    }






    //swapcontext(&parent,&TT[0].child);  // --> exit(0)

    return 0;
}
