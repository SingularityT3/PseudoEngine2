# Pseudocode Syntax

The following document outlines the syntax expected by the interpreter, which is largely based on the Cambridge IGCSE and A level pseudocode specifications, with some additional features.

## Comments
```
// This is a comment
```
Comments begin with '//' and continue till the end of the line

## Variable declaration
```
DECLARE <name> : <data type>
```
```
DECLARE Score : INTEGER
DECLARE Username : STRING
DECLARE IsActive : BOOLEAN
```
Available data types:
- INTEGER - whole numbers
- REAL - real numbers(decimals)
- BOOLEAN - TRUE or FALSE
- CHAR - single character enclosed in single quotes, e.g. 'A'
- STRING - sequence of characters enclosed in double quotes, e.g. "Hello"
- DATE - represents a date.

Dates are in the format dd/mm/yyyy, literals are used like `14/3/2020`

Multiple variables of the same type can be declared with
```
DECLARE <var1>, <var2>, ... : <data type>
```
```
DECLARE x, y, z : INTEGER
DECLARE FirstName, LastName : STRING
```

### Array declaration:
```
DECLARE <name> : ARRAY[<lower bound>:<upper bound>] OF <data type>
```
```
DECLARE Numbers : ARRAY[1:10] OF INTEGER
DECLARE Names : ARRAY[0:4] OF STRING
```
Note: The bounds are inclusive

Multi Dimensional arrays:
```
DECLARE <name> : ARRAY[<lb1>:<ub1>, <lb2>:<ub2>, ..., <lbn>:<ubn>] OF <data type>
```
```
DECLARE TicTacToe : ARRAY[1:3, 1:3] OF INTEGER
DECLARE Cube : ARRAY[0:1, 0:1, 0:1] OF BOOLEAN
```

### Accessing array elements
One-dimensional array:
```
MyArray[index]
```
```
DECLARE MyArray : ARRAY[1:5] OF INTEGER
MyArray[1] <- 10
OUTPUT MyArray[1]
```
Multi-dimensional array:
```
MyArray[index1, index2, ..., indexn]
```
```
DECLARE MyMatrix : ARRAY[1:2, 1:2] OF INTEGER
MyMatrix[1, 2] <- 20
OUTPUT MyMatrix[1, 2]
```

## Variable assignment
```
<variable name> <- <value>
```
```
DECLARE Age : INTEGER
Age <- 30
Name <- "Alice" // automatically defines Name as type STRING
```
Assigning to an undefined variable will define and initialise it to the value assigned

### Assigning to array element
```
MyArray[index] <- <value>
My3dArray[index1, index2, index3] <- <value>
```
```
DECLARE MyArray : ARRAY[1:5] OF INTEGER
MyArray[1] <- 10

DECLARE My3DArray : ARRAY[1:2, 1:2, 1:2] OF STRING
My3DArray[1, 1, 2] <- "Hello"
```

## Constants
```
CONSTANT <name> = <value>
```
or
```
CONSTANT <name> <- <value>
```
```
CONSTANT PI = 3.14159
CONSTANT MaxAttempts <- 5
```

## Types
### Enum definition
```
TYPE <name> = (State1, State2, State3, ...)
```
```
TYPE TrafficLightState = (Red, Yellow, Green)
```

### Pointer definition
```
TYPE <name> = ^<data type>
```
```
TYPE IntegerPointer = ^INTEGER
```

### Composite definition
```
TYPE <name>
    DECLARE <var1> : <data type 1>
    DECLARE <var2> : <data type 2>
    ...
ENDTYPE
```
```
TYPE Student
    DECLARE StudentID : INTEGER
    DECLARE StudentName : STRING
    DECLARE StudentGrade : CHAR
ENDTYPE
```

### Declaration
```
DECLARE <variable name> : <type name>
```
```
DECLARE CurrentLight : TrafficLightState
DECLARE PtrToInteger : IntegerPointer
DECLARE MyStudent : Student
```

