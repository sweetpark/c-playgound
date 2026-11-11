# HINT1
`struct sigaction sa; memset(&sa,0,sizeof sa); sa.sa_handler = on_signal; sigemptyset(&sa.sa_mask); sa.sa_flags = SA_RESTART; sigaction(signo,&sa,NULL)`. 전역은 `static volatile sig_atomic_t`.
