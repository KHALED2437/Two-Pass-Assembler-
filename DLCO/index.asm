label: 
    ldc 0      ; Load constant 0
    ldc -5     ; Load constant -5
    ldc +5     ; Load constant +5 
loop: 
    br loop    ; Branch to loop (infinite loop)
br next        ; Branch to next
next: 
    ldc loop   ; Load address/value of loop
    ldc var1   ; Load address/value of var1
var1: 
    data 0     ; Define var1 with initial data value 0