### Assignment
```
// Enum
<enumVar> <- <state>
<enumVar> <- <enumVar> + <integer>
```
```
DECLARE CurrentLight : TrafficLightState
CurrentLight <- Red
CurrentLight <- CurrentLight + 1 // CurrentLight becomes Yellow
CurrentLight <- CurrentLight + 2 // CurrentLight becomes Red
```
```
// Pointer
<pointerVar> <- <otherPointerVar>
<pointerVar> <- ^<otherVar>
```
```
// IntegerPointer was declared above in the pointer definition section
DECLARE PtrA : IntegerPointer
DECLARE PtrB : IntegerPointer
DECLARE MyInteger : INTEGER
MyInteger <- 10
PtrA <- ^MyInteger // PtrA now points to MyInteger
PtrB <- PtrA // PtrB now also points to MyInteger
```
```
// Composite
<compositeVar> <- <otherCompositeVar>
<compositeVar>.<member> <- <value>
```
```
DECLARE Student1 : Student
DECLARE Student2 : Student
Student1.StudentID <- 123
Student1.StudentName <- "John Doe"
Student2 <- Student1
Student2.StudentGrade <- 95.5
```

### Accessing pointers
```
// Access value of ptrVar and store it in var
<var> <- <ptrVar>^
```
```
DECLARE PtrToValue : IntegerPointer
DECLARE Value : INTEGER
DECLARE AnotherValue : INTEGER
Value <- 100
PtrToValue <- ^Value
AnotherValue <- PtrToValue^ // AnotherValue becomes 100
```
```
// Assign to variable referenced by pointer
<ptrVar>^ <- <value>
```
```
PtrToValue^ <- 200 // Value now becomes 200
```

## Arithmetic operations
- \+ (Addition)
- \- (Subtraction)
- \* (Multiplication)
- / (Division)\
Result of division operator will always be of type `REAL`
- DIV - Integer division
- MOD - Modulus
```
DECLARE Num1, Num2, Result : INTEGER
DECLARE RealResult : REAL

Num1 <- 10
Num2 <- 3

Result <- Num1 + Num2       // 13
Result <- Num1 - Num2       // 7
Result <- Num1 * Num2       // 3
RealResult <- Num1 / Num2   // 3.333...

Result <- Num1 DIV Num2     // 3
Result <- Num1 MOD Num2     // 1
```
Function style syntax may be used for DIV and MOD:
```
DIV(x, y)
MOD(x, y)
```
```
Result <- DIV(Num1, Num2)   // 3
Result <- MOD(Num1, Num2)   // 1
```

## Comparison operators
- \> (Greater than)
- \>= (Greater than or equal to)
- < (Less than)
- <= (Less than or equal to)
- = (Equal to)
- <> (Not equal to)
```
DECLARE A, B : INTEGER
DECLARE IsEqual : BOOLEAN

A <- 10
B <- 20

IsEqual <- (A > B)   // FALSE
IsEqual <- (A <= B)  // TRUE
IsEqual <- (A = B)   // FALSE
IsEqual <- (A <> B)  // TRUE
```
## Logical operators
- AND
- OR
- NOT
```
DECLARE IsRaining, IsSunny, GoOutside : BOOLEAN

IsRaining <- TRUE
IsSunny <- FALSE

GoOutside <- IsSunny AND NOT IsRaining    // FALSE
GoOutside <- IsSunny OR IsRaining         // TRUE
```
## String concatenation
```
<str1> & <str2>
```
Also works with other data types, converting them to strings automatically
```
DECLARE Name, Grade : STRING
DECLARE Marks : INTEGER
Name <- "Alice"
Marks <- 95
Grade <- "A"
OUTPUT "Student: " & Name & ", Marks: " & Marks & ", Grade: " & Grade
// Outputs: Student: Alice, Marks: 95, Grade: A
```

## Selection statements
If statement:
```
IF <condition> THEN
    ...
ELSE IF <another condition> THEN
    ...
ELSE
    ...
ENDIF
```
```
DECLARE Score : INTEGER
Score <- 75

IF Score >= 90 THEN
    OUTPUT "Grade A"
ELSE IF Score >= 80 THEN
    OUTPUT "Grade B"
ELSE IF Score >= 70 THEN
    OUTPUT "Grade C"
ELSE
    OUTPUT "Grade F"
ENDIF
```
- `ELSE IF` and `ELSE` statements are optional
- Any number of `ELSE IF` statements can be used

