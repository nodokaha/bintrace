#include <sys/ptrace.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int stat;
  if(argc < 2)
    {
      exit(1);
    }
  pid_t pid = atoi(argv[1]);
  if(ptrace(PTRACE_ATTACH, pid, NULL, NULL)<0)
    {
      perror("error");
      exit(1);
    }
  struct user_regs_struct regs;
  char com;
  fprintf(stdout, "hello!user!\n>");
  while(com != 'q')
    {
      com = fgetc(stdin);
      if(com == 's'){
	printf("command : step\n");
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	waitpid(pid, &stat, 0);
	if(WIFEXITED(stat))
	  break;
	else if(WIFSIGNALED(stat))
	  printf("%d\n", WTERMSIG(stat));
	else if(WIFSTOPPED(stat))
	  printf("%d\n", WSTOPSIG(stat));
      }else if(com == 'r'){
	printf("%lld %lld %lld %lld\n", regs.orig_rax, regs.rsi, regs.rdx, regs.rdi);
      }else if(com == '\n'){
	fprintf(stdout, ">");
      }else{
	if(WIFEXITED(stat))
	  break;
	else if(WIFSIGNALED(stat))
	  printf("%d\n", WTERMSIG(stat));
	else if(WIFSTOPPED(stat))
	  printf("%d\n", WSTOPSIG(stat));
      }
    }
  ptrace(PTRACE_DETACH, pid, NULL, NULL);
  printf("(-.-)/\n");
  return 0;
}
