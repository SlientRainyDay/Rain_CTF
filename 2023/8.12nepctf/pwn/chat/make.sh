gcc  -o client client.c -lpthread -lsqlite3
gcc  -o serve serve.c -lpthread -lsqlite3
gcc  -o safebox safebox.c -Wall -Wextra -lpthread -lrt