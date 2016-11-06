#define _GNU_SOURCE

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/types.h"


#define NUM_OF_BYTES  (100)
typedef enum { FALSE, TRUE } bool;
typedef enum {LABEL_NONE, LABEL_DATA, LABEL_CODE} label_type;
typedef enum {INST_NONE, INST_DATA, INST_STRING, INST_ENTRY, INST_EXTERN} instruction_type;


char *opcodes = 
{  
"mov"
"cmp"
"add"
"sub"
"not"
"clr"
"lea"
"inc"
"dec"
"jmp"
"bne"
"red"
"prn"
"jsr"
"rts"
"stop"
};


/* Array holding the code sec */
int code_arr[NUM_OF_BYTES];
int IC = 0;

/* Array holding the data */
int data_arr[NUM_OF_BYTES];
int DC = 0;
    


struct LABEL
{
   char *label;
   int value; 
   label_type type;   
} labels_table[NUM_OF_BYTES];

struct LABELS_MAP
{
   struct LABEL labels_table[NUM_OF_BYTES];
   int itr;   
} labels_map;


bool is_instruction(char *pch)
{
   bool result = FALSE;
   
   if (pch[0] == '.')
   {
      result = TRUE;
      
   }
   
   return result;   
}


instruction_type get_instruction_type(char *pch)
{
   instruction_type result = INST_NONE;
   
   if (0 == strcmp(&pch[1], "data"))
   {
      result = INST_DATA;
   }
   else if (0 == strcmp(&pch[1], "string"))
   {
      result = INST_STRING;
   }
   else if (0 == strcmp(&pch[1], "entry"))
   {
      result = INST_ENTRY;
   }
   else if (0 == strcmp(&pch[1], "extern"))
   {
      result = INST_EXTERN;
   }

      
   return result;
   
}

void store_label(char *label, label_type type) 
{
   int table_itr = 0;
   
   while (labels_map.labels_table[table_itr].label != NULL) 
   {
      if (0 == strncmp(labels_map.labels_table[table_itr].label, label, strlen(label)))
      {        
         printf("Label(%s) already exist\n", label);
         exit(EXIT_FAILURE);
      }   

      table_itr++;      
   }
   
   

   labels_map.labels_table[table_itr].label = malloc(strlen(label) * sizeof(char));
   memcpy(labels_map.labels_table[table_itr].label, label, strlen(label) * sizeof(char));
   
   labels_map.labels_table[table_itr].value = DC;
   labels_map.labels_table[table_itr].type = type;

   labels_map.itr = table_itr;
   
}


void perform_7(char *pch, instruction_type i_type)
{
   char *curr_word;
   int str_itr;
   
   if (INST_DATA == i_type)
   {   
      curr_word = strtok(pch, " ");

      while (curr_word != NULL)
      {
         if (curr_word[0] == '+' || curr_word[0] == '-')
         {
            /* TODO - how can check that this is a valid number? */
            /* Does atoi() function also recognize the +/- signs? */
            data_arr[DC++] = atoi(&curr_word[1]);         
         }
         else
         {
            data_arr[DC++] = atoi(&curr_word[0]);         
         }
      

         curr_word = strtok(pch, " ");
      }
   }
   else if (INST_STRING == i_type)
   {
      if (pch[0] != '"')
      {
         printf("Invalid command...n"); /* Maybe add the line of the failure in the .as file */
         exit(EXIT_FAILURE);         
      }  

      str_itr = 0;
      
      while (pch[str_itr] != '\0')
      {         
         if (pch[str_itr] != '"')
         {
            data_arr[DC++] = pch[str_itr++];
         }      
         else
         {
            break;          
         }       
      }

      if (pch[str_itr] == '\0')
      {
         printf("Invalid command, missing parentheses at the end of string...n"); /* Maybe add the line of the failure in the .as file */   
         exit(EXIT_FAILURE);         
      }      
      
      str_itr++;
      
      /* Check trailing charachters are only blanking spaces */
      while (pch[str_itr] != '\0')
      {
         if (pch[str_itr] != ' ')
         {
            printf("Invalid command, non-space charchter after string command...n"); /* Maybe add the line of the failure in the .as file */   
            exit(EXIT_FAILURE);         
         }      
      }      
   }
   else 
   {
      /* TODO - return error... */
   }   
}

bool label_check(char* pch) 
{
   bool result = TRUE;
   
   size_t len = strlen(pch);
   
   if (pch[len-1] != ':') 
   {
      result = FALSE;      
   }
   
   return result;
   
   /* TODO - add all the condition for valid label */

}

bool data_or_string_check(char* pch) 
{
   bool result = FALSE;
   
   
   if (pch[0] == '.') 
   {
      if (0 == strncmp(&pch[1], "data", strlen("data")))
      {
         result = TRUE;
      }
      else if (0 == strncmp(&pch[1], "string", strlen("string")))
      {
         result = TRUE;
      }
   }
   
   return result;
}



int main() 
{
   

   FILE * fp;
   char * line = NULL;
   size_t len = 0;
   ssize_t read;
   char * pch;
   char * first_word;
    
   bool is_label;
   instruction_type i_type;
    
   printf("hellow world!!!\n");


   memset(&labels_map, 0, sizeof(struct LABELS_MAP));
   

    
   fp = fopen("../files/ps.as", "r");
   if (fp == NULL)
      exit(EXIT_FAILURE);

   /* Read line */
   while ((read = getline(&line, &len, fp)) != -1) {
/*        printf("Retrieved line of length %zu :\n", read); */
      /* printf("%s", line);*/

      /* printf ("Splitting string \"%s\" into tokens:\n", line); */

      /* Remove the traing CR/LF */
      line[strcspn(line, "\r\n")] = 0;

      /* Skip comment line or empty line */
      if ((line[0] == ';') || (line[0] == 0))
         continue;
      
      /* TODO - need to skip empty lines */      

      is_label = FALSE;
       
      /* Get first word */
      pch = strtok(line, " ");
      first_word = pch;
       
      /* Mark label flag */
      if (TRUE == label_check(pch)) 
      {
         is_label = TRUE;          
      }

      if (is_label) 
      {          
         /* Get second word */
         pch = strtok (NULL, " ");
          
         /* Check validity */
         if (pch == NULL) 
         {
             
            /* TODO - what shall we do here? */
         }
      }
       
      if (is_instruction(pch)) /* pch holds the string that is suspect to be the instruction type */
      {
         i_type = get_instruction_type(pch);

         if (i_type == INST_DATA || i_type == INST_STRING) 
         {             
            if (is_label == TRUE) 
            {
                
               store_label(first_word, LABEL_DATA);                
            }
             
            /* TODO - need to split into two cases of DATA and STRING labels */
            perform_7(strtok (NULL, " "), i_type);             
         }                 
         else if (i_type == INST_ENTRY) 
         {
            
         }
         else if (i_type == INST_EXTERN)
         {
            /* perform_9();            */
         }          
         else 
         {
            /* TODO - need to add error here... */
         }          
      }    

      else /* action command */
      {
         if (is_label == TRUE) 
         {
            
            store_label(first_word, LABEL_CODE);                
         }
         
         /*  parse_action_command();     */
      }
   }
   
   


   fclose(fp);
   if (line)
      free(line);
   exit(EXIT_SUCCESS);
   
   

}
