
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; LEXER PART ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; This part is from HW1. I use lexer for creating tokens. I use these tokens for parsing. Parser Part starts from line 485.

(setq prevChar nil)
(setq counter 0)              ;; counter for each char in the loop
(setq temp1 0)
(setq d 0)
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
(defvar tokens (list))  



(defconstant LETTER  1)
(defconstant DIGIT   2)
(defconstant OTHER 3)
(defconstant OPERATOR 4)
(defconstant UNKNOWN 5)

(defconstant othersList '(" " "\n" "\t" "\0"))
(defconstant operatorList '("'" "+" "-" "/" "*" "(" ")" "**" "\"" ","))
(defconstant keywordList '("defvar" "and" "or" "not" "equal" "less" "nil" "list" "append" "concat" "set" "deffun" "for" "if" "exit" "load" "disp" "true" "false"))

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

(defun checKWord (string1)
  (cond
    ( (equal (isKeyWord string1) t) 
      (setq tokens (append tokens (list(list "KEYWORD" (determineIdentifier (string string1))))))   
    )
  (t  
      (setq tokens (append tokens (list(list "IDENTIFIER" (string string1)))))
    )
  )
)

(defun checkOperator (op)
  (cond
    ( (equal (isOperator op) t) 
      (setq tokens (append tokens (list(list "OPERATOR" (determineOperator (string op))))))
      (setq subString nil)
    )
  )
)

(defun determineIdentifier (string)
  (cond
    ((equal string "defvar")
      (return-from determineIdentifier "KW_DEFVAR")
    )
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
      ((equal string "'")
        (return-from determineOperator "'")
      )
    )

  )

(defun gppLexer (str)

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
                (setq tokens (append tokens (list(list "COMMENT"))))
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
    ;(setq subString nil)
   )
  ((and (equal previousClass LETTER) (equal currentClass DIGIT))  ;; if digit comes before digit
    (setq isNumber nil)
    (setq isIdentifier nil)
    ;(setq subString nil)
    (setq breakLoop t)
    (format t "~d ~%" "SYNTAX ERROR")
   )

  ((and (equal isNumber t) (equal currentClass OTHER))  ;; if there is an whitespace comes right after number
    (setq isNumber nil)

    (setq tokens (append tokens (list(list "VALUE" (string subString)))))
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
    (checKWord subString)
    (setq subString nil)
   )

  ((and (equal isIdentifier t) (equal (string c) ".")) ;; if dot is found and it is not for real number
    (setq isIdentifier nil)
    (setq isNumber nil)
    (setq breakLoop t)
    (checKWord subString)
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
    (checKWord subString)
  )

    (if (equal isNumber t)  ;; if there is an operator comes right after number
    
    (setq tokens (append tokens (list(list "VALUE" (string subString)))))
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
            (setq tokens (append tokens (list(list "OPERATOR" "OP_OC"))))
            (setq subString nil)
        
      )
      (
        (and (equal(string c) "\"")(equal oc t))      ;; handles "\"" situation
            (setq oc nil)
            (setq tokens (append tokens (list(list "OPERATOR" "OP_CC"))))
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
    (setq tokens (append tokens (list(list "VALUE" (string subString)))))
    (setq isNumber nil)
  )
    
  (
    (and (= (- (length str) 1)  counter)(equal isIdentifier t))     ;; checks if last token is a number
   (checKWord subString)
    (setq isIdentifier nil)
  )
)
  
     

 (setq previousClass currentClass)
 (setq prevChar c)
 (setf counter (+ counter 1))

)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; PARSER PART ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(setq tree nil)
(setq currentLayer 0)
(setq counterList (list 0))
(setq tokens nil)
(setq isCorrect t)

(defun interpreter(tokenList)

  (parserMain)
)

(defun parserMain ()

    (cond 
      ((null  tokens)

        )

        ( (equal t (or (equal (string(getFirstTokenName)) "'") (equal (string (getFirstTokenName)) "null"))) 
          (parseEXPLISTI)
          (parserMain)
        )

        ( (equal t (or (equal (string(getSecondTokenName)) "KW_APPEND") (equal (string (getSecondTokenName)) "KW_CONCAT") (equal (string (getSecondTokenName)) "KW_LIST"))) 
          (parseEXPLISTI)
          (parserMain)
        )

        ( (equal t (equal (string (getFirstTokenType)) "COMMENT"))
          (parserMain)
        )

      (t 
        (parseEXPI)
          (parserMain)
        )
    )
)

  ;;;;;;;;;;;;;;;;; EXP PART ;;;;;;;;;;;;;;;;;

