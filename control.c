#include "header.h"

/* union semun { */
/*   int              val;    /\* Value for SETVAL *\/ */
/*   struct semid_ds *buf;    /\* Buffer for IPC_STAT, IPC_SET *\/ */
/*   unsigned short  *array;  /\* Array for GETALL, SETALL *\/ */
/*   struct seminfo  *__buf;  /\* Buffer for IPC_INFO */
/*                               (Linux-specific) *\/ */
/* }; */

void print_error(int err, char *msg) {
    if (err == -1) {
        // If Error Print Error Message and Errno
        printf("Error: %s\n", msg);
        printf("%s\n", strerror(errno));
    }
}

void shm_create() {
    // Creating Shared Memory
    int shmd = shmget(SHM_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    if (shmd == -1) {
        print_error(shmd, "Unable To Create Shared Memory");
        return;
    }

    // Attaching Segment To Data Variable
    int *data;
    data = shmat(shmd, 0, 0);
    if (data == (void *)-1) {
        print_error(-1, "Unable To Attach Segment To Variable");
        return;
    }

    // Setting Default Value To 0 And Detaching Segment
    *data = 0;
    int err1 = shmdt(data);
    if (err1 == -1) {
        print_error(-1, "Unable To Detach Segment From Variable");
        return;
    }

    // Creating Semaphore
    int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
        print_error(semd, "Unable To Create Semaphore");
        return;
    }

    // Configuring Semaphore
    union semun us;
    us.val = 1;
    int err2 = semctl(semd, 0, SETVAL, us);
    if (err2 == -1) {
        print_error(err2, "Unable To Configure Semaphore");
        return;
    }

    // Opening File
    int fd = open(FILE_NAME, O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        print_error(fd, "Unable To Open Story File");
        return;
    }

    // Closing File
    int err3 = close(fd);
    if (err3 == -1) {
        print_error(err3, "Unable To Close Story File");
        return;
    }

    // Exiting Function
    return;
}

void shm_remove() {
    // Accessing Shared Memory
    int shmd = shmget(SHM_KEY, 1, 0);
    if (shmd == -1) {
        print_error(shmd, "Unable To Access Shared Memory");
        return;
    }

    // Removing Shared Memory
    int err1 = shmctl(shmd, IPC_RMID, 0);
    if (err1 == -1) {
        print_error(err1, "Unable To Remove Shared Memory");
        return;
    }

    // Accessing Semaphore
    int semd = semget(SEM_KEY, 1, 0);
    if (semd == -1) {
        print_error(semd, "Unable To Access Semaphore");
        return;
    }

    // Removing Semaphore
    int err2 = semctl(semd, IPC_RMID, 0);
    if (err2 == -1) {
        print_error(err2, "Unable To Remove Semaphore");
        return;
    }

    // Opening File
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd == -1) {
        print_error(fd, "Unable To Open Story File");
        return;
    }

    // Getting File Size
    struct stat st;
    int err3 = stat(FILE_NAME, &st);
    if (err3 == -1) {
        print_error(err3, "Unable To Get Size Of Story File");
        return;
    }

    // Reading From Story File
    char *buff = calloc(st.st_size+1, sizeof(char));
    int err4 = read(fd, buff, st.st_size);
    if (err4 == -1) {
        print_error(err4, "Unable To Read From Story File");
        return;
    }

    // Printing Contents And Closing File
    printf("%s\n", buff);
    int err5 = close(fd);
    if (err5 == -1) {
        print_error(err5, "Unable To Close Story File");
        return;
    }

    // Freeing Memory And Exiting Function
    free(buff);
    return;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Error Message For No Arguments
        printf("Program requires at most one of the following command line arguments: --create, --remove\n");
    } else {
        // Checking If Command Given Is Valid
        if (strcmp(argv[1], "--create") == 0) shm_create();
        else if (strcmp(argv[1], "--remove") == 0) shm_remove();
        else printf("Program requires at most one of the following command line arguments: --create, --remove\n");
    }

    // Exiting Function
    return 0;
}
