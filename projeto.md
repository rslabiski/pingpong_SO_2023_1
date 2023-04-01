# Links 
* [Make File](https://wiki.osdev.org/Makefile)
* [OSDev Wifi](https://wiki.osdev.org/Main_Page): página muito boa para o projeto

# Material de Apoio (Topicos do Moodle)
* [Everything you need to know about pointers in C](http://boredzo.org/pointers/)
* [Regras das atividades de laboratório [Atualizado em Mar/23]](https://moodle.dainf.ct.utfpr.edu.br/mod/resource/view.php?id=39565)

# Opção 01: PingPong OS - Enunciados dos Projetos A e B
* https://moodle.dainf.ct.utfpr.edu.br/mod/resource/view.php?id=39567
* [Video Explicativo](https://www.youtube.com/watch?v=K9-FcJdXVEw)

# Resumo do documento

Incrementos
* preempção
* contabilização
* semáforos
* fila de mensagens
* acesso a um disco virtual

## Projeto A: Implementação de um escalonador e Contabilização de Métricas
* Objetivos e requisitos
1. escalonador preemptivo com prioridades com envelhecimento
2. preempção por tempo
3. contabilização de métricas sobre a execução de tarefas

### Objetivo 1
*  função scheduler
   *  `task_t* scheduler(...)`
*  função para consulta de prioridade
   *  `void task_setprio(...)`
*  função para ajuste de prioridade
   *  `int task_getprio(...)` 
* observações
  * prioridades -20 a 20
  * $\alpha = -1$: o envelhecimento deve ser implementado dentro do escalonador
  * prioridade default = 0
  * programa de teste: 

### Objetivo 2
* quantum
* contador decrescente a cada tick
* Sinais UNIX
* temporizador UNIX
* implementação

# Dúvidas:
* Onde devem ser implementadas as funções do objetivo A.1?
* Onde está esse escalonador? ele deve ser uma struct?
* Deve ser implementado passo a passo primeiro?
* sobre o *signal*, seção 7 de que?
* Como funciona o temporizador?
* Deverão ser usados os sinais que foram mostrados no documento ou serão outros?
* Onde deve ser implementado cada coisa? em qual arquivo?

# Opção 02: Dashboard do Sistema Operacional - Enunciados dos Projetos A e B
* https://moodle.dainf.ct.utfpr.edu.br/mod/resource/view.php?id=48407