Case statement:
```
CASE OF <variable>
    <case 1> : ...
    <case 2> : ...
    ...
    <case n> : ...
    OTHERWISE: ...
ENDCASE
```
```
DECLARE DayNumber : INTEGER
DayNumber <- 3

CASE OF DayNumber
    1 : OUTPUT "Monday"
    2 : OUTPUT "Tuesday"
    3 : OUTPUT "Wednesday"
    4 : OUTPUT "Thursday"
    5 : OUTPUT "Friday"
    OTHERWISE: OUTPUT "Weekend"
ENDCASE
```
- `OTHERWISE` is optional
- cases may be expressions, e.g `(var / 2 + 1)`

## Loops
While loop:
```
WHILE <condition> DO
    ...
ENDWHILE
```
```
DECLARE Counter : INTEGER
Counter <- 1
WHILE Counter <= 5 DO
    OUTPUT Counter
    Counter <- Counter + 1
ENDWHILE

// Output:
// 1
// 2
// 3
// 4
// 5
```
Loops until condition is false

Repeat until loop:
```
REPEAT
    ...
UNTIL <condition>
```
```
DECLARE Count : INTEGER
Count <- 1
REPEAT
    OUTPUT Count
    Count <- Count + 1
UNTIL Count > 5
```
- Loops until the condition is true
- Condition is checked at the end of an iteration

For loop:
```
FOR <counterVariable> <- <startValue> TO <stopValue> STEP <stepValue>
    ...
NEXT counterVariable
```
```
FOR I <- 1 TO 5
    OUTPUT I
NEXT I

FOR J <- 10 TO 0 STEP -2
    OUTPUT J
NEXT J
```
- Initialises counterVariable to startValue and loops till it reaches stopValue, incrementing it by stepValue each iteration if provided, otherwise incrementing it by 1
- `STEP <stepValue>` and `counterVariable` after `NEXT` are optional

## Procedures
Procedure with no paramaters:
```
PROCEDURE <name>
    ...
ENDPROCEDURE
```
```
PROCEDURE Greet
    OUTPUT "Hello, World!"
ENDPROCEDURE
```

Procedure with parameters:
```
PROCEDURE <name>([BYREF | BYVAL] <parameterName> : <data type>, <parameter2Name> : <data type>, ...)
    ...
ENDPROCEDURE
```
```
PROCEDURE AddNumbers(BYVAL Num1 : INTEGER, Num2 : INTEGER)
    DECLARE Sum : INTEGER
    Sum <- Num1 + Num2
    OUTPUT Sum
ENDPROCEDURE

PROCEDURE Increment(BYREF Value : INTEGER)
    Value <- Value + 1
ENDPROCEDURE
```
- `BYREF` - pass parameters by reference
- `BYVAL` - pass parameters by value
- If `BYREF` or `BYVAL` is not speified, `BYVAL` will be used as the default
- Specifying `BYREF` or `BYVAL` is optional for subsequent parameters after the first one

### Calling procedures
No parameters:
```
CALL <procedureName>
```
OR
```
CALL <procedureName>()
```
```
CALL Greet
CALL Greet()
```
With parameters:
```
CALL <procedureName>(<parameter1>, <parameter2>, ...)
```
```
DECLARE X, Y : INTEGER
X <- 5
Y <- 10
CALL AddNumbers(X, Y) // Outputs 15

DECLARE MyValue : INTEGER
MyValue <- 100
CALL Increment(MyValue) // MyValue becomes 101
```

## Functions
```
FUNCTION <name>(...) RETURNS <data type>
    ...
ENDFUNCTION
```
```
FUNCTION GetMax(BYVAL A : INTEGER, BYVAL B : INTEGER) RETURNS INTEGER
    IF A > B THEN
        RETURN A
    ELSE
        RETURN B
    ENDIF
ENDFUNCTION

FUNCTION CalculateArea(Radius : REAL) RETURNS REAL
    RETURN 3.14159 * Radius * Radius
ENDFUNCTION
```
- Syntax for function parameters are identical to those of procedures
- Functions must have a `RETURN` statement that returns a value of the specified data type

### Calling functions
```
<functionName>(<parameter1>, <parameter2>, ...)
```
```
DECLARE Result : INTEGER
DECLARE R : REAL
R <- 5.0
Result <- GetMax(10, 20) // Result becomes 20
OUTPUT CalculateArea(R) // Outputs 78.53975
```
Function calls may be used inside expressions since they return a data type