(defun parseEXPI()


  (cond 
    ((null tokens) nil)
    ((equal (string (getFirstTokenType)) "IDENTIFIER")
      
      (parseIdentifier)
      
    )

    ((and(equal (string (getSecondTokenType)) "IDENTIFIER") (equal (string (getFirstTokenName)) "OP_OP")) 
      (isOP)
      (parseIdentifier)
      (parseEXPLISTI)
      (isCP)
    )

    ((equal (string (getFirstTokenType)) "VALUE")
      
      (parseValue)
    )

    ((and (equal (string (getSecondTokenType)) "KEYWORD") (not(equal (string (getFirstTokenName)) "KW_FALSE")) (not(equal (string (getFirstTokenName)) "KW_TRUE"))(not(equal (string (getFirstTokenName)) "KW_NIL")))
      
      (parseKeyword)
    )

    ((equal (string (getSecondTokenType)) "OPERATOR") 
      
      (parseOperator)
    )
    (t
      (throwError "in parseEXPI")
    )
  )
)

(defun parseEXPB ()
  
  (cond 
    ((or (equal (string (getFirstTokenName)) "KW_FALSE")(equal (string (getFirstTokenName)) "KW_TRUE")(equal (string (getFirstTokenName)) "KW_NIL"))

      (parseBinaryValue)
    )
    ((equal (string (getFirstTokenType)) "IDENTIFIER")

      (parseIdentifier)
    )
    ((or (equal (string (getSecondTokenName)) "KW_FALSE")(equal (string (getSecondTokenName)) "KW_TRUE")(equal (string (getSecondTokenName)) "KW_NIL"))

      (parseBinaryValue)
    )
    ((equal (string (getSecondTokenType)) "KEYWORD")
  
      (parseKeyword)
    )
    ((equal (string (getSecondTokenType)) "KEYWORD")
    
      (parseKeyword)
    )
    
    (t 
      (throwError "in parseEXPB")
    )
  )
)

(defun parseEXPLISTI ()
  
  (cond 
    ( (equal (string (cadr(nth 0 tokens))) "OP_OP")


      (cond 

        ((equal (string (getSecondTokenName)) "KW_CONCAT") 
          (parseConcat)
        )
        ((equal (string (getSecondTokenName)) "KW_APPEND")  
          (parseAppend)
        )
        ((equal (string (getSecondTokenName)) "KW_LIST")  
          (parseList)
        )
        ((equal (string (getSecondTokenType)) "IDENTIFIER") 
          (parseIdentifier)
        )

      )
    )
    ((equal (string (cadr(nth 0 tokens))) "'")
 
      (parseListValue)
    )
    ;;((equal (string (getFirstTokenName)) "'")
      ;;(print "PARISNG EXPLISTI3")
      ;;(setq tree(insertTree tree 0 currentLayer (list"EXPLISTI")))
      ;;(setq currentLayer (+ currentLayer 1))
      ;;(parseListValue)
    ;;)

    ((equal (string (getFirstTokenType)) "null")

      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
    )

    (t
      (throwError "in parseEXPLISTI")
    )

  )
)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun parseBinaryValue ()
 
  (cond 
    ((or (equal (string (getFirstTokenName)) "KW_FALSE")(equal (string (getFirstTokenName)) "KW_TRUE")(equal (string (getFirstTokenName)) "KW_NIL"))

      (setq tokens (cdr tokens))
    )
    (t
      (throwError "in Binary Value")
    )
  )
)

(defun parseIdentifier ()
  

  (cond 
    ((equal (string (getFirstTokenType)) "IDENTIFIER")      
      (setq tokens (cdr tokens))     
    )
    (t
      (throwError "in parseId")
    )
  )
)

(defun parseIDList ()

  (cond 
    ((equal (string (getFirstTokenName)) "OP_OP")

      
      (isOP)

      (cond
        ((equal (string (getFirstTokenName)) "OP_CP")
          (isCP)  
        )

        (t
          (parseIDList)         
        )
      )

    )

    ((equal (string (getFirstTokenType)) "IDENTIFIER")
     

      (parseIdentifier)
      (cond 
        ((equal (string (getFirstTokenName)) "OP_CP")
          (isCP)
        )

        (t
          (parseIDList)
        )
      )
    )

    (t
      (throwError "in parseIdList")
    )
  )
)

(defun parseValues()
  
  (cond 

    ((equal t (and (equal (string (getSecondTokenType)) "VALUE") (equal (string (getFirstTokenType)) "VALUE")))

      (parseValue)
      (parseValues)
      
    )
    ((equal (string (getFirstTokenType)) "VALUE")

      (parseValue)
    )
  )
)

(defun parseListValue()
  

  (cond 
    ((equal (string (getFirstTokenName)) "'")
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (isOP)
      (cond
        ((not (equal (string (getFirstTokenName)) "OP_CP"))
           (parseValues)
        )
      )
      (isCP)
    )
  )
)

