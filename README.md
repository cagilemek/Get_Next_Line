# get_next_line
 
*This project has been created as part of the 42 curriculum by ckurtul.*
 
## Description
 
**get_next_line** is a C function that reads and returns a single line from a file descriptor every time it is called. The returned line includes the terminating `\n` character if it exists, otherwise it returns the remaining content until EOF.
 
This project focuses on:
 
- Buffered file reading with `read()`
- Persistent memory management using a `static` variable
- Dynamic memory allocation
- Preventing memory leaks and byte loss between function calls
 
The core idea of the project is storing unread data inside a static variable called `stash`, allowing the function to continue exactly where the previous call stopped.
 
---
 
# Compilation
 
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c main.c
```
 
You can test different buffer sizes:
 
```bash
-D BUFFER_SIZE=1
-D BUFFER_SIZE=9999
```
 
The function works correctly regardless of the buffer size.
 
---
 
# Usage
 
```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("example.txt", O_RDONLY);
	if (fd < 0)
		return (1);

	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```
 
---
 
# Project Structure
 
## `get_next_line.c`
 
| Function | Description |
|---|---|
| `get_next_line` | Main function that manages the reading process and returns one line at a time. |
| `readfd` | Reads from the file descriptor and appends data into the stash until `\n` or EOF is found. |
| `cut_line` | Extracts the current line from the stash. |
| `clean_stash` | Removes the extracted line from the stash and keeps the remaining content. |
| `line_len` | Calculates the length of the next line including `\n`. |
 
---
 
## `get_next_line_utils.c`
 
| Function | Description |
|---|---|
| `ft_strlen` | Returns the length of a string. |
| `ft_strchr` | Searches for a character inside a string. |
| `ft_strdup` | Creates a duplicate of a string using dynamic allocation. |
| `ft_strjoin` | Concatenates two strings and frees the first string to avoid memory leaks. |
 
---
 
## `get_next_line.h`
 
Contains:
 
- Function prototypes
- Required libraries
- `BUFFER_SIZE` macro definition
 
```c
#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *s1, const char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
char	*get_next_line(int fd);

#endif
```
 
---
 
# How It Works
 
## 1. Static Variable Persistence
 
```c
static char *stash;
```
 
The static variable stores leftover data between function calls.
 
Example:
 
```text
First call:
"Hello\nNext"

Returned:
"Hello\n"

stash becomes:
"Next"
```
 
This allows `get_next_line` to continue reading without losing bytes.
 
---
 
## 2. Reading From File Descriptor
 
```text
read()
   ↓
buffer
   ↓
ft_strjoin()
   ↓
stash
```
 
The `readfd` function:
 
- Allocates a temporary buffer
- Reads `BUFFER_SIZE` bytes
- Appends them into `stash`
- Stops when:
  - `\n` is found
  - or EOF is reached
 
---
 
## 3. Extracting the Line
 
`cut_line()` copies:
 
```text
stash = "hello world\nnext line"
```
 
into:
 
```text
line = "hello world\n"
```
 
---
 
## 4. Cleaning the Stash
 
After extracting the line:
 
```text
stash before:
"hello world\nnext line"
```
 
becomes:
 
```text
stash after:
"next line"
```
 
This remaining content is preserved for the next function call.
 
---
 
# Memory Management
 
| Situation | Action |
|---|---|
| `ft_strjoin` | Frees old stash after joining |
| `read()` error | Frees buffer and stash |
| Empty stash at EOF | Frees stash and returns `NULL` |
| Allocation failure | Returns `NULL` safely |
 
---
 
# Technical Notes
 
- Uses only allowed functions:
  - `read`
  - `malloc`
  - `free`
- Fully compatible with:
  - very small `BUFFER_SIZE`
  - very large `BUFFER_SIZE`
- Handles:
  - files
  - stdin
  - multiple line lengths
  - EOF without newline
 
---
 
# Example
 
Input file:
 
```text
Hello
World
42
```
 
Program output:
 
```text
Hello
World
42
```
 
---
 
# Resources
 
- `man 2 read`
- `man 3 malloc`
- POSIX file descriptor documentation
- 42 School get_next_line subject
 
---
 
# AI Usage & Academic Integrity
 
AI tools were used only as educational assistance for:
 
- Understanding stash management
- Discussing edge cases
- Debugging memory handling
- Improving README documentation
 
All source code was written, tested, and validated manually by ckurtul.