### In-built functions
#### String functions
```
// Returns the length of a string
LENGTH(s : STRING) RETURNS INTEGER

// Returns the left n characters of a string
LEFT(s : STRING, n : INTEGER) RETURNS STRING

// Returns the right n characters of a string
RIGHT(s : STRING, n : INTEGER) RETURNS STRING

// Returns a string of length y starting at x
MID(s : STRING, x : INTEGER, y : INTEGER) RETURNS STRING

// Identical to MID, specified in IGCSE syllabus
SUBSTRING(s : STRING, x : INTEGER, y : INTEGER) RETURNS STRING

// Converts all alphabetical characters into uppercase
// UCASE() also works (IGCSE syllabus)
TO_UPPER(s : STRING) RETURNS STRING

// Converts all alphabetical characters into lowercase
// LCASE() also works (IGCSE syllabus)
TO_LOWER(s : STRING) RETURNS STRING

// Converts a number into a string
NUM_TO_STR(x : REAL) RETURNS STRING

// Converts a string into a number, returning 0 if the number is invalid
STR_TO_NUM(s : STRING) RETURNS REAL

// Returns whether a string is a valid number
IS_NUM(s : STRING) RETURNS BOOLEAN
```

#### Char functions

```
// A Level
// Converts a character into lowercase if it is alphabetic
LCASE(c : CHAR) RETURNS CHAR

// IGCSE
// Same as TO_LOWER
LCASE(c : STRING) RETURNS STRING

// A Level
// Converts a character into uppercase if it is alphabetic
UCASE(c : CHAR) RETURNS CHAR

// IGCSE
// Same as TO_UPPER
UCASE(c : STRING) RETURNS STRING

// Returns the ASCII value of a character
ASC(c : CHAR) RETURNS INTEGER

// Returns the character representation of an ASCII value
CHR(x : INTEGER) RETURNS CHAR
```

#### Date functions
```
// Returns day of month
DAY(Date : DATE) RETURNS INTEGER

// Returns the month
MONTH(Date : DATE) RETURNS INTEGER

// Returns the year
YEAR(Date : DATE) RETURNS INTEGER

// Returns day of week(Starting on Sunday with value 1)
DAYINDEX(Date : DATE) RETURNS INTEGER

// Returns a date with corresponding day, month and year
SETDATE(Day, Month, Year : INTEGER) RETURNS DATE

// Returns current date
TODAY() RETURNS DATE
```

#### Time functions
```
// Returns the unix timestamp(Seconds elapsed since 1st January 1970 UTC)
TIME() RETURNS INTEGER

// Returns the hours on the local clock
HOURS() RETURNS INTEGER

// Returns the minutes on the local clock
MINUTES() RETURNS INTEGER

// Returns the seconds on the local clock
SECONDS() RETURNS INTEGER
```

#### Math functions
```
// Power
POW(x : REAL, y : REAL) RETURNS REAL

// e^x
EXP(x : REAL) RETURNS REAL

// Trigonometric functions
SIN(x : REAL) RETURNS REAL
COS(x : REAL) RETURNS REAL
TAN(x : REAL) RETURNS REAL
ASIN(x : REAL) RETURNS REAL
ACOS(x : REAL) RETURNS REAL
ATAN(x : REAL) RETURNS REAL
ATAN2(y : REAL, x : REAL) RETURNS REAL

// Square root
SQRT(x : REAL) RETURNS REAL

// Logarithm(Base 10)
LOG(x : REAL) RETURNS REAL

// Natural logarithm
LN(x : REAL) RETURNS REAL
```
#### Misc functions
```
// Returns the integer part of a real(floor)
INT(x : REAL) RETURNS INTEGER

// Rounds a real to the specified number of decimal places
// Returns integer if places is 0
ROUND(x : REAL, places : INTEGER) RETURNS REAL

// Returns a random number from 0 to x inclusive
RAND(x : INTEGER) RETURNS REAL

// Returns a random number from 0 to 1
RANDOM() RETURNS REAL

// Checks if end of file is reached
EOF(filename : STRING) RETURNS BOOLEAN
```

