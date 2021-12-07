#include "header.h"

void print_error(int err, char *msg) {
    if (err == -1) {
        // If Error Print Error Message and Errno
        printf("Error: %s\n", msg);
        printf("%s\n", strerror(errno));
    }
}

int main() {
    // Accessing Semaphore
    int semd = semget(SEM_KEY, 1, 0);
    if (semd == -1) {
        print_error(semd, "Unable To Access Semaphore");
        return -1;
    }

    // Creating Helper Struct
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;

    // Downing Semaphore
    int err1 = semop(semd, &sb, 1);
    if (err1 == -1) {
        print_error(err1, "Unable To Down Semaphore");
        return -1;
    }

    // Accessing Shared Memory
    int shmd = shmget(SHM_KEY, 1, 0);
    if (shmd == -1) {
        print_error(shmd, "Unable To Access Shared Memory");
        return -1;
    }

    // Attaching Segment To Data Variable
    int *data;
    data = shmat(shmd, 0, 0);
    if (data == (void *)-1) {
        print_error(-1, "Unable To Attach Segment To Variable");
        return -1;
    }

    // Opening File
    int fd = open(FILE_NAME, O_RDWR);
    if (fd == -1) {
        print_error(fd, "Unable To Open Story File");
        return -1;
    }

    // Repositioning File Descriptor
    int err2 = lseek(fd, -(*data), SEEK_END);
    if (err2 == -1) {
        print_error(err2, "Unable To Find Last Line");
        return -1;
    }

    // Reading Last Line And Printing
    char *buff1 = calloc(*data+1, sizeof(char));
    int err3 = read(fd, buff1, *data);
    if (err3 == -1) {
        print_error(err3, "Unable To Read Last LIne");
        return -1;
    }
    printf("Last Line: %s\n", buff1);

    // Obtaining New Line
    printf("New Line: ");
    char *buff2 = calloc(BUFF_SIZE, sizeof(char));
    char *ret = fgets(buff2, BUFF_SIZE, stdin);
    if (ret == NULL) {
        print_error(-1, "Unable To Get User Input");
        return -1;
    }
    *data = strlen(buff2);

    // Writing To File
    int err4 = write(fd, buff2, *data);
    if (err4 == -1) {
        print_error(err4, "Unable To Write To File");
        return -1;
    }

    // Detaching Segment
    int err5 = shmdt(data);
    if (err5 == -1) {
        print_error(err5, "Unable To Detach Segment From Variable");
        return -1;
    }

    // Closing File
    int err6 = close(fd);
    if (err6 == -1) {
        print_error(err6, "Unable To Close Story File");
        return -1;
    }

    // Upping Semaphore
    sb.sem_op = 1;
    int err7 = semop(semd, &sb, 1);
    if (err7 == -1) {
        print_error(err7, "Unable To Up Semaphore");
        return -1;
    }

    // Freeing Memory And Exiting Function
    free(buff1);
    free(buff2);
    return 0;
}
