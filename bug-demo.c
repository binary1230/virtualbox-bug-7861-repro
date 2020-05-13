/* Repro code for https://www.virtualbox.org/ticket/8761
 * This is in C code form to minimize the number of system calls to reproduce
 * It appears the issue is triggered after the rename() call */


#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int error_count = 0; // hacky but whatever

void setup(const char* basepath, const char* file_contents) {
    // 1) create a file in a new directory so it looks like this:  "<basepath>/subdir/file.txt"
    // 2) write file_contents into that file

    char subdirpath[100];
    snprintf(subdirpath, sizeof subdirpath, "%s/subdir", basepath);

    char filepath[100];
    snprintf(filepath, sizeof filepath, "%s/file.txt", subdirpath);

    mkdir(basepath, 0777);
    mkdir(subdirpath, 0777);

    int fd = open(filepath, O_WRONLY | O_CREAT, 0644);

    write(fd, file_contents, strlen(file_contents));

    close(fd);
}

int ensure_file_contents_valid(const char* filepath, const char* what_contents_should_be) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        printf("ERROR! Couldn't open file for reading: %s, reason was: ", filepath);
        perror("");
        error_count++;
        return -1;
    }

    // this is not a great/sane/robust way to do this, but it works for us here
    char buf[1000];
    buf[0] = 0;
    ssize_t count = read(fd, buf, sizeof(buf));
    buf[count] = 0;

    if (count <= 0) {
        printf("ERROR: nothing read from file '%s'?\n", filepath);
        close(fd);
        error_count++;
        return -2;
    }

    close(fd);

    if (strcmp(buf, what_contents_should_be) != 0) {
        printf("ERROR: file '%s' contents differ. expected: '%s', actual: '%s'\n", filepath, buf, what_contents_should_be);
        error_count++;
        return -3;
    }

    return 0;
}

// super quick n dirty echo output to a file
void echo(const char* contents, const char* filename) {
      int fd = open(filename, O_WRONLY | O_CREAT, 0644);
      write(fd, contents, strlen(contents));
      close(fd);
}

void print_pid() {
	pid_t pid = getpid();
	printf("pid: %lun\n", pid);
}

void clear_inode_cache() {
    echo("2", "/proc/sys/vm/drop_caches"); // kernel debug break
}

int main() {
    int kernel_stuff = 0;

    if (kernel_stuff) {
      print_pid();
      sleep(5);

      // IGNORE
      echo("g", "/proc/sysrq-trigger"); // kernel debug break
   }

    // unrelated: set stdout to flush for better error messages
    setvbuf (stdout, NULL, _IONBF, BUFSIZ);

    const char* file_conents_A = "file contents: (1)";
    const char* file_conents_B = "file contents: (2)";

    setup("a0", file_conents_A);
    setup("a1", file_conents_B);

    printf("----part1 test------\n");
    ensure_file_contents_valid("a0/subdir/file.txt", file_conents_A);
    ensure_file_contents_valid("a1/subdir/file.txt", file_conents_B);

    // clear_inode_cache();

    printf("-----renaming a1 to a2, a0 to a1 ------\n");
    rename("a1", "a2");
    rename("a0", "a1");

    printf("----part2 test------\n");
    ensure_file_contents_valid("a1/subdir/file.txt", file_conents_A);
    ensure_file_contents_valid("a2/subdir/file.txt", file_conents_B);

    // if cache cleared, everything past this point fails
    // clear_inode_cache();
    //ensure_file_contents_valid("a1/subdir/file.txt", file_conents_A);
    //ensure_file_contents_valid("a2/subdir/file.txt", file_conents_B);

    printf("----deleting------\n");
    unlink("a1/subdir/file.txt");

    printf("----part3 test (expected: passes, actual: fail) ------\n");
    ensure_file_contents_valid("a2/subdir/file.txt", file_conents_B);


    int do_final_test=0;
    if (do_final_test) {
      clear_inode_cache();
      printf("----part4 test------ (expected/actual = pass after clearning inode cache)\n");
      ensure_file_contents_valid("a2/subdir/file.txt", file_conents_B);
    }

    if (error_count > 0) {
        printf("\n\n\n\nOVERALL: FAILURES HAPPENED!\n");
        return -1;
    } else {
        printf("\n\n\n\nOVERALL: EVERYTHING IS LOOKING GOOD: PASSED!\n");
        return 0;
    }
}
