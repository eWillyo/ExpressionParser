# Expression parser


 Simple math solver with variables, goniometric functions, logarithms, etc. Original idea dates back to 2010. I significantly inspired myself on "codeproject.com", when I was creating this easy to implement expression parser. After years I came back to project and finally finished it. 


 NEW: Fixed few bugs and added support for basic vector operations.
 
 TODO: Add support for matrices.
 
 # Examples
 
          Enter expression: sin(RAD(90))
          Result: 1.000
          
          Enter expression: a = 6.66
          Result: 0.000
          Enter expression: 3 + (a - 7)^4
          Result: 3.013
          
          Enter expression: log(1000)
          Result: 3.000
          
          Enter expression: v1 = vec3(1,0,0)
          Result: 0.000
          Enter expression: v2 = vec3(0,1,0)
          Result: 0.000
          Enter expression: DEG(acos(dot(v1,v2)))
          Result: 90.000
          

NOTE: To install on Linux just copy create_solver.sh file and run this commands:
         chmod +x create_solver.sh
         sudo ./create_solver.sh


  [![Expression input](https://github.com/eWillyo/ExpressionParser/blob/a966ae5f640ba9ece3d49580170910264f50ac47/expression_input.png?raw=true)](https://www.youtube.com/watch?v=lFQNcjrtSyo)
