#!/usr/bin/gbf
# a = m(0) b = m(1) flag bsmall eq tmp
#
#

 +++++++++
>+++++++++<

[
   >>[-]+<< # flag = 1
   >[->>>+>+<<<<]< # eq = b  tmp = b  b = 0
   >>>>>[-<<<<+>>>>]<<<<< # b restored tmp = 0
   
   >>>>[[-]<<->>]<<<<  # while (eq) 

   - # a minus 1

   >>[
      <<[-]
      >+
      >>[-]+
      <[-]
   ]<<

   >-< # b minus 1
]

# a = m(0) b = m(1) flag bsmall eq tmp
>>>[->+>+<<]<<< # eq = bsmall tmp = bsmall bsmall = 0
>>>>>[-<<+>>]<<<<< # restore bsmall set tmp = 0
>[->>>+@<<<]<  # eq = bsmall+b  b = 0
>>[-]++<<  # flag = 2
>>>>[[-]<<[-]>>]<<<<
>>[->+<]<<