(defun parseValue()
  
  (setq tokens (cdr tokens))
)

  ;;;;;;;;;;;;;;;;; OPERATOR PART ;;;;;;;;;;;;;;;;;

(defun parseOperator ()
  

  (cond
      ((or (string= "OP_PLUS" (getSecondTokenName)) (string= "OP_MINUS" (getSecondTokenName)) (string= "OP_DIV" (getSecondTokenName)) (string= "OP_MULT" (getSecondTokenName))(string= "OP_DBLMULT" (getSecondTokenName))) 
          (isOP)
          (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
          (parseEXPI)
          (parseEXPI)
          (isCP)
        )
      (t
          (throwError "parseOperator")
      )
  )
)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;;;;;;;;;;;;;;;;; KEYWORD PART ;;;;;;;;;;;;;;;;;

(defun parseKeyword ()
  
  (cond
    ((equal (string (getSecondTokenName))  "KW_AND")
    (parseAnd)
    )

    ((equal (string (getSecondTokenName))  "KW_DEFVAR")
    (parseDefvar)
    )

    ((equal (string (getSecondTokenName)) "KW_OR")
    (parseOr)
    )

    ((equal (string (getSecondTokenName)) "KW_NOT")
    (parseNot)
    )

    ((equal (string (getSecondTokenName)) "KW_EQUAL")
    (parseEqual)
    )

    ((equal (string (getSecondTokenName)) "KW_LESS")
    (parseLess)
    )

    ((equal (string (getSecondTokenName)) "KW_SET")
    (parseSet)
    )

    ((equal (string (getSecondTokenName)) "KW_DEFFUN")
    (parseDeffun)
    )

    ((equal (string (getSecondTokenName)) "KW_FOR")
    (parseFor)
    )

    ((equal (string (getSecondTokenName)) "KW_IF")
    (parseIf)
    )

    ((equal (string (getSecondTokenName)) "KW_EXIT")
    (parseExit)
    )

    ((equal (string (getSecondTokenName)) "KW_LOAD")
    (parseLoad)
    )

    ((equal (string (getSecondTokenName)) "KW_DISP")
    (parseDisp)
    )

   

  )
)

(defun parseSet ()


  (cond
    ((or(equal (string (cadr(nth 3 tokens))) "KW_TRUE") (equal (string(cadr(nth 3 tokens))) "KW_FALSE")(equal (string(cadr(nth 3 tokens))) "KW_NIL"))
     
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseBinaryValue)
      (isCP)
    )
    ((or(equal (string (cadr(nth 3 tokens))) "'")(equal (string (cadr(nth 4 tokens))) "KW_APPEND")(equal (string (cadr(nth 4 tokens))) "KW_CONCAT")(equal (string (cadr(nth 4 tokens))) "KW_LIST"))
      
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseEXPLISTI)
      (isCP)
    )
    (t
     
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseEXPI)
      (isCP)
    )
  )
  
)

(defun parseAppend ()
  
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseEXPI)
  (parseEXPLISTI)
  (isCP)
)

(defun parseConcat ()
  
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseEXPLISTI)
  (parseEXPLISTI)
  (isCP)
)

(defun parseAnd ()
  (isOP)
  (setq tokens (cdr tokens))    ;; removes first token from list which is (car tokens)
  (parseEXPB)
  (parseEXPB)
  (isCP)
)

(defun parseOr ()
  (isOP)
  (setq tokens (cdr tokens))    ;; removes first token from list which is (car tokens)
  (parseEXPB)
  (parseEXPB)
  (isCP)
)

(defun parseNot ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseEXPB)
  (isCP)
)

(defun parseEqual ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (cond
    ((equal t (or (equal (string (getFirstTokenName)) "KW_AND") (equal (string(getFirstTokenName)) "KW_FALSE") (equal (string(getFirstTokenName)) "KW_TRUE")(equal (string(getFirstTokenName)) "KW_EQUAL") 
          (equal (string(getFirstTokenName)) "KW_NOT") (equal (string (getFirstTokenName)) "KW_OR") (equal (string (getFirstTokenName)) "KW_NIL")))
                 (parseEXPB) 
           (parseEXPB)
    )
    (t  
      (parseEXPI)
      (parseEXPI)
    )
  )
  (isCP)                        
)

(defun parseFor ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (isOP)
  (parseIdentifier)
  (parseEXPI )
  (parseEXPI)
  (isCP)
  (parseEXPLISTI)
  (isCP)
)

(defun parseWhile ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (isOP)
  (parseIdentifier)
  (parseEXPB)
  (isCP)
  (parseEXPLISTI)
  (isCP)
)

