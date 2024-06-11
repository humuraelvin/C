#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = ""; 
    const char *database = "c_crud";

    int id;

    // Initialize the MySQL connection
    conn = mysql_init(NULL);

    // Connect to the database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Get user input
    printf("Enter the ID of the record to delete: ");
    scanf("%d", &id);

    // Prepare the query
    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM ParentGuardian WHERE id=%d", id);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "DELETE failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    printf("Data deleted successfully!\n");

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
