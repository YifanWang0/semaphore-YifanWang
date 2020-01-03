#include "header.h"

int shmd, semd, fd;
struct sembuf sb;

int main() {
  sb.sem_num = 0;
  sb.sem_op = -1;
  runWrite();
  return 0;
}

int runWrite() {
  printf("trying to get in\n");
  semd = semget(SEMKEY, 1, 0);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  semop(semd, &sb, 1);
  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  fd = open("semaphone.txt", O_WRONLY | O_APPEND);
  char *last = shmat(shmd, 0, 0); // get the last line from shared memory
  printf("Last addition: %s\n", last);
  char next[SIZE];
  printf("Your addition: ");
  fgets(next, SIZE, stdin); // read in the next line
  printf("\n");

  write(fd, next, strlen(next)); // write that to the file
  close(fd);
  strcpy(last, next); // the last addition becomes what was just added
  shmdt(last);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}
