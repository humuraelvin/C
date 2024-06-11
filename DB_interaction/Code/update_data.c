#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

int main() {
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = ""; 
    const char *database = "c_crud";

    int id;
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
    printf("Enter the ID of the record to update: ");
    scanf("%d", &id);
    printf("Enter new first name: ");
    scanf("%254s", fname);
    printf("Enter new last name: ");
    scanf("%254s", lname);
    printf("Enter new address: ");
    scanf("%254s", address);
    printf("Enter new relationship with student: ");
    scanf("%254s", relationship_with_student);

    // Prepare the query
    char query[1024];
    snprintf(query, sizeof(query),
             "UPDATE ParentGuardian SET fname='%s', lname='%s', address='%s', relationship_with_student='%s' WHERE id=%d",
             fname, lname, address, relationship_with_student, id);

    // Execute the query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "UPDATE failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    printf("Data updated successfully!\n");

    // Close the connection
    mysql_close(conn);

    return EXIT_SUCCESS;
}
