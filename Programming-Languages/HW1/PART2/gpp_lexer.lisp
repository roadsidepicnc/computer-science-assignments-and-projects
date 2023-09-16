
(setq prevChar nil)
(setq counter 0)              ;; counter for each char in the loop
(setq temp1 0)
(setq temp2 0)
(setq temp3 nil)
(setq breakLoop nil)          ;; if syntax error is detected
(defvar subString nil)
(setq strLine nil)
(defvar previousClass nil)
(setq isIdentifier nil)         ;; for checking if current token is a letter
(defvar isNumber nil)           ;; for checking if current token is a number
(defvar done nil)
(defvar doubleStarFound nil)    ;; for checking "**" 
(defvar dotFound nil)
(defvar oc nil)                 ;; for checking '"' character
(defvar currentClass)           ;; for keeping current char's class

(defconstant LETTER  1)
(defconstant DIGIT   2)
(defconstant OTHER 3)
(defconstant OPERATOR 4)
(defconstant UNKNOWN 5)

(defconstant othersList '(" " "\n" "\t" "\0"))
(defconstant operatorList '("+" "-" "/" "*" "(" ")" "**" "\"" ","))
(defconstant keywordList '("and" "or" "not" "equal" "less" "nil" "list" "append" "concat" "set" "deffun" "for" "if" "exit" "load" "disp" "true" "false"))

(defun isOperator(charRead)             ;; checks if currently read char is an operator class
  (loop for key in operatorList do
    (cond 
      ((equal  (string charRead) key)
        (return-from isOperator t)
      )
    )     
  )
  (return-from isOperator nil)
)

(defun isOther(charRead)              ;; checks if currently read char is an other class
  (loop for key in othersList do
    (cond 
      ((equal  (string charRead) key)
        (return-from isOther t)
      )
    )     
  )
  (return-from isOther nil)
)

(defun isKeyWord(charRead)          ;; checks if current char is a keyword
  (loop for key in keywordList do
    (cond 
      ((equal  (string charRead) key)
        (return-from isKeyWord t)
      )
    )     
  )
  (return-from isKeyWord nil)
)


(defun detectCharClass(c)      ;; determines current char's class
  (cond 

    ;Check, character c is [a-z, A-Z]
    ((alpha-char-p c)
      (setq currentClass LETTER)
    )
    ;Check, character c is [0-9]
    ((not (null (digit-char-p c)))
      (setq currentClass DIGIT)
    )
    ((equal (isOperator c) t)
      (setq currentClass OPERATOR)
    )
    ((equal (isOther c) t)
      (setq currentClass OTHER)
    )
    ;Else OTHER.
    (t
      (setq currentClass UNKNOWN)
    )
  )
)

(defun checkWord (string1)
  (cond
    ( (equal (isKeyWord string1) t) 
      (format t "~d ~%" (determineIdentifier string1))    
    )
  (t
    
      (format t "~d ~%" "IDENTIFIER")
    )
  )
)

(defun checkOperator (op)
  (cond
    ( (equal (isOperator op) t) 
      (format t "~d ~%" (determineOperator (string op))) 
        
    )
  )
)

(defun determineIdentifier (string)
  (cond
    ((equal string "and")
      (return-from determineIdentifier "KW_AND")
    )
    ((equal string "or")
      (return-from determineIdentifier "KW_OR")
    )
    ((equal string "not")
      (return-from determineIdentifier "KW_NOT")
    )
    ((equal string "equal")
      (return-from determineIdentifier "KW_EQUAL")
    )
    ((equal string "less")
      (return-from determineIdentifier "KW_LESS")
    )
    ((equal string "nil")
      (return-from determineIdentifier "KW_NIL")
    )
    ((equal string "list")
      (return-from determineIdentifier "KW_LIST")
    )
    ((equal string "append")
      (return-from determineIdentifier "KW_APPEND")
    )
    ((equal string "concat")
      (return-from determineIdentifier "KW_CONCAT")
    )
    ((equal string "set")
      (return-from determineIdentifier "KW_SET")
    )
    ((equal string "deffun")
      (return-from determineIdentifier "KW_DEFFUN")
    )
    ((equal string "for")
      (return-from determineIdentifier "KW_FOR")
    )
    ((equal string "if")
      (return-from determineIdentifier "KW_IF")
    )
    ((equal string "exit")
      (return-from determineIdentifier "KW_EXIT")
    )
    ((equal string "load")
      (return-from determineIdentifier "KW_LOAD")
    )
    ((equal string "disp")
      (return-from determineIdentifier "KW_DISP")
    )
    ((equal string "true")
      (return-from determineIdentifier "KW_TRUE")
    )
    ((equal string "false")
      (return-from determineIdentifier "KW_FALSE")
    )

    )
  )

  (defun determineOperator (string)
    (cond
      ((equal string "+")
        (return-from determineOperator "OP_PLUS")
      )
      ((equal string "-")
        (return-from determineOperator "OP_MINUS")
      )
      ((equal string "/")
        (return-from determineOperator "OP_DIV")
      )
      ((equal string "*")
        (return-from determineOperator "OP_MULT")
      )
      ((equal string "(")
        (return-from determineOperator "OP_OP")
      )
      ((equal string ")")
        (return-from determineOperator "OP_CP")
      )
      ((equal string "**")
        (return-from determineOperator "OP_DBLMULT")
      )
      ((equal string "\"")
        (return-from determineOperator "OP_OC")
      )
      ((equal string "\"")
        (return-from determineOperator "OP_CC")
      )
      ((equal string ",")
        (return-from determineOperator "OP_COMMA")
      )
    )

  )

(defun subFunc (str)



(loop for c across str do

 (detectCharClass c)

(cond
((equal breakLoop nil)
(cond 

  ;; It checks current character and previously read character also. Then, it checks all of the possibilities.

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  ((and (equal (string c) ";"))  ;; checks for comment
    (setq isNumber nil)
    (setq isIdentifier nil)

    (cond
          ( (< (+ counter 1) (length str))
              (setq temp3 (elt str (+ 1 counter)))
            (cond
              ((equal (string temp3) ";")
                (format t "~d ~%" "COMMENT")
                (setq breakLoop t)
              )
              (t
                (format t "~d ~%" "SYNTAX ERROR")
                (setq breakLoop t)
              )
              )
          )
         
          (t
                (format t "~d ~%" "SYNTAX ERROR")
                (setq breakLoop t)
           )
         )
  )

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;; DIGIT PART

  ((and (equal previousClass nil) (equal currentClass DIGIT))   ;; if nothing comes before number
    (setq isNumber t)
    (setq isIdentifier nil)
  )
  ((and (equal previousClass OTHER) (equal currentClass DIGIT))   ;; if whitespace comes before number
    (setq isNumber t)
    (setq isIdentifier nil)
    (setq subString nil)

   )
  ((and (equal previousClass OPERATOR) (equal currentClass DIGIT))  ;; if operator comes before number
    (setq isNumber t)
    (setq isIdentifier nil)
    (setq subString nil)
   )
  ((and (equal previousClass DIGIT) (equal currentClass DIGIT))  ;; if digit comes before digit
    (setq isNumber t)
    (setq isIdentifier nil)
    (setq subString nil)
   )
  ((and (equal previousClass LETTER) (equal currentClass DIGIT))  ;; if digit comes before digit
    (setq isNumber nil)
    (setq isIdentifier nil)
    (setq subString nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
   )

  ((and (equal isNumber t) (equal currentClass OTHER))  ;; if there is an whitespace comes right after number
    (setq isNumber nil)
    (format t "~d ~%" "VALUE")
    (setq subString nil)
   )
  ((and (equal isNumber t) (equal currentClass LETTER))  ;; if a letter comes right after a number
    (setq isNumber nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
   )
  ((and (and(equal isNumber t)(equal (string c) "."))(equal (+ 1 counter)(length str)))  ;; if first char of the word is digit
    (setq isNumber nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
  )
  ((and (and (equal isNumber t)(equal dotFound nil)) (equal (string c) "."))    ;; handles the real number situation
    (setq isNumber t)
    (setq dotFound t)
    (setq isIdentifier nil)
  )
  ((and (equal dotFound t) (equal (string c) ".")) ;; handles the real number situation
    (setq isNumber nil)
    (setq dotFound nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
  )

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


  ;; LETTER PART

 
  ((and (equal previousClass nil) (equal currentClass LETTER))  ;; if nothing comes before letter
    (setq isIdentifier t)
    (setq isNumber nil)

   )
  ((and (equal previousClass OTHER) (equal currentClass LETTER))  ;; if whitespace comes before letter
    (setq isIdentifier t)
    (setq isNumber nil)
    (setq subString nil)

   )
  ((and (equal previousClass OPERATOR) (equal currentClass LETTER))  ;; if operator comes before letter
    (setq subString nil)
    (setq isIdentifier t)
    (setq isNumber nil)
   )
  ((and (equal isIdentifier t) (equal currentClass OTHER))  ;; if there is an whitespace comes right after letter
    (setq isIdentifier nil)
    (checkWord subString)
    (setq subString nil)
   )

  ((and (equal isIdentifier t) (equal (string c) ".")) ;; if dot is found and it is not for real number
    (setq isIdentifier nil)
    (setq isNumber nil)
    (setq breakLoop t)
    (checkWord subString)
    (format t "~d ~%" "SYNTAX ERROR")
   )

  ((and(equal currentClass UNKNOWN)(not(equal (string c) "\0"))) ;; if first char of the word is letter
    (setq isIdentifier nil)
    (setq isNumber nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
   )
  
   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

( (equal currentClass OPERATOR)     ;; checks if current read char is an operator

  (if (equal isIdentifier t)   
    (checkWord subString)
  )

    (if (equal isNumber t)  ;; if there is an operator comes right after number
    
    (format t "~d ~%" "VALUE")
    (setq subString nil)
   )

    (if(equal isNumber t) (setq isNumber nil)
      )
    (if(equal isIdentifier t) (setq isIdentifier nil)
      )
    
    (cond

      ((equal (string c) "-")

        (cond
          ( (< (+ counter 1) (length str))
              (setq temp3 nil)
              (setq temp3 (elt str (+ 1 counter)))
            (cond
              ((not(null (digit-char-p temp3)))
              (setq done t)
              )
              (t
                (checkOperator "-")
                (setq subString nil)
              )
            )
          )
          (t
                (checkOperator "-")
                (setq subString nil)
              )
        )
      )
      (
        (and (equal(string c) "\"")(equal oc nil))    ;; handles "\"" situation
            (setq oc t)
            (format t "~d ~%" "OP_OC")
            (setq subString nil)
        
      )
      (
        (and (equal(string c) "\"")(equal oc t))      ;; handles "\"" situation
            (setq oc nil)
            (format t "~d ~%" "OP_CC")
            (setq subString nil)
        
      )
      ((equal (string c) "*")     ;; handles "*" and "**" situations

        (cond
          ( (equal doubleStarFound t) (setq doubleStarFound nil)
          )
          ( (< (+ counter 1) (length str))
              (setq temp3 (elt str (+ 1 counter)))
            (cond
              
              ((and(equal (string c) "*")(equal (string temp3) "*"))
                (setq doubleStarFound t)
                (checkOperator "**")
                (setq subString nil)
                )
               (t
                (checkOperator "*")
                (setq subString nil)
              )
              )
    
            )
          (t
                (checkOperator "*")
                (setq subString nil)
          )
          )
        )
      
      

      
      (t 
        (checkOperator c)
        (setq subString nil)
        )         
    )
   
)

   )
)

 )

  (setq subString (concatenate 'string subString (string c)))   

  (cond
  (
    (and (= (- (length str) 1)  counter)(equal isNumber t))     ;; checks if last token is an identifier or keyword
    (format t "~d ~%" "VALUE")
    (setq isNumber nil)
  )
    
  (
    (and (= (- (length str) 1)  counter)(equal isIdentifier t))     ;; checks if last token is a number
   (checkWord subString)
    (setq isIdentifier nil)
  )
)
  
     

 (setq previousClass currentClass)
 (setq prevChar c)
 (setf counter (+ counter 1))

)
)




(defun gppinterpreter (&optional input)   ;; This function calls subFunc for the input or no input

 (cond
  ( (equal input nil)

  (format t "~d ~%" "FIRST PART (INPUT COMES FROM USER ENTRIES ON TERMINAL)" "~d ~%")

  (dotimes (x 1000)                   
    (setq strLine nil)

    (format t "~d ~%" "TYPE quit to QUIT " "~d ~%")
    (setq strLine (read-line))       ;; reads input as a linefrom console
(cond 
  ((not(equal (string strLine) "quit"))   ;; You need to write "exit" in order to quit the program
    (setq breakLoop nil)
    (setq subString nil)
    (setq isNumber nil)
    (setq isIdentifier nil)
    (setq temp3 nil)
    (setq prevChar nil)
    (setq previousClass nil)
    (setq currentClass nil)
    (setq doubleStarFound nil)
    (setq dotFound nil)
    (setq done nil)
    (setq counter 0)
    (subFunc strLine)

  )
  (t (setq x 1001) 
    )
)
  
)

)

(t
(format t "~d ~%" "SECOND PART (INPUT COMES FROM FILE)" "~d ~%")

  (let ((x (open input)))
    (when x
      (loop for line = (read-line x nil)
            while line
            do (subFunc line)

    do(setq breakLoop nil)
    do(setq subString nil)
    do(setq isNumber nil)
    do(setq isIdentifier nil)
    do(setq temp3 nil)
    do(setq prevChar nil)
    do(setq previousClass nil)
    do(setq currentClass nil)
    do(setq doubleStarFound nil)
    do(setq dotFound nil)
    do(setq done nil)
    do(setq counter 0)

            ) 
    (close x))) 
)
  )
 )


(gppinterpreter)    ;; first with 0 inputs

(format t "~d ~%" "TYPE fileName: ")
(setq strLine nil)
(setq strLine (read-line))       ;; reads input as a linefrom console 
(gppinterpreter strLine)         ;; secondly with file name input


