#include "resource.h"

#include <string.h>
#include <stdio.h>

//Running_TT_ptr
int res_index = 0;
int k =0;

status_type get_resource(resource_type id)
{
    status_type result = STATUS_OK;

    for(k=0; k<RESOURCES_COUNT; k++)
    {
        if(RR_ptr[k]->res_id == id)
        {
            res_index = k;
        }
    }

    //RR_ptr[res_index]-> owner_id == TT_ptr[0]->id
    if(RR_ptr[res_index]->owner_id == TT_ptr[0]->id)  //TT_ptr[0]->id //idle_task
    {
        result = STATUS_OK;
        RR_ptr[res_index]->owner_id = Running_TT_ptr->id;
        //Running_TT_ptr->before_priority = Running_TT_ptr->now_priority;
        //Running_TT_ptr->now_priority = RR_ptr[res_index]->ceiling_priority;
        if(Running_TT_ptr->now_priority < RR_ptr[res_index]->ceiling_priority)
        {
            Running_TT_ptr->before_priority = Running_TT_ptr->now_priority;
            Running_TT_ptr->now_priority = RR_ptr[res_index]->ceiling_priority;
        }
        Running_TT_ptr->resource_num +=1;
        //Running_TT_ptr->resource_id[]...;

        //printf("%d \n",RR_ptr[res_index]->owner_id);
        //printf("%d \n",Running_TT_ptr->before_priority);
        //printf("%d \n",Running_TT_ptr->now_priority);
        //printf("%d \n",Running_TT_ptr->resource_num);
        //swapcontext( &(Running_TT_ptr->child),main_ptr);
    }
    else
    {
        result = STATUS_ERROR;
    }

    return result;
}

status_type release_resource(resource_type id)
{
    status_type result = STATUS_OK;

    for(k=0; k<RESOURCES_COUNT; k++)
    {
        if(RR_ptr[k]->res_id == id)
        {
            res_index = k;
        }
    }

    if(RR_ptr[res_index]-> owner_id == Running_TT_ptr->id)
    {
        result = STATUS_OK;
        RR_ptr[res_index]-> owner_id = TT_ptr[0]->id;
        Running_TT_ptr->now_priority = Running_TT_ptr->before_priority;
        Running_TT_ptr->resource_num -=1;
        //Running_TT_ptr->resource_id[]...;
        Running_TT_ptr->state = READY;//////
        swapcontext( &(Running_TT_ptr->child),main_ptr);
    }
    else
    {
        result = STATUS_ERROR;
    }


    return result;
}
