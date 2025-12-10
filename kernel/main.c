/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "func.h"
#include "global.h"
#include "process.h"
#include "../games/snake/snake.h"
#include "../lib/lib.h"

extern char keyboard_input(void);
extern void disable_tty_output(void);
/* 声明已有的函数，供我们调用 */
extern void game_start();     /* 贪吃蛇，原来就有 */
extern void clear();          /* 清屏 */
extern int  get_ticks();      /* 时钟 tick，内核已有封装 */


/* 打印菜单（公共函数） */
static void show_menu(void)
{
    printf("\n================ MuteOS Demo Menu ================\n");
    printf("  1) Play Snake Game\n");
    printf("  2) Show System Uptime\n");
    printf("  3) Clear Screen\n");
    printf("  4) About This OS\n");
    printf("==================================================\n");
    printf("Please input 1-4 and press Enter: ");
}

/* 功能 1：贪吃蛇 */
static void feature_snake(void)
{
    clear();
    printf("Entering Snake game, press ESC to quit...\n");
    game_start();             /* 这里直接调用原来的贪吃蛇 */
    printf("\nBack from Snake game.\n");
}

/* 功能 2：显示运行时间 */
static void feature_uptime(void)
{
    int ticks = get_ticks();  /* 获取自启动以来的 tick 数 */
    int seconds = ticks / 100;  /* 如果 1 tick = 10ms，大致是 tick/100 秒 */

    printf("\nSystem uptime: %d ticks (~%d seconds)\n", ticks, seconds);
}

/* 功能 3：清屏 */
static void feature_clear(void)
{
    clear();
    printf("Screen cleared.\n");
}

/* 功能 4：关于本系统 */
static void feature_about(void)
{
    printf("\nMini OS Demo by Leyu.\n");
    printf("This OS provides 4 demo functions:\n");
    printf("  1) Snake game\n");
    printf("  2) Show uptime\n");
    printf("  3) Clear screen\n");
    printf("  4) About information\n");
}

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
    print("[ OK ]", Green);
    print(" kernel_main begins\n", White);

    TASK*      p_task       = task_table;
    PROCESS*   p_proc       = proc_table;
    char*      p_task_stack = task_stack + STACK_SIZE_TOTAL;
    u16        selector_ldt = SELECTOR_LDT_FIRST;
    u8         privilege;
    u8         rpl;
    int        eflags;
    for (int i = 0; i < NR_TASKS + NR_PROCS; i++) {
        if (i < NR_TASKS) {
            // tasks
            p_task = task_table + i;
            privilege = PRIVILEGE_TASK;
            rpl = RPL_TASK;
            eflags = 0x1202;            /* IF=1, IOPL=1, bit2 is always 1 */   
        } 
        else {
            // user processes
            p_task = user_proc_table + (i - NR_TASKS);
            privilege = PRIVILEGE_USER;
            rpl = RPL_USER;
            eflags = 0x202;             /* IF=1, bit2 is always 1 */ 
        }

        strcpy(p_proc->p_name, p_task->name);   // name of the process
        p_proc->pid = i;                        // pid

        p_proc->ldt_sel = selector_ldt;

        memocpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3],
               sizeof(DESCRIPTOR));
        p_proc->ldts[0].attr1 = DA_C | privilege << 5;
        memocpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3],
               sizeof(DESCRIPTOR));
        p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
        p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
            | SA_TIL | rpl;
        p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK)
            | rpl;

        p_proc->regs.eip = (u32)p_task->initial_eip;
        p_proc->regs.esp = (u32)p_task_stack;
        p_proc->regs.eflags = eflags;

        p_task_stack -= p_task->stacksize;
 
        /* init priority */
        proc_table[i].priority = p_task->priority;
        proc_table[i].ticks = proc_table[i].priority * TICK_BIAS;
        proc_table[i].nr_tty = 0;

        p_proc++;
        p_task++;
        selector_ldt += 1 << 3;
    }

    k_reenter = 0;
    ticks = 0;

    init_clock();    
    init_keyboard();

    p_proc_ready    = proc_table; 
    restart();

    while(1){}
}


/*======================================================================*
                               init_clock
 *======================================================================*/
PUBLIC void init_clock()
{
    /* initialize 8253 PIT */
    out_byte(TIMER_MODE, RATE_GENERATOR);
    out_byte(TIMER0, (u8)(TIMER_FREQ / HZ));
    out_byte(TIMER0, (u8)(TIMER_FREQ / HZ) >> 8);

    put_irq_handler(CLOCK_IRQ, clock_handler);  /* 设定时钟中断处理程序 */
    enable_irq(CLOCK_IRQ);                      /* 让8259A可以接收时钟中断 */    
}


/*======================================================================*
                               TestB
 *======================================================================*/
/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
    disable_tty_output();  /* 关闭原来的 TTY 输出，让我们独占键盘和屏幕 */
    clear();

    while (1) {
        char choice = 0;

        /* 每一轮先打印菜单 */
        show_menu();

        /* 等待用户输入“数字 + Enter” */
        while (1) {
            unsigned char ch = (unsigned char)keyboard_input();

            if (ch == 0) {
                /* 没有按键就继续等 */
                continue;
            }

            /* 如果按的是 1~4，就记录下来，并回显一下 */
            if (ch >= '1' && ch <= '4') {
                choice = (char)ch;
                printf("%c", choice);   /* 在屏幕上显示这个数字 */
            }
            /* 如果按的是 Enter（用 snake.h 里定义的 ENTER 宏） */
            else if (ch == ENTER) {
                /* 用户按了回车，结束本轮输入 */
                break;
            }
            /* 其他键直接忽略 */
        }

        /* 根据最终选择的数字执行对应功能 */
        switch (choice) {
        case '1':
            feature_snake();
            break;
        case '2':
            feature_uptime();
            break;
        case '3':
            feature_clear();
            break;
        case '4':
            feature_about();
            break;
        default:
            printf("\nInvalid choice. Please enter 1-4.\n");
            break;
        }

        /* 执行完一个功能后，清屏，然后下一轮再显示菜单 */
        clear();
    }
}