(defun parseIf ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseEXPB)
  (parseEXPLISTI)

  (cond
    ((not(equal (string (getFirstTokenName)) "OP_CP"))
      
      (parseEXPLISTI)
    )
  )
  (isCP)
)

(defun parseDeffun ()
  
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseIdentifier)
  (parseIdList)
  (parseExplisti)
  (isCP)
)

(defun parseLoad ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseIdentifier)
  (isCP)
)

(defun parseLess ()
  (isOP)
  (setq tokens (cdr tokens))    ;; removes first token from list which is (car tokens)
  (parseEXPI)
  (parseEXPI)
  (isCP)
)

(defun parseExit ()
  (isOP)
  (setq tokens (cdr tokens))    ;; removes first token from list which is (car tokens)
  (isCP)
)

(defun parseList ()
  (isOP)
  (setq tokens (cdr tokens))    ;; removes first token from list which is (car tokens)
  (dotimes (x 1000)

    (cond
      ((equal (string (cadr(nth 0 tokens))) "OP_CP")
        (setq x 1001)
      )
      (t (parseValue)
      )
    )
    
  )  (isCP) 
)

(defun parseDefvar ()

  (cond
    ((or(equal (string (cadr(nth 3 tokens))) "KW_TRUE") (equal (string(cadr(nth 3 tokens))) "KW_FALSE")(equal (string(cadr(nth 3 tokens))) "KW_NIL"))
      
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseBinaryValue)
      (isCP)
    )
    ((or(equal (string (cadr(nth 3 tokens))) "'")(equal (string (cadr(nth 4 tokens))) "KW_APPEND")(equal (string (cadr(nth 4 tokens))) "KW_CONCAT")(equal (string (cadr(nth 4 tokens))) "KW_LIST"))
      
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseEXPLISTI)
      (isCP)
    )
    (t
      
      (isOP)
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (parseIdentifier)
      (parseEXPI)
      (isCP)
    )
  )
  
)

