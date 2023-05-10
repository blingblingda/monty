#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * struct stack_s - doubly linked list representation of a stack (or queue)
 * @n: integer
 * @prev: points to the previous element of the stack (or queue)
 * @next: points to the next element of the stack (or queue)
 *
 * Description: doubly linked list node structure
 * for stack, queues, LIFO, FIFO
 */
typedef struct stack_s
{
        int n;
        struct stack_s *prev;
        struct stack_s *next;
} stack_c;

/**
 * struct instruction_s - opcode and its function
 * @opcode: the opcode
 * @f: function to handle the opcode
 *
 * Description: opcode and its function
 * for stack, queues, LIFO, FIFO
 */
typedef struct instruction_s
{
        char *opcode;
        void (*f)(stack_c **stack, unsigned int line_number);
} instruction_t;


int is_integer(char *str)
{
    int i;

    while (str[i] != '\0')
    {
        if (isdigit(str[i]) == 0)
        {
            return (-1);
        }
        i = i + 1;
    }

    return (0);
}

void push(stack_c **stack, __attribute__((unused)) unsigned int line_number)
{
    char *token;
    char *delim = " \n";
    int integer_check;
    int num;
    stack_c *new_node;

    // if (stack == NULL)
    // {
    //     fprintf(stderr, "No stack found\n");
    //     exit(EXIT_FAILURE);
    // }

    token = strtok(NULL, delim);
    if (token == NULL)
    {
        fprintf(stderr, "Usage: push integer\n");
        exit(EXIT_FAILURE);      
    }

    // printf("%s\n",token);
    integer_check = is_integer(token);
    if (integer_check == -1)
    {
        fprintf(stderr, "Usage: push integer check\n");
        exit(EXIT_FAILURE);   
    }
    else
    {
        num = atoi(token);
        new_node = malloc(sizeof(stack_c));
        if (new_node == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            exit(EXIT_FAILURE);           
        }
        new_node->n = num;
        new_node->prev = NULL;
        if (*stack != NULL) //why
        {
            new_node->next = *stack;
            (*stack)->prev = new_node;
        }
        *stack = new_node;
    }
}

void pall(stack_c **stack, __attribute__((unused)) unsigned int line_number)
{
    stack_c *temp;

    temp = *stack;
    while (temp != NULL)
    {
        printf("%d\n", temp->n);
        temp = temp->next;
    }

}

/**
* get_instruction_fn - find function to match opcode
* @str: opcode to check
*
* Return: success pointer to function, opcode not found: NULL
*/
void (*get_instruction_fn(char *str))(stack_c **stack, unsigned int line_number)
{
	instruction_t opcodes[] = {
		{"push", push},
		{"pall", pall},
		{NULL, NULL}
	};
	int i;

	i = 0;
	while (opcodes[i].opcode != NULL && str != 0)
	{
		if (strcmp(opcodes[i].opcode, str) == 0)
		{
			return (opcodes[i].f);
		}
		i = i + 1;
	}

	return (NULL);
}


/**
 * main - entry point
 * @argc: a count of the arguments passed to the program.
 * @argv: an array of pointers to the strings which are those arguments.
 * Return: always 0.
 */

 int main(int argc, __attribute__((unused)) char **argv)
 {
    FILE *fp;
    char *buf = NULL;
    size_t bufsize = 0;
    size_t read = 0;
    char *token;
    char *delim = " \n"; // why?
    unsigned int line_number = 0;
    stack_c **stack = NULL;; //why ????

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    read = getline(&buf, &bufsize, fp);
    while (read != -1)
    {
        line_number = line_number + 1;
        token = strtok(buf, delim);
        if (token == NULL)
        {
            read = getline(&buf, &bufsize, fp); // how could i know that it keep reading the next line?
        }
        else
        {
            if (get_instruction_fn(token) == NULL)
            {
                fprintf(stderr, "L%d: unknown instruction %s\n", line_number, token);
                exit(EXIT_FAILURE);
            }
            get_instruction_fn(token)(stack, line_number);
        }
    }

    fclose(fp);
    free(buf);
    return (0);
 }
