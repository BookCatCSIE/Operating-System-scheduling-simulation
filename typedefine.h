#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#include <ucontext.h>

/*==================[macros]=================================================*/
/* This macro shall be used to start the definition of all tasks. */
#define TASK(name) void TASK_##name(void)

/* return value of API Services */
#define STATUS_OK ((status_type)0U)
#define STATUS_ERROR ((status_type)1U)

/* state of a task */
#define SUSPENDED ((task_state_type)0U)
#define READY ((task_state_type)1U)
#define RUNNING ((task_state_type)2U)

/*********** basic type definition ***********************************/
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef unsigned short int uint16;
typedef unsigned long long uint64;

/* This type is used to represent the return value of API services */
typedef unsigned char status_type;

/*********** task configuration type definition **********************/
typedef void (*entry_point_type)(void);

typedef unsigned char task_priority_type;

typedef unsigned char task_state_type;

typedef uint32 resources_mask_type;

typedef uint8 task_total_type;

typedef uint8 task_type;

typedef task_type *task_ref_type;

/*
    Brief Task Constant type definition

    This structure defines all constants and constant pointers
    needed to manage a task.
*/
typedef struct
{
    entry_point_type entry;               /* point to the entry of this task */
    task_type id;                         /* task id */
    task_priority_type static_priority;   /* the static priority of this task */
} task_const_type;

/*********** resource configuration type definition **********************/
typedef uint8 resource_type;

//--------------------------------------------------

typedef struct
{
    entry_point_type entry;
    task_type id;
    task_priority_type now_priority;
    task_priority_type before_priority;
    int resource_num;
    //resource_type resource_id[10];
    resource_type resource_id[1000];
    ucontext_t child;
    char stack[1024];
    task_state_type state;
    int activate_order;
} my_task;


//my_task *TT_ptr[10];
my_task *TT_ptr[1000];

ucontext_t *main_ptr;

//---------------------------------------

typedef struct
{
    resource_type res_id;
    task_priority_type ceiling_priority;
    task_type owner_id;
    //int in_use;
} my_resource;

//my_resource *RR_ptr[10];
my_resource *RR_ptr[1000];

//------------------------------------------------

my_task *Running_TT_ptr;

int activate_order_value;

#endif /* #ifndef TYPEDEFINE_H */