(defun parseDisp ()
  (isOP)
  (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
  (parseEXPI)
  (isCP)
)

  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; HELPER FUNCTIONS

(defun isOP ()    ;; checks if token is '('

  (cond 
    ((equal (string (cadar tokens)) "OP_OP")
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
    )
    
    (t
        (throwError "in open Paranthesis"))
    )
)

(defun isCP ()    ;; checks if token is ')'

  (cond 
    ((equal (string (cadar tokens)) "OP_CP")
     
      (setq tokens (cdr tokens))  ;; removes first token from list which is (car tokens)
      (setq currentLayer (- currentLayer 1))
    )
    
    (t
        (throwError "in close Paranthesis"))
    )
)

(defun getFirstTokenType ()
  (caar tokens) 
)

(defun getSecondTokenType ()
  (caadr tokens) 
)

(defun getFirstTokenName ()
  (cadar tokens) 
)

(defun getSecondTokenName ()
  (cadadr tokens) 
)

(defun throwError(str)
  (error (concatenate 'string "SYNTAX ERROR: Unexpected token in " str))
)

(setq variables nil)
(setq functions (list))

(defun evaluateLine (&optional(tokensEval tokenList))
  ( let ((currentToken) (a 0) (b 0) (c 2) (d)(result 0)(e)(len (length tokensEval)))

  (setq currentToken (getTokenName (nth 1 tokensEval)))

    (cond

      ((equal (string currentToken) "KW_DEFFUN")
        (setq functions (append functions (list(getTokenName (nth 2 tokensEval)) ) ))
      )

      ((equal (string (getTokenType (nth 1 tokensEval) ) ) "IDENTIFIER")
        (cond
          ((equal (getFromFunctions (getTokenName (nth 1 tokensEval))) nil)
            (print "SYNTAX ERROR: Functions is not defined")
            (exit)
          )

        )
      )

      ((equal (string currentToken) "KW_DISP")
        (setq e (countParanthesis (subseq tokensEval 2)))
        (setq c 0) (setq c (+ c e))
        (setq result (evaluateLine (subseq tokensEval 2 (- len 1))))
      )

      ((or (equal (string currentToken) "OP_PLUS") (equal (string currentToken) "OP_MINUS") (equal (string currentToken) "OP_MULT") (equal (string currentToken) "OP_DIV")(equal (string currentToken) "OP_DBLMULT"))

        (cond
          ((equal (string (getTokenType (nth 2 tokensEval))) "VALUE")    
            (setq a (parse-integer (getTokenName (nth 2 tokensEval)))) (setq c 3)
          )
          ((equal (string (getTokenType (nth 2 tokensEval))) "IDENTIFIER")
            (setq a (parse-integer(getFromVariables (getTokenName (nth 2 tokensEval))))) (setq c 3)
          )
          (t
            (setq e (countParanthesis (subseq tokensEval 2)))
            (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
            (if (and (not (equal c nil)) (< c len)) (setq a (evaluateLine (subseq tokensEval 2 c))) (setq result nil))         
          )
        )
        (if (equal c nil) (setq result nil) (setq d (+ c 2)))
        (cond
          ((not (equal result nil)) 

            (cond
              ((equaL (string (getTokenType (nth c tokensEval))) "VALUE")
                (setq b (parse-integer (getTokenName (nth c tokensEval))))               
              )
              ((equaL (string (getTokenType (nth c tokensEval))) "IDENTIFIER")
                (setq b (parse-integer(getFromVariables (getTokenName (nth c tokensEval)))))
              )
              (t
                (setq e (countParanthesis (subseq tokensEval c)))
                (if (equal e nil) (progn (setq result nil) (setq d nil)) (setq d (+ c e)))
                (if (and (not (equal d nil)) (< d len)) (progn (setq b (evaluateLine (subseq tokensEval c d))) (setq d (+ d 1))) (setq result nil))
              )

            )

          )
          
        )
              
        (cond
          ((and (not (equal result nil)) (equal d len) (not (equal a nil)) (not (equal b nil)))
            (if (equal (string currentToken) "OP_PLUS") (setq result (+ a b)))
            (if (equal (string currentToken) "OP_MINUS") (setq result (- a b)))
            (if (equal (string currentToken) "OP_MULT") (setq result (* a b)))
            (if (equal (string currentToken) "OP_DBLMULT") (setq result (expt a b))) 
            (if (equal (string currentToken) "OP_DIV") (setq result (/ a b)))            
          )
          (t
            (setq result nil)
          )
        )
      )

      ((equal (string currentToken) "KW_SET")
        (cond
          ((equal (getTokenType (nth 3 tokensEval)) "VALUE")
            (setq c (list (list (getTokenName (nth 2 tokensEval)) (getTokenName (nth 3 tokensEval)))))
            (setq variables (append variables c))
            (setq result (parse-integer(getTokenName (nth 3 tokensEval))))
          )
          ((equal (getTokenType (nth 3 tokensEval)) "IDENTIFIER")

            (cond
              ((or(equal (getFromVariables (getTokenName (nth 3 tokensEval))) t)(equal (getFromVariables (getTokenName (nth 3 tokensEval))) -3.1))
                (print "ZXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx")
                (setq c (list (list (getTokenName (nth 2 tokensEval)) (getFromVariables (getTokenName (nth 3 tokensEval))))))
                (setq variables (append variables c))
                (setq result (getFromVariables (getTokenName (nth 3 tokensEval))))
              )

              ((not (equal (getFromVariables (getTokenName (nth 3 tokensEval))) nil))
                (setq c (list (list (getTokenName (nth 2 tokensEval)) (getFromVariables (getTokenName (nth 3 tokensEval))))))
                (setq variables (append variables c))
                (setq result (parse-integer(getFromVariables (getTokenName (nth 3 tokensEval)))))
              )
              (t
                (print "ERROR")
                (exit)
              )
            )           
          )

          ((equal (getTokenName (nth 3 tokensEval)) "KW_TRUE")
            (setq c (list (list (getTokenName (nth 2 tokensEval)) t)))
            (setq variables (append variables c))
            (setq result t)
          )
          ((equal (getTokenName (nth 3 tokensEval)) "KW_FALSE")
            ;(setq c (list (list (getTokenName (nth 2 tokensEval))  nil)))
            (setq c (list (list (getTokenName (nth 2 tokensEval)) -3.1)))
            (setq variables (append variables c))
            (setq result nil)
          )
          ((equal (getTokenName (nth 3 tokensEval)) "KW_NIL")
            ;(setq c (list (list (getTokenName (nth 2 tokensEval))  nil)))
            (setq c (list (list (getTokenName (nth 2 tokensEval)) -3.1)))
            (setq variables (append variables c))
            (setq result nil)
          )
          ((equal (getTokenName (nth 3 tokensEval)) "OP_OP")
            (setq a (evaluateLine (subseq tokensEval 3 (- len 1))))
            (setq c (list (getTokenName (nth 2 tokensEval)) a))
            (setq variables (append variables c))
            (setq result (list (getTokenName (nth 2 tokensEval)) a))
          )
           ((equal (getTokenName (nth 3 tokensEval)) "'")
            (setq a (evaluateLine (subseq tokensEval 3 (- len 1))))
            (setq c  (list (getTokenName (nth 2 tokensEval)) a))
            (setq variables (append variables c))
            (setq result (list (getTokenName (nth 2 tokensEval)) a))
          )
        )
      )

      ((equal (string currentToken) "KW_DEFVAR")
        (cond
          ((equal (getTokenType (nth 3 tokensEval)) "VALUE")
            (setq c (list (list (getTokenName (nth 2 tokensEval)) (getTokenName (nth 3 tokensEval)))))
            (setq variables (append variables c))
          )
          ((equal (getTokenType (nth 3 tokensEval)) "IDENTIFIER")

            (cond
              ((not (equal (getFromVariables (getTokenName (nth 3 tokensEval))) nil))
                (setq c (list (list (getTokenName (nth 2 tokensEval)) (getFromVariables (getTokenName (nth 3 tokensEval))))))
                (setq variables (append variables c))
              )
              (t
                (print "ERROR")
              )
            )           
          )

          ((equal (getTokenName (nth 3 tokensEval)) "KW_TRUE")
            (setq c (list (list (getTokenName (nth 2 tokensEval)) t)))
            (setq variables (append variables c))
          )
          ((equal (getTokenName (nth 3 tokensEval)) "KW_FALSE")
            ;;(setq c (list (list (getTokenName (nth 2 tokensEval))  nil)))
            (setq c (list (list (getTokenName (nth 2 tokensEval)) -3.1)))
            (setq variables (append variables c))
          )
          ((equal (getTokenName (nth 3 tokensEval)) "KW_NIL")
            ;;(setq c (list (list (getTokenName (nth 2 tokensEval))  nil)))
            (setq c (list (list (getTokenName (nth 2 tokensEval)) -3.1)))
            (setq variables (append variables c))
          )
          ((equal (getTokenName (nth 3 tokensEval)) "OP_OP")
            (setq a (evaluateLine (subseq tokensEval 3 (- len 1))))
            (setq c (list (getTokenName (nth 2 tokensEval)) a))
            (setq variables (append variables c))
            (setq result (list (getTokenName (nth 2 tokensEval)) a))
          )
           ((equal (getTokenName (nth 3 tokensEval)) "'")
            (setq a (evaluateLine (subseq tokensEval 3 (- len 1))))
            (setq c  (list (getTokenName (nth 2 tokensEval)) a))
            (setq variables (append variables c))
            (setq result (list (getTokenName (nth 2 tokensEval)) a))
          )
        )
      )

      ((or (equal (string currentToken) "KW_AND") (equal (string currentToken) "KW_OR") (equal (string currentToken) "KW_EQUAL") (equal (string currentToken) "KW_LESS"))
        (cond
          ((equal (string (getTokenName (nth 2 tokensEval))) "KW_TRUE") 
            (setq a t) (setq c 3)
          )
          ((equal (string (getTokenName (nth 2 tokensEval))) "KW_FALSE") 
            (setq a nil) (setq c 3)
          )
          ((equal (string (getTokenName (nth 2 tokensEval))) "KW_NIL") 
            (setq a nil) (setq c 3)
          )
          ((equal (string (getTokenType (nth 2 tokensEval))) "IDENTIFIER") 
            (setq a (getFromVariables (getTokenName (nth 2 tokensEval)))) 

            (cond
              ((equal a -3.1)

                (setq a nil)
              )
              ((equal a t)

                (setq a t)
              )
              (t
                (print "Variable has no value")
                (exit) 
              )
            )

            (setq c 3)         
          )
          ((equal (string (getTokenType (nth 2 tokensEval))) "VALUE") 
            (setq a (parse-integer (getTokenName (nth 2 tokensEval)))) (setq c 3)         
          )
          (t
            (setq e (countParanthesis (subseq tokensEval 2)))
            (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
            (if (and (not (equal c nil)) (< c len)) (progn (setq a (evaluateLine (subseq tokensEval 2 c))) (setq d (+ c 2))) (setq result nil))
          )
        )

        (cond
          ((equal(string (getTokenName (nth c tokensEval))) "KW_TRUE") 
            (setq b t) 
          )
          ((equal(string (getTokenName (nth c tokensEval))) "KW_FALSE") 
            (setq b nil) 
          )
          ((equal(string (getTokenName (nth c tokensEval))) "KW_NIL") 
            (setq b nil) 
          )
          ((equal(string (getTokenType (nth c tokensEval))) "IDENTIFIER")
            (setq b (getFromVariables (getTokenName (nth c tokensEval))))

            (cond
              ((equal b -3.1)
                (setq b nil)
              )
              ((equal b t)
                (setq b t)
              )
              (t
                (print "Variable has no value")
                (exit) 
              )
            )               
          )
          ((equal (string (getTokenType (nth c tokensEval))) "VALUE") 
            (setq b (parse-integer (getTokenName (nth c tokensEval)))) (setq c 3)         
          ) 
          (t 
            (setq e (countParanthesis (subseq tokensEval c)))
            (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
            (if (and (not (equal c nil)) (< c len)) (progn (setq b (evaluateLine (subseq tokensEval c d)))) (setq result nil))
          )
        ) 

        (cond
          ((and (not (equal result nil)))                       
                (if (equal (string currentToken) "KW_AND") (setq result (and a b)))
                (if (equal (string currentToken) "KW_OR") (setq result (or a b)))
                (if (equal (string currentToken) "KW_EQUAL") (setq result (equal a b)))
                (if (equal (string currentToken) "KW_LESS") (setq result (< a b)))             
          )
          (t
           (setq result nil)
          )
        )
      )

      ((equal (string currentToken) "KW_NOT")
        (cond 
          ((or (equal (getTokenName (nth 2 tokensEval)) "KW_TRUE") (equal (getTokenName (nth 2 tokensEval)) "KW_NIL") (equal (getTokenName (nth 2 tokensEval)) "KW_FALSE") (equal (getTokenType (nth 2 tokensEval)) "IDENTIFIER"))

            (cond
              ((equal (string (getTokenName (nth 2 tokensEval))) "KW_TRUE") 
                (setq a t) (setq c 3)
              )
              ((equal (string (getTokenName (nth 2 tokensEval))) "KW_FALSE") 
                (setq a nil) (setq c 3)
              )
              ((equal (string (getTokenName (nth 2 tokensEval))) "KW_NIL") 
                (setq a nil) (setq c 3)
              )
              ((equal (string (getTokenType (nth 2 tokensEval))) "IDENTIFIER") 
                (setq a (getFromVariables (getTokenName (nth 2 tokensEval)))) (setq c 3)         
              )
              
          )
        )
        (t
          (setq e (countParanthesis (subseq tokensEval 2)))
          (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
          (if (and (not (equal c nil)) (< c len)) (progn (setq a (evaluateLine (subseq tokensEval 2 c))) (setq d (+ c 2))) (setq result nil))
        )                     
      )

        (cond
              ( (not (equal result nil)) 
                (setq result (not a))
              )
              (t
                (setq result nil)
              )
        )
    )

      ((and (equal (string currentToken) "KW_CONCAT") (or (equal (string (getTokenName (nth 2 tokensEval))) "'")  (equal (string (getTokenName (nth 3 tokensEval))) "KW_LIST")))
        (setq e (countParanthesis (subseq tokensEval 2)))

        (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
            (cond ((equal c nil) (setq result nil) (setq d (+ c 2))))
            (cond ((and (not (equal c nil)) (< c len)) (setq a (evaluateLine (subseq tokensEval 2 c))) (setq result nil)))
            (if (and (not (equal c nil)) (< c len))
              (progn
                (setq e (countParanthesis (subseq tokensEval c)))
                (if (equal e nil) (progn (setq result nil) (setq d nil)) (setq d (+ c e)))
              )
              (setq d nil)
            )
            (if (and (not (equal d nil)) (< d len)) (progn (setq b (evaluateLine (subseq tokensEval c d))) (setq d (+ d 1))) (setq result nil))
            (if (and  (= d len) (not (equal a nil)) (not (equal b nil)))
              (progn
                (setq result (list))
                (setq result (append result a))
                (setq result (append result b))
              )
              (setq result nil)
            )
      )

      (  (equal (string currentToken) "KW_APPEND")
        (cond
          ((equal (string (getTokenType (nth 2 tokensEval))) "VALUE")
            (setq a (list(parse-integer (getTokenName (nth 2 tokensEval))))) (setq c 3)
          )
          ((string= (getTokenType (nth 2 tokensEval)) "IDENTIFIER")
            (setq a (list (parse-integer(getFromVariables (getTokenName (nth 2 tokensEval)))))) (setq c 3)
          )
          (t
            (setq e (countParanthesis (subseq tokensEval 2)))
            (if (equal e nil) (progn (setq result nil) (setq c nil)) (setq c (+ c e)))
            (if (and (not (equal c nil)) (< c len)) (setq a (evaluateLine (subseq tokensEval 2 c))) (setq result nil))
          )
        )

        (if (equal c nil) (setq result nil) (setq d (+ c 2)))

        (cond
          ((and (not (equal c nil)) (< c len))
            (setq e (countParanthesis (subseq tokensEval c)))
            (if (equal e nil) (progn (setq result nil) (setq d nil)) (setq d (+ c e)))
          )
          (t
            (setq d nil)
          )
        )

        (if (and (not (equal d nil)) (< d len)) (progn (setq b (evaluateLine (subseq tokensEval c d) )) (setq d (+ d 1))) (setq result nil))

        (if (and (not (equal result nil)) (= d len) (not (equal a nil)) (not (equal b nil)))
              (progn
                (setq result nil)
                (setq result (append result a))
                (setq result (append result b))
              )
              (setq result nil)
        )
      )

      ((equal (string (getTokenName (nth 1 tokensEval))) "KW_LIST")  
        (setq a (scanList (subseq tokensEval 1)))
        (setq result a)
      )

      ((string= (getTokenName (nth 0 tokensEval)) "'") 
        (setq a (scanList2 (subseq tokensEval 1)))
        (setq result a)                 
      )
          
    )
    (return-from evaluateLine result)
  )
)

(defun getTokenType (tokens)
  (car tokens) 
)

(defun getTokenName (tokens)
  (cadr tokens) 
)

(defun scanList (tokensEval)
  (let ((len (list-length tokensEval)) (tempList (list))(x)(currentToken) (result 1) )
    (if (> len 2)
      (progn
        (loop for i from 1 to (- len 2)
         do(progn
            (setq currentToken (getTokenType (nth i tokensEval)))
            (if (string= currentToken "VALUE")
              (setq x (list (parse-integer (getTokenName (nth i tokensEval)))))
            )
            (if (not (equal result nil)) (setq tempList (append tempList x)))
          )
        )
        (if (not (equal result nil)) (setq result tempList))
      )
      (setq result nil)
    )
    result
  )
)

(defun scanList2 (tokensEval)
  (let ((len (list-length tokensEval)) (tempList (list))(x)(currentToken) (result 1) )
    (if (> len 2)
      (progn
        (loop for i from 1 to (- len 2)
         do(progn
            (setq currentToken (getTokenType (nth i tokensEval)))
            (if (string= currentToken "VALUE")
              (setq x (list (parse-integer (getTokenName (nth i tokensEval)))))
            )
            (if (not (equal result nil)) (setq tempList (append tempList x)))
          )
        )
        (if (not (equal result nil)) (setq result tempList))
      )
      (setq result nil)
    )
    result

  )
)

(defun countParanthesis (x)
  (let ((counter 0) (str) (j 0) (result nil)) (setq apFound nil)   
    (if (or (string= (getTokenName (nth 0 x)) "OP_OP") (string= (getTokenName (nth 0 x)) "'") )
      (progn
        (loop for i in x
          do (progn
            (setq str (getTokenName i))
            (cond
              ((equal t apFound)
                (setq apFound nil)
              )
              ((string= str "OP_OP")
                (setq counter (+ counter 1))
              )
              ((string= str "OP_CP")
                (setq counter (- counter 1))
              )
              ((string= str "'")
                (setq counter (+ counter 1))
                (setq apFound t)
              )
            )

            (setq j (+ j 1))
            (if (= counter 0) (return j))
          )
        )
        (setq result j)
      )
    )
    result
  )
)

(defvar i)

(defun getFromVariables (id)

    (setq i 0)
    
    (dotimes (h (length variables))

      (cond
        ((equal (getTokenType (nth (-(- (length variables) 1) i) variables)) id)
          

          (return-from getFromVariables (getTokenName(nth (-(- (length variables) 1) i) variables)))
        )
      )
      (setq i (+ i 1))
    )
  
  (return-from getFromVariables nil)

)

(defun getFromFunctions (id)

    (setq i 0)

    
    (dotimes (h (length functions))
      (cond
        ((equal (string(nth i functions)) id)
        
          (return-from getFromFunctions t)
        )
      )
      (setq i (+ i 1))
    )
  
  (return-from getFromFunctions nil)

)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



(defun gppinterpreter (&optional input)   ;; This function calls subFunc for the input or no input

 (cond
  ( (equal input nil)

  (format t "~d ~%" "FIRST PART (INPUT COMES FROM USER ENTRIES ON TERMINAL)" "~d ~%")

  (dotimes (x 1000)                   
    (setq strLine nil)
    (format t "~%~d ~%" "TYPE quit to QUIT " "~d ~%")
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
    (gppLexer strLine)
    (setf tokenList tokens)

    (setq isCorrect t)
    (interpreter tokens)

    (setq tt (evaluateLine))

    (format T "SYNTAX: OK  ")
    (cond
      ((not(equal tt 0)) (format T "Result: ") (print tt)
      )
    )

    (setq tokens nil)
  )
  (t (setq x 1001) 
    )
)
  
)

)

(t
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
    (setq tokens nil)

(format t "~d ~%" "SECOND PART (INPUT COMES FROM FILE)" "~d ~%")

  (let ((x (open input)))
    (when x
      (loop for line = (read-line x nil)
            while line
            do(gppLexer line)
            do  (setf tokenList tokens)
            do  (setq isCorrect t)
            do  (interpreter tokens)
            do  (setq tt (evaluateLine))

            do  (format T "SYNTAX: OK  ") (format T line) (terpri)
            do  (cond
                ((not(equal tt 0)) (format T "Result: ") (print tt) (terpri)
                )
                )

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




