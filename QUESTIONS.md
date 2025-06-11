# Questions & Research Log

## 2025-06-11
### Question: How will I make my program link to the system?
- Had trouble trying to ponder on what I would do
- I'm reading up on how this all works currently
- Solution: use the `<unistd.h>` library for functions that interact with the system

### Question: How should I check for buffer overflow?
- common edge case that made me wonder
- I think that I'll make a buffer size variable to add a bound to the buffer
- Came across the `getline()` function. Learned that it resizes the buffer automatically
- Solution: use the `getline()` function dynamically resize the buffer

### Question: What data type will my parser return?
- made me lost while I was thinking about it
- I need to read up on how to do this
- Came across the`<string.h>` library
- Tried making the parser return a char pointer for tokens
- Solution: return `char**` to have an array of tokens

### Question: How will I parse the input for commands?
- anything with parsing is confusing me currently
- I am reading up on parsing in c
- Realized I needed to tokenize my input after coming across the `<string.h>` library
- Solution: use `<string.h>` library's `strtok()` function to parse input

### Question: How will I execute the input I parsed?
- I made a commands array that has the commands I parsed, but I don't know what to do with them
- I am reading up on executing in c
- Looked through the `<unistd.h>` library and found execute functions.
- Realized I needed for the commands and the c program to run simulaneously
- Solution: Use fork() and execvp() in tandem to execute commands
