*This project has been created as part of the 42 curriculum by ckurtul.*

# get_next_line

## Description

**get_next_line** is a C function that reads and returns a single line from a file descriptor every time it is called. The returned line includes the terminating `\n` character if it exists, otherwise it returns the remaining content until EOF.

The goal of this project is to implement a function that can be called in a loop to read a text file line by line, without reading the whole file at once. It introduces key concepts such as buffered I/O, static variables for persistent state, and careful dynamic memory management.

The core idea is storing unread data inside a static variable called `stash`, allowing the function to continue exactly where the previous call stopped.

---

## Instructions

### Compilation

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

### Usage

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

## Algorithm

### Why a Static Stash?

The central challenge of `get_next_line` is that `read()` does not know where a line ends — it reads raw bytes in fixed-size chunks (`BUFFER_SIZE`). A single `read()` call may return half a line, one full line, or several lines at once depending on the buffer size and file content.

To handle this, a `static char *stash` is used to persist leftover data between calls. Every time `get_next_line` is called, it:

1. Reads from the file descriptor in `BUFFER_SIZE` chunks via `readfd`, appending each chunk to `stash` using `append_stash`.
2. Stops reading when a `\n` is found in `stash` or EOF is reached.
3. Extracts the first line from `stash` using `cut_line` (which internally uses `ft_substr`).
4. Cleans `stash` via `clean_stash`, keeping only the content after the extracted line for the next call.
5. Returns the extracted line to the caller.

### Why ft_substr Instead of ft_strjoin for Line Extraction?

`ft_substr` was chosen for `cut_line` because it allows directly slicing a known portion of `stash` without needing to allocate and copy an intermediate result. Given that `line_len` already calculates the exact length of the current line, `ft_substr` produces the result in a single allocation.

### Memory Management Strategy

Each `append_stash` call frees the old `stash` after joining it with the new buffer, so there is never more than one live copy of the accumulated data. On error or EOF, `stash` is freed and set to `NULL` to prevent leaks across multiple calls.

### Example Walkthrough

```text
BUFFER_SIZE = 5, file content: "Hello\nWorld"

read() call 1 → buffer = "Hello"   stash = "Hello"
read() call 2 → buffer = "\nWorl"  stash = "Hello\nWorl"   → \n found, stop reading
cut_line()    → line   = "Hello\n"
clean_stash() → stash  = "Worl"

Next get_next_line() call:
read() call 3 → buffer = "d"       stash = "World"
EOF reached   → stop reading
cut_line()    → line   = "World"
clean_stash() → stash  = NULL
```

---

## Project Structure

### `get_next_line.c`

| Function | Description |
|---|---|
| `get_next_line` | Main function that manages the reading process and returns one line at a time. |
| `readfd` | Reads from the file descriptor and appends data into the stash until `\n` or EOF is found. |
| `cut_line` | Extracts the current line from the stash using `ft_substr`. |
| `clean_stash` | Removes the extracted line from the stash and keeps the remaining content. |
| `line_len` | Calculates the length of the next line including `\n`. |

### `get_next_line_utils.c`

| Function | Description |
|---|---|
| `ft_strlen` | Returns the length of a string. |
| `ft_strchr` | Searches for a character inside a string. |
| `ft_strdup` | Creates a duplicate of a string using dynamic allocation. |
| `ft_substr` | Extracts a substring from a string using start index and length. |
| `append_stash` | Concatenates the buffer into the stash and frees the old stash to avoid memory leaks. |

### `get_next_line.h`

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
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
char	*append_stash(char *stash, char *buffer);
char	*get_next_line(int fd);

#endif
```

---

## Resources

- `man 2 read` — POSIX read() system call documentation
- `man 3 malloc` — Dynamic memory allocation reference
- 42 School get_next_line subject

### AI Usage

AI tools were used only as educational assistance for:

- Understanding stash management between calls
- Discussing edge cases
- Improving README documentation

All source code was written, tested, and validated manually by ckurtul.