## Implicit type casting
Some data types are implicitly converted into the required type during assignment, function calls etc.

- INTEGER to REAL
- CHAR to STRING
- STRING to CHAR (if string length is 1)

```
DECLARE IntVal : INTEGER
DECLARE RealVal : REAL
IntVal <- 10
RealVal <- IntVal // allowed

DECLARE CharVal : CHAR
DECLARE StrVal : STRING
CharVal <- 'A'
StrVal <- CharVal // allowed

StrVal <- "B"
CharVal <- StrVal // allowed since StrVal length is 1

StrVal <- "Hello"
CharVal <- StrVal // NOT allowed since StrVal length is greater than 1
```

## I/O
### Output to screen
```
OUTPUT <value>
// or
PRINT <value>
```
Multiple values can be output at once with
```
OUTPUT <value1>, <value2>, ...
```
```
DECLARE Name : STRING
Name <- "Alice"
OUTPUT "Hello, ", Name, "!"
PRINT 123
```
### Get user input
```
INPUT <variableName>
// or
READ <variableName>
```
Gets user input and stores it in the given variable
```
DECLARE Age : INTEGER
OUTPUT "Please enter your age: "
INPUT Age
OUTPUT "You are ", Age, " years old."
```

### File Handling
#### Text files
Opening files:
```
// Open a file
// Modes are READ, WRITE and APPEND
// WRITE mode creates the file if it doesn't exist
OPENFILE <filename> FOR <mode>
```
```
OPENFILE "data.txt" FOR WRITE
OPENFILE "log.txt" FOR APPEND
OPENFILE "input.txt" FOR READ
```
Reading from files:
```
// Reads one line form the file into the variable(requires READ mode)
READFILE <filename>, <variable>
```
```
DECLARE LineContent : STRING
READFILE "input.txt", LineContent
OUTPUT LineContent
```
Writing to files:
```
// Writes a line with data provided(requires WRITE or APPEND mode)
WRITEFILE <filename>, <data>
```
```
WRITEFILE "data.txt", "First line of data."
WRITEFILE "log.txt", "New log entry."
```
Closing files:
```
// Closes the file
CLOSEFILE <filename>
```
```
CLOSEFILE "data.txt"
```

#### Random files
```
// Creates empty file if it does not exist
OPENFILE <filename> FOR RANDOM
```
```
OPENFILE "records.dat" FOR RANDOM
```
```
// Move file pointer to address
// First address is 1
// Last address is number of records in file + 1 and is write-only to allow appending to the file
SEEK <filename>, <address>
```
```
SEEK "records.dat", 1
```
```
// Read the record at the file pointer into the variable
GETRECORD <filename>, <variable>
```
```
TYPE RecordType
    DECLARE ID : INTEGER
    DECLARE Name : STRING
ENDTYPE

DECLARE MyRecord : RecordType
GETRECORD "records.dat", MyRecord
```
```
// Write the record stored in the variable into the file
PUTRECORD <filename>, <variable>
```
```
MyRecord.ID <- 1
MyRecord.Name <- "Sample"
PUTRECORD "records.dat", MyRecord
```
```
CLOSEFILE <filename>
```
```
CLOSEFILE "records.dat"
```

## Features outside cambridge syntax
- `BREAK` - Break out of loops early
- `CONTINUE` - Skip to next iteration of loop
```
FOR I <- 1 TO 10
    IF I = 5 THEN
        CONTINUE // Skips to the next iteration, so 5 is not output
    ENDIF
    IF I = 8 THEN
        BREAK // Exits the loop entirely
    ENDIF
    OUTPUT I
NEXT I
// Output: 1, 2, 3, 4, 6, 7
```
- `ELSE IF` - Alternative to reduce nesting
- Alternate method of type conversion(apart from in-built functions): `<data type>(<value>)`. For example: `INTEGER("57")`
```
DECLARE StrNum : STRING
DECLARE IntNum : INTEGER
DECLARE RealVal : REAL

StrNum <- "123"
IntNum <- INTEGER(StrNum) // IntNum becomes 123

RealVal <- REAL(IntNum) // RealVal becomes 123.0
```
- Math functions like `SIN`, `EXP`
- Time functions
- Character escape codes like `\n`, `\t` etc.
