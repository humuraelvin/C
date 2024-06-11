#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = ""; // Replace with your MySQL password
    const char *database = "c_crud";

    char fname[255], lname[255], address[255], relationship_with_student[255];

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
    printf("Enter first name: ");
    scanf("%254s", fname);
    printf("Enter last name: ");
    scanf("%254s", lname);
    printf("Enter address: ");
    scanf("%254s", address);
    printf("Enter relationship with student: ");
    scanf("%254s", relationship_with_student);

    // Prepare the query
    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO ParentGuardian (fname, lname, address, relationship_with_student) VALUES ('%s', '%s', '%s', '%s')",
             fname, lname, address, relationship_with_student);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "INSERT INTO failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    printf("Data inserted successfully!\n");

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
