# Path_maker Language

Language : C   
main.c contains Turkish comments.

## Description
Path_maker is a basic scripting language for creating directory trees. (Rules of the language are below) Write a basic interpreter for Path_maker

<br />

### Rules of the Language

#### 1. Data Types

Path is the only data type. Path constants are relative directory path expressions written in the form:
```
<dir1/dir2/dir3>  // where dir1, dir2 and dir3 are directory names
```
* No file names are of any concern (just directories)
* Directory names start with a letter (upper or lower case) and are made of any combination of letters, digits and underscore characters (only). (Punctuation characters are not allowed. Blank characters are not allowed either.) Directory names are not case sensitive so <AA> and <aa> are basically the same. (since that is the policy of most operating systems)
* Operator “*” can be used instead of a directory name, and it indicates parent directory. It can be used multiple times before any other directory name.
```
<*/*/mydirectory> indicates that one should move up (to parent) twiceand then choose mydirectory.
```
* Operator * can only be used at the beginning of path expressions. <hi/*/there> is not allowed.
* Operator / cannot be used at the beginning or the end of any path. So </hi/there> is not allowed. Neither is <hi/there/> allowed.
* Blanks in a path expression are ignored (unless they exist in a directory name (which is not allowed)) so < * /* / mydirectory> is OK.

<br/>

#### 2. Basic Commands

The only two basic commands are “make” and “go”. 

###### Make :
  ```
  make <myDirectoryPath>;
  make <*/project1/data> goes up once and then creates a directory called project1 and then creates another one called “data” inside it.
  ```
  + It simply creates the directories in the myDirectoryPath. If the path already exists it does nothing (but gives a warning message). If the path partially exists, it completes the path.
  + “make” does not change the current (working) directory. (This is what we do with go command)

###### Go :
  ```
   go <myPathExpression>;
   ```
   + Go command simply changes the current directory
   + If the path does not exist, go does nothing. (Gives an error message but does not exit the execusion) It does not partially follow a path. Partial existence of any path is considered as inexistence. 
   
<br/>

#### 3. Control Structures
There is an “if” clause and a similar “ifnot” clause
```
if <path_expression> command
```
+ This is the basic form of this clause where command can be a basic command or a block. “if” clause executes the command if from the current directory the path <path_expression> exists
+ “if” does not change the current directory
```
ifnot <path_expression> command
```
+ “ifnot” clause has the exact same structure but operates if the path <path_expression> does not exist.
   
<br/>

#### 4. Blocks
A command can be a basic command (“make” or “go”) but it can also be a block. A block is a list of lines of code enclosed in { } set brackets. Blocks may also be nested in one another.
   
<br/>

#### 5. End of line Character
Only “make” and “go” commands require an end of line character and it is ‘;’ (semi-colon)
   
<br/>

#### 6. Keywords
Keywords are case sensitive and all are lowercase. They are: make, go, if, ifnot
   
<br/>

#### 7. Symbols
< , > , { , } , / , *
   
<br/>

#### Code Example
```
if <*>
{ go <*>; make <data/doctors>;
 if <user/ahmet> go <path_expression>;
}
```
* The project contains three code example file which are ex1.pmk , ex2.pmk , ex3.pmk

