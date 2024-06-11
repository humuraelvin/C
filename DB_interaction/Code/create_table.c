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

    // Connect to the database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Create the table
    const char *create_table_query = "CREATE TABLE ParentGuardian("
                                     "id INT AUTO_INCREMENT PRIMARY KEY, "
                                     "fname VARCHAR(255) NOT NULL, "
                                     "lname VARCHAR(255) NOT NULL, "
                                     "address VARCHAR(255), "
                                     "relationship_with_student VARCHAR(255))";

    if (mysql_query(conn, create_table_query)) {
        fprintf(stderr, "CREATE TABLE failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    printf("Table ParentGuardian created successfully!\n");

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
