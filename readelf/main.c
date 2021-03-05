#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>

#define DUMP(title, data) printf("  %-34s %u(0x%x)\n", title, (uint32_t)data, (uint32_t)data);

static void dump_magic(const unsigned char *e_ident) {
    printf("  MAGIC:   ");
    for (int i = 0; i < EI_NIDENT; ++i) {
        printf("%02x ", e_ident[i]);
    }
    printf("\n");
}

static void dump_ehdr(Elf64_Ehdr *ehdr) {
    dump_magic(ehdr->e_ident);
    DUMP("Class:",                             ehdr->e_ident[EI_CLASS]);
    DUMP("Data:",                              ehdr->e_ident[EI_DATA]);
    DUMP("Version:",                           ehdr->e_ident[EI_VERSION]);
    DUMP("OS/ABI:",                            ehdr->e_ident[EI_OSABI]);
    DUMP("ABI Version:",                       ehdr->e_ident[EI_ABIVERSION]);
    DUMP("Type:",                              ehdr->e_type);
    DUMP("Machine:",                           ehdr->e_machine);
    DUMP("Version:",                           ehdr->e_version);
    DUMP("Entry point address:",               ehdr->e_entry);
    DUMP("Start of program headers:",          ehdr->e_phoff);
    DUMP("Start of section headers:",          ehdr->e_shoff);
    DUMP("Flags:",                             ehdr->e_flags);
    DUMP("Size of this header:",               ehdr->e_ehsize);
    DUMP("Size of program headers:",           ehdr->e_phentsize);
    DUMP("Number of program headers:",         ehdr->e_phnum);
    DUMP("Size of section headers:",           ehdr->e_shentsize);
    DUMP("Number of section headers:",         ehdr->e_shnum);
    DUMP("Section header string table index:", ehdr->e_shstrndx);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s elf-file(s)\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        struct stat stbuf;
        int fd = open(argv[i], O_RDONLY);
        FILE *fp = fdopen(fd, "rb");

        printf("\nFile: %s\n", argv[i]);

        fstat(fd, &stbuf);
        unsigned char buf[stbuf.st_size];
        fread(buf, 1, sizeof(buf), fp);
        fclose(fp);

        printf("ELF HEADER(`readelf -h`):\n");
        Elf64_Ehdr *ehdr = (Elf64_Ehdr *)buf;
        dump_ehdr(ehdr);
    }

    return 0;
}

