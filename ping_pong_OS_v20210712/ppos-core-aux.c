#include "ppos.h"
#include "ppos-core-globals.h"


// ****************************************************************************
// Coloque aqui as suas modificações, p.ex. includes, defines variáveis, 
// estruturas e funções
#include <signal.h>
#include <sys/time.h>

#define MAX_PRIORITY 20
#define MIN_PRIORITY -20
#define ALPHA_AGING -1  // fator de envelhecimento do scheduler
#define MAX_TICKS 20    // numero maximo de ticks para uma tarefa

# define A3

struct sigaction action;    // estrutura que define um tratador de sinal
struct itimerval timer;     // estrutura de inicialização do timer

// envelhece a task
void scheduler_aging(task_t *task);

// inicializacao do tratador de sinal
void signal_init();

// inicializacao do timer
void clock_init();

// tratamento de interrupcao gerado pelo timer
void timer_interruption();

// conta tempo de vida e imprime informações da task
void finish_task_and_print(task_t *task);

void scheduler_aging(task_t *task) {
    if(task->dynamicPriority > MIN_PRIORITY)
        task->dynamicPriority += ALPHA_AGING;
}

void task_setprio (task_t *task, int prio) {   
    if(prio > MAX_PRIORITY)
        prio = MAX_PRIORITY;
    else if(prio < MIN_PRIORITY)
        prio = MIN_PRIORITY;

    if(task == NULL) {
        taskExec->staticPriority = prio;
        taskExec->dynamicPriority = prio;
    } else {
        task->staticPriority = prio;
        task->dynamicPriority = prio;
    }
}

int task_getprio (task_t *task) {
    if(task == NULL)
        return taskExec->staticPriority;
    return task->staticPriority; 
}

void signal_init() {
    // registra a ação para o sinal de timer SIGALRM
    action.sa_handler = timer_interruption;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;

    if (sigaction (SIGALRM, &action, 0) < 0) {
        perror ("\nErro em sigaction: ") ;
        exit (1) ;
    }
#ifdef DEBUG
    printf("\nsignal init");
#endif
}

void clock_init() {
    // ajusta valores do temporizador
    timer.it_value.tv_usec = 1 ;        // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec  = 0 ;        // primeiro disparo, em segundos
    timer.it_interval.tv_usec = 1000 ;  // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec = 0;       // disparos subsequentes, em segundos

    // arma o temporizador ITIMER_REAL (vide man setitimer)
    if (setitimer (ITIMER_REAL, &timer, 0) < 0) {
        perror ("\nErro em setitimer: ") ;
        exit (1) ;
    }
#ifdef DEBUG
    printf("\nclock_init");
#endif
}

void timer_interruption() {
    systemTime++;
    if (taskExec != NULL) {
        taskExec->processorTime++;
    }
    // se for uma tarefa de usuario (main, pang, ..., pung)
    if(taskExec != taskDisp) {
        // diminui o quantum da tarefa
        taskExec->quantum--;
        // se o quantum da tarefa terminou
        if(taskExec->quantum == 0) {
            // libera o processador para proxima tarefa
            task_yield();
        }
    }
}

void finish_task_and_print(task_t *task) {
    task->executionTime = systime() - task->executionTime;
    printf("Task %d exit: execution time  %u ms, processor time  %d ms, %d activations \n", 
        task->id,
        task->executionTime, 
        task->processorTime,
        task->numActivations
    );
}
// ****************************************************************************

//#define DEBUG 1

void before_ppos_init () {
    // put your customization here
    signal_init();
    clock_init();
    systemTime = 0;
#ifdef DEBUG
    printf("\ninit - BEFORE");
#endif
}

void after_ppos_init () {
    // put your customization here
#ifdef DEBUG
    printf("\ninit - AFTER");
#endif
}

