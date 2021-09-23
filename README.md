# AskMe_project
A small version of **Asfkm**, which is a file based console application built with c++ using VSCode as an IDE.
## What it does
It allows users to :
- Sign up
- Login
- Logout
- Ask questions to each other, it can be anonymously, and questions can be in a thread.
- Answer questions.
- Print received questions.
- Print sent questions.
- Print feed.
- List users.
- Delete received questions.
## What's used in it
- \<fstream> to manage files (read/write) using the input/output file stream.
- \<sstream> as loading the database -reading from files- can be easier using the string stream.
- \<string> & \<map> are the main data structures in the project.
- \<utility> to use the pair data structure.
- structs , namespaces and functions.
## Important notes to use the app
- This project is **file based**, so make sure that you have downloaded the **-users-** and **-questions-** files as they represent the database for this project.
- **You must close the program from the menu that will be displayed while you're running it otherwise the database will NOT be updated.** Because database will be updated only through the destructor and it'll not be called if you closed the application in a wrong way.
##  
That's all. you can download the source code, run it, test it, and give me your feedback.
