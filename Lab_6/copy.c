// Shailesh Kumar (289008), EOPSY Lab #6.

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <string.h> // memcpy


int copy_mmap(int fd_from, int fd_to)
{
    size_t file_size;
    char *source_file, *destination_file;

    if ((file_size = lseek(fd_from, 0, SEEK_END)) == -1)
        error(1, errno, "lseek() failed: unable to get file size");

    source_file = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd_from, 0);

    if (source_file == MAP_FAILED)
        error(1, errno, "mmap() failed for source file");

    if (ftruncate(fd_to, file_size) == -1)
        error(1, errno, "ftruncate() failed to truncate result file to file_size");

    destination_file = mmap(NULL, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd_to, 0);

    if (destination_file == MAP_FAILED)
        error(1, errno, "mmap() failed for destination file");

    memcpy(destination_file, source_file, file_size);

    if (msync(destination_file, file_size, MS_SYNC) == -1)
        error(1, errno, "msync() failed for destination file");

    if (munmap(source_file, file_size) == -1)
        error(1, errno, "munmap() failed for source file");

    if (munmap(destination_file, file_size) == -1)
        error(1, errno, "munmap() failed for destination file");

    return 0;
}

int copy_read_write(int fd_from, int fd_to)
{
    char inp_buffer[128],
        *out_buffer;
	ssize_t num_read,
	        num_written;

	while ((num_read = read(fd_from, inp_buffer, sizeof inp_buffer)) > 0)
	{

	    out_buffer = inp_buffer;
        if ((num_written = write(fd_to, out_buffer, num_read)) >= 0)
        {
            out_buffer += num_written;
            num_read   -= num_written;
        }
        else
            error(1, errno, "write() failed");
	}
	if (num_read < 0)
	    error(1, errno, "read() failed");
}

int main(int argc, char *argv[])
{
    int arg;
    int flagMemory = 0;


    while ((arg = getopt(argc, argv, "hm")) != -1)
    {
        if (arg == 'h')
        {
            printf("copy - copies one file into another\n\n");
            printf("USAGE:\n");
            printf("    copy [-m] <file_name> <new_file_name>\n");
            printf("    copy [-h]\n\n");
            printf("OPTIONS\n");
            printf("    -m (memory)\n");
            printf("         use memory\n");
            printf("    -h (help)\n");
            printf("         print help\n\n");
            return 0;
        }
        else if (arg == 'm')
        {
            flagMemory = 1;
            break;
        }
        else
        {
            error(1, errno, "copy: error: unexpected argument. Please refer to help\n");
            return 1;
        }
    }
    
    if (argc - optind != 2)
    {
        error(1, errno, "copy: error: expected two positional arguments\n");
        return 1;
    }

    int fd_from, fd_to;

    if ((fd_from = open(argv[optind], O_RDONLY)) == -1)
        error(1, errno, "open() has failed on input file");

    if ((fd_to = open(argv[optind + 1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
        error(1, errno, "open() has failed on output file");

    if (flagMemory)
        copy_mmap(fd_from, fd_to);
    else
        copy_read_write(fd_from, fd_to);

     if (close(fd_from))
    	error(1, errno, "close() has failed on input file");
    
    if (close(fd_to))
    	error(1, errno, "close() has failed on destination file");

    return 0;
}
