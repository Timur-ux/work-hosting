#include <sys/wait.h>
#include "work.hpp"
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <zmq.h>

void startJob(Work work) {
  int pipefd[2];
  if (pipe(pipefd) == -1)
    throw std::runtime_error("Can't create a pipe");

  int pid = fork();
  if (pid == -1)
    throw std::runtime_error("Can't do fork");

  if (pid == 0) { // child process
    close(pipefd[1]);
		dup2(pipefd[0], 0);
    execlp("./scripts/checkWork.sh", "./scripts/checkWork.sh", NULL);
		perror("Failed to execlp!");
		return;
  }

	close(pipefd[0]);
	dup2(pipefd[1], 1);
	printf("%s\n", work.typeAsString());
	printf("%d\n", work.number());
	printf("%s\n", work.gvName().c_str());
	fflush(stdout);
	close(pipefd[1]);
}

int main(int argc, const char *argw[]) {
	Work work{Work::Type::LR, 12, "gerik"};
	startJob(std::move(work));

	wait(NULL);
	return 0;
}
