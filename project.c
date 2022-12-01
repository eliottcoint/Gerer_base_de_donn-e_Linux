#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include<unistd.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include "utils.h"
#include "database.h"
#include "sql.h"
#include "table.h"
#include "check.h"
#include "query_exec.h"

#define SQL_COMMAND_MAX_SIZE 1500

int main(int argc, char *argv[]) {

    // Here: check parameters with getopt
    
    char optstring[]="dl";  
    int c;
    
    char database_name[155] ="";
    char database_parent[155] = "";

    int isd = 0;

    /* Loop through the options */ 
    while( (c=getopt (argc, argv, optstring)) != EOF )
    {   
          if (c == 'd') {
            strcpy(database_name, argv[optind]);  
            isd = 1;          
          }
          else if ( 'l'){
            strcpy(database_parent, argv[optind]);  

          }    
    }
    
    if (isd == 0) {
        printf("Need a database name to continue"); 
        return 0; 
    }
    
    char * database_path = make_full_path(database_parent, database_name);
    
    create_db_directory(database_path); 
    /*Set the current directory to the datase dir*/
    chdir(database_path);
    
    char buffer[SQL_COMMAND_MAX_SIZE];

    do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
            
        // Here: parse SQL, check query, execute query
        
        /* Parse the SQL */
        query_result_t  *result;
        
        result = (query_result_t*) malloc(sizeof(query_result_t)); 
 
        result = parse(buffer, result); 
        
        if ( result != NULL ){
            
            /* Check the validity of the query */
            if ( check_query(result) == true){
                /*..and execute it */
                execute(result); 
            }
            else {
                printf("Query is not valid \n"); 
            }
        }
        else {
            printf("Error in parsing, check your input \n");
        }
        
        free(result); 
    } while (true);

    return 0;
}
