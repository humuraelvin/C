#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = ""; 
    const char *database = "c_crud";

    // Initialize the MySQL connection
    conn = mysql_init(NULL);

    // Check if the initialization was successful
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return EXIT_FAILURE;
    }

    // Connect to the database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // If connected successfully
    printf("Connection successful!\n");

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
