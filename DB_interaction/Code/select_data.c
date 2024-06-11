#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = ""; // Replace with your MySQL password
    const char *database = "c_crud";

    // Initialize the MySQL connection
    conn = mysql_init(NULL);

    // Connect to the database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Execute the query
    if (mysql_query(conn, "SELECT * FROM ParentGuardian")) {
        fprintf(stderr, "SELECT * failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    res = mysql_store_result(conn);

    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Print the results
    printf("ID\tFirst Name\tLast Name\tAddress\t\tRelationship\n");
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s\t%s\t\t%s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4]);
    }

    // Free the result
    mysql_free_result(res);

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
