#include <getopt.h>
#include <commond.h>

void print_help(char *command)
{

    printf("Sensor\n");
    printf("Use:\n %s <hostname> <puerto>", command);
    printf(" %s -h\n", command);
    printf("Opciones:\n");
    printf(" -h\t\t\tHelp, show this message\n");
    printf(" -t\t\t\tSet the time to send information");
}

int main(int argc, char const **argv)
{

    int opt, time;

    while ((opt = getopt(argc, argv, "th:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(argv[0]);
            break;
        case 't':
            time = optarg;
            break;
        case '?':
        default:
            fprintf(stderr, "Use:\n %s <hostname> <puerto>", argv[0]);
            fprintf(stderr, " %s -h\n", argv[0]);
            break;
        }
    }

    return 0;
}
