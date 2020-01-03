#include "header.h"

char flag[5];
int semd, shmd, fd;
union semun su;
struct sembuf sb;

int main(int argc, char *argv[]) {
    su.val = 1;
    sb.sem_num = 0;
    sb.sem_op = -1;
    strcpy(flag, argv[1]);
    run();
    return 0;
}

void run() {
    if (strcmp(flag, "-c") == 0) { //creating
        creating();
    } else if (strcmp(flag, "-v") == 0) { //viewing
        viewing();
    } else if (strcmp(flag, "-r") == 0) { //removing
        removing();
    } else {
        printf("Invalid flag\n");
    }
}

int creating() {
    // creating semaphore
    semd = semget(SEMKEY, 1, IPC_CREAT | 0644);
    if (semd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    printf("semaphore created\n");
    semctl(semd, 0, SETVAL, su);

    // creating shared memory
    shmd = shmget(SHKEY, sizeof(char *), IPC_CREAT | 0644);
    if (shmd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    printf("shared memory created\n");

    // create file
    fd = open("semaphone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    printf("file created\n");
    close(fd);

    return 0;
}

int removing() {
    semd = semget(SEMKEY, 1, 0);
    if (semd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    //trying to get in, will block until available
    printf("trying to get in\n");
    semop(semd, &sb, 1);

    shmd = shmget(SHKEY, sizeof(char *), 0);
    if (shmd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }

    fd = open("semaphone.txt", O_RDONLY);
    if (fd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    char buff[SIZE];
    buff[0] = '\0'; // don't print weird stuff when there's nothing
    read(fd, buff, SIZE);
    if (strlen(buff) != 0) {
        *(strrchr(buff, '\n') + 1) = '\0'; // don't print weird stuff at the end
    }
    printf("The story so far:\n");
    printf("%s\n", buff);
    close(fd);

    shmctl(shmd, IPC_RMID, 0); // remove shared memory
    printf("shared memory removed\n");
    semctl(semd, IPC_RMID, 0); // remove semaphore
    printf("semaphore removed\n");
    remove("semaphne.txt"); // remove file
    printf("file removed\n");

    return 0;
}

int viewing() {
    fd = open("semaphone.txt", O_RDONLY);
    if (fd < 0) {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    char buff[SIZE];
    buff[0] = '\0';
    read(fd, buff, SIZE);
    if (strlen(buff) != 0) {
        *(strrchr(buff, '\n') + 1) = '\0';
    }
    printf("The story so far:\n");
    printf("%s\n", buff);
    close(fd);

    return 0;
}