void before_task_create (task_t *task ) {
    // put your customization here
    if (task != NULL) {
        task->executionTime = systime();
        task->processorTime = 0;
        task->numActivations = 1;
    }
#ifdef DEBUG
    printf("\ntask_create - BEFORE - [%d]", task->id);
#endif
}

void after_task_create (task_t *task ) {
    // put your customization here
    task->quantum = MAX_TICKS;
#ifdef DEBUG
    printf("\ntask_create - AFTER - [%d]", task->id);
#endif
}

void before_task_exit () {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_exit () {
    // put your customization here
#ifdef A3
    if (freeTask != NULL) {
        finish_task_and_print(freeTask);
    }
    if (countTasks == 1) {
        finish_task_and_print(taskDisp);
    }
#endif
    
#ifdef DEBUG
    printf("\ntask_exit - AFTER- [%d]", taskExec->id);
#endif
}

void before_task_switch ( task_t *task ) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
#endif
}

void after_task_switch ( task_t *task ) {
    // put your customization here
    if(taskExec != NULL) {
        taskExec->numActivations++;
    }
#ifdef DEBUG
    printf("\ntask_switch - AFTER - [%d -> %d]", taskExec->id, task->id);
#endif
}

void before_task_yield () {
    // put your customization here
    taskExec->quantum = MAX_TICKS; // restaura o quantum da tarefa
#ifdef DEBUG
    printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_yield () {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_yield - AFTER - [%d]", taskExec->id);
#endif
}

void before_task_suspend( task_t *task ) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_suspend - BEFORE - [%d]", task->id);
#endif
}

void after_task_suspend( task_t *task ) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_suspend - AFTER - [%d]", task->id);
#endif
}

void before_task_resume(task_t *task) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_resume - BEFORE - [%d]", task->id);
#endif
}

void after_task_resume(task_t *task) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_resume - AFTER - [%d]", task->id);
#endif
}

void before_task_sleep () {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_sleep () {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
#endif
}

int before_task_join (task_t *task) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_task_join (task_t *task) {
    // put your customization here
#ifdef DEBUG
    printf("\ntask_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}


int before_sem_create (semaphore_t *s, int value) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_create (semaphore_t *s, int value) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_down (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_down (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_up (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_up (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_destroy (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_destroy (semaphore_t *s) {
    // put your customization here
#ifdef DEBUG
    printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_create (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_create (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_lock (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_lock (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_unlock (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_unlock (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_destroy (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_destroy (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_create (barrier_t *b, int N) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_create (barrier_t *b, int N) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_join (barrier_t *b) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_join (barrier_t *b) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_destroy (barrier_t *b) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_destroy (barrier_t *b) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_create (mqueue_t *queue, int max, int size) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_create (mqueue_t *queue, int max, int size) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_send (mqueue_t *queue, void *msg) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_send (mqueue_t *queue, void *msg) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_recv (mqueue_t *queue, void *msg) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_recv (mqueue_t *queue, void *msg) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_destroy (mqueue_t *queue) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_destroy (mqueue_t *queue) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_msgs (mqueue_t *queue) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_msgs (mqueue_t *queue) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

task_t * scheduler() {
    
    if ( readyQueue != NULL ) {

        task_t *lowest_prio, *aux;
        lowest_prio = readyQueue;
        aux = lowest_prio->next;
        
        // percorre a fila circular para encontrar a tarefa com menor valor de prioridade dynamicPriority
        while(aux != readyQueue) {
            if(aux->dynamicPriority < lowest_prio->dynamicPriority ||
                aux->dynamicPriority == lowest_prio->dynamicPriority && 
                aux->staticPriority < lowest_prio->staticPriority) {
                scheduler_aging(lowest_prio);
                lowest_prio = aux;
            } else {
                scheduler_aging(aux);
            }
            aux = aux->next;
        }

        // reajuste da prioridade da tarefa escolhida
        lowest_prio->dynamicPriority = task_getprio(lowest_prio);

        return lowest_prio;
    }
    return NULL;
}


